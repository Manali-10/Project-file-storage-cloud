#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <strings.h>
#include <string.h>
#include <pthread.h>

struct data{
	int type;
};

int main(){
	char *ser_ip = "127.0.0.1";
	int sockfd;
	struct sockaddr_in servaddr,cliaddr;
	socklen_t servlen ;
	ssize_t sen,rec;
	struct data arr;

        sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd < 0){
                perror("server\n");
                exit(1);
        }

	bzero(&servaddr,sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(2000);
        inet_pton(AF_INET, ser_ip, &servaddr.sin_addr);
	servlen = sizeof(servaddr);
	while(1){
	printf("1.registation\n2.login\n");
	scanf("%d",&arr.type);
	sen = sendto(sockfd,&arr,sizeof(struct data), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
	printf("send\n");
	}
	return 0;	
}
