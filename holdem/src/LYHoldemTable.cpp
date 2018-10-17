/*
 * LYHoldemTable.cpp
 *
 *  Created on: 2013-7-6
 *      Author: caiqingfeng
 */

#include "LYHoldemTable.h"
//#include <boost/foreach.hpp>
#include "LYHoldemTrunk.h"
//#include "common/src/my_log.h"
#include "LYHoldemGame.h"
#include <iostream>

LYHoldemTable::LYHoldemTable(const std::string &tid, unsigned int ts, enum LYTableStatus sta,
		const std::string &nm, const std::string &oid, unsigned int occupied, LYProfileMgr *prof_mgr)
{
	// TODO Auto-generated constructor stub
	_id = tid;
	owner = oid;
	status = sta;
	name = nm;
	total_seats = ts;
	occupied_seat_number = occupied;
	createSeats();
    trunk = NULL;
    profileMgr = prof_mgr;
}

LYHoldemTable::LYHoldemTable(const std::string &tid, LYProfileMgr *prof_mgr)
{
	// TODO Auto-generated constructor stub
	_id = tid;
	status = LYTableOpen;
	name = "";
	total_seats = 9;
	createSeats();
    trunk = NULL;
    profileMgr = prof_mgr;
}

LYHoldemTable::~LYHoldemTable() {
	// TODO Auto-generated destructor stub
}

bool LYHoldemTable::playGame(LYHoldemAction &action)
{
//	LY_LOG_DBG("enter LYHoldemTable::playHoldem");
	if (trunk == NULL) {
//		LY_LOG_ERR("fatal error, without trunk!");
		return false;
	}
	return ((LYHoldemTrunk *)trunk)->playGame(action);
}

/*
 * 在反序列化时被外部调用，因为反序列化时，先生成HoldemTable实例
 * 然后得到Table的TotalSeats，再生成Seats实例
 */
void LYHoldemTable::createSeats()
{
	for (unsigned int i = 0; i < total_seats; i++) {
		LYHoldemSeat *seat = new LYHoldemSeat((enum LYApplicant) (i + 1));
		LYSeatPtr sp((LYSeat *)seat);
		seats.push_back(sp);
	}
}

/*
 * after having loaded data from db, call run to set occupied seats
 * register timers
 * ...
 * 20140312 deprecated
 */
void LYHoldemTable::run()
{
//	LY_LOG_DBG("running table...");
	if (trunk == NULL) {
//		LY_LOG_ERR("fatal error, without trunk!");
		return;
	}
	//trunk->run();
}

bool LYHoldemTable::ready2go()
{
//	LY_LOG_DBG("ready2go");
	if (trunk == NULL) {
//		LY_LOG_ERR("fatal error, without trunk!");
		return false;
	}
	return trunk->ready2go();
}

bool LYHoldemTable::isGameOver()
{
//	LY_LOG_DBG("isGameOver");
	if (trunk == NULL) {
//		LY_LOG_ERR("fatal error, without trunk!");
		return true;
	}
	return trunk->isGameOver();
}

void LYHoldemTable::createGame(const std::string &game_id, LYHoldemAlgorithmDelegate *had)
{
//	LY_LOG_DBG("createGame");
	if (trunk == NULL) {
//		LY_LOG_ERR("fatal error, without trunk!");
		return;
	}
	((LYHoldemTrunk *)trunk)->createGame(game_id, had);
	game_round ++;
	current_game_id = game_id;
}

/*
 * 创建游戏
 */
void LYHoldemTable::createGameInstance(const std::string &game_id)
{
	if (trunk == NULL) {
		return;
	}
	((LYHoldemTrunk *)trunk)->createGameInstance(game_id);
	game_round ++;
	current_game_id = game_id;
}

/*
 * 20160311
 */
void LYHoldemTable::createGameInstance(const std::string &game_id, std::vector < std::pair<std::string, std::string> >& kvps)
{
	if (trunk == NULL) {
		return;
	}
	((LYHoldemTrunk *)trunk)->createGameInstance(game_id, kvps);
	game_round ++;
	current_game_id = game_id;
}

void LYHoldemTable::openTrunk(const std::string &trunk_id, const std::string &trunk_name,
		const std::string &player, const std::string &prof_id)
{
	trunk = new LYHoldemTrunk(trunk_id, trunk_name,
			this, this->total_seats, this->seats,
			player, prof_id);
}

void LYHoldemTable::openTrunk(const std::string &trunk_id)
{
	trunk = new LYHoldemTrunk(trunk_id, "trunk_name_is_to_be_done",
			this, this->total_seats, this->seats,
			"", "");
}

void LYHoldemTable::openTrunk(const std::string &trunk_id, const std::string &trunk_name,
			const std::string &player, LYPokerProfile *prof)
{
	trunk = new LYHoldemTrunk(trunk_id, trunk_name,
			this, this->total_seats, this->seats,
			player, prof->_id);
	trunk->profile = prof;
}

void LYHoldemTable::resetSeats() //reset seat to initial status
{
	std::vector<LYSeatPtr>::iterator it = seats.begin();
	for (; it != seats.end(); it++) {
		LYHoldemSeat *seat = (LYHoldemSeat *)(*it).get();
		seat->reset();
	}

	occupied_seat_number = 0;
}

void LYHoldemTable::setSeat(const LYHoldemSeat &seat)
{
	LYSeatPtr st = fetchSeat(seat.seatNo);
	if (NULL == st.get()) {
		return;
	}
	LYHoldemSeat *hst = (LYHoldemSeat *) st.get();
	*hst = seat;
}

LYRanking LYHoldemTable::caculateSeatRanking(LYApplicant seatNo)
{
	LYHoldemGame *game = (LYHoldemGame *)getCurrentGame();
	if (NULL == game) {
		return HighCard;
	}
	return game->caculateSeatRanking(seatNo);
}

unsigned int LYHoldemTable::getCurrentMaxBuyin()
{
	LYHoldemTrunk *trunk = (LYHoldemTrunk *)getTrunk();
	if (trunk == NULL) return 0;
	return trunk->getCurrentMaxBuyin();
}

unsigned int LYHoldemTable::getCurrentMinBuyin()
{
	LYHoldemTrunk *trunk = (LYHoldemTrunk *)getTrunk();
	if (trunk == NULL) return 0;
	return trunk->getCurrentMinBuyin();
}

bool LYHoldemTable::isGameOngoing()
{
	LYHoldemGame* game = (LYHoldemGame*)this->getCurrentGame();
	if (game == NULL) return false;
	return (game->gameStatus == LY_HOLDEMGAME_ONGOING);
}

std::vector < std::pair<std::string, std::string> > LYHoldemTable::table2kvps()
{
	std::vector < std::pair<std::string, std::string> > key_value_pairs;
	key_value_pairs = LYTable::table2kvps();
	return key_value_pairs;
}

std::vector<std::string> LYHoldemTable::getWhiteList()
{
	std::vector<std::string> w;
	LYHoldemGame *game = (LYHoldemGame *)getCurrentGame();
	if (game == NULL) return w;
	return game->getWhiteList();
}

std::vector<std::string> LYHoldemTable::showLastGame()
{
	std::vector<std::string> logs;
	LYHoldemGame *game = (LYHoldemGame *)getLastGame();
	if (game == NULL) return logs;

	return game->getGameLogs();
}

