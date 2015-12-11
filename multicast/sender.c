#include <stdio.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>

int port = 6789;

int main(int argc, char **argv)
{
    int socketfd;
    struct sockaddr_in address;
    struct sockaddr_in addr;
    unsigned char ttl;
    unsigned char loop;

    socketfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (-1 == socketfd){
        perror("Opening socket");
        exit(EXIT_FAILURE);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("192.168.2.153");
    //addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    //addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(8000);
    if(bind(socketfd, (struct sockaddr *)&addr, sizeof(addr))<0){
        perror("bind");
        close(socketfd);
        exit(-1); 
    }
    // 设置多播的TTL值
    ttl = 2;
    if(setsockopt(socketfd,IPPROTO_IP,IP_MULTICAST_TTL,&ttl,sizeof(ttl))<0){
       perror("IP_MULTICAST_TTL");
       return -1;
    }
    
    loop = 1;
    if (setsockopt(socketfd, IPPROTO_IP, IP_MULTICAST_LOOP, &loop, sizeof(loop)) < 0){
        perror("setsockopt: IP_MULTICAST_LOOP");
        exit(EXIT_FAILURE);
    }

    socklen_t size;
    printf("before get\n");
    if (getsockopt(socketfd, IPPROTO_IP, IP_MULTICAST_LOOP, &loop, &size) < 0){
        perror("getsockopt: IP_MULTICAST_LOOP");
        exit(EXIT_FAILURE);                            
    } else {
        printf("loop got: %d\n", loop);
    }

    //starting Mcast transmit...
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("224.0.0.1");
    address.sin_port = htons(port);

    char msg[] = "test from a multicast sender";
    while(1){
        if (sendto(socketfd, msg, sizeof(msg), 0,(struct sockaddr *)&address, sizeof(address)) < 0){
            perror("sendto");
            exit(EXIT_FAILURE);
        }
        sleep(1);
    }
    exit(EXIT_SUCCESS);
}
