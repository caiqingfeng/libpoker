/*
 * LYGame.cpp
 *
 *  Created on: 2013-7-5
 *      Author: caiqingfeng
 */

#include "LYGame.h"
//#include "common/src/my_log.h"
//#include <boost/foreach.hpp>

LYGame::LYGame(const std::string &game_id, std::vector<LYSeatPtr> &all_seats,
		LYTable *tbl, LYPokerAlgorithmDelegate *sd) : deck(sd)
{
	// TODO Auto-generated constructor stub
	table = tbl;
	_id = game_id;
	seatsOfTable = all_seats;
	pad = sd;
	deck.shuffle();
}

LYGame::LYGame(LYPokerAlgorithmDelegate *sd) : deck(sd)
{
	table = NULL;
	_id = "";
	seatsOfTable.clear();
	pad = sd;
	deck.shuffle();
}

LYGame::~LYGame() {
	// TODO Auto-generated destructor stub
}

LYSeatPtr LYGame::fetchSeat(enum LYApplicant player)
{
	//caiqingfeng 20130418, 不能调用Game的接口，因为需要在没有Game的时候调用
//    if (NULL == dealer.get() || NULL == dealer->scene.get())
//        return LYSeatPtr();
//    return dealer->scene->fetchSeat(player);
	if ((unsigned int)player < 1 || (unsigned int)player > 9) {
//		LY_LOG_ERR("player must be between 1..9, " << (unsigned int)player);
		return LYSeatPtr();
	}
	std::vector<LYSeatPtr>::iterator it = seatsOfTable.begin();
	for(; it != seatsOfTable.end(); it++) {
		LYSeatPtr seat = *it;
        if (seat->isMe(player)) {
             return seat;
        }
    }
	return LYSeatPtr();
}

void LYGame::setPokerAlgorithmDelegate(LYPokerAlgorithmDelegate *p)
{
	pad = p;
}

