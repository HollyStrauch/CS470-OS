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

int* createdSHM(int size){
    int shmID;
    int* lst;
    key_t key = IPC_PRIVATE;
    size_t SHM_SIZE = sizeof(int)*size;

    // Create the segment.
    if ((shmID = shmget(key, SHM_SIZE, IPC_CREAT | 0666)) < 0)
    {
        cout << "Error: shmget ... Failed to create shared memory segment" << endl;
        exit(1);
    }

    // Now we attach the segment to our data space.
    if ((lst = (int *)shmat(shmID, NULL, 0)) == (int *) -1)
    {
        cout << "Error: shmat ... Failed to attach memory segment" << endl;
        exit(1);
    }
    return lst;
}



int main() {
    string sep = ",,";
    int size = 0;
    string file = "/home/holly/dev/CS470/Sorting/test.txt";
    vector<int> temp = readIn(file, ",,");
    size = temp.size();

    int* lst = createdSHM(size);


    makeLst(&temp, lst);
    MergeSort::mergeSort(lst, 0, size - 1);

    for(int i = 0; i < size; i++){
        cout << lst[i] << " ";
    }
    cout << endl;

    return 0;
}

