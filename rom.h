#ifndef ROM_H_INCLUDED
#define ROM_H_INCLUDED

#include "mips.h"

SC_MODULE(ROM) {
	sc_in <	sc_bv<DWORD> >	a_read;
	sc_out< sc_bv<DWORD> >	d_read;
	sc_in< bool >		clk;
	
	sc_bv<8>* romfile;

	void rom_thread(void);
	void rom_init(const char* filename);
	void rom_dump(const char* filename);

	SC_CTOR(ROM) {
		SC_THREAD(rom_thread);
		sensitive_pos << clk;
		sensitive << a_read; 

		romfile = new sc_bv<8> [ROMSIZE];
	}
};

#endif
