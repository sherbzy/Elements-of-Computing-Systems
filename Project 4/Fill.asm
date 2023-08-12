// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Fill.asm

// Runs an infinite loop that listens to the keyboard input.
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel;
// the screen should remain fully black as long as the key is pressed. 
// When no key is pressed, the program clears the screen, i.e. writes
// "white" in every pixel;
// the screen should remain fully clear as long as no key is pressed.


(LOOP)
	// initialize the fill bits to 1
	@fill
	M=-1

	// store the screen size
	@8192
	D=A
	@scrSize
	M=D

	// store the screen position
	@SCREEN
	D=A
	@scrPos
	M=D

	// check keyboard and jump if fill should be 1
	@24576
	D=M
	@DRAW
	D;JGT
	
	// otherwise, set fill to 0
	@fill
	M=0

//use a loop to fill the whole screen with the chosen color
(DRAW)
	// get the fill bits
	@fill
	D=M

	// set current word on the screen to the chosen color
	@scrPos
	A=M
	M=D

	// increment screen position and store
	A=A+1
	D=A
	@scrPos
	M=D

	// decrement screen size
	@scrSize
	M=M-1

	// jump if more screen needs to be filled
	D=M
	@DRAW
	D;JGT

	// go back to beginning of loop when screen is filled
	@LOOP
	D;JMP