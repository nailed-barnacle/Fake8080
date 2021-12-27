// 8080 cycle by cycle operation codes
// to be placed in a nice wide ROM with lots of signal outputs
//
// bits		name		function
// 0-3		src			the source of a transfer
// 4-7		dest		the destination of a transfer
// 8-9		add			which register pair drives the address bus
// 10-13	maths		the desired alu funtion
// 14		carry		we want the carry to be stored
// 15		zerosign	we want the zero and sign to be stored
// 18		intc		do we want to see the C or IC output from the flags
// 19		nzero		select zero or middle three bits of rst instruction to alu output mpx
// 20		rst			select rst mpx to alu output
// 21		inton		true to enable interrupts
// 22		intoff		true to disable interrupts
// 31		last		this is the last cycle of an instruction


// source registers - bits 0-3
#define S_B 	0
#define S_C 	1
#define S_D 	2
#define S_E 	3
#define S_H 	4
#define S_L 	5
#define S_M 	6
#define S_A 	7
#define S_PCH 	8
#define S_PCL 	9
#define S_SPH 	10
#define S_SPL 	11
#define S_MAH 	12
#define S_MAL 	13
#define S_FLAG 	14
#define S_IR	15

// destination registers - bits 4-7
#define D_B 	(0 << 4)
#define D_C 	(1 << 4)
#define D_D 	(2 << 4)
#define D_E 	(3 << 4)
#define D_H 	(4 << 4)
#define D_L 	(5 << 4)
#define D_M 	(6 << 4)
#define D_A 	(7 << 4)
#define D_PCH 	(8 << 4)
#define D_PCL 	(9 << 4)
#define D_SPH 	(10 << 4)
#define D_SPL 	(11 << 4)
#define D_MAH 	(12 << 4)
#define D_MAL 	(13 << 4)
#define D_FLAG 	(14 << 4)
#define D_IR	(15 << 4)

// address source - bits 8-9
#define	HL		(0 << 8)
#define PC		(1 << 8)
#define SP		(2 << 8)
#define MA		(3 << 8)

// arithmetic operations - bits 10-13
#define ADDOP	(0 << 10)
#define ADCOP	(1 << 10)
#define	SUBOP	(2 << 10)
#define SBBOP	(3 << 10)
#define	ANDOP	(4 << 10)
#define	XOROP	(5 << 10)
#define OROP	(6 << 10)
#define CMPOP	(7 << 10)
#define INCLOP	(8 << 10)
#define INCHOP 	(9 << 10)
#define DECLOP	(10 << 10)
#define DECHOP	(11 << 10)
#define RAROP	(12 << 10)
#define RRCOP	(13 << 10)
#define ZEROOP	(14 << 10)
#define BYPASS	(15 << 10)

// flags
#define	CARRYF	(1 << 14)
#define ZSF		(1 << 15)		// zero and sign flags are always set together

// interrupts
#define INTON	(1 << 16)
#define INTOFF	(1 << 17)

// carry set and complement
#define STC		(1 << 18)
#define CMC		(1 << 19)

// xchg flip bit for de-hl swap
#define XCHG	(1 << 20)




#define LAST	(1 << 31)


// now we make constants for individual operations
//
// an operation requires one memory address; thirty-two operations are permitted per instruction
// instructions are (generally) repeated twice in a sixty-four address block; an address input is
// false if a flag matches a selected flag in a conditional instruction and high if not (for non-
// conditional instructions the input could be either)
//
// every instruction must begin by loading the IR
//
// in general, the bits that make up the control word can be ignored if not required; unused equates
// will contribute zero bitss to the word
//
// SRC		DEST	ADDR	ALUOP	FLAGS
//
// these are a mix of individual instructions and generic things that we might use lots of
//
// add LAST if an operation the last in the instruction sequence
// this first block is for frequently used constants

#define	LD_IR		S_M + D_IR + PC + BYPASS
#define INC_PCL		S_PCL +	D_PCL + PC + INCLOP
#define INC_PCH		S_PCH +	D_PCH +	PC + INCHOP
#define INC_MAL		S_MAL + D_MAL + PC + INCLOP
#define INC_MAH		S_MAH + D_MAH + PC + INCHOP
#define INC_SPL		S_SPL + D_SPL + PC + INCLOP
#define INC_SPH		S_SPH + D_SPH + PC + INCHOP
#define DEC_SPL		S_SPL + D_SPL + PC + DECLOP
#define DEC_SPH		S_SPH + D_SPH + PC + DECHOP
#define M2MAL		S_M + D_MAL + PC + BYPASS
#define M2MAH		S_M + D_MAH + PC + BYPASS

#define PCHL1		S_H + D_PCH + PC + BYPASS			// copy HL to PC
#define PCHL2		S_L + D_PCL	+ PC + BYPASS

#define XTHL1		S_H + D_MAH + PC + BYPASS			// swap HL and the top of stack
#define XTHL2		S_L + D_MAL + PC + BYPASS

/*
// the slow way
#define XCHG1		S_H + D_MAH + PC + BYPASS			// swap DE and HL (via MA)
#define XCHG2		S_L + D_MAL + PC + BYPASS
#define XCHG3		S_D + D_H + PC + BYPASS
#define XCHG4		S_E + D_L + PC + BYPASS
#define XCHG5		S_MAH + D_D + PC + BYPASS
#define XCHG6		S_MAL + D_E + PC + BYPASS
*/
// the quick wat
#define XCHG1		S_A + D_MAH + PC + BYPASS + XCHG

// alu immediate
#define ADI			S_M + D_A + PC + ADDOP + CARRYF + ZSF
#define ACI			S_M + D_A + PC + ADCOP + CARRYF + ZSF
#define SUI			S_M + D_A + PC + SUBOP + CARRYF + ZSF
#define SBI			S_M + D_A + PC + SBBOP + CARRYF + ZSF
#define ANI			S_M + D_A + PC + ANDOP + CARRYF + ZSF
#define XRI			S_M + D_A + PC + XOROP + CARRYF + ZSF
#define ORI			S_M + D_A + PC + OROP  + CARRYF + ZSF
#define CPI			S_M + D_MAH + PC + CMPOP + CARRYF + ZSF

// jmp absolute
#define JMP1		M2MAH								// low byte target address to MAH
#define JMP2		S_M + D_PCH + PC + BYPASS			// high byte straight to PCH
#define JMP3		S_MAH + D_PCL + PC + BYPASS			// and low byte to PCL


// call/rst
#define CALL1		S_MAH + D_PCH + PC + BYPASS
#define CALL2		S_MAL + D_PCL + PC + BYPASS

#define RST1		S_A + D_PCH + PC + ZEROOP
#define RST2		S_IR + D_PCL + PC + BYPASS			// sets restart vector to 0000 0000 00xx x000 where xxx is the rst number

// push and pop
#define PUSHB		S_B + D_M + SP + BYPASS
#define PUSHC		S_C + D_M + SP + BYPASS
#define PUSHD		S_D + D_M + SP + BYPASS
#define PUSHE		S_E + D_M + SP + BYPASS
#define PUSHH		S_H + D_M + SP + BYPASS
#define PUSHL		S_L + D_M + SP + BYPASS
#define PUSHA		S_A + D_M + SP + BYPASS
#define PUSHF		S_FLAG + D_M + SP + BYPASS
#define PUSHPCL		S_PCL + D_M + SP + BYPASS
#define PUSHPCH		S_PCH + D_M + SP + BYPASS
#define PUSHMAL		S_MAL + D_M + SP + BYPASS
#define PUSHMAH		S_MAH + D_M + SP + BYPASS

#define POPB		S_M + D_B + SP + BYPASS
#define POPC		S_M + D_C + SP + BYPASS
#define POPD		S_M + D_D + SP + BYPASS
#define POPE		S_M + D_E + SP + BYPASS
#define POPH		S_M + D_H + SP + BYPASS
#define POPL		S_M + D_L + SP + BYPASS
#define POPA		S_M + D_A + SP + BYPASS
#define POPF		S_M + D_FLAG + SP + BYPASS
#define POPPCL		S_M + D_PCL + SP + BYPASS
#define POPPCH		S_M + D_PCH + SP + BYPASS
#define POPMAL		S_M + D_MAL + SP + BYPASS
#define POPMAH		S_M + D_MAH + SP + BYPASS


/*
// set and complement carry
// my approach is extremely slow - six and seven clocks - while the 8080 does it in four clocks
// I suspect the designers used a JK latch for the carry, in which case a single bit would either
// set it (STC) or toggle it (CMC) but for now I will explicitly perform operations which set
// or toggle the carry. They're a bit sneaky, and slow :(

#define STC1		S_MAH + D_MAH + PC + ZEROOP			// get a zero
#define STC2		S_MAH + D_MAH + PC + DECLOP			// turn it into minus one
#define STC3		S_MAH + D_MAH + PC + ADDOP + CARRYF	// and add it to itself to set the carry... slow, but doesn't
														// require a direct control to the carry flip-flop

#define CMC1		S_A + D_MAH + PC + BYPASS			// save the accumulator: complementing the carry is even slower
#define CMC2		S_FLAG + D_A + PC + INCLOP			// carry is bit one; incrementing inverts bit one
#define CMC3		S_A + D_A + PC + RAROP + CARRYF		// shift that bit into the carry flag
#define CMC4		S_MAH + D_A + PC + BYPASS			// and restore the accumulator
*/

// or the hardware version
#define STC1		S_A + D_MAH + PC + STC + CARRYF
#define CMC1		S_A + D_MAH + PC + CMC + CARRYF


// absolute store and load
#define LDA			S_M + D_A + MA + BYPASS
#define STA			S_A + D_M + MA + BYPASS

// cma				one's complement of accumulator, doesn't affect flags - can't do it in one :(
#define CMA1		S_A + D_MAH + PC + ZEROOP			// put a zero in mah
#define CMA2		S_MAH + D_MAH + PC + DECLOP			// make it 0xff
#define CMA3		S_MAH + D_A + PC + XOROP			// and invert the acc

// shld and lhld
#define SHLD1		S_L + D_M + MA + BYPASS
#define SHLD2		S_H + D_M + MA + BYPASS
#define LHLD1		S_M + D_L + MA + BYPASS
#define LHLD2		S_M + D_H + MA + BYPASS

// dad - only affect carry flag
// sadly, we can only add a register to the accumulator, not another accumulator, so we have to move things around
#define DAD1		S_A + D_MAH + PC + BYPASS			// save the accumulator
#define DADB2		S_L + D_A + PC + BYPASS				// put L in instead
#define DADB3		S_C + D_L + PC + ADDOP	+ CARRYF	// add low reg to L - we need to maintain carry
#define DADB4		S_H + D_A + PC + BYPASS				// now get H in Acc
#define DADB5		S_B + D_H + PC + ADCOP	+ CARRYF	// result in HL
#define DAD6		S_MAH + D_A + PC + BYPASS			// and return the accumulator from exile

#define DADD2		S_L + D_A + PC + BYPASS				// put L in instead
#define DADD3		S_E + D_L + PC + ADDOP	+ CARRYF	// add low reg to L - we need to maintain carry
#define DADD4		S_H + D_A + PC + BYPASS				// now get H in Acc
#define DADD5		S_D + D_H + PC + ADCOP	+ CARRYF	// result in HL

#define DADH2		S_L + D_A + PC + BYPASS				// put L in instead
#define DADH3		S_L + D_L + PC + ADDOP	+ CARRYF	// add low reg to L - we need to maintain carry
#define DADH4		S_H + D_A + PC + BYPASS				// now get H in Acc
#define DADH5		S_H + D_H + PC + ADCOP	+ CARRYF	// result in HL

#define DADSP2		S_L + D_A + PC + BYPASS				// put L in instead
#define DADSP3		S_SPL + D_L + PC + ADDOP + CARRYF	// add low reg to L - we need to maintain carry
#define DADSP4		S_H + D_A + PC + BYPASS				// now get H in Acc
#define DADSP5		S_SPH + D_H + PC + ADCOP + CARRYF	// result in HL


// rotates all operate on the accumulator and affect the carry
// rlc shifts left one bit, bit seven goes to carry and also to bit zero
#define RLC1		S_A + D_A + PC + ADDOP	+ CARRYF			// does the shift into the carry
#define RLC2		S_A + D_A + PC + INCHOP						// and adds the carry to the result
// rrc shifts right one bit; bit zero goes to carry and also to bit seven
#define RRC			S_A + D_A + PC + RRCOP + CARRYF
// ral shifts left one bit into the carry; original carry moves to bit zero
#define RAL			S_A + D_A + PC + ADCOP + CARRYF
// rar shifts right one bit into the carry; original carry moves to bit seven
#define RAR			S_A + D_A + PC + RAROP + CARRYF

// 8 bit move immediates
#define	MVIB		S_M + D_B + PC + BYPASS
#define	MVIC		S_M + D_C + PC + BYPASS
#define	MVID		S_M + D_D + PC + BYPASS
#define	MVIE		S_M + D_E + PC + BYPASS
#define	MVIH		S_M + D_H + PC + BYPASS
#define	MVIL		S_M + D_L + PC + BYPASS
#define	MVIM1		S_M + D_MAH + PC + BYPASS 					// use MAH as a temporary store
#define	MVIM2		S_MAH + D_M + HL + BYPASS 					// and write to [HL]
#define	MVIA		S_M + D_A + PC + BYPASS

// 16-bit decrements
#define DCXB1		S_C + D_C + PC + DECLOP						// by following a declop immediately with dechop the carry is automatic
#define DCXB2		S_B + D_B + PC + DECHOP
#define DCXD1		S_E + D_E + PC + DECLOP
#define DCXD2		S_D + D_D + PC + DECHOP
#define DCXH1		S_L + D_L + PC + DECLOP
#define DCXH2		S_H + D_H + PC + DECHOP
#define DCXSP1		S_SPL + D_SPL + PC + DECLOP
#define DCXSP2		S_SPH + D_SPH + PC + DECHOP

// 16-bit increments
#define INXB1		S_C + D_C + PC + INCLOP						// and the same for inclop/inchop
#define INXB2		S_B + D_B + PC + INCHOP
#define INXD1		S_E + D_E + PC + INCLOP
#define INXD2		S_D + D_D + PC + INCHOP
#define INXH1		S_L + D_L + PC + INCLOP
#define INXH2		S_H + D_H + PC + INCHOP
#define INXSP1		S_SPL + D_SPL + PC + INCLOP
#define INXSP2		S_SPH + D_SPH + PC + INCHOP

// 16 bit stores and load
#define C2MAL		S_C + D_MAL + PC + BYPASS					// copy C to MAL
#define B2MAH		S_B + D_MAH + PC + BYPASS					// copy B to MAH
#define E2MAL		S_E + D_MAL + PC + BYPASS
#define D2MAH		S_D + D_MAH + PC + BYPASS					// and the same for DE
#define	LDAX		S_M + D_A + MA + BYPASS						// once address bus is set up, read
#define STAX		S_A + D_M + MA + BYPASS						// or write

// lxi
#define	LXIB1		S_M + D_C + PC + BYPASS
#define LXIB2		S_M + D_B + PC + BYPASS
#define	LXID1		S_M + D_E + PC + BYPASS
#define LXID2		S_M + D_D + PC + BYPASS
#define	LXIH1		S_M + D_L + PC + BYPASS
#define LXIH2		S_M + D_H + PC + BYPASS
#define	LXISP1		S_M + D_SPL + PC + BYPASS
#define LXISP2		S_M + D_SPH + PC + BYPASS

// 8-bit increments
#define INRB		S_B + D_B + HL + INCLOP + ZSF
#define INRC		S_C + D_C + HL + INCLOP + ZSF
#define INRD		S_D + D_D + HL + INCLOP + ZSF
#define INRE		S_E + D_E + HL + INCLOP + ZSF
#define INRH		S_H + D_H + HL + INCLOP + ZSF
#define INRL		S_L + D_L + HL + INCLOP + ZSF
#define INRM1		S_M + D_MAH + HL + INCLOP + ZSF				// memory to temporary MAH
#define INRM2		S_MAH + D_M + HL + BYPASS					// MAH to memory
#define INRA		S_A + D_A + HL + INCLOP + ZSF

// 8-bit decrements
#define DCRB		S_B + D_B + HL + DECLOP + ZSF
#define DCRC		S_C + D_C + HL + DECLOP + ZSF
#define DCRD		S_D + D_D + HL + DECLOP + ZSF
#define DCRE		S_E + D_E + HL + DECLOP + ZSF
#define DCRH		S_H + D_H + HL + DECLOP + ZSF
#define DCRL		S_L + D_L + HL + DECLOP + ZSF
#define DCRM1		S_M + D_MAH + HL + DECLOP + ZSF				// memory to temporary MAH, decrementing
#define DCRM2		S_MAH + D_M + HL + BYPASS					// MAH to memory
#define DCRA		S_A + D_A + HL + DECLOP + ZSF


// register to a ALU operations
#define ADDB		S_B + D_A + HL + ADDOP + CARRYF + ZSF
#define ADCB		S_B + D_A + HL + ADCOP + CARRYF + ZSF
#define SUBB		S_B + D_A + HL + SUBOP + CARRYF + ZSF
#define SBBB		S_B + D_A + HL + SBBOP + CARRYF + ZSF
#define ANDB		S_B + D_A + HL + ANDOP + CARRYF + ZSF
#define XORB		S_B + D_A + HL + XOROP + CARRYF + ZSF
#define ORB			S_B + D_A + HL + OROP + CARRYF + ZSF
#define CMPB		S_B + D_MAH + HL + CMPOP + CARRYF + ZSF

#define ADDC		S_C + D_A + HL + ADDOP + CARRYF + ZSF
#define ADCC		S_C + D_A + HL + ADCOP + CARRYF + ZSF
#define SUBC		S_C + D_A + HL + SUBOP + CARRYF + ZSF
#define SBBC		S_C + D_A + HL + SBBOP + CARRYF + ZSF
#define ANDC		S_C + D_A + HL + ANDOP + CARRYF + ZSF
#define XORC		S_C + D_A + HL + XOROP + CARRYF + ZSF
#define ORC			S_C + D_A + HL + OROP + CARRYF + ZSF
#define CMPC		S_C + D_MAH + HL + CMPOP + CARRYF + ZSF

#define ADDD		S_D + D_A + HL + ADDOP + CARRYF + ZSF
#define ADCD		S_D + D_A + HL + ADCOP + CARRYF + ZSF
#define SUBD		S_D + D_A + HL + SUBOP + CARRYF + ZSF
#define SBBD		S_D + D_A + HL + SBBOP + CARRYF + ZSF
#define ANDD		S_D + D_A + HL + ANDOP + CARRYF + ZSF
#define XORD		S_D + D_A + HL + XOROP + CARRYF + ZSF
#define ORD			S_D + D_A + HL + OROP + CARRYF + ZSF
#define CMPD		S_D + D_MAH + HL + CMPOP + CARRYF + ZSF

#define ADDE		S_E + D_A + HL + ADDOP + CARRYF + ZSF
#define ADCE		S_E + D_A + HL + ADCOP + CARRYF + ZSF
#define SUBE		S_E + D_A + HL + SUBOP + CARRYF + ZSF
#define SBBE		S_E + D_A + HL + SBBOP + CARRYF + ZSF
#define ANDE		S_E + D_A + HL + ANDOP + CARRYF + ZSF
#define XORE		S_E + D_A + HL + XOROP + CARRYF + ZSF
#define ORE			S_E + D_A + HL + OROP + CARRYF + ZSF
#define CMPE		S_E + D_MAH + HL + CMPOP + CARRYF + ZSF

#define ADDH		S_H + D_A + HL + ADDOP + CARRYF + ZSF
#define ADCH		S_H + D_A + HL + ADCOP + CARRYF + ZSF
#define SUBH		S_H + D_A + HL + SUBOP + CARRYF + ZSF
#define SBBH		S_H + D_A + HL + SBBOP + CARRYF + ZSF
#define ANDH		S_H + D_A + HL + ANDOP + CARRYF + ZSF
#define XORH		S_H + D_A + HL + XOROP + CARRYF + ZSF
#define ORH			S_H + D_A + HL + OROP + CARRYF + ZSF
#define CMPH		S_H + D_MAH + HL + CMPOP + CARRYF + ZSF

#define ADDL		S_L + D_A + HL + ADDOP + CARRYF + ZSF
#define ADCL		S_L + D_A + HL + ADCOP + CARRYF + ZSF
#define SUBL		S_L + D_A + HL + SUBOP + CARRYF + ZSF
#define SBBL		S_L + D_A + HL + SBBOP + CARRYF + ZSF
#define ANDL		S_L + D_A + HL + ANDOP + CARRYF + ZSF
#define XORL		S_L + D_A + HL + XOROP + CARRYF + ZSF
#define ORL			S_L + D_A + HL + OROP + CARRYF + ZSF
#define CMPL		S_L + D_MAH + HL + CMPOP + CARRYF + ZSF

#define ADDM		S_M + D_A + HL + ADDOP + CARRYF + ZSF
#define ADCM		S_M + D_A + HL + ADCOP + CARRYF + ZSF
#define SUBM		S_M + D_A + HL + SUBOP + CARRYF + ZSF
#define SBBM		S_M + D_A + HL + SBBOP + CARRYF + ZSF
#define ANDM		S_M + D_A + HL + ANDOP + CARRYF + ZSF
#define XORM		S_M + D_A + HL + XOROP + CARRYF + ZSF
#define ORM			S_M + D_A + HL + OROP + CARRYF + ZSF
#define CMPM		S_M + D_MAH + HL + CMPOP + CARRYF + ZSF

#define ADDA		S_A + D_A + HL + ADDOP + CARRYF + ZSF
#define ADCA		S_A + D_A + HL + ADCOP + CARRYF + ZSF
#define SUBA		S_A + D_A + HL + SUBOP + CARRYF + ZSF
#define SBBA		S_A + D_A + HL + SBBOP + CARRYF + ZSF
#define ANDA		S_A + D_A + HL + ANDOP + CARRYF + ZSF
#define XORA		S_A + D_A + HL + XOROP + CARRYF + ZSF
#define ORA			S_A + D_A + HL + OROP + CARRYF + ZSF
#define CMPA		S_A + D_MAH + HL + CMPOP + CARRYF + ZSF



// register-register moves
#define MOVBB		S_B + D_B +	HL + BYPASS
#define MOVBC		S_C + D_B +	HL + BYPASS
#define MOVBD		S_D + D_B +	HL + BYPASS
#define MOVBE		S_E + D_B +	HL + BYPASS
#define MOVBH		S_H + D_B +	HL + BYPASS
#define MOVBL		S_L + D_B +	HL + BYPASS
#define MOVBM		S_M + D_B +	HL + BYPASS
#define MOVBA		S_A + D_B +	HL + BYPASS

#define MOVCB		S_B + D_C +	HL + BYPASS
#define MOVCC		S_C + D_C +	HL + BYPASS
#define MOVCD		S_D + D_C +	HL + BYPASS
#define MOVCE		S_E + D_C +	HL + BYPASS
#define MOVCH		S_H + D_C +	HL + BYPASS
#define MOVCL		S_L + D_C +	HL + BYPASS
#define MOVCM		S_M + D_C +	HL + BYPASS
#define MOVCA		S_A + D_C +	HL + BYPASS

#define MOVDB		S_B + D_D +	HL + BYPASS
#define MOVDC		S_C + D_D +	HL + BYPASS
#define MOVDD		S_D + D_D +	HL + BYPASS
#define MOVDE		S_E + D_D +	HL + BYPASS
#define MOVDH		S_H + D_D +	HL + BYPASS
#define MOVDL		S_L + D_D +	HL + BYPASS
#define MOVDM		S_M + D_D +	HL + BYPASS
#define MOVDA		S_A + D_D +	HL + BYPASS

#define MOVEB		S_B + D_E +	HL + BYPASS
#define MOVEC		S_C + D_E +	HL + BYPASS
#define MOVED		S_D + D_E +	HL + BYPASS
#define MOVEE		S_E + D_E +	HL + BYPASS
#define MOVEH		S_H + D_E +	HL + BYPASS
#define MOVEL		S_L + D_E +	HL + BYPASS
#define MOVEM		S_M + D_E +	HL + BYPASS
#define MOVEA		S_A + D_E +	HL + BYPASS

#define MOVHB		S_B + D_H +	HL + BYPASS
#define MOVHC		S_C + D_H +	HL + BYPASS
#define MOVHD		S_D + D_H +	HL + BYPASS
#define MOVHE		S_E + D_H +	HL + BYPASS
#define MOVHH		S_H + D_H +	HL + BYPASS
#define MOVHL		S_L + D_H +	HL + BYPASS
#define MOVHM		S_M + D_H +	HL + BYPASS
#define MOVHA		S_A + D_H +	HL + BYPASS

#define MOVLB		S_B + D_L +	HL + BYPASS
#define MOVLC		S_C + D_L +	HL + BYPASS
#define MOVLD		S_D + D_L +	HL + BYPASS
#define MOVLE		S_E + D_L +	HL + BYPASS
#define MOVLH		S_H + D_L +	HL + BYPASS
#define MOVLL		S_L + D_L +	HL + BYPASS
#define MOVLM		S_M + D_L +	HL + BYPASS
#define MOVLA		S_A + D_L +	HL + BYPASS

#define MOVMB		S_B + D_M +	HL + BYPASS
#define MOVMC		S_C + D_M +	HL + BYPASS
#define MOVMD		S_D + D_M +	HL + BYPASS
#define MOVME		S_E + D_M +	HL + BYPASS
#define MOVMH		S_H + D_M +	HL + BYPASS
#define MOVML		S_L + D_M +	HL + BYPASS
//#define MOVMM		S_M + D_M +	HL + BYPASS		// no, you can't do this!
#define MOVMA		S_A + D_M +	HL + BYPASS

#define MOVAB		S_B + D_A +	HL + BYPASS
#define MOVAC		S_C + D_A +	HL + BYPASS
#define MOVAD		S_D + D_A +	HL + BYPASS
#define MOVAE		S_E + D_A +	HL + BYPASS
#define MOVAH		S_H + D_A +	HL + BYPASS
#define MOVAL		S_L + D_A +	HL + BYPASS
#define MOVAM		S_M + D_A +	HL + BYPASS
#define MOVAA		S_A + D_A +	HL + BYPASS



// now build the control seqences, fill unused slots with zeros
// the first 32 entries in each named block are for the condition false
// instructions and the second 32 for the condition true
// these are only different for the conditional call, jump, returns

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

static uint32_t control [(64 * 256)] =
{
// 0x00 - NOP
	LD_IR,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,LDA,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,LDA,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0x01 - LXI B
	LD_IR,
	INC_PCL,
	INC_PCH,
	LXIB1,
	INC_PCL,
	INC_PCH,
	LXIB2,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	INC_PCL,
	INC_PCH,
	LXIB1,
	INC_PCL,
	INC_PCH,
	LXIB2,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0x02 - STAX B
	LD_IR,
	C2MAL,
	B2MAH,
	STAX,
	INC_PCL,
	INC_PCH + LAST,
	0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	C2MAL,
	B2MAH,
	STAX,
	INC_PCL,
	INC_PCH + LAST,
	0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0x03 - INXB
	LD_IR,
	INXB1,
	INXB2,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	INXB1,
	INXB2,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0x04 - INR B
	LD_IR,
	INRB,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	INRB,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0x05 - DEC B
	LD_IR,
	DCRB,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	DCRB,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0x06 - MVI B
	LD_IR,
	INC_PCL,
	INC_PCH,
	MVIB,
	INC_PCL,
	INC_PCH + LAST,
	0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	INC_PCL,
	INC_PCH,
	MVIB,
	INC_PCL,
	INC_PCH + LAST,
	0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0x07 - RLC
	LD_IR,
	RLC1,
	RLC2,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	RLC1,
	RLC2,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0x08 - NOP
	LD_IR,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0x09 - DAD B
	LD_IR,
	DAD1,
	DADB2,
	DADB3,
	DADB4,
	DADB5,
	DAD6,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	DAD1,
	DADB2,
	DADB3,
	DADB4,
	DADB5,
	DAD6,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0x0a - LDAX B
	LD_IR,
	C2MAL,
	B2MAH,
	LDAX,
	INC_PCL,
	INC_PCH + LAST,
	0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	C2MAL,
	B2MAH,
	LDAX,
	INC_PCL,
	INC_PCH + LAST,
	0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0x0b - DCX B
	LD_IR,
	DCXB1,
	DCXB2,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	DCXB1,
	DCXB2,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0x0c - INR C
	LD_IR,
	INRC,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	INRC,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0x0d - DCR C
	LD_IR,
	DCRC,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	DCRC,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0x0e - MVI C
	LD_IR,
	INC_PCL,
	INC_PCH,
	MVIC,
	INC_PCL,
	INC_PCH + LAST,
	0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	INC_PCL,
	INC_PCH,
	MVIC,
	INC_PCL,
	INC_PCH + LAST,
	0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0x0f - RRC
	LD_IR,
	RRC,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	RRC,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0x10 - NOP
	LD_IR,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0x11 - LXI D
	LD_IR,
	INC_PCL,
	INC_PCH,
	LXID1,
	INC_PCL,
	INC_PCH,
	LXID2,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	INC_PCL,
	INC_PCH,
	LXID1,
	INC_PCL,
	INC_PCH,
	LXID2,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0x12 - STAX D
	LD_IR,
	E2MAL,
	D2MAH,
	STAX,
	INC_PCL,
	INC_PCH + LAST,
	0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	E2MAL,
	D2MAH,
	STAX,
	INC_PCL,
	INC_PCH + LAST,
	0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0x13 - INXD
	LD_IR,
	INXD1,
	INXD2,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	INXD1,
	INXD2,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0x14 - INR D
	LD_IR,
	INRD,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	INRD,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0x15 - DCR D
	LD_IR,
	DCRD,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	DCRD,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0x16 - MVI D
	LD_IR,
	INC_PCL,
	INC_PCH,
	MVID,
	INC_PCL,
	INC_PCH + LAST,
	0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	INC_PCL,
	INC_PCH,
	MVID,
	INC_PCL,
	INC_PCH + LAST,
	0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0x17 - RAL
	LD_IR,
	RAL,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	RAL,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0x18 - NOP
	LD_IR,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0x19 - DAD D
	LD_IR,
	DAD1,
	DADD2,
	DADD3,
	DADD4,
	DADD5,
	DAD6,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	DAD1,
	DADD2,
	DADD3,
	DADD4,
	DADD5,
	DAD6,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0x1a - LDAX D
	LD_IR,
	E2MAL,
	D2MAH,
	LDAX,
	INC_PCL,
	INC_PCH + LAST,
	0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	E2MAL,
	D2MAH,
	LDAX,
	INC_PCL,
	INC_PCH + LAST,
	0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0x1b - DCX D
	LD_IR,
	DCXD1,
	DCXD2,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	DCXD1,
	DCXD2,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0x1c - INR E
	LD_IR,
	INRE,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	INRE,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0x1d - DCR E
	LD_IR,
	DCRE,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	DCRE,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0x1e - MVI E
	LD_IR,
	INC_PCL,
	INC_PCH,
	MVIE,
	INC_PCL,
	INC_PCH + LAST,
	0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	INC_PCL,
	INC_PCH,
	MVIE,
	INC_PCL,
	INC_PCH + LAST,
	0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0x1f - RAR
	LD_IR,
	RAR,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	RAR,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0x20 - NOP
	LD_IR,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0x21 - LXI H
	LD_IR,
	INC_PCL,
	INC_PCH,
	LXIH1,
	INC_PCL,
	INC_PCH,
	LXIH2,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	INC_PCL,
	INC_PCH,
	LXIH1,
	INC_PCL,
	INC_PCH,
	LXIH2,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0x22 - SHLD
	LD_IR,
	INC_PCL,
	INC_PCH,
	M2MAL,
	INC_PCL,
	INC_PCH,
	M2MAH,
	SHLD1,
	INC_MAL,
	INC_MAH,
	SHLD2,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	INC_PCL,
	INC_PCH,
	M2MAL,
	INC_PCL,
	INC_PCH,
	M2MAH,
	SHLD1,
	INC_MAL,
	INC_MAH,
	SHLD2,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0x23 - INXH
	LD_IR,
	INXH1,
	INXH2,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	INXH1,
	INXH2,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0x24 - INR H
	LD_IR,
	INRH,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	INRH,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0x25 - DCR H
	LD_IR,
	DCRH,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	DCRH,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0x26 - MVIH
	LD_IR,
	INC_PCL,
	INC_PCH,
	MVIH,
	INC_PCL,
	INC_PCH + LAST,
	0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	INC_PCL,
	INC_PCH,
	MVIH,
	INC_PCL,
	INC_PCH + LAST,
	0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0x27 - NOP (DAA)
	LD_IR,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0x28 - NOP
	LD_IR,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0x29 - DAD H
	LD_IR,
	DAD1,
	DADH2,
	DADH3,
	DADH4,
	DADH5,
	DAD6,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	DAD1,
	DADH2,
	DADH3,
	DADH4,
	DADH5,
	DAD6,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0x2A - LHLD
	LD_IR,
	INC_PCL,
	INC_PCH,
	M2MAL,
	INC_PCL,
	INC_PCH,
	M2MAH,
	LHLD1,
	INC_MAL,
	INC_MAH,
	LHLD2,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	INC_PCL,
	INC_PCH,
	M2MAL,
	INC_PCL,
	INC_PCH,
	M2MAH,
	LHLD1,
	INC_MAL,
	INC_MAH,
	LHLD2,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0x2b - DCX H
	LD_IR,
	DCXH1,
	DCXH2,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	DCXH1,
	DCXH2,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0x2c - INR L
	LD_IR,
	INRL,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	INRL,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0x2d - DCR L
	LD_IR,
	DCRL,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	DCRL,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0x2e - MVI L
	LD_IR,
	INC_PCL,
	INC_PCH,
	MVIL,
	INC_PCL,
	INC_PCH + LAST,
	0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	INC_PCL,
	INC_PCH,
	MVIL,
	INC_PCL,
	INC_PCH + LAST,
	0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0x2f - CMA
	LD_IR,
	CMA1,
	CMA2,
	CMA3,
	INC_PCL,
	INC_PCH + LAST,
	0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	CMA1,
	CMA2,
	CMA3,
	INC_PCL,
	INC_PCH + LAST,
	0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0x30 - NOP
	LD_IR,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0x31 - LXI SP
	LD_IR,
	INC_PCL,
	INC_PCH,
	LXISP1,
	INC_PCL,
	INC_PCH,
	LXISP2,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	INC_PCL,
	INC_PCH,
	LXISP1,
	INC_PCL,
	INC_PCH,
	LXISP2,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0x32 - STA abs
	LD_IR,
	INC_PCL,
	INC_PCH,
	M2MAL,
	INC_PCL,
	INC_PCH,
	M2MAH,
	STA,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	INC_PCL,
	INC_PCH,
	M2MAL,
	INC_PCL,
	INC_PCH,
	M2MAH,
	STA,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0x33 - INX SP
	LD_IR,
	INXSP1,
	INXSP2,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	INXSP1,
	INXSP2,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0x34 - INR M
	LD_IR,
	INRM1,
	INRM2,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	INRM1,
	INRM2,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0x35 - DCR M
	LD_IR,
	DCRM1,
	DCRM2,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	DCRM1,
	DCRM2,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0x36 - MVI M
	LD_IR,
	INC_PCL,
	INC_PCH,
	MVIM1,
	MVIM2,
	INC_PCL,
	INC_PCH + LAST,
	0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	INC_PCL,
	INC_PCH,
	MVIM1,
	MVIM2,
	INC_PCL,
	INC_PCH + LAST,
	0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0x37 - STC
	LD_IR,
	STC1,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	STC1,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0x38 - NOP
	LD_IR,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0x39 - DAD SP
	LD_IR,
	DAD1,
	DADSP2,
	DADSP3,
	DADSP4,
	DADSP5,
	DAD6,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	DAD1,
	DADSP2,
	DADSP3,
	DADSP4,
	DADSP5,
	DAD6,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0x3a - LDA abs
	LD_IR,
	INC_PCL,
	INC_PCH,
	M2MAL,
	INC_PCL,
	INC_PCH,
	M2MAH,
	LDA,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	INC_PCL,
	INC_PCH,
	M2MAL,
	INC_PCL,
	INC_PCH,
	M2MAH,
	LDA,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0x3b - DCX SP
	LD_IR,
	DCXSP1,
	DCXSP2,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	DCXSP1,
	DCXSP2,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0x3c - INR A
	LD_IR,
	INRA,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	INRA,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0x3d - DCR A
	LD_IR,
	DCRA,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	DCRA,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0x3e - MVI A
	LD_IR,
	INC_PCL,
	INC_PCH,
	MVIA,
	INC_PCL,
	INC_PCH + LAST,
	0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	INC_PCL,
	INC_PCH,
	MVIA,
	INC_PCL,
	INC_PCH + LAST,
	0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0x3f - CMC
	LD_IR,
	CMC1,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	CMC1,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0x40 - Mov B,B
	LD_IR,
	MOVBB,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	MOVBB,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x41 - MOV B,C
	LD_IR,
	MOVBC,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	MOVBC,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x42 - MOV B,D
	LD_IR,
	MOVBD,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	MOVBD,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x43 - MOV B,E
	LD_IR,
	MOVBE,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	MOVBE,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x44 - MOV B,H
	LD_IR,
	MOVBH,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	MOVBH,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x45 - MOV B,L
	LD_IR,
	MOVBL,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	MOVBL,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x46 - Mov B,M
	LD_IR,
	MOVBM,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	MOVBM,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x47 - Mov B,A
	LD_IR,
	MOVBA,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	MOVBA,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x48 - Mov C,B
	LD_IR,
	MOVCB,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	MOVCB,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x49 - MOV C,C
	LD_IR,
	MOVCC,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	MOVCC,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x4a - MOV C,D
	LD_IR,
	MOVCD,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	MOVCD,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x4b - MOV C,E
	LD_IR,
	MOVCE,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	MOVCE,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x4c - MOV C,H
	LD_IR,
	MOVCH,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	MOVCH,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x4d - MOV C,L
	LD_IR,
	MOVCL,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	MOVCL,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x4e - Mov C,M
	LD_IR,
	MOVCM,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	MOVCM,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x4f - Mov C,A
	LD_IR,
	MOVCA,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	MOVCA,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x50 - Mov D,B
	LD_IR,
	MOVDB,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	MOVDB,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x51 - MOV D,C
	LD_IR,
	MOVDC,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	MOVDC,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x52 - MOV D,D
	LD_IR,
	MOVDD,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	MOVDD,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x53 - MOV D,E
	LD_IR,
	MOVDE,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	MOVDE,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x54 - MOV D,H
	LD_IR,
	MOVDH,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	MOVDH,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x55 - MOV D,L
	LD_IR,
	MOVDL,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	MOVDL,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x56 - Mov D,M
	LD_IR,
	MOVDM,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	MOVDM,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x57 - Mov D,A
	LD_IR,
	MOVDA,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	MOVDA,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x58 - Mov E,B
	LD_IR,
	MOVEB,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	MOVEB,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x59 - MOV E,C
	LD_IR,
	MOVEC,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	MOVEC,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x5a - MOV E,D
	LD_IR,
	MOVED,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	MOVED,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x5b - MOV E,E
	LD_IR,
	MOVEE,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	MOVEE,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x5c - MOV E,H
	LD_IR,
	MOVEH,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	MOVEH,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x5d - MOV E,L
	LD_IR,
	MOVEL,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	MOVEL,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x5e - Mov E,M
	LD_IR,
	MOVEM,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	MOVEM,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x5f - Mov E,A
	LD_IR,
	MOVEA,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	MOVEA,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x60 - Mov H,B
	LD_IR,
	MOVHB,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	MOVHB,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x61 - MOV H,C
	LD_IR,
	MOVHC,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	MOVHC,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x62 - MOV H,D
	LD_IR,
	MOVHD,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	MOVHD,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x63 - MOV H,E
	LD_IR,
	MOVHE,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	MOVHE,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x64 - MOV H,H
	LD_IR,
	MOVHH,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	MOVHH,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x65 - MOV H,L
	LD_IR,
	MOVHL,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	MOVHL,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x66 - Mov H,M
	LD_IR,
	MOVHM,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	MOVHM,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x67 - Mov H,A
	LD_IR,
	MOVHA,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	MOVHA,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x68 - Mov L,B
	LD_IR,
	MOVLB,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	MOVLB,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x69 - MOV L,C
	LD_IR,
	MOVLC,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	MOVLC,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x6a - MOV L,D
	LD_IR,
	MOVLD,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	MOVLD,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x6b - MOV L,E
	LD_IR,
	MOVLE,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	MOVLE,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x6c - MOV L,H
	LD_IR,
	MOVLH,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	MOVLH,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x6d - MOV L,L
	LD_IR,
	MOVLL,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	MOVLL,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x6e - Mov L,M
	LD_IR,
	MOVLM,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	MOVLM,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x6f - Mov L,A
	LD_IR,
	MOVLA,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	MOVLA,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x70 - Mov M,B
	LD_IR,
	MOVMB,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	MOVMB,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x71 - MOV M,C
	LD_IR,
	MOVMC,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	MOVMC,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x72 - MOV M,D
	LD_IR,
	MOVMD,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	MOVMD,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x73 - MOV M,E
	LD_IR,
	MOVME,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	MOVME,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x74 - MOV M,H
	LD_IR,
	MOVMH,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	MOVMH,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x75 - MOV M,L
	LD_IR,
	MOVML,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	MOVML,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x76 - NOP
	LD_IR,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x77 - Mov M,A
	LD_IR,
	MOVMA,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	MOVMA,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x78 - Mov A,B
	LD_IR,
	MOVAB,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	MOVAB,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x79 - MOV A,C
	LD_IR,
	MOVAC,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	MOVAC,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x7a - MOV A,D
	LD_IR,
	MOVAD,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	MOVAD,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x7b - MOV A,E
	LD_IR,
	MOVAE,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	MOVAE,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x7c - MOV A,H
	LD_IR,
	MOVAH,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	MOVAH,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x7d - MOV A,L
	LD_IR,
	MOVAL,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	MOVAL,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x7e - Mov A,M
	LD_IR,
	MOVAM,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	MOVAM,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x7f - Mov A,A
	LD_IR,
	MOVAA,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	MOVAA,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x80 - ADD B
	LD_IR,
	ADDB,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	ADDB,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x81 - ADD C
	LD_IR,
	ADDC,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	ADDC,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x82 - ADD D
	LD_IR,
	ADDD,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	ADDD,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x83 - ADD E
	LD_IR,
	ADDE,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	ADDE,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x84 - ADD H
	LD_IR,
	ADDH,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	ADDH,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x85 - ADD L
	LD_IR,
	ADDL,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	ADDL,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x86 - ADD M
	LD_IR,
	ADDM,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	ADDM,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x87 - ADD A
	LD_IR,
	ADDA,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	ADDA,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x88 - ADC B
	LD_IR,
	ADCB,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	ADCB,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x89 - ADC C
	LD_IR,
	ADCC,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	ADCC,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x8a - ADC D
	LD_IR,
	ADCD,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	ADCD,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x8b - ADC E
	LD_IR,
	ADCE,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	ADCE,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x8c - ADC H
	LD_IR,
	ADCH,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	ADCH,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x8d - ADC L
	LD_IR,
	ADCL,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	ADCL,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x8e - ADC M
	LD_IR,
	ADCM,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	ADCM,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x8f - ADC A
	LD_IR,
	ADCA,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	ADCA,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x90 - SUB B
	LD_IR,
	SUBB,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	SUBB,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x91 - SUB C
	LD_IR,
	SUBC,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	SUBC,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x92 - SUB D
	LD_IR,
	SUBD,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	SUBD,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x93 - SUB E
	LD_IR,
	SUBE,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	SUBE,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x94 - SUB H
	LD_IR,
	SUBH,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	SUBH,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x95 - SUB L
	LD_IR,
	SUBL,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	SUBL,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x96 - SUB M
	LD_IR,
	SUBM,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	SUBM,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x97 - SUB A
	LD_IR,
	SUBA,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	SUBA,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x98 - SBB B
	LD_IR,
	SBBB,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	SBBB,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x99 - SBB C
	LD_IR,
	SBBC,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	SBBC,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x9a - SBB D
	LD_IR,
	SBBD,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	SBBD,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x9b - SBB E
	LD_IR,
	SBBE,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	SBBE,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x9c - SBB H
	LD_IR,
	SBBH,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	SBBH,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x9d - SBB L
	LD_IR,
	SBBL,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	SBBL,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x9e - SBB M
	LD_IR,
	SBBM,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	SBBM,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0x9f - SBB A
	LD_IR,
	SBBA,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	SBBA,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0xa0 - AND B
	LD_IR,
	ANDB,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	ANDB,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0xa1 - AND C
	LD_IR,
	ANDC,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	ANDC,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0xa2 - AND D
	LD_IR,
	ANDD,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	ANDD,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0xa3 - AND E
	LD_IR,
	ANDE,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	ANDE,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0xa4 - AND H
	LD_IR,
	ANDH,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	ANDH,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0xa5 - AND L
	LD_IR,
	ANDL,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	ANDL,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0xa6 - AND M
	LD_IR,
	ANDM,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	ANDM,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0xa7 - AND A
	LD_IR,
	ANDA,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	ANDA,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0xa8 - XOR B
	LD_IR,
	XORB,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	XORB,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0xa9 - XOR C
	LD_IR,
	XORC,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	XORC,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0xaa - XOR D
	LD_IR,
	XORD,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	XORD,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0xab - XOR E
	LD_IR,
	XORE,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	XORE,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0xac - XOR H
	LD_IR,
	XORH,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	XORH,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0xad - XOR L
	LD_IR,
	XORL,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	XORL,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0xae - XOR M
	LD_IR,
	XORM,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	XORM,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0xaf - XOR A
	LD_IR,
	XORA,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	XORA,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0xb0 - OR B
	LD_IR,
	ORB,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	ORB,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0xb1 - OR C
	LD_IR,
	ORC,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	ORC,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0xb2 - OR D
	LD_IR,
	ORD,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	ORD,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0xb3 - OR E
	LD_IR,
	ORE,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	ORE,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0xb4 - OR H
	LD_IR,
	ORH,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	ORH,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0xb5 - OR L
	LD_IR,
	ORL,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	ORL,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0xb6 - OR M
	LD_IR,
	ORM,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	ORM,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0xb7 - OR A
	LD_IR,
	ORA,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	ORA,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0xb8 - CMP B
	LD_IR,
	CMPB,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	CMPB,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0xb9 - CMP C
	LD_IR,
	CMPC,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	CMPC,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0xba - CMP D
	LD_IR,
	CMPD,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	CMPD,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0xbb - CMP E
	LD_IR,
	CMPE,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	CMPE,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0xbc - CMP H
	LD_IR,
	CMPH,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	CMPH,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0xbd - CMP L
	LD_IR,
	CMPL,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	CMPL,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0xbe - CMP M
	LD_IR,
	CMPM,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	CMPM,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0xbf - CMP A
	LD_IR,
	CMPA,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	LD_IR,
	CMPA,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// 0xc0 - RNZ
	LD_IR,				// condition does not match
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,				// condition matches
	POPPCL,
	INC_SPL,
	INC_SPH,
	POPPCH,
	INC_SPL,
	INC_SPH + LAST,
	0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0xc1 - POP B		; inc sp after pop
	LD_IR,
	POPC,
	INC_SPL,
	INC_SPH,
	POPB,
	INC_SPL,
	INC_SPH,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	POPC,
	INC_SPL,
	INC_SPH,
	POPB,
	INC_SPL,
	INC_SPH,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0xc2 - JNZ
	LD_IR,				// condition not matched; move to next instruction
	INC_PCL,
	INC_PCH,
	INC_PCL,
	INC_PCH,
	INC_PCL,
	INC_PCH + LAST,
	0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,				// condition matched, do the jump
	INC_PCL,
	INC_PCH,
	JMP1,
	INC_PCL,
	INC_PCH,
	JMP2,
	JMP3 + LAST,
	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0xc3 - JMP abs
	LD_IR,
	INC_PCL,
	INC_PCH,
	JMP1,
	INC_PCL,
	INC_PCH,
	JMP2,
	JMP3 + LAST,
	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	INC_PCL,
	INC_PCH,
	JMP1,
	INC_PCL,
	INC_PCH,
	JMP2,
	JMP3 + LAST,
	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0xc4 - CNZ
	LD_IR,				// condition not matched; move to next instruction
	INC_PCL,
	INC_PCH,
	INC_PCL,
	INC_PCH,
	INC_PCL,
	INC_PCH + LAST,
	0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	INC_PCL,
	INC_PCH,	// move to low byte of target
	M2MAL,		// and grab it
	INC_PCL,
	INC_PCH,
	M2MAH,		// same for high byte
	INC_PCL,
	INC_PCH,	// move to next instruction
	DEC_SPL,
	DEC_SPH,
	PUSHPCH,
	DEC_SPL,
	DEC_SPH,
	PUSHPCL,
	CALL1,		// copy
	CALL2 + LAST,
	0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0xc5 - PUSH B	; dec sp before push
	LD_IR,
	DEC_SPL,
	DEC_SPH,
	PUSHB,
	DEC_SPL,
	DEC_SPH,
	PUSHC,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	DEC_SPL,
	DEC_SPH,
	PUSHB,
	DEC_SPL,
	DEC_SPH,
	PUSHC,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0xc6 - ADI
	LD_IR,
	INC_PCL,
	INC_PCH,
	ADI,
	INC_PCL,
	INC_PCH + LAST,
	0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	INC_PCL,
	INC_PCH,
	ADI,
	INC_PCL,
	INC_PCH + LAST,
	0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0xc7 - RST 0
	LD_IR,
	INC_PCL,
	INC_PCH,
	DEC_SPL,
	DEC_SPH,
	PUSHPCH,
	DEC_SPL,
	DEC_SPH,
	PUSHPCL,
	RST1,
	RST2 + LAST,
	0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	INC_PCL,
	INC_PCH,
	DEC_SPL,
	DEC_SPH,
	PUSHPCH,
	DEC_SPL,
	DEC_SPH,
	PUSHPCL,
	RST1,
	RST2 + LAST,
	0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0xc8 - RZ
	LD_IR,				// condition does not match
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,				// condition matches
	POPPCL,
	INC_SPL,
	INC_SPH,
	POPPCH,
	INC_SPL,
	INC_SPH + LAST,
	0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
/// 0xc9 - RET
	LD_IR,
	POPPCL,
	INC_SPL,
	INC_SPH,
	POPPCH,
	INC_SPL,
	INC_SPH + LAST,
	0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	POPPCL,
	INC_SPL,
	INC_SPH,
	POPPCH,
	INC_SPL,
	INC_SPH + LAST,
	0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0xca - JZ
	LD_IR,				// condition not matched; move to next instruction
	INC_PCL,
	INC_PCH,
	INC_PCL,
	INC_PCH,
	INC_PCL,
	INC_PCH + LAST,
	0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,				// condition matched, do the jump
	INC_PCL,
	INC_PCH,
	JMP1,
	INC_PCL,
	INC_PCH,
	JMP2,
	JMP3 + LAST,
	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0xcb - jmp abs
	LD_IR,
	INC_PCL,
	INC_PCH,
	JMP1,
	INC_PCL,
	INC_PCH,
	JMP2,
	JMP3 + LAST,
	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	INC_PCL,
	INC_PCH,
	JMP1,
	INC_PCL,
	INC_PCH,
	JMP2,
	JMP3 + LAST,
	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0xcc - CZ
	LD_IR,				// condition not matched; move to next instruction
	INC_PCL,
	INC_PCH,
	INC_PCL,
	INC_PCH,
	INC_PCL,
	INC_PCH + LAST,
	0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,				// condition matched, do the jump
	INC_PCL,
	INC_PCH,
	JMP1,
	INC_PCL,
	INC_PCH,
	JMP2,
	JMP3 + LAST,
	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0xcd - CALL
	LD_IR,
	INC_PCL,
	INC_PCH,	// move to low byte of target
	M2MAL,		// and grab it
	INC_PCL,
	INC_PCH,
	M2MAH,		// same for high byte
	INC_PCL,
	INC_PCH,	// move to next instruction
	DEC_SPL,
	DEC_SPH,
	PUSHPCH,
	DEC_SPL,
	DEC_SPH,
	PUSHPCL,
	CALL1,		// copy
	CALL2 + LAST,
	0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	INC_PCL,
	INC_PCH,	// move to low byte of target
	M2MAL,		// and grab it
	INC_PCL,
	INC_PCH,
	M2MAH,		// same for high byte
	INC_PCL,
	INC_PCH,	// move to next instruction
	DEC_SPL,
	DEC_SPH,
	PUSHPCH,
	DEC_SPL,
	DEC_SPH,
	PUSHPCL,
	CALL1,		// copy
	CALL2 + LAST,
	0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0xce - ACI
	LD_IR,
	INC_PCL,
	INC_PCH,
	ACI,
	INC_PCL,
	INC_PCH + LAST,
	0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	INC_PCL,
	INC_PCH,
	ACI,
	INC_PCL,
	INC_PCH + LAST,
	0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0xcf - RST1
	LD_IR,
	INC_PCL,
	INC_PCH,
	DEC_SPL,
	DEC_SPH,
	PUSHPCH,
	DEC_SPL,
	DEC_SPH,
	PUSHPCL,
	RST1,
	RST2 + LAST,
	0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	INC_PCL,
	INC_PCH,
	DEC_SPL,
	DEC_SPH,
	PUSHPCH,
	DEC_SPL,
	DEC_SPH,
	PUSHPCL,
	RST1,
	RST2 + LAST,
	0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0xd0 - RNC
	LD_IR,				// condition does not match
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,				// condition matches
	POPPCL,
	INC_SPL,
	INC_SPH,
	POPPCH,
	INC_SPL,
	INC_SPH + LAST,
	0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0xd1 - POP D
	LD_IR,
	POPE,
	INC_SPL,
	INC_SPH,
	POPD,
	INC_SPL,
	INC_SPH,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	POPE,
	INC_SPL,
	INC_SPH,
	POPD,
	INC_SPL,
	INC_SPH,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0xd2 - JNC
	LD_IR,				// condition not matched; move to next instruction
	INC_PCL,
	INC_PCH,
	INC_PCL,
	INC_PCH,
	INC_PCL,
	INC_PCH + LAST,
	0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,				// condition matched, do the jump
	INC_PCL,
	INC_PCH,
	JMP1,
	INC_PCL,
	INC_PCH,
	JMP2,
	JMP3 + LAST,
	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0xd3 - NOP
	LD_IR,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0xd4 - CNC
	LD_IR,				// condition not matched; move to next instruction
	INC_PCL,
	INC_PCH,
	INC_PCL,
	INC_PCH,
	INC_PCL,
	INC_PCH + LAST,
	0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	INC_PCL,
	INC_PCH,	// move to low byte of target
	M2MAL,		// and grab it
	INC_PCL,
	INC_PCH,
	M2MAH,		// same for high byte
	INC_PCL,
	INC_PCH,	// move to next instruction
	DEC_SPL,
	DEC_SPH,
	PUSHPCH,
	DEC_SPL,
	DEC_SPH,
	PUSHPCL,
	CALL1,		// copy
	CALL2 + LAST,
	0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0xd5 - PUSH D
	LD_IR,
	DEC_SPL,
	DEC_SPH,
	PUSHD,
	DEC_SPL,
	DEC_SPH,
	PUSHE,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	DEC_SPL,
	DEC_SPH,
	PUSHD,
	DEC_SPL,
	DEC_SPH,
	PUSHE,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0xd6 - SUI
	LD_IR,
	INC_PCL,
	INC_PCH,
	SUI,
	INC_PCL,
	INC_PCH + LAST,
	0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	INC_PCL,
	INC_PCH,
	SUI,
	INC_PCL,
	INC_PCH + LAST,
	0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0xd7 - RST2
	LD_IR,
	INC_PCL,
	INC_PCH,
	DEC_SPL,
	DEC_SPH,
	PUSHPCH,
	DEC_SPL,
	DEC_SPH,
	PUSHPCL,
	RST1,
	RST2 + LAST,
	0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	INC_PCL,
	INC_PCH,
	DEC_SPL,
	DEC_SPH,
	PUSHPCH,
	DEC_SPL,
	DEC_SPH,
	PUSHPCL,
	RST1,
	RST2 + LAST,
	0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0xd8 - RC
	LD_IR,				// condition does not match
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,				// condition matches
	POPPCL,
	INC_SPL,
	INC_SPH,
	POPPCH,
	INC_SPL,
	INC_SPH + LAST,
	0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0xd9 - RET
	LD_IR,
	POPPCL,
	INC_SPL,
	INC_SPH,
	POPPCH,
	INC_SPL,
	INC_SPH + LAST,
	0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	POPPCL,
	INC_SPL,
	INC_SPH,
	POPPCH,
	INC_SPL,
	INC_SPH + LAST,
	0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0xda - JC
	LD_IR,				// condition not matched; move to next instruction
	INC_PCL,
	INC_PCH,
	INC_PCL,
	INC_PCH,
	INC_PCL,
	INC_PCH + LAST,
	0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,				// condition matched, do the jump
	INC_PCL,
	INC_PCH,
	JMP1,
	INC_PCL,
	INC_PCH,
	JMP2,
	JMP3 + LAST,
	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0xdb - NOP
	LD_IR,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0xdc - CC
	LD_IR,				// condition not matched; move to next instruction
	INC_PCL,
	INC_PCH,
	INC_PCL,
	INC_PCH,
	INC_PCL,
	INC_PCH + LAST,
	0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,				// condition matched, do the jump
	INC_PCL,
	INC_PCH,
	JMP1,
	INC_PCL,
	INC_PCH,
	JMP2,
	JMP3 + LAST,
	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0xdd - CALL
	LD_IR,
	INC_PCL,
	INC_PCH,	// move to low byte of target
	M2MAL,		// and grab it
	INC_PCL,
	INC_PCH,
	M2MAH,		// same for high byte
	INC_PCL,
	INC_PCH,	// move to next instruction
	DEC_SPL,
	DEC_SPH,
	PUSHPCH,
	DEC_SPL,
	DEC_SPH,
	PUSHPCL,
	CALL1,		// copy
	CALL2 + LAST,
	0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	INC_PCL,
	INC_PCH,	// move to low byte of target
	M2MAL,		// and grab it
	INC_PCL,
	INC_PCH,
	M2MAH,		// same for high byte
	INC_PCL,
	INC_PCH,	// move to next instruction
	DEC_SPL,
	DEC_SPH,
	PUSHPCH,
	DEC_SPL,
	DEC_SPH,
	PUSHPCL,
	CALL1,		// copy
	CALL2 + LAST,
	0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0xde - SBI
	LD_IR,
	INC_PCL,
	INC_PCH,
	SBI,
	INC_PCL,
	INC_PCH + LAST,
	0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	INC_PCL,
	INC_PCH,
	SBI,
	INC_PCL,
	INC_PCH + LAST,
	0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0xdf - RST3
	LD_IR,
	INC_PCL,
	INC_PCH,
	DEC_SPL,
	DEC_SPH,
	PUSHPCH,
	DEC_SPL,
	DEC_SPH,
	PUSHPCL,
	RST1,
	RST2 + LAST,
	0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	INC_PCL,
	INC_PCH,
	DEC_SPL,
	DEC_SPH,
	PUSHPCH,
	DEC_SPL,
	DEC_SPH,
	PUSHPCL,
	RST1,
	RST2 + LAST,
	0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0xe0 - RPO not implemented
	LD_IR,				// condition does not match
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,				// condition matches
	POPPCL,
	INC_SPL,
	INC_SPH,
	POPPCH,
	INC_SPL,
	INC_SPH + LAST,
	0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0xe1 - POP h
	LD_IR,
	POPL,
	INC_SPL,
	INC_SPH,
	POPH,
	INC_SPL,
	INC_SPH,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	POPL,
	INC_SPL,
	INC_SPH,
	POPH,
	INC_SPL,
	INC_SPH,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0xe2 - JPO - not implemented, no P flag
	LD_IR,				// condition not matched; move to next instruction
	INC_PCL,
	INC_PCH,
	INC_PCL,
	INC_PCH,
	INC_PCL,
	INC_PCH + LAST,
	0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,				// condition matched, do the jump
	INC_PCL,
	INC_PCH,
	JMP1,
	INC_PCL,
	INC_PCH,
	JMP2,
	JMP3 + LAST,
	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0xe3 - XTHL
	LD_IR,
	XTHL1,
	XTHL2,
	POPL,
	INC_SPL,
	INC_SPH,
	POPH,
	PUSHMAH,
	DEC_SPL,
	DEC_SPH,
	PUSHMAL,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	XTHL1,
	XTHL2,
	POPL,
	INC_SPL,
	INC_SPH,
	POPH,
	PUSHMAH,
	DEC_SPL,
	DEC_SPH,
	PUSHMAL,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0xe4 - CPO - not implemented
	LD_IR,				// condition not matched; move to next instruction
	INC_PCL,
	INC_PCH,
	INC_PCL,
	INC_PCH,
	INC_PCL,
	INC_PCH + LAST,
	0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	INC_PCL,
	INC_PCH,	// move to low byte of target
	M2MAL,		// and grab it
	INC_PCL,
	INC_PCH,
	M2MAH,		// same for high byte
	INC_PCL,
	INC_PCH,	// move to next instruction
	DEC_SPL,
	DEC_SPH,
	PUSHPCH,
	DEC_SPL,
	DEC_SPH,
	PUSHPCL,
	CALL1,		// copy
	CALL2 + LAST,
	0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0xe5 - PUSH H
	LD_IR,
	DEC_SPL,
	DEC_SPH,
	PUSHH,
	DEC_SPL,
	DEC_SPH,
	PUSHL,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	DEC_SPL,
	DEC_SPH,
	PUSHH,
	DEC_SPL,
	DEC_SPH,
	PUSHL,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0xe6 - ANI
	LD_IR,
	INC_PCL,
	INC_PCH,
	ANI,
	INC_PCL,
	INC_PCH + LAST,
	0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	INC_PCL,
	INC_PCH,
	ANI,
	INC_PCL,
	INC_PCH + LAST,
	0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0xe7 - RST4
	LD_IR,
	INC_PCL,
	INC_PCH,
	DEC_SPL,
	DEC_SPH,
	PUSHPCH,
	DEC_SPL,
	DEC_SPH,
	PUSHPCL,
	RST1,
	RST2 + LAST,
	0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	INC_PCL,
	INC_PCH,
	DEC_SPL,
	DEC_SPH,
	PUSHPCH,
	DEC_SPL,
	DEC_SPH,
	PUSHPCL,
	RST1,
	RST2 + LAST,
	0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0xe8 - RPE not implemented
	LD_IR,				// condition does not match
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,				// condition matches
	POPPCL,
	INC_SPL,
	INC_SPH,
	POPPCH,
	INC_SPL,
	INC_SPH + LAST,
	0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0xe9 - PCHL
	LD_IR,
	PCHL1,
	PCHL2 + LAST,
	0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	PCHL1,
	PCHL2 + LAST,
	0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0xea - JPE - not implemented, no P flag
	LD_IR,				// condition not matched; move to next instruction
	INC_PCL,
	INC_PCH,
	INC_PCL,
	INC_PCH,
	INC_PCL,
	INC_PCH + LAST,
	0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,				// condition matched, do the jump
	INC_PCL,
	INC_PCH,
	JMP1,
	INC_PCL,
	INC_PCH,
	JMP2,
	JMP3 + LAST,
	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0xeb - XCHG							swap hl and de
	LD_IR,
	XCHG1,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	XCHG1,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0xec - CPE - not implemented
	LD_IR,				// condition not matched; move to next instruction
	INC_PCL,
	INC_PCH,
	INC_PCL,
	INC_PCH,
	INC_PCL,
	INC_PCH + LAST,
	0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,				// condition matched, do the jump
	INC_PCL,
	INC_PCH,
	JMP1,
	INC_PCL,
	INC_PCH,
	JMP2,
	JMP3 + LAST,
	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0xed - CALL
	LD_IR,
	INC_PCL,
	INC_PCH,	// move to low byte of target
	M2MAL,		// and grab it
	INC_PCL,
	INC_PCH,
	M2MAH,		// same for high byte
	INC_PCL,
	INC_PCH,	// move to next instruction
	DEC_SPL,
	DEC_SPH,
	PUSHPCH,
	DEC_SPL,
	DEC_SPH,
	PUSHPCL,
	CALL1,		// copy
	CALL2 + LAST,
	0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	INC_PCL,
	INC_PCH,	// move to low byte of target
	M2MAL,		// and grab it
	INC_PCL,
	INC_PCH,
	M2MAH,		// same for high byte
	INC_PCL,
	INC_PCH,	// move to next instruction
	DEC_SPL,
	DEC_SPH,
	PUSHPCH,
	DEC_SPL,
	DEC_SPH,
	PUSHPCL,
	CALL1,		// copy
	CALL2 + LAST,
	0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0xee - XRI
	LD_IR,
	INC_PCL,
	INC_PCH,
	XRI,
	INC_PCL,
	INC_PCH + LAST,
	0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	INC_PCL,
	INC_PCH,
	XRI,
	INC_PCL,
	INC_PCH + LAST,
	0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0xef - RST5
	LD_IR,
	INC_PCL,
	INC_PCH,
	DEC_SPL,
	DEC_SPH,
	PUSHPCH,
	DEC_SPL,
	DEC_SPH,
	PUSHPCL,
	RST1,
	RST2 + LAST,
	0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	INC_PCL,
	INC_PCH,
	DEC_SPL,
	DEC_SPH,
	PUSHPCH,
	DEC_SPL,
	DEC_SPH,
	PUSHPCL,
	RST1,
	RST2 + LAST,
	0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0xf0 - RP
	LD_IR,				// condition does not match
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,				// condition matches
	POPPCL,
	INC_SPL,
	INC_SPH,
	POPPCH,
	INC_SPL,
	INC_SPH + LAST,
	0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
/// 0xf1 - POP PSW
	LD_IR,
	POPF,
	INC_SPL,
	INC_SPH,
	POPA,
	INC_SPL,
	INC_SPH,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	POPF,
	INC_SPL,
	INC_SPH,
	POPA,
	INC_SPL,
	INC_SPH,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0xf2 - JP
	LD_IR,				// condition not matched; move to next instruction
	INC_PCL,
	INC_PCH,
	INC_PCL,
	INC_PCH,
	INC_PCL,
	INC_PCH + LAST,
	0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,				// condition matched, do the jump
	INC_PCL,
	INC_PCH,
	JMP1,
	INC_PCL,
	INC_PCH,
	JMP2,
	JMP3 + LAST,
	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0xf3 - DI
	LD_IR,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0xf4 - CP
	LD_IR,				// condition not matched; move to next instruction
	INC_PCL,
	INC_PCH,
	INC_PCL,
	INC_PCH,
	INC_PCL,
	INC_PCH + LAST,
	0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	INC_PCL,
	INC_PCH,	// move to low byte of target
	M2MAL,		// and grab it
	INC_PCL,
	INC_PCH,
	M2MAH,		// same for high byte
	INC_PCL,
	INC_PCH,	// move to next instruction
	DEC_SPL,
	DEC_SPH,
	PUSHPCH,
	DEC_SPL,
	DEC_SPH,
	PUSHPCL,
	CALL1,		// copy
	CALL2 + LAST,
	0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0xf5 - PUSH PSW
	LD_IR,
	DEC_SPL,
	DEC_SPH,
	PUSHA,
	DEC_SPL,
	DEC_SPH,
	PUSHF,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	DEC_SPL,
	DEC_SPH,
	PUSHA,
	DEC_SPL,
	DEC_SPH,
	PUSHF,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0xf6 - ORI
	LD_IR,
	INC_PCL,
	INC_PCH,
	ORI,
	INC_PCL,
	INC_PCH + LAST,
	0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	INC_PCL,
	INC_PCH,
	ORI,
	INC_PCL,
	INC_PCH + LAST,
	0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0xf7 - RST6
	LD_IR,
	INC_PCL,
	INC_PCH,
	DEC_SPL,
	DEC_SPH,
	PUSHPCH,
	DEC_SPL,
	DEC_SPH,
	PUSHPCL,
	RST1,
	RST2 + LAST,
	0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	INC_PCL,
	INC_PCH,
	DEC_SPL,
	DEC_SPH,
	PUSHPCH,
	DEC_SPL,
	DEC_SPH,
	PUSHPCL,
	RST1,
	RST2 + LAST,
	0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0xf8 - RM
	LD_IR,				// condition does not match
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,				// condition matches
	POPPCL,
	INC_SPL,
	INC_SPH,
	POPPCH,
	INC_SPL,
	INC_SPH + LAST,
	0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0xf9 - SPHL				copy hl to sp, hl remains unchanged
	LD_IR,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0xfa - JM
	LD_IR,				// condition not matched; move to next instruction
	INC_PCL,
	INC_PCH,
	INC_PCL,
	INC_PCH,
	INC_PCL,
	INC_PCH + LAST,
	0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,				// condition matched, do the jump
	INC_PCL,
	INC_PCH,
	JMP1,
	INC_PCL,
	INC_PCH,
	JMP2,
	JMP3 + LAST,
	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0xfb - EI
	LD_IR,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	INC_PCL,
	INC_PCH + LAST,
	0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0xfc - CM
	LD_IR,				// condition not matched; move to next instruction
	INC_PCL,
	INC_PCH,
	INC_PCL,
	INC_PCH,
	INC_PCL,
	INC_PCH + LAST,
	0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	INC_PCL,
	INC_PCH,	// move to low byte of target
	M2MAL,		// and grab it
	INC_PCL,
	INC_PCH,
	M2MAH,		// same for high byte
	INC_PCL,
	INC_PCH,	// move to next instruction
	DEC_SPL,
	DEC_SPH,
	PUSHPCH,
	DEC_SPL,
	DEC_SPH,
	PUSHPCL,
	CALL1,		// copy
	CALL2 + LAST,
	0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0xfd - CALL
	LD_IR,
	INC_PCL,
	INC_PCH,	// move to low byte of target
	M2MAL,		// and grab it
	INC_PCL,
	INC_PCH,
	M2MAH,		// same for high byte
	INC_PCL,
	INC_PCH,	// move to next instruction
	DEC_SPL,
	DEC_SPH,
	PUSHPCH,
	DEC_SPL,
	DEC_SPH,
	PUSHPCL,
	CALL1,		// copy
	CALL2 + LAST,
	0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	INC_PCL,
	INC_PCH,	// move to low byte of target
	M2MAL,		// and grab it
	INC_PCL,
	INC_PCH,
	M2MAH,		// same for high byte
	INC_PCL,
	INC_PCH,	// move to next instruction
	DEC_SPL,
	DEC_SPH,
	PUSHPCH,
	DEC_SPL,
	DEC_SPH,
	PUSHPCL,
	CALL1,		// copy
	CALL2 + LAST,
	0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0xfe - CPI
	LD_IR,
	INC_PCL,
	INC_PCH,
	CPI,
	INC_PCL,
	INC_PCH + LAST,
	0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	INC_PCL,
	INC_PCH,
	CPI,
	INC_PCL,
	INC_PCH + LAST,
	0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
// 0xff - RST7
	LD_IR,
	INC_PCL,
	INC_PCH,
	DEC_SPL,
	DEC_SPH,
	PUSHPCH,
	DEC_SPL,
	DEC_SPH,
	PUSHPCL,
	RST1,
	RST2 + LAST,
	0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
	LD_IR,
	INC_PCL,
	INC_PCH,
	DEC_SPL,
	DEC_SPH,
	PUSHPCH,
	DEC_SPL,
	DEC_SPH,
	PUSHPCL,
	RST1,
	RST2 + LAST,
	0,0,0,0,0,	0,0,0,0,0,0,0,0,	0,0,0,0,0,0,0,0,
};

int main (void)
{
	bool halt = false;

	printf ("v2.0 raw\n");
	for (int r = 0; r < (256 * 8); r++)
	{
		for (int q = 0; q < 8; q++)
		{
			// every 64th entry should be LD_IR - sanity check that it is
			if ((r % 8) == 0)
			{
				if (q == 0)
				{
					if (control[q + (8 * r)] != LD_IR)
					{
						fprintf (stderr, "Alignment error at instruction %02x\n", r / 8);
						halt = true;
						break;
					}
				}
			}
			printf ("%08x ", control[q + (8 * r)]);
		}
		printf ("\n");
		if (halt)
		{
			break;
		}

	}
}
