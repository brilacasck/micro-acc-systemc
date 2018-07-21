/*
 * @ASCK
 */

#include <systemc.h>

SC_MODULE (WB) {
 
    sc_in_clk clk;
    sc_in <sc_int<8>> prev_alu_result; 
    sc_in <sc_int<8>> prev_mem_result; 
    sc_in <bool> prev_WbMux; 
    sc_in <bool> prev_regWrite; 
    sc_in <sc_uint<3>> prev_rd; 
    
    sc_out <sc_int<8>> next_alu_result;
    sc_out <sc_int<8>> next_mem_result;
    sc_out <bool> next_WbMux;
    sc_out <bool> next_regWrite;
    sc_out <sc_uint<3>> next_rd;


    /*
    ** module global variables 
    */

    SC_CTOR (WB){
        SC_THREAD (process);
        sensitive << clk.pos();
    }
    
    void process () {
        while(true){
            wait();
            if(now_is_call){
                wait(micro_acc_ev);
            }
            next_alu_result.write(prev_alu_result.read());
            next_mem_result.write(prev_mem_result.read());
            next_WbMux.write(prev_WbMux.read());
            next_regWrite.write(prev_regWrite);
            next_rd.write(prev_rd.read());
        }
    }
    
    
};