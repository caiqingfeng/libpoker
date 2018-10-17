/*
 * LYPotManager.h
 *
 *  Created on: 2013-3-25
 *      Author: caiqingfeng
 */

#ifndef LYPOTMANAGER_H_
#define LYPOTMANAGER_H_

#include "poker/src/LYPokerConstants.h"
#include "LYHoldemConstants.h"
#include "LYPot.h"

class LYHoldemGame;

class LYPotManager {
private:
	LYHoldemGame *game;
public:
	LYPotManager(LYHoldemGame *gm);
	virtual ~LYPotManager();

public:
	LYPotPtr mainPot; //same reason as LYSeat, LYPot is pointer here!
	std::vector<LYPotPtr > pots; //same reason as container of LYSeat
	enum LYPotLevel topLevel;

public:
	void createPots(unsigned int pot_size);
	void createSidePot(unsigned intchips, std::vector<LYSeatPtr>seats);
	void addPot(LYPot *pot);
	LYPotPtr fetchPot(enum LYPotLevel pl);
	void betBySeat(unsigned int chips, LYSeatPtr st); //自动加入正确的Pot
	void newRound();
	void reset();
	unsigned int getChipsIfWin(enum LYApplicant player);
	void fold(LYApplicant seat);//将该seat从InvolvedSeats去掉
	LYPotPtr& topPot();

public: //added 20160310
	void buildPot(std::vector<std::pair<std::string, std::string>>& kvps);
	void buyAssurance(unsigned int round, LYSeatPtr st, unsigned int assurance, const std::string& pot);

private:
	LYSeatPtr findSeatInPot(LYSeatPtr st, LYPotPtr pot);

};

#endif /* LYPOTMANAGER_H_ */
