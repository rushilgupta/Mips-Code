#ifndef SIGNEXTEND2_H_INCLUDED
#define SIGNEXTEND2_H_INCLUDED

#include "mips.h"

SC_MODULE(SIGNEXTEND2) {
	sc_in< sc_bv<28> >	in;
	sc_out< sc_bv<32> >		out;

	void signextend2_thread(void);

	// Constructor
	SC_CTOR(SIGNEXTEND2) {
		SC_THREAD(signextend2_thread);
		sensitive << in;
	}
};

#endif
