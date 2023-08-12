Project 3
Overview and Instructions
Do project 3 from The Elements of Computing Systems - use the gates you built in Projects 1 and 2, plus the provided DFF chip to create a 1-bit register (solution given in lecture and in the book), a 16-bit register, RAM, and a program counter.

If you haven't already done so, you should probably read pages 27-35 of the Hardware Simulator tutorial found at https://www.nand2tetris.org/software before completing this project.

Grading
There are 8 gates, which will each be worth (95/8) points. They will all be graded pass/fail according to the test scripts provided to you.

Task	Value
README	5 points
Each (of 8) gates	(95/8) points each
Total:	100 points
Submitting the Project
Submit a zip file on Canvas containing:

README
Your .hdl files
This project is split into two parts, a and b, mostly for efficiency in simulating all the chips you are constructing. You may submit your code as a .zip file of these two directories, or just your .hdl files, or whatever works best for you.

README:

Include your name and the names of anyone who assisted you (other than the instructor). This is also a good place to indicate any other resources you found helpful.
How long did you spend on this assignment (an estimate is fine)?
Any other notes about the project, such as anything interesting you did in your solution, any "a ha" moments brought on by the assignment, etc.




Project README - 
1. Lauren Sherburne

2. 4-5 hours

3. Once I figured out the first Ram, the other Rams were easy to build since they are accomplishing
the same goal but on a larger scale. While building the program counter, I originally checked the
load, inc, and reset bits in no specific order. However, I eventually determined that the reset bit
must be last checked, otherwise the logic was incorrect. In the if-statement that describes the
program counter, the reset bit is checked first. This means that no matter what the inc or load bits
hold, the reset bit will override these and reset the output. Once I realized my mistake, I
rearranged my mux16s and solved the problem.
