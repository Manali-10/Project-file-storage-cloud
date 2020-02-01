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

struct data{
	int type;
	char name[20];
	int cmd;
	char buf[100];
	char filename[20];
	int size;
	int ack;
};

int main(){
	char *ser_ip = "127.0.0.1";
	int sockfd;
	struct sockaddr_in servaddr,cliaddr;
	socklen_t servlen,clilen ;
	ssize_t sen,rec;
	struct data arr;
	clilen=sizeof(cliaddr);

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
	if(arr.type==1){
		char buf[20];
		printf("enter name:");
		scanf("%s",arr.name);
		sen = sendto(sockfd,&arr,sizeof(struct data), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
		rec=recvfrom(sockfd,buf,20,0,(struct sockaddr *)&servaddr,&servlen);
		printf("%s\n",buf);
	
	}
	else{
		char buf[51];
		printf("enter name:");
		scanf("%s",arr.name);
		sen = sendto(sockfd,&arr,sizeof(struct data), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
		while(1){
			rec=recvfrom(sockfd,buf,51,0,(struct sockaddr *)&servaddr,&servlen);
			printf("%s\n",buf);
			scanf("%d",&arr.cmd);
			if(arr.cmd==1){
				sen = sendto(sockfd,&arr,sizeof(struct data), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
				rec=recvfrom(sockfd,&arr,sizeof(struct data),0,(struct sockaddr *)&servaddr,&servlen);
				printf("%s\n",arr.buf);
				}
			else if(arr.cmd==2){
				int fd;
				char *s;
				printf("Enter filename:");
				scanf("%s",arr.filename);
				sen = sendto(sockfd,&arr,sizeof(struct data), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
				rec=recvfrom(sockfd,&arr,sizeof(struct data),0,(struct sockaddr *)&servaddr,&servlen);
				s=(char *)malloc(arr.size);
				arr.ack=1;
				sen = sendto(sockfd,&arr,sizeof(struct data), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
				rec=recvfrom(sockfd,s,arr.size,0,(struct sockaddr *)&servaddr,&servlen);
				fd=open(arr.filename,O_CREAT | O_EXCL | O_RDWR,0777);
				write(fd,s,arr.size);
				printf("get command sucessfully done\n");			
			
				}
			else if(arr.cmd==3){
				int fd;
				char *s;
				struct stat buf1;
				printf("Enter filename:");
				scanf("%s",arr.filename);
				int info=stat(arr.filename,&buf1);
				arr.size=buf1.st_size;
				s=(char *)malloc(arr.size);
				sen = sendto(sockfd,&arr,sizeof(struct data), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
				rec=recvfrom(sockfd,&arr,sizeof(struct data),0,(struct sockaddr *)&servaddr,&servlen);
				if(arr.ack==1){
					fd=open(arr.filename,O_RDONLY);
					read(fd,s,arr.size);
					sen = sendto(sockfd,s,arr.size, 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
					}
				printf("put command sucessfully done\n");			
			
				}
			else if(arr.cmd==4){
				sen = sendto(sockfd,&arr,sizeof(struct data), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
				printf("logout %s\n",arr.name);
				break;
				}
			}
		
		}
	printf("\n--------------------------------------------------------------\n");
	}
	return 0;	
}
