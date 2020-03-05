#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <math.h>
#include <values.h>
#include <inttypes.h>
#include <pthread.h>

int PORT = 5437;
int n = 10;
char * printBuff;
uint64_t bigNum;
uint64_t base;
uint64_t factor = 2;
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;


long int generate(){
    char num[18];
    char *eptr;
    memset(num, '0', sizeof(num));

    for(int i = 0; i < 18; i++){
        num[i] = (char) (rand() % (base) + 48);
    }

    long int val = strtol(num, &eptr, 10);
    return val;
}

void *sendFactor(void *arg){
    puts("in string");
    int connfd = (int)arg;
    printf("connfd %d\n", connfd);

    int recBuff[1], n;
    memset(recBuff,0, sizeof(recBuff));

    uint64_t * sendBuff = malloc(3 * sizeof(uint64_t));

    while(1) {
        pthread_mutex_lock(&mut);
        sendBuff[0] = bigNum;
        sendBuff[1] = base;
        sendBuff[2] = factor;

        printf("Value: %" PRIu64 "\n", sendBuff[0]);
        printf("Base: %" PRIu64 "\n", sendBuff[1]);
        printf("Prime Factor: %" PRIu64 "\n", sendBuff[2]);

        if(write(connfd, sendBuff, 3 * sizeof(uint64_t)) < 1){
            printf("Could not write to client, disconnecting");
            break;
        }

        if(recv(connfd, recBuff, sizeof(recBuff), 0) < 1){
            printf("Client disconnected, exiting thread.");
            break;
        }

        if(recBuff[0] > 0) {
            sprintf(printBuff + strlen(printBuff), " %ld^%d, ", factor, recBuff[0]);
        }
        factor++;
        pthread_mutex_unlock(&mut);
    }

    close(connfd);

}

void initOutput(){

    base = (rand() % (10 - 1)) + 2;
    bigNum = (uint64_t )generate();

    printf("bigNum %" PRIu64 "\n", bigNum);
    printf("base %" PRIu64 "\n", base);

}


int main(int argc, char *argv[])
{
    srand(time(NULL));



    int listenfd = 0;
    struct sockaddr_in serv_addr;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(PORT);

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    listen(listenfd, 10);

    pthread_t threads[16];

    FILE * fp = fopen("primes.txt", "r");
    char prime[255];
    int factors[n];
    memset(&factors, 0, sizeof(factors));


    for(int i = 0; i < n; i ++){
        initOutput();
        printBuff = malloc(1024 * sizeof(char));

        while(1) {
            puts("Listening for client");
            int connfd = accept(listenfd, (struct sockaddr *) NULL, NULL);

            if (connfd < 1){
                break;
            }

            pthread_create(&threads[i], NULL, sendFactor, (void *) connfd);
        }

        void* exp[16];
        char printBuff[1024];
        sprintf(printBuff, "\nPrime Factorization: ");

        for(int i = 0; i < n; i ++) {
            pthread_join(threads[i], NULL);
        }

        printf("%s\n", printBuff);
        free(printBuff);
    }


}
