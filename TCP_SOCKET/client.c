#include <netinet/in.h>
#include <stdio.h>

#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

int main() {

    int sockfd;

    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(9876);
    servaddr.sin_addr.s_addr = htonl(0x7f000001); // 127.0.0.1

    //创建一个TCP套接字,TCP是一个字节流协议,仅支持SOCK_STREAM套接字
    sockfd = socket(AF_INET,SOCK_STREAM,0);
    if (-1 == sockfd){
        perror("创建套接字失败!");
        exit(EXIT_FAILURE);
    }

    //若成功返回为0,若出错则为 -1
    if(connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1){
        printf("套接字连接失败!");
        exit(EXIT_FAILURE);
    }

    printf("套接字连接成功!");

    //向服务端写入数据
    int length = 10; // the size of message
    char* msg = (char*)malloc((length+1)*sizeof(char)); // Look out the end mark '/0' of a C string
    if (msg == NULL)
        exit(1);
    int i;
    for (i=0; i<length; i++)
        strcat(msg, "a");

    printf("%s\n", msg);
    write(sockfd, msg,length);

    return 0;
}
