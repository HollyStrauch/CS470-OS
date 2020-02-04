/**
 * Author: Holly Strauch
 * 2/5/2020
 * CS 470 Lab 2
 * File: MergeSort.h
 *
 * Program:  Integers are read in from a file with a specified string separator between each number.  Filename and separator
 * must be given as command line arguments.  The program then sorts the numbers using mergesort with forking processes.
 * PID statements and the status of the array of numbers is given as printouts to the console to track the progress of
 * the algorithm.
 */

#ifndef SORTING_MERGESORT_H
#define SORTING_MERGESORT_H

#include <fstream>
#include <string>
#include <vector>

using namespace std;


class MergeSort {
    static void printLst(int* lst, int l, int h);
    static void merge(int* lst, int l, int h);
public:
    static void mergeSort(int* lst, int l, int h);

};


#endif //SORTING_SORT_H
