/*
 * @ASCK
 */

#include <systemc.h>

SC_MODULE (Mux8) {
    sc_in <bool> sel;
    sc_in <sc_int<8>> in0; 
    sc_in <sc_int<8>> in1;
    
    sc_out <sc_int<8>> out;


    /*
    ** module global variables 
    */

    SC_CTOR (Mux8){
        SC_METHOD (process);
        sensitive << in0 << in1 << sel;
    }
    
    void process () {
        if(!sel.read()){
            out.write(in0.read());
        }
        else{
            out.write(in1.read());
        }
    }
    
    
};