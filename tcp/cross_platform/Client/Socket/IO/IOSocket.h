//
//  IOSocket.h
//  IOSocketClient
//
//  Created by sbx_fc on 15-5-12.
//
//

#ifndef __IOSocketClient__IOSocket__
#define __IOSocketClient__IOSocket__

#ifdef WIN32
    #include <winsock.h>
    typedef int				socklen_t;
#else
    #include "cocos2d.h"
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <netdb.h>
    #include <fcntl.h>
    #include <unistd.h>
    #include <sys/stat.h>
    #include <sys/types.h>
    #include <arpa/inet.h>
    typedef int				SOCKET;

    //#pragma region define win32 const variable in linux
    #define INVALID_SOCKET	-1
    #define SOCKET_ERROR	-1
    //#pragma endregion
#endif

class IOSocket
{
    
public:
    
    /**
     * 创建Socket对象
     * @param family 协议族
     * @param type 套接字类型
     * @param protocol 通信协议类型
     */
    bool Create(int family = AF_INET, int type = SOCK_STREAM, int protocol = 0);
    
    /**
     * 连接服务器
     * @param ip ip地址
     * @param port 端口
     */
    bool Connect(const char* ip, unsigned short port);
    
    /**
     * 发送数据
     */
    int Send(const char* buf, int len, int flags = 0);
    
    /**
     * 接收数据
     */
    int Recv(char* buf, int len, int flags = 0);
    
    /**
     * 关闭socket
     */
    int Close();
    
    // Init winsock DLL
    static int Init();
public:
    /**
     * 默认构造函数
     */
    IOSocket(SOCKET sock = INVALID_SOCKET);
    
    /**
     * 析构函数
     */
    ~IOSocket();
    
    /**
     * 赋值操作
     */
    IOSocket& operator = (SOCKET& rhs);
    
    operator SOCKET ();

    
    
protected:
    SOCKET sockfd;
};


#endif /* defined(__IOSocketClient__IOSocket__) */
