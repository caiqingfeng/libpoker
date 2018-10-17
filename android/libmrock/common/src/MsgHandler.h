//
//  MsgHandler.h
//  ibuddyholdem
//
//  Created by chenfeng on 13-7-6.
//  Copyright (c) 2013年 chenfeng. All rights reserved.
//

#ifndef MSGHANDLER_H_
#define MSGHANDLER_H_

#include "common/src/my_log.h"
#include "common/src/zhelpers.hpp"
//#include "libmrock/common/src/msg.pb.h"
#include "common/src/msg.pb.h"

using namespace std;

namespace com {
namespace luyun {
namespace msg {

class MsgHandler {
public:
    MsgHandler(zmq::socket_t& socket, const string& uid = string(), const string& key = string());
    virtual ~MsgHandler();
	void shakeHands();
    virtual void onMsg(string& msg) = 0;
    void quit();
    void reconnect(std::string& connect_to);
    //forward表明是转发还是真正的设置。
	virtual void setUidAndKey(std::string& uid, std::string& key, bool forward = false);
    virtual void sendMsg(OtaMsg& msg, const std::string server_handler="default_service");
    virtual void sendMsg(string& msg, bool hasMore = false);
//    virtual void sendCryptoMsg(OtaMsg& msg);
    virtual void sendCryptoMsg(string& msg);
    void buildMsg(OtaMsg& msg, const std::string server_handler="");
	virtual void zmq_sendmore_wrapper(zmq::socket_t &z, const string &msg); //为了增加可测性，在测试函数中重载此方法即可实现不连ZMQ也能单元测试的目的
	virtual void zmq_send_wrapper(zmq::socket_t &z, const string &msg); //为了增加可测性，在测试函数中重载此方法即可实现不连ZMQ也能单元测试的目的

protected:
    zmq::socket_t& _socket;
    string _uid;
    string _key; /* only used in client*/
};

} /* namespace msg */
} /* namespace luyun */
} /* namespace com */

#endif /* MSGHANDLER_H_ */
