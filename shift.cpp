#include "shift.h"

void
SHIFTRIGHT::
shiftright_thread(void)
{
	sc_bv<DWORD> a;
	sc_bv<DWORD> b;

	while (true) {
		#ifdef VERBOSE
		cerr << "SHIFTRIGHT" << endl;
		#endif

		a = in.read();
		b = a >> SHIFTBIT;
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

void
SHIFTLEFT::
shiftleft_thread(void)
{
	sc_bv<DWORD> a;
	sc_bv<DWORD> b;

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
