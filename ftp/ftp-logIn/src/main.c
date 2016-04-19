
#include <stdio.h>
#include <netinet/in.h>
#include <string.h>

#define MAXLINE 4096
#define USER_NAME "admin\r\n"
#define USER_PWD "123456\r\n"

int main(int argc,char ** argv){
	
	if(argc < 3){
		printf("Invalid args!Interface {ip} {port}\n");
		exit(-1);
	}
	
	ssize_t n;
	char* ip = argv[1];
	int port = atoi(argv[2]);
	char read_buf[MAXLINE];
	char write_buf[MAXLINE];

	int sockfd = socket(PF_INET, SOCK_STREAM,0);

	struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &address.sin_addr);
    address.sin_port = htons(port);

	int rst;
    if((rst = connect(sockfd,(struct sockaddr *)&address,sizeof(address)))<0){
		 printf("connecting error!%i\n",rst);
		 exit(-1);
	}

	memset(read_buf,0,MAXLINE);
	n = read(sockfd, read_buf, MAXLINE);
	printf("ftp-->%s\r",read_buf);
	
	/*发送用户名*/
	memset(write_buf,0,MAXLINE);
	strcpy(write_buf,"USER ");
	strcpy(write_buf+(strlen(write_buf)),USER_NAME);
	printf("ftp-->%s\r",write_buf);	
	write(sockfd, write_buf, strlen(write_buf));
	/* 客户端接收服务器的响应码和信息，正常为 ”331 XXXXXXXXXXXXXXXXXXX” */
	memset(read_buf,0,MAXLINE);
	n =	read(sockfd, read_buf, MAXLINE);
	printf("ftp-->%s\r",read_buf);

	/*发送密码*/
	memset(write_buf,0,MAXLINE);
	strcpy(write_buf,"PASS ");
	strcpy(write_buf+(strlen(write_buf)),USER_PWD);
	printf("ftp-->%s\r",write_buf);	
	write(sockfd, write_buf, strlen(write_buf));
	memset(read_buf,0,MAXLINE);
	n =	read(sockfd, read_buf, MAXLINE);
	printf("ftp-->%s\r",read_buf);

	return 0;
	
}
