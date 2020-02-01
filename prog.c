#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <strings.h>
#include <string.h>
#include <pthread.h>

int main(){
	int retval,sockfd;
	struct sockaddr_in servaddr,cliaddr;
	socklen_t clilen;
	size_t rec;
	char arr[100];

	sockfd=socket(AF_INET,SOCK_DGRAM,0);
	if(sockfd<0)
		perror("socket failed\n");
	printf("sockfd=%d\n",sockfd);
	
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	servaddr.sin_port=htons(2000);
	
	retval=bind(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
	if(retval<0)
		perror("bind failed\n");

	clilen=sizeof(cliaddr);

	rec=recvfrom(sockfd,arr,100,0,(struct sockaddr *)&cliaddr,&clilen);
	if(rec<0)
		perror("recvfrom fail\n");

	printf("%s\n",arr);

	

}
