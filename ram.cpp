#include "ram.h"
#define RAM_BITMASK 0xffffffff

void
RAM::
ram_thread(void)
{
	sc_uint<DWORD>		addr_t;
	sc_uint<DWORD>		data_t;
	sc_uint<W_MEMWRITE>	w_t;
	sc_uint<W_MEMREAD>	r_t;
	sc_uint<DWORD>		data;

	while (true) {
		#ifdef VERBOSE
		clog << "RAM" << endl;
		#endif

		if (clk.read() == 1) {
			// Check that data must be read from mem
			r_t = r.read();

			if(r_t == 1) {		// Read word
				addr_t = a_read.read();
				addr_t = addr_t & RAM_BITMASK;

				// Check that address is word aligned, 
				// if not issue a warning
				if ((addr_t & 0x3) != 0) {
					cout << "WARNING: memory address ("; 
					cout << addr_t.to_string(SC_HEX);
					cout << ") not word aligned in read operation.";
					cout << endl;
				}

				// Word align address
				addr_t = addr_t & ~(0x3);

				if (addr_t < RAMSIZE) {	
					data = 0;
					#ifdef BIG_ENDIAN
					for (int i = 0; i < (DWORD / 8); i++) {
					#else // Little-endian
					for (int i = (DWORD / 8) - 1; i >= 0; i--) {
					#endif
						data = data << 8;
						data_t = ramfile[addr_t + i];
						data = data | data_t; 
					}
					d_read.write(data);
				}
			}
			else if(r_t == 2) {	// Read byte
				addr_t = a_read.read();
				addr_t = addr_t & RAM_BITMASK;

				if (addr_t < RAMSIZE) {	
					data = 0;
					#ifdef BIG_ENDIAN
					for (int i = 0; i < (DWORD / 8); i++) {
					#else // Little-endian
					for (int i = (DWORD / 8) - 1; i >= 0; i--) {
					#endif
						data = data << 8;
						data_t = ramfile[addr_t + i];
						data = data | data_t; 
					}
					d_read.write(data);
				}
			}
		}
        	if (clk.read() == 0) {
			// Check that data must be written to mem
			w_t = w.read();

			if(w_t == 1) {	// write word to ram
				addr_t = a_write.read();
				addr_t = addr_t & RAM_BITMASK;

				// Check that address is word aligned, 
				// if not issue a warning
				if ((addr_t & 0x3) != 0) {
					cout << "WARNING: memory address ("; 
					cout << addr_t.to_string(SC_HEX);
					cout << ") not word aligned in write operation.";
					cout << endl;
				}
				// Word align address
				addr_t = addr_t & ~(0x3);
				data = d_write.read();

				if (addr_t < RAMSIZE) {
					#ifdef BIG_ENDIAN
					for (int i = (DWORD / 8); i > 0; i--) {
						ramfile[addr_t + i - 1] = data & 0xff;
					#else	// Little-endian
					for (int i = 0; i < (DWORD / 8); i++) {
						ramfile[addr_t + i] = data & 0xff;
					#endif
						data = data >> 8;
					}
				}
				else {
					cout << "WARNING: writting to invalid memory location. (";
					cout << addr_t << ")" << endl;
				}
			}
			else if (w_t == 2) {	// write byte to ram
				addr_t = a_write.read();
				addr_t = addr_t & RAM_BITMASK;
				data = d_write.read();

				if (addr_t < RAMSIZE) {
					ramfile[addr_t] = data & 0xff;
				}
				else {
					cout << "WARNING: writting to invalid memory location. (";
					cout << addr_t << ")" << endl;
				}
			} 
		}

		wait();
	}
}

void
RAM::
ram_init(const char *filename)
{
	sc_uint<DWORD> word;
	sc_uint<DWORD> addr = 0;
	int c;

	ifstream ramf(filename, ios::in | ios::binary);

	while (!ramf.eof()) {
		word = 0;

		c = ramf.get();
                if (c==-1) 
			break;
		word = c;

		ramfile[addr] = word;
		addr++;

		if (addr >= RAMSIZE)
			break;
	}
}

void
RAM::
ram_dump(const char *filename)
{
	ofstream ramf(filename, ios::out | ios::binary);
	char c;
	sc_uint<8> a;
	
	for (unsigned int i = 0; i < RAMSIZE; i++) {
		a = ramfile[i];
		c = a;
		ramf.put(c);
	}
	
	ramf.close();
}

