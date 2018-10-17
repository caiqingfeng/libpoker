/*
 * LYHandOdds.cpp
 *
 *  Created on: 2013-6-5
 *      Author: caiqingfeng
 */

//#include <boost/foreach.hpp>
#include "LYHandOdds.h"

LYHandOdds::LYHandOdds(std::vector<LYCard > const &cds, LYHoldemAlgorithmDelegate *had) :
	LYHandStrength(cds, had)
{
	// TODO Auto-generated constructor stub
	std::vector<LYCard>::const_iterator it = cds.begin();
	for (; it != cds.end(); it++) {
		LYCard card = *it;
		this->putCardIntoMap(card);
	}
	clean_dup_straight();
	clean_dup_fullhouse();
}

LYHandOdds::~LYHandOdds() {
	// TODO Auto-generated destructor stub
}

unsigned int LYHandOdds::flush_base()
{
	std::string indexs[] = {"FluSpades", "FluHearts", "FluDiamonds", "FluClubs"};
	unsigned int baseCount = 0;
	for (unsigned int i=0; i<4; i++) {
		std::string idx = indexs[i];
//		LY_LOG_DBG("in flush_base: idx=" << idx);
		std::map<std::string, std::vector<LYCard> >::iterator it = oddsMap.find(idx);
		if (it != oddsMap.end() && (oddsMap[idx].size() > baseCount)) {
			baseCount = oddsMap[idx].size();
		}
	}
	return baseCount;
}

void LYHandOdds::clean_dup_straight()
{
	for (unsigned int face = 1; face <= 10; face++) {
		std::string idx = "Str*2*";
		if (face == 1) {
			idx = "StrA25";
		} else {
			idx[3] = g_faces[face];
			idx[5] = g_faces[face+4];
		}
		std::map<std::string, std::vector<LYCard> >::iterator it = oddsMap.find(idx);
		if (it == oddsMap.end()) {
			continue;
		}
		//drop duplicated faces
		std::vector<LYCard> newcards;
		std::vector<LYCard>::iterator it2 = oddsMap[idx].begin();
		for (; it2 != oddsMap[idx].end(); it2++) {
			LYCard cd = *it2;
			bool dup = false;
			std::vector<LYCard>::iterator it2 = newcards.begin();
			for (; it2 != newcards.end(); it2++) {
				LYCard cd2 = *it2;
				if (cd2 != cd && cd2.face == cd.face) {
					dup = true;
					break;
				}
			}
			if (!dup) {
				newcards.push_back(cd);
			}
		}
		oddsMap[idx] = newcards;
	}
}

void LYHandOdds::clean_dup_fullhouse()
{
//	LY_LOG_DBG("enter clean_dup_fullhouse");
	char first, second;
	for (int i = 0; i<15; i++)
	{
		first = g_faces[i];
		if (first == 'X' || first == 'Y') continue;
		for (int j = 0; j<15; j++)
		{
			second = g_faces[j];
			if (second == 'X' || second == 'Y') continue;
			std::string idx = "Ful**";
			idx[3] = first;
			idx[4] = second;
			std::map<std::string, std::vector<LYCard> >::iterator it = oddsMap.find(idx);
			if (it == oddsMap.end()) {
				continue;
			}
			//FulAK should have two As and one K, delete one A with two Ks
//			LY_LOG_DBG("cleanning ..." << idx << ",size=" << oddsMap[idx].size());
			if (oddsMap[idx].size() < 4) {
				oddsMap.erase(it);
				continue;
			}
			unsigned int firstCount = 0;
			unsigned int secondCount = 0;
			std::vector<LYCard>::iterator it2 = oddsMap[idx].begin();
			for (; it2 != oddsMap[idx].end(); it2++) {
				LYCard cd = *it2;
				if (cd.getFaceSymbol() == first) {
					firstCount ++;
				} else if (cd.getFaceSymbol() == second) {
					secondCount ++;
				}
			}
//			LY_LOG_DBG("firstCount=" << firstCount << ",secondCount=" << secondCount);
			if (firstCount != 3) {
				oddsMap.erase(it);
			}
		}
	}
}

unsigned int LYHandOdds::straight_base()
{
	unsigned int baseCount = 0;
	for (unsigned int face = 1; face <= 10; face++) {
		std::string idx = "Str*2*";
		if (face == 1) {
			idx = "StrA25";
		} else {
			idx[3] = g_faces[face];
			idx[5] = g_faces[face+4];
		}
		std::map<std::string, std::vector<LYCard> >::iterator it = oddsMap.find(idx);
		if (it == oddsMap.end()) {
			continue;
		}
		if (oddsMap[idx].size() > baseCount) {
			baseCount = oddsMap[idx].size();
		}
	}
	return baseCount;
}

void LYHandOdds::putCardIntoMap(const LYCard &cd)
{
	std::vector<std::string> indexs = genIndex(cd);
//	LY_LOG_DBG("card = " << cd.face << cd.suit);
	std::vector<std::string>::iterator it = indexs.begin();
	for (; it != indexs.end(); it++) {
		std::string idx = *it;
//		LY_LOG_DBG("idx=" << idx);
		std::map<std::string, std::vector<LYCard> >::iterator it = oddsMap.find(idx);
		if (it != oddsMap.end()) {
//			LY_LOG_DBG("added one card, already have " << ((std::vector<LYCard>)it->second).size());
//			BOOST_FOREACH(LYCard kd, ((std::vector<LYCard>)it->second)) {
//				LY_LOG_DBG("card=" << kd.toString());
//			}
			oddsMap[idx].push_back(cd);
		} else {
//			LY_LOG_DBG("added first card");
			std::vector<LYCard> cards;
			cards.push_back(cd);
			oddsMap[idx] = cards;
		}
	}
}

std::vector<LYCard> LYHandOdds::get_odds(std::string idx)
{
	std::map<std::string, std::vector<LYCard> >::iterator it = oddsMap.find(idx);
	if (it != oddsMap.end()) {
		return oddsMap[idx];
	}
	return std::vector<LYCard>();
}

std::vector<std::string> LYHandOdds::genIndex(const LYCard &cd)
{
	std::vector<std::string> indexs;
	switch (cd.suit)
	{
	case Spades:
		indexs.push_back("FluSpades");
		break;
	case Hearts:
		indexs.push_back("FluHearts");
		break;
	case Diamonds:
		indexs.push_back("FluDiamonds");
		break;
	case Clubs:
		indexs.push_back("FluClubs");
		break;
	default:
		break;
	}
	//set/quad/onepair
	std::string setStr = "Set*";
	setStr[3] = cd.getFaceSymbol();
	indexs.push_back(setStr);
	std::string quadStr = "Qua*";
	quadStr[3] = cd.getFaceSymbol();
	indexs.push_back(quadStr);
	std::string onePairStr = "1pr*";
	onePairStr[3] = cd.getFaceSymbol();
	indexs.push_back(onePairStr);

	//2pairs
	for (unsigned int face = 2; face <= 14; face++) {
		std::string twoPair = "2pr**";
		if (face > (unsigned int)cd.face) {
			twoPair[3] = g_faces[face];
			twoPair[4] = cd.getFaceSymbol();
		} else if (face < (unsigned int)cd.face) {
			twoPair[3] = cd.getFaceSymbol();
			twoPair[4] = g_faces[face];
		} else {
			continue;
		}
		indexs.push_back(twoPair);
	}

	//full house
	for (unsigned int face = 2; face <= 14; face++) {
		if (face == (unsigned int)cd.face) {
			continue;
		}
		std::string fullHouse1 = "Ful**";
		fullHouse1[3] = g_faces[face];
		fullHouse1[4] = cd.getFaceSymbol();
		std::string fullHouse2 = "Ful**";
		fullHouse2[3] = cd.getFaceSymbol();
		fullHouse2[4] = g_faces[face];
		indexs.push_back(fullHouse1);
		indexs.push_back(fullHouse2);
	}

	//straight and straight flush
	if ((unsigned int)cd.face <= 5) {
		std::string straight = "StrA25";
		std::string straightFlush = "SnF*A25";
		straightFlush[3] = cd.getSuitSymbol();
		indexs.push_back(straight);
		indexs.push_back(straightFlush);
	}
	if ((unsigned int)cd.face == 14) { //Ace
		std::string straight1 = "StrA25";
		std::string straight2 = "StrT2A";
		indexs.push_back(straight1);
		indexs.push_back(straight2);
		std::string straightFlush1 = "SnF*A25";
		straightFlush1[3] = cd.getSuitSymbol();
		std::string straightFlush2 = "SnF*T2A";
		straightFlush2[3] = cd.getSuitSymbol();
		indexs.push_back(straightFlush1);
		indexs.push_back(straightFlush2);
	} else {
		for (unsigned int face = 2; face <= 10; face++) {
			if (face > (unsigned int)cd.face || (unsigned int)cd.face > face+4) {
				continue;
			}
			// face<= cd.face <= face+4
			std::string straight = "Str*2*";
			straight[3] = g_faces[face];
			straight[5] = g_faces[face+4];
			std::string straightFlush = "SnF**2*";
			straightFlush[3] = cd.getSuitSymbol();
			straightFlush[4] = g_faces[face];
			straightFlush[6] = g_faces[face+4];
			indexs.push_back(straight);
			indexs.push_back(straightFlush);
		}
	}
	return indexs;
}
