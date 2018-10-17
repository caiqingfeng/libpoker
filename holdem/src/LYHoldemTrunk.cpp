/*
 * LYHoldemTrunk.cpp
 *
 *  Created on: 2013-7-5
 *      Author: caiqingfeng
 */

#include <cstdlib>
#include "LYHoldemTrunk.h"
#include "LYHoldemTable.h"
#include "LYHoldemGame.h"
//#include <boost/foreach.hpp>
//#include "common/src/my_log.h"

LYHoldemTrunk::	LYHoldemTrunk(const std::string &trunk_id, const std::string &trunk_name,
		LYHoldemTable *tbl, unsigned int &ts, std::vector<LYSeatPtr> &all_seats,
		const std::string &player, const std::string &pf) :
		LYTrunk(trunk_id, trunk_name, (LYTable *)tbl, ts, all_seats, player, pf)
{
	// TODO Auto-generated constructor stub
	if (NULL == tbl->profileMgr) {
		profile = NULL;
	} else {
		if ("" != pf) {
			profile = (LYHoldemProfile *)tbl->profileMgr->getHoldemProfileById(pf).get();
		}
	}
}

LYHoldemTrunk::~LYHoldemTrunk() {
	// TODO Auto-generated destructor stub
}

/*
 * Game设计只处理当前的Action，可以保证Server和Client代码一致
 */
bool LYHoldemTrunk::playGame(LYHoldemAction &action)
{
//	LY_LOG_DBG("enter LYHoldemTrunk::playGame");
	if (currentGame == NULL) return false;
	bool validAction = ((LYHoldemGame *)currentGame)->onAction(action);
	if (!validAction) {
//		LY_LOG_INF("invalid action");
		return false;
	}
	return true;
}

void LYHoldemTrunk::createGame(const std::string &game_id, LYHoldemAlgorithmDelegate *had)
{
	if (!this->ready2go()) {
//		LY_LOG_DBG("not ready to go");
		return;
	}

	this->resetAllSeatsForNewGame();

	if (lastGame != NULL) {
		delete lastGame;
		lastGame = NULL;
	}

	lastGame = currentGame;
	LYHoldemGame *last_game = (LYHoldemGame *)lastGame;
	LYSeatPtr btnSeat;
	LYSeatPtr sbSeat;
	LYSeatPtr bbSeat;
	if (lastGame != NULL) {
		//brand new game
		btnSeat = ((LYHoldemTable *)this->table)->fetchNextOccupiedSeat(last_game->btnSeatNo);
	} else {
		srand(time(NULL));
		LYApplicant random_seat = (enum LYApplicant)(rand()%9+1);
		btnSeat = ((LYHoldemTable *)this->table)->fetchNextOccupiedSeat(random_seat);
	}
	sbSeat = ((LYHoldemTable *)this->table)->fetchNextOccupiedSeat(btnSeat->seatNo);
	bbSeat = ((LYHoldemTable *)this->table)->fetchNextOccupiedSeat(sbSeat->seatNo);
	if (sbSeat->seatNo == btnSeat->seatNo || bbSeat->seatNo == btnSeat->seatNo) {
		//20160413增加，只有2个人的时候，Btn小盲
		sbSeat = btnSeat;
		bbSeat = ((LYHoldemTable *)this->table)->fetchNextOccupiedSeat(btnSeat->seatNo);
	}
	currentGame = new LYHoldemGame(game_id, seats, this->getSmallBlindPrice(), this->getBigBlindPrice(),
			btnSeat, sbSeat, bbSeat, (LYHoldemTable *)table, had);
}

unsigned int LYHoldemTrunk::getSmallBlindPrice()
{
	//此处应该调用ProfileMgr的接口
	if (NULL == profile) {
		return 10;
	}
	return profile->get_small_blind();
}

unsigned int LYHoldemTrunk::getBigBlindPrice()
{
	//此处应该调用ProfileMgr的接口
	if (NULL == profile) {
		return 25;
	}
	return profile->get_big_blind();
}

unsigned int LYHoldemTrunk::getCurrentMaxBuyin()
{
	//此处应该调用ProfileMgr的接口
	if (NULL == profile) {
		return 0;
	}
	return profile->get_max_chips();
}

unsigned int LYHoldemTrunk::getCurrentMinBuyin()
{
	//此处应该调用ProfileMgr的接口
	if (NULL == profile) {
		return 0;
	}
	return profile->get_min_chips();
}

bool LYHoldemTrunk::ready2go()
{
	if (currentGame != NULL) {
		enum LYGameRound round = ((LYHoldemGame *)currentGame)->getRound();
		if (round != LYGameWaiting && round != LYGameClosed && round != LYGameInit) {
//			LY_LOG_ERR("there is a game ongoing ... round=" << ((LYHoldemGame *)currentGame)->getRound());
			return false;
		}
	}

	unsigned int seated = 0;
	std::vector<LYSeatPtr>::iterator it = seats.begin();
	for (; it!=seats.end(); it++) {
		LYSeatPtr st = *it;
		if (st->status != LYSeatOpen && st->chipsAtHand > 0) {
			seated++;
		}
	}
	if (seated < 2) {
//		LY_LOG_ERR("seated:" << seated << " must be greater than 2" );
		return false;
	}
	return true;
}

bool LYHoldemTrunk::isGameOver()
{
	if (currentGame == NULL || ((LYHoldemGame *)currentGame)->getRound() == LYGameClosed) {
		return true;
	}
	return false;
}

void LYHoldemTrunk::resetAllSeatsForNewGame()
{
	std::vector<LYSeatPtr>::iterator it = seats.begin();
	for(; it != seats.end(); it++) {
		LYSeatPtr st = *it;
		LYHoldemSeat *holdemSeat = (LYHoldemSeat *)st.get();
		holdemSeat->resetForNewGame();
	}
}

void LYHoldemTrunk::activeProfile()
{
	if ("" != profile_id && ((LYHoldemTable *)table)->profileMgr != NULL) {
		profile = (LYHoldemProfile *)(((LYHoldemTable *)table)->profileMgr->getHoldemProfileById(profile_id).get());
	}
}

/*
 * 只是给客户端从空口中创建实例用，或者服务器侧从数据库中恢复实例
 * 所有状态都在后续tableFromOta或者tableFromDb中设置
 */
void LYHoldemTrunk::createGameInstance(const std::string &game_id)
{
	if (lastGame != NULL) {
		delete lastGame;
		lastGame = NULL;
	}

	lastGame = currentGame;
	LYHoldemGame *last_game = (LYHoldemGame *)lastGame;
	currentGame = new LYHoldemGame(game_id, seats,
			(LYHoldemTable *)table);
}

/*
 * 只是给客户端从空口中创建实例用，或者服务器侧从数据库中恢复实例
 * 20160311
 */
void LYHoldemTrunk::createGameInstance(const std::string &game_id, std::vector < std::pair<std::string, std::string> >& kvps)
{
	if (lastGame != NULL) {
		delete lastGame;
		lastGame = NULL;
	}

	lastGame = currentGame;
	LYHoldemGame *last_game = (LYHoldemGame *)lastGame;
	currentGame = new LYHoldemGame(game_id, seats,
			(LYHoldemTable *)table, kvps);
}