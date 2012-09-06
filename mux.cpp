#include "mux.h"

void
MUX::
mux_thread(void)
{
	sc_bv<DWORD> data;

	while (true) {
		#ifdef VERBOSE
		cerr << "MUX" << endl;
		#endif

		// Read correct input
		if(sel.read() == 0)
			data = in0.read();
		else
			data = in1.read();

		// Write output
		out.write(data);

		// wait for new event
		wait();
	}
}

void
MUX2_AWORDREG::
mux2_thread(void)
{
	sc_bv<AWORDREG> data;

	while (true) {
		#ifdef VERBOSE
		cerr << "MUX" << endl;
		#endif

		if(sel.read() == 0)
			data = in0.read();
		else
			data = in1.read();

		out.write(data);

		// wait for new event
		wait();
	}
}
