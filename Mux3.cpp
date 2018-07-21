/*
 * @ASCK
 */

#include <systemc.h>

SC_MODULE (Mux3) {
    sc_in <bool> sel;
    sc_in <sc_uint<3>> in0; 
    sc_in <sc_uint<3>> in1;
    
    sc_out <sc_uint<3>> out;


    /*
    ** module global variables 
    */

    SC_CTOR (Mux3){
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