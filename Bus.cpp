/*
 * @ASCK
 */

#include <systemc.h>

SC_MODULE (Bus) {
 
    
    sc_in_clk clk; 
    sc_in <bool> req; // X DIDN'T USED! X
    sc_in <bool> read; 
    sc_in <bool> write; 
    sc_in <bool> call;
    sc_in <sc_uint<13>> addr;   // for both Mem. and Acc.
    sc_in <sc_int<8>> data; 

    sc_out <bool> ack;  // X DIDN'T USED! X
    sc_out <bool> read_out; 
    sc_out <bool> write_out; 
    sc_out <bool> call_out;
    sc_out <sc_uint<13>> addr_out;   // for both Mem. and Acc.
    sc_out <sc_int<8>> data_out; 

    /*
    ** module global variables 
    */

    SC_CTOR (Bus){
        SC_METHOD (process);
        sensitive << clk.pos();
    }
    
    void process () {
        ack.write(req.read());
        read_out.write(read.read());
        write_out.write(write.read());
        call_out.write(call.read());
        addr_out.write(addr.read());
        data_out.write(data.read());
    }

    
};