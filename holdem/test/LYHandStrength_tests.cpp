/*
 * LYHandOdds_tests.cpp
 *
 *  Created on: 2013-6-5
 *      Author: caiqingfeng
 */

#include <gtest/gtest.h>

#include "src/LYHandStrength.h"
#include "poker/src/LYDeck.h"
#include "poker/src/LYCardHelpers.h"
//#include "common/src/my_log.h"

class LYHandStrength_tests : public ::testing::Test
{
protected:

    void SetUp()
    {
    	LYHoldemAlgorithmDelegate had;
    	std::vector<LYCard> cards;
    	cards.push_back(sQ);
    	cards.push_back(hQ);
    	cards.push_back(h6);
//    	cards.push_back(h9);
//    	cards.push_back(hT);
    	hs1 = new LYHandStrength(cards, &had);

    	cards.clear();
    	cards.push_back(s8);
    	cards.push_back(h8);
    	cards.push_back(h7);
//    	cards.push_back(hJ);
//    	cards.push_back(hK);
    	hs2 = new LYHandStrength(cards, &had);

    }
    void TearDown()
    {
    	delete hs1;
    	delete hs2;
   }
public:
    LYHandStrength *hs1, *hs2;

};

TEST_F(LYHandStrength_tests, init)
{
	ASSERT_EQ(hs1->ranking, OnePair);
	ASSERT_EQ(hs2->ranking, OnePair);
	ASSERT_EQ(hs1->topRankCard.face, QUEEN);
	ASSERT_EQ(hs2->topRankCard.face, EIGHT);
	LYCard card(hs1->topRankCard.face, hs2->topRankCard.suit);
	ASSERT_EQ(card>hs2->topRankCard, true);
	ASSERT_EQ(*hs1>*hs2, true);
	ASSERT_EQ(hs1->hasGhost(), false);
	hs1->cards.push_back(smallGhost);
	ASSERT_EQ(hs1->hasGhost(), true);
}

TEST_F(LYHandStrength_tests, a_real_hand)
{
    LYHoldemAlgorithmDelegate had;
    std::vector<LYCard> cards;
    LYCardHelpers::stringifiedIntToCards("34,31,40,16,44,2,8", cards);
    LYHandStrength hand1(cards, &had);
//    std::cout << hand1.cardString << std::endl;
    cards.clear();
    LYCardHelpers::stringifiedIntToCards("34,31,40,16,44,3,38", cards);
    LYHandStrength hand2(cards, &had);
//    std::cout << hand2.cardString << std::endl;
	ASSERT_EQ(hand1 < hand2, true);
}
