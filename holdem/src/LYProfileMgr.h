/*
 * LYProfileMgr.h
 *
 *  Created on: 2013-7-13
 *      Author: caiqingfeng
 */

#ifndef LYPROFILEMGR_H_
#define LYPROFILEMGR_H_

#include "LYHoldemProfile.h"
#include <map>
#include <string>

class LYProfileMgr {
protected:
	std::map<std::string, LYHoldemProfilePtr> profiles;

public:
	LYProfileMgr();
	virtual ~LYProfileMgr();

public:
	LYHoldemProfilePtr getHoldemProfileById(const std::string &prof_id);
	std::map<std::string, LYHoldemProfilePtr> &getAllHoldemProfiles() { return profiles; };
	bool isValidBuyin(const std::string& profile, unsigned int chipsAtHand, unsigned int buyin); //买入是否在允许范围内
};

#endif /* LYPROFILEMGR_H_ */
