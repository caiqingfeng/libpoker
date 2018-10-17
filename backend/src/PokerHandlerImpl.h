/*
 * ServerHandler.h
 *
 *  Created on: 2014-4-10
 *      Author: caiqingfeng
 */

#ifndef POKER_HANDLERIMPL_H_
#define POKER_HANDLERIMPL_H_

#include "backend/src/ServerHandlerImpl.h"
#include "libpoker/poker/src/LYTable.h"

using namespace std;
using namespace com::luyun::msg;

/*
 * 所有的PokerHandler都需要连接 timer server(Concreted)，并对定时任务进行处理
 */
class PokerHandlerImpl: public com::luyun::handler::server::ServerHandlerImpl {
protected:
	zmq::socket_t& zmq_timer;

public:
	PokerHandlerImpl(zmq::socket_t& socket, zmq::socket_t& timer_socket);
	virtual ~PokerHandlerImpl();
	virtual void pushNotificationToMobile(const std::string& uid, const std::string& msg) {}; //do nothing for push
	virtual void onPayload(string& payload, std::string& uid) {};

public:
/*进入，坐下，起身，离开
 *设置OtaPayload，后续也许考虑将OtaPayload拆分成两个消息,这种与游戏类型无关的动作，可以放在这个类里实现
 * marked by cqf, 20151225
 */
    virtual void mcastEnterTable(const std::string& uid, int chips,
    		const std::string& table_id, std::vector<std::string> players) = 0;
	virtual void mcastQuitTable(const std::string& uid,
			const std::string& table_id, std::vector<std::string> players) = 0;
	virtual void mcastTakeSeat(const std::string& uid, const std::string& table_id, unsigned int seat_no,
			unsigned int chips_at_hand, std::vector<std::string> players) = 0;
	virtual void mcastBuyin(const std::string& uid, const std::string& table_id, unsigned int seat_no,
			unsigned int buyin, int chips_at_hand, std::vector<std::string> players) = 0;
	virtual void mcastLeaveSeat(const std::string& uid, const std::string& table_id, unsigned int seat_no,
			std::vector<std::string> players) = 0;

public:
//与定时器有关的部分
	virtual void onTimeout(LYTimer& t) = 0; //调用hall->onTimeout()
	virtual void kickout_player(const std::string& table_id,
			unsigned int seat_no, const std::string& uid) = 0; //踢出玩家，主要是将长期未连线的旁观玩家踢出
	virtual void shake_hand(const std::string& table_id, const std::string& uid) = 0; //与玩家握手

	virtual void sendTimerMsg(LYTimerPtr t, enum LYTimerAction cmd);
	virtual void onShakeHandTimer();
};

#endif /* SERVERHANDLER_H_ */
