/*
 * AppdPersistenceService.cpp
 *
 *  Created on: 2013-6-27
 *      Author: caiqingfeng
 */

#include "PokerPersistenceService.h"
//#include "PersistenceServiceMongo.h"
#include "persistence/src/PersistenceServiceRedis.h"
#include "PokerPersistenceServiceRedis.h"
#include "PokerPersistenceServiceMysql.h"
//#include "SerializeHelper.h"

PokerPersistenceService::PokerPersistenceService(const char db[])
{
	// TODO Auto-generated constructor stub
//	_memStorage = new PokerPersistenceServiceRedis(db);
////	_diskStorage = new AppdPersistenceServiceMysql("rpineapple");
//	std::string rails_db_name;
//	char* railsDbName = getenv("RAILS_DB_NAME");
//	if (railsDbName == NULL) {
//		rails_db_name = "rhappyholdem";
//	} else {
//		rails_db_name = std::string(railsDbName);
//	}
//	_diskStorage = new PokerPersistenceServiceMysql(rails_db_name);
}

PokerPersistenceService::~PokerPersistenceService()
{
	// TODO Auto-generated destructor stub
//	delete _memStorage;
//	delete _diskStorage;
}

/*
 * 20150206新增接口，在服务器启动时就要调用本方法，用于后续区分是对私局操作还是对公局操作
 * 本方法只对Redis有效，因为Tables是存在Redis中
 * 其它的数据如Users，Profiles这些都是对公局和私局均生效
 */
void PokerPersistenceService::set_poker_collection()
{
	((PokerPersistenceServiceRedis *) _memStorage)->set_poker_collection();
}

std::string PokerPersistenceService::get_collection()
{
	return ((PokerPersistenceServiceRedis *) _memStorage)->poker_collection;
}

/*
 * chips_at_hand 这个参数，目前并不用，会从数据库里查出来
 */
void PokerPersistenceService::enter_table(const std::string& table_id,
		const std::string& uid, unsigned int chips_at_hand)
{
	int chips = ((PokerPersistenceServiceMysql *) _diskStorage)->get_user_chips(table_id, uid);
//	std::cout << "chips=" << chips << std::endl;
	((PokerPersistenceServiceRedis *) _memStorage)->enter_table(table_id, uid, chips);
}

unsigned int PokerPersistenceService::leave_table(const std::string& table_id,
		const std::string& uid)
{
	unsigned int chips_at_hand = ((PokerPersistenceServiceRedis *) _memStorage)->leave_table(table_id, uid);
	((PokerPersistenceServiceMysql *) _diskStorage)->save_user_chips(table_id, uid, chips_at_hand);
	return chips_at_hand;
}

void PokerPersistenceService::list_table(std::vector<std::string>& tables)
{
	((PokerPersistenceServiceRedis *) _memStorage)->list_table(tables);
}

void PokerPersistenceService::loadAllProfiles(
		std::vector<LYPokerProfile *> &db_profiles)
{
	((PokerPersistenceServiceMysql *) _diskStorage)->loadAllProfiles(
			db_profiles);
}


void PokerPersistenceService::update_seat_status(LYTable* table, unsigned int seat_no)
{
	((PokerPersistenceServiceRedis *) _memStorage)->update_seat_status(table, seat_no);
}

void PokerPersistenceService::fetch_player(const std::string& uid,
		LYPlayer& player)
{
	((PokerPersistenceServiceMysql *) _diskStorage)->fetch_player(uid, player);
}

void PokerPersistenceService::update_balance(const std::string& uid,
		const std::string& game, int& delta)
{
	((PokerPersistenceServiceMysql *) _diskStorage)->update_balance(uid, game,
			delta);
}

int PokerPersistenceService::get_user_chips(const std::string& table_id, const std::string& uid)
{
	return ((PokerPersistenceServiceMysql *) _diskStorage)->get_user_chips(table_id, uid);
}

std::string PokerPersistenceService::get_club_id_by_table(const std::string& table)
{
	return ((PokerPersistenceServiceMysql *) _diskStorage)->get_club_id_by_table(table);
}

//20160701 added
void PokerPersistenceService::change_occupied_seats(const std::string& table_id, bool increase)
{
	((PokerPersistenceServiceMysql *) _diskStorage)->change_occupied_seats(table_id, increase);
}

void PokerPersistenceService::change_occupied_seats(const std::string& table_id, unsigned int seats)
{
	((PokerPersistenceServiceRedis *) _memStorage)->change_occupied_seats(table_id, seats);
	((PokerPersistenceServiceMysql *) _diskStorage)->change_occupied_seats(table_id, seats);
}

unsigned int PokerPersistenceService::get_occupied_seats(const std::string& table_id)
{
	return ((PokerPersistenceServiceMysql *) _diskStorage)->get_occupied_seats(table_id);
}

void PokerPersistenceService::increase_total_buyin(const std::string& table_id, unsigned int chips)
{
	((PokerPersistenceServiceMysql *) _diskStorage)->increase_total_buyin(table_id, chips);
}

unsigned int PokerPersistenceService::get_total_buyin(const std::string& table_id)
{
	return ((PokerPersistenceServiceMysql *) _diskStorage)->get_total_buyin(table_id);
}

void PokerPersistenceService::change_insurance_income(const std::string& table_id, int chips)
{
	((PokerPersistenceServiceMysql *) _diskStorage)->change_insurance_income(table_id, chips);
}

void PokerPersistenceService::set_table_insurance_income(const std::string& table_id, int chips)
{
	((PokerPersistenceServiceMysql *) _diskStorage)->set_table_insurance_income(table_id, chips);
}

int PokerPersistenceService::get_insurance_income(const std::string& table_id)
{
	return ((PokerPersistenceServiceMysql *) _diskStorage)->get_insurance_income(table_id);
}

void PokerPersistenceService::increase_user_buyin(const std::string& table_id, const std::string& uid, unsigned int chips)
{
	((PokerPersistenceServiceMysql *) _diskStorage)->increase_user_buyin(table_id, uid, chips);
}

unsigned int PokerPersistenceService::get_user_buyin(const std::string& table_id, const std::string& uid)
{
	return ((PokerPersistenceServiceMysql *) _diskStorage)->get_user_buyin(table_id, uid);
}

void PokerPersistenceService::save_user_chips(const std::string& table_id, const std::string& uid, int chips)
{
	((PokerPersistenceServiceMysql *) _diskStorage)->save_user_chips(table_id, uid, chips);
}

void PokerPersistenceService::increase_table_games(const std::string& table_id)
{
	((PokerPersistenceServiceMysql *) _diskStorage)->increase_table_games(table_id);
}

unsigned int PokerPersistenceService::get_table_games(const std::string& table_id)
{
	return ((PokerPersistenceServiceMysql *) _diskStorage)->get_table_games(table_id);
}

void PokerPersistenceService::increase_club_games(const std::string& club)
{
	((PokerPersistenceServiceMysql *) _diskStorage)->increase_club_games(club);
}

unsigned int PokerPersistenceService::get_club_games(const std::string& club)
{
	return ((PokerPersistenceServiceMysql *) _diskStorage)->get_club_games(club);
}

void PokerPersistenceService::increase_user_club_games(const std::string& cid, const std::string& uid)
{
	((PokerPersistenceServiceMysql *) _diskStorage)->increase_user_club_games(cid, uid);
}

unsigned int PokerPersistenceService::get_user_club_games(const std::string& cid, const std::string& uid)
{
	return ((PokerPersistenceServiceMysql *) _diskStorage)->get_user_club_games(cid, uid);
}

void PokerPersistenceService::increase_user_club_inpots(const std::string& cid, const std::string& uid)
{
	((PokerPersistenceServiceMysql *) _diskStorage)->increase_user_club_inpots(cid, uid);
}

unsigned int PokerPersistenceService::get_user_club_inpots(const std::string& cid, const std::string& uid)
{
	return ((PokerPersistenceServiceMysql *) _diskStorage)->get_user_club_inpots(cid, uid);
}

void PokerPersistenceService::increase_user_club_wins(const std::string& cid, const std::string& uid)
{
	((PokerPersistenceServiceMysql *) _diskStorage)->increase_user_club_wins(cid, uid);
}

unsigned int PokerPersistenceService::get_user_club_wins(const std::string& cid, const std::string& uid)
{
	return ((PokerPersistenceServiceMysql *) _diskStorage)->get_user_club_wins(cid, uid);
}

void PokerPersistenceService::change_user_club_income(const std::string& cid, const std::string& uid, int income)
{
	((PokerPersistenceServiceMysql *) _diskStorage)->change_user_club_income(cid, uid, income);
}

int PokerPersistenceService::get_user_club_income(const std::string& cid, const std::string& uid)
{
	return ((PokerPersistenceServiceMysql *) _diskStorage)->get_user_club_income(cid, uid);
}

void PokerPersistenceService::increase_user_table_games(const std::string& table_id, const std::string& uid)
{
	((PokerPersistenceServiceMysql *) _diskStorage)->increase_user_table_games(table_id, uid);
}

unsigned int PokerPersistenceService::get_user_table_games(const std::string& table_id, const std::string& uid)
{
	return ((PokerPersistenceServiceMysql *) _diskStorage)->get_user_table_games(table_id, uid);
}

void PokerPersistenceService::increase_user_table_wins(const std::string& table_id, const std::string& uid)
{
	((PokerPersistenceServiceMysql *) _diskStorage)->increase_user_table_wins(table_id, uid);
}

unsigned int PokerPersistenceService::get_user_table_wins(const std::string& table_id, const std::string& uid)
{
	return ((PokerPersistenceServiceMysql *) _diskStorage)->get_user_table_wins(table_id, uid);
}

void PokerPersistenceService::increase_user_table_inpots(const std::string& table_id, const std::string& uid)
{
	((PokerPersistenceServiceMysql *) _diskStorage)->increase_user_table_inpots(table_id, uid);
}

unsigned int PokerPersistenceService::get_user_table_inpots(const std::string& table_id, const std::string& uid)
{
	return ((PokerPersistenceServiceMysql *) _diskStorage)->get_user_table_inpots(table_id, uid);
}

void PokerPersistenceService::change_user_table_income(const std::string& table_id, const std::string& uid, int income)
{
	((PokerPersistenceServiceMysql *) _diskStorage)->change_user_table_income(table_id, uid, income);
}

int PokerPersistenceService::get_user_table_income(const std::string& table_id, const std::string& uid)
{
	return ((PokerPersistenceServiceMysql *) _diskStorage)->get_user_table_income(table_id, uid);
}

void PokerPersistenceService::increase_user_chips_to_mb(const std::string& tid, const std::string& uid,
					unsigned int seat_no, int buyin) //更新用户的chips_at_hand, redis
{
	((PokerPersistenceServiceRedis *) _memStorage)->increase_user_chips_to_mb(tid, uid, seat_no, buyin);
}

void PokerPersistenceService::increase_user_chips_buyin_to_db(const std::string& tid, const std::string& uid,
					int buyin) //更新用户的chips_at_hand, buyin, mysql
{
	((PokerPersistenceServiceMysql *) _diskStorage)->increase_user_chips_buyin_to_db(tid, uid, buyin);
}

void PokerPersistenceService::save_user_chips_to_mb(const std::string& tid, const std::string& uid,
					unsigned int seat_no, int buyin) //更新用户的chips_at_hand, redis
{
	((PokerPersistenceServiceRedis *) _memStorage)->save_user_chips_to_mb(tid, uid, seat_no, buyin);
}

int PokerPersistenceService::get_user_chips_from_db(const std::string& tid, const std::string& uid
					) //查询用户的chips_at_hand mysql
{
	return 	get_user_chips(tid, uid);
}

int PokerPersistenceService::get_user_chips_from_mb(const std::string& tid, const std::string& uid
					) //查询用户的chips_at_hand, redis
{
	return ((PokerPersistenceServiceRedis *) _memStorage)->get_user_chips_from_mb(tid, uid);
}

void PokerPersistenceService::drop_user_from_mb(const std::string& tid, const std::string& uid
					) //删除用户, redis
{
	((PokerPersistenceServiceRedis *) _memStorage)->drop_user_from_mb(tid, uid);
}

unsigned int PokerPersistenceService::get_occupied_seats_from_mb(const std::string& tid)
{
	return ((PokerPersistenceServiceRedis *) _memStorage)->get_occupied_seats_from_mb(tid);
}

