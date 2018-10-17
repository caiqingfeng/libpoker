/*
 * AppdHandlerImpl.cpp
 *
 *  Created on: 2014-4-10
 *      Author: caiqingfeng
 */

#include "libmrock/backend/src/ServerHandler.h"
#include "PokerHandlerImpl.h"
#include <boost/foreach.hpp>

using namespace com::luyun::msg;
using namespace com::luyun::timer::shared;

PokerHandlerImpl::PokerHandlerImpl(zmq::socket_t& socket, zmq::socket_t& timer_socket) :
		        ServerHandlerImpl(socket), zmq_timer(timer_socket)
{
}

PokerHandlerImpl::~PokerHandlerImpl()
{
    // TODO Auto-generated destructor stub
}

void PokerHandlerImpl::sendTimerMsg(LYTimerPtr t, enum LYTimerAction cmd)
{
	com::luyun::timer::shared::LYTimerMsg tmsg;
	tmsg.set_version(1);
	LY_LOG_DBG("server timer=" << t->get_server_timer_id() << "cmd=" << cmd);
	if (t->get_server_timer_id() != "" &&
			(cmd == LY_TIMER_CAN || cmd == LY_TIMER_RESET)) {
		tmsg.set_server_timer_id(t->get_server_timer_id());
	}
	tmsg.set_client_timer_id(t->get_client_timer_id());
	tmsg.set_second_client_timer_id(t->get_second_client_timer_id());
	tmsg.set_after(t->get_after());
	tmsg.set_repeat(0.0);
	tmsg.set_client_reserved1(t->get_client_reserved1());
	tmsg.set_client_reserved2(t->get_client_reserved2());
	bool send_reg = false;
	do {
		send_reg = false;
		tmsg.set_action(cmd);
		if (LY_TIMER_RESET == cmd) { //先发一次Cancel，然后发一次Register
			tmsg.set_action(LY_TIMER_CAN);
			cmd = LY_TIMER_REG;
			send_reg = true;
		}
		LY_LOG_DBG("message to timerd: \n" << tmsg.DebugString());
		string msg;
		tmsg.SerializeToString(&msg);
		this->zmq_send_wrapper(this->zmq_timer, msg); //s_send(this->peer_zmq, dlmsg_str);
	} while (cmd == LY_TIMER_RESET || send_reg == true);
}

/*
 * 跟定时服务器握手
 */
void PokerHandlerImpl::onShakeHandTimer()
{
	std::string msg = "shakehand";
	this->zmq_send_wrapper(this->zmq_timer, msg);
}
