/*
 * LYHoldemGame.h
 *
 *  Created on: 2013-7-6
 *      Author: caiqingfeng
 */

#ifndef LYHOLDEMGAME_H_
#define LYHOLDEMGAME_H_

#include <vector>
#include <string>
#include "poker/src/LYGame.h"
#include "LYHoldemSeat.h"
#include "LYPotManager.h"
#include "LYHoldemScene.h"
#include "LYHoldemAction.h"
#include "LYHoldemTable.h"
#include "LYHoldemAlgorithmDelegate.h"
#include "LYHoldemActionView.h"
#include "poker/src/LYGameLogMgr.h"

/*
 * HoldeGame有多个Scene
 * HoldemGame本身保存在数据库里的信息只需要LYHoldemStatic
 * 之所以从LYHoldemDynamics也派生，是因为在类处理的需要
 */

class LYHoldemGame : public LYGame, public LYHoldemStatic, public LYHoldemDynamics{
public:
	std::vector<LYHoldemScenePtr> scenes; //Scene只由Game管理
	LYHoldemScene *lastScene();

public:
	LYHoldemGame(const std::string &game_id,
			std::vector<LYSeatPtr> &all_seats, unsigned int sbp, unsigned int bbp,
			LYSeatPtr btnSeat, LYSeatPtr sbSeat, LYSeatPtr bbSeat,
			LYHoldemTable *tbl, LYHoldemAlgorithmDelegate *had=NULL,
			unsigned int rot = 0);
	LYHoldemGame(const std::string &game_id,
			std::vector<LYSeatPtr> &all_seats, LYHoldemTable *tbl,
			LYHoldemAlgorithmDelegate *had=NULL);
	//从内存数据库中恢复Game, added 20160311
	LYHoldemGame(const std::string &game_id,
			std::vector<LYSeatPtr> &all_seats, LYHoldemTable *tbl,
			std::vector < std::pair<std::string, std::string> >& kvps,
			LYHoldemAlgorithmDelegate *had=NULL);
	LYHoldemGame(const std::string &game_id="", LYHoldemAlgorithmDelegate *had=NULL);
	virtual ~LYHoldemGame();
	void reset();

public:
	//just for internal process
	std::vector<LYSeatPtr> sortedSeats;
	LYPotManager pm;
	LYPotPtr mainPot() {return pm.mainPot; }; //快捷方式，其也在pots时保存一个指针，但是同一个Pot
	std::vector<LYPotPtr>& pots() {return pm.pots; }; //当前Pot的情况
	LYHoldemAction flop_action;
	LYHoldemAction turn_action;
	LYHoldemAction river_action;
	LYHoldemAction assurance_summary_action;
	LYGameLogMgr logMgr;

public:
	LYSeatPtr buttonSeat();
	LYSeatPtr smallBlindSeat();
	LYSeatPtr bigBlindSeat();
	LYSeatPtr dutySeat();
	LYSeatPtr betBy();

public:
	std::vector<std::pair<std::string, std::string> > game2kvps();

private:
	void genCommunityCardActions();
	void reverseCommunityCardActions();
	void genNewScene();

public:
	std::vector<LYCard> getCommunityCards();

public:
	enum LYGameRound getRound();
	LYHoldemScenePtr getLastScene();

public: //游戏动作
	bool onAction(LYHoldemAction &action);
	void onPolling(LYSeatPtr st);
	void onCall(LYSeatPtr st, unsigned int bet);
	void onRaise(LYSeatPtr st, unsigned int bet);
	void onBet(LYSeatPtr st, unsigned int bet);
	void onBetSmallBlind(LYSeatPtr st, unsigned int bet);
	void onBetBigBlind(LYSeatPtr st, unsigned int bet);
	void onAllin(LYSeatPtr st, unsigned int bet);
	void onFold(LYSeatPtr st, const std::string &reason="byuser");
	void onCheck(LYSeatPtr st);
	void onAssure(unsigned int round, LYSeatPtr st, unsigned int bet, const std::string& pot);

public:
	bool isValidAction(LYHoldemAction &action);
	void updateHoldemRound(LYHoldemAction &action);
	void gameOver();
	void dutyMoveToNextSeat();

public:
	bool newRoll();
	LYSeatPtr fetchNextSeatInGame(LYApplicant seat_no);
	LYSeatPtr fetchNextSeatInGameAndNotAllin(LYApplicant seat_no);
	LYRanking getSeatRanking(LYApplicant seatNo);
	bool isWinner(LYApplicant seatNo);
	void sortSeats(); // sort seats and set game status to HoldemWaitng
	void sortSeatsWithoutChangeStatus(); //discarded 20160730
	void sortSeatsInGame(); //added 20160730
	void cleanZombies();

private:
	LYSeatPtr fetchFirstSeatInGame();
	void resetSeatsForNextGame();

public:
	void newRound();
	LYHandStrength *getHandStrength(enum LYApplicant player);
	//added 20130615
	std::string getHandCardFace(enum LYApplicant player);
	bool isHandCardFlush(enum LYApplicant player);
	enum LYHandsOrder judgeHand(enum LYApplicant player);
	enum LYHandsOrder judgeCommunity();
	unsigned int getOuts(enum LYApplicant player);
	unsigned int getChipsIfWin(enum LYApplicant player);
	LYHandOdds *getHandOdds(enum LYApplicant player);

	void onFlop(const std::vector<LYCard> &cards);
	void onTurn(const LYCard &card);
	void onRiver(const LYCard &card);

	bool isButtonSeat(LYSeatPtr st);
	bool isBigBlindSeat(LYSeatPtr st);
	bool isSmallBlindSeat(LYSeatPtr st);

	void betBySeat(unsigned int chips, LYSeatPtr st); //自动加入正确的Pot
	void startGame();
	bool ready2go(); //为true要求坐下大于2人
	virtual bool readyForNext(); //只在ServerHall的takeSeat时调用，为true要求 round为非游戏状态或者只有两个人时依然是游戏状态（说明恢复状态时处理有问题）
	enum LYGameRound checkRoundByAction(const LYHoldemAction &action);

	LYRanking caculateSeatRanking(LYApplicant seatNo);

public:
	unsigned int getMaxChipsLeftOfOthers(LYApplicant seatNo);
	void genActionView(LYApplicant seatNo, LYHoldemActionView& actionView); //View调用本方法，将自己的位置传入
	void genAssureActions(std::vector<LYHoldemAction>& actions, const LYCard card=flipCard, bool nomoreAssure=false);
	void makePaidAssurances(std::vector<LYHoldemAction>& actions, const LYCard& card);
	void checkAssurance(const LYCard &card);
	void caculateAssuranceResult(LYAssurance *assure);
	bool finishAssurance();

public: //added 20160310
	void buildPot(std::vector<std::pair<std::string, std::string>>& kvps);
	std::vector<LYCard> getAllVisibleCards();

	//added 20160701
	std::vector<std::string> getWhiteList();
	unsigned int totalSeatsInGame();
	LYSeatPtr findLastSeatInGame();

public:
	void debugGame();
	std::string toString();

	LYHoldemAction popAssureAction();

public:
	void appendLog(const std::string& log);
	std::vector<std::string> getGameLogs();
};

#endif /* LYHOLDEMGAME_H_ */
