/*
 * LYSeat.h
 *
 *  Created on: 2013-3-25
 *      Author: caiqingfeng
 */

#ifndef LYSEAT_H_
#define LYSEAT_H_

#include <memory>
#include <vector>
#include "LYCard.h"
#include "LYPokerConstants.h"
#include <string>

/*
 * 在游戏中变化的参数
 * 例如在HoldemScene中就只保存Dynamic，
 */
class LYSeatVolatile {
public:
	int chipsAtHand;
	enum LYSeatStatus status;
	unsigned int lastBet;
	unsigned int timeoutCount; //serialize it but without transfer it (over the air)

};

class LYSeatDynamics : public LYSeatVolatile
{
public:
	enum LYApplicant seatNo;

public:
	virtual std::string toString();
};

/*
 * 在游戏中不变的参数
 * 例如在Game中就只保存Static，
 */
class LYSeatStatic {
public:
	enum LYApplicant seatNo;
	std::string playerId;
	std::vector<LYCard> handCards;
	int chipsAtBeginning; //changed 20160329 from unsigned to signed
	unsigned int newBuyin; //added 20151225 cqf, buyin 单独加，每次新游戏开始的时候把buyin加到chipsAtBeginning/chipsAtHand上

};

class LYSeat : public LYSeatVolatile, public LYSeatStatic {
public:
	LYSeat(enum LYApplicant sn);
	LYSeat(std::vector < std::pair<std::string, std::string> >& kvps);
	virtual ~LYSeat();

public:
	virtual void reset();
	virtual void resetForGame();

	LYSeat& operator = (const LYSeat &right)  {
		this->seatNo = right.seatNo;
		this->playerId = right.playerId;
		this->handCards = right.handCards;
		this->lastBet = right.lastBet;
		this->chipsAtHand = right.chipsAtHand;
		this->chipsAtBeginning = right.chipsAtBeginning;
		this->status = right.status;
		return *this;
	}

	virtual bool isInGame();
	virtual bool isOccupied();
	bool hasBiggerSeatNo(enum LYApplicant player) { return this->seatNo > player;}
	bool hasSmallerSeatNo(enum LYApplicant player) { return this->seatNo < player;}
	bool isMe(enum LYApplicant player) { return this->seatNo == player;}
	bool isMe(const std::string &player) { return this->playerId == player;}
	enum LYSeatStatus getStatus() { return status; }
	int getChipsAtHand() { return chipsAtHand; }
	void addToLastBet(int bet) { lastBet += bet; }
	unsigned int getLastBet() { return lastBet; };
	enum LYApplicant getSeatNo() { return seatNo; };
	bool isSeated();

public:
	void genDynamics(LYSeatDynamics &sd);
	virtual std::vector < std::pair<std::string, std::string> > seat2kvps();

public:
	virtual void occupy(unsigned int buyin, const std::string &uid);
	virtual void buyin(unsigned int buyin, const std::string &uid);
	std::string toString();
	virtual bool allowLeave();
};

typedef ::std::shared_ptr<LYSeat> LYSeatPtr;

#endif /* LYSEAT_H_ */
