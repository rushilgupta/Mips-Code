#ifndef RAM_H_INCLUDED
#define RAM_H_INCLUDED

#include "mips.h"

SC_MODULE(RAM) {
	sc_in <	sc_bv<DWORD> >		a_read;
	sc_out < sc_bv<DWORD> >		d_read;
	sc_in< sc_bv<DWORD> >		a_write;
	sc_in< sc_bv<DWORD> >		d_write;
	sc_in< sc_bv<W_MEMWRITE> >	w;
	sc_in< sc_bv<W_MEMREAD> >	r;
	sc_in< bool >			clk;

	sc_bv<8>* ramfile;

	void ram_thread(void);
	void ram_init(const char* filename);
	void ram_dump(const char* filename);

	SC_CTOR(RAM) {
		SC_THREAD(ram_thread);
		sensitive << clk;
		sensitive << a_read;
		sensitive << a_write;
		sensitive << d_write;
		sensitive << w;
		sensitive << r;
        
		ramfile = new sc_bv<8> [RAMSIZE];

	}
};

#endif
