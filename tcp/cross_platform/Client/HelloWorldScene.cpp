#include "HelloWorldScene.h"


//////////////////////////////
// 引入头文件

#include "ByteArray.h"
#include "Net/Socket/GameSocket.h"

//
//////////////////////////////

USING_NS_CC;


Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    
//////////////////////////////
// 测试Socket连接
    
    ByteArray bytes(1);
    bytes.writeByte(24);
    bytes.writeString("写入中文");
    bytes.writeInt(2048);

    const char* ip = "192.168.2.118";
    int port = 8082;
    GameSocket ::GetInstance() -> Init(ip,port);
    GameSocket ::GetInstance() -> Send(bytes.data(),bytes.size());
    
//
//////////////////////////////

    
    return true;
}



