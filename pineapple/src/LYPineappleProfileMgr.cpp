#include "LYPineappleProfileMgr.h"
#include <iostream>

LYPineappleProfileMgr::LYPineappleProfileMgr()
{

}

LYPineappleProfileMgr::~LYPineappleProfileMgr()
{

}

void LYPineappleProfileMgr::addProfile(LYPineappleProfilePtr p)
{
	std::pair<std::string, LYPineappleProfilePtr> item(p->_id, p);
	profiles.insert(item);
}

unsigned int LYPineappleProfileMgr::getGameType(const std::string& prof_id)
{
	LYPineappleProfilePtr p = getPineappleProfileById(prof_id);
	if (p.get() == NULL) return LYPINEAPPLE_NORMAL;
	return p->game_type;
}

unsigned int LYPineappleProfileMgr::getUnitPrice(const std::string& prof_id)
{
	LYPineappleProfilePtr p = getPineappleProfileById(prof_id);
	if (p.get() == NULL) return 0;
	return p->unit_price;
}

unsigned int LYPineappleProfileMgr::getMinChips(const std::string& prof_id)
{
	LYPineappleProfilePtr p = getPineappleProfileById(prof_id);
	if (p.get() == NULL) return 0;
	return p->min_chips;
}

LYPineappleProfilePtr LYPineappleProfileMgr::getPineappleProfileById(const std::string &prof_id)
{
	std::map<std::string, LYPineappleProfilePtr>::iterator it = profiles.find(prof_id);
	if (it != profiles.end()) {
		return it->second;
	}
	return LYPineappleProfilePtr();
}

void LYPineappleProfileMgr::listAllProfiles(std::vector<LYPineappleProfilePtr>& prof_list)
{
	prof_list.clear();
	std::map<std::string, LYPineappleProfilePtr>::iterator it = profiles.begin();
	for (; it != profiles.end(); it++) {
		prof_list.push_back(it->second);
	}
}

bool LYPineappleProfileMgr::canTakeSeat(const std::string& prof, unsigned int chips)
{
	LYPineappleProfilePtr p = getPineappleProfileById(prof);
	if (p.get() == NULL) {
//		LY_LOG_DBG("profile not found ..." << std::string(prof));
//		std::cerr << "profile not found ..." << std::string(prof);
		return false;
	}
	return (chips >= p->min_chips);
}
