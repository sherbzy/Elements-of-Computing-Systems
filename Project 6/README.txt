Project 6
Overview and Instructions
Do project 6 from The Elements of Computing Systems - write an Assembler to translate Hack assembly code into Hack machine language code. This project is a bit more challenging than some, but if you follow the approach suggested in lecture, it should be straightforward.

For this project, you may use the programming language of your choice, within reason - it has to be a language I can reasonably run on a linux platform. Languages I can definitely run without difficulty include:

C/C++
Java
Python 3
If you want to use a different language, you do not need to clear it with me first. Do your best to provide me with any necessary build instructions, or any unusual information like a specific compiler or library version I should use. If I can't get things to work, I will have you demonstrate your code on your machine.

Also, on the topic of building: you can include a Makefile if you wish. I should also be able to handle a CMake project. If history is a guide, I will have severe difficulty attempting to build a project using Maven or Gradle, so please avoid those build systems.

It is also perfectly fine to just provide source files and brief instructions on how to compile from the command line. If using Java, Python, or C/C++, you probably don't need to even provide that, unless you are doing something unusual - I can figure it out. Do not provide instructions that start with "install this IDE...".

Grading
You are given 4 test assembler programs, each of which will be worth 24 points. 12 points of each (except for add) are allocated to correctly assembling the non-symbol version of the code, while an additional 12 points will be rewarded for correctly assembling the symbol version. The add program only has a non-symbol version, worth 24 points. Your README is worth 4 points.

Task	Value
README	4 points
Each test assembler program	24 points
Total:	100 points
Submitting the Project
Submit a zip file on Canvas containing:

README
Your source files only (no binaries)
README:

Include your name and the names of anyone who assisted you (other than the instructor). This is also a good place to indicate any other resources you found helpful.
How long did you spend on this assignment (an estimate is fine)?
Any other notes about the project, such as anything interesting you did in your solution, any "a ha" moments brought on by the assignment, etc.
Any build instructions needed for this project.





Project README - 
1. Lauren Sherburne

2. 15 hours

3. I think the hardest part of this project was getting started. Conceptually, I understood what
   needed to be done, but putting it into code form was difficult. I found it interesting that my
   solution required me to have a really good understanding of the many different obstacles that a
   hack program could contain.

4. Compile and run this C++ program using the command line. It requires one command line argument: a
.asm file containing a Hack assembly language program. 
