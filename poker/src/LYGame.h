/*
 * LYGame.h
 *
 *  Created on: 2013-7-5
 *      Author: caiqingfeng
 */

#ifndef LYGAME_H_
#define LYGAME_H_

#include <memory>
#include <string>
#include "LYSeat.h"
#include "LYDeck.h"
#include "LYPokerAlgorithmDelegate.h"

class LYTable;

class LYGame {
public:
	std::string _id;
	LYTable *table;
	std::vector<LYSeatPtr> seatsOfTable;
	LYDeck deck;
	LYPokerAlgorithmDelegate *pad;

public:
	LYGame(const std::string &game_id, std::vector<LYSeatPtr> &all_seats,
			LYTable *tbl, LYPokerAlgorithmDelegate *sd = NULL);
	LYGame(LYPokerAlgorithmDelegate *sd = NULL);
	virtual ~LYGame();
	virtual bool readyForNext() {return true;}; //只在ServerHall的takeSeat时调用，为true要求 round为非游戏状态或者只有两个人时依然是游戏状态（说明恢复状态时处理有问题）
	LYSeatPtr fetchSeat(enum LYApplicant sn);
	void setPokerAlgorithmDelegate(LYPokerAlgorithmDelegate *p);
};

typedef ::std::shared_ptr<LYGame> LYGamePtr;
#endif /* LYGAME_H_ */
