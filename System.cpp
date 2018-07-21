/*
 * @ASCK
 */

#include <systemc.h>

/*
**  GLOBAL EVENT FOR MICRO-ACC
*/
//////////////////////////
sc_event micro_acc_ev;///
////////////////////////

/*
**  GLOBAL EVENT FOR ACC-MEMORY -> READ MODE
*/
//////////////////////////
sc_event acc_mem_read_ev;///
////////////////////////

/*
**  GLOBAL EVENT FOR ACC-MEMORY -> WRITE MODE
*/
//////////////////////////
sc_event acc_mem_write_ev;///
////////////////////////

/*
**  variable for checking if IF/ID/EXE/WB should have been stopped after call=1 on memory-access stage or not!  
*/
bool now_is_call = false; 


#include <./Micro.cpp>
#include <./Bus.cpp>
#include <./Memory.cpp>
#include <./Acc.cpp>


SC_MODULE(System)
{

    sc_in_clk clk;
    sc_in_clk clk_bus;

    // testing wires
    sc_out<sc_uint<14>> pc;
    sc_out<sc_uint<5>> test_aluop;
    sc_out<sc_int<8>> reg_dump[8];

    /*
    ** module global variables 
    */

    //
    // SIGNALS
    //

    // MICRO
    sc_signal<sc_int<8>> micro_data_in; // input
    sc_signal<bool> micro_read, micro_write, micro_call;
    sc_signal<sc_uint<13>> micro_addr;
    sc_signal<sc_int<8>> micro_data_out; // output

    // BUS
    sc_signal<bool> req;
    sc_signal<bool> read_in;
    sc_signal<bool> write_in;
    sc_signal<bool> call_in;
    sc_signal<sc_uint<13>> addr_in; // for both Mem. and Acc.
    sc_signal<sc_int<8>> data_in;
    //// INPUTS -up- / -down- OUTPUTS
    sc_signal<bool> ack;
    sc_signal<bool> read_out;
    sc_signal<bool> write_out;
    sc_signal<bool> call_out;
    sc_signal<sc_uint<13>> addr_out; // for both Mem. and Acc.
    sc_signal<sc_int<8>> data_out;

    // MEMORY
    sc_signal<sc_int<8>> mem_data_in, mem_data_out;
    sc_signal<sc_uint<13>> mem_addr;
    sc_signal<bool> r_nw;

    // ACC1
    sc_signal <bool> acc_call_in, acc_read, acc_write;
    sc_signal <sc_uint<13>> acc_addr_out;
    sc_signal <sc_int<8>> acc_data_in, acc_data_out; 

    //TESTING SIGNALS
    sc_signal<sc_uint<5>> test_aluOp;
    sc_signal<sc_uint<14>> test_pc;
    sc_signal<sc_uint<20>> test_inst;

    /*
    **  CREATE POINTER TO COMPONENTS
    */
    Micro *micro;
    Bus *bus;
    Memory *memory;
    Acc *acc;

    SC_CTOR(System)
    {
        SC_METHOD(process);
        sensitive << clk_bus.pos();

        micro = new Micro("Micro");
        bus = new Bus("Bus");
        memory = new Memory("MEMORY");
        acc = new Acc("Acc1");

        micro->clk(clk);
        micro->mem_data(micro_data_in);
        micro->read(micro_read);
        micro->write(micro_write);
        micro->call(micro_call);
        micro->addr(micro_addr);
        micro->data(micro_data_out);

        micro->test_aluOp(test_aluOp);
        micro->test_pc(test_pc);
        micro->test_inst(test_inst);

        for (int i = 0; i < 8; i++)
        {
            micro->reg_dump[i](reg_dump[i]);
        }

        req = 1;
        bus->clk(clk_bus);
        bus->req(req);
        bus->read(read_in);
        bus->write(write_in);
        bus->call(micro_call);
        bus->addr(addr_in);
        bus->data(data_in);

        bus->ack(ack);
        bus->read_out(read_out);
        bus->write_out(write_out);
        bus->call_out(call_out);
        bus->addr_out(addr_out);
        bus->data_out(data_out);

        r_nw = 1;
        memory->r_nw(r_nw);
        memory->addr(mem_addr);
        memory->data(mem_data_in);
        memory->out(mem_data_out);

        acc->mem_data(acc_data_in);
        acc->call(acc_call_in);
        acc->read(acc_read);
        acc->write(acc_write);
        acc->addr(acc_addr_out);
        acc->data(acc_data_out);
        
    }

    int c = 0; //clk counter for printing

    /*
    **  FLAG: if the **acc_read** of accelerator is enabled then we know that after 2 clks
    **        we will have the memory data on the bus data_out!
    **
    **        BRIEF: this flag acknowledge us whether we have to notify the acc_mem_read_ev or not!   
    */
    int notify_flag_read = 0;
    int notify_flag_write = 0;

    void process()
    {
        // testing wires
        test_aluop.write(test_aluOp.read());
        pc.write(test_pc.read());

        cout << "-----------------------------------------------" << endl;
        cout << "\t-___ " << "bus_clk: 0X" <<c++ << " ___-" << endl << endl;        


        /*
        ** Micro - MEMORY - ACC
        */

        mem_addr = addr_out.read();
        mem_data_in = data_out.read();
        micro_data_in = data_out.read();
        acc_data_in = data_out.read();
        acc_call_in = call_out.read();

        if (read_out.read() || write_out.read() || call_out.read()){
            if (read_out.read()){
                r_nw = read_out.read();  
                data_in = mem_data_out.read();
            }
            else if (write_out.read()){
                r_nw = !(write_out.read());
            }
        }


        ////////////////////////HANDLE ACC READ/WRITE////////////////////////
        if (notify_flag_write !=0 && notify_flag_write < 3){
            // increment the flag to get to the intended clk count
            notify_flag_write++;
            return;
        }
        else if (notify_flag_write == 3){
            // the write operation should have been done
            notify_flag_write = 0;

            acc_mem_write_ev.notify();
            return; 
        }

        if (notify_flag_read !=0 && notify_flag_read < 4){
            // increment the flag to get to the intended clk count
            notify_flag_read++;
            return;
        }
        else if (notify_flag_read == 4){
            // should we notify accelerator event? (two clocks have passed)
            notify_flag_read = 0;
            
            acc_mem_read_ev.notify();
            return;
        }




        ///////////////////////////////////////////////////////////////////MICRO
        if (micro_read.read() || micro_write.read() || micro_call.read())
        {
            read_in = micro_read.read();
            write_in = micro_write.read();
            call_in = micro_call.read();

            if (micro_read.read()){

                addr_in = micro_addr.read();
            }
            else if (micro_write.read()){
                data_in = micro_data_out.read();
                addr_in = micro_addr.read();
            }
        }
        ///////////////////////////////////////////////////////////////////ACC
        if (acc_read.read() || acc_write.read())
        {
            
            read_in = acc_read.read();
            write_in = acc_write.read();

            if (acc_read.read()){
                // increment accelerator notify_flag_read
                notify_flag_read++;

                addr_in = acc_addr_out.read();
            }
            else if (acc_write.read()){
                // increment accelerator notify_flag_write
                notify_flag_write++;

                data_in = acc_data_out.read();
                addr_in = acc_addr_out.read();
            }
        }


    }
};