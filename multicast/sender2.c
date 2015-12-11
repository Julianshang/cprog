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
    struct ip_mreq command;

    socketfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (-1 == socketfd){
        perror("Opening socket");
        exit(EXIT_FAILURE);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    //addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(8000);
    if(bind(socketfd, (struct sockaddr *)&addr, sizeof(addr))<0){
        perror("bind");
        close(socketfd);
        exit(-1); 
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
