#include <iostream>
#include <fstream>
#include <stdio.h>
#include "MT/mt19937ar.h"

using namespace std;

void createFile(string filename, int matrixSize){
    ofstream myfile (filename, ios::binary);


    for(int i = 0; i < matrixSize*matrixSize; i++){
        myfile << rand() % 2;
    }

    myfile.close();
}

int main (int argc, char** argv) {

    //createFile("test.txt", 4);

    fstream File("test.txt", ios::in | ios::out );

    File.seekg(2, ios::beg);
    char F[2];
    File.read(F, 1);
    F[1] = 0;
    cout <<F<< endl;
    File.close();
}
