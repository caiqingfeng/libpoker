/*
 * SerializeHelper.cpp
 *
 *  Created on: 2014年10月16日
 *      Author: luyunstaff001
 */

#include "SerializeHelper.h"
#include "libmrock/common/src/my_log.h"
#include "poker/src/LYCardHelpers.h"
#include <boost/lexical_cast.hpp>

SerializeHelper::SerializeHelper() {
	// TODO Auto-generated constructor stub

}

SerializeHelper::~SerializeHelper() {
	// TODO Auto-generated destructor stub
}

//std::vector<std::pair<std::string, std::string> > SerializeHelper::table2kvp(
//		LYTable *table) {
//	std::vector < std::pair<std::string, std::string> > key_value_pairs;
//	if (table->_id == "") {
//		table->_id = LYUtils::genId();
//	}
//
//	std::pair < std::string, std::string > kvp;
//	kvp.first = "name";
//	kvp.second = table->name;
////	LY_LOG_DBG("name:"+table->name);
//	key_value_pairs.push_back(kvp);
//
//	kvp.first = "owner";
//	kvp.second = table->owner;
////	LY_LOG_DBG("owner:"+table->owner);
//	key_value_pairs.push_back(kvp);
//
//	/*
//	 * integer should be processed to string
//	 */
//	kvp.first = "status";
//	kvp.second = std::to_string(table->status);
////	LY_LOG_DBG("status:"+table->status);
//	key_value_pairs.push_back(kvp);
//
//	kvp.first = "total_seats";
//	kvp.second = std::to_string(table->total_seats);
////	LY_LOG_DBG("total_seats:"+table->total_seats);
//	key_value_pairs.push_back(kvp);
//
//	kvp.first = "timeout";
//	kvp.second = std::to_string(table->timeout);
////	LY_LOG_DBG("total_seats:"+table->total_seats);
//	key_value_pairs.push_back(kvp);
//
//	kvp.first = "occupied_seat_number";
//	kvp.second = std::to_string(table->occupied_seat_number);
////	LY_LOG_DBG("total_seats:"+table->total_seats);
//	key_value_pairs.push_back(kvp);
//
//	kvp.first = "profile_id";
//	kvp.second = table->profile_id;
////	LY_LOG_DBG("total_seats:"+table->total_seats);
//	key_value_pairs.push_back(kvp);
//
//	kvp.first = "timestamp";
//	kvp.second = std::to_string(time(NULL));
//	key_value_pairs.push_back(kvp);
//
//	return key_value_pairs;
//}
//
/*
 * 只处理Table的动态信息
 */
std::vector<std::pair<std::string, std::string> > SerializeHelper::tableStatus2kvp(
		LYTable *table) {
	std::vector < std::pair<std::string, std::string> > key_value_pairs;
	if (table->_id == "") {
		return key_value_pairs;
	}

	std::pair < std::string, std::string > kvp;
	/*
	 * integer should be processed to string
	 */
	kvp.first = "status";
	kvp.second = std::to_string(table->status);
	key_value_pairs.push_back(kvp);

	kvp.first = "occupied_seat_number";
	kvp.second = std::to_string(table->occupied_seat_number);
//	LY_LOG_DBG("total_seats:"+table->total_seats);
	key_value_pairs.push_back(kvp);

	return key_value_pairs;
}

void SerializeHelper::kvp2table(
		std::vector<std::pair<std::string, std::string> >& to,
		LYTable *table) {
	std::vector<std::pair<std::string, std::string> >::iterator it = to.begin();
	try {
		for (; it != to.end(); it++) {
			std::pair < std::string, std::string > kvp = *it;
//		std::cout << "first=" << kvp.first << ", second=" << kvp.second <<  std::endl;
			if (kvp.first == "name") {
				table->name = kvp.second;
			} else if (kvp.first == "total_seats") {
				table->total_seats = boost::lexical_cast<unsigned int>(
						kvp.second);
			} else if (kvp.first == "started_at") {
				table->started_at = boost::lexical_cast<unsigned int>(
						kvp.second);
			} else if (kvp.first == "last_duration") {
				table->last_duration = boost::lexical_cast<unsigned int>(
						kvp.second);
			} else if (kvp.first == "total_games") {
				table->game_round = boost::lexical_cast<unsigned int>(
						kvp.second);
			} else if (kvp.first == "total_buyin") {
				table->total_buyin = boost::lexical_cast<unsigned int>(
						kvp.second);
			} else if (kvp.first == "assurance_income") {
				table->assurance_income = boost::lexical_cast<unsigned int>(
						kvp.second);
			} else if (kvp.first == "assurance_flag") {
				table->assurance_flag = boost::lexical_cast<unsigned int>(
						kvp.second);
//			} else if (kvp.first == "timeout") {
//				table->timeout = boost::lexical_cast<unsigned int>(kvp.second);
			} else if (kvp.first == "occupied_seat_number") {
				table->occupied_seat_number = boost::lexical_cast<unsigned int>(kvp.second);
			} else if (kvp.first == "profile_id") {
				table->profile_id = kvp.second;
			} else if (kvp.first == "status") {
				table->status =
						(LYTableStatus) boost::lexical_cast<unsigned int>(
								kvp.second);
			} else if (kvp.first == "owner") {
				table->owner = kvp.second;
			}
		}
	} catch (boost::bad_lexical_cast & e) {
		LY_LOG_DBG("bad_lexical_cast:" << e.what());
	}
}

//std::vector<std::pair<std::string, std::string> > SerializeHelper::seat2kvp(
//		LYSeat* seat) {
//	std::vector < std::pair<std::string, std::string> > key_value_pairs;
//
//	//table  LYTable
//	std::pair < std::string, std::string > kvp;
//	kvp.first = "seatNo";
//	kvp.second = std::to_string(seat->seatNo);
//	//	LY_LOG_DBG("name:"+table->name);
//	key_value_pairs.push_back(kvp);
//
//	kvp.first = "playerId";
//	kvp.second = seat->playerId;
//	//	LY_LOG_DBG("name:"+table->name);
//	key_value_pairs.push_back(kvp);
//
//	kvp.first = "status";
//	kvp.second = std::to_string(seat->status);
//	//	LY_LOG_DBG("name:"+table->name);
//	key_value_pairs.push_back(kvp);
//
//	return key_value_pairs;
//}
//
/*
 * added 20150221 by cqf
 * 仅更新seat status/gameStatus
 */
std::vector<std::pair<std::string, std::string> > SerializeHelper::seatStatus2kvp(
		LYSeat* seat) {
	std::vector < std::pair<std::string, std::string> > key_value_pairs;

	std::pair < std::string, std::string > kvp;
	kvp.first = "seatNo";
	kvp.second = std::to_string(seat->seatNo);
	//	LY_LOG_DBG("name:"+table->name);
	key_value_pairs.push_back(kvp);

	kvp.first = "status";
	kvp.second = std::to_string(seat->status);
	//	LY_LOG_DBG("name:"+table->name);
	key_value_pairs.push_back(kvp);

	kvp.first = "playerId";
	kvp.second = "NULL";

	if (seat->status == LYSeatOccupied) {
		kvp.second = seat->playerId;
	}
	key_value_pairs.push_back(kvp);

	return key_value_pairs;
}

void SerializeHelper::kvp2seat(
		std::vector<std::pair<std::string, std::string> >& to,
		LYSeat* seat)
{
	std::vector<std::pair<std::string, std::string> >::iterator it = to.begin();
	try {
		for (; it != to.end(); it++) {
			std::pair < std::string, std::string > kvp = *it;
			if (kvp.first == "seatNo") {
				seat->seatNo = (LYApplicant)std::stoi(kvp.second);
			} else if (kvp.first == "chipsAtHand") {
				seat->chipsAtHand = std::stoi(kvp.second);
			} else if (kvp.first == "status") {
				seat->status = (LYSeatStatus)std::stoi(kvp.second);
			} else if (kvp.first == "lastBet") {
				seat->lastBet = std::stoi(kvp.second);
			} else if (kvp.first == "timeoutCount") {
				seat->timeoutCount = std::stoi(kvp.second);
			} else if (kvp.first == "playerId") {
				seat->playerId = kvp.second;
			} else if (kvp.first == "handCards") {
				LYCardHelpers::stringifiedIntToCards(kvp.second, seat->handCards);
			} else if (kvp.first == "chipsAtBeginning") {
				seat->chipsAtBeginning = std::stoi(kvp.second);
			} else if (kvp.first == "newBuyin") {
				seat->newBuyin = std::stoi(kvp.second);
			}
		}
	} catch (boost::bad_lexical_cast & e) {
		LY_LOG_DBG("bad_lexical_cast:" << e.what());
	}
}
