/*
 * ServerHandler.h
 *
 *  Created on: 2014-4-10
 *      Author: caiqingfeng
 */

#ifndef CLIENTHANDLER_IMPL_H_
#define CLIENTHANDLER_IMPL_H_

#include "common/src/MsgHandler.h"
#include "Callback.h"

using namespace std;
using namespace com::luyun::msg;

namespace com {
namespace luyun {
namespace msg {
namespace client {

class ClientHandlerImpl: public com::luyun::msg::MsgHandler {
protected:
    Callback *_callback; /*这个也在MsgAdapter中实例化，并显式的从外部赋值过来*/

public:
	ClientHandlerImpl(zmq::socket_t& socket);
	virtual ~ClientHandlerImpl();

	void setCallback(Callback* cb);
	virtual void onPayload(string& payload) = 0;
};

} /* namespace server */
} /* namespace msg */
} /* namespace luyun */
} /* namespace com */

#endif /* SERVERHANDLER_IMPL_H_ */
