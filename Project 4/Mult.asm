// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Mult.asm

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)
//
// This program only needs to handle arguments that satisfy
// R0 >= 0, R1 >= 0, and R0*R1 < 32768.

	// result = 0
	@result
	M=0

	// if R0 is 0, answer is 0
	@0
	D=M
	@DONE
	D;JEQ


	// if R1 is 0, answer is 0
	@1
	D=M
	@DONE
	D;JEQ

(LOOP)
	// result += R1
	@1
	D=M
	@result
	M=D+M

	// R0 -= 1
	@0
	M=M-1

	// loop
	D=M
	@LOOP
	D;JGT

// store result in R2
(DONE)
	@result
	D=M
	@2
	M=D

// end program
(END)
	@END
	0;JMP
