/*
 * LYTuringStrategy.cpp
 *
 *  Created on: 2014年11月25日
 *      Author: caiqingfeng
 */
#include "LYHoldemTuringStrategy.h"

#include <iostream>

#include "poker/src/LYCardHelpers.h"
#include "LYHoldemGame.h"

LYHoldemTuringStrategy *LYHoldemTuringStrategy::_instance = NULL;

LYHoldemTuringStrategy::LYHoldemTuringStrategy()
{

}

LYHoldemTuringStrategy::~LYHoldemTuringStrategy()
{

}

LYHoldemTuringStrategy* LYHoldemTuringStrategy::Instance()
{
	if (NULL == _instance) {
		_instance = new LYHoldemTuringStrategy();
	}
	return _instance;
}

/*
 * call 到底：
 */
void LYHoldemTuringStrategy::makeAction(LYHoldemTable* table, LYApplicant seat_no, LYHoldemAction& action)
{
	LYSeatPtr seat;
	if (table == NULL || ((seat = table->fetchSeat(seat_no)).get() == NULL)) {
		return;
	}

	LYHoldemGame *game = (LYHoldemGame *)table->getCurrentGame();

	LYHoldemSeat* ps = (LYHoldemSeat* )seat.get();
	action.call(seat_no, game->betIfCall);
}
