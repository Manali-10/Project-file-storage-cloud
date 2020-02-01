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
#include <dirent.h>


static int entries=0;
int i;

struct data{
	int type;
	char name[20];
};
struct data arr;

struct list{
	char folder[20];
	struct list *next;
};
struct list *start,*ptr,*ptr1,*ptr2;

void registation(){
	entries++;

	int crt;

	for(i=0;i<20;i++){
		ptr->folder[i]=arr.name[i];
	}
	printf("ptr=%p\t folder=%s\n",ptr,ptr->folder);
	ptr->next=(struct list *)malloc(sizeof(struct list));
	ptr=ptr->next;
	printf("ptr->next=%p\n",ptr->next);
	crt=mkdir(arr.name,0777);

	printf("registattion process done\n");
	
}

void list(){
	ptr2=start;
	for(i=0;i<entries;i++){
		printf("ptr2=%p\tptr2->folder=%s\tptr->next=%p\n",ptr2,ptr2->folder,ptr2->next);
		ptr2=ptr2->next;

	}
}

void login(){
	list();
	ptr1=start;
	int ret;
	for(i=0;i<entries;i++){
		ret=strcmp(arr.name,ptr1->folder);
		if(ret==0){
			
			printf("open function called\n");
			break;
		}
		ptr1=ptr1->next;
	}
	
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
		else{
			printf("login process\n");
			login();
		}
	
	}
	return 0;

}
