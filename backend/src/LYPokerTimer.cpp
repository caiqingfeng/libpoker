/*
 * LYPineappleTimer.cpp
 *
 *  Created on: 2015-1-27
 *      Author: caiqingfeng
 */

#include "LYPokerTimer.h"
#include "ServerHall.h"

LYPokerTimer::LYPokerTimer() {
	// TODO Auto-generated constructor stub

}

LYPokerTimer::~LYPokerTimer() {
	// TODO Auto-generated destructor stub
}

void LYPokerTimer::clearTimersExceptCloseTable()
{
	std::pair<std::string, LYTimerPtr> closeTable;
	std::map<std::string, LYTimerPtr>::iterator it2 = this->tableTimers.begin();
	for (; it2 != this->tableTimers.end(); it2++) {
		LYTimerPtr t = it2->second;
		if (t->get_second_client_timer_id() == TIMER_ID_CLOSE_TABLE){
			closeTable = *it2;
			break;
		}
	}
	this->tableTimers.clear();
	this->tableTimers[closeTable.first] = closeTable.second;
}
