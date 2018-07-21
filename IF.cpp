/*
 * @ASCK
 */

#include <systemc.h>

SC_MODULE (IF) {
 
    sc_in_clk clk;
    sc_in <sc_uint<20>> prev_data; 

    sc_out <sc_uint<20>> next_data;


    /*
    ** module global variables 
    */

    SC_CTOR (IF){
        SC_THREAD (process);
        sensitive << clk.pos();
    }
    
    void process () {
        while(true){
            wait();
            if(now_is_call){
                cout<<"\t\t\t\t*******************" << endl;
                wait(micro_acc_ev);
            }
            next_data.write(prev_data.read());
        }
    }
    
    
};