#include <stdio.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>

int port = 9999;

int main(int argc, char **argv)
{
    int socketfd;
    struct sockaddr_in sin;
    struct group_req req;
    int loop = 1, reuse = 1, iter = 0, sin_len;
    char message[256];

    socketfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (-1 == socketfd){
        perror("Opening socket");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0){
        perror("setsockopt: SO_REUSEADDR");
        exit(EXIT_FAILURE);
    }

    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    //接收目的ip为224.0.0.1，目的port为8888的报文
    // sin.sin_addr.s_addr = inet_addr("224.0.0.1");
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_port = htons(port);
    if(bind(socketfd, (struct sockaddr *)&sin, sizeof(sin)) < 0){
        perror("bind");
        exit(EXIT_FAILURE);
    }

    loop = 1;
    if (setsockopt(socketfd, IPPROTO_IP, IP_MULTICAST_LOOP, &loop, sizeof(loop)) < 0){
        perror("setsockopt: IP_MULTICAST_LOOP");
        exit(EXIT_FAILURE);
    }

    req.gr_interface = 0;
    sin.sin_addr.s_addr = inet_addr("224.0.0.1");
    memcpy(&req.gr_group, &sin, sizeof(sin));
    if(setsockopt(socketfd, IPPROTO_IP, MCAST_JOIN_GROUP, &req, sizeof(req)) < 0){
        perror("MCAST_JOIN_GROUP\n");
        exit(EXIT_FAILURE);
    }

    while(1){
        sin_len = sizeof(sin);
        //if (recvfrom(socketfd, message, 256, 0, (struct sockaddr *)&sin, &sin_len) == -1)
        if (recvfrom(socketfd, message, 256, 0, NULL, NULL) == -1){
            perror("recvfrom");
        }

        printf("Response #%-2d from server: %s\n", ++iter, message);
    }

    close(socketfd);
    exit(EXIT_SUCCESS);
}

