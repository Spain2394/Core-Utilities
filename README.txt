#Project 1: Matrix

7/7/2017
Allen Spain (811-565-587)

## Instructions

To compile and link my code, type:
make


To run my code, type:
./p1


To clean, type: 
make clean


## Reflection

Matrix.cpp: 

Memory allocation is key. Each constructor is used to allocate 
memory for the new instance. By using a 2D pointer attribute 
in the Matrix class, a dynamically allocated 2D array is achievable.
This dynamic array could be modified by dereferencing the pointer twice.
overloading operators allowed for unique features, which provide the user 
the ability to perform both scalar and matrix arithmetic, without hastle. 

Visualizing and manipuplating a double pointer posed challenging. Also I learned
through the iterative tuning proccess that the initializer list cannot be used like
a 2D pointer but insttead, should be treated like a list of lists, that can 
be assigned using by incrementing the list.begin pointer to access the various
list, each list of list can then be assigned by dereferencing each succesive list.begin() 
pointer. Going forward, pointers can be used to keep track of locations in memory, so 
that data being used can tracked, and modified dynamically. This memory can also be freed
by deleting it after the program has run its course.
