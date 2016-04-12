//
//  IOSocket.cpp
//  IOSocketClient
//
//  Created by sbx_fc on 15-5-12.
//
//

#include "IOSocket.h"

#ifdef WIN32
#include "StdAfx.h"
#pragma comment(lib, "wsock32")
#endif

int IOSocket::Init()
{
#ifdef WIN32
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;
    wVersionRequested = MAKEWORD(2, 0);
    err = WSAStartup(wVersionRequested, &wsaData);
    if(0 != err) //检查Socket初始化是否成功
    {
        CCLOG("Socket2.0初始化失败，Exit!");
        return -1;
    }
#endif
    
    return 0;
}

/**
 * 创建Socket
 */
bool IOSocket::Create(int family, int type, int protocol)
{
    sockfd = socket(family, type, protocol);
    if (INVALID_SOCKET == sockfd) {
        CCLOG("Socket 创建失败，Exit!");
        return false;
    }
    return true;
}

/**
 * 建立Socket连接
 * @param ip ip地址
 * @param port 端口
 */
bool IOSocket::Connect(const char* ip, unsigned short port)
{
    /**
     * 设置发往的地址
     */
    struct sockaddr_in addrto;
    addrto.sin_family = AF_INET;//地址类型为internetwork
    addrto.sin_addr.s_addr = inet_addr(ip);
    addrto.sin_port = htons(port);
    
    int ret = connect(sockfd, (struct sockaddr*)&addrto, sizeof(addrto));
    if ( ret == SOCKET_ERROR ) {
        return false;
    }
    return true;
}

 /**
 * 发送数据
 */
int IOSocket::Send(const char* buf, int len, int flags)
{
    int bytes;
    int count = 0;
    
    while ( count < len ) {
        
        bytes = send(sockfd, buf + count, len - count, flags);
        if ( bytes == -1 || bytes == 0 )
        {
            CCLOG("数据发送失败!");
            return -1;
        }
        count += bytes;
    } 
    
    return count;
}

/**
 * 接收服务器数据
 */
int IOSocket::Recv(char* buf, int len, int flags)
{
    return (recv(sockfd, buf, len, flags));
}

int IOSocket::Close()
{
#ifdef WIN32
    return (closesocket(sockfd));
#else
    return (close(sockfd));
#endif
}

#pragma mark 定义IOSocket

IOSocket::IOSocket(SOCKET sock)
{
    sockfd = sock;
    
}

IOSocket::~IOSocket()
{
    
}

IOSocket& IOSocket::operator = (SOCKET& rhs)
{
    sockfd = rhs;
    return (*this);
}

IOSocket::operator SOCKET ()
{
    return sockfd;
}
