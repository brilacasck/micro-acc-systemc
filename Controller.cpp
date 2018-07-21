/*
 * @ASCK
 */

#include <systemc.h>

SC_MODULE (Controller) {
 
    sc_in <sc_uint<4>> opcode, opselect;

    sc_out <sc_uint<5>> aluOp;
    sc_out <bool> regWrite, r, w, aluMux, regMux, wbMux, call; 


    /*
    ** module global variables 
    */

   sc_uint<4> opcd, opsel;
   sc_uint<5> op;

    //tmp
   int c = 0;

    SC_CTOR (Controller){
        SC_METHOD (process);
        sensitive << opcode << opselect;
    }
    
    void process () {
        opcd = opcode.read();
        opsel = opselect.read();

        switch (opcd){
            case 0b0000:
            case 0b0001:
                op = opsel;
                op = opsel << 1;
                op[0] = opcd[0];
                aluOp.write(op); // concatinated to produce aluop
                regWrite.write(1);
                r.write(0);
                w.write(0);
                regMux.write(0);    // r1 = rs
                aluMux.write(0);    // B = Rr2
                wbMux.write(0);     // use alu result
                call.write(0);
                break;

            /*
            **  Transfer Imm to the Register rd
            */
            case 0b0010:
                aluOp.write(0b00111); // transfer B (rd = Imm)
                regWrite.write(1);
                r.write(0);
                w.write(0);
                regMux.write(0);
                aluMux.write(1);    // B = imm
                wbMux.write(0);
                call.write(0);
                break;

            /*
            **  Add Imm with register rd content and move it to the Register rd
            */
            case 0b0011:
                aluOp.write(0b00100); // Add A and B (rd += Imm) 
                regWrite.write(1);
                r.write(0);
                w.write(0);
                regMux.write(1);
                aluMux.write(1);
                wbMux.write(0);
                call.write(0);
                break;

            /*
            **  LOAD from Imm address of memory to rd
            */
            case 0b0100:
                aluOp.write(0);
                regWrite.write(1);
                r.write(1);
                w.write(0);
                regMux.write(0);
                aluMux.write(0);
                wbMux.write(1);     // use memory result
                call.write(0);
                break;

            /*
            **  STORE rd content to imm address of memory
            */
            case 0b0101:
                aluOp.write(0); // transfer A
                regWrite.write(0);  // don't write back to register
                r.write(0);
                w.write(1);
                regMux.write(1);    // r1 = reg (rd)
                aluMux.write(0); // ignorable!
                wbMux.write(0); // ignorable!
                call.write(0);
                break;
                
            /*
            **  NOP
            */
            case 0b1111:
                aluOp.write(0);
                regWrite.write(0);
                r.write(0);
                w.write(0);
                regMux.write(0);
                aluMux.write(0);
                wbMux.write(0);
                call.write(0);
                break;

            /*
            **  CALL Acc.
            */
            default: 
                regWrite.write(0);  // don't write back to register
                r.write(0);
                w.write(0);
                call.write(1);
                break;
        }
    }
    
    
};