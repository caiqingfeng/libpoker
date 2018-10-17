/*
 * LYHoldemProfile.cpp
 *
 *  Created on: 2013-4-10
 *      Author: caiqingfeng
 */

#include "LYHoldemProfile.h"

LYHoldemProfile::LYHoldemProfile()
{
}

LYHoldemProfile::LYHoldemProfile(unsigned int small, unsigned big) :
	LYPokerProfile(small, big)
{
}

LYHoldemProfile::LYHoldemProfile(const std::string &pid) :
	LYPokerProfile(pid)
{
}

LYHoldemProfile::~LYHoldemProfile() {
	// TODO Auto-generated destructor stub
}

