/*
 * LYHoldemAction.h
 *
 *  Created on: 2014-9-14
 *      Author: caiqingfeng
 */

#ifndef LYPINEAPPLEACTION_VIEW_H_
#define LYPINEAPPLEACTION_VIEW_H_
#include <string>

/*
 * 本类用于指示View正确的设置Action
 * 到了自己就设置Duty，否则设置ActionInAdvance
 */
class LYPineappleActionView {
public:
	LYPineappleActionView();
	virtual ~LYPineappleActionView();
public:
	bool onDuty; //true: action, false: action in advance

public:
	std::string toString();
};

#endif /* LYPINEAPPLEACTION_VIEW_H_ */
