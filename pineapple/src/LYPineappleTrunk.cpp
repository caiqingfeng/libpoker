/*
 * LYPineappleTrunk.cpp
 *
 *  Created on: 2014-10-11
 *      Author: caiqingfeng
 */

#include "LYPineappleTrunk.h"
#include "LYPineappleGame.h"

LYPineappleTrunk::~LYPineappleTrunk()
{

}

LYPineappleTrunk::LYPineappleTrunk(const std::string &trunk_id, const std::string &trunk_name,
		LYPineappleTable *tbl, unsigned int &ts, std::vector<LYSeatPtr> &all_seats,
		const std::string &player, const std::string &prof_id) :
		LYTrunk(trunk_id, trunk_name, (LYTable *)tbl, ts, all_seats, player, prof_id)
{

}

/*
 * Game设计只处理当前的Action，可以保证Server和Client代码一致
 */
bool LYPineappleTrunk::playGame(LYPineappleAction &action)
{
//	LY_LOG_DBG("enter LYHoldemTrunk::playGame");
	bool validAction = ((LYPineappleGame *)currentGame)->onAction(action);
	if (!validAction) {
//		LY_LOG_INF("invalid action");
		return false;
	}
	return true;
}

/*
 *
 */
void LYPineappleTrunk::run(LYHoldemAlgorithmDelegate *had)
{
	currentGame->pad = had;
}

void LYPineappleTrunk::createGame(const std::string &game_id, LYHoldemAlgorithmDelegate *had,
		std::string prof)
{
//	if (!this->ready2go()) {
////		LY_LOG_DBG("not ready to go");
//		return;
//	}

//	this->resetAllSeatsForNewGame();

	if (lastGame != NULL) {
		delete lastGame;
		lastGame = NULL;
	}

	lastGame = currentGame;
	LYPineappleGame *last_game = (LYPineappleGame *)lastGame;
	LYSeatPtr btnSeat;
	if (lastGame != NULL) {
		//brand new game
		btnSeat = ((LYPineappleTable *)this->table)->fetchNextOccupiedSeat(last_game->btnSeatNo);
	} else {
		LYSeatPtr tmpSeat = ((LYPineappleTable *)this->table)->fetchNextOccupiedSeat(LYSeat1);
		btnSeat = tmpSeat;
		tmpSeat = ((LYPineappleTable *)this->table)->fetchNextOccupiedSeat(tmpSeat->seatNo);
		if (tmpSeat->seatNo == LYSeat1) {
			//只有两人
		} else {
			btnSeat = tmpSeat;
		}
		if (btnSeat->status == LYSeatOpen) return;
	}
	currentGame = new LYPineappleGame(game_id, seats,
			btnSeat, (LYPineappleTable *)table, had, false, prof);
}

/*
 * 只是给客户端从空口中创建实例用，或者服务器侧从数据库中恢复实例
 * 所有状态都在后续tableFromOta或者tableFromDb中设置
 */
void LYPineappleTrunk::createGameInstance(const std::string &game_id)
{
	if (lastGame != NULL) {
		delete lastGame;
		lastGame = NULL;
	}

	lastGame = currentGame;
	LYPineappleGame *last_game = (LYPineappleGame *)lastGame;
	currentGame = new LYPineappleGame(game_id, seats,
			LYSeatPtr(), (LYPineappleTable *)table, NULL, true);
}
