/*
 * LYHoldemAction.h
 *
 *  Created on: 2013-7-4
 *      Author: caiqingfeng
 */

#ifndef LYHOLDEMACTION_VIEW_H_
#define LYHOLDEMACTION_VIEW_H_
#include <string>

/*
 * 本类用于指示View正确的设置Action
 * 到了自己就设置Duty，否则设置ActionInAdvance
 */
class LYHoldemActionView {
public:
	LYHoldemActionView();
	virtual ~LYHoldemActionView();
public:
	bool onDuty; //true: action, false: action in advance
	bool checkEnabledForBigBlind; //Preflop时大家一路Call到BigBlind时，允许大盲Check
	bool allinIfCall; //true: 显示 fold allin; false: 显示fold call raise+slider，其范围为raiseStart-raiseEnd
//	bool allinIfRaise; //true: 显示 fold call allin; false: 显示 fold call raise+slider, 其范围为raiseStart-raiseEnd
	unsigned int betIfCall; //最新的BetIfCall
	unsigned int betToCall; //用于显示在Call Btn上，＝betIfCall-betToCall
	unsigned int raiseMin;
	unsigned int raiseMax;

public:
	std::string toString();
};

#endif /* LYHOLDEMACTION_H_ */
