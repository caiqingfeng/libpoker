/*
 * LYPineappleProfileMgr.h
 *
 *  Created on: 2014-9-13
 *      Author: caiqingfeng
 */

#ifndef LYPINEAPPLEPROFILEMGR_H_
#define LYPINEAPPLEPROFILEMGR_H_

#include "LYPineappleProfile.h"
#include <map>
#include <string>
#include <vector>

class LYPineappleProfileMgr {
protected:
	std::map<std::string, LYPineappleProfilePtr> profiles;

public:
	LYPineappleProfileMgr();
	virtual ~LYPineappleProfileMgr();

public:
	LYPineappleProfilePtr getPineappleProfileById(const std::string &prof_id);
	std::map<std::string, LYPineappleProfilePtr> &getAllPineappleProfiles() { return profiles; };
	void listAllProfiles(std::vector<LYPineappleProfilePtr>& prof_list);
	unsigned int getGameType(const std::string& prof);
	unsigned int getUnitPrice(const std::string& prof);
	unsigned int getMinChips(const std::string& prof);
	void addProfile(LYPineappleProfilePtr p);

public:
	bool canTakeSeat(const std::string& prof, unsigned int chips);
};

#endif /* LYPINEAPPLEPROFILEMGR_H_ */
