#ifndef SHIFTJ_H_INCLUDED
#define SHIFTJ_H_INCLUDED

#include "mips.h"


SC_MODULE(SHIFTLEFT2) {
	sc_in< sc_bv<26> >	in;
	sc_out< sc_bv<28> >	out;

	void shiftleft2_thread(void);

	SC_CTOR(SHIFTLEFT2) {
		SC_THREAD(shiftleft2_thread);
		sensitive << in;
	}
};

#endif
