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
#include <fcntl.h>

int PORT = 5437;
int n = 10;
int activeClient = 0;
char * printBuff;
uint64_t bigNum;
uint64_t base;
uint64_t factor = 2;
uint64_t bigNumDec = 10;
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;


long int generate(){
    char num[5];
    char *eptr;
    memset(num, '0', sizeof(num));

    for(int i = 0; i < 5; i++){
        num[i] = (char) (rand() % (base) + 48);
    }

    long int val = strtol(num, &eptr, 10);
    return val;
}

void *sendFactor(void *arg){
    puts("in string");
    int connfd = (int)arg;
    printf("connfd %d\n", connfd);

    uint64_t recBuff[2];
    memset(recBuff,0, sizeof(recBuff));

    uint64_t * sendBuff = malloc(3 * sizeof(uint64_t));


    while(1) {
        pthread_mutex_lock(&mut);

        if(bigNumDec == 10) {
            sendBuff[0] = bigNum;
            sendBuff[1] = base;
        }else{
            sendBuff[0] = bigNumDec;
            sendBuff[1] = 10;
        }

        sendBuff[2] = factor;

        /*
        printf("Value: %" PRIu64 "\n", sendBuff[0]);
        printf("Base: %" PRIu64 "\n", sendBuff[1]);
        printf("Prime Factor: %" PRIu64 "\n", sendBuff[2]);
*/
        if(write(connfd, sendBuff, 3 * sizeof(uint64_t)) < 1){
            printf("Could not write to client, disconnecting\n");
            break;
        }

        if(recv(connfd, recBuff, sizeof(recBuff), 0) < 1){
            printf("Client disconnected\n");
            break;
        }

        if(recBuff[0] > 0) {
            sprintf(printBuff + strlen(printBuff), " %ld^%ld, ", factor, recBuff[0]);
        }

        bigNumDec = recBuff[1];
        bigNumDec /= (uint64_t)pow((double)factor, (double)recBuff[0]);

        if(factor == 2) {
            factor++;
        }else{
            factor += 2;
        }

        pthread_mutex_unlock(&mut);
    }

    activeClient--;
    pthread_mutex_unlock(&mut);

    puts("Exiting Thread");
    close(connfd);

}

void initOutput(){

    base = (rand() % (10 - 1)) + 2;
    bigNum = (uint64_t )generate();

    printf("bigNum %" PRIu64 "\n", bigNum);
    printf("base %" PRIu64 "\n", base);

}

void * endCondition(void * arg){

}

int main(int argc, char *argv[])
{
    srand(time(NULL));
    int start = 1;

    int sockfd = 0;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    fcntl(sockfd, F_SETFL, O_NONBLOCK);

    struct sockaddr_in serv_addr;
    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(PORT);

    bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));


    listen(sockfd, 10);

    pthread_t threads[16];

    puts("before for loop");
    for(int i = 0; i < n; i ++){
        puts("Generating new number");
        initOutput();
        printBuff = malloc(1024 * sizeof(char));
        sprintf(printBuff, "\nPrime Factorization of %ld base %ld: ", bigNum, base);

        do{
            printf("activeClient %d, factor: %ld, bigNumDec %ld\n\n", activeClient, factor, bigNumDec);
            int connfd = accept(sockfd, (struct sockaddr *) NULL, NULL);

            if( connfd < 0){

                continue;
            }

            if (connfd < 1){
                break;
            }

            puts("New client accepted");
            activeClient++;

            pthread_create(&threads[i], NULL, sendFactor, (void *) connfd);
            start = 0;
        }while(start || (activeClient > 0 && factor < bigNumDec / 2));

        pthread_mutex_lock(&mut);
        puts("Outside while");
        if(bigNumDec > 1){
            sprintf(printBuff + strlen(printBuff), " %ld^1, ", bigNumDec);
        }

        printf("%s\n\n", printBuff);
        free(printBuff);
        pthread_mutex_unlock(&mut);
    }

    for(int i = 0; i < n; i ++) {
        pthread_join(threads[i], NULL);
    }

}
