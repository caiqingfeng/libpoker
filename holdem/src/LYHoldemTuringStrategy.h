/*
 * LYTuringStrategy.h
 *
 *  Created on: 2014年11月25日
 *      Author: caiqingfeng
 */

#ifndef LY_HOLDEM_TURINGSTRATEGY_H_
#define LY_HOLDEM_TURINGSTRATEGY_H_

#include "LYHoldemTable.h"
#include "LYHoldemAction.h"

class LYHoldemTuringStrategy
{
private:
	static LYHoldemTuringStrategy *_instance;

protected:
	LYHoldemTuringStrategy();

public:
    static LYHoldemTuringStrategy *Instance();
	virtual ~LYHoldemTuringStrategy();

	void makeAction(LYHoldemTable* table, LYApplicant seat, LYHoldemAction& action);
};


#endif /* PINEAPPLE_SRC_LYTURINGSTRATEGY_H_ */
