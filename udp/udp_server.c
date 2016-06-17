#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXLINE 4096



void dg_echo(int sockfd,struct sockaddr* pcliaddr,socklen_t clilen){
	int n;
	socklen_t len;
	char mesg[MAXLINE];
	for(;;){
		len = clilen;
        printf("wait!\n");
		n = recvfrom(sockfd,mesg,MAXLINE,0,pcliaddr,&len);
        printf("recv mesg!\n");
		sendto(sockfd,mesg,n,0,pcliaddr,len);
	}
}



int main(int argc,char **argv)
{
	int sockfd;
	struct sockaddr_in servaddr,cliaddr;
    
	sockfd = socket(AF_INET,SOCK_DGRAM,0);
    
	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(0x7f000001);
	servaddr.sin_port = htons(9877);

	bind(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr));

	printf("bind!\n");

	dg_echo(sockfd,(struct sockaddr*)&cliaddr,sizeof(cliaddr));
}
