/*
 * LYPokerProfile.cpp
 *
 *  Created on: 2013-4-10
 *      Author: caiqingfeng
 */

#include "LYPokerProfile.h"
#include <iostream>

LYPokerProfile::LYPokerProfile()
{
	_id = "888"; // random
	game_type = 1; //LYRingGame;
	small_blind = 10;
	big_blind = 25;
	min_chips = 100*small_blind;
	max_chips = 200*big_blind;
	entry_fee = 0;
	service_charge = 0;
	addon_price = 0;
	addon_chips = 0;
	break_time = 0;
	round_minutes_after_addon = 0;
	round_minutes_before_addon = 0;
}

LYPokerProfile::LYPokerProfile(unsigned int small, unsigned big)
{
	_id = "888"; // random
	game_type = 1; //LYRingGame;
	small_blind = small;
	big_blind = big;
	min_chips = 100*small;
	max_chips = 200*big;
	entry_fee = 0;
	service_charge = 0;
	addon_price = 0;
	addon_chips = 0;
	break_time = 0;
	round_minutes_after_addon = 0;
	round_minutes_before_addon = 0;
}

LYPokerProfile::LYPokerProfile(const std::string &pid) {
	// TODO Auto-generated constructor stub
	entry_fee = 0;
	service_charge = 0;
	addon_price = 0;
	addon_chips = 0;
	break_time = 0;
	round_minutes_after_addon = 0;
	round_minutes_before_addon = 0;
	if (pid == "888") {
		_id = pid;
		game_type = 1; //LYRingGame;
		small_blind = 10;
		big_blind = 25;
		min_chips = 1000;
		max_chips = 5000;
	} else {
		_id = pid;
		game_type = 0;
		small_blind = 0;
		big_blind = 0;
		min_chips = 0;
		max_chips = 0;
	}
}

LYPokerProfile::~LYPokerProfile() {
	// TODO Auto-generated destructor stub
}

std::string& LYPokerProfile::get_id()
{
    return _id;
}

unsigned int LYPokerProfile::get_big_blind()//
{
	return big_blind;
}

unsigned int LYPokerProfile::get_small_blind() //
{
	return small_blind;
}

unsigned int LYPokerProfile::get_min_chips()//
{
	return min_chips;
}

unsigned int LYPokerProfile::get_max_chips() //
{
	return max_chips;
}

bool LYPokerProfile::isValidBuyin(unsigned int chipsAtHand, unsigned int buyin) //买入是否在允许范围内
{
	//必须买入100BB的整数倍
	if ((buyin % 100*big_blind) > 0) {
		return false;
	}
	unsigned int totalAmount = chipsAtHand + buyin;
	return (totalAmount <= max_chips && totalAmount >= min_chips);
}

std::string LYPokerProfile::toString()
{
	std::string str = "";
	str = "profile:" + _id +
			",min_chips:" + std::to_string(min_chips) +
			",max_chips:" + std::to_string(max_chips) +
			",small_blind:" + std::to_string(small_blind) +
			",big_blind:" + std::to_string(big_blind);
	return str;
}