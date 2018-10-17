/*
 * LYHall.cpp
 *
 *  Created on: 2013-6-27
 *      Author: caiqingfeng
 */

//#include "common/src/zhelpers.hpp"
#include "LYHall.h"
#include <iostream>


LYHall::LYHall(LYPlayerMgr *pm) : playerMgr(pm){
	// TODO Auto-generated constructor stub
//	LY_LOG_DBG("constructing LYHall ...");
	reset();
}

LYHall::~LYHall() {
	// TODO Auto-generated destructor stub
}

void LYHall::createTable(const string &table_id, const unsigned int ts,
		enum LYTableStatus sta, const string &tbl_name)
{
	LYTablePtr table = LYTablePtr(new LYTable(table_id, ts, sta, tbl_name));
	tables[table_id] = table;
}

void LYHall::createTable(const string &table_id, LYTablePtr tbl)
{
	tables[table_id] = tbl;
}

void LYHall::closeTable(const string &table_id)
{
	tables.erase(table_id);
}

//string LYHall::getOwnerUid(const string& table_id)
//{
//    if (this->haveTable(table_id))
//    {
//        return tables[table_id]->get_owner_uid();
//    }
//
//    return "";
//}
//

void LYHall::reset()
{
	tables.clear();
}

std::vector<LYTablePtr> LYHall::fetchTablesByName(const std::string &name)
{
	std::vector<LYTablePtr> result;
	std::map<std::string, LYTablePtr >::iterator it = tables.begin();
	for (; it != tables.end(); it++) {
		LYTablePtr table = it->second;
		if (table->isThisName(name)) {
			result.push_back(table);
		}
	}
	return result;
}

LYTablePtr LYHall::fetchTableById(const std::string &tid)
{
	LYTablePtr table;
	if (tables.size() == 0) return table;
	std::map<std::string, LYTablePtr >::iterator it = tables.find(tid);
	if (it != tables.end()) {
		table = it->second;
	}
	return table;
}

