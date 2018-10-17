/*
 * LYTuringStrategy.cpp
 *
 *  Created on: 2014年11月25日
 *      Author: caiqingfeng
 */
#include "LYTuringStrategy.h"

#include <iostream>

#include "poker/src/LYCardHelpers.h"

LYTuringStrategy *LYTuringStrategy::_instance = NULL;

LYTuringStrategy::LYTuringStrategy()
{

}

LYTuringStrategy::~LYTuringStrategy()
{

}

LYTuringStrategy* LYTuringStrategy::Instance()
{
	if (NULL == _instance) {
		_instance = new LYTuringStrategy();
	}
	return _instance;
}

/*
 * 乱摆策略：
 * 从头到尾的摆
 *
 * 智能策略
 * 第一手策略说明：
 * 1、如果拿到了K、Q对，摆头道
 * 2、A对，摆中道
 * 3、其它任何两对，摆底道
 * 4、三条、四条，摆底道
 * 5、单张K、Q头道
 * 6、单张A中间
 * 7、天同花或者天顺子（无A，K，Q）摆底
 *
 * 其它手说明：
 * 1、
 */
void LYTuringStrategy::makeAction(LYPineappleTable* table, LYApplicant seat_no, LYPineappleAction& action)
{
	LYSeatPtr seat;
	if (table == NULL || ((seat = table->fetchSeat(seat_no)).get() == NULL)) {
		return;
	}

	LYPineappleSeat* ps = (LYPineappleSeat* )seat.get();
	if (ps->cardsOnHand.size() == 0) {
//		std::cout << "not a pinup generation...." << std::endl;
		return;
	}
	LYCardHelpers::sortCardsByFace(ps->cardsOnHand);

	//
	unsigned int idx_top = ps->topBricks.size();
	unsigned int idx_middle = ps->middleBricks.size();
	unsigned int idx_bottom = ps->bottomBricks.size();
	//unsigned int start_idx = idx_top + idx_middle + idx_bottom;

	std::vector<LYPineappleBrick> brks;
	for (unsigned int i=0; i<ps->cardsOnHand.size(); i++) {
		if (idx_top < 3) {
			LYPineappleBrick brick(idx_top+1, ps->cardsOnHand[i]);
			idx_top++;
	//	LYPineappleBrick brick(start_idx+1, ps->cardsOnHand[i]);
	//	start_idx ++;
			brks.push_back(brick);
		} else if (idx_middle < 5) {
			LYPineappleBrick brick(3 + idx_middle + 1, ps->cardsOnHand[i]);
			idx_middle++;
			brks.push_back(brick);
		} else if (idx_bottom < 5) {
			LYPineappleBrick brick(8 + idx_bottom + 1, ps->cardsOnHand[i]);
			idx_bottom++;
			brks.push_back(brick);
		}
	}

	if (brks.size() == 3 || brks.size() == 14) brks.pop_back();

	action.pinup(table->_id, seat_no, brks);
}
