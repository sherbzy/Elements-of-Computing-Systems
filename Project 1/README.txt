Project 1
Overview
Do project 1 from The Elements of Computing Systems - create some basic building blocks from NAND gates: NOT, AND, OR, XOR, MUX, DMUX and multi-bit versions of the same. There are a lot of gates to create, but once you get the hang of it, most will go quickly. Give yourself time to understand the HDL (Hardware Description Language) and the supporting software (Hardware Simulator).

Instructions
Download the entire nand2tetris software suite from https://www.nand2tetris.org/software and unzip it on your machine somewhere. This contains both the starter files for each project, and the software tools (the various simulators and such) needed to complete each project.

I strongly recommend you next work through the Hardware Simulator tutorial (which can be found on the same page linked above) up through slide 26. You can experiment with the XOR gate, already built for you, in the projects/demo subdirectory (from the nand2tetris download). Then try to build a simple gate to start with, like NOT (in general, you should build them in the order listed in the project description in your textbook).

You may wish to spend some time working out your solutions on paper before working in HDL. Write down some Boolean expressions matching the truth tables for each gate, and see if you can simplify them. Then draw the gate circuit to match the Boolean expression, labeling each input and output. Once you've done all of that, it should be straightforward to implement your circuit in HDL.

Note: if you haven't purchased the textbook yet, you can read chapter 1 and the instructions for project 1 here: https://www.nand2tetris.org/course. (Note that not all chapters are available online. Get the book.)

Grading
For this project, gates will be graded pass/fail according to the test scripts provided.

Task	Value
README	5 points
Each (of 15) gates	(95/15) points each
Total:	100 points
Submitting The Project
Submit a zip file on Canvas containing:

README
A directory containing your .hdl files
For hardware projects your solution only need consist of the .hdl files you created and your README. For simplicity, if you include other files such as .cmp or .tst files, that is fine, they will be ignored. I will test each of your gates separately against the built-in chip implementations, so a problem with one of your solutions will not cause problems with any of your other solutions.

README:

Include your name and the names of anyone who assisted you (other than the instructor). This is also a good place to indicate any other resources you found helpful.
How long did you spend on this assignment (an estimate is fine)?
Any other notes about the project, such as anything interesting you did in your solution, any "a ha" moments brought on by the assignment, etc.




Project README - 
1. Lauren Sherburne

2. 7 hours

3. I did have a difficult time trying to change how I think about figuring out the logic gates 
because originally I was stuck trying to use if-statement logic. However, once I figured out some of
the gates, the rest became much easier. I do think the whole process would have been easier if I had
visualizations of the gates (I was thinking about making a few paper cutouts to mess with).
