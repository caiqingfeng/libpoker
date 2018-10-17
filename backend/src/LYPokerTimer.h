/*
 * LYPineappleTimer.h
 *
 *  Created on: 2015-1-27
 *      Author: caiqingfeng
 */

#ifndef LYPOKERTIMER_H_
#define LYPOKERTIMER_H_

//改写LYPokerTimer, 把Timer从Table中移到Hall中，一个Table有两类最多10个Timer，分别是nextgame和polling1-9
#include <string>
#include <memory>
#include "libmrock/common/src/LYTimer.h"

class LYPokerTimer {
public:
	LYPokerTimer();
	virtual ~LYPokerTimer();

public:
	std::map<std::string, LYTimerPtr> tableTimers;
	void clearTimersExceptCloseTable();
};

typedef std::shared_ptr<LYPokerTimer> LYPokerTimerPtr;

#endif /* LYHOLDEMTIMER_H_ */
