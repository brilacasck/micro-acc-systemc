/*
 * @ASCK
 */

#include <systemc.h>

SC_MODULE (EXE) {
 
    sc_in_clk clk;
    sc_in <sc_int<8>> prev_result; 
    sc_in <sc_int<13>> prev_Imm; 
    sc_in <bool> prev_r; 
    sc_in <bool> prev_w; 
    sc_in <bool> prev_WbMux; 
    sc_in <bool> prev_call; 
    sc_in <bool> prev_regWrite; 
    sc_in <sc_uint<3>> prev_rd; 
    
    sc_out <sc_int<8>> next_result;
    sc_out <sc_int<13>> next_Imm;
    sc_out <bool> next_r;
    sc_out <bool> next_w;
    sc_out <bool> next_WbMux;
    sc_out <bool> next_call;
    sc_out <bool> next_regWrite;
    sc_out <sc_uint<3>> next_rd;


    /*
    ** module global variables 
    */

    SC_CTOR (EXE){
        SC_THREAD (process);
        sensitive << clk.pos();
    }
    
    void process () {
        while(true){
            wait();
            if(now_is_call){
                wait(micro_acc_ev);
            }
            next_result.write(prev_result.read());
            next_Imm.write(prev_Imm.read());
            next_r.write(prev_r.read());
            next_w.write(prev_w.read());
            next_WbMux.write(prev_WbMux.read());
            next_call.write(prev_call.read());
            next_regWrite.write(prev_regWrite);
            next_rd.write(prev_rd.read());
        }
    }
    
    
};