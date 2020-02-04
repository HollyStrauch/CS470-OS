/**
 * Author: Holly Strauch
 * 2/5/2020
 * CS 470 Lab 2
 * File: MergeSort.cpp
 *
 * Program:  Integers are read in from a file with a specified string separator between each number.  Filename and separator
 * must be given as command line arguments.  The program then sorts the numbers using mergesort with forking processes.
 * PID statements and the status of the array of numbers is given as printouts to the console to track the progress of
 * the algorithm.
 */

#include <iostream>
#include <unistd.h>
#include <wait.h>
#include "MergeSort.h"


/**
 * \brief Runs a mergesort algorithm using forking processes.
 * @param lst The full list being sorted
 * @param l the lower index (inclusive) for the section of list being sorted
 * @param h the upper index (inclusive) for the section of list being sorted
 */
void MergeSort::mergeSort(int* lst, int l, int h){
    pid_t c1_pid, c2_pid;
    int status;

    cout << "Current PID: " << getpid() << "; Parent PID: " << getppid() << " : ";
    printLst(lst, l, h);

    if (h - l < 1){
        return;
    }
    c1_pid = fork();

    if (c1_pid == 0) {
        // CHILD

        mergeSort(lst, l, (l + h) / 2);
        exit(0);

    } else if (c1_pid > 0) {
         //PARENT
        c2_pid = fork();

        if (c2_pid == 0) {

            mergeSort(lst, (l + h) / 2 + 1, h);
            exit(0);

        } else if (c2_pid < 0){
            perror("fork failed");
            exit(1);
        }

    } else {
        perror("fork failed");
        exit(1);
    }

    // Wait for child processes to finish
    waitpid(c1_pid, &status, 0);
    waitpid(c2_pid, &status, 0);

    // Merge the sorted subarrays
    merge(lst, l, h);
    cout << "After merging branches of " << getpid() << ": ";
    printLst(lst, l, h);


}

/**
 * \brief Prints out the current list within the given bounds
 * @param lst The full list being sorted
 * @param l the lower index (inclusive) for the section of list being sorted
 * @param h the upper index (inclusive) for the section of list being sorted
 */
void MergeSort::printLst(int* lst, int l, int h){

    for(int i = l; i <= h; i++){
        cout << lst[i] << " ";
    }
    cout << endl;

}

/**
 * \brief Merges two consecutive sections of a list into sorted order.
 * @param lst The full list being sorted
 * @param l the lower index (inclusive) for the section of list being sorted
 * @param h the upper index (inclusive) for the section of list being sorted
 */
void MergeSort::merge(int* lst, int l, int h){
    int mid = (l + h) / 2 + 1;
    int tempSize = h - l + 1;
    int sorted[tempSize];
    int aInd = l, bInd = mid, sInd = 0;

    while (aInd < mid && bInd <= h){
        if (lst[aInd] < lst[bInd]){
            sorted[sInd] = lst[aInd];
            aInd++;
            sInd++;
        }else{
            sorted[sInd] = lst[bInd];
            bInd++;
            sInd++;
        }
    }

    if(aInd < mid){
        for (int i = aInd; i < mid; i++, sInd++){
            sorted[sInd] = lst[i];
        }
    }
    else if(bInd <= h){
        for (int i = bInd; i <= h; i++, sInd++){
            sorted[sInd] = lst[i];
        }
    }

    for (int i = l; i <= h; i++){
        lst[i] = sorted[i - l];
    }

}

