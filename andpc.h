#ifndef ANDPC_H_INCLUDED
#define ANDPC_H_INCLUDED

#include "mips.h"

SC_MODULE(ANDPC) {
	sc_in<  sc_bv<32> > a;	// First operand
	sc_out< sc_bv<32> > r;	// Result

	void andpc_thread(void);

	// Constructor
	SC_CTOR(ANDPC) {
		SC_THREAD(andpc_thread);
		sensitive << a;
	}
};

#endif
