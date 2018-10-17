/*
 * LYHoldemSeat.h
 *
 *  Created on: 2013-7-5
 *      Author: caiqingfeng
 */

#ifndef LYHOLDEMSEAT_H_
#define LYHOLDEMSEAT_H_

#include <memory>
#include <string>
#include "poker/src/LYSeat.h"
#include "poker/src/LYDeck.h"
#include "LYHandStrength.h"
#include "LYHandOdds.h"
#include "LYHoldemAlgorithmDelegate.h"

/*
 * 本类仅用于序列化
 */
class LYHoldemSeatDynamics : public LYSeatDynamics {
public:
	enum LYSeatHoldemStatus gameStatus;
};

class LYHoldemSeat : public LYSeat{
public:
	LYRanking ranking; //20130718增加，保存从网络传过来的RankingString
	LYHandStrengthPtr handStrength;
	enum LYSeatHoldemStatus gameStatus;

public:
	LYHandStrengthPtr genHandStrength(const std::vector<LYCard> &communityCards, LYHoldemAlgorithmDelegate *had);
	/*
	 * 以下两个函数只操作Ranking
	 */
	LYRanking getRanking();
	void setRanking(enum LYRanking rk) { ranking = rk; };

public:
	std::vector<LYCard> caculateOuts(LYHandStrength *againstHand, const std::vector<LYCard>& communityCards,
									std::vector<LYCard>& allVisibleCards);

public:
	LYHoldemSeat(LYApplicant seat_no);
	LYHoldemSeat(LYHoldemSeatDynamics &seat_dyn);
	LYHoldemSeat(std::vector < std::pair<std::string, std::string> >& kvps);
	virtual ~LYHoldemSeat();

	void resetForNewGame();
	virtual void reset();
	virtual std::string toString();
	virtual void resetForGame();

	const LYCard& firstCard();
	void setFirstCard(const LYCard &card);
	void setSecondCard(const LYCard &card);
	const LYCard& secondCard();
	void setFirst(const LYCard &cd);
	void setSecond(const LYCard &cd);
	void genHoldemDynamics(LYHoldemSeatDynamics &sd);
//	void buyAssurance(unsigned int round, unsigned int assurance, const std::string& pot);
	unsigned int getChipsLeft() { return chipsAtHand+lastBet;}

	virtual bool isInGame();
	bool isInGameAndNotAllin() {
		if (status != LYSeatInGame) {
			return false;
		}
		switch (gameStatus) {
		case LYSeatHoldemWaiting:
		case LYSeatHoldemBetSmall:
		case LYSeatHoldemBetBig:
		case LYSeatHoldemInCalled:
		case LYSeatHoldemInChecked:
		case LYSeatHoldemInRaised:
		case LYSeatHoldemInBet:
		case LYSeatHoldemMakingDecision:
			return true;
		case LYSeatHoldemInFold:
		case LYSeatHoldemInAllin:
		default:
			return false;
		}
	}

	LYHoldemSeat& operator = (const LYHoldemSeat &right)  {
		*((LYSeat *)this) = (LYSeat)right;
		this->ranking = right.ranking;
		this->handStrength = right.handStrength;
		this->gameStatus = right.gameStatus;
		return *this;
	}

public: //added 20160315
	virtual std::vector < std::pair<std::string, std::string> > seat2kvps();
	virtual void buyin(unsigned int buyin, const std::string &uid);
	void fold(const std::string& reason);
	virtual bool allowLeave();
};

typedef ::std::shared_ptr<LYHoldemSeat> LYHoldemSeatPtr;

#endif /* LYHOLDEMSEAT_H_ */
