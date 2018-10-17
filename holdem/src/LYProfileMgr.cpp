/*
 * LYProfileMgr.cpp
 *
 *  Created on: 2013-7-13
 *      Author: caiqingfeng
 */

#include "LYProfileMgr.h"
#include <iostream>
//#include "common/src/my_log.h"

LYProfileMgr::LYProfileMgr() {
	// TODO Auto-generated constructor stub

}

LYProfileMgr::~LYProfileMgr() {
	// TODO Auto-generated destructor stub
}

LYHoldemProfilePtr LYProfileMgr::getHoldemProfileById(const std::string &prof_id)
{
//	LY_LOG_DBG("profile size="<<profiles.size());
	std::map<std::string, LYHoldemProfilePtr>::iterator it = profiles.find(prof_id);
	if (it != profiles.end()) {
		return it->second;
	}
	return LYHoldemProfilePtr();
}

bool LYProfileMgr::isValidBuyin(const std::string& profile, unsigned int chipsAtHand, unsigned int buyin) //买入是否在允许范围内
{
	LYHoldemProfilePtr profilePtr = getHoldemProfileById(profile);
	if (profilePtr.get() == NULL) {
		return false;
	}

	return profilePtr->isValidBuyin(chipsAtHand, buyin);
}
