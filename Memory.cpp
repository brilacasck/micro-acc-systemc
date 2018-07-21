/*
 * @ASCK
 */

#include <systemc.h>

SC_MODULE (Memory) {
 
    sc_in <bool> r_nw;
    sc_in <sc_uint<13>> addr;
    sc_in <sc_int<8>> data;

    sc_out <sc_int<8>> out;

    /*
    ** module global variables 
    */

    sc_int<8> mem[8192] = {0};    // 2^13 rows 
    bool done = false;

    SC_CTOR (Memory){
        SC_METHOD (process);
        sensitive << r_nw << addr << data;

    }
    
    void process () {
        if(done){
            return;
        }
        if(addr.read() < 8192){
            if(r_nw.read()){
                out.write(mem[addr.read()]);
            }
            else{
                mem[addr.read()] = data.read();
                out.write(mem[addr.read()]);
            }
        }
        else{
            out.write(0);
        }
    }
    
    
};