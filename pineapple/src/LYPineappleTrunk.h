/*
 * LYHoldemTrunk.h
 *
 *  Created on: 2013-7-5
 *      Author: caiqingfeng
 */

#ifndef LYHOLDEMTRUNK_H_
#define LYHOLDEMTRUNK_H_

#include "poker/src/LYTrunk.h"
#include "LYPineappleAction.h"
#include "LYPineappleTable.h"
#include "holdem/src/LYHoldemAlgorithmDelegate.h"

/*
 * Dealer和Trunk生命周期相同
 */
class LYPineappleTrunk : public LYTrunk{
public:
	LYPineappleTrunk(const std::string &trunk_id, const std::string &trunk_name,
				LYPineappleTable *tbl, unsigned int &ts, std::vector<LYSeatPtr> &all_seats,
				const std::string &player, const std::string &prof_id);
	virtual ~LYPineappleTrunk();

	bool playGame(LYPineappleAction &action);

public: //游戏动作
	virtual void createGame(const std::string &game_id="newgame", LYHoldemAlgorithmDelegate *sd=NULL,
			std::string prof="private_noking"); //only one参数，让Trunk根据自身状态即可作出判断
	void createGameInstance(const std::string &game_id);
	void run(LYHoldemAlgorithmDelegate *had=NULL);
//	virtual bool ready2go();
//	virtual bool isGameOver();
//	virtual void activeProfile();

private:
//	void resetAllSeatsForNewGame();
};

#endif /* LYHOLDEMTRUNK_H_ */
