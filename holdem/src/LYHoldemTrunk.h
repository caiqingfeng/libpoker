/*
 * LYHoldemTrunk.h
 *
 *  Created on: 2013-7-5
 *      Author: caiqingfeng
 */

#ifndef LYHOLDEMTRUNK_H_
#define LYHOLDEMTRUNK_H_

#include "poker/src/LYTrunk.h"
#include "LYHoldemAction.h"
#include "LYHoldemProfile.h"
#include "LYProfileMgr.h"
#include "LYHoldemTable.h"
#include "LYHoldemAlgorithmDelegate.h"

/*
 * Dealer和Trunk生命周期相同
 */
class LYHoldemTrunk : public LYTrunk{
public:
	LYHoldemTrunk(const std::string &trunk_id, const std::string &trunk_name,
				LYHoldemTable *tbl, unsigned int &ts, std::vector<LYSeatPtr> &all_seats,
				const std::string &player, const std::string &prof_id);
	virtual ~LYHoldemTrunk();

	bool playGame(LYHoldemAction &action);

public: //游戏动作
	virtual void createGame(const std::string &game_id="newgame", LYHoldemAlgorithmDelegate *sd=NULL); //only one参数，让Trunk根据自身状态即可作出判断
	virtual void createGameInstance(const std::string &game_id);
	virtual void createGameInstance(const std::string &game_id, std::vector < std::pair<std::string, std::string> >& kvps);
	virtual bool ready2go();
	virtual bool isGameOver();
	unsigned int getSmallBlindPrice();
	unsigned int getBigBlindPrice();
	unsigned int getCurrentMaxBuyin();
	unsigned int getCurrentMinBuyin();
	virtual void activeProfile();

public:
	void run(); //当从数据库中恢复数据时，重新设置定时器

private:
	void resetAllSeatsForNewGame();
};

#endif /* LYHOLDEMTRUNK_H_ */
