/*
 * @ASCK
 */

#include <systemc.h>
#include <./PC.cpp>
#include <./IR.cpp>
#include <./IF.cpp>
#include <./Mux3.cpp>
#include <./Mux8.cpp>
#include <./RegFile.cpp>
#include <./Controller.cpp>
#include <./ID.cpp>
#include <./ALU.cpp>
#include <./EXE.cpp>
#include <./WB.cpp>

#include <bitset>


SC_MODULE (Micro) {

    sc_in_clk clk; 
 
    sc_in <sc_int<8>> mem_data; 
    
    sc_out <bool> read, write, call;
    sc_out <sc_uint<13>> addr;
    sc_out <sc_int<8>> data;

    // testing wires
    sc_out <sc_uint<5>> test_aluOp;
    sc_out <sc_uint<14>> test_pc;
    sc_out <sc_uint<20>> test_inst;
    sc_out <sc_int<8>> reg_dump[8];
    // sc_out <bool> test_regWrite, test_r_nw, test_aluMux, test_regMux, test_wbMux, test_call;


    /*
    ** module global variables 
    */
    
    /*
    **  SIGNALS
    */
    // -----
    sc_uint<14> tmp_pc_prev_addr;
    sc_signal<sc_uint<14>> pc_prev_addr;
    sc_signal<sc_uint<14>> pc_next_addr;

    sc_signal <sc_uint<20>> ir_inst;

    sc_signal <sc_uint<20>> if_next_data;
    sc_signal <sc_uint<4>> opcode;
    sc_signal <sc_uint<3>> rd;
    sc_signal <sc_uint<3>> rs;
    sc_signal <sc_uint<3>> rt;
    sc_signal <sc_uint<3>> sa;
    sc_signal <sc_uint<4>> opselect;
    sc_uint<20> tmp;
    sc_signal <sc_int<13>> offset;

    // controller output signal
    sc_signal <sc_uint<5>> aluOp;
    sc_signal <bool> regWrite, r, w, aluMux, regMux, wbMux, acc_call; 

    sc_signal <sc_uint<3>> mux_reg_res;

    /*
    **  Register File signals
    */
    sc_signal <sc_int<8>> Rr1;
    sc_signal <sc_int<8>> Rr2;
    sc_signal <sc_int<8>> regs[8];

    sc_signal <sc_int<8>> id_next_A;
    sc_signal <sc_int<8>> id_next_B;
    sc_signal <sc_int<13>> id_next_Imm;
    sc_signal <sc_uint<3>> id_next_Sa;
    sc_signal <sc_uint<5>> id_next_AluOp;
    sc_signal <bool> id_next_r;
    sc_signal <bool> id_next_w;
    sc_signal <bool> id_next_AluMux;
    sc_signal <bool> id_next_WbMux;
    sc_signal <bool> id_next_call;
    sc_signal <bool> id_next_regWrite;
    sc_signal <sc_uint<3>> id_next_rd;

    sc_signal <sc_int<8>> alu_in_B;
    sc_signal <sc_int<8>> id_nex_imm_8bits;

    sc_signal <sc_int<8>> alu_out;
    sc_signal<bool> carry;

    sc_signal <sc_int<8>> exe_next_result;
    sc_signal <sc_int<13>> exe_next_Imm;
    sc_signal <bool> exe_next_r;
    sc_signal <bool> exe_next_w;
    sc_signal <bool> exe_next_WbMux;
    sc_signal <bool> exe_next_call;
    sc_signal <bool> exe_next_regWrite;
    sc_signal <sc_uint<3>> exe_next_rd;

    sc_signal <sc_int<8>> wb_next_result;
    sc_signal <sc_int<8>> wb_prev_mem_data;
    sc_signal <sc_int<8>> wb_next_mem_data;
    sc_signal <bool> wb_next_WbMux;
    sc_signal <bool> wb_next_regWrite;
    sc_signal <sc_uint<3>> wb_next_rd;

    sc_signal<sc_uint<13>> mem_addr;

    sc_signal <sc_int<8>> regFileData;

    sc_int<13> tmp_id_next_Imm;

    sc_signal <sc_int<8>> sig_mem_data;

    // -----

    PC *pc;
    IR *ir;
    IF *iff;
    Controller *ctl;
    Mux3 *mux_reg;
    ID *id;
    Mux8 *alu_in_mux;
    ALU *alu;
    EXE *exe;
    WB *wb;
    Mux8 *wb_out_mux;
    RegFile *reg_file;

    SC_CTOR (Micro){

        SC_THREAD (process);
        sensitive << clk.pos();
 

        
        pc = new PC("PC");
        ir = new IR("IR");
        iff = new IF ("IF");
        ctl = new Controller ("Controller");
        mux_reg = new Mux3 ("RegMux3bits");
        id = new ID ("ID");
        alu_in_mux = new Mux8 ("AluInputMux");
        alu = new ALU  ("ALU");
        exe = new EXE  ("EXE");
        wb = new WB ("WB");
        wb_out_mux = new Mux8 ("WBOutputMux");
        reg_file = new RegFile ("RegisterFile");
 



        pc->clk(clk);
        pc->prev_addr(pc_prev_addr);
        pc->next_addr(pc_next_addr);

        ir->addr(pc_next_addr);
        ir->inst(ir_inst);

        iff->clk(clk);
        iff->prev_data(ir_inst);
        iff->next_data(if_next_data);
        
        //ctl (opcode, opselect, aluOp, regWrite, r_nw, aluMux, regMux, wbMux, acc_call);
        ctl->opcode(opcode);
        ctl->opselect(opselect);
        ctl->aluOp(aluOp);
        ctl->regWrite(regWrite);
        ctl->r(r);
        ctl->w(w);
        ctl->aluMux(aluMux);
        ctl->regMux(regMux);
        ctl->wbMux(wbMux);
        ctl->call(acc_call);

        //mux_reg (regMux, rs, rd, mux_reg_res);
        mux_reg->sel(regMux);
        mux_reg->in0(rs);
        mux_reg->in1(rd);
        mux_reg->out(mux_reg_res);

        //id (clk, Rr1, id_next_A, Rr2, id_next_B, offset, id_next_Imm, sa, id_next_Sa, aluOp, id_next_AluOp, r_nw, id_next_MemOp, aluMux, id_next_AluMux, wbMux, id_next_WbMux, acc_call, id_next_call, rd, id_next_rd);
        id->clk(clk);
        id->prev_A(Rr1);
        id->next_A(id_next_A);
        id->prev_B(Rr2);
        id->next_B(id_next_B);
        id->prev_Imm(offset);
        id->next_Imm(id_next_Imm);
        id->prev_Sa(sa);
        id->next_Sa(id_next_Sa);
        id->prev_AluOp(aluOp);
        id->next_AluOp(id_next_AluOp);
        id->prev_r(r);
        id->next_r(id_next_r);
        id->prev_w(w);
        id->next_w(id_next_w);
        id->prev_AluMux(aluMux);
        id->next_AluMux(id_next_AluMux);
        id->prev_WbMux(wbMux);
        id->next_WbMux(id_next_WbMux);
        id->prev_call(acc_call);
        id->next_call(id_next_call);
        id->prev_regWrite(regWrite);
        id->next_regWrite(id_next_regWrite);
        id->prev_rd(rd);
        id->next_rd(id_next_rd);

        /*
        ** Mux 8 for immediate or B
        */
        //alu_in_mux (id_next_AluMux, id_next_B, id_nex_imm_8bits, alu_in_B);
        alu_in_mux->sel(id_next_AluMux);
        alu_in_mux->in0(id_next_B);
        alu_in_mux->in1(id_nex_imm_8bits);
        alu_in_mux->out(alu_in_B);

        /*
        ** ALU
        */
        carry = 1;
        //alu (id_next_A, alu_in_B, carry, id_next_AluOp, id_next_Sa, alu_out);
        alu->in1(id_next_A);
        alu->in2(alu_in_B);
        alu->c(carry);
        alu->aluop(id_next_AluOp);
        alu->sa(id_next_Sa);
        alu->out(alu_out);

        /*
        ** EXE
        */
        //exe (clk, alu_out, exe_next_result, id_next_Imm, exe_next_Imm, id_next_MemOp, exe_next_MemOp, id_next_WbMux, exe_next_WbMux, id_next_call, exe_next_call, id_next_rd, exe_next_rd);
        exe->clk(clk);
        exe->prev_result(alu_out);
        exe->next_result(exe_next_result);
        exe->prev_Imm(id_next_Imm);
        exe->next_Imm(exe_next_Imm);
        exe->prev_r(id_next_r);
        exe->next_r(exe_next_r);
        exe->prev_w(id_next_w);
        exe->next_w(exe_next_w);
        exe->prev_WbMux(id_next_WbMux);
        exe->next_WbMux(exe_next_WbMux);
        exe->prev_call(id_next_call);
        exe->next_call(exe_next_call);
        exe->prev_regWrite(id_next_regWrite);
        exe->next_regWrite(exe_next_regWrite);
        exe->prev_rd(id_next_rd);
        exe->next_rd(exe_next_rd);


        /*
        ** WB
        */
        //wb (clk, exe_next_result, wb_next_result, wb_prev_mem_data, wb_next_mem_data, exe_next_WbMux, wb_next_WbMux, exe_next_rd, wb_next_rd);
        wb->clk(clk);
        wb->prev_alu_result(exe_next_result);
        wb->next_alu_result(wb_next_result);
        wb->prev_mem_result(mem_data);
        wb->next_mem_result(wb_next_mem_data);
        wb->prev_WbMux(exe_next_WbMux);
        wb->next_WbMux(wb_next_WbMux);
        wb->prev_regWrite(exe_next_regWrite);
        wb->next_regWrite(wb_next_regWrite);
        wb->prev_rd(exe_next_rd);
        wb->next_rd(wb_next_rd);

        /*
        ** Mux 8 bits for WB
        */
        //wb_out_mux (wb_next_WbMux, wb_next_result, wb_next_mem_data, regFileData);
        wb_out_mux->sel(wb_next_WbMux);
        wb_out_mux->in0(wb_next_result);
        wb_out_mux->in1(wb_next_mem_data);
        wb_out_mux->out(regFileData);


        /*
        ** Register File Module
        */
        //reg_file (clk, regWrite, mux_reg_res, rt, wb_next_rd, regFileData, Rr1, Rr2);
        reg_file->clk(clk);
        reg_file->regWrite(wb_next_regWrite);
        reg_file->r1(mux_reg_res);
        reg_file->r2(rt);
        reg_file->r3(wb_next_rd);
        reg_file->data(regFileData);
        reg_file->Rr1(Rr1);
        reg_file->Rr2(Rr2);

        for (int i=0; i<8; i++){
            reg_file->r[i](regs[i]);
        }

    }

    /*
    **  CLOCK THREAD FOR DOING PROCESSES
    */
    void process(){
        while(true){
            if(id_next_call){
                now_is_call = true;
            }
            wait();

            ///////////////
            pcInc();
            decode();
            ImmTo8bits();
            busAccess();
            tester();
            /////////////
            
            /*
            ** HANDLE ACCELERATOR CALLS
            */
            if(exe_next_call.read()){
                call.write(1);
                pc_prev_addr = (pc_prev_addr.read());
                wait(micro_acc_ev);
                pc_prev_addr = (pc_prev_addr.read()) + 1;
            }
        }
    }

    void pcInc (){
        // increment the pc
        tmp_pc_prev_addr = pc_prev_addr.read();
        pc_prev_addr = ++tmp_pc_prev_addr;
    }

    void decode (){
        /*
        **  split next instruction to the corresponding signals (Instruction Decode)
        */
        tmp = ir_inst.read();
        opcode = tmp.range(19, 16);
        opselect = tmp.range(3, 0);
        rd = tmp.range(15, 13);
        rs = tmp.range(12, 10);
        rt = tmp.range(9, 7);
        sa = tmp.range(6, 4);
        offset = (sc_int<13>) tmp.range(12, 0);
    }

    void ImmTo8bits (){
        /*
        ** Mux 8 for immediate or B
        */
        tmp_id_next_Imm = offset.read();
        id_nex_imm_8bits = (tmp_id_next_Imm.range(7, 0));
    }

    void busAccess (){
        /*
        ** ACCESS MEM. VIA BUS
        */
        wb_prev_mem_data = mem_data.read();
        mem_addr = (sc_uint<13>) id_next_Imm.read();
        read.write(exe_next_r.read());
        write.write(exe_next_w.read());
        addr.write(mem_addr.read());
        data.write(exe_next_result.read());
        call.write(0); // setting of the value is also performs in process function
    }

    void tester () {

        // testing wires
        for(int i=0; i<8; i++){
            reg_dump[i].write(regs[i].read()); 
        }
        test_aluOp.write(aluOp.read());
        test_pc.write(pc_next_addr.read());
        test_inst.write((sc_uint<20>)if_next_data.read());

        print();
    }

    void print(){
        cout << "pc addr ((FETCH)) :\t" << pc_next_addr << endl << endl; 

        cout << "IF inst ((DECODE)):\t" << std::hex << if_next_data << endl; 
        cout << "controller| opcode:\t" << opcode << endl;
        cout << "regFile| regMux   :\t" << regMux << endl;
        cout << "regFile| r1       :\t" << mux_reg_res << endl;
        cout << "regFile| r2       :\t" << rt << endl; 
        cout << "regFile| r3       :\t" << wb_next_rd << endl; 
        cout << "regFile| imm      :\t" << offset << endl; 
        cout << "regFile| data     :\t" << wb_next_rd << endl; 
        cout << "regFile| regWrite :\t" << wb_next_regWrite << endl << endl; 

        cout << "A ((EXE))         :\t" << id_next_A << endl; 
        cout << "B                 :\t" << alu_in_B << endl; 
        cout << "aluOp             :\t" << id_next_AluOp << endl; 
        cout << "aluMux            :\t" << id_next_AluMux << endl; 
        cout << "imm               :\t" << id_next_Imm << endl; 
        cout << "aluOut            :\t" << alu_out << endl << endl; 

        cout << "imm               :\t" << exe_next_Imm << endl; 
        cout << "data_in ((MEM))   :\t" << exe_next_result << endl; 
        cout << "addr              :\t" << exe_next_Imm << endl; 
        cout << "read              :\t" << exe_next_r << endl; 
        cout << "write             :\t" << exe_next_w << endl; 
        cout << "data_out          :\t" << mem_data << endl << endl; 

        cout << "data ((WB))       :\t" << regFileData << endl; 
        cout << "rd                :\t" << exe_next_rd << endl; 
        cout << "wbMux             :\t" << wb_next_WbMux << endl << endl; 
    }

    
};