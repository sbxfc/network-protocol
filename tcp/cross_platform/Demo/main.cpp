//
//  main.cpp
//  TCPSocket
//
//  Created by sbx_fc on 15-1-29.
//  Copyright (c) 2015年 RG. All rights reserved.
//

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SERV_ADD "192.168.1.118"
#define SERV_PORT 8082

int main(void)
{
    int sockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (-1 == sockfd)
    {
        perror("创建套接字失败!");
        exit(EXIT_FAILURE);
    }
    
    int res;
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(struct sockaddr_in));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    res = inet_pton(AF_INET, SERV_ADD, &servaddr.sin_addr);
    
    if (-1 == res)
    {
        perror("error: 使用了不被支持的地址族!\n");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    else if (0 == res)
    {
        perror("error:不是有效的IP地址!\n");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    
    if (-1 == connect(sockfd, (const struct sockaddr *)&servaddr, sizeof(struct sockaddr_in)))
    {
        perror("套接字连接失败!\n");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    
    /* perform read write operations ... */

    shutdown(sockfd, SHUT_RDWR);
    
    close(sockfd);
    return 0;
}



