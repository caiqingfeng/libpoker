#include <gtest/gtest.h>

#include "../src/LYCardHelpers.h"
#include "src/LYCard.h"
#include "src/LYDeck.h"
#include <boost/lexical_cast.hpp>

class LYCardHelpers_tests : public ::testing::Test
{
protected:

    void SetUp()
    {
    }
    void TearDown()
    {
   }

};

TEST_F(LYCardHelpers_tests, sortFace)
{
	std::vector<LYCard> cards;
	cards.push_back(s2);
	cards.push_back(h8);
	cards.push_back(h2);
	cards.push_back(sA);
	cards.push_back(hA);

	LYCardHelpers::sortCardsByFace(cards);
	ASSERT_EQ(cards[0], sA);
	ASSERT_EQ(cards[1], hA);
	ASSERT_EQ(cards[2], h8);
	ASSERT_EQ(cards[3], s2);
	ASSERT_EQ(cards[4], h2);

	std::string str;
	LYCardHelpers::cardsToString(cards, str);
	ASSERT_EQ(str, "As,Ah,8h,2s,2h,");
	LYCardHelpers::cardsToStringifiedInt(cards, str);
	std::string res;
	res = boost::lexical_cast<std::string>(sA.toInteger());
	res = res + "," + boost::lexical_cast<std::string>(hA.toInteger());
	res = res + "," + boost::lexical_cast<std::string>(h8.toInteger());
	res = res + "," + boost::lexical_cast<std::string>(s2.toInteger());
	res = res + "," + boost::lexical_cast<std::string>(h2.toInteger()) + ",";
	ASSERT_EQ(str, res);

	std::string str2;
	LYCardHelpers::sortCardsToStringifiedIntByFace(cards, str2);
	ASSERT_EQ(str, str2);
}

TEST_F(LYCardHelpers_tests, sortSuit)
{
	std::vector<LYCard> cards;
	cards.push_back(s2);
	cards.push_back(h8);
	cards.push_back(h2);
	cards.push_back(sA);
	cards.push_back(hA);

	LYCardHelpers::sortCardsBySuit(cards);
	ASSERT_EQ(cards[0], sA);
	ASSERT_EQ(cards[1], s2);
	ASSERT_EQ(cards[2], hA);
	ASSERT_EQ(cards[3], h8);
	ASSERT_EQ(cards[4], h2);

	std::string str;
	LYCardHelpers::cardsToString(cards, str);
	ASSERT_EQ(str, "As,2s,Ah,8h,2h,");
	LYCardHelpers::cardsToStringifiedInt(cards, str);
	std::string res;
	res = boost::lexical_cast<std::string>(sA.toInteger());
	res = res + "," + boost::lexical_cast<std::string>(s2.toInteger());
	res = res + "," + boost::lexical_cast<std::string>(hA.toInteger());
	res = res + "," + boost::lexical_cast<std::string>(h8.toInteger());
	res = res + "," + boost::lexical_cast<std::string>(h2.toInteger()) + ",";
	ASSERT_EQ(str, res);

	std::string str2;
	LYCardHelpers::sortCardsToStringifiedIntBySuit(cards, str2);
	ASSERT_EQ(str, str2);

	std::vector<LYCard> newCards;
	LYCardHelpers::stringifiedIntToCards(res, newCards);
	ASSERT_EQ(newCards.size(), 5);
	LYCardHelpers::cardsToString(newCards, str);
	ASSERT_EQ(str, "As,2s,Ah,8h,2h,");

}

TEST_F(LYCardHelpers_tests, sortSuitWithGhost)
{
	std::vector<LYCard> cards;
	cards.push_back(smallGhost);
	cards.push_back(h8);
	cards.push_back(h2);
	cards.push_back(sA);
	cards.push_back(hA);

	LYCardHelpers::sortCardsBySuit(cards);
	ASSERT_EQ(cards[0], smallGhost);
	ASSERT_EQ(cards[1], sA);
	ASSERT_EQ(cards[2], hA);
	ASSERT_EQ(cards[3], h8);
	ASSERT_EQ(cards[4], h2);

	std::string str;
	LYCardHelpers::cardsToString(cards, str);
	ASSERT_EQ(str, "Xn,As,Ah,8h,2h,");
	LYCardHelpers::cardsToStringifiedInt(cards, str);
	std::string res;
	res = boost::lexical_cast<std::string>(smallGhost.toInteger());
	res = res + "," + boost::lexical_cast<std::string>(sA.toInteger());
	res = res + "," + boost::lexical_cast<std::string>(hA.toInteger());
	res = res + "," + boost::lexical_cast<std::string>(h8.toInteger());
	res = res + "," + boost::lexical_cast<std::string>(h2.toInteger()) + ",";
	ASSERT_EQ(str, res);

	std::string str2;
	LYCardHelpers::sortCardsToStringifiedIntBySuit(cards, str2);
	ASSERT_EQ(str, str2);

	std::vector<LYCard> newCards;
	LYCardHelpers::stringifiedIntToCards(res, newCards);
	ASSERT_EQ(newCards.size(), 5);
	LYCardHelpers::cardsToString(newCards, str);
	ASSERT_EQ(str, "Xn,As,Ah,8h,2h,");

	cards.clear();
	str = "[Xn,As,Ah,8h,2h,]";
	LYCardHelpers::stringToCards(str, cards);
	ASSERT_EQ(cards.size(), 5);
	LYCardHelpers::cardsToStringifiedInt(cards, str);
//	std::cout << str << std::endl;
//	str = "53,1,2,26,50,";
	ASSERT_EQ(str, "53,1,2,26,50,");

	str = "[53,1,2,26,50,]";
	cards.clear();
	LYCardHelpers::stringifiedIntToCards(str, cards);
	ASSERT_EQ(cards.size(), 5);
	LYCardHelpers::cardsToStringifiedInt(cards, str);
	ASSERT_EQ(str, "53,1,2,26,50,");

	str = "[]";
	cards.clear();
	LYCardHelpers::stringifiedIntToCards(str, cards);
	ASSERT_EQ(cards.size(), 0);
	LYCardHelpers::cardsToStringifiedInt(cards, str);
	ASSERT_EQ(str, "");
}

TEST_F(LYCardHelpers_tests, stringToCards)
{
	std::vector<LYCard> cards;
	cards.push_back(h8);
	cards.push_back(h2);
	cards.push_back(sA);
	cards.push_back(hA);

	std::string str;
	LYCardHelpers::sortCardsBySuit(cards);
	LYCardHelpers::cardsToString(cards, str);
	ASSERT_EQ(str, "As,Ah,8h,2h,");

	cards.clear();
	LYCardHelpers::stringToCards(str, cards);
	ASSERT_EQ(cards.size(), 4);
	ASSERT_EQ(cards[0], sA);

	cards.clear();
	str = "[As,Ah,8h,2h,]";
	LYCardHelpers::stringToCards(str, cards);
	ASSERT_EQ(cards.size(), 4);
	ASSERT_EQ(cards[0], sA);
}

TEST_F(LYCardHelpers_tests, getRestCards)
{
	std::vector<LYCard> cards;
	cards.push_back(h8);
	cards.push_back(h2);
	cards.push_back(sA);
	cards.push_back(hA);

	std::vector<LYCard> restCards = LYCardHelpers::getRestCards(cards);
	ASSERT_EQ(restCards.size(), 52-4);
	ASSERT_EQ(LYCardHelpers::hasThisCard(restCards, h8), false);
}
