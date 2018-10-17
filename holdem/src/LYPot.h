/*
 * LYPot.h
 *
 *  Created on: 2013-3-25
 *      Author: caiqingfeng
 */

#ifndef LYPOT_H_
#define LYPOT_H_

#include <vector>
#include <memory>

#include "poker/src/LYPokerConstants.h"
#include "LYHoldemConstants.h"
#include "LYHoldemSeat.h"
#include "LYHoldemAction.h"

class LYHoldemGame;

class LYAssurance {
public:
	LYGameRound round; // 在Turn上买的，还是在River上买的
	unsigned int seat; //领先的玩家
	std::vector<LYCard> againstOuts; //被击败的可能outs
	float odds; //买保险的赔率
	unsigned int maxchips; //锅有多少
	unsigned int assurance; //买的保险
	unsigned int expected; //期望赔多少，以大盲四舍五入
	unsigned int result; //最终结果，以大盲为四舍五入

public:
	LYAssurance();
	LYAssurance(LYGameRound rd, unsigned int st, unsigned int potsize, unsigned int assurance, const std::vector<LYCard>& outs);
	~LYAssurance();

public:
	bool doesHitAssurance(LYGameRound rd, unsigned int st, const LYCard &card);
	void makeResult(LYGameRound rd, unsigned int st, const LYCard &card);
	std::string toString();
};

typedef ::std::shared_ptr<LYAssurance> LYAssurancePtr;

class LYPot {
public:
	LYPot();
	LYPot(unsigned int chips);
	virtual ~LYPot();

public: //added 20160311,反序列化需要
	LYPot(std::vector<std::pair<std::string, std::string>>& kvps);
	void setPot(LYPot *pot);

public:
	enum LYPotLevel potLevel;
	bool potOpen; //缺省YES，在重新开始一轮Bet时，除了最后一个Pot，其它的全部是NO
	unsigned int chipsInPot; //当前所有的Chips
	unsigned int unitPrice; //单价
	bool priceFixed; //
	LYHandStrength *winningHand;
	std::vector<LYApplicant> seatsInPot; //
	std::vector<LYApplicant> winnersInPot; //
	std::vector<LYCard> chasingOuts; //当前每一轮的outs，每次会重新清空
	std::vector<LYAssurancePtr> assurances; //所有买的保险放在这里，一个Pot可能有不止一个人领先（可以买保险）
	std::vector<LYHoldemAction> assureActions;

public:
	void initWithChips(unsigned int chips);
	void addSeat(LYSeatPtr st);
	void addSeat(LYApplicant st);
	void fold(LYApplicant seat);
	void reset();
	bool haveSeat(enum LYApplicant player);
	bool isWinner(enum LYApplicant player);

public:
	std::string getSeatsString();
	std::string getWinnersString();
	std::string toString();
	std::vector<std::pair<std::string, std::string> > pot2kvps();

public:
	void buyAssurance(LYGameRound round, LYSeatPtr st, unsigned int assurance);
	bool validAssurance(unsigned int assurance);
	void caculateMatchResult(LYHoldemGame *game);
	void prepareMatchResult(LYHoldemGame *game);
	void caculateOuts(LYHoldemGame *game);
	void checkAssurance(LYGameRound rd, const LYCard &card);
	LYAssurancePtr findAssurance(LYGameRound round, LYApplicant seat, const LYCard &card);

	void makeAskAssuranceActions(LYHoldemGame *game, std::vector<LYHoldemAction>& assures);
	void makePaidAssuranceActions(LYHoldemGame *game, const LYCard& card, std::vector<LYHoldemAction>& assures);
	LYHoldemAction popAssureAction();
};

typedef ::std::shared_ptr<LYPot> LYPotPtr;


#endif /* LYPOT_H_ */
