//
// Created by holly on 1/22/20.
//

#ifndef SORTING_SORTFILE_H
#define SORTING_SORTFILE_H

#include <fstream>
#include <string>
#include <vector>

using namespace std;


class SortFile {
private:
    string sep;
    vector <int> unsorted;

public:
    SortFile(string sep);
    void readIn(string file);
    vector<int> mergeSort(vector<int> list);
    vector<int> merge(vector <int> listA, vector <int> listB);
    void printVect();

};


#endif //SORTING_SORT_H
