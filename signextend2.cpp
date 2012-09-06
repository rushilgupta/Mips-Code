#include "signextend2.h"

void
SIGNEXTEND2::
signextend2_thread(void)
{
	sc_bv<28>	a;
	sc_bv<32>	b;

	while (true) {
		#ifdef VERBOSE
		cerr << "SIGNEXTEND" << endl;
		#endif

		a = in.read();

		b = a;

		for (unsigned int i = 28; i < 32; i++)			// converting 28 bit jump address to 32 bit to further concatenate 4 bits of PC
			b[i] = a[28 - 1];

		out.write(b);

		wait();
	}
}
