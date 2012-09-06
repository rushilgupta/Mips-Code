#ifndef REGISTER_H_INCLUDED 
#define REGISTER_H_INCLUDED

#include "mips.h"

SC_MODULE(REGISTER) {
	sc_in<  sc_bv<DWORD> >	in;	// Data input
	sc_in<  bool >		w;	// Write control signal
	sc_out< sc_bv<DWORD> >	out;	// Data output
	sc_in<  bool >		clk;	// Clock

	void reg_thread(void);

	sc_bv<DWORD> data;

	SC_CTOR(REGISTER) {
		SC_THREAD(reg_thread);
		sensitive_pos << clk;
	}
};

#endif
