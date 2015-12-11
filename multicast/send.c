/*************************************************************************
    > File Name: main.c
    > Author: Julian
    > Email:  1042029354@qq.com 
    > Created Time: Mon 30 Nov 2015 11:17:12 AM CST
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>	       /* See NOTES */
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include <sys/utsname.h>
#include<arpa/inet.h>
#include<netinet/in.h>

#define MAXLINE 255
#define SERV_PORT 8000

int
udp_client(const char *host, const char *serv, struct sockaddr **saptr, socklen_t *lenp)
{
	int				sockfd, n;
	struct addrinfo	hints, *res, *ressave;

	bzero(&hints, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;

	if ((n = getaddrinfo(host, serv, &hints, &res)) != 0){
		perror("udp_client error: getaddrinfo");
        exit(EXIT_FAILURE);
    } 
	ressave = res;

	do {
		sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		if (sockfd >= 0)
			break;		/* success */
	} while ( (res = res->ai_next) != NULL);

	if (res == NULL){	/* errno set from final socket() */
		perror("udp_client error: socket");
        exit(EXIT_FAILURE);
    } 

	*saptr = malloc(res->ai_addrlen);
    if(*saptr == NULL)
        return -1;
	memcpy(*saptr, res->ai_addr, res->ai_addrlen);
	*lenp = res->ai_addrlen;

	freeaddrinfo(ressave);

	return(sockfd);
}
/* end udp_client */

int
Udp_client(const char *host, const char *serv, struct sockaddr **saptr, socklen_t *lenptr)
{
	return(udp_client(host, serv, saptr, lenptr));
}

int main(int argc, char *argv[])
{
    int sendfd;
    socklen_t mcast_len;
    struct sockaddr_in *mcast_addr, addr_serv;
    struct group_req req;
    char line[MAXLINE]; 
    struct utsname myname;
    unsigned int loop;

    if(argc != 4){
        printf("usage: send <IP-multicast-address <port#> local_address");
        exit(EXIT_FAILURE);
    }

    sendfd = socket(AF_INET, SOCK_DGRAM, 0);
    sendfd = Udp_client(argv[1], argv[2], (struct sockaddr **)&mcast_addr, &mcast_len);
    if(sendfd < 0){
        perror("socket\n");
        exit(EXIT_FAILURE);
    }
   
     /*服务器端地址*/
    memset(&addr_serv,0,sizeof(struct sockaddr_in));
    addr_serv.sin_family = AF_INET;
    //addr_serv.sin_addr.s_addr = inet_addr("192.168.2.150");
    addr_serv.sin_addr.s_addr = inet_addr(argv[3]);
    addr_serv.sin_port = htons(SERV_PORT);
    
    if(bind(sendfd, (struct sockaddr *)&addr_serv, sizeof(addr_serv)) < 0){
        perror("bind");
        close(sendfd);
        exit(-1);                             
    }

    loop = 1;
    if (setsockopt(sendfd, IPPROTO_IP, IP_MULTICAST_LOOP, &loop, sizeof(loop)) < 0){
        perror("setsockopt: IP_MULTICAST_LOOP");
        exit(EXIT_FAILURE);                            
    }

    socklen_t size = sizeof(loop);
    printf("before get\n");
    if (getsockopt(sendfd, IPPROTO_IP, IP_MULTICAST_LOOP, &loop, &size) < 0){
        perror("getsockopt: IP_MULTICAST_LOOP");
        exit(EXIT_FAILURE);                            
    } else {
        printf("loop got: %d\n", loop);
    }

    req.gr_interface = 0;
    memcpy(&req.gr_group, mcast_addr, sizeof(*mcast_addr));
    if(setsockopt(sendfd, IPPROTO_IP, MCAST_JOIN_GROUP, &req, sizeof(req)) < 0){
        perror("setsockopt\n");
        exit(EXIT_FAILURE);
    }

    if(uname(&myname) < 0){
        perror("uname\n");
        exit(EXIT_FAILURE);
    }

    snprintf(line, sizeof(line), "%s, %d\n", myname.nodename, getpid());

    for(;;){
        if(sendto(sendfd, line, strlen(line), 0, (struct sockaddr *)mcast_addr, sizeof(*mcast_addr)) < 0){
            perror("sendto error\n");
            exit(EXIT_FAILURE);
        }
        sleep(1);
    }
}
