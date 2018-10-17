/*
 * LYHoldemProfile.h
 *
 *  Created on: 2013-4-10
 *      Author: caiqingfeng
 */

#ifndef LY_POKER_PROFILE_H_
#define LY_POKER_PROFILE_H_

#include <memory>
#include "poker/src/LYPokerConstants.h"

//注：20130705 现在是写死的Profile，要改成从数据库里读
//应该改成一个Profile包括多个Round
//RingGame就只有一个Round
//SNG/MTT有多个Round，每个Round的Small/Big都可能变化
class LYPokerProfile {
public:
	LYPokerProfile();
	LYPokerProfile(const std::string &pid);
	LYPokerProfile(unsigned int small, unsigned int big);
	virtual ~LYPokerProfile();

public: //要改成Private
	std::string _id; //not the _id stored in mongo but the name field
	unsigned int game_type; // default: 1->ringGame, 2->sng, 3->mtt
//	unsigned int timeout; //seconds of timeout, discarded 20130718
	unsigned int small_blind; //=initial blind bets if SNG or MTT
	unsigned int big_blind; //=	initial blind bets if SNG or MTT
	unsigned int min_chips; //buyin, min=max if SNG or MTT
	unsigned int max_chips;
	unsigned int service_charge;

	unsigned int entry_fee;
	unsigned int addon_price; //mtt only
	unsigned int addon_chips; //mtt only
	unsigned int round_minutes_before_addon; //minutes of round before addon (sng, mtt)
	unsigned int round_minutes_after_addon; //minutes of round after addon (mtt only)
	unsigned int break_time; //break time for addon (mtt only)
	std::string award_rule; //奖励规则

public:
	std::string& get_id();
	unsigned int get_big_blind(); //
	unsigned int get_small_blind(); //
	unsigned int get_min_chips(); //
	unsigned int get_max_chips(); //

	LYPokerProfile& operator = (const LYPokerProfile &right)  {
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

	bool isValidBuyin(unsigned int chipsAtHand, unsigned int buyin); //买入是否在允许范围内

public:
	std::string toString();
};

typedef ::std::shared_ptr<LYPokerProfile> LYPokerProfilePtr;

#endif /* LYHoldemProfile_H_ */
