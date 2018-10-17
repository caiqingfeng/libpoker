/*
 * LYPineappleRace.h
 *
 *  Created on: 2014-9-23
 *      Author: caiqingfeng
 */

#ifndef LYPINEAPPLERACE_H_
#define LYPINEAPPLERACE_H_

#include <string>

const unsigned int LYEVEN = 0;
const unsigned int LYSCOOPED = 1;
const unsigned int LYBEATALL = 2;

class LYPineappleRace {
public:
	unsigned int player; //

public:
	int scoreAgainst;
	unsigned int playerAgainst; //玩家位置
	unsigned int statusAgainst; //0 even, 1 scooped, 2 beat_all

public:
	int scoreAgainstTop;
	int scoreAgainstMiddle;
	int scoreAgainstBottom;

public:
	LYPineappleRace();
	LYPineappleRace(const LYPineappleRace& pr);
	virtual ~LYPineappleRace();

public:
	LYPineappleRace& operator = (const LYPineappleRace &right)  {
		this->player = right.player;
		this->scoreAgainst = right.scoreAgainst;
		this->playerAgainst = right.playerAgainst;
		this->statusAgainst = right.statusAgainst;
		this->scoreAgainstTop = right.scoreAgainstTop;
		this->scoreAgainstMiddle = right.scoreAgainstMiddle;
		this->scoreAgainstBottom = right.scoreAgainstBottom;
		return *this;
	}

	LYPineappleRace turnNegative(); //*-1

public:
	std::string toString();
};


#endif /* LYPINEAPPLERACE_H_ */

