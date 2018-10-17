/*
 * MessageStubs.h
 *
 *  Created on: 2013-7-10
 *      Author: caiqingfeng
 */

#include <string>
#include "MessageStubs.h"

void MockMsgHandler::zmq_sendmore_wrapper(zmq::socket_t &z, const string &msg)
{
	LY_LOG_DBG("output:" << msg);
//	std::cout << msg;
}

void MockMsgHandler::zmq_send_wrapper(zmq::socket_t &z, const string &msg)
{
	LY_LOG_DBG("output:" << msg);
//	std::cout << msg;
}
