/*
 * LYPineappleAction.cpp
 *
 *  Created on: 2014-9-24
 *      Author: caiqingfeng
 */

#include "LYPineappleAction.h"
#include <sstream>
#include "poker/src/LYCardHelpers.h"
#include <boost/algorithm/string.hpp>
#include <iostream>

LYPineappleAction::LYPineappleAction() {
	// TODO Auto-generated constructor stub
	applicant = LYEMPTYSEAT;
	verb = LYCMD_RESET;
	bricks.clear();
}
//bricks:"0:8,0:22,0:40,0:34,0:45"
LYPineappleAction::LYPineappleAction(const std::string& table_id,
		unsigned int applicant, const std::string& bricks, unsigned int verb, const std::string& uid) {
	// TODO Auto-generated constructor stub
	this->applicant = applicant;
	this->verb = verb;
	this->table_id = table_id;
	this->bricks.clear();
	this->player = uid;
	const char *d = ",";
	const char *d1 = ":";
	char *p;
	std::vector <char*> splited_bricks;
	p = strtok(const_cast<char*>(bricks.c_str()), d);
	while (p) {
		splited_bricks.push_back(p);
		p = strtok(NULL, d);
	}
	for (int j = 0; j < splited_bricks.size(); ++j) {
		char *p1;
		p1 = strtok(splited_bricks[j], d1);
		std::vector <std::string> brick;
		int i = 0;
		while (p1) {
			std::string s(p1);
			brick.push_back(s);
			i += 1;
			p1 = strtok(NULL, d1);
		}
		int brick_place = atoi(brick[0].c_str());
		int brick_value = atoi(brick[1].c_str());
		LYPineappleBrick temp((unsigned int) brick_place,
				(unsigned int) brick_value);
		this->bricks.push_back(temp);
	}
}

LYPineappleAction::~LYPineappleAction() {
	// TODO Auto-generated destructor stub
}

std::string LYPineappleAction::toString() {
	std::string str;
	std::ostringstream ios;
	ios << "table:" << table_id << std::endl;
	ios << "applicant:" << applicant << std::endl;
	ios << "player:" << player << std::endl;
	ios << "verb:" << verb << std::endl;
	std::vector<LYPineappleBrick>::iterator it = bricks.begin();
	for (; it != bricks.end(); it++) {
		LYPineappleBrick bk = *it;
		ios << "briks:" << bk.toString() << std::endl;
	}
	str = ios.str();
	return str;
}

void LYPineappleAction::setBricksByCards(std::vector<LYCard> &cds) {
	std::vector<LYCard>::iterator it = cds.begin();
	for (; it != cds.end(); it++) {
		LYPineappleBrick bk(LY_UNDEFINED_POSITION, *it);
		bricks.push_back(bk);
	}
}

void LYPineappleAction::issueFirstHand(std::vector<LYCard> &cds,
		const std::string& table, const std::string& uid, unsigned int seat) {
	table_id = table;
	applicant = seat;
	verb = LYCMD_ISSUE_FIRST_HAND;
	player = uid;
	setBricksByCards(cds);
}

void LYPineappleAction::issueSecondHand(std::vector<LYCard> &cds,
		const std::string& table, const std::string& uid, unsigned int seat) {
	table_id = table;
	applicant = seat;
	verb = LYCMD_ISSUE_SECOND_HAND;
	player = uid;
	setBricksByCards(cds);
}

void LYPineappleAction::issueThirdHand(std::vector<LYCard> &cds,
		const std::string& table, const std::string& uid, unsigned int seat) {
	table_id = table;
	applicant = seat;
	verb = LYCMD_ISSUE_THIRD_HAND;
	player = uid;
	setBricksByCards(cds);
}

void LYPineappleAction::issueFouthHand(std::vector<LYCard> &cds,
		const std::string& table, const std::string& uid, unsigned int seat) {
	table_id = table;
	player = uid;
	applicant = seat;
	verb = LYCMD_ISSUE_FOUTH_HAND;
	setBricksByCards(cds);
}

void LYPineappleAction::issueLastHand(std::vector<LYCard> &cds,
		const std::string& table, const std::string& uid, unsigned int seat) {
	table_id = table;
	player = uid;
	applicant = seat;
	verb = LYCMD_ISSUE_LAST_HAND;
	setBricksByCards(cds);
}

void LYPineappleAction::pollingDecision(const std::string& table,
		unsigned int st) {
	table_id = table;
	verb = LYCMD_POLLING_DECISION;
	applicant = st;
	bricks.clear();
}

void LYPineappleAction::pinup(const std::string& table, unsigned int st,
		const std::vector<LYPineappleBrick>& bks) {
	table_id = table;
	verb = LYCMD_PINUP;
	applicant = st;
	bricks = bks;
}

void LYPineappleAction::issueHand(std::vector<LYCard>& cds,
		const std::string& table, const std::string& uid, unsigned int seat, unsigned int round) {
	switch (round) {
	case 1:
		issueFirstHand(cds, table, uid, seat);
		break;
	case 2:
		issueSecondHand(cds, table, uid, seat);
		break;
	case 3:
		issueThirdHand(cds, table, uid, seat);
		break;
	case 4:
		issueFouthHand(cds, table, uid, seat);
		break;
	case 5:
		issueLastHand(cds, table, uid, seat);
		break;
	default:
		break;
	}
}

std::string LYPineappleAction::stringOfBricks() {
	std::string sb;
	std::vector<LYPineappleBrick>::iterator it = bricks.begin();
	for (; it != bricks.end(); it++) {
		sb = sb + (*it).toString() + ",";
	}
	return sb;
}

std::string LYPineappleAction::sortedByFaceString()
{
	std::string sb;
	std::vector<LYCard> cards;
	std::vector<LYPineappleBrick>::iterator it = bricks.begin();
	for (; it!=bricks.end(); it++) {
		cards.push_back((*it).card);
	}
	LYCardHelpers::sortCardsByFace(cards);
	LYCardHelpers::cardsToStringifiedInt(cards, sb);
	return sb;
}

std::string LYPineappleAction::sortedBySuitString()
{
	std::string sb;
	std::vector<LYCard> cards;
	std::vector<LYPineappleBrick>::iterator it = bricks.begin();
	for (; it!=bricks.end(); it++) {
		cards.push_back((*it).card);
	}
	LYCardHelpers::sortCardsBySuit(cards);
	LYCardHelpers::cardsToStringifiedInt(cards, sb);
	return sb;
}
