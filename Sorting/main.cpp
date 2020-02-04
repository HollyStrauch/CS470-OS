/**
 * Author: Holly Strauch
 * 2/5/2020
 * CS 470 Lab 2
 * File: main.cpp
 *
 * Program:  Integers are read in from a file with a specified string separator between each number.  Filename and separator
 * must be given as command line arguments.  The program then sorts the numbers using mergesort with forking processes.
 * PID statements and the status of the array of numbers is given as printouts to the console to track the progress of
 * the algorithm.
 */



#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <iostream>
#include "MergeSort.h"


/**
 * \brief Creates an array from a vector
 * @param temp The vector with integer values
 * @param lst The integer array to transfer values into
 * @return
 */
void makeLst(vector<int>* temp, int* lst) {
    int size = temp->size();

    for (int i = 0; i < size; i++) {
        lst[i] = temp->at(i);
    }
}


/**
 * \brief Reads in values from a file and parses out the integers from the separator string
 * @param file The filename
 * @param sep The separator between integers used in the file
 * @return A vector of all the integers in the file
 */
vector<int> readIn(string file, string sep){
    string hold, token;
    size_t pos = 0;
    int value;
    bool end = true;
    vector <int> temp;

    ifstream in (file);
    getline(in, hold);
    while(end && hold != ""){

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

/**
 * \creates shared memory segment and assigns it to an integer pointer
 * @param size The size of the shared memory segment to be created
 * @return integer point lst to the shared memory
 */
int* createdSHM(int size){
    int shmID;
    int* lst;
    key_t key = IPC_PRIVATE;
    size_t SHM_SIZE = sizeof(int)*size;

    if ((shmID = shmget(key, SHM_SIZE, IPC_CREAT | 0666)) < 0)
    {
        cout << "Error: shmget ... Failed to create shared memory segment" << endl;
        exit(1);
    }

    if ((lst = (int *)shmat(shmID, NULL, 0)) == (int *) -1)
    {
        cout << "Error: shmat ... Failed to attach memory segment" << endl;
        exit(1);
    }

    return lst;
}

void printLst(int* lst, int size){

    for(int i = 0; i < size; i++){
        cout << lst[i] << " ";
    }
    cout << endl;
}

//"/home/holly/dev/CS470/Sorting/test.txt"
int main(int argc, char **argv) {
    int size = 0;
    string file = argv[1];
    string sep = argv[2];
    vector<int> temp = readIn(file, sep);
    size = temp.size();

    if (size == 0){
        cout << "Empty File.  Exiting Program." << endl;
        return 0;
    }

    int* lst = createdSHM(size);
    makeLst(&temp, lst);
    MergeSort::mergeSort(lst, 0, size - 1);

    cout << "FINAL LIST: " << endl;
    printLst(lst, size);

    return 0;
}

