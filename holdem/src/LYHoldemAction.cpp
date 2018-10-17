/*
 * LYHoldemAction.cpp
 *
 *  Created on: 2013-7-4
 *      Author: caiqingfeng
 */

#include "LYHoldemAction.h"
#include "poker/src/LYCardHelpers.h"
#include <boost/algorithm/string.hpp>
#include <iostream>

void cleanQuote(std::string& s)
{
    if (s.size() < 1) return;
    if (s[0] == '\"') s = s.substr(1, s.size()-1);
    if (s[s.size()-1] == '\"') s = s.substr(0, s.size()-1);
}

LYHoldemAction::LYHoldemAction(const std::string& tid) {
	// TODO Auto-generated constructor stub
    reset();

	table_id = tid;
}

void LYHoldemAction::reset() {
	// TODO Auto-generated constructor stub
	table_id = "";
	bet = 0;
	applicant = LYHouse;
	verb = LYHousePollingDecision;
	desc = "";
	cards.clear();
}

LYHoldemAction::LYHoldemAction(std::vector<std::pair<std::string, std::string> >& kvps)
{
    std::vector<std::pair<std::string, std::string> >::iterator it = kvps.begin();
    for (; it!=kvps.end(); it++) {
        std::pair<std::string, std::string> kvp = *it;
		if (kvp.first == "table_id" || kvp.first == "\"table_id\"") {
			this->table_id = kvp.second;
			cleanQuote(this->table_id);
		} else if (kvp.first == "applicant" || kvp.first == "\"applicant\"") {
			this->applicant = (LYApplicant)std::stoi(
					kvp.second);
		} else if (kvp.first == "verb" || kvp.first == "\"verb\"") {
			this->verb = (LYVerb)std::stoi(
					kvp.second);
		} else if (kvp.first == "bet" || kvp.first == "\"bet\"") {
			this->bet = std::stoi(
					kvp.second);
		} else if (kvp.first == "desc" || kvp.first == "\"desc\"") {
			this->desc = kvp.second;
			cleanQuote(this->desc);
		} else if (kvp.first == "cards" || kvp.first == "\"cards\"") {
//		std::cout << "cards:" << kvp.second << std::endl;
			cleanQuote(kvp.second);
		    LYCardHelpers::stringToCards(kvp.second, this->cards);
		}
    }
}

LYHoldemAction::~LYHoldemAction() {
	// TODO Auto-generated destructor stub
}

LYHoldemAction& LYHoldemAction::operator = (const LYHoldemAction& right)
{
    this->table_id = right.table_id;
    this->applicant = right.applicant;
    this->verb = right.verb;
    this->bet = right.bet;
    this->desc = right.desc;
    this->cards = right.cards;
    return *this;
}

bool LYHoldemAction::operator == (LYHoldemAction& right)
{
    return (this->table_id == right.table_id &&
        this->applicant == right.applicant &&
        this->verb == right.verb &&
        this->bet == right.bet &&
        this->desc == right.desc &&
        this->cards.size() == right.cards.size());
}


std::string LYHoldemAction::toString()
{
    std::string cardsStr;
    LYCardHelpers::cardsToString(cards, cardsStr);
	std::string str = "";
	cleanQuote(this->table_id);
	cleanQuote(this->desc);
	cleanQuote(cardsStr);
	str = "\"table_id\":\"" + table_id + "\"," +
	        "\"applicant\":" + std::to_string(applicant) + "," +
	        "\"verb\":" + std::to_string(verb) + "," +
	        "\"bet\":" + std::to_string(bet) + "," +
	        "\"desc\":\"" + desc + "\"," +
	        "\"cards\":\"" + cardsStr + "\"";
	return str;
}

LYHoldemAction LYHoldemAction::build_from_action_string(std::string& action_str)
{
    std::vector<std::pair<std::string, std::string>> actions_vector;

    std::vector<std::string> actions;
        // 对cards要特殊处理
    int i = action_str.find("cards");
    std::string s = action_str.substr(0, i);
        // boost::is_any_of这里相当于分割规则了
    boost::split(actions, s, boost::is_any_of(","));
    std::vector<std::string>::iterator it = actions.begin();
    for (; it!=actions.end(); it++) {
        std::vector<std::string> v;
        std::string s = *it;
        boost::split(v, s, boost::is_any_of(":"));
        if (v.size() != 2) continue;
        std::pair<std::string, std::string> kvp;
        kvp.first = v[0];
        cleanQuote(kvp.first);
//        std::cout << kvp.first << std::endl;
        kvp.second = v[1];
        cleanQuote(kvp.second);
//        std::cout << kvp.second << std::endl;
        actions_vector.push_back(kvp);
    }

    s = action_str.substr(i, action_str.size() - i);
//        std::cout << s << std::endl;
    std::vector<std::string> v;
    boost::split(v, s, boost::is_any_of(":"));
    if (v.size() == 2) {
        std::pair<std::string, std::string> kvp;
        kvp.first = v[0];
        cleanQuote(kvp.first);
//        std::cout << kvp.first << std::endl;
        kvp.second = v[1];
        cleanQuote(kvp.second);
//        std::cout << kvp.second << std::endl;
        actions_vector.push_back(kvp);
    }

    LYHoldemAction action(actions_vector);
    return action;
}

void LYHoldemAction::issueFlopCards(const std::vector<LYCard> &cds)
{
    verb = LYHouseIssueFlop;
    cards = cds;
    bet = 0;
    applicant = LYHouse;
    desc = "";
}

void LYHoldemAction::issueTurnCard(const LYCard &cd)
{
    cards.clear();
    verb = LYHouseIssueTurn;
    cards.push_back(cd);
    bet = 0;
    applicant = LYHouse;
    desc = "";
}

void LYHoldemAction::issueRiverCard(const LYCard &cd)
{
    cards.clear();
    verb = LYHouseIssueRiver;
    cards.push_back(cd);
    bet = 0;
    applicant = LYHouse;
    desc = "";
}

void LYHoldemAction::pollingDecision(enum LYApplicant st, unsigned int chips)
{
    verb = LYHousePollingDecision;
    applicant = st;
    bet = chips;
    desc = "";
    cards.clear();
}

void LYHoldemAction::fold(enum LYApplicant st, const std::string &reason )
{
    verb = LYFold;
    applicant = st;
    bet = 0;
    desc = reason;
    cards.clear();
}

void LYHoldemAction::check(enum LYApplicant st)
{
    verb = LYCheck;
    applicant = st;
    bet = 0;
    desc = "";
    cards.clear();
}

void LYHoldemAction::allin(enum LYApplicant st, unsigned int chips)
{
    verb = LYAllin;
    applicant = st;
    bet = chips;
    desc = "";
    cards.clear();
}

void LYHoldemAction::call(enum LYApplicant st, unsigned int chips)
{
    verb = LYCall;
    applicant = st;
    bet = chips;
    desc = "";
    cards.clear();
}

void LYHoldemAction::doBet(enum LYApplicant st, unsigned int chips)
{
    verb = LYBet;
    applicant = st;
    bet = chips;
    desc = "";
    cards.clear();
}

void LYHoldemAction::betSmallBlind(enum LYApplicant st, unsigned int chips)
{
    verb = LYBetSmallBlind;
    applicant = st;
    bet = chips;
    desc = "";
    cards.clear();
}

void LYHoldemAction::betBigBlind(enum LYApplicant st, unsigned int chips)
{
    verb = LYBetBigBlind;
    applicant = st;
    bet = chips;
    desc = "";
    cards.clear();
}

void LYHoldemAction::raise(enum LYApplicant st, unsigned int chips)
{
    verb = LYRaise;
    applicant = st;
    bet = chips;
    desc = "";
    cards.clear();
}

void LYHoldemAction::assure(enum LYApplicant st, unsigned int chips, const std::string pot)
{
    verb = LYBuyAssurance;
    applicant = st;
    bet = chips;
    desc = pot;
    cards.clear();
}

void LYHoldemAction::askAssure(enum LYApplicant st, unsigned int chips,
    const std::vector<LYCard>& outs, const std::string pot)
{
    verb = LYAskAssurance;
    applicant = st;
    bet = chips;
    desc = pot;
    cards = outs;
}

void LYHoldemAction::payAssure(enum LYApplicant st, unsigned int chips, const std::string pot)
{
    verb = LYPayAssurance;
    applicant = st;
    bet = chips;
    desc = pot;
}

void LYHoldemAction::summarizeAssure(int chips, int total_income)
{
    verb = LYSummarizeAssurance;
    applicant = LYHouse;
    bet = chips;
    desc = std::to_string(total_income);
}

bool LYHoldemAction::isAssuranceRelevant()
{
    switch (verb)
    {
    case LYSummarizeAssurance:
    case LYBuyAssurance:
    case LYAskAssurance:
    case LYPayAssurance:
        return true;
    default:
        return false;
    }
    
    return false;
}

/*
 * discarded 20160701
 */
std::vector<std::pair<std::string, std::string> > LYHoldemAction::to_kvps()
{
	std::vector<std::pair<std::string, std::string> > kvps;
	std::pair < std::string, std::string > kvp;

	kvp.first = "table_id";
	kvp.second = this->table_id;
	kvps.push_back(kvp);

	kvp.first = "applicant";
	kvp.second = std::to_string(this->applicant);
	kvps.push_back(kvp);

	kvp.first = "verb";
	kvp.second = std::to_string(this->verb);
	kvps.push_back(kvp);

	kvp.first = "bet";
	kvp.second = std::to_string(this->bet);
	kvps.push_back(kvp);

	kvp.first = "desc";
	kvp.second = this->desc;
	kvps.push_back(kvp);

    std::string cardsStr;
    LYCardHelpers::cardsToString(cards, cardsStr);
	kvp.first = "cards";
	kvp.second = cardsStr;
	kvps.push_back(kvp);

	return kvps;
}

bool LYHoldemAction::isPolling()
{
    return (verb == LYHousePollingDecision || verb == LYAskAssurance);
}