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

struct output {
    uint64_t * sendBuff;
    int connfd;
    int factor;

};


long int generate(int base){
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
    struct  output* out = (struct output *)arg;
    int recBuff[1], n;
    int sock = out->connfd;
    memset(recBuff,0, sizeof(recBuff));

    out->sendBuff[2] = (uint64_t )out->factor;


    write(out->connfd, out->sendBuff, 3 * sizeof(uint64_t));

    n = recv(sock, recBuff, sizeof(recBuff), 0);

    close(out->connfd);

    return recBuff[0];
}

struct output * initOutput(){

    struct output * out = malloc(sizeof(struct output));
    out->sendBuff = malloc(3 * sizeof(uint64_t));
    out->factor= 1;


    int base = (rand() % (10 - 1)) + 2;

    printf("base %d\n", base);

    out->sendBuff[0] = generate(base);
    out->sendBuff[1] = base;

    printf("%" PRIu64 "\n", out->sendBuff[0]);
    printf("%" PRIu64 "\n", out->sendBuff[1]);
    out->connfd = 0;

    return out;
}


int main(int argc, char *argv[])
{
    srand(time(NULL));

    struct output * out = initOutput();

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
        int p = atoi(fgets(prime, 255, fp));
        factors[i] = p;

        puts("Listening for client");
        out->connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
        out->factor = p;

        pthread_create(&threads[i], NULL, sendFactor, (void *) out);
    }

    void* exp[16];
    char printBuff[1024];
    sprintf(printBuff, "\nPrime Factorization: ");

    for(int i = 0; i < n; i ++) {
        pthread_join(threads[i], &exp[i]);

        if (exp[i] > 0) {
            sprintf(printBuff + strlen(printBuff), " %d^%d, ", factors[i], (int)exp[i]);
        }
    }

    printf("%s\n", printBuff);
}
