/*
 * LYServerGame.h
 *
 *  Created on: 2013-6-24
 *      Author: caiqingfeng
 */

#ifndef LYPLAYERMGR_H_
#define LYPLAYERMGR_H_

#include <map>
#include <string>
#include "LYPlayer.h"

class LYPlayerMgr {
//protected:
//	std::map<std::string, LYPlayerPtr> onlinePlayers;

public:
	LYPlayerMgr();
	virtual ~LYPlayerMgr() {};
	
public:
	virtual LYPlayer fetchPlayer(const std::string& uid);

public:
	virtual void checkIn(std::string& uid, const string& did, LYDevice& device) {};
	virtual void shakeHand() {};
	void takeSeat(const std::string& table, unsigned int seat, const std::string& uid);
	void leaveSeat(const std::string& table, unsigned int seat, const std::string& uid);
	void enterTable(const std::string& table, const std::string& uid);
	void leaveTable(const std::string& table_id, const std::string& uid);
	unsigned int getBalance(const std::string& uid);
	std::vector<std::string> getAllTables(const std::string& uid);

public:
	virtual void createOrUpdate(const std::string& key, const std::string& val) {};
	virtual void dropValue(const std::string& key, const std::string& val) {};
	virtual std::vector<std::string> getAllValues(const std::string& key) {};

};

#endif /* LYPLAYERMGR_H_ */
