/*
 * @ASCK
 */

#include <systemc.h>
#include "../PC.cpp"


using namespace std;

int sc_main(int argc, char* argv[]){
    cout << "starting PC" << endl;

    sc_signal<bool> clk;
    sc_signal<sc_uint<14>> prev_addr;
    sc_signal<sc_uint<14>> next_addr;

    PC pc ("PC");
    pc (clk, prev_addr, next_addr);
    
    sc_trace_file *wf = sc_create_vcd_trace_file("tracer");
    sc_trace (wf, clk, "clk");
    sc_trace (wf, prev_addr, "prev");
    sc_trace (wf, next_addr, "next");
    
    sc_int<14> tmp = 0;

    for (int i=0; i<32 ;i++){
        prev_addr = tmp +1;
        tmp = tmp +1;
        clk = 0;
        sc_start(1, SC_NS);
        clk = 1;
        sc_start(1, SC_NS);
    }
    
    
    sc_close_vcd_trace_file(wf);
    

    return 0;
}
