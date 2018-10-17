/*
 * ClientHandler.h
 *
 *  Created on: 2013-8-26
 *      Author: chenfeng
 */

#ifndef CLIENTHANDLER_H_
#define CLIENTHANDLER_H_

#include "Callback.h"
#include "ClientHandlerImpl.h"

using namespace std;
using namespace com::luyun::msg;

namespace com {
namespace luyun {
namespace msg {
namespace client {

/*
 * 在客户端如Android的工程：jni/MsgAdapter中实例化ClientHandler
 * ZmqThread中会处理服务器发过来的消息，将其交给impl来处理
 * ZmqService也会实例化本类，但主要是发送消息给ZmqThread，由Dispatcher在Dispatch中转发给服务器
 */
class ClientHandler: public com::luyun::msg::MsgHandler {
private:
	ClientHandlerImpl *handlerImpl;

public:
    ClientHandler(zmq::socket_t& socket, com::luyun::msg::client::ClientHandlerImpl *handler = NULL,
    		const std::string& uid = string(), const std::string& key = string());
    virtual ~ClientHandler();

	virtual void setUidAndKey(std::string& uid, std::string& key, bool forward = false);
    void setCallback(Callback* cb);
    ClientHandlerImpl* getImpl() {return handlerImpl;};

    void onPlainText(string& msg);
    void onPayload(string& msg);
    virtual void onMsg(string& msg) {} ;
};

} /* namespace client */
} /* namespace msg */
} /* namespace luyun */
} /* namespace com */
#endif /* CLIENTHANDLER_H_ */
