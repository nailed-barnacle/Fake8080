# Fake8080
Necessary files to implement an 8080 - or most of one - in 74HC logic.
Stage one is to simulate the operation in Logisim-Evolution, using Logisim default parts and a PROM to control the sequencer.
Stage two modifies the design to use TTL compatible Logisim parts.
Stage three looks at whether it's worthwhile to replace the sequencer proms with discrete logic.
Stage four converts the design to Kicad to create schematics and a PCB.
Stage five live tests the PCB and hopes the smoke stays inside where it should.

Targets: to run a modified version of cpudiag.asm, and Dr Li Chen's Tiny Basic.

Note: I will not be implementing a number of 8080 instructions, largely because I never used them, Tiny Basic doesn't use them, and I can't really see a need for them in this day and age: DAA, and all the jump, call, and return instructions conditional upon the parity flag.

ALU_181_base.circ is a Logisim file which will implement most of the 8080. Use the module Fake8080 for the complete system.

cpudiag.asm is a test program by Kelly Smith at Microcosm Associates, modified by me to remove CP/M calls and tests for DAA and parity conditionals.

tiny.raw is Dr Li Chen's Palo Alto Tiny Basic.

NOTE: this emulation writes to the terminal at 0xf100, no checking is required. Data from the keyboard appears at 0xf000, or as 0x00 is there is no character waiting.
