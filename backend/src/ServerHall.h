/*
 * ServerHandler.h
 *
 *  Created on: 2015-12-23
 *      Author: caiqingfeng
 */

#ifndef SERVER_HALL_H_
#define SERVER_HALL_H_

#include "poker/src/LYHall.h"
#include "LYPokerTimer.h"
#include "ServerPlayerMgr.h"
#include "holdem/src/LYHoldemConstants.h"
#include "poker/src/LYPokerProfile.h"

const std::string TIMER_ID_REQ_NEW_GAME = "new_game_request";
const std::string TIMER_ID_CLOSE_TABLE = "close_table";
const std::string TIMER_ID_NEXT_ROUND = "next_round";
const std::string TIMER_ID_POLL_SEAT1 = "polling1";
const std::string TIMER_ID_POLL_SEAT2 = "polling2";
const std::string TIMER_ID_POLL_SEAT3 = "polling3";
const std::string TIMER_ID_POLL_SEAT4 = "polling4";
const std::string TIMER_ID_POLL_SEAT5 = "polling5";
const std::string TIMER_ID_POLL_SEAT6 = "polling6";
const std::string TIMER_ID_POLL_SEAT7 = "polling7";
const std::string TIMER_ID_POLL_SEAT8 = "polling8";
const std::string TIMER_ID_POLL_SEAT9 = "polling9";
const std::string TIMER_ID_POLLING[] = {
		TIMER_ID_POLL_SEAT1, TIMER_ID_POLL_SEAT2, TIMER_ID_POLL_SEAT3, TIMER_ID_POLL_SEAT4,
		TIMER_ID_POLL_SEAT5, TIMER_ID_POLL_SEAT6, TIMER_ID_POLL_SEAT7, TIMER_ID_POLL_SEAT8,
		TIMER_ID_POLL_SEAT9};

//added 20150215, c++11 特性，支持vector声明时赋值
const std::vector<std::string> TIMER_ID_POLLING_vector = {TIMER_ID_POLL_SEAT1, TIMER_ID_POLL_SEAT2, TIMER_ID_POLL_SEAT3, TIMER_ID_POLL_SEAT4,
		TIMER_ID_POLL_SEAT5, TIMER_ID_POLL_SEAT6, TIMER_ID_POLL_SEAT7, TIMER_ID_POLL_SEAT8,
		TIMER_ID_POLL_SEAT9};

const std::string TIMER_ID_RESERVE_SEAT1 = "reserve_seat1";
const std::string TIMER_ID_RESERVE_SEAT2 = "reserve_seat2";
const std::string TIMER_ID_RESERVE_SEAT3 = "reserve_seat3";
const std::string TIMER_ID_RESERVE_SEAT4 = "reserve_seat4";
const std::string TIMER_ID_RESERVE_SEAT5 = "reserve_seat5";
const std::string TIMER_ID_RESERVE_SEAT6 = "reserve_seat6";
const std::string TIMER_ID_RESERVE_SEAT7 = "reserve_seat7";
const std::string TIMER_ID_RESERVE_SEAT8 = "reserve_seat8";
const std::string TIMER_ID_RESERVE_SEAT9 = "reserve_seat9";

const std::string TIMER_ID_RESERVE_SEAT[] = {
		TIMER_ID_RESERVE_SEAT1, TIMER_ID_RESERVE_SEAT2, TIMER_ID_RESERVE_SEAT3, TIMER_ID_RESERVE_SEAT4, TIMER_ID_RESERVE_SEAT5,
		TIMER_ID_RESERVE_SEAT6, TIMER_ID_RESERVE_SEAT7, TIMER_ID_RESERVE_SEAT8, TIMER_ID_RESERVE_SEAT9};

//added 20150215, c++11 特性，支持vector声明时赋值
const std::vector<std::string> TIMER_ID_RESERVE_SEAT_vector = {
		TIMER_ID_RESERVE_SEAT1, TIMER_ID_RESERVE_SEAT2, TIMER_ID_RESERVE_SEAT3, TIMER_ID_RESERVE_SEAT4, TIMER_ID_RESERVE_SEAT5,
        TIMER_ID_RESERVE_SEAT6, TIMER_ID_RESERVE_SEAT7, TIMER_ID_RESERVE_SEAT8, TIMER_ID_RESERVE_SEAT9};

//added 20160321, Assurance
const std::string TIMER_ID_ASSURE_SEAT1 = "assure1";
const std::string TIMER_ID_ASSURE_SEAT2 = "assure2";
const std::string TIMER_ID_ASSURE_SEAT3 = "assure3";
const std::string TIMER_ID_ASSURE_SEAT4 = "assure4";
const std::string TIMER_ID_ASSURE_SEAT5 = "assure5";
const std::string TIMER_ID_ASSURE_SEAT6 = "assure6";
const std::string TIMER_ID_ASSURE_SEAT7 = "assure7";
const std::string TIMER_ID_ASSURE_SEAT8 = "assure8";
const std::string TIMER_ID_ASSURE_SEAT9 = "assure9";
const std::string TIMER_ID_ASSURE_SEAT[] = {
		TIMER_ID_ASSURE_SEAT1, TIMER_ID_ASSURE_SEAT2, TIMER_ID_ASSURE_SEAT3, TIMER_ID_ASSURE_SEAT4, TIMER_ID_ASSURE_SEAT5,
		TIMER_ID_ASSURE_SEAT6, TIMER_ID_ASSURE_SEAT7, TIMER_ID_ASSURE_SEAT8, TIMER_ID_ASSURE_SEAT9};
const std::vector<std::string> TIMER_ID_ASSURE_SEAT_vector = {
		TIMER_ID_ASSURE_SEAT1, TIMER_ID_ASSURE_SEAT2, TIMER_ID_ASSURE_SEAT3, TIMER_ID_ASSURE_SEAT4, TIMER_ID_ASSURE_SEAT5,
		TIMER_ID_ASSURE_SEAT6, TIMER_ID_ASSURE_SEAT7, TIMER_ID_ASSURE_SEAT8, TIMER_ID_ASSURE_SEAT9};

class ServerHall: public LYHall {
protected:
	std::map<std::string, LYPokerTimerPtr> timers;

public:
	ServerHall(ServerPlayerMgr *pm);
	virtual ~ServerHall();

public:
	//调用handler的send message
	virtual void sendTimerMsg(LYTimerPtr t, LYTimerAction cmd) = 0;
	virtual void sendShakeHandMsg(const string &table_id, const string &uid) = 0;
	virtual void sendKickoutPlayerMsg(const string &table_id, LYApplicant seat_no, const string &uid) = 0;
	virtual void sendReplyEnterTableMsg(LYTable* table, const string &uid, int chips) = 0;
	virtual void sendReplyLeaveTableMsg(LYTable* table, const string &uid) = 0;
	virtual void sendReplyTakeSeatMsg(const string &table_id, LYApplicant seat_no, unsigned int buyin,
				const string &uid, const std::vector<std::string>& players) = 0;
	virtual void sendReplyBuyinMsg(const string &table_id, LYApplicant seat_no, unsigned int new_buyin,
				int chips_at_hand, const string &uid, const std::vector<std::string>& players) = 0;
	virtual void sendReplyLeaveSeatMsg(const string &table_id, LYApplicant seat_no, int buyin,
				const string &uid, const std::vector<std::string>& players) = 0;
public:
	//调用profile manager
	virtual LYPokerProfile *getProfileById(const std::string& profile) = 0;
	virtual bool isValidBuyin(const std::string& profile, unsigned int chipsAtHand, unsigned int buyin) = 0; //买入是否在允许范围内
	//是否可以向Bank继续购买筹码，作为一个风险控制措施
	virtual bool canBuyinFromBank(LYTablePtr table, LYPlayer& player, unsigned int buyin);
	//discarded 20160701
	virtual void updateBalance(LYTablePtr table, LYPlayer& player, unsigned int seat_no, int buyin);

public:
    virtual void checkToStartGame(LYTable *table) = 0; //新局开始，清理原来的定时器，并注册定时器
    virtual void checkToEndGame(LYTable *table) = 0; //清理原来的定时器，并注册新局定时器，
    virtual bool isOpen(const std::string& table_id) = 0; //判断是否关闭桌子
    virtual void closeTable(LYTable *table) = 0;
    virtual void nextRound(LYTable *table) = 0;
    virtual void forceLeaveSeat(LYTable* table, LYSeat* seat) = 0;
    virtual void foldBeforeLeave(LYTable* table, LYSeat* seat) = 0;

public:
	//调用persistence_service
	virtual LYTablePtr load_new_table(const string &table_id) = 0;
	virtual void changeOccupiedSeats(const string &table_id, unsigned int seats) = 0;
	virtual int getUserChips(const string &table_id, const string &uid) = 0;
	virtual void saveGame(LYTable* table) = 0;

public:
    //20150205新增，以下方法为跟定时有关的接口，在KingPineappleHall中重载实现，在本类中为空
    virtual void registerTimer(const string &table_id, const string &timer_id, int time_passed, int reserved1=0);
    virtual void clearTimer(const string &table_id, const string &timer_id);
    virtual void clearTimers(const string &table_id); //清理所有的定时器
	virtual void onTimeout(LYTimer &timer);
//
	virtual LYPokerTimerPtr getTimer(const std::string &table_id);
	virtual void shakeHand(const string &table_id, const string &uid); //仅供内部调用
	virtual void onShakeHand(const string &table_id, const string &uid);
	virtual void kickoutPlayer(LYTable* table, LYSeat* seat);

	//added 20160320，为了确保ServerHall与业务无关
	virtual void seatOnPollingTimeout(const std::string& table_id, LYApplicant seat);
    virtual void seatOnAssuranceTimeout(const std::string& table_id, LYApplicant seat);
////
public:
    virtual void enterTable(const std::string& t, const std::string& uid);
//    virtual void leaveTable(LYTable* table, const std::string& uid);
    virtual void leaveTable(const std::string& t, const std::string& uid);
    virtual void takeSeat(const std::string& t, enum LYApplicant seat_no,
                  			const std::string& uid, unsigned int buyin);
    virtual void leaveSeat(const std::string& t, enum LYApplicant seat_no,
                  			const std::string& uid, int cashout=0);
	virtual void applyBuyin(const std::string &table_id, enum LYApplicant seat_no,
							const std::string &uid, unsigned int buyin);
	virtual void monitorPlayer(LYTable* table, unsigned int seat_no,
				LYHoldemRoundContinue roundCont=LYHoldemRoundCont); //added 20160321 for assurance
	virtual bool allowUser(LYTablePtr table, const std::string& uid);
};

#endif /* SERVERHANDLER_H_ */
