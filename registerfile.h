#ifndef REGISTERFILE_H_INCLUDED
#define REGISTERFILE_H_INCLUDED

#include "systemc.h"
#include "mips.h"

SC_MODULE(REGFILE) {
	sc_in< sc_bv<AWORDREG> >	r_addr_reg1;
	sc_out< sc_bv<DWORD> >		r_data_reg1;

	sc_in< sc_bv<AWORDREG> >	r_addr_reg2;
	sc_out< sc_bv<DWORD> >		r_data_reg2;

	sc_in< sc_bv<AWORDREG> >	w_addr_reg;
	sc_in< sc_bv<DWORD> >		w_data_reg;
	sc_in< sc_bv<1> >		w;

	sc_in<  bool >			clk;

	sc_bv<DWORD>			rfile[REGSIZE];

	void regfile_read_thread(void);
	void regfile_write_thread(void);
	void regfile_init(void);

	SC_CTOR(REGFILE) {
		SC_THREAD(regfile_read_thread);
		sensitive << clk;
		sensitive << r_addr_reg1;
		sensitive << r_addr_reg2;
		sensitive << w_addr_reg;
		sensitive << w_data_reg;
		sensitive << w;

		SC_THREAD(regfile_write_thread);
		sensitive_neg << clk;

		regfile_init();
	} 
};

#endif
