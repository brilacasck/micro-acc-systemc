/*
 * @ASCK
 */

#include <systemc.h>
#include "../RegFile.cpp"


using namespace std;

int sc_main(int argc, char* argv[]){
    cout << "starting RegFile" << endl;

    sc_signal<bool> clk, regWrite;
    sc_signal<sc_uint<3>> r1, r2, r3;
    sc_signal<sc_int<8>> data, Rr1, Rr2;

    RegFile rf ("RegFile");
    rf (clk, regWrite, r1, r2, r3, data, Rr1, Rr2);
    
    sc_trace_file *wf = sc_create_vcd_trace_file("tracer");
    sc_trace (wf, clk, "clk");
    sc_trace (wf, regWrite, "regWrite");
    sc_trace (wf, r1, "r1");
    sc_trace (wf, r2, "r2");
    sc_trace (wf, r3, "r3");
    sc_trace (wf, data, "data");
    sc_trace (wf, Rr1, "Rr1");
    sc_trace (wf, Rr2, "Rr2");
    


    for (int i=0; i<8 ;i++){
        clk = 0;
        regWrite = 1;
        r3 = i;
        data = i*2;
        sc_start(1,SC_NS);
        clk = 1;
        sc_start(1,SC_NS);
    }

    for (int i=0; i<7 ;i++){
        clk = 0;
        regWrite = 0;
        r1 = i;
        r2 = i+1;
        sc_start(1,SC_NS);
        clk = 1;
        sc_start(1,SC_NS);
    }

    
    sc_close_vcd_trace_file(wf);
    

    return 0;
}
