#include "add.h"
#include "aluctrl.h"
#include "alu.h"
#include "and.h"
#include "ctrl.h"
#include "decoder.h"
#include "mux.h"
#include "ram.h"
#include "rom.h"
#include "registerfile.h"
#include "register.h"
#include "shift.h"
#include "shiftj.h"
#include "signextend.h"
#include "signextend2.h"
#include "andpc.h"
int
sc_main(int argc, char *argv[])
{
	sc_report_handler::
	set_actions("/IEEE_Std_1666/deprecated", SC_DO_NOTHING);

	char stbuf[256];

	// SIGNALS

	// Data signals
	sc_signal< sc_bv<DWORD> >		bus_mux1;
	sc_signal< sc_bv<DWORD> >		bus_mux2;
	sc_signal< sc_bv<DWORD> >		bus_mux3;
	sc_signal< sc_bv<AWORDREG> >		bus_mux4;
	sc_signal< sc_bv<DWORD> >		bus_mux5;

	sc_signal< sc_bv<6> >			bus_decoder_instr_31_26;
	sc_signal< sc_bv<AWORDREG> >		bus_decoder_instr_25_21;
	sc_signal< sc_bv<AWORDREG> >		bus_decoder_instr_20_16;
	sc_signal< sc_bv<AWORDREG> >		bus_decoder_instr_15_11;
	sc_signal< sc_bv<SIGNEXTENDBIT> >	bus_decoder_instr_15_0;
	sc_signal< sc_bv<6> >			bus_decoder_instr_5_0;
	sc_signal< sc_bv<26> >			bus_decoder_instr_25_0;   //jump

	sc_signal< sc_bv<DWORD> >		bus_pc;

	sc_signal< sc_bv<DWORD> >		bus_add1;
	sc_signal< sc_bv<DWORD> >		bus_add2;

	sc_signal< sc_bv<DWORD> >		bus_shiftleft;
	sc_signal< sc_bv<28> >			bus_shiftleft2;   // output of 26-28 shifting
	sc_signal< sc_bv<32> >			bus_sx;   // output of 26-28 shifting
	sc_signal< sc_bv<DWORD> >		bus_pcx;	// upper 4 bits
	sc_signal< sc_bv<DWORD> >		bus_jadd;	// jump address

	sc_signal< sc_bv<DWORD> >		bus_signextend;

	sc_signal< sc_bv<DWORD> >		bus_imem_1;

	sc_signal< sc_bv<DWORD> >		bus_dmem_1;

	sc_signal< sc_bv<DWORD> >		bus_alu_result;
	sc_signal< sc_bv<1> >			bus_alu_zero;

	sc_signal< sc_bv<DWORD> >		bus_registers_1;
	sc_signal< sc_bv<DWORD> >		bus_registers_2;

	// Control signals
	sc_signal< sc_bv<1> >		bus_ctrl_regdst;
	sc_signal< sc_bv<1> >		bus_ctrl_branch;
	sc_signal< sc_bv<1> >		bus_ctrl_memread;
	sc_signal< sc_bv<1> >		bus_ctrl_memtoreg;
	sc_signal< sc_bv<2> >		bus_ctrl_aluop;
	sc_signal< sc_bv<1> >		bus_ctrl_memwrite;
	sc_signal< sc_bv<1> >		bus_ctrl_alusrc;
	sc_signal< sc_bv<1> >		bus_ctrl_regwrite;
	sc_signal< sc_bv<1> >		bus_ctrl_jump;     //jump control
	sc_signal< sc_bv<DWORD> >	bus_ctrl_c4;

	sc_signal< sc_bv<3> >		bus_aluctrl;

	sc_signal< sc_bv<1> >		bus_and1;
	// MODULES

	REGISTER	pc("pc");
                            
	ADD		add1("add1");
	ADD		add2("add2");
	ADD		add3("add3");		// adding sx and pcx concatenating

	AND		and1("and1");
	ANDPC		andpc("andpc");

	ROM		imem("instruction_memory");	// Instruction memory
	RAM		dmem("data_memory");		// Data memory

	REGFILE		registers("registers");		// Registerfile

	ALU		alu("alu");
	ALUCTRL		aluctrl("aluctrl");

	SIGNEXTEND	signextend("signextend");
	SIGNEXTEND2	signextend2("signextend2");

	SHIFTLEFT	shiftleft("shiftleft");
	SHIFTLEFT2	shiftleft2("shiftleft2");	// shiftleft 26-28

	CTRL		ctrl("ctrl");

	DECODER		decoder("decoder");

	MUX		mux1("mux1");
	MUX		mux2("mux2");
	MUX		mux3("mux3");
	MUX2_AWORDREG	mux4("mux4");
	MUX		mux5("mux5");

	sc_clock	clk("clock", 20);		// Clock

	// CONNECTIONS

	// Program counter
	pc.in(bus_mux5);
	pc.out(bus_pc);
	pc.w(clk);
	pc.clk(clk);

	// Add 1 (PC + 4)
	add1.a(bus_pc);
	add1.b(bus_ctrl_c4);
	add1.r(bus_add1);

	// Add 2 (add1 + shiftleft)
	add2.a(bus_add1);
	add2.b(bus_shiftleft);
	add2.r(bus_add2);

	// Mux 1 (add1 or add2)
	mux1.in0(bus_add1);
	mux1.in1(bus_add2);
	mux1.sel(bus_and1);
	mux1.out(bus_mux1);

	// Shift left 2
	shiftleft.in(bus_signextend);
	shiftleft.out(bus_shiftleft);

	// Shift leftII 2				to enable 26-28 in jump
	shiftleft2.in(bus_decoder_instr_25_0);
	shiftleft2.out(bus_shiftleft2);

	// Sign extend
	signextend.in(bus_decoder_instr_15_0);
	signextend.out(bus_signextend);

	// Sign extend2					// 28-32
	signextend2.in(bus_shiftleft2);
	signextend2.out(bus_sx);
	
	// AND 1111 higher bits to PC
	andpc.a(bus_add1);
	andpc.r(bus_pcx);

	// Add 2 (concat pcx and sx)
	add3.a(bus_sx);
	add3.b(bus_pcx);
	add3.r(bus_jadd);				// jump address 0-31

	// MUX5: use mux1 input to multiplex between jump and branch
	mux5.in0(bus_mux1);
	mux5.in1(bus_jadd);
	mux5.sel(bus_ctrl_jump);
	mux5.out(bus_mux5);
	

	// Decoder (Select correct part of instruction for registerfile)
	decoder.instr(bus_imem_1);
	decoder.instr_31_26(bus_decoder_instr_31_26);
	decoder.instr_25_21(bus_decoder_instr_25_21);
	decoder.instr_20_16(bus_decoder_instr_20_16);
	decoder.instr_15_11(bus_decoder_instr_15_11);
	decoder.instr_15_0(bus_decoder_instr_15_0);
	decoder.instr_5_0(bus_decoder_instr_5_0);
	decoder.instr_25_0(bus_decoder_instr_25_0);

	// Mux 4 (Select address for write to registerfile)
	mux4.in0(bus_decoder_instr_20_16);
	mux4.in1(bus_decoder_instr_15_11);
	mux4.sel(bus_ctrl_regdst);
	mux4.out(bus_mux4);

	// ALU
	alu.a(bus_registers_1);
	alu.b(bus_mux2);
	alu.r(bus_alu_result);
	alu.z(bus_alu_zero);
	alu.ctrl(bus_aluctrl);

	// Mux 2 (Registerfile or signextend)
	mux2.in0(bus_registers_2);
	mux2.in1(bus_signextend);
	mux2.sel(bus_ctrl_alusrc);
	mux2.out(bus_mux2);

	// ALU ctrl
	aluctrl.ALUop(bus_ctrl_aluop);
	aluctrl.functionCode(bus_decoder_instr_5_0);
	aluctrl.ALUctrl(bus_aluctrl);

	// Mux 3 (ALU result or memory result to register)
	mux3.in0(bus_alu_result);
	mux3.in1(bus_dmem_1);
	mux3.sel(bus_ctrl_memtoreg);
	mux3.out(bus_mux3);

	// AND
	and1.a(bus_alu_zero);
	and1.b(bus_ctrl_branch);
	and1.r(bus_and1);

	// Registerfile
	registers.r_addr_reg1(bus_decoder_instr_25_21);
	registers.r_addr_reg2(bus_decoder_instr_20_16);
	registers.w_addr_reg(bus_mux4);
	registers.r_data_reg1(bus_registers_1);
	registers.r_data_reg2(bus_registers_2);
	registers.w_data_reg(bus_mux3);
	registers.w(bus_ctrl_regwrite);
	registers.clk(clk);

	// Data memory
	dmem.a_read(bus_alu_result);
	dmem.d_read(bus_dmem_1);
	dmem.r(bus_ctrl_memread);
	dmem.a_write(bus_alu_result);
	dmem.d_write(bus_registers_2);
	dmem.w(bus_ctrl_memwrite);
	dmem.clk(clk);

	// Instruction Memory 
	imem.a_read(bus_pc);
	imem.d_read(bus_imem_1);
	imem.clk(clk);

	// Controller
	ctrl.Opcode(bus_decoder_instr_31_26);
	ctrl.RegDst(bus_ctrl_regdst);
	ctrl.Branch(bus_ctrl_branch);
	ctrl.MemRead(bus_ctrl_memread);
	ctrl.MemtoReg(bus_ctrl_memtoreg);
	ctrl.ALUop(bus_ctrl_aluop);
	ctrl.MemWrite(bus_ctrl_memwrite);
	ctrl.ALUSrc(bus_ctrl_alusrc);
	ctrl.RegWrite(bus_ctrl_regwrite);
	ctrl.Jump(bus_ctrl_jump);
	ctrl.c4(bus_ctrl_c4);

	// INITIALIZATION

	imem.rom_init("mips.rom"); 
	dmem.ram_init("mips.ram");

	// TRACING

	sc_trace_file* tf;
	tf = sc_create_vcd_trace_file("mips");

	// Signals
	sc_trace(tf, clk.signal(),		"clock");
	sc_trace(tf, bus_mux1,			"bus_mux1");
	sc_trace(tf, bus_mux2,			"bus_mux2");
	sc_trace(tf, bus_mux3,			"bus_mux3");
	sc_trace(tf, bus_mux4,			"bus_mux4");
	sc_trace(tf, bus_pc,			"bus_pc");
	sc_trace(tf, bus_add1,			"bus_add1");
	sc_trace(tf, bus_add2,			"bus_add2");
	sc_trace(tf, bus_shiftleft,		"bus_shiftleft");
	sc_trace(tf, bus_signextend,		"bus_signextend");
	sc_trace(tf, bus_imem_1,		"bus_imem_1");
	sc_trace(tf, bus_dmem_1,		"bus_dmem_1");
	sc_trace(tf, bus_alu_result,		"bus_alu_result");
	sc_trace(tf, bus_alu_zero,		"bus_alu_zero");
	sc_trace(tf, bus_registers_1,		"bus_registers_1");
	sc_trace(tf, bus_registers_2,		"bus_registers_2");
	sc_trace(tf, bus_ctrl_regdst,		"bus_ctrl_regdst");
	sc_trace(tf, bus_ctrl_branch,		"bus_ctrl_branch");
	sc_trace(tf, bus_ctrl_memread,		"bus_ctrl_memread");
	sc_trace(tf, bus_ctrl_memtoreg,		"bus_ctrl_memtoreg");
	sc_trace(tf, bus_ctrl_aluop,		"bus_ctrl_aluop");
	sc_trace(tf, bus_ctrl_memwrite,		"bus_ctrl_memwrite");
	sc_trace(tf, bus_ctrl_alusrc,		"bus_ctrl_alusrc");
	sc_trace(tf, bus_ctrl_regwrite,		"bus_ctrl_regwrite");
	sc_trace(tf, bus_ctrl_c4,		"bus_ctrl_c4");
	sc_trace(tf, bus_aluctrl,		"bus_aluctrl");
	sc_trace(tf, bus_and1,			"bus_and1");
	sc_trace(tf, bus_decoder_instr_31_26,	"bus_decoder_instr_31_26");
	sc_trace(tf, bus_decoder_instr_25_21,	"bus_decoder_instr_25_21");
	sc_trace(tf, bus_decoder_instr_20_16,	"bus_decoder_instr_20_16");
	sc_trace(tf, bus_decoder_instr_15_11,	"bus_decoder_instr_15_11");
	sc_trace(tf, bus_decoder_instr_15_0,	"bus_decoder_instr_15_0");
	sc_trace(tf, bus_decoder_instr_5_0,	"bus_decoder_instr_5_0");

	for (int i = 0; i < REGSIZE; i++) {
		sprintf(stbuf, "registers.reg(%d)", i);
		sc_trace(tf, registers.rfile[i], stbuf);
	}

	for (int i = 0; i < RAMSIZE; i++) {
		sprintf(stbuf, "memory.dmem(%d)", i);
		sc_trace(tf, dmem.ramfile[i], stbuf);
	}

	for (int i = 0; i < ROMSIZE; i++) {
		sprintf(stbuf, "memory.imem(%d)", i);
		sc_trace(tf, imem.romfile[i], stbuf);
	}

	// SIMULATION

	unsigned int sim_time = 500;
	if (argc == 2)
		sim_time = atoi(argv[1]);

	sc_start(sim_time);

	sc_close_vcd_trace_file(tf);
	dmem.ram_dump("mips_ram.dump");

	return 0;
}
