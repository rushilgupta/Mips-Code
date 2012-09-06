#include "shiftj.h"

void
SHIFTLEFT2::
shiftleft2_thread(void)
{
	sc_bv<26> a;
	sc_bv<28> b;

	while (true) {
		#ifdef VERBOSE
		cerr << "SHIFT" << endl;
		#endif

		a = in.read();
		b = a << SHIFTBIT;
		wait(1, SC_NS);
		out.write(b);

		#ifdef VERBOSE
		cerr << "Shifter: " << a;
		cerr << " -> ";
		cerr << b << endl;
		#endif

		wait();
	}
}
