#ifndef AND_H_INCLUDED
#define AND_H_INCLUDED

#include "mips.h"

SC_MODULE(AND) {
	sc_in<  sc_bv<1> > a;	// First operand
	sc_in<  sc_bv<1> > b;	// Second operand
	sc_out< sc_bv<1> > r;	// Result

	void and_thread(void);

	// Constructor
	SC_CTOR(AND) {
		SC_THREAD(and_thread);
		sensitive << a;
		sensitive << b;
	}
};

#endif
