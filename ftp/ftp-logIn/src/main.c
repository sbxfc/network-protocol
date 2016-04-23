
#include <stdio.h>
#include <netinet/in.h>
#include <string.h>

#define MAXLINE 4096
#define USER_NAME "administrator\r\n"
#define USER_PWD "123456\r\n"
#define FTP_CODE_CONNECTED "220"
#define FTP_CODE_NAME_OK "331"
#define FTP_CODE_PWD_OK "230"

#define PASSIVE_MODE_SET_SUCC "227"

/**
 * 解析ftp协议返回的状态码
*/
void 
get_ftp_code(char* buf,char* code)
{
	memcpy(code,buf,3);
	*(code+3)='\0';
}

int 
main(int argc,char ** argv)
{
	
	if(argc < 3){
		printf("Invalid args!Interface {ip} {port}\n");
		exit(-1);
	}
	
	char* ip = argv[1];
	int port = atoi(argv[2]);
	char read_buf[MAXLINE];
	char write_buf[MAXLINE];
	char* ftp_code = malloc(sizeof(char)*4);//free
	ssize_t n;
	
	//创建ftp套接字
	int sockfd = socket(PF_INET, SOCK_STREAM,0);

	struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &address.sin_addr);
    address.sin_port = htons(port);

	//通过套接字连接ftp
	int rst;
    if((rst = connect(sockfd,(struct sockaddr *)&address,sizeof(address)))<0){
		 printf("connecting error!%i\n",rst);
		 exit(-1);
	}

	memset(read_buf,0,MAXLINE);
	n = read(sockfd, read_buf, MAXLINE);
	get_ftp_code(read_buf,ftp_code);
	if(strcmp(ftp_code,FTP_CODE_CONNECTED) != 0){
		printf("ftp-->%s\r",read_buf);
		exit(-1);
	}
	printf("ftp-->%s\r",read_buf);

	/*验证用户名*/
	printf("-->验证用户名\n");
	memset(write_buf,0,MAXLINE);
	strcpy(write_buf,"USER ");
	strcpy(write_buf+(strlen(write_buf)),USER_NAME);
	printf("ftp-->%s\r",write_buf);		
	write(sockfd, write_buf, strlen(write_buf));
	memset(read_buf,0,MAXLINE);
	n =	read(sockfd, read_buf, MAXLINE);
	get_ftp_code(read_buf,ftp_code);
	if(strcmp(ftp_code,FTP_CODE_NAME_OK) != 0){
		printf("ftp-->%s\r",read_buf);
		exit(-1);
	}
	printf("ftp-->%s\r",read_buf);
	
	/*验证密码*/
	printf("-->验证密码\n");
	memset(write_buf,0,MAXLINE);
	strcpy(write_buf,"PASS ");
	strcpy(write_buf+(strlen(write_buf)),USER_PWD);
	printf("ftp-->%s\r",write_buf);		
	write(sockfd, write_buf, strlen(write_buf));
	memset(read_buf,0,MAXLINE);
	n =	read(sockfd, read_buf, MAXLINE);
	get_ftp_code(read_buf,ftp_code);
	if(strcmp(ftp_code,FTP_CODE_PWD_OK) != 0){
		printf("ftp-->%s\r",read_buf);
		exit(-1);
	}
	printf("ftp-->%s\r",read_buf);

	/*设置FTP服务器为被动模式*/
	memset(write_buf,0,MAXLINE);
	strcpy(write_buf,"PASV\r\n");
	printf("ftp-->%s\r",write_buf);	
	write(sockfd, write_buf, strlen(write_buf));
	memset(read_buf,0,MAXLINE);
	n =	read(sockfd, read_buf, MAXLINE);
	get_ftp_code(read_buf,ftp_code);
	if(strcmp(ftp_code,PASSIVE_MODE_SET_SUCC) != 0){
		printf("ftp-->%s\r",read_buf);
		exit(-1);
	}
	printf("ftp-->%s\r",read_buf);

	return 0;
}
