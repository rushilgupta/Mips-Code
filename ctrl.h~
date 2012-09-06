#ifndef CTRL_H_INCLUDED
#define CTRL_H_INCLUDED

#include "mips.h"

SC_MODULE(CTRL) {
	sc_in<  sc_bv<6> >	Opcode;	// Instruction bits [31-26]
	sc_out< sc_bv<1> >	RegDst;
	sc_out< sc_bv<1> >	Branch;
	sc_out< sc_bv<1> >	MemRead;
	sc_out< sc_bv<1> >	MemtoReg;
	sc_out< sc_bv<2> >	ALUop;
	sc_out< sc_bv<1> >	MemWrite;
	sc_out< sc_bv<1> >	ALUSrc;
	sc_out< sc_bv<1> >	RegWrite;
	sc_out< sc_bv<DWORD> >	c4;	// Constant value 4

	void ctrl_thread(void);

	// Constructor
	SC_CTOR(CTRL) {
		SC_THREAD(ctrl_thread);
		sensitive << Opcode;
	}
};

#endif
