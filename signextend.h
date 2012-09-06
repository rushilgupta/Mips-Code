#ifndef SIGNEXTEND_H_INCLUDED
#define SIGNEXTEND_H_INCLUDED

#include "mips.h"

SC_MODULE(SIGNEXTEND) {
	sc_in< sc_bv<SIGNEXTENDBIT> >	in;
	sc_out< sc_bv<DWORD> >		out;

	void signextend_thread(void);

	// Constructor
	SC_CTOR(SIGNEXTEND) {
		SC_THREAD(signextend_thread);
		sensitive << in;
	}
};

#endif
