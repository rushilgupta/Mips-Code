#include "register.h"

void
REGISTER::
reg_thread(void)
{
	// Initialize register
	data = 0;

	while (true) {
		#ifdef VERBOSE
		cerr << "REGISTER" << endl;
		#endif

		out.write(data);	// Write data to output

		#ifdef VERBOSE
		cerr << "Reg (out): " << data << endl;
		#endif

		wait();

		if(w.read()) {
			data = in.read();	// Write signal, 
						// read data from input

			#ifdef VERBOSE
			cerr << "Reg (write): " << data << endl;
			#endif
		}
	}
}
