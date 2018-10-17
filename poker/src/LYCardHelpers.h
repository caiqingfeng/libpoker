//
//  LYCardHelpers.h
//
//  Created by 蔡 庆丰 on 14-10-25.
//  Copyright (c) 2013年 蔡 庆丰. All rights reserved.
//
#ifndef _LY_CARD_HELPERS_H
#define _LY_CARD_HELPERS_H

#include "LYCard.h"
#include <vector>
#include <string>

class LYCardHelpers {
public:
	static void sortCardsByFace(std::vector<LYCard>& cards);
	static void sortCardsBySuit(std::vector<LYCard>& cards);

	//一组Card，输出"As,Ah,..."
	static void cardsToString(std::vector<LYCard>& cards, std::string& cs);
	//一组Card，输出"1,5,6..."
	static void cardsToStringifiedInt(std::vector<LYCard>& cards, std::string& cs);
//	static void cardsToStringifiedJsonInt(std::vector<LYCard>& cards, std::string& cs);
	static void sortCardsToStringifiedIntByFace(std::vector<LYCard>& cards, std::string& cs);
	static void sortCardsToStringifiedIntBySuit(std::vector<LYCard>& cards, std::string& cs);

	//String to Cards
	static void stringifiedIntToCards(const std::string& cs, std::vector<LYCard>& cards);
	//"KcQs" to LYCard
	static void stringToCards(const std::string& cs, std::vector<LYCard>& cards);

	//判断是否有这个牌
	static bool hasThisFace(std::vector<LYCard>& cards, const LYCard& cd);
	static bool hasThisCard(std::vector<LYCard>& cards, const LYCard& cd);

	//
	static std::vector<LYCard> getRestCards(std::vector<LYCard>& cards);
};

#endif
