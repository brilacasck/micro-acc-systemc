/*
 * @ASCK
 */

#include <systemc.h>
#include "../Bus.cpp"


using namespace std;

int sc_main(int argc, char* argv[]){
    cout << "starting Bus" << endl;

    sc_signal<bool> clk,read,write,req,call;
    sc_signal<sc_uint<13>> addr;
    sc_signal<sc_int<8>> data;

    Bus bus ("Bus");
    bus (clk, req,read,write,call,addr,data);
    
    sc_trace_file *wf = sc_create_vcd_trace_file("tracer");
    sc_trace (wf, clk, "clk");
    sc_trace (wf, req, "req");
    sc_trace (wf, read, "read");
    sc_trace (wf, write, "write");
    sc_trace (wf, call, "call");
    sc_trace (wf, addr, "addr");
    sc_trace (wf, data, "data");


    for (int i=0; i<6 ;i++){
        clk = 0;
        req = i%1;
        read = 1;
        write = 0;
        call = i%2-1;
        addr = i*5;
        data = i*3;
        sc_start(1,SC_NS);
        clk = 1;
        sc_start(1,SC_NS);
    }
    
    
    sc_close_vcd_trace_file(wf);
    

    return 0;
}
