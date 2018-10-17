/*
 * PersistenceServiceRedis.cpp
 *
 *  Created on: 2014-4-10
 *      Author: caiqingfeng
 */

#include "PokerPersistenceServiceRedis.h"
#include "SerializeHelper.h"

PokerPersistenceServiceRedis::PokerPersistenceServiceRedis(const char collec[]) :
		PersistenceServiceRedis(collec)
{
	poker_collection = dbName + "_" + railsEnv; //".pineapple"
}

PokerPersistenceServiceRedis::~PokerPersistenceServiceRedis()
{
	// TODO Auto-generated destructor stub
}

void PokerPersistenceServiceRedis::set_poker_collection()
{
}

/*
 * added 20150221 by cqf
 * 主要是take/leave seat时需要更新 seat status
 * 同时要更新table->occupied_seat_number
 */
void PokerPersistenceServiceRedis::update_seat_status(LYTable* table, unsigned int seat_no)
{
	//首先更新seat status
	std::string seats_collection = poker_collection + ".tables."
			+ table->_id + ".seats";
	LYSeatPtr st = table->fetchSeat((LYApplicant)seat_no);
	LYSeat *seat = (LYSeat *) st.get();
	std::vector < std::pair<std::string, std::string> > kvps = SerializeHelper::seatStatus2kvp(seat);
	store_kvps_to_hash(seats_collection, std::to_string(seat_no),
			kvps);

	//其次更新table occupied number
	std::string tables_collection = poker_collection + ".tables";
	kvps = SerializeHelper::tableStatus2kvp(table);
	store_kvps_to_hash(tables_collection, table->_id,
			kvps);
}

/*
 * .tables.table1234.players.player1.{uid, player1}
 *
 *
 */
void PokerPersistenceServiceRedis::enter_table(const std::string& table_id,
		const std::string& uid, int chips_at_hand)
{
	std::vector < std::pair<std::string, std::string> > kvps;
	std::string instance = uid;
	std::string table_players_collection = poker_collection + ".tables." + table_id + ".players" ;

	std::pair<std::string, std::string> player;
	player.first = "uid";
	player.second = uid;
	kvps.push_back(player);
	player.first = "chipsAtHand";
	player.second = std::to_string(chips_at_hand);
	kvps.push_back(player);
	player.first = "enter_at";
	player.second = std::to_string(time(NULL));
	kvps.push_back(player);
	player.first = "in_table";
	player.second = std::to_string(true);
	kvps.push_back(player);

	store_kvps_to_hash(table_players_collection, instance,
			kvps);
}

int PokerPersistenceServiceRedis::leave_table(const std::string& table_id,
		const std::string& uid)
{
	int chips_at_hand = get_user_chips_from_mb(table_id, uid);
	drop_user_from_mb(table_id, uid);

	return chips_at_hand;
}

void PokerPersistenceServiceRedis::list_table(std::vector<std::string>& tables)
{
	std::string table_collection = poker_collection + ".tables";
	retrieve_index_to_vector(table_collection, tables);
}

/*
 * 同时更新player's chip_at_hand和seat's chips_at_hand
 */
void PokerPersistenceServiceRedis::increase_user_chips_to_mb(const std::string& table_id, const std::string& uid,
					unsigned int seat_no, int buyin) //更新用户的chips_at_hand, redis
{
	int chips_at_hand = get_user_chips_from_mb(table_id, uid);

	std::vector < std::pair<std::string, std::string> > kvps;
	std::pair<std::string, std::string> player;
	player.first = "chipsAtHand";
	player.second = std::to_string(chips_at_hand + buyin);
	kvps.push_back(player);

//更新player's chips_at_hand
	std::string instance = uid;
	std::string table_players_collection = poker_collection + ".tables." + table_id + ".players" ;
	store_kvps_to_hash(table_players_collection, instance,
			kvps);

	if (seat_no == 0 || seat_no > 9) return;

//更新seat's chips_at_hand
	std::string table_seats_collection = poker_collection + ".tables." + table_id + ".seats" ;
	instance = std::to_string(seat_no);
	store_kvps_to_hash(table_seats_collection, instance,
			kvps);
}

void PokerPersistenceServiceRedis::save_user_chips_to_mb(const std::string& table_id, const std::string& uid,
					unsigned int seat_no, int buyin)//更新用户的chips_at_hand, redis
{
	std::vector < std::pair<std::string, std::string> > kvps;
	std::pair<std::string, std::string> player;
	player.first = "chipsAtHand";
	player.second = std::to_string(buyin);
	kvps.push_back(player);

//更新player's chips_at_hand
	std::string instance = uid;
	std::string table_players_collection = poker_collection + ".tables." + table_id + ".players" ;
	store_kvps_to_hash(table_players_collection, instance,
			kvps);

	if (seat_no == 0 || seat_no > 9) return;

//更新seat's chips_at_hand
	std::string table_seats_collection = poker_collection + ".tables." + table_id + ".seats" ;
	instance = std::to_string(seat_no);
	store_kvps_to_hash(table_seats_collection, instance,
			kvps);
}

int PokerPersistenceServiceRedis::get_user_chips_from_mb(const std::string& table_id, const std::string& uid
					) //查询用户的chips_at_hand, redis
{
	std::vector < std::pair<std::string, std::string> > kvps;
	std::string instance = uid;
	std::string table_players_collection = poker_collection + ".tables." + table_id + ".players" ;
	retrieve_hash_to_kvps(table_players_collection, instance, kvps);
	int chips_at_hand = 0;
	std::vector < std::pair<std::string, std::string> >::iterator it = kvps.begin();
	for (; it != kvps.end(); it++) {
		std::string k = (*it).first;
		if (k == "chipsAtHand") {
			std::string v = (*it).second;
			chips_at_hand = std::stoi(v);
			break;
		}
	}

	return chips_at_hand;
}

void PokerPersistenceServiceRedis::drop_user_from_mb(const std::string& table_id, const std::string& uid
					) //删除用户, redis
{
	std::vector < std::pair<std::string, std::string> > kvps;
	std::string instance = uid;
	std::string table_players_collection = poker_collection + ".tables." + table_id + ".players" ;
	std::string table_players_collection_index = table_players_collection + ".index" ;

	rm_index_from_zset(table_players_collection_index, instance);
	drop_hash(table_players_collection, instance);

	return;
}

unsigned int PokerPersistenceServiceRedis::get_occupied_seats_from_mb(const std::string& table_id)
{
	unsigned int s = 0;
	std::vector < std::pair<std::string, std::string> > kvps;
	std::string instance = table_id;
	std::string tables_collection = poker_collection + ".tables";
	std::string table_collection = tables_collection + "." + instance;

	retrieve_hash_to_kvps(tables_collection, instance, kvps);
	std::vector<std::pair<std::string, std::string> >::iterator it = kvps.begin();
	try {
		for (; it != kvps.end(); it++) {
			std::pair < std::string, std::string > kvp = *it;
			if (kvp.first == "occupied_seat_number") {
				s = std::stoi(kvp.second);
				return s;
			}
		}
	} catch (std::exception & e) {
		LY_LOG_DBG("bad_lexical_cast:" << e.what());
	}

	return s;
}

void PokerPersistenceServiceRedis::change_occupied_seats(const std::string& table_id, unsigned int seats)
{
//table basic informations
	std::vector < std::pair<std::string, std::string> > kvps;
	std::pair<std::string, std::string> kvp;
	kvp.first = "occupied_seat_number";
	kvp.second = std::to_string(seats);
	kvps.push_back(kvp);

	std::string tables_collection = poker_collection + ".tables";
	store_kvps_to_hash(tables_collection, table_id, kvps);
}