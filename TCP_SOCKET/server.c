#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <errno.h>

#define EINTR 4
#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0
#define MAXPENDING 5
#define MAXSIZE 1024




void doSomeThing(int connfd){
    ssize_t n;
    char buf[4096];
again:
    while ((n = read(connfd, buf, 4096)) > 0) {
        printf("Received message: %s\n",buf);
        write(connfd, buf, n);
    }

    if( n < 0 && errno == EINTR){
        goto again;
    }
    else if (n < 0){
        perror("str_echo:read error");
    }

}

int main() {

    int listenfd;

    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(9876);
    servaddr.sin_addr.s_addr = htonl(0x7f000001); // 127.0.0.1

    //创建一个TCP套接字,TCP是一个字节流协议,仅支持SOCK_STREAM套接字
    listenfd = socket(AF_INET,SOCK_STREAM,0);
    if (-1 == listenfd){
        perror("创建套接字失败!");
        exit(EXIT_FAILURE);
    }

    bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    /* Mark the socket so it will listen for incoming connections */
    if (listen(listenfd, MAXPENDING) < 0){
        perror("listen() failed!");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in cliaddr;
    socklen_t clilen;
    int connfd;
    int fid;
    for (;;) /* Run forever */
    {
        clilen = sizeof(struct sockaddr_in);
        connfd = accept(listenfd,(struct sockaddr*) &cliaddr,&clilen);

        if((fid = fork()) == 0 ) {
            //在fork出的子线程执行
            close(listenfd);
            doSomeThing(connfd);
            exit(EXIT_SUCCESS);
        }
        close(connfd);
    }

    return 0;
}
