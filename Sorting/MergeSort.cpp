//
// Created by holly on 1/22/20.
//

#include <iostream>
#include <unistd.h>
#include <wait.h>
#include "MergeSort.h"

///Input: single line text file


void MergeSort::mergeSort(int* lst, int l, int h){
    pid_t c1_pid, c2_pid;
    int status;

    if (h - l < 1){
        return;
    }
    c1_pid = fork();

    if (c1_pid == 0) {
        // CHILD
        mergeSort(lst, l, (l + h) / 2) ;
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

}


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

