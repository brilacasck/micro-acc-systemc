/*
 * @ASCK
 */

#include <systemc.h>

SC_MODULE (IR) {
 
    sc_in <sc_uint<14>> addr; 
    sc_out <sc_uint<20>> inst;


    /*
    ** module global variables 
    */

    sc_uint<20> mem[819]; // 819 rows and 819*20=16380 bits = (16KB - 4bits) ~= 16KB

    int instNum = 50;
    sc_uint<20> instruction[50] = {
     // 0b01234567890123456789
        0b00000000000000000000,
        0b00100000000000000111,  // transfer (r0 <- 7): 7
        0b00010010010000000000,  // inc (r1++): 1

        //stall for achieving the correct register data - sub
        0b11110000000000000000, //stall
        0b11110000000000000000, //stall
        0b11110000000000000000, //stall

        0b00000110000010000010,  // sub (r3 <-r0 - r1): 6
        
        0b11110000000000000000, //stall
        0b11110000000000000000, //stall
        0b11110000000000000000, //stall

        0b00011000110010000001,  // addc (r4 <- r3 + r1 + 1): 8

        0b11110000000000000000, //stall
        0b11110000000000000000, //stall
        0b11110000000000000000, //stall

        0b00001001000000101001,  // shiftR (r4 >> 2): 2

        0b01010000000000000101, // stroe (mem[5] <= r0) : 7
        0b01001010000000000101, // load  (r5 <= mem[5]) : 7

        0b01100000000000000000, // do accelerator

        0b01000100000000010100, // load (r2 <= mem[20]) : 17 => 0x11

        0b11110000000000000000, //nop
        0b11110000000000000000, //nop
        0b11110000000000000000, //nop
        0b11110000000000000000, //nop
        0b11110000000000000000, //nop
        0b11110000000000000000  //nop
   };

    SC_CTOR (IR){
        SC_METHOD (process);
        sensitive << addr;

        for(int i=0; i<instNum; i++){
            mem[i] = instruction[i];
        }

        // filling out other rows with a nop opcode
        for(int i=instNum; i<819; i++){
            mem[i] = 0b11110000000000000000;
        }
    }
    
    void process () {
        if(addr.read() < 819){
            inst.write(mem[addr.read()]);
        }
        else{
            inst.write(0);
        }
    }
    
    
};