/*
 * LYPineappleRace.cpp
 *
 *  Created on: 2014-9-23
 *      Author: caiqingfeng
 */

#include "LYPineappleRace.h"
#include <boost/lexical_cast.hpp>

LYPineappleRace::LYPineappleRace() {
	// TODO Auto-generated constructor stub

	player = 0;
	scoreAgainst = 0;
	scoreAgainstTop = 0;
	scoreAgainstMiddle = 0;
	scoreAgainstBottom = 0;
	playerAgainst = 0;
	statusAgainst = LYEVEN;
}

LYPineappleRace::LYPineappleRace(const LYPineappleRace& right) {
	// TODO Auto-generated constructor stub
	this->player = right.player;
	this->scoreAgainst = right.scoreAgainst;
	this->playerAgainst = right.playerAgainst;
	this->statusAgainst = right.statusAgainst;
	this->scoreAgainstTop = right.scoreAgainstTop;
	this->scoreAgainstMiddle = right.scoreAgainstMiddle;
	this->scoreAgainstBottom = right.scoreAgainstBottom;
}

LYPineappleRace::~LYPineappleRace() {
	// TODO Auto-generated destructor stub
}

LYPineappleRace LYPineappleRace::turnNegative()
{
	LYPineappleRace pr;
	pr.player = this->playerAgainst;
	pr.playerAgainst = this->player;
	pr.scoreAgainst = this->scoreAgainst * (-1);
	pr.scoreAgainstTop = this->scoreAgainstTop *(-1);
	pr.scoreAgainstMiddle = this->scoreAgainstMiddle * (-1);
	pr.scoreAgainstBottom = this->scoreAgainstBottom * (-1);
	switch (this->statusAgainst)
	{
	case LYEVEN:
		pr.statusAgainst = LYEVEN;
		break;
	case LYSCOOPED:
		pr.statusAgainst = LYBEATALL;
		break;
	case LYBEATALL:
		pr.statusAgainst = LYSCOOPED;
		break;
	default:
		break;
	}
	return pr;
}

std::string LYPineappleRace::toString()
{
	std::string res;
	res = "player:" + boost::lexical_cast<std::string>(player)
			+ "," + "scoreAgainst:" + boost::lexical_cast<std::string>(scoreAgainst)
			+ "," + "playerAgainst:" + boost::lexical_cast<std::string>(playerAgainst)
			+ "," + "statusAgainst:" + boost::lexical_cast<std::string>(statusAgainst)
			+ "," + "scoreAgainstTop:" + boost::lexical_cast<std::string>(scoreAgainstTop)
			+ "," + "scoreAgainstMiddle:" + boost::lexical_cast<std::string>(scoreAgainstMiddle)
			+ "," + "scoreAgainstBottom:" + boost::lexical_cast<std::string>(scoreAgainstBottom);
	return res;
}
