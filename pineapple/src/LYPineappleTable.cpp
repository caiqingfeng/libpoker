/*
 * LYPineappleTable.cpp
 *
 *  Created on: 2014-10-11
 *      Author: caiqingfeng
 */

#include <sstream>
#include "LYPineappleTable.h"
#include "LYPineappleTrunk.h"
#include "libmrock/common/src/LYUtils.h"
#include "LYPineappleGame.h"

LYPineappleTable::LYPineappleTable(const std::string& creator, std::string prof) : LYTable()
{
	_id = LYUtils::genId();
	owner = creator;
	total_seats = 3;
	seat1 = NULL;
	seat2 = NULL;
	seat3 = NULL;
	profile_id = prof;
	createSeats();
}

LYPineappleTable::LYPineappleTable(const std::string& tid, const std::string& creator,
		unsigned int size, std::string prof)  : LYTable()
{
	_id = tid;
	owner = creator;
	total_seats = size;
	profile_id = prof;
	seat1 = NULL;
	seat2 = NULL;
	seat3 = NULL;
	profile_id = prof;
	createSeats();
}

LYPineappleTable::LYPineappleTable()
{
}

LYPineappleTable::~LYPineappleTable()
{
}

/*
 * 在反序列化时被外部调用，因为反序列化时，先生成HoldemTable实例
 * 然后得到Table的TotalSeats，再生成Seats实例
 */
void LYPineappleTable::createSeats()
{
	for (unsigned int i = 0; i < total_seats; i++) {
		LYPineappleSeat *seat = new LYPineappleSeat((enum LYApplicant) (i + 1));
		if (i == 0) seat1 = seat;
		if (i == 1) seat2 = seat;
		if (i == 2) seat3 = seat;
		LYSeatPtr sp((LYSeat *)seat);
		seats.push_back(sp);
	}
}


/*
 * 创建游戏
 */
void LYPineappleTable::createGame(const std::string &game_id, LYHoldemAlgorithmDelegate *had,
		std::string prof)
{
	if (trunk == NULL) {
		return;
	}
	status = LYTableInGame;
	((LYPineappleTrunk *)trunk)->createGame(game_id, had, prof);
	game_round ++;
	current_game_id = game_id;
}

/*
 * 创建游戏
 */
void LYPineappleTable::createGameInstance(const std::string &game_id)
{
	if (trunk == NULL) {
		return;
	}
	((LYPineappleTrunk *)trunk)->createGameInstance(game_id);
	game_round ++;
	current_game_id = game_id;
}

/*
 * 创建游戏
 */
void LYPineappleTable::run( LYHoldemAlgorithmDelegate *had)
{
	if (trunk == NULL) {
		return;
	}
	((LYPineappleTrunk *)trunk)->run(had);
}

void LYPineappleTable::openTrunk(const std::string &trunk_id, const std::string &trunk_name,
		const std::string &player, const std::string &prof_id)
{
	trunk = new LYPineappleTrunk(trunk_id, trunk_name,
			this, this->total_seats, this->seats,
			player, prof_id);
}

bool LYPineappleTable::playGame(LYPineappleAction &action)
{
	if (trunk == NULL) {
		return false;
	}
	return ((LYPineappleTrunk *)trunk)->playGame(action);
}

void LYPineappleTable::resetSeatsForNewGame()
{
	if (total_seats < 2) return;
	seat1->resetForNewGame();
	seat2->resetForNewGame();
	if (total_seats > 2) seat3->resetForNewGame();
}

/*
 * 如果有Fantasy话，必须Go
 * 否则
 */
bool LYPineappleTable::ready2go(bool auto_start)
{
	LYPineappleGame *game = (LYPineappleGame *)this->getCurrentGame();
	if (game != NULL && game->hasFantasy()) return true;

	if (total_seats < 2) return false;
	if (seat1->status == LYSeatOpen || seat1->status == LYSeatReserved) return false;
	if (seat2->status == LYSeatOpen || seat2->status == LYSeatReserved) return false;
	if (total_seats > 2 &&
			(seat3->status == LYSeatOpen || seat2->status == LYSeatReserved)) return false;
	if (!auto_start) {
		if (!seat1->voteNext) return false;
		if (!seat2->voteNext) return false;
		if (total_seats > 2 &&
				!seat3->voteNext) return false;
	}
	return true;
}

void LYPineappleTable::voteForNext(const std::string& uid)
{
	LYSeatPtr st = this->fetchSeatByPlayerId(uid);
	LYPineappleSeat* seat = (LYPineappleSeat*)st.get();
	if (seat == NULL) return;
	seat->voteNext = true;
	seat->gameStatus = LYNORMAL;
}

/*
 * 20150205新增
 * 判断是否在游戏中，如果刚好在游戏结束时，是否是范特西
 */
bool LYPineappleTable::canLeave(unsigned int seat_no)
{
	LYSeatPtr st = this->fetchSeat((LYApplicant)seat_no);
	LYPineappleSeat* seat = (LYPineappleSeat*)st.get();
	if (seat == NULL) return true;

	if (status != LYTableInGame) return true;
	LYPineappleGame* game = (LYPineappleGame *)getCurrentGame();
	if (game->gameStatus == LYPINEAPPLE_GAME_ONGOING) return false;
	//游戏已经结束的状态
	if (game->hasFantasy()) return false;
	return true;
}

/*
 * 20150208新增
 * 判断是否需要被踢出
 */
std::vector<LYSeatPtr> LYPineappleTable::findUnqualifiedSeats(unsigned int min_chips)
{
	std::vector<LYSeatPtr> sts;
	std::vector<LYSeatPtr>::iterator it = seats.begin();
	for (; it != seats.end(); it++) {
		LYPineappleSeat* ps = (LYPineappleSeat*)(*it).get();
		if ((ps->status == LYSeatOccupied || ps->status == LYSeatReserved) && ps->chipsAtHand < min_chips) {
			sts.push_back(*it);
		}
	}
	return sts;
}

std::vector<LYSeatPtr> LYPineappleTable::findReservedSeats()
{
	std::vector<LYSeatPtr> sts;
	std::vector<LYSeatPtr>::iterator it = seats.begin();
	for (; it != seats.end(); it++) {
		LYPineappleSeat* ps = (LYPineappleSeat*)(*it).get();
		if (ps->status == LYSeatReserved) {
			sts.push_back(*it);
		}
	}
	return sts;
}

bool LYPineappleTable::isGameOngoing()
{
	LYPineappleGame* game = (LYPineappleGame*)this->getCurrentGame();
	if (game == NULL) return false;
	return (game->gameStatus == LYPINEAPPLE_GAME_ONGOING);
}
