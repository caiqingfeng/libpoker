/*
 * Fixture.h
 *
 *  Created on: 2014-4-17
 *      Author: caiqingfeng
 */

#ifndef FIXTURE_H_
#define FIXTURE_H_

#include "src/ServerPlayerMgr.h"
#include "src/ServerHall.h"
#include "MessageStubs.h"
#include "src/PokerPersistenceService.h"
#include "src/PokerPersistenceServiceRedis.h"
#include "src/PokerPersistenceServiceMysql.h"

#include <iostream>

class ServerHall_instance : public ServerHall
{
public:
	ServerHall_instance(ServerPlayerMgr *pm) : ServerHall(pm) {};
	virtual ~ServerHall_instance() {};

public:
	//调用handler的send message
	virtual void sendTimerMsg(LYTimerPtr t, LYTimerAction cmd) {} ;
	virtual LYPokerProfile *getProfileById(const std::string& profile){};
	virtual void sendShakeHandMsg(const string &table_id, const string &uid) {} ;
	virtual void sendKickoutPlayerMsg(const string &table_id, LYApplicant seat_no, const string &uid) {};
	virtual void sendReplyEnterTableMsg(LYTable* table, const string &uid, int chips) {} ;
	virtual void sendReplyLeaveTableMsg(LYTable* table, const string &uid) {} ;
	virtual void sendReplyTakeSeatMsg(const string &table_id, LYApplicant seat_no, unsigned int buyin,
				const string &uid, const std::vector<std::string>& players) {} ;
	virtual void sendReplyBuyinMsg(const string &table_id, LYApplicant seat_no, unsigned int new_buyin,
            	int chips_at_hand, const string &uid, const std::vector<std::string>& players)  {} ;
	virtual void sendReplyLeaveSeatMsg(const string &table_id, LYApplicant seat_no, int buyin,
				const string &uid, const std::vector<std::string>& players) {} ;
public:
	//调用profile manager
	virtual bool isValidBuyin(const std::string& profile, unsigned int chipsAtHand, unsigned int buyin) ; //买入是否在允许范围内

public:
    virtual void checkToStartGame(LYTable *table) {}; //新局开始，清理原来的定时器，并注册定时器
    virtual void checkToEndGame(LYTable *table) {} ; //清理原来的定时器，并注册新局定时器，
    virtual bool isOpen(const std::string& table_id); //判断是否关闭桌子
    virtual void closeTable(LYTable *table) { table->status = LYTableClose;} ;
    virtual void nextRound(LYTable *table){};
    virtual void forceLeaveSeat(LYTable* table, LYSeat* seat) {} ;
    virtual void foldBeforeLeave(LYTable* table, LYSeat* seat) {} ;

public:
	//调用persistence_service
	virtual LYTablePtr load_new_table(const string &table_id) ;
	virtual void changeOccupiedSeats(const string &table_id, unsigned int seats){};
	virtual int getUserChips(const string &table_id, const string &uid){ return 0;};
	virtual void saveGame(LYTable* table) {};

};

class ServerPlayerMgr_instance : public ServerPlayerMgr
{
public:
	virtual void load_user_from_db(const std::string& uid, const std::string &device_id, LYPlayer& player);
	virtual void update_balance_to_db(const std::string& uid, const std::string& game, int& delta) {}; //只保存用户Balance
	virtual void update_position_to_db(LYPlayer& user, const std::string&, int& chips, bool) {}; //只保存用户position

public: //added 20160701
	virtual void increase_user_chips_to_mb(const std::string& tid, const std::string& uid,
					unsigned int seat_no, int buyin) {}; //更新用户的chips_at_hand, redis
	virtual void increase_user_chips_buyin_to_db(const std::string& tid, const std::string& uid,
					int buyin)  {}; //更新用户的chips_at_hand, buyin, mysql
	virtual void save_user_chips_to_mb(const std::string& tid, const std::string& uid,
					unsigned int seat_no, int buyin)  {}; //更新用户的chips_at_hand, redis
	virtual int get_user_chips_from_db(const std::string& tid, const std::string& uid
					)  {}; //查询用户的chips_at_hand mysql
	virtual int get_user_chips_from_mb(const std::string& tid, const std::string& uid
					)  {}; //查询用户的chips_at_hand, redis
	virtual void drop_user_from_mb(const std::string& tid, const std::string& uid
					)  {}; //删除用户, redis
};

class Fixture
{
public:
	Fixture();
	~Fixture();

public:
	ServerPlayerMgr_instance* pm;
	ServerHall_instance* hall;
	MockMsgHandler *mockHandler;

    PokerPersistenceService* aps;
    PokerPersistenceServiceRedis* apsr;
    PokerPersistenceServiceMysql* apsm;

	void create_tables();
	void drop_tables();
};

#endif


