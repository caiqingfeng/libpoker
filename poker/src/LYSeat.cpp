/*
 * LYSeat.cpp
 *
 *  Created on: 2013-3-25
 *      Author: caiqingfeng
 */

#include <sstream>
#include "LYSeat.h"
#include "LYDeck.h"
#include "poker/src/LYCardHelpers.h"
#include <iostream>

LYSeat::LYSeat(enum LYApplicant sn) {
	// TODO Auto-generated constructor stub
	seatNo = sn;
	reset();
}

LYSeat::~LYSeat() {
	// TODO Auto-generated destructor stub
}

LYSeat::LYSeat(std::vector < std::pair<std::string, std::string> >& kvps)
{
	reset();
	std::vector < std::pair<std::string, std::string> >::iterator it = kvps.begin();
	for (; it!=kvps.end(); it++) {
		std::pair<std::string, std::string> kvp = *it;
		if (kvp.first == "seatNo") {
			this->seatNo = (LYApplicant)std::stoi(kvp.second);
		} else if (kvp.first == "chipsAtHand") {
			this->chipsAtHand = std::stoi(kvp.second);
		} else if (kvp.first == "status") {
			this->status = (LYSeatStatus)std::stoi(kvp.second);
		} else if (kvp.first == "lastBet") {
			this->lastBet = std::stoi(kvp.second);
		} else if (kvp.first == "timeoutCount") {
			this->timeoutCount = std::stoi(kvp.second);
		} else if (kvp.first == "playerId") {
			this->playerId = kvp.second;
		} else if (kvp.first == "handCards") {
			LYCardHelpers::stringifiedIntToCards(kvp.second, this->handCards);
		} else if (kvp.first == "chipsAtBeginning") {
			this->chipsAtBeginning = std::stoi(kvp.second);
		} else if (kvp.first == "newBuyin") {
			this->newBuyin = std::stoi(kvp.second);
		}
	}
}

std::string LYSeatDynamics::toString()
{
    std::string str;
    std::ostringstream ios;
    ios << "seatNo:" << seatNo << std::endl;
    ios << "status:" << status << std::endl;
    ios << "chipsAtHand:" << chipsAtHand << std::endl;
    ios << "lastBet:" << lastBet << std::endl;
    str = ios.str();
    return str;
}

std::string LYSeat::toString()
{
    std::string str;
    std::string hands;
    LYCardHelpers::cardsToStringifiedInt(handCards, hands);
    str = "\"seatNo\":" + std::to_string(seatNo) + "," +
    		"\"status\":" + std::to_string(status) + "," +
    		"\"chipsAtHand\":" + std::to_string(chipsAtHand) + "," +
    		"\"newBuyin\":" + std::to_string(newBuyin) + "," +
    		"\"lastBet\":" + std::to_string(lastBet) + "," +
    		"\"playerId\":\"" + playerId + "\"," +
    		"\"handCards\":\"" + hands + "\"," +
    		"\"chipsAtBeginning\":" + std::to_string(chipsAtBeginning) + "," +
    		"\"timeoutCount\":" + std::to_string(timeoutCount);

    return str;
}

/*
 * reset lastBet, chipsAtHand, firstCard, ...
 * not reset status
 */
void LYSeat::resetForGame()
{
	if (status == LYSeatOpen) return;

	lastBet = 0;
	handCards.clear();
//	chipsAtHand = chipsAtHand + newBuyin;
	chipsAtBeginning = chipsAtHand;
    if (this->chipsAtHand == 0) {
    	this->status = LYSeatOccupied;
    } else if (this->status != LYSeatOpen) {
    	this->status = LYSeatInGame;
    }
}

/*
 * reset all
 * 
 */
void LYSeat::reset()
{
	lastBet = 0;
	timeoutCount = 0;
	handCards.clear();
	chipsAtHand = 0;
	chipsAtBeginning = 0;
	status = LYSeatOpen;
	playerId = "";
	newBuyin = 0;
}

bool LYSeat::isInGame() { return false; }

/*
 * added 20160422 for client game sortSeats
 */
bool LYSeat::isOccupied()
{
	if (status == LYSeatOpen) return false;
	return true;
}

void LYSeat::occupy(unsigned int buyin, const std::string &uid)
{
    this->chipsAtBeginning = buyin;
    this->chipsAtHand = buyin;
    this->lastBet = 0;
    this->timeoutCount = 0; //added 20130519 reset timeoutCount when player taking seat
    this->status = LYSeatOccupied;
    this->playerId = uid;
}

void LYSeat::buyin(unsigned int buyin, const std::string &uid)
{
std::cout << "enter lyseat::buyin" << std::endl;
	if (this->playerId != uid) {
		return;
	}
//    this->chipsAtBeginning = this->chipsAtBeginning + buyin;
//    this->chipsAtHand = this->chipsAtBeginning;
	this->newBuyin = buyin;
}

/*
 * sd作为出参
 */
void LYSeat::genDynamics(LYSeatDynamics &sd)
{
	sd.chipsAtHand = this->chipsAtHand;
	sd.seatNo = this->seatNo;
	sd.lastBet = this->lastBet;
	sd.status = this->status;
	sd.timeoutCount = this->timeoutCount;
}

/*
 * 用于新局开始前检查有多少坐下
 * 不适用于已有游戏在该桌上
 */
bool LYSeat::isSeated()
{
	if (status != LYSeatOpen && chipsAtHand > 0) {
		return true;
	}
	return false;
}

std::vector < std::pair<std::string, std::string> > LYSeat::seat2kvps()
{
	std::vector < std::pair<std::string, std::string> > key_value_pairs;

	//table  LYTable
	std::pair < std::string, std::string > kvp;
	kvp.first = "seatNo";
	kvp.second = std::to_string(this->seatNo);
	//	LY_LOG_DBG("name:"+table->name);
	key_value_pairs.push_back(kvp);

	kvp.first = "chipsAtHand";
	kvp.second = std::to_string(this->chipsAtHand);
	//	LY_LOG_DBG("name:"+table->name);
	key_value_pairs.push_back(kvp);

	kvp.first = "playerId";
	kvp.second = this->playerId;
	//	LY_LOG_DBG("name:"+table->name);
	key_value_pairs.push_back(kvp);

	kvp.first = "handCards";
	if (this->handCards.size() > 0) {
		LYCardHelpers::cardsToStringifiedInt(this->handCards, kvp.second);
	} else {
		kvp.second = "0,0";
	}
	//	LY_LOG_DBG("name:"+table->name);
	key_value_pairs.push_back(kvp);

	kvp.first = "status";
	kvp.second = std::to_string(this->status);
	//	LY_LOG_DBG("name:"+table->name);
	key_value_pairs.push_back(kvp);

	kvp.first = "lastBet";
	kvp.second = std::to_string(this->lastBet);
	//	LY_LOG_DBG("name:"+table->name);
	key_value_pairs.push_back(kvp);

	kvp.first = "timeoutCount";
	kvp.second = std::to_string(this->timeoutCount);
	//	LY_LOG_DBG("name:"+table->name);
	key_value_pairs.push_back(kvp);

	kvp.first = "chipsAtBeginning";
	kvp.second = std::to_string(this->chipsAtBeginning);
	//	LY_LOG_DBG("name:"+table->name);
	key_value_pairs.push_back(kvp);

	kvp.first = "newBuyin";
	kvp.second = std::to_string(this->newBuyin);
	//	LY_LOG_DBG("name:"+table->name);
	key_value_pairs.push_back(kvp);

	return key_value_pairs;
}

bool LYSeat::allowLeave()
{
	return true;
}
