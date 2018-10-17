/*
 * LYHoldemAction.h
 *
 *  Created on: 2013-7-4
 *      Author: caiqingfeng
 */

#ifndef LYHOLDEMACTION_H_
#define LYHOLDEMACTION_H_

#include <vector>
#include <string>
#include "poker/src/LYPokerConstants.h"
#include "LYHoldemConstants.h"
#include "poker/src/LYCard.h"

class LYHoldemAction {
public:
	LYHoldemAction(const std::string& tid="");
	LYHoldemAction(std::vector<std::pair<std::string, std::string> >& kvps);
	virtual ~LYHoldemAction();
	void reset();
	LYHoldemAction& operator = (const LYHoldemAction& right);
	bool operator == (LYHoldemAction& right);

public:
	std::string table_id;
	enum LYApplicant applicant;
	enum LYVerb verb;
	int bet; //bet/call/raise时与game->betIfCall相等，polling时为该位置需要投入的筹码
	std::string desc;  //used for fold reason, 20130516, 20160323 added, for pot level, 20160429 added, for assurance summary table income
	std::vector<LYCard> cards;

public: //20130704，目前把Action精简，下一步是否还可以把发牌这些动作移出来？
	void issueFlopCards(const std::vector<LYCard> &cds);
	void issueTurnCard(const LYCard &cd);
	void issueRiverCard(const LYCard &cd);
	void pollingDecision(enum LYApplicant player, unsigned int chips);
	void fold(enum LYApplicant st, const std::string &reason ="byuser");
	void check(enum LYApplicant st);
	void allin(enum LYApplicant st, unsigned int chips=0);
	void call(enum LYApplicant st, unsigned int chips);
	void doBet(enum LYApplicant st, unsigned int chips);
	void betSmallBlind(enum LYApplicant st, unsigned int chips);
	void betBigBlind(enum LYApplicant st, unsigned int chips);
	void raise(enum LYApplicant st, unsigned int chips);
	void assure(enum LYApplicant st, unsigned int chips, const std::string pot="1");
	void askAssure(enum LYApplicant st, unsigned int chips, const std::vector<LYCard>& outs, const std::string pot="1");
	void payAssure(enum LYApplicant st, unsigned int chips, const std::string pot="1");
	void summarizeAssure(int chips, int total_income);
	bool isAssuranceRelevant();

public:
	std::string toString();
	//discarded 20160701
	std::vector<std::pair<std::string, std::string> > to_kvps();

public:
	static LYHoldemAction build_from_action_string(std::string& action_str);
	bool isPolling();
};

#endif /* LYHOLDEMACTION_H_ */
