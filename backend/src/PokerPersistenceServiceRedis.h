/*
 * PersistenceService redis implements.h
 *
 *  Created on: 2014-9-27
 *      Author: caiqingfeng
 */

#ifndef POKER_PERSISTENCESERVICEREDIS_H
#define POKER_PERSISTENCESERVICEREDIS_H

#include <hiredis/hiredis.h>
#include "libmrock/persistence/src/PersistenceServiceRedis.h"
#include "poker/src/LYTable.h"

/*
 * 所有活动游戏的当前状态存在内存数据库中
 */
class PokerPersistenceServiceRedis: public PersistenceServiceRedis {
public:
	//for the sake of unit testing
	std::string poker_collection;

public:
	PokerPersistenceServiceRedis(const char collec[]);
	virtual ~PokerPersistenceServiceRedis();
	void set_poker_collection();

public:
	void list_table(std::vector<std::string>& tables);
	void enter_table(const std::string& table_id, const std::string& uid, int chips_at_hand=0);
	int leave_table(const std::string& table_id, const std::string& uid);
	void update_seat_status(LYTable* table, unsigned int seat_no);

public:
//	int get_chips_at_hand(const std::string& table_id, unsigned int seat_no);
//	void update_seat_chips(LYTable* table, unsigned int seat_no, int chips);
	void increase_user_chips_to_mb(const std::string& tid, const std::string& uid,
					unsigned int seat_no, int buyin); //更新用户的chips_at_hand, redis
	void save_user_chips_to_mb(const std::string& tid, const std::string& uid,
					unsigned int seat_no, int buyin); //更新用户的chips_at_hand, redis
	int get_user_chips_from_mb(const std::string& tid, const std::string& uid
					); //查询用户的chips_at_hand, redis
	void drop_user_from_mb(const std::string& tid, const std::string& uid
					); //删除用户, redis

public:
	unsigned int get_occupied_seats_from_mb(const std::string& tid);
	void change_occupied_seats(const std::string& table_id, unsigned int seats);
};

#endif /* PERSISTENCESERVICEREDIS_H */
