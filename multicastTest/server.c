#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<assert.h>

#define PORT 9999
#define INTEVAL 1

int main(int argc,char* argv[])
{
	int sockfd;
	struct sockaddr_in multicastAddr;
	struct in_addr outerInterface;
	socklen_t addrLen=sizeof(multicastAddr);
	int sendNum;
	int err=0;

	int times=0;
	char buff[100];
	
    if(argc != 3){
		printf("./server localAddr multicastAddr\n");
		return 0;
	}

	sockfd=socket(AF_INET,SOCK_DGRAM,0);
	assert(sockfd > 0);
	
	memset(&multicastAddr, 0, sizeof(multicastAddr));
	multicastAddr.sin_family=AF_INET;
	multicastAddr.sin_port = htons(PORT);
	err = inet_pton(AF_INET,argv[2],&multicastAddr.sin_addr);
	assert(err == 1);	

	err = inet_pton(AF_INET,argv[1],&outerInterface);	
	assert(err == 1);
	err = setsockopt(sockfd, IPPROTO_IP, IP_MULTICAST_IF, &outerInterface, sizeof(outerInterface));
	assert(err == 0);
	
	while(1){
		snprintf(buff, 100, "server send %d message: hello\n", times);
		sendNum = sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr*)&multicastAddr, addrLen);
		assert(sendNum > 0);
		printf("send %dst message success\n", times++);
		sleep(INTEVAL);
	}
}
