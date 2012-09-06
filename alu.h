#ifndef ALU_H_INCLUDED
#define ALU_H_INCLUDED

#include "mips.h"
 
SC_MODULE(ALU) {
	sc_in<  sc_bv<3> >	ctrl;	// Control input
	sc_in<  sc_bv<DWORD> >	a;	// First operand input 
	sc_in<  sc_bv<DWORD> >	b;	// Second operand input
	sc_out< sc_bv<DWORD> >	r;	// Result
	sc_out< sc_bv<1> >	z;	// Zero (one in case result equal zero)

	void alu_thread(void);

	// Constructor
	SC_CTOR(ALU) {
		SC_THREAD(alu_thread);
		sensitive << ctrl;
		sensitive << a;
		sensitive << b;
	}
};

#endif
