//
//  GameSocket.cpp
//  IOSocketClient
//
//  Created by sbx_fc on 15-5-12.
//
//

#include "GameSocket.h"
#include "ByteArray.h"

#define SBX_MAX_SIZE 1024

int GameSocket::Init(const char* ip, unsigned short port){
    
    CCLOG("初始化socket!");
    
    socket.Init();
    socket.Create(AF_INET, SOCK_STREAM, 0);
    
    bool result = socket.Connect(ip, port);
    if (!result) {
        CCLOG("服务器连接失败!");
        return -1;
    }
    
    CCLOG("连接成功!");

    //开启新线程，在子线程中，接收数据
    std::thread recvThread = std::thread(&GameSocket::recieveData, this);
    recvThread.detach();
    
    return 1;
}

/**
 * 发送数据
 */
void GameSocket::Send(const char* buf, int len)
{
    CCLOG("发送数据->长度:%d",len);

    socket.Send(buf,len);
}

/**
 * 接到数据
 */
void GameSocket::recieveData()
{
    while (true)
    {
        char buf[SBX_MAX_SIZE];
        int result = socket.Recv(buf, SBX_MAX_SIZE,0);
        switch (result) {
            case SOCKET_ERROR:
            {
                CCLOG("接收数据出错!");
            }
                break;
            case 0:
            {
                CCLOG("服务器断开连接!");
            }
                break;
            default:
                break;
        }
        
//        ByteArray bytes(1);
//        bytes.writeChars(buf,SBX_MAX_SIZE);
//        
//        int val = bytes.readByte();
//        cout << "接到byte:" << val << endl;
//        string str = bytes.readString();
//        cout << "接到字符串:" << str << endl;
//        int valInt = bytes.readInt();
//        cout << "接到整数:" << valInt << endl;
        
    }
    // 关闭连接
    socket.Close();
}


