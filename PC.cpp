/*
 * @ASCK
 */

#include <systemc.h>

SC_MODULE (PC) {
 
    sc_in <bool> clk;
    sc_in <sc_uint<14>> prev_addr; 
    
    sc_out <sc_uint<14>> next_addr;


    /*
    ** module global variables 
    */

    SC_CTOR (PC){
        SC_METHOD (process);
        sensitive << clk.pos();
    }
    
    void process () {
        next_addr.write(prev_addr.read());
    }
    
    
};