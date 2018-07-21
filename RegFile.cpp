/*
 * @ASCK
 */

#include <systemc.h>

SC_MODULE (RegFile) {
 
    sc_in_clk clk; 
    sc_in <bool> regWrite; 
    sc_in <sc_uint<3>> r1; 
    sc_in <sc_uint<3>> r2; 
    sc_in <sc_uint<3>> r3; 
    sc_in <sc_int<8>> data; 

    sc_out <sc_int<8>> Rr1;
    sc_out <sc_int<8>> Rr2;

    //testing wires
    sc_out <sc_int<8>> r[8];


    /*
    ** module global variables 
    */

    sc_int<8> reg[8];   // 8 registers in register file

    int c = 0;
    SC_CTOR (RegFile){
        SC_METHOD (process);
        sensitive << regWrite << r1 << r2 << r3 << data;
    }
    
    void process () {
        // whether the regWrite is 0 or not, the Rr1 and Rr2 have the corresponding output! 
        Rr1.write(reg[r1.read()]);
        Rr2.write(reg[r2.read()]);
        if(regWrite.read() == 1){
            reg[r3.read()] = data.read();
        }

        for (int i=0; i< 8; i++){
            r[i].write(reg[i]);
        }

        if (c++ == 32) {
            reg[0] = 3;
        }
    }
    
    
};