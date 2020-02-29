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

long int MAX = LONG_MAX;
long int MIN = 36028797018963967;

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

int main(int argc, char *argv[])
{
    srand(time(NULL));
    int base = (rand() % (10 - 1)) + 2;
    uint64_t sendBuff[3];
    memset(sendBuff,(uint64_t )0, sizeof(sendBuff));
    sendBuff[0] = genNum(base);
    sendBuff[1] = base;

    int recBuff[1];
    memset(recBuff,0, sizeof(recBuff));

    printf("%" PRIu64 "\n", sendBuff[0]);
    printf("%" PRIu64 "\n", sendBuff[1]);

    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr;


    time_t ticks;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));


    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5437);

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    listen(listenfd, 10);

    int n = 0;
    for(int i = 2; i < 18; i ++){
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);

        sendBuff[2] = i;
        write(connfd, sendBuff, 3 * sizeof(uint64_t));

        n = recv(connfd, recBuff, sizeof(recBuff), 0);

        printf("Factor %d appears %d times\n", i, recBuff[0]);
        close(connfd);
        sleep(1);
    }

}