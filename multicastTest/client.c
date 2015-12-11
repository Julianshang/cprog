#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<errno.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<unistd.h>
#include<assert.h>

#define PORT 9999

int main(int argc,char* argv[])//localInterface and multicastAddr
{
	int sockfd;
	struct sockaddr_in localAddr,serverAddr;
	socklen_t addrLen = sizeof(localAddr);
	struct ip_mreq mAddr;
	int err;
	
	char buff[100];
	int num=0;
	int times=0;
    
    if(argc != 3){
		printf("./client localAddr multicastAddr\n");
		return 0;
	}

	memset(&localAddr,0,sizeof(localAddr));
	localAddr.sin_family = AF_INET;
	localAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	localAddr.sin_port = htons(PORT);
	
	err = inet_pton(AF_INET, argv[2], &mAddr.imr_multiaddr);
	if(err == -1)
		perror("inet_pton error");
	inet_pton(AF_INET,argv[1],&mAddr.imr_interface);

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	assert(sockfd > 0);
	bind(sockfd, (struct sockaddr*)&localAddr, addrLen);
	
	err = setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mAddr, sizeof(mAddr));
	assert(err == 0);
	
	while(1){
		num = recvfrom(sockfd, buff, 100, 0, (struct sockaddr*)&serverAddr, &addrLen);
		assert(num >= 0);
		buff[num]='\0';
		printf("recv %dst message :%s", times++, buff);
	}
}
