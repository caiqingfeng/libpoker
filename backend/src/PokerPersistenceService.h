/*
 * PersistenceService.h
 *
 *  Created on: 2013-6-27
 *      Author: caiqingfeng
 */

#ifndef POKER_PERSISTENCESERVICE_H
#define POKER_PERSISTENCESERVICE_H

#include "persistence/src/PersistenceService.h"
#include <vector>
#include <string>
#include "libpoker/holdem/src/LYHoldemTable.h"
#include <map>
#include "libpoker/holdem/src/LYHoldemGame.h"
#include "libpoker/poker/src/LYPlayer.h"

/*
 * Wrapper for PokerPersistenceMysql, PokerPersistenceRedis
 */
class PokerPersistenceService: public PersistenceService {
public:
	PokerPersistenceService(const char db[]);
	virtual ~PokerPersistenceService();
	void set_poker_collection();
	std::string get_collection();

public:
	void list_table(std::vector<std::string>& tables);
	void list_house_table(std::vector<std::string>& tables);

	/*
	 * 用户进入桌子时，要先从mysql中取到上次的chips_at_hand
	 * 用户离开桌子时，从redis中删除该用户，并将chips_at_hand存到mysql中
	 */
	void enter_table(const std::string& table_id, const std::string& uid, unsigned int chips_at_hand=0);
	unsigned int leave_table(const std::string& table_id, const std::string& uid);
	void update_seat_status(LYTable* table, unsigned int seat_no);
	void fetch_player(const std::string& uid, LYPlayer& player);
	void update_balance(const std::string& uid, const std::string& game,
			int& delta);
	void loadAllProfiles(std::vector<LYPokerProfile *> &db_profiles);

public:
	std::string get_club_id_by_table(const std::string& table);

public:
	//20160701 added
	void change_occupied_seats(const std::string& table_id, bool increase);
	void change_occupied_seats(const std::string& table_id, unsigned int seats);
	unsigned int get_occupied_seats(const std::string& table_id);

	//discarded, replaced by set_table_insurance_income
	void increase_total_buyin(const std::string& table_id, unsigned int chips);
	unsigned int get_total_buyin(const std::string& table_id);
	void set_table_insurance_income(const std::string& table_id, int chips);

	void change_insurance_income(const std::string& table_id, int chips);
	int get_insurance_income(const std::string& table_id);

	void increase_user_buyin(const std::string& table_id, const std::string& uid, unsigned int chips);
	unsigned int get_user_buyin(const std::string& table_id, const std::string& uid);

	void save_user_chips(const std::string& table_id, const std::string& uid, int chips);
	int get_user_chips(const std::string& table_id, const std::string& uid);

	void increase_table_games(const std::string& table_id);
	unsigned int get_table_games(const std::string& table_id);

	void increase_club_games(const std::string& club);
	unsigned int get_club_games(const std::string& club);

	void increase_user_club_games(const std::string& cid, const std::string& uid);
	unsigned int get_user_club_games(const std::string& cid, const std::string& uid);

	void increase_user_club_inpots(const std::string& cid, const std::string& uid);
	unsigned int get_user_club_inpots(const std::string& cid, const std::string& uid);

	void increase_user_club_wins(const std::string& cid, const std::string& uid);
	unsigned int get_user_club_wins(const std::string& cid, const std::string& uid);

	void change_user_club_income(const std::string& cid, const std::string& uid, int income);
	int get_user_club_income(const std::string& cid, const std::string& uid);

	void increase_user_table_games(const std::string& table_id, const std::string& uid);
	unsigned int get_user_table_games(const std::string& table_id, const std::string& uid);

	void increase_user_table_wins(const std::string& table_id, const std::string& uid);
	unsigned int get_user_table_wins(const std::string& table_id, const std::string& uid);

	void increase_user_table_inpots(const std::string& table_id, const std::string& uid);
	unsigned int get_user_table_inpots(const std::string& table_id, const std::string& uid);

	void change_user_table_income(const std::string& table_id, const std::string& uid, int income);
	int get_user_table_income(const std::string& table_id, const std::string& uid);

public:
	//20160701 added
	void increase_user_chips_to_mb(const std::string& tid, const std::string& uid,
					unsigned int seat_no, int buyin); //更新用户的chips_at_hand, redis
	void increase_user_chips_buyin_to_db(const std::string& tid, const std::string& uid,
					int buyin); //更新用户的chips_at_hand, buyin, mysql
	void save_user_chips_to_mb(const std::string& tid, const std::string& uid,
					unsigned int seat_no, int buyin); //更新用户的chips_at_hand, redis
	int get_user_chips_from_db(const std::string& tid, const std::string& uid
					); //查询用户的chips_at_hand mysql
	int get_user_chips_from_mb(const std::string& tid, const std::string& uid
					); //查询用户的chips_at_hand, redis
	void drop_user_from_mb(const std::string& tid, const std::string& uid
					); //删除用户, redis

public:
	//20160705 added
	unsigned int get_occupied_seats_from_mb(const std::string& tid);
};

#endif /* SERVERHOLDEMMSGHANDLER_H_ */
