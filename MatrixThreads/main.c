/**
 * Author: Holly Strauch
 * 2/18/2020
 * CS 470 Lab 3
 * File: main.c
 *
 * Description:  Code creates a binary file with 0s and 1s representing a square matrix.
 * Threading is used to randomly select then change an entry of the file to whichever
 * value around it occurs the most often.  The process stops when the entire matrix is
 * 1's or 0's
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include "MT/mt19937ar.h"

#define FILENAME "matrix.bin"

//Global
int MATRIX = 0;
int END = 1;
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

/**
 * Reads values from the file and prints them off in square matrix format
 */
void printMatrix(){
    unsigned char c;
    FILE *fp;
    fp = fopen (FILENAME, "rb");

    for(int i = 0; i < MATRIX; i++){
        for(int j = 0; j < MATRIX; j++){
            fseek( fp, i*MATRIX + j, SEEK_SET );
            c = fgetc(fp);
            printf("%c ", c);
        }
        printf("\n");
    }
    printf("\n");
    fflush(stdout);
    fclose(fp);
}

/**
 * Creates a file and fills it randomly with 0s and 1s
 */
void createFile(){
    FILE *fp;
    fp = fopen (FILENAME, "wb");

    for(int i = 0; i < MATRIX*MATRIX; i++){
        fputc((rand() % 2) + 48, fp);
    }

    fclose(fp);
}

/**
 * Gets integer input from the user
 * @return returns valid integer input
 */
int getInput(){
    int input = 0;
    char buf[100];

    while( input <= 0) {
        printf("Please enter an integer value:\n");
        fgets(buf, 100, stdin);
        input = atoi(buf);
    }

    return input;
}


/**
 * Looks at all neighbors of a specific location and finds if there are more 1s or 0s
 * @param row The row of the specific location
 * @param col the column of the specific location
 * @return the value that occurs most, tie breaker goes to 0
 */
int findVal(int row, int col){
    FILE *fp;
    fp = fopen (FILENAME, "rb");
    int c0 = 0, c1 = 0;

    for(int i = -1; i <= 1; i++){
        for(int j = -1; j <= 1; j ++){
            if (i == 0 && j == 0) continue;

            int nRow = row + i;
            int nCol = col + j;

            if (nRow < 0 || nRow > MATRIX - 1 || nCol < 0 || nCol > MATRIX - 1) continue;

            int offset = nCol + nRow * MATRIX;
            fseek( fp, offset, SEEK_SET );

            unsigned char c = fgetc(fp);

            if(c == '0'){
                c0++;
            }else if(c == '1'){
                c1++;
            }

        }
    }


    fclose(fp);
    if (c0 >= c1){
        return 48;
    }
    return 49;
}

/**
 * THREADING FUNCTION
 * Loops through file to check if all the values are the same
 */
void checkAll(void * arg){
    puts("Control Thread created");
    fflush(stdout);
    unsigned char c0, c1;

    fflush(stdout);
    while(END) {
        int dif = 1;

        FILE *fp;
        fp = fopen(FILENAME, "r+b");

        fseek(fp, 0, SEEK_SET);
        c0 = fgetc(fp);

        for (int i = 0; i < MATRIX * MATRIX; i++) {
            fseek(fp, i, SEEK_SET);
            c1 = fgetc(fp);
            if (c0 != c1){
                dif = 0;
                break;
            }
        }

        if (dif){
            fclose(fp);
            END = 0;
            break;
        }
        if (fp != NULL) {
            fclose(fp);
        }
    }

    pthread_exit(NULL);
}

/**
 * //THREADING FUNCTION
 * Generates a random location and changes the value based on neighbors
 * @param arg: the assigned thread number
 */
void *setVal(void * arg)
{
    int ThreadID = (int) arg;
    while(END) {
        int row = genrand_int32() % MATRIX;
        int col = genrand_int32() % MATRIX;
        int loc = row * MATRIX + col;

        FILE *fp = fopen(FILENAME, "r+b");

        pthread_mutex_lock(&mut);
        int new = findVal(row, col);
        fseek(fp, loc, SEEK_SET);
        fputc((unsigned char) new, fp);
        fflush(fp);

        printf("Thread [%d] assigned row %d column %d: \n", ThreadID, row, col);
        printMatrix();
        fflush(stdout);

        pthread_mutex_unlock(&mut);

        if (fp != NULL) {
            fclose(fp);
        }
    }

    puts("Leaving Thread");
    pthread_exit(NULL);
}


/**
 * Gets user input, creates matrix file, creates threads, joins threads;
 */
int main (int argc, char** argv) {
    init_genrand(time(NULL));

    puts("Enter the size of the matrix:");
    MATRIX = getInput();

    puts("Enter the number of threads:");
    int threads = getInput();

    pthread_t threads1[threads];
    pthread_t endCondition;

    printf("Creating File %s\n", FILENAME);
    createFile();
    puts("Starting Matrix:");
    printMatrix();

    pthread_create(&endCondition, NULL, checkAll,(void *) 0);

    for (int i = 0; i < threads; i++) {
        pthread_create(&threads1[i], NULL, setVal, (void *) i + 1);
    }

    for (int i = 0; i < threads; i++) {
        pthread_join(threads1[i], NULL);
    }

    pthread_join(endCondition, NULL);

    return 0;
}
