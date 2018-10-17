/*
 * LYPlayerMgr.cpp
 *
 *  Created on: 2013-7-2
 *      Author: caiqingfeng
 */

#include "LYPlayerMgr.h"

LYPlayerMgr::LYPlayerMgr()
{

}

LYPlayer LYPlayerMgr::fetchPlayer(const std::string &uid)
{
	LYPlayer player;
	return player;
}

void LYPlayerMgr::enterTable(const std::string& table, const std::string& uid)
{
	createOrUpdate(uid, table);
}

void LYPlayerMgr::leaveTable(const std::string& table, const std::string& uid)
{
	dropValue(uid, table);
}

std::vector<std::string> LYPlayerMgr::getAllTables(const std::string& uid)
{
	return getAllValues(uid);
}

unsigned int LYPlayerMgr::getBalance(const std::string& uid)
{
	LYPlayer player = fetchPlayer(uid);
	return player.balance;
}
