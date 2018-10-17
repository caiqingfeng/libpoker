//
//  ClientDispatcher.h
//
//  Created by chenfeng on 13-7-9.
//  Copyright (c) 2013年 chenfeng. All rights reserved.
//

#ifndef CLIENTDISPATCHER_H_
#define CLIENTDISPATCHER_H_

#include "ClientHandler.h"

using namespace com::luyun::msg::client;

namespace com {
namespace luyun {
namespace msg {
namespace client {

/*
 * 跟Server一样，设计成Singleton
 * 客户端client如Android的jni/MsgAdapter中，实例化ClientDispatcher
 * _msgHandler实际为一全局变量，该变量定义在setup_env中，由MsgAdapter实例化
 */
class ClientDispatcher {
private:
    static ClientDispatcher* _instance;

private:
    zmq::socket_t& _socket_svr;
    zmq::socket_t& _socket_view;
    ClientHandler *_msgHandler;
    
private:
    ClientDispatcher(zmq::socket_t& socket_svr, zmq::socket_t& socket_view, ClientHandler *msgHandler);
//    ClientDispatcher();

public:
    static ClientDispatcher* Instance(zmq::socket_t& socket_svr, zmq::socket_t& socket_view, ClientHandler *msgHandler);

public:
    virtual ~ClientDispatcher();
    void dispatch();
};

} /* namespace client */
} /* namespace msg */
} /* namespace luyun */
} /* namespace com */

#endif /* defined(__test__MsgDispatcher__) */
