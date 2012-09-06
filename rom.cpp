#include "rom.h"

void
ROM::
rom_thread(void)
{
	sc_uint<DWORD>	addr_t;
	sc_uint<DWORD>	data_t;
	sc_uint<DWORD>	data;
	sc_uint<1>	clk_t;

	while (true) {
		#ifdef VERBOSE
		clog << "ROM" << endl;
		#endif

		clk_t = (int)clk.read();

		if(clk_t == 1) {
			addr_t = a_read.read();
			if (addr_t < ROMSIZE) {	
				data = 0;
				#ifdef BIG_ENDIAN
				for (int i = 0; i < (DWORD / 8); i++) {
				#else	// Little-endian
				for (int i = (DWORD / 8) - 1; i >= 0; i--) {
				#endif
					data = data << 8;
					data_t = romfile[addr_t + i];
					data = data | data_t; 
				}
				d_read.write(data);
			}
		}

		wait();
	}
}

void
ROM::
rom_init(const char* filename)
{
	sc_uint<DWORD>	word;
	sc_uint<DWORD>	addr = 0;
	int c;

	ifstream romf(filename, ios::in | ios::binary);

	while (!romf.eof()) {
		word = 0;
		c = romf.get();
                if (c==-1) 
			break;
		word = c;

		romfile[addr] = word;
		addr++;

		if (addr >= ROMSIZE)
			break;
	}
}

void
ROM::
rom_dump(const char* filename)
{
	ofstream romf(filename, ios::out | ios::binary);
	char c;
	sc_uint<8> a;
	
	for (unsigned int i = 0; i < ROMSIZE; i++) {
		a = romfile[i];
		c = a;
		romf.put(c);
	}
	
	romf.close();
}
