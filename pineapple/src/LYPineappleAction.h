/*
 * LYPineappleAction.h
 *
 *  Created on: 2014-7-14
 *      Author: caiqingfeng
 */

#ifndef LYPINEAPPLEACTION_H_
#define LYPINEAPPLEACTION_H_

#include <vector>
#include <string>
#include "poker/src/LYPokerConstants.h"
#include "LYPineappleConstants.h"
#include "LYPineappleBrick.h"
#include "poker/src/LYCard.h"

const unsigned int LYCMD_RESET = 0;
const unsigned int LYCMD_POLLING_DECISION = 1;
const unsigned int LYCMD_ISSUE_FIRST_HAND = 100;
const unsigned int LYCMD_ISSUE_SECOND_HAND = 101;
const unsigned int LYCMD_ISSUE_THIRD_HAND = 102;
const unsigned int LYCMD_ISSUE_FOUTH_HAND = 103;
const unsigned int LYCMD_ISSUE_LAST_HAND = 105;
const unsigned int LYCMD_PINUP = 500;

class LYPineappleAction {
public:
	LYPineappleAction();
	LYPineappleAction(const std::string& table_id, unsigned int applicant,
			const std::string& bricks, unsigned int verb=0, const std::string& uid="");
	virtual ~LYPineappleAction();
public:
	std::string table_id;
	unsigned int applicant;  //在C－S交互的类中不用enum（ LYApplicant）
	std::string player; //20141112增加，为了过滤掉牌，在Action中要增加这个字段，客户端上行不用设置，只用于在下行传送时设置
						//20150205新增功能，如果在Pinup中设置该值，并且为timeout, 则seat->timeoutCount++
	std::vector<LYPineappleBrick> bricks; //
	unsigned int verb;

private:
	void setBricksByCards(std::vector<LYCard> &cds);

public:
	//服务器发牌
	void issueHand(std::vector<LYCard>& cds, const std::string& table,
			const std::string& player, unsigned int seat, unsigned int round);
	void issueFirstHand(std::vector<LYCard> &cds, const std::string& table,
			const std::string& player, unsigned int seat);
	void issueSecondHand(std::vector<LYCard> &cds, const std::string& table,
			const std::string& player, unsigned int seat);
	void issueThirdHand(std::vector<LYCard> &cds, const std::string& table,
			const std::string& player, unsigned int seat);
	void issueFouthHand(std::vector<LYCard> &cds, const std::string& table,
			const std::string& player, unsigned int seat);
	void issueLastHand(std::vector<LYCard> &cds, const std::string& table,
			const std::string& player, unsigned int seat);
	void pollingDecision(const std::string& table, unsigned int seat);

public:
	//玩家动作只有一个，pinup
	void pinup(const std::string& table, unsigned int seat,
			const std::vector<LYPineappleBrick>& bricks);

public:
	std::string toString();
	std::string stringOfBricks();
	std::string sortedByFaceString();
	std::string sortedBySuitString();
};

#endif /* LYPINEAPPLEACTION_H_ */

