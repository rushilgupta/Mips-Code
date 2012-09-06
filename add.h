#ifndef ADD_H_INCLUDED
#define ADD_H_INCLUDED


#include "mips.h"

SC_MODULE(ADD) {
	sc_in<  sc_bv<DWORD> >  a;  // First operand
	sc_in<  sc_bv<DWORD> >  b;  // Second operand
	sc_out< sc_bv<DWORD> >  r;  // Result

	void add_thread(void);
    
	// Constructor
	SC_CTOR(ADD) {
		SC_THREAD(add_thread);
		sensitive << a;
		sensitive << b;
	}
};

#endif
