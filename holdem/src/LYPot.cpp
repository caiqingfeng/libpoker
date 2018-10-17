/*
 * LYPot.cpp
 *
 *  Created on: 2013-3-25
 *      Author: caiqingfeng
 */

#include <iostream>
#include <sstream>
//#include <boost/foreach.hpp>
#include "LYPot.h"
#include <boost/algorithm/string.hpp>
#include "holdem/src/LYHoldemGame.h"
#include "common/src/my_log.h"

const unsigned int ODDSSIZE = 23;
const float ODDS[ODDSSIZE] = {0, 30/*1*/, 15/*2*/, 10/*3*/, 7.5/*4*/, 6/*5*/, 5/*6*/,
					4/*7*/, 3.5/*8*/, 3/*9*/, 2.6/*10*/, 2.4/*11*/, 2.2/*12*/, 2.0/*13*/, 1.8/*14*/,
					1.6/*15*/, 1.4/*16*/, 1.2/*17*/, 1.0/*18*/, 0.8/*19*/, 0.6/*20*/, 0.4/*21*/, 0.2/*22*/};

LYAssurance::LYAssurance()
{
	round = LYGameTurn;
	seat = 0;
	againstOuts.clear();
	odds = 0.0;
	maxchips = 0;
	assurance = 0;
	expected = 0;
	result = 0;
}

LYAssurance::LYAssurance(LYGameRound rd, unsigned int st, unsigned int potsize,
		unsigned int assure, const std::vector<LYCard>& outs)
{
	round = rd;
	seat = st;
	againstOuts = outs;
	maxchips = potsize;
	assurance = assure;

	expected = ODDS[outs.size()] * assure;
	result = 0;
}

LYAssurance::~LYAssurance()
{
}

bool LYAssurance::doesHitAssurance(LYGameRound rd, unsigned int st, const LYCard &card)
{
	return (round == rd && seat == st && LYCardHelpers::hasThisCard(againstOuts, card));
}

void LYAssurance::makeResult(LYGameRound rd, unsigned int st, const LYCard &card)
{
	if (!doesHitAssurance(rd, st, card)) return;

	result = expected;
}

std::string LYAssurance::toString()
{
	std::string outs;
	LYCardHelpers::cardsToString(againstOuts, outs);
	std::string str = std::string("round:") + std::to_string(round) + "\n" +
						std::string("seat:") + std::to_string(seat) + "\n" +
						std::string("maxchips:") + std::to_string(maxchips) + "\n" +
						std::string("assurance:") + std::to_string(assurance) + "\n" +
						std::string("expected:") + std::to_string(expected) + "\n" +
						std::string("result:") + std::to_string(result) + "\n" +
						std::string("outs:") + outs;
	return str;
}

LYPot::LYPot() {
	// TODO Auto-generated constructor stub
	potLevel = LYMainPot;
	potOpen = true;
	chipsInPot = 0;
	unitPrice = 0;
	priceFixed = false;
}

LYPot::LYPot(unsigned int chips) {
	// TODO Auto-generated constructor stub
	potLevel = LYMainPot;
	potOpen = true;
	chipsInPot = chips;
	unitPrice = 0;
	priceFixed = false;
}

LYPot::~LYPot() {
	// TODO Auto-generated destructor stub
}

//added 20160311, 序列化时需要
LYPot::LYPot(std::vector<std::pair<std::string, std::string>>& kvps)
{
	std::vector<std::pair<std::string, std::string>>::iterator it = kvps.begin();
	for (; it!=kvps.end(); it++) {
		std::pair<std::string, std::string> kvp = *it;
		if (kvp.first == "potLevel") {
			potLevel = (LYPotLevel)(std::stoi(kvp.second));
		} else if (kvp.first == "potOpen") {
			potOpen = (bool)(std::stoi(kvp.second));
		} else if (kvp.first == "chipsInPot") {
			chipsInPot = (std::stoi(kvp.second));
		} else if (kvp.first == "unitPrice") {
			unitPrice = (std::stoi(kvp.second));
		} else if (kvp.first == "priceFixed") {
			priceFixed = (bool)(std::stoi(kvp.second));
		} else if (kvp.first == "seatsInPot") {
			std::vector<std::string> seats;
			boost::split(seats, kvp.second, boost::is_any_of(","));
			std::vector<std::string>::iterator sit = seats.begin();
			for (; sit != seats.end(); sit++) {
				std::string s = *sit;
				boost::trim_if(s, boost::is_any_of(", "));
				if (s.size() > 0)
					seatsInPot.push_back((LYApplicant) (std::stoi(s)));
			}
		} else if (kvp.first == "winnersInPot") {
			std::vector<std::string> seats;
			std::string ss = kvp.second;
			boost::split(seats, ss, boost::is_any_of(","));
			std::vector<std::string>::iterator sit = seats.begin();
			for (; sit != seats.end(); sit++) {
				std::string s = *sit;
				boost::trim_if(s, boost::is_any_of(",\n\r\t"));
				if (s.size() > 0)
					winnersInPot.push_back((LYApplicant) (std::stoi(s)));
			}
		}
	}
}

void LYPot::setPot(LYPot *pot)
{
	this->potLevel = pot->potLevel;
	this->potOpen = pot->potOpen;
	this->chipsInPot = pot->chipsInPot;
	this->unitPrice = pot->unitPrice;
	this->priceFixed = pot->priceFixed;
}

void LYPot::addSeat(LYSeatPtr st)
{
    seatsInPot.push_back(st->seatNo);
}

void LYPot::addSeat(LYApplicant seat)
{
    seatsInPot.push_back(seat);
}

void LYPot::fold(LYApplicant seat)
{
	std::vector<LYApplicant>::iterator it = seatsInPot.begin();
	for (; it!=seatsInPot.end(); it++) {
		if (seat == *it) {
			seatsInPot.erase(it);
			return;
		}
	}
}

void LYPot::reset()
{
	potLevel = LYMainPot;
	potOpen = true;
	chipsInPot = 0;
	unitPrice = 0;
	priceFixed = false;
	seatsInPot.clear();
	winnersInPot.clear();
}

bool LYPot::haveSeat(enum LYApplicant player)
{
	std::vector<LYApplicant>::iterator it = seatsInPot.begin();
	for (; it!=seatsInPot.end(); it++) {
		LYApplicant seat = *it;
		if (seat == player) {
			return true;
		}
	}
	return false;
}

bool LYPot::isWinner(enum LYApplicant player)
{
	std::vector<LYApplicant>::iterator it = winnersInPot.begin();
	for (; it!=winnersInPot.end(); it++) {
		LYApplicant seat = *it;
		if (seat == player) {
			return true;
		}
	}
	return false;
}

std::string LYPot::toString()
{
    std::string str;
    std::ostringstream ios;
    ios << "potLevel:" << potLevel << std::endl;
    ios << "potOpen:" << potOpen << std::endl;
    ios << "chipsInPot:" << chipsInPot << std::endl;
    ios << "unitPrice:" << unitPrice << std::endl;
    ios << "priceFixed:" << priceFixed << std::endl;
    ios << "involved seats:" << std::endl;
	std::vector<LYApplicant>::iterator it = seatsInPot.begin();
	for(; it != seatsInPot.end(); it++) {
		LYApplicant seat = *it;
        ios << "seat:" << seat << std::endl;
    }
    ios << "winner seats:" << std::endl;
	it = winnersInPot.begin();
	for(; it != winnersInPot.end(); it++) {
		LYApplicant seat = *it;
        ios << "seat:" << seat << std::endl;
    }
    str = ios.str();
    return str;
}

std::vector<std::pair<std::string, std::string> > LYPot::pot2kvps()
{
	std::vector<std::pair<std::string, std::string> > kvps;
	std::pair<std::string, std::string> kvp;

	kvp.first = "potLevel";
	kvp.second = std::to_string(this->potLevel);
	//	LY_LOG_DBG("name:"+table->name);
	kvps.push_back(kvp);

	kvp.first = "potOpen";
	kvp.second = std::to_string(this->potOpen);
	//	LY_LOG_DBG("name:"+table->name);
	kvps.push_back(kvp);

	kvp.first = "chipsInPot";
	kvp.second = std::to_string(this->chipsInPot);
	//	LY_LOG_DBG("name:"+table->name);
	kvps.push_back(kvp);

	kvp.first = "unitPrice";
	kvp.second = std::to_string(this->unitPrice);
	//	LY_LOG_DBG("name:"+table->name);
	kvps.push_back(kvp);

	kvp.first = "priceFixed";
	kvp.second = std::to_string(this->priceFixed);
	//	LY_LOG_DBG("name:"+table->name);
	kvps.push_back(kvp);

	kvp.first = "seatsInPot";
	kvp.second = this->getSeatsString();
	//	LY_LOG_DBG("name:"+table->name);
	if (kvp.second.size() > 0)
		kvps.push_back(kvp);

	kvp.first = "winnersInPot";
	kvp.second = this->getWinnersString();
	//	LY_LOG_DBG("name:"+table->name);
	if (kvp.second.size() > 0)
		kvps.push_back(kvp);

	return kvps;
}

std::string LYPot::getSeatsString()
{
	std::string s;
	std::vector<LYApplicant>::iterator it = seatsInPot.begin();
	for (; it != seatsInPot.end(); it++) {
		s += std::to_string(*it) + ",";
	}
	return s;
}

std::string LYPot::getWinnersString()
{
	std::string s;
	std::vector<LYApplicant>::iterator it = winnersInPot.begin();
	for (; it != winnersInPot.end(); it++) {
		s += std::to_string(*it) + ",";
	}
	return s;
}

bool LYPot::validAssurance(unsigned int assurance)
{
	if (chasingOuts.size() > ODDSSIZE) return false;

	float willPay = ODDS[chasingOuts.size()] * assurance;
	return (willPay <= (1.0 * chipsInPot));
}

/*
 *
 */
void LYPot::buyAssurance(LYGameRound round, LYSeatPtr st, unsigned int assurance)
{
	if (!isWinner(st->seatNo) || !validAssurance(assurance) || winnersInPot.size() == 0) {
		return;
	}
	LYAssurance *assure = new LYAssurance(round, (unsigned int)st->seatNo, chipsInPot/winnersInPot.size(), assurance, chasingOuts);
	LYAssurancePtr p(assure);
	assurances.push_back(p);
}

/*
 * 比较复杂的一个方法
 * 根据Outs来计算保险，首先要得到所有allin的牌
 * 在winners里找出Winner的handstrength，然后在落后的seats中，依次计算Outs，最后把outs加起来
 */
void LYPot::prepareMatchResult(LYHoldemGame *game)
{
	this->winnersInPot.clear();
	std::vector<LYApplicant>::iterator it = this->seatsInPot.begin();
	//用一个类似于冒泡排序的算法，计算最强Hand
	for (; it!=this->seatsInPot.end(); it++) {
	    LYApplicant sn = *it;
//std::cout << "seat:" << std::to_string(sn) << std::endl;
		LYSeatPtr st = game->fetchSeat(sn);
		LYHoldemSeat *seat = (LYHoldemSeat *)st.get();
		if (seat->status == LYSeatInGame && seat->gameStatus != LYSeatHoldemInFold) {
			LYHandStrengthPtr hs = seat->genHandStrength(game->getCommunityCards(), (LYHoldemAlgorithmDelegate *)game->pad);
			if (this->winnersInPot.size() == 0) {
				this->winnersInPot.push_back(sn);
				winningHand = hs.get();
				continue;
			}
			if (NULL == hs.get()) {
//					LY_LOG_ERR("error happend, should have only one seat in game here!");
				return;
			}
			enum LYApplicant win = this->winnersInPot[0];
			LYSeatPtr winSeat = game->fetchSeat(win);
			LYHoldemSeat *prev_seat_in_winner = (LYHoldemSeat *)winSeat.get();
//std::cout << "previous winner handStrength with cards: " << prev_seat_in_winner->handStrength->cardString << std::endl;
			if (*hs == *(prev_seat_in_winner->handStrength)) {
//std::cout << "handStrength with cards: " << hs->cardString << std::endl;
				this->winnersInPot.push_back(sn);
			} else if (*hs < *(prev_seat_in_winner->handStrength)) {
//std::cout << "<handStrength " << hs->ranking << " str=" << hs->rankingString << " with cards: " << hs->cardString << std::endl;
						//nothing happend
			} else if (*hs > *(prev_seat_in_winner->handStrength)) {
//std::cout << ">handStrength "  << hs->ranking << " str=" << hs->rankingString << " with cards: " << hs->cardString << std::endl;
				this->winnersInPot.clear();
				this->winnersInPot.push_back(sn);
				winningHand = hs.get();
			}
		}
	}
}

/*
 * 被GameOver调用
 */
void LYPot::caculateMatchResult(LYHoldemGame *game)
{
	prepareMatchResult(game);
	//首先把赢家的筹码加上
	unsigned int winners = this->winnersInPot.size();
//std::cout << "winners in pot:" << winners <<  std::endl;
	unsigned int chipsToWin = 0;
	if (winners == 0) { // it happens only when all seats quit
//			LY_LOG_ERR("fatal, winners should be greater than 0" << winners);
	} else {
		chipsToWin = this->chipsInPot/winners;
	}
	std::vector<LYApplicant>::iterator it = this->winnersInPot.begin();
	for (; it!=this->winnersInPot.end(); it++) {
    	LYApplicant sn = *it;
		LYSeatPtr seat = game->fetchSeat(sn);
		seat->chipsAtHand += chipsToWin;
//	std::cout << "seat:" << sn << "winning:" << seat->chipsAtHand << std::endl;
	}

	//其次将保险的胜负计算出来
	std::vector<LYAssurancePtr>::iterator ait = assurances.begin();
	for (; ait != assurances.end(); ait++) {
		LYAssurancePtr assure = *ait;
		game->caculateAssuranceResult(assure.get());
	}
}

void LYPot::caculateOuts(LYHoldemGame *game)
{
	chasingOuts.clear();
 	std::vector<LYCard> allVisibleCards = game->getAllVisibleCards();
 	std::vector<LYCard> communityCards = game->getCommunityCards();

	if (winningHand == NULL) return;
//	std::cout << "dddd: winning=" << winningHand->cardString << std::endl;
	std::vector<LYApplicant>::iterator it = this->seatsInPot.begin();
 	for (; it!=seatsInPot.end(); it++) {
 		if (isWinner(*it)) continue;
 		LYSeatPtr st = game->fetchSeat(*it);
 		LYHoldemSeat *seat = (LYHoldemSeat *)st.get();
 		std::vector<LYCard> outs = seat->caculateOuts(winningHand, communityCards, allVisibleCards);
 		chasingOuts.insert(chasingOuts.end(), outs.begin(), outs.end());
 	}
}

/*
 * 注意：此处调用方的round比这个函数要推后一，即在turn的时候 check/find flop assurance
 */
LYAssurancePtr LYPot::findAssurance(LYGameRound rd, LYApplicant st, const LYCard &card)
{
	std::vector<LYAssurancePtr>::iterator it = assurances.begin();
	for (; it!=assurances.end(); it++) {
		LYAssurancePtr assure = *it;
		if (assure->doesHitAssurance(rd, st, card)) return assure;
	}

	return LYAssurancePtr();
}

/*
 * 注意：此处调用方的round比这个函数要推后一，即在turn的时候 check/find flop assurance
 */
void LYPot::checkAssurance(LYGameRound rd, const LYCard &card)
{
//LY_LOG_DBG("enter checkAssurance:" );
	if (!LYCardHelpers::hasThisCard(chasingOuts, card)) {
		return;
	}

	std::vector<LYApplicant>::iterator it = winnersInPot.begin();
	for (; it!=winnersInPot.end(); it++) {
		LYApplicant st = *it;
		LYAssurancePtr assure = findAssurance(rd, st, card);
		if (assure.get() == NULL) return;
		assure->makeResult(rd, st, card);
	}
}

void LYPot::makeAskAssuranceActions(LYHoldemGame *game, std::vector<LYHoldemAction>& actions)
{
	assureActions.clear();
	this->prepareMatchResult(game);
	if (this->winnersInPot.size() == 0) return;
	this->caculateOuts(game);
	if (this->chasingOuts.size() == 0) return; //drawing dead at this moment

	unsigned int chips = this->chipsInPot / this->winnersInPot.size();
	std::vector<LYApplicant>::iterator sit = this->winnersInPot.begin();
	for (; sit!=this->winnersInPot.end(); sit++) {
		LYHoldemAction action(game->table->_id);
		action.askAssure(*sit, chips, this->chasingOuts, std::to_string(this->potLevel));
		assureActions.push_back(action);
		actions.push_back(action);
	}
}

/*
 * 对上一轮的保险进行Pay
 * 输入为该Card
 */
void LYPot::makePaidAssuranceActions(LYHoldemGame *game, const LYCard& card, std::vector<LYHoldemAction>& actions)
{
LY_LOG_DBG("enter makePaidAssuranceActions:" );
	if (card == flipCard) return;
	LYGameRound round = game->round;
	switch (game->round)
	{
	case LYGameTurn:
		round = LYGameFlop;
		break;
	case LYGameRiver:
		round = LYGameTurn;
		break;
	default:
		break;
	}
//	std::cout << "round=" << round << std::endl;
    this->checkAssurance(round, card);
    std::vector<LYAssurancePtr>::iterator it = assurances.begin();
    for (; it!=assurances.end(); it++) {
    	LYAssurancePtr assure = *it;
//	std::cout << "assure round=" << assure->round << std::endl;
    	if (assure->round != round) continue;
		LYHoldemAction action(game->table->_id);
		action.payAssure((LYApplicant)assure->seat, assure->result, std::to_string(this->potLevel));
    	actions.push_back(action);
    }
}

LYHoldemAction LYPot::popAssureAction()
{
	LYHoldemAction action;
	if (assureActions.size() > 0) {
		action = assureActions.back();
		assureActions.pop_back();
	}
	if (assureActions.size() == 0) {
//		chasingOuts.clear(); //在buy assurance时需要这个size
	}
	return action;
}
