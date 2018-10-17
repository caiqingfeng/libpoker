//
//  LYCardHelpers.cpp
//
//  Created by 蔡 庆丰 on 14-10-25.
//  Copyright (c) 2013年 蔡 庆丰. All rights reserved.
//

#include <vector>
#include "LYCardHelpers.h"
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include "LYDeck.h"
#include <iostream>

void LYCardHelpers::sortCardsByFace(std::vector<LYCard>& cards)
{
	std::vector<LYCard> sortedCards;
	std::vector<LYCard>::iterator it = cards.begin();
	for (; it!=cards.end(); it++) {
		LYCard card = *it;
		if (sortedCards.size() < 1) {
			sortedCards.push_back(card);
		} else {
			std::vector<LYCard>::iterator it2 = sortedCards.begin();
			for (; it2!=sortedCards.end(); it2++) {
				if (card < (*it2)) continue;
				break;
			}
			sortedCards.insert(it2, card);
		}
	}

	cards = sortedCards;
}

void LYCardHelpers::sortCardsBySuit(std::vector<LYCard>& cards)
{
	std::vector<LYCard> sortedCards, spadeCards, heartCards, diamondCards, clubCards, ghostCards;
	std::vector<LYCard>::iterator it = cards.begin();
	for (; it!=cards.end(); it++) {
		LYCard card = *it;
		switch (card.suit)
		{
			case Spades:
				spadeCards.push_back(card);
				break;
			case Hearts:
				heartCards.push_back(card);
				break;
			case Diamonds:
				diamondCards.push_back(card);
				break;
			case Clubs:
				clubCards.push_back(card);
				break;
			case Nosuit:
				ghostCards.push_back(card);
				break;
			default:
				break;
		}
	}

	sortCardsByFace(ghostCards);
	sortCardsByFace(spadeCards);
	sortCardsByFace(heartCards);
	sortCardsByFace(diamondCards);
	sortCardsByFace(clubCards);
	cards.clear();
	cards.insert(cards.end(), ghostCards.begin(), ghostCards.end());
	cards.insert(cards.end(), spadeCards.begin(), spadeCards.end());
	cards.insert(cards.end(), heartCards.begin(), heartCards.end());
	cards.insert(cards.end(), diamondCards.begin(), diamondCards.end());
	cards.insert(cards.end(), clubCards.begin(), clubCards.end());
}

void LYCardHelpers::cardsToString(std::vector<LYCard>& cards, std::string& cs)
{
	cs = "";
	std::vector<LYCard>::iterator it = cards.begin();
	for (; it!=cards.end(); it++) {
		LYCard card = *it;
		cs = cs + card.toString() + ",";
	}
}

void LYCardHelpers::cardsToStringifiedInt(std::vector<LYCard>& cards, std::string& cs)
{
	cs = "";
	std::vector<LYCard>::iterator it = cards.begin();
	for (; it!=cards.end(); it++) {
		LYCard card = *it;
		cs = cs + boost::lexical_cast<std::string>(card.toInteger()) + ",";
	}
}

void LYCardHelpers::stringifiedIntToCards(const std::string& cs, std::vector<LYCard>& cards)
{
	cards.clear();
	std::vector<std::string> cds;
	boost::split(cds, cs, boost::is_any_of(","));
	std::vector<std::string>::iterator it = cds.begin();
	for (; it!=cds.end(); it++) {
		std::string str = *it;
		if (str.size() > 1 && str[0] == '[') str = str.substr(1, str.size() - 1);
		if (str.size() > 1 && str[str.size()-1] == ']') str = str.substr(0, str.size() - 2);
		if (str == "" || str[0] == '[' || str[str.size()-1] == ']') continue;
		int i = boost::lexical_cast<int>(str);
		LYCard cd(i);
		cards.push_back(cd);
	}
}

void LYCardHelpers::stringToCards(const std::string& cs, std::vector<LYCard>& cards)
{
	cards.clear();
	std::vector<std::string> cds;
	boost::split(cds, cs, boost::is_any_of(","));
	std::vector<std::string>::iterator it = cds.begin();
	for (; it!=cds.end(); it++) {
		std::string str = *it;
		if (str.size() == 3) {
			if (str[0] == '[' || str[2] == ']') {
				str = str.substr(1, 2);
			} else {
				continue;
			}
		}
//		std::cout << str << std::endl;
		if (str == "" || str.size() != 2 || str[0] == '[' || str[1] == ']') continue;
		LYCard cd(str[0], str[1]);
		cards.push_back(cd);
	}
}

bool LYCardHelpers::hasThisCard(std::vector<LYCard>& cards, const LYCard& cd)
{
    if (cards.size() == 0) return false;
	for (std::vector<LYCard>::iterator it=cards.begin(); it!=cards.end(); ++it) {
		if (cd == (*it)) {
			return true;
		}
	}
    return false;
}

bool LYCardHelpers::hasThisFace(std::vector<LYCard>& cards, const LYCard& cd)
{
    if (cards.size() == 0) return false;
	for (std::vector<LYCard>::iterator it=cards.begin(); it!=cards.end(); ++it) {
		if (cd.face == (*it).face) {
			return true;
		}
	}
    return false;
}

void LYCardHelpers::sortCardsToStringifiedIntByFace(std::vector<LYCard>& cards, std::string& cs)
{
	sortCardsByFace(cards);
	cardsToStringifiedInt(cards, cs);
}

void LYCardHelpers::sortCardsToStringifiedIntBySuit(std::vector<LYCard>& cards, std::string& cs)
{
	sortCardsBySuit(cards);
	cardsToStringifiedInt(cards, cs);
}

std::vector<LYCard> LYCardHelpers::getRestCards(std::vector<LYCard>& cards)
{
	std::vector<LYCard> restCards;
	for (int i=1; i<53; i++) {
		if (LYCardHelpers::hasThisCard(cards, sortedDeck[i])) continue;
		restCards.push_back(sortedDeck[i]);
	}

	return restCards;
}