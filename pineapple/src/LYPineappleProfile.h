/*
 * LYPineappleProfile.h
 *
 *  Created on: 2013-4-10
 *      Author: caiqingfeng
 */

#ifndef LYPINEAPPLEPROFILE_H_
#define LYPINEAPPLEPROFILE_H_

#include <memory>
#include <string>

const unsigned int LYPINEAPPLE_NORMAL = 0; // 正常52张牌
const unsigned int LYPINEAPPLE_ONEGHOST = 100; //有一个王

/*
 *
 */
class LYPineappleProfile {
public:

public:
	LYPineappleProfile();
	LYPineappleProfile(const std::string &pid);
	virtual ~LYPineappleProfile();

public: //要改成Private
	std::string _id; //not the _id stored in mongo but the name field
	unsigned int game_type; // default: LYPINEAPPLE_NORMAL
	unsigned int entry_fee;
	unsigned int uptime; // 0无限，其它代表小时数
	unsigned int min_chips;
	unsigned int max_chips;
	unsigned int unit_price;
	unsigned int service_charge;
	unsigned int timeout;

public:
	std::string& get_id();

	LYPineappleProfile& operator = (const LYPineappleProfile &right)  {
		this->_id = right._id;
		this->game_type = right.game_type;
		this->timeout = right.timeout;
		this->min_chips = right.min_chips;
		this->max_chips = right.max_chips;
		this->unit_price = right.unit_price;
		this->service_charge = right.service_charge;
		this->entry_fee = right.entry_fee;
		return *this;
	}

	bool isValidBuyin(unsigned int chips); //买入是否在允许范围内
};

typedef ::std::shared_ptr<LYPineappleProfile> LYPineappleProfilePtr;

#endif /* LYHoldemProfile_H_ */
