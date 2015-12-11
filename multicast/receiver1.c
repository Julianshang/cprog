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
    struct sockaddr_in sin;
    struct sockaddr_in server_addr;
    struct ip_mreq command;
    int loop = 1, iter = 0, sin_len;
    char message[256];

    socketfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (-1 == socketfd){
        perror("Opening socket");
        exit(EXIT_FAILURE);
    }

    loop = 1;
    if (setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &loop, sizeof(loop)) < 0){
        perror("setsockopt: SO_REUSEADDR");
        exit(EXIT_FAILURE);
    }

    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    //接收目的ip为224.0.0.1，目的port为6789的报文
    sin.sin_addr.s_addr = inet_addr("224.0.0.1");
    sin.sin_port = htons(port);
    if(bind(socketfd, (struct sockaddr *)&sin, sizeof(sin)) < 0){
        perror("bind");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    //接收源ip为127.0.0.1,源port为8000的报文
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    //server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(8000);
    if(connect(socketfd, (struct sockaddr *)&server_addr, sizeof(server_addr))<0){
        perror("connect error");
        close(socketfd);
        exit(-1);
    }

    loop = 1;
    if (setsockopt(socketfd, IPPROTO_IP, IP_MULTICAST_LOOP, &loop, sizeof(loop)) < 0){
        perror("setsockopt: IP_MULTICAST_LOOP");
        exit(EXIT_FAILURE);
    }

    command.imr_multiaddr.s_addr = inet_addr("224.0.0.1");
    command.imr_interface.s_addr = htonl(INADDR_ANY);
    if (command.imr_multiaddr.s_addr == -1){
        perror("224.0.0.1 not a legal multicast address.");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(socketfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &command, sizeof(command)) < 0){
        perror("setsockopt: IP ADD_MEMBERSHIP");
    }

    while(1){
        sin_len = sizeof(sin);
        //if (recvfrom(socketfd, message, 256, 0, (struct sockaddr *)&sin, &sin_len) == -1)
        if (recvfrom(socketfd, message, 256, 0, NULL, NULL) == -1){
            perror("recvfrom");
        }

        printf("Response #%-2d from server: %s\n", ++iter, message);
        sleep(1);
    }

    if (setsockopt(socketfd, IPPROTO_IP, IP_DROP_MEMBERSHIP, &command, sizeof(command)) < 0){
        perror("setsockopt: IP_DROP_MEMBERSHIP");
    }

    close(socketfd);
    exit(EXIT_SUCCESS);
}
