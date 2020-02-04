**********************************************************README**********************************************************

Author: Holly Strauch
2/5/2020
CS470 Lab2

Program: Sorting

 * Program:  Integers are read in from a file with a specified string separator between each number.  Filename and separator
 * must be given as command line arguments.  The program then sorts the numbers using mergesort with forking processes.
 * PID statements and the status of the array of numbers is given as printouts to the console to track the progress of
 * the algorithm.


“I pledge that I have neither given nor received help from anyone
other than the instructor/TA for all program components included here!”

*******************************************************COMPILE & RUN******************************************************

Compile code in terminal by navigating into the directory containing the files and enter the following command:

    g++ -std=c++0x main.cpp MergeSort.cpp MergeSort.h

Program requires 2 commandline arguments in the specified order:

    <filename> <separator>

Run the code with the following command:

    ./a.out <filename> <separator>

**************************************************Input Requirements********************************************************

The file must contain a single line of characters.  The characters should consist of integers separated by a string of
characters indicated by the second command line argument.  The separator can be of any length > 0.

EX: Separator ^%
    34^%9^%14^%21^%65^%13^%42^%19