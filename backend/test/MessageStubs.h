/*
 * MessageStubs.h
 *
 *  Created on: 2013-7-10
 *      Author: caiqingfeng
 */

#ifndef MESSAGESTUBS_H_
#define MESSAGESTUBS_H_

#include "common/src/zhelpers.hpp"
//#include "backend/src/AppdHandlerImpl.h"
#include "backend/src/PokerHandlerImpl.h"

//avoid sending data via zmq
//for the sake of unit testing
//class MockMsgHandler : public AppdHandlerImpl
//20150207修改，继承自ＫingHandlerImpl
class MockMsgHandler : public PokerHandlerImpl
{
public:
//	MockMsgHandler(zmq::socket_t &peer) : AppdHandlerImpl(peer) {};
	MockMsgHandler(zmq::socket_t &peer) : PokerHandlerImpl(peer, peer) {};
	virtual void zmq_sendmore_wrapper(zmq::socket_t &z, const string &msg); //为了增加可测性，在测试函数中重载此方法即可实现不连ZMQ也能单元测试的目的
	virtual void zmq_send_wrapper(zmq::socket_t &z, const string &msg); //为了增加可测性，在测试函数中重载此方法即可实现不连ZMQ也能单元测试的目的

public:
/*进入，坐下，起身，离开
 *设置OtaPayload，后续也许考虑将OtaPayload拆分成两个消息,这种与游戏类型无关的动作，可以放在这个类里实现
 * marked by cqf, 20151225
 */
    virtual void mcastEnterTable(const std::string& uid, int chips,
    		const std::string& table_id, std::vector<std::string> players){};
	virtual void mcastQuitTable(const std::string& uid,
			const std::string& table_id, std::vector<std::string> players){};
	virtual void mcastTakeSeat(const std::string& uid, const std::string& table_id, unsigned int seat_no,
			unsigned int chips_at_hand, std::vector<std::string> players){};
	virtual void mcastBuyin(const std::string& uid, const std::string& table_id, unsigned int seat_no,
			unsigned int buyin, int chips_at_hand, std::vector<std::string> players){};
	virtual void mcastLeaveSeat(const std::string& uid, const std::string& table_id, unsigned int seat_no,
			std::vector<std::string> players){};

public:
//与定时器有关的部分
	virtual void onTimeout(LYTimer& t){}; //调用hall->onTimeout()
	virtual void kickout_player(const std::string& table_id,
			unsigned int seat_no, const std::string& uid){}; //踢出玩家，主要是将长期未连线的旁观玩家踢出
	virtual void shake_hand(const std::string& table_id, const std::string& uid){}; //与玩家握手
};

#endif


