/*
 * LYHoldemProfile.h
 *
 *  Created on: 2013-4-10
 *      Author: caiqingfeng
 */

#ifndef LYHOLDEMPROFILE_H_
#define LYHOLDEMPROFILE_H_

#include <memory>
#include "poker/src/LYPokerConstants.h"
#include "LYHoldemConstants.h"
#include "poker/src/LYPokerProfile.h"

//注：20130705 现在是写死的Profile，要改成从数据库里读
//应该改成一个Profile包括多个Round
//RingGame就只有一个Round
//SNG/MTT有多个Round，每个Round的Small/Big都可能变化
class LYHoldemProfile : public LYPokerProfile{
public:
	LYHoldemProfile();
	LYHoldemProfile(const std::string &pid);
	LYHoldemProfile(unsigned int small, unsigned int big);
	virtual ~LYHoldemProfile();

public:
	LYHoldemProfile& operator = (const LYHoldemProfile &right)  {
		this->_id = right._id;
		this->game_type = right.game_type;
//		this->timeout = right.timeout;
		this->small_blind = right.small_blind;
		this->big_blind = right.big_blind;
		this->min_chips = right.min_chips;
		this->max_chips = right.max_chips;
		this->service_charge = right.service_charge;
		this->entry_fee = right.entry_fee;
		this->addon_price = right.addon_price;
		this->round_minutes_before_addon = right.round_minutes_before_addon;
		this->round_minutes_after_addon = right.round_minutes_after_addon;
		this->addon_chips = right.addon_chips;
		this->break_time = right.break_time;
		this->award_rule = right.award_rule;
		return *this;
	}

};

typedef ::std::shared_ptr<LYHoldemProfile> LYHoldemProfilePtr;

#endif /* LYHoldemProfile_H_ */
