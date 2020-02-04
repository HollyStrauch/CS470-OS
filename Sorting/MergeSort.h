//
// Created by holly on 1/22/20.
//

#ifndef SORTING_MERGESORT_H
#define SORTING_MERGESORT_H

#include <fstream>
#include <string>
#include <vector>

using namespace std;


class MergeSort {
    static void printLst(int* lst, int l, int h);
public:
    static void mergeSort(int* lst, int l, int h);
    static void merge(int* lst, int l, int h);
};


#endif //SORTING_SORT_H
