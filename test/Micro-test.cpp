/*
 * @ASCK
 */

#include <systemc.h>

#include <../Micro.cpp>

using namespace std;

int sc_main(int argc, char* argv[]){

    cout << "starting the Micro" << endl;
    
    sc_trace_file *wf = sc_create_vcd_trace_file("test");

    sc_signal <bool> clk;
    sc_signal <sc_int<8>> mem_data; 
    sc_signal <bool> read, write, call;
    sc_signal <sc_uint<13>> addr;
    sc_signal <sc_int<8>> data;
    sc_signal <sc_uint<5>> aluop;
    sc_signal <sc_uint<14>> pc;
    sc_signal <sc_uint<20>> inst;

    sc_signal <sc_int<8>> reg_dump[8];

    Micro micro ("Micro");
    micro (clk, mem_data, read, write, call, addr, data, aluop, pc, inst);
    for (int i=0; i<8; i++){
        micro.reg_dump[i](reg_dump[i]);
    }

    sc_trace (wf, clk, "clk");
    sc_trace (wf, mem_data, "mem_data");
    sc_trace (wf, read, "read");
    sc_trace (wf, write, "write");
    sc_trace (wf, call, "call");
    sc_trace (wf, addr, "addr");
    sc_trace (wf, data, "data");
    sc_trace (wf, aluop, "aluOp");
    sc_trace (wf, pc, "pc");
    sc_trace (wf, inst, "inst");

    for (int i=0; i<8; i++){
        char str[3];
        sprintf(str, "%d", i);
        sc_trace (wf, reg_dump[i], "R" + string(str));
    }
    cout << "testing" << endl;

    /* CLK MUST START WITH HIGH LEVEL */ //to ensure that pc address goes through healthy 0 -> (1) -> 2 -> 3 -> ... 
    for (int i=0; i<25 ;i++){
        clk = 1;
        mem_data = 0;
        sc_start(500,SC_NS);
        clk = 0;
        sc_start(500,SC_NS);
    }
    sc_start(500,SC_NS);

    sc_close_vcd_trace_file(wf);

    cout << "vcd file completed" << endl << "+ DONE +" << endl;

    return 0;
}
