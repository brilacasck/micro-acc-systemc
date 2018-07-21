/*
 * @ASCK
 */

#include <systemc.h>
#include "../ALU.cpp"


using namespace std;

int sc_main(int argc, char* argv[]){
    cout << "starting ALU" << endl;

    sc_signal<sc_int<8>> a, b, out;
    sc_signal<sc_uint<5>> sel;
    sc_signal<sc_uint<3>> sa;
    sc_signal<bool> cin;

    ALU alu ("ALU");
    alu (a ,b, cin, sel, sa, out);
    
    sc_trace_file *wf = sc_create_vcd_trace_file("tracer");
    sc_trace (wf, a, "A");
    sc_trace (wf, b, "B");
    sc_trace (wf, cin, "Cin");
    sc_trace (wf, sel, "Sel");
    sc_trace (wf, sa, "SA");
    sc_trace (wf, out, "Output");
    
    //RESET
    a = 0;
    b = 0;
    cin = 0;
    sel = 00000;
    sa = 0;
    sc_start(1,SC_NS);


    for (int i=0; i<32 ;i++){
        a = 10;
        b = 5;
        cin = 1;
        sel = i;
        sa = 2;
        sc_start(2,SC_NS);
    }


    //RESET
    a = 0;
    b = 0;
    cin = 0;
    sel = 00000;
    sa = 0;
    sc_start(1,SC_NS);
    
    
    sc_close_vcd_trace_file(wf);
    

    return 0;
}
