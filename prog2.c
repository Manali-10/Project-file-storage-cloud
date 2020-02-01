#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <strings.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

struct data{
	int type;
	char name[20];
};
struct data arr;

struct list{
	char folder[20];
	struct list *next;
};
struct list *start,*ptr;

void registation(){
	int crt;
	printf("arr.name = %s\n",arr.name);
	for(int i=0;i<20;i++){
		ptr->folder[i]=arr.name[i];
	}
	printf("list name =%s\n",ptr->folder);
	ptr=(struct list *)malloc(sizeof(struct list));
	ptr->next=ptr;
	crt=mkdir(arr.name,0777);

	if(!crt)
		printf("sucessfully\n");

	printf("registattion process done\n");
	
}

int main(){
	int retval,sockfd;
	struct sockaddr_in servaddr,cliaddr;
	socklen_t clilen;
	size_t rec;

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

	start=(struct list *)malloc(sizeof(struct list));
	ptr=start;
	while(1){
		rec=recvfrom(sockfd,&arr,sizeof(struct data),0,(struct sockaddr *)&cliaddr,&clilen);
		if(rec<0)
			perror("recvfrom fail\n");
	
		if(arr.type==1){
			printf("registation process\n");
			registation();
		}	
		else
			printf("login process\n");
	
	}
	return 0;

}
