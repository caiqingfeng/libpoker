/*
 * ServerPlayerMgr.h
 *
 *  Created on: 2013-7-2
 *      Author: caiqingfeng
 */

#ifndef SERVERPLAYERMGR_H_
#define SERVERPLAYERMGR_H_

#include "poker/src/LYPlayerMgr.h"

class ServerPlayerMgr : public LYPlayerMgr{
private:
	int onlineUserCount;

protected:

public:
//	virtual void load_user_from_db(const std::string& uid, const std::string &device_id, LYPlayerPtr player) = 0;
	virtual void load_user_from_db(const std::string& uid, const std::string &device_id, LYPlayer& player) = 0;
	virtual void update_balance_to_db(const std::string& uid, const std::string& game, int& delta) = 0; //只保存用户Balance
	virtual void update_position_to_db(LYPlayer& user, const std::string& tid, int& chips_at_hand, bool enter_table) = 0; //保存用户position

public: //added 20160701
	virtual void increase_user_chips_to_mb(const std::string& tid, const std::string& uid,
					unsigned int seat_no, int buyin) = 0; //更新用户的chips_at_hand, redis
	virtual void increase_user_chips_buyin_to_db(const std::string& tid, const std::string& uid,
					int buyin) = 0; //更新用户的chips_at_hand, buyin, mysql
	virtual void save_user_chips_to_mb(const std::string& tid, const std::string& uid,
					unsigned int seat_no, int buyin) = 0; //更新用户的chips_at_hand, redis
	virtual int get_user_chips_from_db(const std::string& tid, const std::string& uid
					) = 0; //查询用户的chips_at_hand mysql
	virtual int get_user_chips_from_mb(const std::string& tid, const std::string& uid
					) = 0; //查询用户的chips_at_hand, redis
	virtual void drop_user_from_mb(const std::string& tid, const std::string& uid
					) = 0; //删除用户, redis

public:
	ServerPlayerMgr();
	virtual ~ServerPlayerMgr();

//	LYPlayerPtr getPlayerById(const std::string &uid);
	LYPlayer getPlayerById(const std::string &uid);
	void updateBalance(const std::string& uid, const std::string& game, int& delta);
//	void updatePosition(LYPlayerPtr player);
	void updatePosition(LYPlayer& player, const std::string& tid, int& chips_at_hand, bool enter_table);

public:
//	void enterTable(LYPlayerPtr player, const string& tid);
//	void leaveTable(LYPlayerPtr player, const string& tid);
	void enterTable(LYPlayer& player, const string& tid, int& chips_at_hand, bool reentry=false);
	void leaveTable(LYPlayer& player, const string& tid);

	//added 20160701
	void buyin(const std::string& table_id, const std::string& uid, unsigned int seat_no, unsigned int chips, bool reentry);
//	void leaveSeat(LYTable *table, const std::string& uid, unsigned int seat_no, int chips);

};

#endif /* SERVERPLAYERMGR_H_ */
