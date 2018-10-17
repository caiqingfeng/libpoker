/*
 * LYTuringStrategy.h
 *
 *  Created on: 2014年11月25日
 *      Author: caiqingfeng
 */

#ifndef PINEAPPLE_SRC_LYTURINGSTRATEGY_H_
#define PINEAPPLE_SRC_LYTURINGSTRATEGY_H_

#include "LYPineappleTable.h"
#include "LYPineappleAction.h"

class LYTuringStrategy
{
private:
	static LYTuringStrategy *_instance;

protected:
	LYTuringStrategy();

public:
    static LYTuringStrategy *Instance();
	virtual ~LYTuringStrategy();

	void makeAction(LYPineappleTable* table, LYApplicant seat, LYPineappleAction& action);
};


#endif /* PINEAPPLE_SRC_LYTURINGSTRATEGY_H_ */
