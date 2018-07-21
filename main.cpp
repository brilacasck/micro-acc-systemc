/*
 * @ASCK
 */

#include <systemc.h>

#include <System.cpp>

using namespace std;

int sc_main(int argc, char* argv[]){

    cout << "starting the complete project" << endl;
    
    sc_trace_file *wf = sc_create_vcd_trace_file("project");

    sc_signal <bool> clk;
    sc_signal <bool> clk_bus;
    sc_signal <sc_int<8>> reg_dump[8];
    sc_signal <sc_uint<5>> aluop;
    sc_signal <sc_uint<14>> pc;


    System sys ("System");
    sys (clk, clk_bus, pc, aluop);

    for (int i=0; i<8; i++){
        sys.reg_dump[i](reg_dump[i]);
    }

    sc_trace (wf, clk, "clk");
    sc_trace (wf, clk_bus, "bus_clk");
    sc_trace (wf, pc, "pc");
    sc_trace (wf, aluop, "aluop");
    for (int i=0; i<8; i++){
        char str[3];
        sprintf(str, "%d", i);
        sc_trace (wf, reg_dump[i], "R" + string(str));
    }


    for (int i=0; i<40 ;i++){
        clk_bus = 0;
        clk = 1;
        sc_start(1,SC_NS);
        clk_bus = 1;
        sc_start(1,SC_NS);
        clk_bus = 0;
        sc_start(1,SC_NS);
        clk_bus = 1;
        sc_start(1,SC_NS);
        clk_bus = 0;
        clk = 0;
        sc_start(1,SC_NS);
        clk_bus = 1;
        sc_start(1,SC_NS);  
        clk_bus = 0;
        sc_start(1,SC_NS);
        clk_bus = 1;
        sc_start(1,SC_NS);
    }
    

    sc_close_vcd_trace_file(wf);

    cout << "vcd file completed" << endl;

    return 0;
}
