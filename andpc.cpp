#include "andpc.h"

void
ANDPC::
andpc_thread(void)
{
	sc_bv<32> a_t;
	sc_bv<32> r_t;

	while(true) {
		#ifdef VERBOSE
		cerr << "AND" << endl;
		#endif

		// Read inputs
		a_t = a.read();

		// Compute result
		r_t = a_t & 4026531840;  //(2^28)*15

		// Set result on output
		r.write(r_t);

		// Wait for new event
		wait();
	}
}
