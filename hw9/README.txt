HOMEWORK 9: MINIBLAST


NAME: Raz Reed


COLLABORATORS AND OTHER RESOURCES:
List the names of everyone you talked to about this assignment
(classmates, TAs, ALAC tutors, upperclassmen, students/instructor via
LMS, etc.), and all of the resources (books, online reference
material, etc.) you consulted in completing this assignment.

Yevgeniy Grebinskiy (classmate)
Samuel Goodwin (classmate)
Rhaad Rabbani (TA)
Some mentors/TAs at the library on Thursday

Remember: Your implementation for this assignment must be done on your
own, as described in "Academic Integrity for Homework" handout.



ESTIMATE OF # OF HOURS SPENT ON THIS ASSIGNMENT: 8


HASH FUNCTION DESCRIPTION

My hash function is borrowed from http://www.partow.net/programming/hashfunctions/. I use their code to generate a hash number given a key, which I then do quadratic probing on to find an index in my table at which to store that key.

HASH TABLE IMPLEMENTATION

My hash table is implemented with a vector of Kmer objects. Kmer is a simple custom class I designed that has two members: a string called "key" and a vector of integers called "positions". key is a sequence of k characters taken from the genome, and positions is a vector containing each location/index of key in the genome. I use the hash function and quadratic probing on a key to find the index of the corresponding Kmer object in the table.


ANALYSIS OF PERFORMANCE OF YOUR ALGORITHM:
(order notation & concise paragraph, < 200 words)

L = length of the genome sequence
q - query length
p - number of different locations where key is found
k - key size (k-mer size)

How much memory will the map data structure require (order notation for memory use)?

O(L*k)

What is the order notation for performance (running time) of each of
the commands?

genome -- O(1)
table_size -- O(1)
occupancy -- O(1)
kmer -- O(L*k*p)
query -- O((q-k)*p)
quit -- O(1)

EXTRA CREDIT
Add a new command to implement the database using one of the other
 data structures that we have covered so far in the course: 
vectors, lists, arrays etc. 

Compare the performance your alternative method to the homework method
by making a table of run times for each of the genomes and query 
sets provided with the homework  and compare the times to build
the index and the times to process the queries. 
Document any new commands you have added in your README file.



MISC. COMMENTS TO GRADER:  
(optional, please be concise!)


