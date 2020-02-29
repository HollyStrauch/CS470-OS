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
    if(argc != 2)
    {
        printf("\n Usage: %s <ip of server> \n",argv[0]);
        return 1;
    }

    int sockfd = 0, n = 0;
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
    serv_addr.sin_port = htons(5437);

    if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)
    {
        printf("\n inet_pton error occured\n");
        return 1;
    }

    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\n Error : Connect Failed \n");
        return 1;
    }

    n = read(sockfd, recvBuff, sizeof(recvBuff)-1);
    if(n < 0)
    {
        printf("\n Read error \n");
    }else{
        printf("%" PRIu64 "\n", recvBuff[0]);
        printf("%" PRIu64 "\n", recvBuff[1]);
        printf("%" PRIu64 "\n", recvBuff[2]);

        uint64_t num = baseToDec(recvBuff[0], (int)recvBuff[1]);
        printf("Dec: %" PRIu64 "\n", num);
        int factors = numFactor(num, (int)recvBuff[2]);
        printf("Factor: %d\n", factors);
        sendBuff[0] = factors;

        write(sockfd, sendBuff, sizeof(sendBuff));
    }

    close(sockfd);

    return 0;

}