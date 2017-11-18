HOMEWORK 3: MATRIX CLASS


NAME:  Raz Reed


COLLABORATORS AND OTHER RESOURCES:
List the names of everyone you talked to about this assignment
(classmates, TAs, ALAC tutors, upperclassmen, students/instructor via
LMS, etc.), and all of the resources (books, online reference
material, etc.) you consulted in completing this assignment.

Yev Grebinskiy (classmate)
Sam Goodwin (classmate)
Rhaad Rabbani (TA)
guy at office hours whose name I don't know
stackoverflow.com
cppreference.com
cplusplus.com

Remember: Your implementation for this assignment must be done on your
own, as described in "Academic Integrity for Homework" handout.


ESTIMATE OF # OF HOURS SPENT ON THIS ASSIGNMENT:  10



ORDER NOTATION:
For each of the functions below, write the order notation O().
Write each answer in terms of m = the number of rows and n = the
number of columns.  You should assume that calling new [] or delete []
on an array will take time proportional to the number of elements in
the array.

get O(1)

set O(1)

num_rows O(1)

get_column O(m)

operator<< O(mn)

quarter O(mn)

operator== O(mn)

operator!= O(mn)

swap_rows O(n)

rref (provided in matrix_main.cpp) O(m^2*n^2)



TESTING & DEBUGGING STRATEGY: 
Discuss your strategy for testing & debugging your program.  
What tools did you use (gdb/lldb/Visual Studio debugger,
Valgrind/Dr. Memory, std::cout & print, etc.)?  How did you test the
"corner cases" of your Matrix class design & implementation?


EXTRA CREDIT: 
Indicate here if you implemented resize() for extra credit.  
Also document the order notation for your resize() function.

I did implement resize(). It runs in O(rows*cols) time, where rows and
cols represent the number of rows and columns to resize the matrix to.

MISC. COMMENTS TO GRADER:  
(optional, please be concise!)

