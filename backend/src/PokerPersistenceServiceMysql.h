/*
 * MysqlPersistence.h
 *
 *  Created on: 2013-9-11
 *      Author: chenfeng
 */

#ifndef POKER_PERSISTENCEMYSQL_H_
#define POKER_PERSISTENCEMYSQL_H_

#include "persistence/src/PersistenceService.h"
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <vector>

#include "persistence/src/PersistenceServiceMysql.h"
#include "libpoker/poker/src/LYPlayer.h"
#include "libpoker/poker/src/LYTable.h"
#include "libpoker/poker/src/LYPokerProfile.h"

int API_TimeToString(string &strDateStr,const int &timeData);
int API_StringToTime(const std::string &strDateStr, int &timeData);

//#include "common/src/PineapplePlayer.h"
class PokerPersistenceServiceMysql: public PersistenceServiceMysql {
public:
	//resources name
	std::string poker_tables; //
	std::string poker_profiles;
	std::string poker_users;
public:
	PokerPersistenceServiceMysql(const std::string& db_name, const std::string table_name="holdem_tables",
								const std::string profile_name="holdem_profiles",
								const std::string poker_user_name="poker_users");

public:
	virtual ~PokerPersistenceServiceMysql();
	virtual void clean_db();

	void load_table(LYTable* tbl);
	void fetch_player(const std::string& uid, LYPlayer& player);
	void update_balance(const std::string& uid, const std::string& game,
			int& delta);

	void loadAllProfiles(std::vector<LYPokerProfile*> &db_profiles);

	/* enter_table, leave_table, get_tables
	 * 目前还未确定是否用得上，可能会只放到redis中即可，不需要存Mysql
	 */
	void enter_table(const std::string& table_id, const std::string& uid);
	unsigned int leave_table(const std::string& table_id, const std::string& uid);
	void get_tables_by_uid(const std::string& id,
		std::vector<std::string>& results);

public:
	std::string get_club_id_by_table(const std::string& table);

public:
	//20160701 added
	void change_occupied_seats(const std::string& table_id, bool increase);
	void change_occupied_seats(const std::string& table_id, unsigned int seats);
	unsigned int get_occupied_seats(const std::string& table_id);

	void increase_total_buyin(const std::string& table_id, unsigned int chips);
	unsigned int get_total_buyin(const std::string& table_id);

	//discarded, replaced by set_table_insurance_income
	void change_insurance_income(const std::string& table_id, int chips);
	int get_insurance_income(const std::string& table_id);
	void set_table_insurance_income(const std::string& table_id, int chips);

	void increase_user_buyin(const std::string& table_id, const std::string& uid, unsigned int chips);
	unsigned int get_user_buyin(const std::string& table_id, const std::string& uid);

	void save_user_chips(const std::string& table_id, const std::string& uid, int chips);
	void increase_user_chips(const std::string& table_id, const std::string& uid, int chips);
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
	void increase_user_chips_buyin_to_db(const std::string& tid, const std::string& uid,
					int buyin); //更新用户的chips_at_hand, buyin, mysql

private:
	bool table_have_user(const std::string& tid, const std::string& uid);
};

#endif /* APPDPERSISTENCEMYSQL_H_ */
