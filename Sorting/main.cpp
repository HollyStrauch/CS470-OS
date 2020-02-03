#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <iostream>
#include "MergeSort.h"



int* makeLst(vector<int>* temp, int* lst) {
    int size = temp->size();

    for (int i = 0; i < size; i++) {
        lst[i] = temp->at(i);
    }
}


//TODO fix final reading misprint
vector<int> readIn(string file, string sep){
    string hold, token;
    size_t pos = 0;
    int value;
    bool end = true;
    vector <int> temp;

    ifstream in (file);

    getline(in, hold);
    while(end){

        if((pos = hold.find(sep)) == string::npos){
            end = false;
        }

        token = hold.substr(0, pos);

        try {
            value = stoi(token);
            temp.push_back(value);
        }catch(invalid_argument const&){
            cout << "File contains non-numerical values. Skipping value." << endl;
        }catch(out_of_range const&){
            cout << "out_of_range error.  Number in file is too large" << endl;
        }

        hold.erase(0, pos + sep.length());
    }
    in.close();
    return temp;
}


int main() {

    string file = "/home/holly/dev/CS470/Sorting/test.txt";
    SortFile s = SortFile(",,");


    s.readIn(file);
    s.printVect();

    return 0;
}