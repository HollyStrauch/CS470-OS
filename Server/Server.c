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

long int MAX = LONG_MAX;
long int MIN = 36028797018963967;

struct output {
    uint64_t * sendBuff;
    int connfd;
    int factor;

};

int isValid(long int num, int base) {
    do  {
        int digit = num % 10;
        if (digit >= base) {
            return 1;
        }
        num /= 10;
    } while (num > 0);

    return 0;

}

long int genNum(int base){

    long int DIF = MAX - MIN;
    long int num = 0;


    do {
        double r = (double)rand()/(double)RAND_MAX;

        num = (long int) (r * (double) DIF + MIN);
    }while(isValid(num, base));

    return num;
}
/*
void * sendFactor(void * arg){
    struct output * out = (struct output *)arg;
    int recBuff[1], n;
    memset(recBuff,0, sizeof(recBuff));

    out->sendBuff[2] = out->factor;

    printf("%" PRIu64 "\n", out->sendBuff[0]);
    printf("%" PRIu64 "\n",  out->sendBuff[1]);
    printf("%" PRIu64 "\n",  out->sendBuff[2]);

    write(out->connfd, out->sendBuff, 3 * sizeof(uint64_t));

    n = recv(out->connfd, recBuff, sizeof(recBuff), 0);

    printf("Factor %d appears %d times\n", out->factor, recBuff[0]);

    return (void *)recBuff[0];
}
*/
struct output * initOutput(){

    struct output * out = malloc(sizeof(struct output));
    out ->sendBuff = malloc(3 * sizeof(uint64_t));


    int base = (rand() % (10 - 1)) + 2;

    out->sendBuff[0] = genNum(base);
    out->sendBuff[1] = base;

    return out;
}

int main(int argc, char *argv[])
{
    srand(time(NULL));

    struct output * out = initOutput();
    printf("%" PRIu64 "\n", out->sendBuff[0]);
    printf("%" PRIu64 "\n", out->sendBuff[1]);



    int listenfd = 0;
    struct sockaddr_in serv_addr;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));


    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5437);

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    listen(listenfd, 10);

    int n = 0;

    pthread_t threads1[16];

    char printBuff[1024];
    sprintf(printBuff, "Prime Factorization: ");
    for(int i = 2; i < 18; i ++){

        out->connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
        out->factor = i;

        int exp = 0;
       // pthread_create(&threads1[i], NULL, sendFactor, (void *)out);
        printf("exp: %d\n", exp);

        int recBuff[1];
        memset(recBuff,0, sizeof(recBuff));
        out->sendBuff[2] = out->factor;
        write(out->connfd, out->sendBuff, 3 * sizeof(uint64_t));

        n = recv(out->connfd, recBuff, sizeof(recBuff), 0);

        printf("Factor %d appears %d times\n", out->factor, recBuff[0]);

        if(recBuff[0] > 0) {
            sprintf(printBuff + strlen(printBuff), " %d^%d,", out->factor, recBuff[0]);
        }
        close(out->connfd);
        sleep(1);
    }

    printf("%s\n", printBuff);

}