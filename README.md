# **Core Utilies**
## Basic Unix Utilities

________________________________________________________________________________
## Instructions

To compile and link all the programs, type: '$make' into the console.


To run a program, type: ```$./[executable]``` followed by the corresponding flags
and arguments for the respective program.


To clean, removing the source and executable files, type: '$make clean' into 
the console.

________________________________________________________________________________

## Reflections

head:
Seemingly straightforward at first, modifying example code from class, but 
some difficulty in accidentally printing extra characters behind the bash 
prompt after it finished. 

ln:
Straightforward, looked further into the difference between symbolic vs hard 
links.

ls:
The actual printing out of files and their associated information proved to
be fairly easy. Sorting the files into alphabetical order was more difficult and
eliminating memory leaks ultimately took up the bulk of the time spent on this.

mkdir:
Not too bad, learned how to convert to octal to set the mode of folders.

env:
Use main prototype which includes the environmental variables array and them 
iterate over the length of the array.

tail:
Learned you can seek from end of file using a negative offset from the end.

true/false:
Trivial, return 0 for true, 1 for false.

wc:
Program structure makes all the difference, static variables made it so we had 
easy access the data, since it would update global variables without excessive
pass-by-reference.

pwd:
Proved much easier than the other extra credit program. Simply printed out what
was retrieved from get_current_dir_name().

