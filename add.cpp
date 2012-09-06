#include "add.h"


void 
ADD::
add_thread(void)
{
	sc_int<DWORD> a_t;
	sc_int<DWORD> b_t;
	sc_int<DWORD> r_t;

	while(true) {
		#ifdef VERBOSE
		cerr << "ADD" << endl;
		#endif

		// Read input
		a_t = a.read();
		b_t = b.read();

		// Compute result
		r_t = a_t + b_t;

		// Put result on output
		r.write(r_t);

		// Wait for next event
		wait();
	}
}
