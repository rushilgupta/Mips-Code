What kind of a MIPS is this?
    
    This directory contains the source for a mini mini MIPS (mmMIPS) described
    in Patterson and Hennessy in section 5.3. The block diagram is shown in Fig.
    5.24 of the book. A decoder is added to this. This decoder splits the 
    instruction word in the required bits for the various components in the MIPS.
    
Which MIPS operations are supported?
    
    * add sub and or slt
    * sw
    * lw
    * beq

Which endianess is used?

    Big-endian
    
Where to start using the MIPS?
    
    Set SYSTEMC directory variable in Makefile and compile the program.
    Run 'mips' to simulate the MIPS processor with the program contained
    in 'mips.bin'. An image of the initial content of the RAM is stored in 
    'mips.ram'. The program generates a VCD file 'mips.vcd' that contains all 
    signals and a file 'mips_ram.dump' that contains a image of the data memory
    after running the program. The VCD file can be viewed with every VCD viewer. 
    Use for example GTK wave. This can be downloaded from:
    http://www.linux-workshop.com/bybell/ver/wave/wave.html .
    
What program does 'mips.rom' contain?
    The dissasembled program stored in the file 'mips.rom', which is loaded into 
    the instruction memory looks as follows:
    
    lw  $1,10($0)
    add $2,$1,$0
    sub $3,$1,$2
    and $4,$2,$3
    or  $5,$2,$3
    slt $6,$4,$5
    sw  $6,10($1)
    beq $1,$0,10
    beq $2,$2,4
 
What ram image does 'mips.ram' contain?

     address   | value (decimal)                                                                
     ----------+----------------------------------------------------------------
     0000:0000 | 084 104 105 115 032 105 115 032 097 032 114 097 110 100 111 109 
     0000:0010 | 032 115 101 113 117 101 110 099 101 032 111 102 032 098 121 116 
     0000:0020 | 101 115 032 116 104 097 116 032 105 115 032 115 116 111 114 101 
     0000:0030 | 100 032 105 110 032 116 104 101 032 109 101 109 111 114 121 046 
     0000:0040 | 032 073 116 032 109 105 103 104 116 032 115 101 101 109 115 032  
     0000:0050 | 115 116 114 097 110 103 101 044 032 098 117 116 032 105 115 032  
     0000:0060 | 105 115 032 110 111 116 032 058 045 041 032 073 032 104 111 112 
     0000:0070 | 101 032 121 111 117 032 115 101 101 032 116 104 097 116 046 046 

What is the size of the data and instruction memory?

    The sizes of the memories are defined in 'mips.h' and are default 128 bytes.
