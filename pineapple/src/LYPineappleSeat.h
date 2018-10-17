/*
 * LYPineappleSeat.h
 *
 *  Created on: 2013-7-5
 *      Author: caiqingfeng
 */

#ifndef LYPINEAPPLESEAT_H_
#define LYPINEAPPLESEAT_H_

#include <string>
#include "poker/src/LYSeat.h"
#include "poker/src/LYDeck.h"
#include "holdem/src/LYHandStrength.h"
#include "holdem/src/LYHoldemAlgorithmDelegate.h"
#include "LYPineappleBrick.h"
#include "LYPineappleRace.h"

/*
 * 本类仅用于序列化
 */

const unsigned int LYNORMAL = 0;
const unsigned int LYFOUL = 1;
const unsigned int LYABORT = 2;
const unsigned int LYFANTASY = 3;
const unsigned int LYTOPLINE = 0;
const unsigned int LYMIDDLELINE = 1;
const unsigned int LYBOTTOMLINE = 2;

class LYPineappleSeatDynamics : public LYSeatDynamics { //只用到了seatNo, seatStatus
public:
	unsigned int gameStatus; //LYNORMAL or LYFOUL
};

class LYPineappleSeat : public LYSeat{
public:
	int score; //总得分
	int score_race; //本局总得分情况
	unsigned int gameStatus; //LYNORMAL or LYFOUL
	std::vector<LYCard> topBricks;
	std::vector<LYCard> middleBricks;
	std::vector<LYCard> bottomBricks;
	std::vector<LYCard> cardsOnHand;
	bool voteNext;

public:
	LYRanking rankingOfTop; //头道大小
	LYRanking rankingOfMiddle; //中间道大小
	LYRanking rankingOfBottom; //底道大小
	LYHandStrengthPtr handStrengthOfTop;
	LYHandStrengthPtr handStrengthOfMiddle;
	LYHandStrengthPtr handStrengthOfBottom;

public:
	//20150208 新增
	void abortGame();

public:
	std::string allBricksStr();
	std::string cardsOnHandStr();
	std::string topLineStr();
	std::string middleLineStr();
	std::string bottomLineStr();

public:
	int raceAgainst(LYPineappleSeat& player, LYPineappleRace& race); //返回相对他的总得分，及每一道的得分
	void prepareForRace(LYHoldemAlgorithmDelegate* had = NULL);
	bool finishGame();

private:
	LYHandStrengthPtr genHandStrength(unsigned int line, LYHoldemAlgorithmDelegate *had);
	LYRanking getRanking(unsigned int line);

public:
	LYHandStrengthPtr genHandStrengthOfTop(LYHoldemAlgorithmDelegate *had, LYHandStrength* capHs=NULL);
	LYHandStrengthPtr genHandStrengthOfMiddle(LYHoldemAlgorithmDelegate *had, LYHandStrength* capHs=NULL);
	LYHandStrengthPtr genHandStrengthOfBottom(LYHoldemAlgorithmDelegate *had, LYHandStrength* capHs=NULL);
	/*
	 * 以下函数只操作Ranking
	 */
	LYRanking getRankingOfTop() {return getRanking(LYTOPLINE);};
	LYRanking getRankingOfMiddle(){return getRanking(LYMIDDLELINE);};;
	LYRanking getRankingOfBottom(){return getRanking(LYBOTTOMLINE);};;
	void setRankingOfTop(enum LYRanking rk) { rankingOfTop = rk; };
	void setRankingOfMiddle(enum LYRanking rk) { rankingOfBottom = rk; };
	void setRankingOfBottom(enum LYRanking rk) { rankingOfBottom = rk; };

public:
	unsigned int bonusOfTop();
	unsigned int bonusOfMiddle();
	unsigned int bonusOfBottom();

public:
	LYPineappleSeat(LYApplicant seat_no);
	LYPineappleSeat(LYPineappleSeatDynamics &seat_dyn);
	virtual ~LYPineappleSeat();

	virtual void reset();
	virtual void resetForNewGame();
	virtual std::string toString();

public:
	//重载occupy，reset gameStatus
	virtual void occupy(unsigned int buyin, const std::string &uid);

public:
	//设定自己的牌，称之为砌墙，砌好了就不能挪动。
	int pinup(LYPineappleBrick &pb); //0: OK, -1: 如果之前该Brick有牌则失败
	int pinup(std::vector<LYPineappleBrick> &pb);
	int blindPinup();
	int issueCards(std::vector<LYPineappleBrick> &pb);
	const LYCard& getCard(unsigned int &brick);

	LYPineappleSeat& operator = (const LYPineappleSeat &right)  {
		*((LYSeat *)this) = (LYSeat)right;
		this->score = right.score;
		this->score_race = right.score;
		this->rankingOfTop = right.rankingOfTop;
		this->rankingOfMiddle = right.rankingOfMiddle;
		this->rankingOfBottom = right.rankingOfBottom;
		this->handStrengthOfTop = right.handStrengthOfTop;
		this->handStrengthOfMiddle = right.handStrengthOfMiddle;
		this->handStrengthOfBottom = right.handStrengthOfBottom;
		return *this;
	}

	bool isInGame();
	bool isFantasy();

};

typedef std::shared_ptr<LYPineappleSeat> LYPineappleSeatPtr;

#endif /* LYHOLDEMSEAT_H_ */
