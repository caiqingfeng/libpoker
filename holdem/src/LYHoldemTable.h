/*
 * LYHoldemTable.h
 *
 *  Created on: 2013-7-6
 *      Author: caiqingfeng
 */

#ifndef LYHOLDEMTABLE_H_
#define LYHOLDEMTABLE_H_

#include "poker/src/LYTable.h"
#include "LYHoldemSeat.h"
#include "LYHoldemAction.h"
#include "LYProfileMgr.h"
#include "LYHoldemAlgorithmDelegate.h"

class LYHoldemTable : public LYTable {
public:
	LYProfileMgr *profileMgr;

public:
	LYHoldemTable(const std::string &tid, unsigned int ts, enum LYTableStatus sta,
			const std::string &nm, const std::string &oid=LUYUN_HOUSE, unsigned int occupied=0, LYProfileMgr *prof_mgr=NULL);
	LYHoldemTable(const std::string &tid, LYProfileMgr *prof_mgr=NULL);
	virtual ~LYHoldemTable();

public:
	void createSeats();
	void resetSeats(); //reset seat to initial status
	void setSeat(const LYHoldemSeat &seat);
	bool playGame(LYHoldemAction &action);
	LYRanking caculateSeatRanking(LYApplicant seatNo);

public:
	virtual void openTrunk(const std::string &trunk_id, const std::string &trunk_name,
			const std::string &player, const std::string &prof_id);
	virtual void openTrunk(const std::string &trunk_id, const std::string &trunk_name,
			const std::string &player, LYPokerProfile *prof);
	virtual void openTrunk(const std::string &trunk_id);

public:
	virtual bool ready2go();
	virtual bool isGameOver();
	virtual void createGame(const std::string &game_id="newgame", LYHoldemAlgorithmDelegate *had=NULL);
	virtual void createGameInstance(const std::string &game_id);
	void createGameInstance(const std::string &game_id, std::vector < std::pair<std::string, std::string> >& kvps);
	void run();
	virtual bool isGameOngoing();

public:
	unsigned int getCurrentMaxBuyin();
	unsigned int getCurrentMinBuyin();
	//added 20160701
	std::vector<std::string> getWhiteList();

public:
	virtual std::vector < std::pair<std::string, std::string> > table2kvps();
	std::vector<std::string> showLastGame();

};

#endif /* LYHOLDEMTABLE_H_ */
