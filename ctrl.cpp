#include "ctrl.h"

void
CTRL::
ctrl_thread(void)
{
	sc_uint<6> opcode;

	// Write constant 4 to output
	c4.write(0x4);

	while (true) {
		#ifdef VERBOSE
		cerr << "CTRL" << endl;
		#endif

		// Read the input
		opcode = Opcode.read();

		// Determine output
		switch (opcode) {
			case OP_RTYPE:	// R-format instruction
				RegDst.write(1);
				ALUSrc.write(0);
				MemtoReg.write(0);
				RegWrite.write(1);
				MemRead.write(0);
				MemWrite.write(0);
				Branch.write(0);
				ALUop.write(0x2);
				Jump.write(0);
				break;
			case OP_LW:	// lw instruction
				RegDst.write(0);
				ALUSrc.write(1);
				MemtoReg.write(1);
				RegWrite.write(1);
				MemRead.write(1);
				MemWrite.write(0);
				Branch.write(0);
				ALUop.write(0);
				Jump.write(0);
				break;
			case OP_SW:	// sw instruction
				RegDst.write(0);
				ALUSrc.write(1);
				MemtoReg.write(0);
				RegWrite.write(0);
				MemRead.write(0);
				MemWrite.write(1);
				Branch.write(0);
				ALUop.write(0);
				Jump.write(0);
				break;
			case OP_BEQ:	// beq instruction
				RegDst.write(0);
				ALUSrc.write(0);
				MemtoReg.write(0);
				RegWrite.write(0);
				MemRead.write(0);
				MemWrite.write(0);
				Branch.write(1);
				ALUop.write(0x1);
				Jump.write(0);
				break;
			case OP_J:	// jump instruction
				RegDst.write(0);
				ALUSrc.write(0);
				MemtoReg.write(0);
				RegWrite.write(0);
				MemRead.write(0);
				MemWrite.write(0);
				Branch.write(0);
				ALUop.write(0x1);
				Jump.write(1);
				break;
			case OP_ADDI:			// add immediate instruction
				RegDst.write(0);	// register destination is 0 not 1 even though add
				ALUSrc.write(1);	// alusrc is 1 in case of add immedite
				MemtoReg.write(0);	// but mem2reg is still 0, alu result not mem result
				RegWrite.write(1);	// write destination
				MemRead.write(0);
				MemWrite.write(0);
				Branch.write(0);
				ALUop.write(0);		// ALUop= 0 only ALU instruction is add
				Jump.write(0);
				break;
		}

		// wait for new event
		wait(); 
	}
} 
