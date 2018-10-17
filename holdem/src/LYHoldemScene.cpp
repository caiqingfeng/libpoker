/*
 * LYHoldemScene.cpp
 *
 *  Created on: 2013-7-6
 *      Author: caiqingfeng
 */

#include "LYHoldemScene.h"
//#include <boost/foreach.hpp>
#include <iostream>
#include <sstream>

LYHoldemScene::LYHoldemScene(std::vector<LYSeatPtr> &all_seats, std::vector<LYPotPtr> &all_pots,
		enum LYApplicant duty_seat, unsigned int bet_if_call, unsigned int min_bet_if_raise,
		enum LYApplicant bet_by, enum LYGameRound rnd,
		enum LYHoldemRoundContinue cont, unsigned int seq)
{
	seqNo = seq;
	dutySeatNo = duty_seat;
	betIfCall = bet_if_call;
	minBetIfRaise = min_bet_if_raise;
	betBySeatNo = bet_by;
	round = rnd;
	continueThisRound = cont;
	lastPollingAt = 0;
	lastPollingAction.reset();

	std::vector<LYSeatPtr>::iterator it = all_seats.begin();
    for (; it!=all_seats.end(); it++) {
    	LYSeatPtr st = *it;
		LYHoldemSeat *seat = (LYHoldemSeat *)st.get();
		LYHoldemSeatDynamics sd;
		seat->genHoldemDynamics(sd);
		seats.push_back(sd);
	}
	std::vector<LYPotPtr>::iterator it2 = all_pots.begin();
    for (; it2!=all_pots.end(); it2++) {
    	LYPotPtr pot = *it2;
		pots.push_back(*pot);
	}
}

LYHoldemScene::~LYHoldemScene() {
	// TODO Auto-generated destructor stub
}

std::string LYHoldemScene::toString()
{
    std::string str;
    std::ostringstream ios;
    ios << "seqNo:" << seqNo << std::endl;
	std::vector<LYHoldemSeatDynamics>::iterator it = seats.begin();
    for (; it!=seats.end(); it++) {
    	LYHoldemSeatDynamics seat = *it;
        ios << "seat:" << seat.toString() << std::endl;
    }
    ios << "dutySeat:" << dutySeatNo << std::endl;
    ios << "round:" << round << std::endl;
	std::vector<LYPot>::iterator it2 = pots.begin();
    for (; it2!=pots.end(); it2++) {
    	LYPot pot = *it2;
        ios << "pot:" << pot.toString() << std::endl;
    }
    ios << "betIfCall:" << betIfCall << std::endl;
    ios << "minBetIfRaise:" << minBetIfRaise << std::endl;
    ios << "betBy:" << betBySeatNo << std::endl;
    ios << "continueThisRound:" << continueThisRound << std::endl;
    LYHoldemAction action(lastPollingAction);
    ios << "lastPollingAction:" << action.toString() << std::endl;
    ios << "lastPollingAt:" << lastPollingAt << std::endl;
    str = ios.str();
    return str;
}

std::vector<std::pair<std::string, std::string> > LYHoldemStatic::static2kvps()
{
	std::vector<std::pair<std::string, std::string> > kvps;
	std::pair < std::string, std::string > kvp;

	kvp.first = "gameStatus";
	kvp.second = std::to_string(this->gameStatus);
	//	LY_LOG_DBG("name:"+table->name);
	kvps.push_back(kvp);

	kvp.first = "roundOfTournament";
	kvp.second = std::to_string(this->roundOfTournament);
	//	LY_LOG_DBG("name:"+table->name);
	kvps.push_back(kvp);

	kvp.first = "btnSeatNo";
	kvp.second = std::to_string(this->btnSeatNo);
	//	LY_LOG_DBG("name:"+table->name);
	kvps.push_back(kvp);

	kvp.first = "sbSeatNo";
	kvp.second = std::to_string(this->sbSeatNo);
	//	LY_LOG_DBG("name:"+table->name);
	kvps.push_back(kvp);

	kvp.first = "bbSeatNo";
	kvp.second = std::to_string(this->bbSeatNo);
	//	LY_LOG_DBG("name:"+table->name);
	kvps.push_back(kvp);

//	kvp.first = "flopCards";
//	kvp.second = this->getFlopString();
//	kvps.push_back(kvp);
//
//	kvp.first = "turnCard";
//	kvp.second = std::to_string(this->turnCard.toInteger());
//	//	LY_LOG_DBG("name:"+table->name);
//	kvps.push_back(kvp);
//
//	kvp.first = "riverCard";
//	kvp.second = std::to_string(this->riverCard.toInteger());
//	//	LY_LOG_DBG("name:"+table->name);
//	kvps.push_back(kvp);
//
	kvp.first = "smallBlindPrice";
	kvp.second = std::to_string(this->smallBlindPrice);
	//	LY_LOG_DBG("name:"+table->name);
	kvps.push_back(kvp);

	kvp.first = "bigBlindPrice";
	kvp.second = std::to_string(this->bigBlindPrice);
	//	LY_LOG_DBG("name:"+table->name);
	kvps.push_back(kvp);

	return kvps;
}

std::vector<std::pair<std::string, std::string> > LYHoldemDynamics::dynamic2kvps()
{
	std::vector<std::pair<std::string, std::string> > kvps;
	std::pair < std::string, std::string > kvp;

	kvp.first = "seqNo";
	kvp.second = std::to_string(this->seqNo);
	//	LY_LOG_DBG("name:"+table->name);
	kvps.push_back(kvp);

	kvp.first = "round";
	kvp.second = std::to_string(this->round);
	//	LY_LOG_DBG("name:"+table->name);
	kvps.push_back(kvp);

	kvp.first = "dutySeatNo";
	kvp.second = std::to_string(this->dutySeatNo);
	//	LY_LOG_DBG("name:"+table->name);
	kvps.push_back(kvp);

	kvp.first = "betIfCall";
	kvp.second = std::to_string(this->betIfCall);
	//	LY_LOG_DBG("name:"+table->name);
	kvps.push_back(kvp);

	kvp.first = "minBetIfRaise";
	kvp.second = std::to_string(this->minBetIfRaise);
	//	LY_LOG_DBG("name:"+table->name);
	kvps.push_back(kvp);

	kvp.first = "betBySeatNo";
	kvp.second = std::to_string(this->betBySeatNo);
	//	LY_LOG_DBG("name:"+table->name);
	kvps.push_back(kvp);

	kvp.first = "continueThisRound";
	kvp.second = std::to_string(this->continueThisRound);
	//	LY_LOG_DBG("name:"+table->name);
	kvps.push_back(kvp);

	kvp.first = "lastPollingAction";
	kvp.second = this->lastPollingAction.toString();
	//	LY_LOG_DBG("name:"+table->name);
	kvps.push_back(kvp);

	kvp.first = "lastPollingAt";
	kvp.second = std::to_string(this->lastPollingAt);
	//	LY_LOG_DBG("name:"+table->name);
	kvps.push_back(kvp);

	return kvps;
}
