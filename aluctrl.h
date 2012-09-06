#ifndef ALUCTRL_H_INCLUDED
#define ALUCTRL_H_INCLUDED

#include "mips.h"

SC_MODULE(ALUCTRL) {
	sc_in<  sc_bv<6> >  functionCode;	// Bit [0,5] of instruction
	sc_in<  sc_bv<2> >  ALUop;		// ALU operation code
	sc_out< sc_bv<3> >  ALUctrl;		// Control output to ALU

	void aluctrl_thread(void);

	// Constructor
	SC_CTOR(ALUCTRL) {
		SC_THREAD(aluctrl_thread);
		sensitive << functionCode;
		sensitive << ALUop;
	}
};

#endif
