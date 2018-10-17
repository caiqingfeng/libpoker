/*
 * LYPineappleSeat.cpp
 *
 *  Created on: 2014-10-5
 *      Author: caiqingfeng
 */

#include <sstream>
#include "LYPineappleSeat.h"
#include <boost/lexical_cast.hpp>

LYPineappleSeat::LYPineappleSeat(LYApplicant seat_no) : LYSeat(seat_no)
{
	reset();
}

LYPineappleSeat::LYPineappleSeat(LYPineappleSeatDynamics &seat_dyn) : LYSeat(seat_dyn.seatNo)
{
	reset();
}

LYPineappleSeat::~LYPineappleSeat() {
	// TODO Auto-generated destructor stub
}

LYHandStrengthPtr LYPineappleSeat::genHandStrength(unsigned int line,
		LYHoldemAlgorithmDelegate *had)
{
	std::vector<LYCard> cards;
	switch (line) {
	case LYTOPLINE:
	{
		cards = topBricks;
		LYHandStrength *hs = new LYHandStrength(cards, had);
		this->handStrengthOfTop = LYHandStrengthPtr(hs);
		this->rankingOfTop = this->handStrengthOfTop->ranking;
		return this->handStrengthOfTop;
	}
		break;
	case LYMIDDLELINE:
	{
		cards = middleBricks;
		LYHandStrength *hs = new LYHandStrength(cards, had);
		this->handStrengthOfMiddle = LYHandStrengthPtr(hs);
		this->rankingOfMiddle = this->handStrengthOfMiddle->ranking;
		return this->handStrengthOfMiddle;
	}
		break;
	case LYBOTTOMLINE:
	{
		cards = bottomBricks;
		LYHandStrength *hs = new LYHandStrength(cards, had);
		this->handStrengthOfBottom = LYHandStrengthPtr(hs);
		this->rankingOfBottom = this->handStrengthOfBottom->ranking;
		return this->handStrengthOfBottom;
	}
		break;
	default:
		cards = topBricks;
		break;
	}
	//added  20130718
	return LYHandStrengthPtr();
}

LYRanking LYPineappleSeat::getRanking(unsigned int line)
{
	switch (line) {
	case LYTOPLINE:
	{
		if (NULL == handStrengthOfTop.get()) {
			return HighCard;
		}
		return handStrengthOfTop->ranking;
	}
		break;
	case LYMIDDLELINE:
	{
		if (NULL == handStrengthOfMiddle.get()) {
			return HighCard;
		}
		return handStrengthOfMiddle->ranking;
	}
		break;
	case LYBOTTOMLINE:
	{
		if (NULL == handStrengthOfBottom.get()) {
			return HighCard;
		}
		return handStrengthOfBottom->ranking;
	}
		break;
	}
	return HighCard;
}

void LYPineappleSeat::reset()
{
	LYSeat::reset();
	score  = 0;
	score_race = 0;
	resetForNewGame();
	gameStatus = LYNORMAL;
}

void LYPineappleSeat::resetForNewGame()
{
	if (gameStatus != LYFANTASY) {
		gameStatus = LYNORMAL;
	}
	voteNext = false;
	topBricks.clear();
	middleBricks.clear();
	bottomBricks.clear();
	cardsOnHand.clear();
	rankingOfTop = HighCard;
	handStrengthOfTop = LYHandStrengthPtr();
	rankingOfMiddle = HighCard;
	handStrengthOfMiddle = LYHandStrengthPtr();
	rankingOfBottom = HighCard;
	handStrengthOfBottom = LYHandStrengthPtr();
}

std::string LYPineappleSeat::toString()
{
    std::string str;
    std::ostringstream ios;
    ios << "seatNo:" << seatNo << std::endl;
    ios << "topLine:" << topLineStr() << std::endl;
    ios << "middleLine:" << middleLineStr() << std::endl;
    ios << "bottomLine:" << bottomLineStr() << std::endl;
    str = ios.str();
    return str;
}

std::string LYPineappleSeat::allBricksStr()
{
    std::string str = topLineStr() + middleLineStr() + bottomLineStr() + cardsOnHandStr();

    return str;
}

std::string LYPineappleSeat::topLineStr()
{
    std::string str;
    std::ostringstream ios;
    for (unsigned int i = 0; i < topBricks.size(); i++) {
        ios << i+1 << ":" << topBricks[i].toInteger() << ",";
    }
    for (unsigned int i = topBricks.size(); i < 3; i++) {
        ios << i+1 << ":" << flipCard.toInteger() << ",";
    }
    str = ios.str();
    return str;
}

std::string LYPineappleSeat::cardsOnHandStr()
{
    std::string str;
    for (unsigned int i = 0; i < cardsOnHand.size(); i++) {
        str = str + "0:" + boost::lexical_cast<std::string>(cardsOnHand[i].toInteger()) + ",";
    }
    return str;
}

std::string LYPineappleSeat::middleLineStr()
{
    std::string str;
    std::ostringstream ios;
    for (unsigned int i = 0; i < middleBricks.size(); i++) {
        ios << i+4 << ":" <<  middleBricks[i].toInteger() << ",";
    }
    for (unsigned int i = middleBricks.size(); i < 5; i++) {
        ios << i+4 << ":" << flipCard.toInteger() << ",";
    }
    str = ios.str();
    return str;
}

std::string LYPineappleSeat::bottomLineStr()
{
    std::string str;
    std::ostringstream ios;
    for (unsigned int i = 0; i < bottomBricks.size(); i++) {
        ios << i+9 << ":" << bottomBricks[i].toInteger() << ",";
    }
    for (unsigned int i = bottomBricks.size(); i < 5; i++) {
        ios << i+9 << ":" << flipCard.toInteger() << ",";
    }
    str = ios.str();
    return str;
}

void LYPineappleSeat::prepareForRace(LYHoldemAlgorithmDelegate* had)
{
	if (this->bottomBricks.size() != 5 || this->topBricks.size() != 3 || this->middleBricks.size() != 5) {
		return;
	}

	LYHoldemAlgorithmDelegate* _had = had;
	if (had == NULL) {
		_had = new LYHoldemAlgorithmDelegate();
	}
	LYHandStrengthPtr bottomLine = this->genHandStrengthOfBottom(_had);
//	std::cout << "bottom here!" << bottomLine->ranking << std::endl;
	LYHandStrengthPtr middleLine = this->genHandStrengthOfMiddle(_had, bottomLine.get());
	LYHandStrengthPtr topLine = this->genHandStrengthOfTop(_had, middleLine.get());
	if (had == NULL) {
		delete _had;
	}

	cardsOnHand.clear();

	if (gameStatus == LYABORT) {
		gameStatus = LYFOUL;
		return;
	}

	if (*(topLine.get()) > *(middleLine.get()) || *(middleLine.get()) > *(bottomLine.get())) {
		gameStatus = LYFOUL;
		return;
	}

	if (gameStatus == LYFANTASY) {
		if (topLine->ranking == ThreeOfAKind ||
				bottomLine->ranking == FourOfAKind || bottomLine->ranking == StraightFlush) {
			gameStatus = LYFANTASY;
		} else {
			gameStatus = LYNORMAL;
		}
		return;
	}
	if (topLine->ranking == ThreeOfAKind ||
			(topLine->ranking == OnePair && topLine->topRankCard.face > JACK)) {
		gameStatus = LYFANTASY;
		return;
	}
	gameStatus = LYNORMAL;
}

LYHandStrengthPtr LYPineappleSeat::genHandStrengthOfTop(
		LYHoldemAlgorithmDelegate *had, LYHandStrength* capHs)
{
	if (topBricks.size() != 3) {
		return LYHandStrengthPtr();
	}

	LYHandStrength *hs = new LYHandStrength(topBricks, had, capHs);
	this->handStrengthOfTop = LYHandStrengthPtr(hs);
	this->rankingOfTop = this->handStrengthOfTop->ranking;
	return this->handStrengthOfTop;
}

LYHandStrengthPtr LYPineappleSeat::genHandStrengthOfMiddle(
		LYHoldemAlgorithmDelegate *had, LYHandStrength* capHs)
{
	if (middleBricks.size() != 5) {
		return LYHandStrengthPtr();
	}
//	if (capHs) {
//		std::cout << "bottom:" << capHs->ranking << ",cards:" << capHs->cardString << std::endl;
//	}
	LYHandStrength *hs = new LYHandStrength(middleBricks, had, capHs);
	this->handStrengthOfMiddle = LYHandStrengthPtr(hs);
//	std::cout << "handStrengthOfMiddle:" << this->handStrengthOfMiddle->ranking << std::endl;
	this->rankingOfMiddle = this->handStrengthOfMiddle->ranking;
	return this->handStrengthOfMiddle;
}

LYHandStrengthPtr LYPineappleSeat::genHandStrengthOfBottom(
		LYHoldemAlgorithmDelegate *had, LYHandStrength* capHs)
{
	if (bottomBricks.size() != 5) {
		return LYHandStrengthPtr();
	}

	LYHandStrength *hs = new LYHandStrength(bottomBricks, had);
	this->handStrengthOfBottom = LYHandStrengthPtr(hs);
	this->rankingOfBottom = this->handStrengthOfBottom->ranking;
	return this->handStrengthOfBottom;
}

int LYPineappleSeat::raceAgainst(LYPineappleSeat& player, LYPineappleRace& race)
{
	if (gameStatus == LYFOUL) {
		if (player.gameStatus == LYFOUL) {
			return 0;
		}
	}
	int winningLine = 0;
	race.scoreAgainstTop = (int)bonusOfTop() - (int)player.bonusOfTop();
	if (gameStatus == LYFOUL ||
			(player.gameStatus != LYFOUL && *(handStrengthOfTop.get()) < *(player.handStrengthOfTop.get()))) {
		race.scoreAgainstTop -= 1;
		winningLine --;
	} else if (player.gameStatus == LYFOUL ||
			(gameStatus != LYFOUL && *(handStrengthOfTop.get()) > *(player.handStrengthOfTop.get()))) {
		race.scoreAgainstTop += 1;
		winningLine ++;
	}
	race.scoreAgainstMiddle = (int)bonusOfMiddle() - (int)player.bonusOfMiddle();
	if (gameStatus == LYFOUL ||
			(player.gameStatus != LYFOUL && *(handStrengthOfMiddle.get()) < *(player.handStrengthOfMiddle.get()))) {
		race.scoreAgainstMiddle -= 1;
		winningLine --;
	} else if (player.gameStatus == LYFOUL ||
			(gameStatus != LYFOUL && *(handStrengthOfMiddle.get()) > *(player.handStrengthOfMiddle.get()))) {
		race.scoreAgainstMiddle += 1;
		winningLine ++;
	}
	race.scoreAgainstBottom = (int)bonusOfBottom() - (int)player.bonusOfBottom();
	if (gameStatus == LYFOUL ||
			(player.gameStatus != LYFOUL && *(handStrengthOfBottom.get()) < *(player.handStrengthOfBottom.get()))) {
		race.scoreAgainstBottom -= 1;
		winningLine --;
	} else if (player.gameStatus == LYFOUL ||
			(gameStatus != LYFOUL && *(handStrengthOfBottom.get()) > *(player.handStrengthOfBottom.get()))) {
		race.scoreAgainstBottom += 1;
		winningLine ++;
	}
	race.statusAgainst = LYEVEN;
	race.scoreAgainst = race.scoreAgainstTop + race.scoreAgainstMiddle + race.scoreAgainstBottom;
	if (winningLine == 3) {
		race.statusAgainst = LYBEATALL;
		race.scoreAgainst += 3;
	} else if (winningLine == -3) {
		race.statusAgainst = LYSCOOPED;
		race.scoreAgainst -= 3;
	}

	return race.scoreAgainst;
}

int LYPineappleSeat::pinup(LYPineappleBrick &pb) //0: OK, -1: 如果之前该Brick有牌则失败
{
	if (pb.brick < 1 || pb.brick > 13) {
		return -1;
	}

	if (pb.brick < 4 && pb.brick > 0) {
		if (pb.brick != (topBricks.size() + 1)) {
			return -1;
		}
		topBricks.push_back(pb.card);
		return 0;
	}
	if (pb.brick < 9 && pb.brick > 3) {
		if (pb.brick != (middleBricks.size() + 4)) {
			return -1;
		}
		middleBricks.push_back(pb.card);
		return 0;
	}
	if (pb.brick < 14 && pb.brick > 8) {
		if (pb.brick != (bottomBricks.size() + 9)) {
			return -1;
		}
		bottomBricks.push_back(pb.card);
		return 0;
	}
	return 0;
}

int LYPineappleSeat::pinup(std::vector<LYPineappleBrick> &pb)
{
	std::vector<LYPineappleBrick>::iterator it = pb.begin();
	for (; it != pb.end(); it++) {
		int r = pinup(*it);
		if (r == -1) return -1;
	}
	cardsOnHand.clear();
	return 0;
}

unsigned int LYPineappleSeat::bonusOfTop()
{
	if (gameStatus == LYFOUL) return 0;
	LYRanking rank = getRankingOfTop();
	switch (rank) {
	case OnePair:
		if (handStrengthOfTop->topRankCard.face < 6) {
			return 0;
		}
		return handStrengthOfTop->topRankCard.face - 5;
		break;
	case ThreeOfAKind:
		return 8+handStrengthOfTop->topRankCard.face;
		break;
	default:
		return 0;
		break;
	}
	return 0;
}

unsigned int LYPineappleSeat::bonusOfMiddle()
{
	if (gameStatus == LYFOUL) return 0;
	LYRanking rank = getRankingOfMiddle();
	switch (rank) {
	case ThreeOfAKind:
		return 2;
		break;
	case Flush:
		return 8;
		break;
	case Straight:
		return 4;
		break;
	case FullHouse:
		return 12;
		break;
	case FourOfAKind:
		return 20;
		break;
	case StraightFlush:
		if (handStrengthOfMiddle->topRankCard.face == ACE) {
			return 50;
		}
		return 30;
		break;
	default:
		return 0;
		break;
	}
	return 0;
}

unsigned int LYPineappleSeat::bonusOfBottom()
{
	if (gameStatus == LYFOUL) return 0;
	LYRanking rank = getRankingOfBottom();
	switch (rank) {
	case Flush:
		return 4;
		break;
	case Straight:
		return 2;
		break;
	case FullHouse:
		return 6;
		break;
	case FourOfAKind:
		return 10;
		break;
	case StraightFlush:
		if (handStrengthOfBottom->topRankCard.face == ACE) {
			return 25;
		}
		return 15;
		break;
	default:
		return 0;
		break;
	}
	return 0;
}

int LYPineappleSeat::issueCards(std::vector<LYPineappleBrick> &bricks)
{
	this->cardsOnHand.clear();
	std::vector<LYPineappleBrick>::iterator it = bricks.begin();
	for (; it!=bricks.end(); it++) {
		this->cardsOnHand.push_back((*it).card);
	}
	return 0;
}

bool LYPineappleSeat::finishGame()
{
	if (this->status == LYSeatOpen) return true;
	if (topBricks.size() == 3 &&
			middleBricks.size() == 5 &&
			bottomBricks.size() == 5) {
		return true;
	}
	return false;
}

bool LYPineappleSeat::isFantasy()
{
	if (gameStatus == LYFANTASY) return true;
	return false;
}

bool LYPineappleSeat::isInGame()
{
	if (status != LYSeatInGame) {
		return false;
	}
	switch (gameStatus) {
	case LYABORT:
		return false;
	default:
		return true;
	}
}

void LYPineappleSeat::abortGame()
{
	gameStatus = LYABORT;
}

void LYPineappleSeat::occupy(unsigned int buyin, const std::string &uid)
{
	LYSeat::occupy(buyin, uid);
	gameStatus = LYNORMAL;
}
