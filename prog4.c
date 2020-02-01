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
       #include <unistd.h>


int retval,sockfd;
struct sockaddr_in servaddr,cliaddr;
socklen_t clilen;
size_t rec;

static int entries=0;
int i,sen;

struct data{
	int type;
	char name[20];
	int cmd;
	char buf[100];
	char filename[20];
	int size;
	int ack;
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
	ptr->next=(struct list *)malloc(sizeof(struct list));
	ptr=ptr->next;
	crt=mkdir(arr.name,0777);
	sen = sendto(sockfd,"registation done",18, 0, (struct sockaddr *)&cliaddr, sizeof(cliaddr));
	printf("registation done\n");
	
}

/*void list(){
	ptr2=start;
	for(i=0;i<entries;i++){
		printf("ptr2=%p\tptr2->folder=%s\tptr->next=%p\n",ptr2,ptr2->folder,ptr2->next);
		ptr2=ptr2->next;

	}
}*/

void open_new(){
	int fd;
	
	struct stat buf1;
	
	chdir(arr.name);	
	printf("%s\n",arr.name);
	while(1){
	sen = sendto(sockfd,"Enter the cmd\n1.dir\n2.get file\n3.put file\n4.logout",51, 0, (struct sockaddr *)&cliaddr, sizeof(cliaddr));
	rec=recvfrom(sockfd,&arr,sizeof(struct data),0,(struct sockaddr *)&cliaddr,&clilen);
	if(arr.cmd==1){
		system("dir>txt.txt");
		fd=open("./txt.txt",O_RDONLY);
		read(fd,&arr.buf,100);
		sen = sendto(sockfd,&arr,sizeof(struct data), 0, (struct sockaddr *)&cliaddr, sizeof(cliaddr));
	}
	else if(arr.cmd==2){
		
		char *s;
		
		//rec=recvfrom(sockfd,&arr,sizeof(struct data),0,(struct sockaddr *)&cliaddr,&clilen);
		int info=stat(arr.filename,&buf1);
		arr.size=buf1.st_size;
		s=(char *)malloc(arr.size);
		sen = sendto(sockfd,&arr,sizeof(struct data), 0, (struct sockaddr *)&cliaddr, sizeof(cliaddr));
		
		rec=recvfrom(sockfd,&arr,sizeof(struct data),0,(struct sockaddr *)&cliaddr,&clilen);
		
		if(arr.ack==1){
			fd=open(arr.filename,O_RDONLY);
			read(fd,s,arr.size);
			sen = sendto(sockfd,s,arr.size, 0, (struct sockaddr *)&cliaddr, sizeof(cliaddr));			
			
		}
		printf("get command sucessfully done\n");
	}
	else if(arr.cmd==3){
		
		char *s;
		//rec=recvfrom(sockfd,&arr,sizeof(struct data),0,(struct sockaddr *)&cliaddr,&clilen);
		
		s=(char *)malloc(arr.size);
		arr.ack=1;
		sen = sendto(sockfd,&arr,sizeof(struct data), 0, (struct sockaddr *)&cliaddr, sizeof(cliaddr));
		
		rec=recvfrom(sockfd,s,arr.size,0,(struct sockaddr *)&cliaddr,&clilen);
		
		fd=open(arr.filename,O_CREAT | O_EXCL | O_RDWR,0777);
		write(fd,s,arr.size);
		printf("put command sucessfully done\n");			
	}
	else if(arr.cmd==4){
		printf("logout %s\n",arr.name);
		break;
	}
	}
	chdir("..");
}

void login(){
	//list();
	ptr1=start;
	int ret;
	for(i=0;i<entries;i++){
		ret=strcmp(arr.name,ptr1->folder);
		if(ret==0){
			
			open_new();
			break;
		}
		ptr1=ptr1->next;
	}
	
}

int main(){
	

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
			printf("registation start\n");
			registation();
			}	
		else{
			printf("login ");
			login();
			}
		printf("\n---------------------------------------------------------------------------\n");
	
	}
	return 0;

}
