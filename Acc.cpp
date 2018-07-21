/*
 * @ASCK
 */

#include <systemc.h>

SC_MODULE (Acc) {
 
    
    sc_in <sc_int<8>> mem_data; 
    sc_in <bool> call;

    sc_out <bool> read, write;
    sc_out <sc_uint<13>> addr;
    sc_out <sc_int<8>> data;

    /*
    ** module global variables 
    */


    SC_CTOR (Acc){
        SC_THREAD (process);
        sensitive << call;
    }

    void process () {
        while(true){
            wait();
            if(call.read()==1){
                std::cout << "@@-_.|*^<!! STARTING ACCELERATOR !!>^*|._-@@"    << endl;
                operation();
            }
        }
    }

    void operation () {
        // store value 10 to address 11
        write_to_mem (11, 10);

        // read from address 5 of memory : data=7 (this data has already stored by micro processor)
        sc_int<8> a = read_from_mem(5);

        // read from address 11 of memory : data=10
        sc_int<8> b = read_from_mem(11);

        // a + b
        sc_int<8> c = a+b;

        // store c to address 20 of memory
        write_to_mem (20, c);


        // end the acc task and begin the micro task
        micro_notif();
    }

    /*
    ** notify micro processor that accelerator job is done and return back to the micro processor job 
    */
    void micro_notif (){
        micro_acc_ev.notify(); 
        now_is_call = false;            
    }

    // read from memory
    sc_int<8> read_from_mem (int ad) {
        read.write(1);
        write.write(0);
        addr.write(ad);

        wait(acc_mem_read_ev);

        no_read_write();
        wait(10, SC_NS);

        return mem_data.read();
    }

    // write to memory
    void write_to_mem (int ad, int dt) {
        read.write(0);
        write.write(1);
        addr.write(ad);
        data.write(dt);
        
        wait(acc_mem_write_ev);

        no_read_write();
        wait(1, SC_NS);
    }

    // change the read and write signals not to read and not to write
    void no_read_write () {
        read.write(0);
        write.write(0);
    }
    
};