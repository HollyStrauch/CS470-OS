#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <inttypes.h>
#include <math.h>
#include <time.h>
#include <ctype.h>


struct params{
    char * IP;
    int port;
    int timeout;
    int n;
};

struct params * defaults(){
    puts("Using default values");
    struct params * param = malloc(sizeof(struct params));
    param->IP = malloc(sizeof(char) * 255);
    param->IP = "127.0.0.1";
    param->port = 5437;
    param->timeout = 10;
    param->n = 10;

    return param;
}

char * getFileValues(char * line){

    char* token = strtok(line, "=");
    token = strtok(NULL, "=");

    return token;
}

char *trimwhitespace(char *str)
{
    char *end;

    while(isspace((unsigned char)*str)) str++;

    if(*str == 0) return str;

    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;

    end[1] = '\0';

    return str;
}

struct params * readFileParams(FILE * fp) {
    struct params * param = malloc(sizeof(struct params));
    char * buff = malloc(255);
    char * intBuff = malloc(255);
    param->IP = malloc(sizeof(char) * 255);

    fgets(buff, 255, (FILE *) fp);
    fgets(buff, 255, (FILE *) fp);
    param->IP = trimwhitespace(getFileValues(buff));

    fgets(intBuff, 255, (FILE *) fp);
    param->port = atoi(getFileValues(intBuff));

    fgets(intBuff, 255, (FILE *) fp);
    param->timeout = atoi(getFileValues(intBuff));

    fgets(intBuff, 255, (FILE *) fp);
    fgets(intBuff, 255, (FILE *) fp);
    param->n = atoi(getFileValues(intBuff));

    return param;
}



struct params * initParams(char * filepath){
    struct params* param;

    FILE * fp = fopen(filepath, "r");

    if(fp != NULL){
        param = readFileParams(fp);
        fclose(fp);
    }else{
        printf("File not found. ");
        param = defaults();
    }


    return param;
}

int numFactor(uint64_t num, int factor){
    int count = 0;

    while(num % factor == 0){
        count++;
        num /= factor;
    }

    return count;
}

uint64_t baseToDec(uint64_t num, int base){
    uint64_t dec = 0;
    int exp = 0;


    while(num > 0){
        int digit = num % 10;
        uint64_t mult = pow((double)base, (double)exp);
        dec += digit * mult;

        exp++;
        num /= 10;
    }

    return dec;
}


int main(int argc, char *argv[])
{
    struct params * param;

    if(argc != 2){
        param = defaults();
    }else {
        param = initParams(argv[1]);
    }

    printf("IP: %s, Port: %d, time: %d, n: %d\n", param->IP, param->port, param->timeout, param->n);


    int sockfd = 0, num = 0;
    struct sockaddr_in serv_addr;

    uint64_t recvBuff[3];
    memset(recvBuff, (uint64_t )0,sizeof(recvBuff));

    int sendBuff[1];
    memset(sendBuff, 0, sizeof(int));

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(param->port);

    if(inet_pton(AF_INET, param->IP, &serv_addr.sin_addr)<=0)
    {
        printf("\n inet_pton error occured\n");
        return 1;
    }

    time_t start = time(NULL);
    while(1)
    {
        if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) >= 0) {
            break;
        }

        if (time(NULL) - start > param->timeout){
            printf("\n Error: Timeout, connect failed \n");
            return 1;
        }

        sleep(1);
    }

    num = recv(sockfd, recvBuff, sizeof(recvBuff), 0);

    if(num < 0)
    {
        printf("\n Read error \n");
    }else{
        printf("Value: %" PRIu64 "\n", recvBuff[0]);
        printf("Base: %" PRIu64 "\n", recvBuff[1]);
        printf("Prime Factor: %" PRIu64 "\n", recvBuff[2]);

        if(recvBuff[2] < 2){
            puts("Error receiving data");
            return 1;
        }

        uint64_t num = baseToDec(recvBuff[0], (int)recvBuff[1]);
        printf("Dec: %" PRIu64 "\n", num);
        int factors = numFactor(num, (int)recvBuff[2]);
        printf("Exponent: %d\n", factors);
        sendBuff[0] = factors;

        write(sockfd, sendBuff, sizeof(sendBuff));
    }

    close(sockfd);

    return 0;

}