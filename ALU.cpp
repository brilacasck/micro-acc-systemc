/*
 * @ASCK
 */

#include <systemc.h>

SC_MODULE (ALU) {
 
    sc_in <sc_int<8>> in1;   // A
    sc_in <sc_int<8>> in2;   // B
    sc_in <bool> c;   // Carry Out  // in this project, this signal is always 1

   // ALUOP // has 5 bits by merging: opselect (4bits) and first LSB bit of opcode (1bit) 
    sc_in <sc_uint<5>> aluop;
    sc_in <sc_uint<3>> sa;       // Shift Amount

    sc_out <sc_int<8>> out;  // Output

    /*
    ** module global variables 
    */

    //

    SC_CTOR (ALU){
        SC_METHOD (process);
        sensitive << in1 << in2 << aluop;
    }
    
    void process () {

        sc_int<8> a = in1.read();
        sc_int<8> b = in2.read();
        bool cin = c.read();
        sc_uint<5> op = aluop.read();
        sc_uint<3> sh = sa.read();

        switch (op){
            case 0b00000 :
                out.write(a);
                break;
            case 0b00001 :
                out.write(++a);
                break;
            case 0b00010 :
                out.write(a+b);
                break;
            case 0b00011 :
                out.write(a+b+cin);
                break;
            case 0b00100 :
                out.write(a-b);
                break;
            case 0b00101 :
                out.write(a-b-cin);
                break;
            case 0b00110 :
                out.write(--a);
                break;
            case 0b00111 :
                out.write(b);
                break;
            case 0b01000 :
            case 0b01001 :
                out.write(a&b);
                break;
            case 0b01010 :
            case 0b01011 :
                out.write(a|b);
                break;
            case 0b01100 :
            case 0b01101 :
                out.write(a^b);
                break;
            case 0b01110 :
            case 0b01111 :
                out.write(~a);
                break;
            case 0b10000 :
            case 0b10001 :
            case 0b10010 :
            case 0b10011 :
            case 0b10100 :
            case 0b10101 :
            case 0b10110 :
            case 0b10111 :
                out.write(a>>sh);
                break;
            default:
                out.write(a<<sh);
                break;
        }
    }
    
    
};