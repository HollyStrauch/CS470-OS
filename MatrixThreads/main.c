#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include "MT/mt19937ar.h"

#define FILENAME "test.txt"

int MATRIX = 0;

void createFile(int matrixSize){
    FILE *fp;
    fp = fopen (FILENAME, "wb");

    for(int i = 0; i < matrixSize*matrixSize; i++){
        fputc((rand() % 2) + 48, fp);
    }

    fclose(fp);
}


int getInput(){
    int input = 0;
    char buf[3];

    while( input <= 0) {
        printf("Please enter an integer value:\n");
        fgets(buf, 2, stdin);
        input = atoi(buf);
    }

    return input;
}

int checkAll(){
    unsigned char c0, c1;

    FILE *fp;
    fp = fopen (FILENAME, "r+b");

    fseek( fp, 0, SEEK_SET );
    c0 = fgetc(fp);

    for (int i = 0; i < MATRIX * MATRIX; i++){
        fseek( fp, i, SEEK_SET );
        c1 = fgetc(fp);
        if (c0 != c1){
            return 0;
        }
    }

    return 1;

}

int findVal(int loc){
    FILE *fp;
    fp = fopen (FILENAME, "r+b");
    int c0 = 0, c1 = 0;

    for(int i = -1; i <= 1; i++){
        for(int j = 1; j <= 1; j ++){
            if (i == 0 && j == 0) continue;

            int nRow = loc + i;
            int nCol = loc + j;

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

    if (c0 > c1){
        return 50;
    }
    return 51;
}


// thread function
void *setRow(void * arg)
{
    int r = rand() % MATRIX;
    ///set back to r
    unsigned char a = 3 + 48;
    int ThreadID = (int)arg;
    printf("Thread [%d] assigned row & column %d\n",ThreadID, r);
    fflush(stdout);

    FILE *fp = fopen(FILENAME, "r+b");

    //row
    for(int i = r * MATRIX; i < r * MATRIX + MATRIX; i++)
    {
        int new = findVal(i);
        fseek( fp, i, SEEK_SET );
        fputc((unsigned char) new, fp);
    }

    //column
    for(int i = 0; i < MATRIX; i++)
    {
        int new = findVal(i);
        fseek( fp, i, SEEK_SET );
        fputc((unsigned char) new, fp);
    }

    printf("Thread [%d] before finishing\n",ThreadID);
    fflush(stdout);

    fclose(fp);
    pthread_exit(NULL);
}


int main (int argc, char** argv) {
    srand (time(NULL));
    int nRet1;

    puts("Enter the size of the matrix:");
    MATRIX = getInput();
    puts("Enter the number of threads:");
    int threads = getInput();

    pthread_t threads1[threads];

    createFile(MATRIX);

   // while (!checkAll()) {
        for (int i = 0; i < threads; i++) {
            nRet1 = pthread_create(&threads1[i], NULL, setRow, (void *) i);
        }

        for (int i = 0; i < threads; i++) {
            pthread_join(threads1[i], NULL);
        }
 //   }
/*
    unsigned char c;
    fseek( fp, 0, SEEK_SET );
    c = fgetc(fp);
    unsigned char a = '4';
    fputc(a, fp);

    fseek( fp, 1, SEEK_SET );
    c = fgetc(fp);
    a = '5';
    fputc(a, fp);
    printf("%c", c);
*/
}
