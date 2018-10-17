/*
 * LYHoldemActionView.cpp
 *
 *  Created on: 2014-1-14
 *      Author: caiqingfeng
 */

#include "LYHoldemActionView.h"
#include <sstream>

LYHoldemActionView::~LYHoldemActionView()
{
}

LYHoldemActionView::LYHoldemActionView()
{
	onDuty = false;
	checkEnabledForBigBlind = false;
	allinIfCall = false;
//	allinIfRaise = false;
	betIfCall = 0;
	betToCall = 0;
	raiseMin = 0;
	raiseMax = 0;
}

std::string LYHoldemActionView::toString()
{
	std::string str;
	std::ostringstream ios;
    ios << "onDuty:" << onDuty << std::endl;
    ios << "checkEnabledForBigBlind:" << checkEnabledForBigBlind  << std::endl;
    ios << "allinIfCall:" << allinIfCall  << std::endl;
//    ios << "allinIfRaise:" << allinIfRaise  << std::endl;
    ios << "betIfCall:" << betIfCall  << std::endl;
    ios << "betToCall:" << betToCall  << std::endl;
    ios << "raiseMin:" << raiseMin  << std::endl;
    ios << "raiseMax:" << raiseMax  << std::endl;
    str = ios.str();
    return str;
}
