/*
 * LYPineappleGame.h
 *
 *  Created on: 2014-9-16
 *      Author: caiqingfeng
 */

#ifndef LYPINEAPPLEGAME_H_
#define LYPINEAPPLEGAME_H_

#include <vector>
#include <string>
#include "poker/src/LYGame.h"
#include "LYPineappleScene.h"
#include "LYPineappleAction.h"
#include "LYPineappleTable.h"
#include "holdem/src/LYHoldemAlgorithmDelegate.h"
#include "LYPineappleActionView.h"

/*
 * PineappleGame有多个Scene
 * PineappleGame本身保存在数据库里的信息只需要LYHoldemStatic
 * 之所以从LYHoldemDynamics也派生，是因为在类处理的需要
 */

class LYPineappleGame : public LYGame, public LYPineappleStatic, public LYPineappleDynamics{
public:
	std::vector<LYPineappleScenePtr> scenes; //Scene只由Game管理
	LYPineappleScene *lastScene();

public:
	/*
	 * 服务器在创建桌子时，会调用 new LYPineappleGame生成一个新的Game，此时pendingForRestore为False，会在构造函数里自动调用
	 * reset all seats, generate actions等
	 * 客户端从Ota中恢复数据，此时pending为True，并不会去Reset Seats和Gen Actions
	 * 服务器从DB中恢复数据，同客户端，pending为True
	 */
	LYPineappleGame(const std::string &game_id,
			std::vector<LYSeatPtr> &all_seats,
			LYSeatPtr btnSeat,
			LYPineappleTable *tbl, LYHoldemAlgorithmDelegate *had=NULL,
			bool pendingForRestore=false, std::string prof="private_noking");
	virtual ~LYPineappleGame();
	void reset();

public:
	//just for internal process
	std::vector<LYSeatPtr> sortedSeats;
	LYPineappleSeat* seat1, *seat2, *seat3;
	LYPineappleRace seat1_vs_seat2; //
	LYPineappleRace seat2_vs_seat3; //
	LYPineappleRace seat1_vs_seat3; //

	std::vector<LYPineappleAction> actions;
	unsigned int actionIndex;
	LYPineappleAction& popAction();
	void genActions();
	LYPineappleAction genFantasyAction(const std::string& player, enum LYApplicant seat);
	LYPineappleAction genHandAction(const std::string& player, enum LYApplicant seat, unsigned int round);

public:
	LYSeatPtr buttonSeat();
	LYSeatPtr dutySeat();

private:
	void genNewScene();

public:
	unsigned int getRound();
	LYPineappleScenePtr getLastScene();

public: //游戏动作
	bool onAction(LYPineappleAction &action);
	bool isValidAction(LYPineappleAction &action);
	void onIssueCards(LYPineappleSeat* seat, std::vector<LYPineappleBrick>& bricks);
	void onPolling(LYPineappleSeat* seat);
	bool onPinup(LYPineappleSeat* seat, std::vector<LYPineappleBrick>& bricks);
	bool onPinup(LYPineappleSeat* seat);

public:
	void gameOver();
	void dutyMoveToNextSeat(); //deprecated!!!
	bool hasFantasy();

public:
	bool newRoll();
	bool isWinner(LYApplicant seatNo);
	void sortSeats(); // sort seats and set game status to HoldemWaitng
	void sortSeatsWithoutChangeStatus();

private:
	LYSeatPtr fetchFirstSeatInGame();
	LYSeatPtr fetchNextSeatInGame(LYApplicant seatNo);
	void resetSeatsForNextGame();

public:
	//added 20130615
	std::string getHandCardFace(enum LYApplicant player);
	bool isHandCardFlush(enum LYApplicant player);
	enum LYHandsOrder judgeHand(enum LYApplicant player);
	enum LYHandsOrder judgeCommunity();

	void startGame();
	bool ready2go(); //为true要求坐下大于2人
public:

//	void genActionView(LYApplicant seatNo, LYHoldemActionView& actionView); //View调用本方法，将自己的位置传入
};

typedef ::std::shared_ptr<LYPineappleGame> LYPineappleGamePtr;

#endif /* LYPINEAPPLEGAME_H_ */
