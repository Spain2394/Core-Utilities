#**Project 2**
##Basic Unix Utilities

Allen Spain (811-565-587)

Nathan Su   (811-342-394)

7/31/2017

________________________________________________________________________________
## Instructions

To compile and link all the programs, type: '$make' into the console.


To run a program, type: '$./[executable]' followed by the corresponding flags
and arguments for the respective program.


To clean, removing the source and executable files, type: '$make clean' into 
the console.


#Reflections:

tail:
Learned you can seek from end of file using a negative offset from the end.

true/false:
Trivial, return 0 for true, 1 for false.

wc:
Program structure makes all the difference, static variables made it so we had easy access
the data, since it would update global variables without excessive pass-by-reference.

env:
Use main prototype which includes the environmental variables array and them iterate over the
length of the array.


