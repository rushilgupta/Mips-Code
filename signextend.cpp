#include "signextend.h"

void
SIGNEXTEND::
signextend_thread(void)
{
	sc_bv<SIGNEXTENDBIT>	a;
	sc_bv<DWORD>		b;

	while (true) {
		#ifdef VERBOSE
		cerr << "SIGNEXTEND" << endl;
		#endif

		a = in.read();

		b = a;

		for (unsigned int i = SIGNEXTENDBIT; i < DWORD; i++)
			b[i] = a[SIGNEXTENDBIT - 1];

		out.write(b);

		wait();
	}
}
