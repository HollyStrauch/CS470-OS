#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include "MT/mt19937ar.h"

#define FILENAME "test.txt"

int MATRIX = 0;
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t pmut = PTHREAD_MUTEX_INITIALIZER;

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
    fflush(stdout);
    fclose(fp);
}

void createFile(){
    FILE *fp;
    fp = fopen (FILENAME, "wb");

    for(int i = 0; i < MATRIX*MATRIX; i++){
        fputc((rand() % 2) + 48, fp);
    }

    fclose(fp);
}


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
            fclose(fp);
            return 0;
        }
    }

    fclose(fp);
    return 1;

}

int findVal(int loc){
    FILE *fp;
    fp = fopen (FILENAME, "rb");
    int c0 = 0, c1 = 0;

    int row = loc / MATRIX;
    int col = loc % MATRIX;

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


// thread function
void *setRow(void * arg)
{
    int r = rand() % MATRIX;
    int ThreadID = (int)arg;
    printf("Thread [%d] assigned row & column %d\n",ThreadID, r);
    fflush(stdout);

    FILE *fp = fopen(FILENAME, "r+b");

    //row
    for(int i = r * MATRIX; i < r * MATRIX + MATRIX; i++)
    {
        pthread_mutex_lock(&mut);
        int new = findVal(i);
        fseek( fp, i, SEEK_SET );
        fputc((unsigned char) new, fp);
        pthread_mutex_unlock(&mut);
    }

    //column
    for(int i = 0; i < MATRIX; i++)
    {
        pthread_mutex_lock(&mut);
        int new = findVal(i * MATRIX + r);
        fseek( fp, i * MATRIX + r, SEEK_SET );
        fputc((unsigned char) new, fp);
        pthread_mutex_unlock(&mut);
    }
    fclose(fp);

    pthread_mutex_lock(&pmut);
    printf("Thread [%d] before finishing\n",ThreadID);
    printMatrix();
    pthread_mutex_unlock(&pmut);


    pthread_exit(NULL);
}


int main (int argc, char** argv) {
    pid_t pid;
    struct flock lock;
    int fd;

    fd = open (FILENAME, O_WRONLY);
    memset(&lock, 0, sizeof(lock));
    lock.l_type = F_WRLCK;
    fcntl(fd, F_SETLKW, &lock);

    srand(time(NULL));

    puts("Enter the size of the matrix:");
    MATRIX = getInput();

    puts("Enter the number of threads:");
    int threads = getInput();

    pthread_t threads1[threads];

    createFile();
    printMatrix();

    while (1) {
        pid = fork();

        if (pid == 0){
            int status = checkAll();
            if (status){
                printf("Ending condition met, locking file\n");
                fcntl (fd, F_SETLKW, &lock);
            }

            return status;

        }else if (pid < 0){
            printf("Error forking process");
            return 1;
        }else {

            int status = 0;
            wait(&status);
            status = status>>8;

            if (status){
                break;
            }

            for (int i = 0; i < threads; i++) {
                pthread_create(&threads1[i], NULL, setRow, (void *) i);
            }

            for (int i = 0; i < threads; i++) {
                pthread_join(threads1[i], NULL);
            }
        }
    }
    close(fd);
    return 0;
}
