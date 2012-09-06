#include "and.h"

void
AND::
and_thread(void)
{
	sc_bv<1> a_t;
	sc_bv<1> b_t;
	sc_bv<1> r_t;

	while(true) {
		#ifdef VERBOSE
		cerr << "AND" << endl;
		#endif

		// Read inputs
		a_t = a.read();
		b_t = b.read();

		// Compute result
		r_t = a_t & b_t;

		// Set result on output
		r.write(r_t);

		// Wait for new event
		wait();
	}
}
