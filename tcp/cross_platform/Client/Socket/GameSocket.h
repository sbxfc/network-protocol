//
//  GameSocket.h
//  IOSocketClient
//
//  Created by sbx_fc on 15-5-12.
//
//

#ifndef __IOSocketClient__GameSocket__
#define __IOSocketClient__GameSocket__

#include <iostream>
#include "IOSocket.h"
#include "ByteArray.h"

using namespace std;

class GameSocket
{
public:
    static GameSocket *GetInstance()
    {
        static GameSocket m_Instance;
        return &m_Instance;
    }
    
    /**
     * 初始化IOSocket
     */
    int Init(const char* ip, unsigned short port);
    
    void Send(const char* buf, int len);

private:
    
     /**
     * 接收数据
     */
    void recieveData();
    
    GameSocket(){ };
    IOSocket socket;
};


#endif /* defined(__IOSocketClient__GameSocket__) */
