/*
 * LYHoldemScene.h
 *
 *  Created on: 2013-7-6
 *      Author: caiqingfeng
 */

#ifndef LYHOLDEMSCENE_H_
#define LYHOLDEMSCENE_H_

#include <string>
#include <vector>
#include <memory>
#include "LYPot.h"
#include "poker/src/LYPokerConstants.h"
#include "LYHoldemConstants.h"
#include "poker/src/LYSeat.h"
#include "poker/src/LYCardHelpers.h"

//在一局正在进行时，即赋值为1，一局结束时赋值为0
const unsigned int LY_HOLDEMGAME_OVER = 0;
const unsigned int LY_HOLDEMGAME_ONGOING = 1;

class LYHoldemStatic {
public:
	//静态信息, 保存在数据库中
	unsigned int gameStatus;
	unsigned int roundOfTournament; //如果是Tournament，当前处于第几轮
//	LYSeatPtr buttonSeat; //changed to seat no 20130719
	LYApplicant btnSeatNo;
//	LYSeatPtr smallBlindSeat;
	LYApplicant sbSeatNo;
//	LYSeatPtr bigBlindSeat;
	LYApplicant bbSeatNo;
	std::vector<LYCard> flopCards;
	LYCard turnCard;
	LYCard riverCard;
	unsigned int smallBlindPrice;
	unsigned int bigBlindPrice;

public:
	LYCard getCommunityCard(int index) {
		if (index > 4) {
			return flipCard;
		}
		switch (index) {
		case 4:
			return riverCard;
		case 3:
			return turnCard;
		default:
			if (flopCards.size() != 3) {
				return flipCard;
			}
			return flopCards[index];
		}
		return flipCard;
	}

	std::string getFlopString() {
		std::string s;
		LYCardHelpers::cardsToStringifiedInt(flopCards, s);
		return s;
	}

public:
	std::vector<std::pair<std::string, std::string> > static2kvps();
};

class LYHoldemDynamics {
public:
	unsigned int seqNo; //sequence of game scene, for synchronize
	enum LYGameRound round; //当前处于一局游戏的哪一轮：Preflop/Flop/Turn/River？
//	LYSeatPtr dutySeat; //正在作决定的位置 = nil 意味着新一轮的开始，通常是Preflop，当前这一轮所有决定做完了交由LYHouse
	LYApplicant dutySeatNo;
	unsigned int betIfCall; //当前最大注
	unsigned int minBetIfRaise; //如果Raise，最少筹码
//	LYSeatPtr betBy; //当前最大注是谁发起的
	LYApplicant betBySeatNo;
	enum LYHoldemRoundContinue continueThisRound; //非常重要的一个变量，决定是否继续游戏还是Showdown
	std::vector<LYHoldemAction> assureActions;
	unsigned int assureActionNumbers;
//	std::vector<std::pair<std::string, std::string> > lastPollingAction; //added 20160630,存放上次问客户端的命令的to_kvps，可以是polling，也可以是ask_assurance
	LYHoldemAction lastPollingAction; //added 20160630,存放上次问客户端的命令的to_kvps，可以是polling，也可以是ask_assurance
	unsigned int lastPollingAt; //added 20160630,存放上次问客户端的时间

public:
	std::vector<std::pair<std::string, std::string> > dynamic2kvps();
};

class LYHoldemScene : public LYHoldemDynamics{
public:
	std::string _id;
	std::vector<LYPot> pots; //当前Pot的情况 此处不存指针，而保留一个快照
	std::vector<LYHoldemSeatDynamics> seats; //所有Seats的情况，只处只存Seat的动态信息

public:
	LYHoldemScene(std::vector<LYSeatPtr> &all_seats, std::vector<LYPotPtr> &all_pots,
			enum LYApplicant duty_seat, unsigned int bet_if_call, unsigned int min_bet_if_raise,
			enum LYApplicant bet_by, enum LYGameRound rnd, enum LYHoldemRoundContinue cont,
			unsigned int seq);
	virtual ~LYHoldemScene();
	std::string toString();

public:
	std::vector<std::pair<std::string, std::string> > scene2kvps();
};

typedef ::std::shared_ptr<LYHoldemScene> LYHoldemScenePtr;

#endif /* LYHOLDEMSCENE_H_ */
