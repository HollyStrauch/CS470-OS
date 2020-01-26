//
// Created by holly on 1/22/20.
//

#include <iostream>
#include <unistd.h>
#include <wait.h>
#include "SortFile.h"

///Input: single line text file

SortFile::SortFile(string sep){
    this->sep = sep;
}

void SortFile::readIn(string file){
    string hold, token;
    size_t pos = 0;
    int value;
    bool end = true;

    ifstream in (file);

    getline(in, hold);
    while(end){

        if((pos = hold.find(sep)) == string::npos){
            end = false;
        }

        token = hold.substr(0, pos);

        try {
            value = stoi(token);
            unsorted.push_back(value);
        }catch(invalid_argument const&){
            cout << "File contains non-numerical values. Skipping value." << endl;
        }catch(out_of_range const&){
            cout << "out_of_range error.  Number in file is too large" << endl;
        }

        hold.erase(0, pos + sep.length());
    }
    in.close();
}

void SortFile::printVect(){

    for (int i = 0; i < unsorted.size(); i++){
        cout << unsorted[i] << " " << flush;
    }
}

vector<int> SortFile::mergeSort(vector<int> list){
    pid_t c_pid, pid;
    int status;

    c_pid = fork();

    if (c_pid == 0) {
        /* CHILD */
        
    } else if (c_pid > 0) {
        /* PARENT */

        if ((pid = wait(&status)) < 0) {
            perror("wait");
            _exit(1);
        }

    } else {
        perror("fork failed");
        _exit(1);
    }
}

vector<int> SortFile::merge(vector <int> listA, vector <int> listB){
    vector<int> sorted = {};
    int aInd = 0, bInd = 0;

    while (aInd < listA.size() || bInd < listB.size()){
        if (listA[aInd] < listB[bInd]){
            sorted.push_back(listA[aInd]);
            aInd++;
        }else{
            sorted.push_back(listB[bInd]);
            bInd++;
        }
    }

    if(aInd < listA.size()){
        for (int i = aInd; i < listA.size(); i++){
            sorted.push_back(listA[i]);
        }
    }
    else if(bInd < listB.size()){
        for (int i = bInd; i < listB.size(); i++){
            sorted.push_back(listB[i]);
        }
    }

    return sorted;

}

