/*
 * @ASCK
 */

#include <systemc.h>
#include "../Memory.cpp"


using namespace std;

int sc_main(int argc, char* argv[]){
    cout << "starting Memory" << endl;

    sc_signal<sc_int<8>> data, out;
    sc_signal<bool> rw;
    sc_signal<sc_uint<13>> addr;

    Memory mem ("MEM");
    mem (rw, addr, data, out);
    
    sc_trace_file *wf = sc_create_vcd_trace_file("tracer");
    sc_trace (wf, rw, "r/~w");
    sc_trace (wf, addr, "addr");
    sc_trace (wf, data, "data");
    sc_trace (wf, out, "Output");
    
    //RESET
    rw = 1;
    addr = 0;
    data = 0;
    sc_start(1,SC_NS);

    for (int i=0; i<5 ;i++){
        rw = 0;
        addr = i;
        data = i*2;
        sc_start(1,SC_NS);
    }

    for (int i=0; i<5 ;i++){
        rw = 1;
        addr = i;
        data = 0;
        sc_start(1,SC_NS);
    }

    //RESET
    rw = 1;
    addr = 0;
    data = 0;
    sc_start(1,SC_NS);
    
    
    sc_close_vcd_trace_file(wf);
    

    return 0;
}
