#include <iostream>
#include "SortFile.h"

int main() {

    string file = "/home/holly/dev/CS470/Sorting/test.txt";
    SortFile s = SortFile(",,");


    s.readIn(file);
    s.printVect();

    return 0;
}