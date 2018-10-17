/*
 * LYPlayer.cpp
 *
 *  Created on: 2013-7-2
 *      Author: caiqingfeng
 */

#include "LYPlayer.h"
#include <boost/date_time/posix_time/posix_time.hpp>

//const std::vector<unsigned int> daily_bonus_from_beginning = {500, 800, 1000, 1200, 1500, 3000};
//because android toolchain using c++98 standards, don't support 
unsigned int daily_bonus_from_beginning[] = {500, 800, 1000, 1200, 1500, 3000};

LYPlayer::LYPlayer(const string& uid, const string& device_id) {
	// TODO Auto-generated constructor stub
	_id = uid;
	dev_id = device_id;
	balance = 0;
	nick_name = "";
//	position = "";
	icon_url = "";
	role = ROLE_PLAYER;
	activing = false;

	//added 20140316
	last_lucky_time = 0;
	day_in_sequence = 0;
}

LYPlayer::LYPlayer() {
	// TODO Auto-generated constructor stub
	_id = "";
	dev_id = "";
	balance = 0;
	nick_name = "";
//	position = "";
	icon_url = "";
	role = ROLE_PLAYER;
	activing = false;

	//added 20140316
	last_lucky_time = 0;
	day_in_sequence = 0;
}

LYPlayer::~LYPlayer() {
	// TODO Auto-generated destructor stub
}

using namespace boost::posix_time;
bool LYPlayer::list_daily_bonus(unsigned int& seq, std::vector<unsigned int>& result)
{
	ptime previous_lucky = from_time_t(last_lucky_time);
	ptime this_lucky = from_time_t(time(NULL));

	if (last_lucky_time ==0 || this_lucky.date().day() > previous_lucky.date().day() + 1) {
		day_in_sequence = 1;
	}
	seq = day_in_sequence;
	//return daily_bonus_from_beginning;
//	std::vector<unsigned int> result;
	for (int i = 0; i < MAX_BONUS_SEQ; i ++) {
		result.push_back(daily_bonus_from_beginning[i]);
	}
	if (last_lucky_time !=0 && this_lucky.date().day() == previous_lucky.date().day()) {
		//already applied
		return false;
	}
	return true;
}

unsigned int LYPlayer::apply_daily_bonus(unsigned int& seq)
{
	ptime previous_lucky = from_time_t(last_lucky_time);
	ptime this_lucky = from_time_t(time(NULL));

	int this_day = this_lucky.date().day();
	int last_day = previous_lucky.date().day();
	if (day_in_sequence == 0 || last_lucky_time == 0 || this_day > last_day + 1) {
		day_in_sequence = 1;
		seq = day_in_sequence;
		balance += daily_bonus_from_beginning[0];
		last_lucky_time = time(NULL);
		return daily_bonus_from_beginning[0];
	}

	if (this_lucky.date().day() == previous_lucky.date().day()) {
		//already applied
		return 0;
	}

	day_in_sequence = day_in_sequence + 1;
	seq = day_in_sequence;
	last_lucky_time = time(NULL);
	if (day_in_sequence >= MAX_BONUS_SEQ) {
		balance += daily_bonus_from_beginning[MAX_BONUS_SEQ - 1];
		return daily_bonus_from_beginning[MAX_BONUS_SEQ - 1];
	}
	balance += daily_bonus_from_beginning[day_in_sequence-1];
	return daily_bonus_from_beginning[day_in_sequence-1];
}

void LYPlayer::enterTable(const string& table_id)
{
//        this->position = table_id;
//     std::vector<LYPlayerPosition>::iterator it = positions.begin();
//     for (; it!=positions.end(); it++)
}

void LYPlayer::leaveTable(const string& table_id)
{
//        this->role = ROLE_PLAYER;
//        this->position = "hall";
}

