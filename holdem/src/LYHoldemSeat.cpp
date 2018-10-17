/*
 * LYHoldemSeat.cpp
 *
 *  Created on: 2013-7-5
 *      Author: caiqingfeng
 */

#include <sstream>
#include "LYHoldemSeat.h"
//#include "common/src/my_log.h"
#include "poker/src/LYCardHelpers.h"
#include <iostream>

LYHoldemSeat::LYHoldemSeat(LYApplicant seat_no) : LYSeat(seat_no)
{
	handCards.push_back(flipCard);
	handCards.push_back(flipCard);
}

LYHoldemSeat::LYHoldemSeat(LYHoldemSeatDynamics &seat_dyn) : LYSeat(seat_dyn.seatNo)
{
	handCards.push_back(flipCard);
	handCards.push_back(flipCard);
	gameStatus = seat_dyn.gameStatus;
	chipsAtHand = seat_dyn.chipsAtHand;
	status = seat_dyn.status;
	lastBet = seat_dyn.lastBet;
	timeoutCount = seat_dyn.timeoutCount;
}

LYHoldemSeat::LYHoldemSeat(std::vector < std::pair<std::string, std::string> >& kvps) :
	LYSeat(kvps)
{
	std::vector < std::pair<std::string, std::string> >::iterator it = kvps.begin();
	for (; it!=kvps.end(); it++) {
		std::pair<std::string, std::string> kvp = *it;
		if (kvp.first == "gameStatus") {
			this->gameStatus = (LYSeatHoldemStatus)std::stoi(kvp.second);
		} else if (kvp.first == "ranking") {
			this->ranking = (LYRanking)std::stoi(kvp.second);
		}
	}
}

LYHoldemSeat::~LYHoldemSeat() {
	// TODO Auto-generated destructor stub
}

bool LYHoldemSeat::isInGame() {
	if (status != LYSeatInGame) {
		return false;
	}
	switch (gameStatus) {
	case LYSeatHoldemWaiting:
	case LYSeatHoldemBetSmall:
	case LYSeatHoldemBetBig:
	case LYSeatHoldemInCalled:
	case LYSeatHoldemInAllin:
	case LYSeatHoldemInChecked:
	case LYSeatHoldemInRaised:
	case LYSeatHoldemInBet:
	case LYSeatHoldemMakingDecision:
		return true;
	case LYSeatHoldemInFold:
	default:
		return false;
	}
}

const LYCard& LYHoldemSeat::firstCard()
{
	if (handCards.size()<1) return flipCard;
	return handCards[0];
}
const LYCard& LYHoldemSeat::secondCard()
{
	if (handCards.size()<2) return flipCard;
	return handCards[1];
}

void LYHoldemSeat::setFirst(const LYCard &cd)
{
//	handCards[0] = cd;
	handCards.clear();
	handCards.push_back(cd);
}

void LYHoldemSeat::setSecond(const LYCard &cd)
{
//	handCards[1] = cd;
	handCards.push_back(cd);
}

LYHandStrengthPtr LYHoldemSeat::genHandStrength(const std::vector<LYCard> &communityCards,
		LYHoldemAlgorithmDelegate *had)
{
//	LY_LOG_DBG("community cards "<< communityCards.size());
	if (communityCards.size() < 3) {
		return LYHandStrengthPtr();
	}
	std::vector<LYCard> cards = communityCards;
	cards.push_back(firstCard());
	cards.push_back(secondCard());
	LYHandStrength *hs = new LYHandStrength(cards, had);
	this->handStrength = LYHandStrengthPtr(hs);
	//added  20130718
	this->ranking = this->handStrength->ranking;
	return this->handStrength;
}

LYRanking LYHoldemSeat::getRanking()
{
	if (NULL == handStrength.get()) {
		return HighCard;
	}
	return handStrength->ranking;
}

void LYHoldemSeat::setFirstCard(const LYCard &card)
{
//	std::vector<LYCard>::iterator it = handCards.begin();
//	*it = card;
	setFirst(card);
}

void LYHoldemSeat::setSecondCard(const LYCard &card)
{
//	std::vector<LYCard>::iterator it = handCards.begin();
//	it++;
//	*it = card;
	setSecond(card);
}

void LYHoldemSeat::genHoldemDynamics(LYHoldemSeatDynamics &sd)
{
	genDynamics(sd);
	sd.gameStatus = this->gameStatus;
}

void LYHoldemSeat::reset()
{
	LYSeat::reset();
	ranking = HighCard;
	handStrength = LYHandStrengthPtr();
	gameStatus = LYSeatHoldemWaiting;
}

void LYHoldemSeat::resetForNewGame()
{
//std::cout << "resetting seat:" << std::to_string(seatNo) << ", status=" << std::to_string(status)
// 		<< ", gameStatus=" << std::to_string(gameStatus) << ", newBuyin=" << newBuyin << std::endl;
	if (status == LYSeatOpen) {
		gameStatus = LYSeatHoldemWaiting;
		return;
	}
//	status = LYSeatOccupied;
	status = LYSeatInGame;
	gameStatus = LYSeatHoldemWaiting;
	lastBet = 0;
	chipsAtHand += newBuyin;
	if (chipsAtHand == 0) {
//		status = LYSeatOpen;
	}
	chipsAtBeginning = chipsAtHand;
	ranking = HighCard;
	handStrength = LYHandStrengthPtr();
	setFirstCard(flipCard);
	setSecondCard(flipCard);
	//timeoutCount = 0; //DO not reset timeoutCount here.
	newBuyin = 0;
}

//重载LYSeat::resetForGame
void LYHoldemSeat::resetForGame()
{
	resetForNewGame();
}

void LYHoldemSeat::buyin(unsigned int buyin, const std::string &uid)
{
//std::cout << "enter buyin" << std::endl;
	if (this->playerId != uid) {
		return;
	}
	if (this->isInGame()) {
		this->newBuyin += buyin;
	} else {
		this->chipsAtHand += buyin;
	}
}

std::vector < std::pair<std::string, std::string> > LYHoldemSeat::seat2kvps()
{
	std::vector < std::pair<std::string, std::string> > key_value_pairs = LYSeat::seat2kvps();

	std::pair < std::string, std::string > kvp;
	kvp.first = "ranking";
	kvp.second = std::to_string(this->ranking);
	//	LY_LOG_DBG("name:"+table->name);
	key_value_pairs.push_back(kvp);

	kvp.first = "gameStatus";
	kvp.second = std::to_string(this->gameStatus);
	//	LY_LOG_DBG("name:"+table->name);
	key_value_pairs.push_back(kvp);

	return key_value_pairs;
}

//void LYHoldemSeat::buyAssurance(unsigned int round, unsigned int assurance, const std::string& pot)
//{
//	if (round == 1) {
//		firstAssurance = assurance;
//	} else if (round == 2) {
//		secondAssurance = assurance;
//	}
//}


std::vector<LYCard> LYHoldemSeat::caculateOuts(LYHandStrength *againstHand,
			const std::vector<LYCard>& commnunitCards,
			std::vector<LYCard>& allVisibleCards)
{
	std::vector<LYCard> outs;
	if (commnunitCards.size() >= 5) return outs; //在river时已经无Outs可言了

    LYHoldemAlgorithmDelegate had;
	LYHandStrengthPtr hs =  this->genHandStrength(commnunitCards, &had);
	if (*hs > *againstHand || *hs == *againstHand) {
		return outs;
	}

	std::vector<LYCard> cardsAtBegin = commnunitCards;
	cardsAtBegin.insert(cardsAtBegin.end(), handCards.begin(), handCards.end());
	std::vector<LYCard> restCards = LYCardHelpers::getRestCards(allVisibleCards);
	std::vector<LYCard>::iterator it = restCards.begin();
	for (; it!=restCards.end(); it++) {
		std::vector<LYCard> cards = cardsAtBegin;
		cards.push_back(*it);
		std::vector<LYCard> cardsB = againstHand->cards;
		cardsB.push_back(*it);
		LYHandStrength newhs(cards, &had);
		LYHandStrength newhsB(cardsB, &had);
		if (newhs > newhsB) {
			outs.push_back(*it);
		}
	}
	return outs;
}

std::string LYHoldemSeat::toString()
{
    std::string str;
    str = LYSeat::toString();

    str = str + "," +
    		"\"gameStatus\":" + std::to_string(gameStatus) + "," +
    		"\"ranking\":" + std::to_string(ranking);

    return str;
}

void LYHoldemSeat::fold(const std::string& reason)
{
	if (!reason.compare("byuser")) {
		this->timeoutCount = 0;
	} else if (!reason.compare("timeout")) {
		this->timeoutCount ++;
	}

	chipsAtHand += newBuyin;
	newBuyin = 0;
}


bool LYHoldemSeat::allowLeave()
{
	if (gameStatus == LYSeatHoldemInAllin) return false;
	return true;
}