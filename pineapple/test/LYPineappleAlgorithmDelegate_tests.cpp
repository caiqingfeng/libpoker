/*
 * LYPineappleAlgorithmDelegate_tests.cpp
 *
 *  Created on: 2014-11-10
 *      Author: caiqingfeng
 */

#include <gtest/gtest.h>

#include "src/LYPineappleAlgorithmDelegate.h"
//#include "common/src/my_log.h"
#include "poker/src/LYDeck.h"
#include "holdem/src/LYHandStrength.h"

class LYPineappleAlgorithmDelegate_tests : public ::testing::Test
{
protected:

    void SetUp()
    {
    	pad = new LYPineappleAlgorithmDelegate();

    	royalFlush.push_back(hA);
    	royalFlush.push_back(hK);
    	royalFlush.push_back(hQ);
    	royalFlush.push_back(hJ);
    	royalFlush.push_back(hT);
    	royalFlushHS = new LYHandStrength(royalFlush, pad, NULL);

    	// straightHand.push_back(dA);
    	// straightHand.push_back(cK);
    	// straightHand.push_back(cQ);
    	// straightHand.push_back(hJ);
    	// straightHand.push_back(hT);
    	// straightHS = new LYHandStrength(straightHand, pad, NULL);

    	// fullhouseHand.push_back(dQ);
    	// fullhouseHand.push_back(cQ);
    	// fullhouseHand.push_back(sQ);
    	// fullhouseHand.push_back(hJ);
    	// fullhouseHand.push_back(dJ);
    	// fullhouseHS = new LYHandStrength(fullhouseHand, pad, NULL);

    	// twoPairHS.push_back(dQ);
    	// twoPairHS.push_back(cQ);
    	// twoPairHS.push_back(s2);
    	// twoPairHS.push_back(hJ);
    	// twoPairHS.push_back(dJ);
    	// twoPairHS = new LYHandStrength(twoPairHS, pad, NULL);

    	straightFlushHand.push_back(d6);
    	straightFlushHand.push_back(d7);
    	straightFlushHand.push_back(d8);
    	straightFlushHand.push_back(d9);
    	straightFlushHand.push_back(dT);
    	straightFlushHS = new LYHandStrength(straightFlushHand, pad, NULL);

    	min_straightFlushHand.push_back(dA);
    	min_straightFlushHand.push_back(d3);
    	min_straightFlushHand.push_back(d2);
    	min_straightFlushHand.push_back(d4);
    	min_straightFlushHand.push_back(d5);
    	min_straightFlushHS = new LYHandStrength(min_straightFlushHand, pad, NULL);

    	max_straightFlushHand.push_back(dK);
    	max_straightFlushHand.push_back(dQ);
    	max_straightFlushHand.push_back(dT);
    	max_straightFlushHand.push_back(dA);
    	max_straightFlushHand.push_back(dJ);
    	max_straightFlushHS = new LYHandStrength(max_straightFlushHand, pad, NULL);

    	fourOfAKindHand.push_back(s6);
    	fourOfAKindHand.push_back(d6);
    	fourOfAKindHand.push_back(c6);
    	fourOfAKindHand.push_back(h6);
    	fourOfAKindHand.push_back(dT);
    	fourOfAKindHS = new LYHandStrength(fourOfAKindHand, pad, NULL);

    	min_fourOfAKindHand.push_back(s2);
    	min_fourOfAKindHand.push_back(d2);
    	min_fourOfAKindHand.push_back(c2);
    	min_fourOfAKindHand.push_back(h2);
    	min_fourOfAKindHand.push_back(d3);
    	min_fourOfAKindHS = new LYHandStrength(min_fourOfAKindHand, pad, NULL);

    	max_fourOfAKindHand.push_back(sA);
    	max_fourOfAKindHand.push_back(dA);
    	max_fourOfAKindHand.push_back(cA);
    	max_fourOfAKindHand.push_back(hA);
    	max_fourOfAKindHand.push_back(dK);
    	max_fourOfAKindHS = new LYHandStrength(max_fourOfAKindHand, pad, NULL);

    	fullhouseHand.push_back(sQ);
    	fullhouseHand.push_back(dQ);
    	fullhouseHand.push_back(cQ);
    	fullhouseHand.push_back(h5);
    	fullhouseHand.push_back(d5);
    	fullhouseHS = new LYHandStrength(fullhouseHand, pad, NULL);

    	min_fullhouseHand.push_back(s2);
    	min_fullhouseHand.push_back(d2);
    	min_fullhouseHand.push_back(c2);
    	min_fullhouseHand.push_back(h3);
    	min_fullhouseHand.push_back(d3);
    	min_fullhouseHS = new LYHandStrength(min_fullhouseHand, pad, NULL);

    	max_fullhouseHand.push_back(sA);
    	max_fullhouseHand.push_back(dA);
    	max_fullhouseHand.push_back(cK);
    	max_fullhouseHand.push_back(hA);
    	max_fullhouseHand.push_back(dK);
    	max_fullhouseHS = new LYHandStrength(max_fullhouseHand, pad, NULL);

    	flushHand.push_back(s6);
    	flushHand.push_back(s5);
    	flushHand.push_back(s9);
    	flushHand.push_back(s3);
    	flushHand.push_back(sJ);
    	flushHS = new LYHandStrength(flushHand, pad, NULL);

    	min_flushHand.push_back(s2);
    	min_flushHand.push_back(s5);
    	min_flushHand.push_back(s4);
    	min_flushHand.push_back(s3);
    	min_flushHand.push_back(s7);
    	min_flushHS = new LYHandStrength(min_flushHand, pad, NULL);

    	max_flushHand.push_back(sA);
    	max_flushHand.push_back(sQ);
    	max_flushHand.push_back(sK);
    	max_flushHand.push_back(s9);
    	max_flushHand.push_back(sJ);
    	max_flushHS = new LYHandStrength(max_flushHand, pad, NULL);

    	straightHand.push_back(s5);
    	straightHand.push_back(d3);
    	straightHand.push_back(d4);
    	straightHand.push_back(h6);
    	straightHand.push_back(s7);
    	straightHS = new LYHandStrength(straightHand, pad, NULL);

    	min_straightHand.push_back(s2);
    	min_straightHand.push_back(d3);
    	min_straightHand.push_back(d4);
    	min_straightHand.push_back(hA);
    	min_straightHand.push_back(s5);
    	min_straightHS = new LYHandStrength(min_straightHand, pad, NULL);

    	max_straightHand.push_back(sA);
    	max_straightHand.push_back(dK);
    	max_straightHand.push_back(dQ);
    	max_straightHand.push_back(hJ);
    	max_straightHand.push_back(sT);
    	max_straightHS = new LYHandStrength(max_straightHand, pad, NULL);

    	threeOfAKindHand.push_back(s5);
    	threeOfAKindHand.push_back(d5);
    	threeOfAKindHand.push_back(d5);
    	threeOfAKindHand.push_back(h6);
    	threeOfAKindHand.push_back(s7);
    	threeOfAKindHS = new LYHandStrength(threeOfAKindHand, pad, NULL);

    	min_threeOfAKindHand.push_back(s2);
    	min_threeOfAKindHand.push_back(d2);
    	min_threeOfAKindHand.push_back(h2);
    	min_threeOfAKindHand.push_back(h3);
    	min_threeOfAKindHand.push_back(s4);
    	min_threeOfAKindHS = new LYHandStrength(min_threeOfAKindHand, pad, NULL);

		max_threeOfAKindHand.push_back(sA);
    	max_threeOfAKindHand.push_back(dA);
    	max_threeOfAKindHand.push_back(hA);
    	max_threeOfAKindHand.push_back(hK);
    	max_threeOfAKindHand.push_back(sQ);
    	max_threeOfAKindHS = new LYHandStrength(max_threeOfAKindHand, pad, NULL);


    	twoPairHand.push_back(s5);
    	twoPairHand.push_back(d5);
    	twoPairHand.push_back(d7);
    	twoPairHand.push_back(h6);
    	twoPairHand.push_back(s7);
    	twoPairHS = new LYHandStrength(twoPairHand, pad, NULL);

    	min_twoPairHand.push_back(s2);
    	min_twoPairHand.push_back(d2);
    	min_twoPairHand.push_back(d3);
    	min_twoPairHand.push_back(h3);
    	min_twoPairHand.push_back(s4);
    	min_twoPairHS = new LYHandStrength(min_twoPairHand, pad, NULL);

    	max_twoPairHand.push_back(sA);
    	max_twoPairHand.push_back(dA);
    	max_twoPairHand.push_back(dK);
    	max_twoPairHand.push_back(hK);
    	max_twoPairHand.push_back(sQ);
    	max_twoPairHS = new LYHandStrength(max_twoPairHand, pad, NULL);

    	onePairHand.push_back(s5);
    	onePairHand.push_back(d5);
    	onePairHand.push_back(d9);
    	onePairHand.push_back(h6);
    	onePairHand.push_back(s7);
    	onePairHS = new LYHandStrength(onePairHand, pad, NULL);

    	min_onePairHand.push_back(s2);
    	min_onePairHand.push_back(d2);
    	min_onePairHand.push_back(d3);
    	min_onePairHand.push_back(h4);
    	min_onePairHand.push_back(s5);
    	min_onePairHS = new LYHandStrength(min_onePairHand, pad, NULL);

    	max_onePairHand.push_back(sA);
    	max_onePairHand.push_back(dA);
    	max_onePairHand.push_back(dK);
    	max_onePairHand.push_back(hQ);
    	max_onePairHand.push_back(sJ);
    	max_onePairHS = new LYHandStrength(max_onePairHand, pad, NULL);

    	cards.clear();
    	cards.push_back(s5);
    	cards.push_back(d2);
    	cards.push_back(d9);
    	cards.push_back(h6);
    	cards.push_back(sQ);
    	highCardHS = new LYHandStrength(cards, pad, NULL);

    	cards.clear();
    	cards.push_back(s2);
    	cards.push_back(d3);
    	cards.push_back(d4);
    	cards.push_back(h5);
    	cards.push_back(s7);
    	min_highCardHS = new LYHandStrength(cards, pad, NULL);

    	cards.clear();
    	cards.push_back(sA);
    	cards.push_back(dK);
    	cards.push_back(dQ);
    	cards.push_back(hJ);
    	cards.push_back(s9);
    	max_highCardHS = new LYHandStrength(cards, pad, NULL);
    }
    void TearDown()
    {
    	delete pad;
    	delete royalFlushHS;
    	delete straightFlushHS;
    	delete min_straightFlushHS;
    	delete max_straightFlushHS;
    	delete fourOfAKindHS;
    	delete min_fourOfAKindHS;
    	delete max_fourOfAKindHS;
    	delete fullhouseHS;
    	delete min_fullhouseHS;
    	delete max_fullhouseHS;
    	delete flushHS;
    	delete min_flushHS;
    	delete max_flushHS;
    	delete straightHS;
    	delete min_straightHS;
    	delete max_straightHS;
    	delete threeOfAKindHS;
    	delete min_threeOfAKindHS;
    	delete max_threeOfAKindHS;
    	delete twoPairHS;
    	delete min_twoPairHS;
    	delete max_twoPairHS;
    	delete onePairHS;
    	delete min_onePairHS;
    	delete max_onePairHS;
    	delete highCardHS;
    	delete min_highCardHS;
    	delete max_highCardHS;
    }
public:
    LYPineappleAlgorithmDelegate* pad;
    std::vector<LYCard> cards;
    std::vector<LYCard> royalFlush;
    std::vector<LYCard> straightFlushHand;
    std::vector<LYCard> min_straightFlushHand;
    std::vector<LYCard> max_straightFlushHand;
    std::vector<LYCard> fourOfAKindHand;
    std::vector<LYCard> min_fourOfAKindHand;
    std::vector<LYCard> max_fourOfAKindHand;
    std::vector<LYCard> fullhouseHand;
    std::vector<LYCard> min_fullhouseHand;
    std::vector<LYCard> max_fullhouseHand;
    std::vector<LYCard> flushHand;
    std::vector<LYCard> min_flushHand;
    std::vector<LYCard> max_flushHand;
    std::vector<LYCard> straightHand;
    std::vector<LYCard> min_straightHand;
    std::vector<LYCard> max_straightHand;
    std::vector<LYCard> threeOfAKindHand;
    std::vector<LYCard> min_threeOfAKindHand;
    std::vector<LYCard> max_threeOfAKindHand;
    std::vector<LYCard> twoPairHand;
    std::vector<LYCard> min_twoPairHand;
    std::vector<LYCard> max_twoPairHand;
    std::vector<LYCard> onePairHand;
    std::vector<LYCard> min_onePairHand;
    std::vector<LYCard> max_onePairHand;

    LYHandStrength* royalFlushHS;
    LYHandStrength* straightFlushHS;
    LYHandStrength* min_straightFlushHS;
    LYHandStrength* max_straightFlushHS;
    LYHandStrength* fourOfAKindHS;
    LYHandStrength* min_fourOfAKindHS;
    LYHandStrength* max_fourOfAKindHS;
    LYHandStrength* fullhouseHS;
    LYHandStrength* min_fullhouseHS;
    LYHandStrength* max_fullhouseHS;
    LYHandStrength* flushHS;
    LYHandStrength* min_flushHS;
    LYHandStrength* max_flushHS;
    LYHandStrength* straightHS;
    LYHandStrength* min_straightHS;
    LYHandStrength* max_straightHS;
    LYHandStrength* threeOfAKindHS;
    LYHandStrength* min_threeOfAKindHS;
    LYHandStrength* max_threeOfAKindHS;
    LYHandStrength* twoPairHS;
    LYHandStrength* min_twoPairHS;
    LYHandStrength* max_twoPairHS;
    LYHandStrength* onePairHS;
    LYHandStrength* min_onePairHS;
    LYHandStrength* max_onePairHS;
    LYHandStrength* highCardHS;
    LYHandStrength* min_highCardHS;
    LYHandStrength* max_highCardHS;


};

TEST_F(LYPineappleAlgorithmDelegate_tests, straightFlush)
{
	std::vector<LYCard> cards;
	cards.push_back(sA);
	cards.push_back(sK);
	cards.push_back(sQ);
	cards.push_back(sJ);
//	cards.push_back(sT);
	cards.push_back(smallGhost);

	LYHandStrength* hs = new LYHandStrength(cards, pad, NULL);
	ASSERT_EQ(hs->hasGhost(), true);
	hs->reset();
	pad->isStraightFlush(hs, NULL);
	ASSERT_EQ(hs->ranking, StraightFlush);
	ASSERT_EQ(hs->rank[0].toString(), "As");
	ASSERT_EQ(hs->rank[1].toString(), "Ks");
	ASSERT_EQ(hs->rank[2].toString(), "Qs");
	ASSERT_EQ(hs->rank[3].toString(), "Js");
	ASSERT_EQ(hs->rank[4].toString(), "Ts");
	delete hs;
}

TEST_F(LYPineappleAlgorithmDelegate_tests, straightFlush2)
{
	std::vector<LYCard> cards;
	cards.push_back(sA);
	cards.push_back(s2);
	cards.push_back(s3);
	cards.push_back(s4);
//	cards.push_back(sT);
	cards.push_back(smallGhost);

	LYHandStrength* hs = new LYHandStrength(cards, pad, NULL);
	ASSERT_EQ(hs->hasGhost(), true);
//	pad->isStraightFlush(hs, NULL);
	ASSERT_EQ(hs->ranking, StraightFlush);
	delete hs;
}

TEST_F(LYPineappleAlgorithmDelegate_tests, straightFlush3)
{
	ASSERT_EQ(royalFlushHS->ranking, StraightFlush);

	std::vector<LYCard> cards;
	cards.push_back(sA);
	cards.push_back(s2);
	cards.push_back(s3);
	cards.push_back(s4);
//	cards.push_back(sT);
	cards.push_back(smallGhost);

	LYHandStrength* hs = new LYHandStrength(cards, pad, royalFlushHS);
	ASSERT_EQ(hs->hasGhost(), true);
//	pad->isStraightFlush(hs, royalFlushHS);
	ASSERT_EQ(hs->ranking, StraightFlush);
	delete hs;
}

TEST_F(LYPineappleAlgorithmDelegate_tests, straightFlush4)
{
	ASSERT_EQ(royalFlushHS->ranking, StraightFlush);

	std::vector<LYCard> cards;
	cards.push_back(sA);
	cards.push_back(s2);
	cards.push_back(s5);
	cards.push_back(s4);
	cards.push_back(smallGhost);

	LYHandStrength* hs = new LYHandStrength(cards, pad, royalFlushHS);
	ASSERT_EQ(hs->hasGhost(), true);
//	pad->isStraightFlush(hs, royalFlushHS);
	ASSERT_EQ(hs->ranking, StraightFlush);
	delete hs;
}

TEST_F(LYPineappleAlgorithmDelegate_tests, straightFlush5)
{
	ASSERT_EQ(straightHS->ranking, Straight);

	std::vector<LYCard> cards;
	cards.push_back(sA);
	cards.push_back(s2);
	cards.push_back(s3);
	cards.push_back(s4);
//	cards.push_back(sT);
	cards.push_back(smallGhost);

	LYHandStrength* hs = new LYHandStrength(cards, pad, straightHS);
	ASSERT_EQ(hs->hasGhost(), true);
//	hs->reset();
//	pad->isStraightFlush(hs, straightHS);
	ASSERT_EQ(hs->ranking, Straight);
	delete hs;
}

TEST_F(LYPineappleAlgorithmDelegate_tests, straightFlush4_called_by_highCard)
{
	ASSERT_EQ(straightHS->ranking, Straight);

	std::vector<LYCard> cards;
	cards.push_back(sA);
	cards.push_back(s2);
	cards.push_back(s3);
	cards.push_back(s4);
//	cards.push_back(sT);
	cards.push_back(smallGhost);

	LYHandStrength* hs = new LYHandStrength(cards, pad, straightHS);
	ASSERT_EQ(hs->hasGhost(), true);
	hs->reset();
	pad->isHighCard(hs, straightHS);
//	ASSERT_EQ(hs->ranking, Straight);
	delete hs;
}

TEST_F(LYPineappleAlgorithmDelegate_tests, straightFlush6)
{
    std::vector<LYCard> cards;
    cards.push_back(d5);
    cards.push_back(d4);
    cards.push_back(d3);
    cards.push_back(d6);
//  cards.push_back(sT);
    cards.push_back(smallGhost);

    LYHandStrength* hs = new LYHandStrength(cards, pad, min_straightFlushHS);
    ASSERT_EQ(min_straightFlushHS->ranking, StraightFlush);
    ASSERT_EQ(min_straightFlushHS->topRankCard, d5);
    ASSERT_EQ(hs->hasGhost(), true);
    hs->reset();
    ASSERT_EQ(hs->rank.size(), 0);
    LYHandStrength* h = pad->isStraightFlush(hs, min_straightFlushHS);
    ASSERT_EQ(h==NULL, true);
    delete hs;
}

TEST_F(LYPineappleAlgorithmDelegate_tests, FourOfAKind)
{
	std::vector<LYCard> cards;
	cards.push_back(sA);
	cards.push_back(hA);
	cards.push_back(dA);
	cards.push_back(cA);
//	cards.push_back(sT);
	cards.push_back(smallGhost);

	LYHandStrength* hs = new LYHandStrength(cards, pad, NULL);
	ASSERT_EQ(hs->hasGhost(), true);
	hs->reset();
	ASSERT_EQ(hs->rank.size(), 0);
	pad->isFourOfAKind(hs, NULL);
	ASSERT_EQ(hs->ranking, FourOfAKind);
	delete hs;
}

TEST_F(LYPineappleAlgorithmDelegate_tests, FourOfAKind2)
{
	std::vector<LYCard> cards;
	cards.push_back(sA);
	cards.push_back(hA);
	cards.push_back(dA);
	cards.push_back(cK);
//	cards.push_back(sT);
	cards.push_back(smallGhost);

	LYHandStrength* hs = new LYHandStrength(cards, pad, NULL);
	ASSERT_EQ(hs->hasGhost(), true);
	hs->reset();
	ASSERT_EQ(hs->rank.size(), 0);
	pad->isFourOfAKind(hs, NULL);
	ASSERT_EQ(hs->ranking, FourOfAKind);
	delete hs;
}

TEST_F(LYPineappleAlgorithmDelegate_tests, FourOfAKind3)
{
	std::vector<LYCard> cards;
	cards.push_back(sA);
	cards.push_back(hA);
	cards.push_back(dA);
	cards.push_back(cK);
//	cards.push_back(sT);
	cards.push_back(smallGhost);

	LYHandStrength* hs = new LYHandStrength(cards, pad, royalFlushHS);
	ASSERT_EQ(hs->hasGhost(), true);
	hs->reset();
	ASSERT_EQ(hs->rank.size(), 0);
	pad->isFourOfAKind(hs, royalFlushHS);
	ASSERT_EQ(hs->ranking, FourOfAKind);
	delete hs;
}

TEST_F(LYPineappleAlgorithmDelegate_tests, FourOfAKind4)
{
	std::vector<LYCard> cards;
	cards.push_back(sA);
	cards.push_back(hA);
	cards.push_back(dA);
	cards.push_back(cK);
//	cards.push_back(sT);
	cards.push_back(smallGhost);

	LYHandStrength* hs = new LYHandStrength(cards, pad, straightHS);
	ASSERT_EQ(hs->hasGhost(), true);
	hs->reset();
	ASSERT_EQ(hs->rank.size(), 0);
	pad->isFourOfAKind(hs, straightHS);
//	ASSERT_EQ(hs->ranking, HighCard);
	delete hs;
}

TEST_F(LYPineappleAlgorithmDelegate_tests, FourOfAKind4_called_by_highCard)
{
	std::vector<LYCard> cards;
	cards.push_back(sA);
	cards.push_back(hA);
	cards.push_back(dA);
	cards.push_back(cK);
//	cards.push_back(sT);
	cards.push_back(smallGhost);

	LYHandStrength* hs = new LYHandStrength(cards, pad, straightHS);
	ASSERT_EQ(hs->hasGhost(), true);
	hs->reset();
	ASSERT_EQ(hs->rank.size(), 0);
	pad->isHighCard(hs, straightHS);
//	ASSERT_EQ(hs->ranking, HighCard);
	delete hs;
}

TEST_F(LYPineappleAlgorithmDelegate_tests, FullHouse)
{
	std::vector<LYCard> cards;
	cards.push_back(s8);
	cards.push_back(h8);
	cards.push_back(d8);
	cards.push_back(cK);
//	cards.push_back(sT);
	cards.push_back(smallGhost);

	LYHandStrength* hs = new LYHandStrength(cards, pad, fullhouseHS);
	ASSERT_EQ(hs->hasGhost(), true);
	hs->reset();
	ASSERT_EQ(hs->rank.size(), 0);
	pad->isFullHouse(hs, fullhouseHS);
	ASSERT_EQ(hs->ranking, FullHouse);
	delete hs;
}

TEST_F(LYPineappleAlgorithmDelegate_tests, FullHouse2)
{
	std::vector<LYCard> cards;
	cards.push_back(sK);
	cards.push_back(hK);
	cards.push_back(dK);
	cards.push_back(c2);
//	cards.push_back(sT);
	cards.push_back(smallGhost);

	LYHandStrength* hs = new LYHandStrength(cards, pad, fullhouseHS);
	ASSERT_EQ(hs->hasGhost(), true);
	hs->reset();
	ASSERT_EQ(hs->rank.size(), 0);
	pad->isFullHouse(hs, fullhouseHS);
	ASSERT_NE(hs->ranking, FullHouse);
	delete hs;
}

TEST_F(LYPineappleAlgorithmDelegate_tests, FullHouse3)
{
	std::vector<LYCard> cards;
	cards.push_back(sK);
	cards.push_back(hK);
	cards.push_back(d2);
	cards.push_back(c2);
//	cards.push_back(sT);
	cards.push_back(smallGhost);

	LYHandStrength* hs = new LYHandStrength(cards, pad, fullhouseHS);
	ASSERT_EQ(hs->hasGhost(), true);
	hs->reset();
	ASSERT_EQ(hs->rank.size(), 0);
	pad->isFullHouse(hs, fullhouseHS);
	ASSERT_EQ(hs->ranking, FullHouse);
	delete hs;
}

TEST_F(LYPineappleAlgorithmDelegate_tests, FullHouse3_called_by_highCard)
{
	std::vector<LYCard> cards;
	cards.push_back(sK);
	cards.push_back(hK);
	cards.push_back(d2);
	cards.push_back(c2);
//	cards.push_back(sT);
	cards.push_back(smallGhost);

	LYHandStrength* hs = new LYHandStrength(cards, pad, fullhouseHS);
	ASSERT_EQ(hs->hasGhost(), true);
	hs->reset();
	ASSERT_EQ(hs->rank.size(), 0);
	pad->isHighCard(hs, fullhouseHS);
	ASSERT_EQ(hs->ranking, FullHouse);
	delete hs;
}

TEST_F(LYPineappleAlgorithmDelegate_tests, FullHouse_4)
{
    std::vector<LYCard> cards;
    cards.push_back(s5);
    cards.push_back(h5);
    cards.push_back(d6);
    cards.push_back(c6);
//  cards.push_back(sT);
    cards.push_back(smallGhost);

    LYHandStrength* hs = new LYHandStrength(cards, pad, min_fullhouseHS);
    ASSERT_EQ(hs->hasGhost(), true);
    hs->reset();
    ASSERT_EQ(hs->rank.size(), 0);
    pad->isFullHouse(hs, min_fullhouseHS);
    ASSERT_EQ(hs->ranking, TwoPair);
    delete hs;
}

TEST_F(LYPineappleAlgorithmDelegate_tests, Flush1)
{
	std::vector<LYCard> cards;
	cards.push_back(sK);
	cards.push_back(sA);
	cards.push_back(s3);
	cards.push_back(s2);
//	cards.push_back(sT);
	cards.push_back(smallGhost);

	LYHandStrength* hs = new LYHandStrength(cards, pad, NULL);
	ASSERT_EQ(hs->hasGhost(), true);
	hs->reset();
	ASSERT_EQ(hs->rank.size(), 0);
	pad->isFlush(hs, NULL);
	ASSERT_EQ(hs->ranking, Flush);
	delete hs;
}

TEST_F(LYPineappleAlgorithmDelegate_tests, Flush2)
{
	std::vector<LYCard> cards;
	cards.push_back(d5);
	cards.push_back(d4);
	cards.push_back(d3);
	cards.push_back(d2);
//	cards.push_back(sT);
	cards.push_back(smallGhost);

	LYHandStrength* hs = new LYHandStrength(cards, pad, fullhouseHS);
	ASSERT_EQ(hs->hasGhost(), true);
	hs->reset();
	ASSERT_EQ(hs->rank.size(), 0);
	pad->isFlush(hs, fullhouseHS);
	ASSERT_EQ(hs->ranking, Flush);
//	std::cout << "topRankCard: " << hs->topRankCard.toString() << std::endl;
	// ASSERT_EQ(hs->topRankCard, dK);
	std::vector<LYCard>::iterator it = hs->rank.begin();
	for (; it!=hs->rank.end(); it++) {
		ASSERT_EQ((*it).suit, Diamonds);
	}
	for (unsigned int i=0; i<hs->rank.size(); i++) {
		ASSERT_EQ(hs->rank[i].suit, Diamonds);
	}
	// ASSERT_EQ(hs->rank[0].toString(), "5d");
	// ASSERT_EQ(hs->rank[1].toString(), "4d");
	// ASSERT_EQ(hs->rank[2].toString(), "3d");
	// ASSERT_EQ(hs->rank[3].toString(), "2d");
	// ASSERT_EQ(hs->rank[4].toString(), "0d");
	ASSERT_EQ(hs->rank[0], dK);
	delete hs;
}


TEST_F(LYPineappleAlgorithmDelegate_tests, Flush_3)
{
    std::vector<LYCard> cards;
    cards.push_back(d5);
    cards.push_back(d4);
    cards.push_back(d3);
    cards.push_back(d6);
//  cards.push_back(sT);
    cards.push_back(smallGhost);

    LYHandStrength* hs = new LYHandStrength(cards, pad, min_straightFlushHS);
    ASSERT_EQ(min_straightFlushHS->ranking, StraightFlush);
    ASSERT_EQ(min_straightFlushHS->topRankCard, d5);
    ASSERT_EQ(hs->hasGhost(), true);
    hs->reset();
    ASSERT_EQ(hs->rank.size(), 0);
    pad->isFlush(hs, min_straightFlushHS);
    ASSERT_EQ(hs->ranking, Flush);
    ASSERT_EQ(hs->topRankCard, dA);
    delete hs;
}

TEST_F(LYPineappleAlgorithmDelegate_tests, Flush_4)
{
    std::vector<LYCard> cards;
    cards.push_back(d5);
    cards.push_back(d4);
    cards.push_back(d3);
    cards.push_back(d2);
//  cards.push_back(sT);
    cards.push_back(smallGhost);

    LYHandStrength* hs = new LYHandStrength(cards, pad, min_flushHS);
    ASSERT_EQ(hs->hasGhost(), true);
    ASSERT_EQ(min_flushHS->topRankCard.face, SEVEN);
    hs->reset();
    ASSERT_EQ(hs->rank.size(), 0);
    pad->isFlush(hs, min_flushHS);   
    ASSERT_EQ(hs->ranking, Flush);
    ASSERT_EQ(hs->topRankCard, d7);
    delete hs;
}

TEST_F(LYPineappleAlgorithmDelegate_tests, Flush_5)
{
    std::vector<LYCard> cards;
    cards.push_back(d5);
    cards.push_back(d4);
    cards.push_back(d3);
    cards.push_back(d2);
//  cards.push_back(sT);
    cards.push_back(smallGhost);

    LYHandStrength* hs = new LYHandStrength(cards, pad, max_flushHS);
    ASSERT_EQ(hs->hasGhost(), true);
    hs->reset();
    ASSERT_EQ(hs->rank.size(), 0);
    pad->isFlush(hs, max_flushHS);
    ASSERT_EQ(hs->ranking, Flush);
    ASSERT_EQ(hs->topRankCard, dK);
    delete hs;
}

TEST_F(LYPineappleAlgorithmDelegate_tests, Flush_6)
{
    std::vector<LYCard> cards;
    cards.push_back(d5);
    cards.push_back(d4);
    cards.push_back(d3);
    cards.push_back(d2);
//  cards.push_back(sT);
    cards.push_back(smallGhost);

    LYHandStrength* hs = new LYHandStrength(cards, pad, fourOfAKindHS);
    ASSERT_EQ(hs->hasGhost(), true);
    hs->reset();
    ASSERT_EQ(hs->rank.size(), 0);
    pad->isFlush(hs, fourOfAKindHS);  
    ASSERT_EQ(hs->ranking, Flush);
    // std::cout << "==============RankCard: ============" << hs->topRankCard.toString()[0] << std::endl;
    ASSERT_EQ(hs->topRankCard.suit, Diamonds);
    ASSERT_EQ(hs->topRankCard.face, KING);
    delete hs;
}

TEST_F(LYPineappleAlgorithmDelegate_tests, Flush_6_called_by_highCard)
{
    std::vector<LYCard> cards;
    cards.push_back(d5);
    cards.push_back(d4);
    cards.push_back(d3);
    cards.push_back(d2);
//  cards.push_back(sT);
    cards.push_back(smallGhost);

    LYHandStrength* hs = new LYHandStrength(cards, pad, fourOfAKindHS);
    ASSERT_EQ(hs->hasGhost(), true);
    hs->reset();
    ASSERT_EQ(hs->rank.size(), 0);
    pad->isHighCard(hs, fourOfAKindHS);
    ASSERT_EQ(hs->ranking, Flush);
    // std::cout << "==============RankCard: ============" << hs->topRankCard.toString()[0] << std::endl;
    ASSERT_EQ(hs->topRankCard.suit, Diamonds);
    ASSERT_EQ(hs->topRankCard.face, KING);
    delete hs;
}

TEST_F(LYPineappleAlgorithmDelegate_tests, straight_0)
{
    std::vector<LYCard> cards;
    cards.push_back(d2);
    cards.push_back(d4);
    cards.push_back(d3);
    cards.push_back(s5);
//  cards.push_back(sT);
    cards.push_back(smallGhost);

    LYHandStrength* hs = new LYHandStrength(cards, pad, NULL);
    ASSERT_EQ(hs->hasGhost(), true);
    hs->reset();
    ASSERT_EQ(hs->rank.size(), 0);
    pad->isStraight(hs, NULL);
    ASSERT_EQ(hs->ranking, Straight);
    ASSERT_EQ(hs->topRankCard.face, SIX);
    delete hs;
}

TEST_F(LYPineappleAlgorithmDelegate_tests, straight_1)
{
    std::vector<LYCard> cards;
    cards.push_back(d6);
    cards.push_back(d4);
    cards.push_back(d3);
    cards.push_back(d2);
//  cards.push_back(sT);
    cards.push_back(smallGhost);

    LYHandStrength* hs = new LYHandStrength(cards, pad, min_flushHS);
    ASSERT_EQ(hs->hasGhost(), true);
    hs->reset();
    ASSERT_EQ(hs->rank.size(), 0);
    pad->isStraight(hs, min_flushHS);
    // for (unsigned int i=0; i<hs->rank.size(); i++) {
    //     std::cout << "==============RankCard: ============" << hs->rank[i].toString() << std::endl;
    // }
    ASSERT_EQ(hs->ranking, Straight);
    ASSERT_NE(hs->rank[1].suit, Diamonds);
    ASSERT_EQ(hs->rank[1].face, FIVE);
    delete hs;
}

TEST_F(LYPineappleAlgorithmDelegate_tests, straight_2)
{
    std::vector<LYCard> cards;
    cards.push_back(dK);
    cards.push_back(dQ);
    cards.push_back(dJ);
    cards.push_back(dT);
//  cards.push_back(sT);
    cards.push_back(smallGhost);

    LYHandStrength* hs = new LYHandStrength(cards, pad, min_flushHS);
    ASSERT_EQ(hs->hasGhost(), true);
    hs->reset();
    ASSERT_EQ(hs->rank.size(), 0);
    pad->isStraight(hs, min_flushHS);
    // for (unsigned int i=0; i<hs->rank.size(); i++) {
    //     std::cout << "RankCard: " << hs->rank[i].toString() << std::endl;
    // }
    ASSERT_EQ(hs->ranking, Straight);
    ASSERT_NE(hs->topRankCard.suit, Diamonds);
    ASSERT_EQ(hs->topRankCard.face, ACE);
    delete hs;
}

TEST_F(LYPineappleAlgorithmDelegate_tests, straight_3)
{
    std::vector<LYCard> cards;
    cards.push_back(d4);
    cards.push_back(d5);
    cards.push_back(d3);
    cards.push_back(s2);
//  cards.push_back(sT);
    cards.push_back(smallGhost);

    LYHandStrength* hs = new LYHandStrength(cards, pad, min_straightHS);            //min_straigth is 12345
    ASSERT_EQ(hs->hasGhost(), true);
    hs->reset();
    ASSERT_EQ(hs->rank.size(), 0);
    pad->isStraight(hs, min_straightHS);
    // for (unsigned int i=0; i<hs->rank.size(); i++) {
    //     std::cout << "==============RankCard: ============" << hs->rank[i].toString() << std::endl;
    // }
    ASSERT_EQ(hs->ranking, Straight);
    ASSERT_EQ(hs->topRankCard.face, FIVE);
    delete hs;
}

TEST_F(LYPineappleAlgorithmDelegate_tests, straight_4)
{
    std::vector<LYCard> cards;
    cards.push_back(d4);
    cards.push_back(d5);
    cards.push_back(d3);
    cards.push_back(sA);
//  cards.push_back(sT);
    cards.push_back(smallGhost);

    LYHandStrength* hs = new LYHandStrength(cards, pad, min_straightHS);            //min_straigth is 12345
    ASSERT_EQ(hs->hasGhost(), true);
    hs->reset();
    ASSERT_EQ(hs->rank.size(), 0);
    pad->isStraight(hs, min_straightHS);
    // for (unsigned int i=0; i<hs->rank.size(); i++) {
    //     std::cout << "==============RankCard: ============" << hs->rank[i].toString() << std::endl;
    // }
    ASSERT_EQ(hs->ranking, Straight);
    ASSERT_EQ(hs->rank[4].face, TWO);
    delete hs;
}

TEST_F(LYPineappleAlgorithmDelegate_tests, straight_4_called_by_highCard)
{
    std::vector<LYCard> cards;
    cards.push_back(d4);
    cards.push_back(d5);
    cards.push_back(d3);
    cards.push_back(sA);
//  cards.push_back(sT);
    cards.push_back(smallGhost);

    LYHandStrength* hs = new LYHandStrength(cards, pad, min_straightHS);            //min_straigth is 12345
    ASSERT_EQ(hs->hasGhost(), true);
    hs->reset();
    ASSERT_EQ(hs->rank.size(), 0);
    pad->isHighCard(hs, min_straightHS);
    // for (unsigned int i=0; i<hs->rank.size(); i++) {
    //     std::cout << "==============RankCard: ============" << hs->rank[i].toString() << std::endl;
    // }
    ASSERT_EQ(hs->ranking, Straight);
    ASSERT_EQ(hs->rank[4].face, TWO);
    delete hs;
}

TEST_F(LYPineappleAlgorithmDelegate_tests, threeOfAKind_1)
{
    std::vector<LYCard> cards;
    cards.push_back(d6);
    cards.push_back(s6);
    cards.push_back(d3);
    cards.push_back(d2);
//  cards.push_back(sT);
    cards.push_back(smallGhost);

    LYHandStrength* hs = new LYHandStrength(cards, pad, NULL);
    ASSERT_EQ(hs->hasGhost(), true);
    hs->reset();
    ASSERT_EQ(hs->rank.size(), 0);
    pad->isThreeOfAKind(hs, NULL);
//    for (unsigned int i=0; i<hs->rank.size(); i++) {
//        std::cout << "==============RankCard: ============" << hs->rank[i].toString() << std::endl;
//    }
    ASSERT_EQ(hs->ranking, ThreeOfAKind);
    ASSERT_EQ(hs->rank[1], h6);
    delete hs;
}


TEST_F(LYPineappleAlgorithmDelegate_tests, threeOfAKind_2)
{
    std::vector<LYCard> cards;
    cards.push_back(d6);
    cards.push_back(s6);
    cards.push_back(d3);
    cards.push_back(d2);
//  cards.push_back(sT);
    cards.push_back(smallGhost);

    LYHandStrength* hs = new LYHandStrength(cards, pad, max_threeOfAKindHS);
    ASSERT_EQ(hs->hasGhost(), true);
    hs->reset();
    ASSERT_EQ(hs->rank.size(), 0);
    pad->isThreeOfAKind(hs, max_threeOfAKindHS);
//    for (unsigned int i=0; i<hs->rank.size(); i++) {
//        std::cout << "==============RankCard: ============" << hs->rank[i].toString() << std::endl;
//    }
    ASSERT_EQ(hs->ranking, ThreeOfAKind);
    ASSERT_EQ(hs->rank[1], h6);
    delete hs;
}

TEST_F(LYPineappleAlgorithmDelegate_tests, threeOfAKind_3)
{
    std::vector<LYCard> cards;
    cards.push_back(d4);
    cards.push_back(s2);
    cards.push_back(d3);
    cards.push_back(d2);
//  cards.push_back(sT);
    cards.push_back(smallGhost);

    LYHandStrength* hs = new LYHandStrength(cards, pad, min_threeOfAKindHS);
    ASSERT_EQ(hs->hasGhost(), true);
    hs->reset();
    ASSERT_EQ(hs->rank.size(), 0);
    pad->isThreeOfAKind(hs, min_threeOfAKindHS);
//    for (unsigned int i=0; i<hs->rank.size(); i++) {
//        std::cout << "==============RankCard: ============" << hs->rank[i].toString() << std::endl;
//    }
    ASSERT_EQ(hs->ranking, ThreeOfAKind);
    ASSERT_EQ(hs->rank[1], h2);
    delete hs;
}

TEST_F(LYPineappleAlgorithmDelegate_tests, threeOfAKind_4)
{
    std::vector<LYCard> cards;
    cards.push_back(d6);
    cards.push_back(s6);
    cards.push_back(h6);
    cards.push_back(d2);
//  cards.push_back(sT);
    cards.push_back(smallGhost);

    LYHandStrength* hs = new LYHandStrength(cards, pad, max_threeOfAKindHS);
    ASSERT_EQ(hs->hasGhost(), true);
    hs->reset();
    ASSERT_EQ(hs->rank.size(), 0);
    pad->isThreeOfAKind(hs, max_threeOfAKindHS);
//    for (unsigned int i=0; i<hs->rank.size(); i++) {
//        std::cout << "==============RankCard: ============" << hs->rank[i].toString() << std::endl;
//    }
    ASSERT_EQ(hs->ranking, ThreeOfAKind);
    ASSERT_EQ(hs->kicker[0], sA);
    delete hs;
}

TEST_F(LYPineappleAlgorithmDelegate_tests, TwoPair_1)
{
	std::vector<LYCard> cards;
	cards.push_back(d4);
	cards.push_back(s4);
	cards.push_back(s5);
	cards.push_back(d5);
	//  cards.push_back(sT);
	cards.push_back(smallGhost);

	LYHandStrength* hs = new LYHandStrength(cards, pad, min_threeOfAKindHS);
	ASSERT_EQ(hs->hasGhost(), true);
	hs->reset();
	ASSERT_EQ(hs->rank.size(), 0);
	pad->isTwoPair(hs, min_threeOfAKindHS);
	//     for (unsigned int i=0; i<hs->rank.size(); i++) {
	//         std::cout << "RankCard: " << hs->rank[i].toString() << std::endl;
	//     }
	ASSERT_EQ(hs->ranking, TwoPair);
	ASSERT_EQ(hs->kicker[0], sA);
	delete hs;
}

TEST_F(LYPineappleAlgorithmDelegate_tests, TwoPair_2)
{
	std::vector<LYCard> cards;
	cards.push_back(d4);
	cards.push_back(s4);
	cards.push_back(s9);
	cards.push_back(d5);
	//  cards.push_back(sT);
	cards.push_back(smallGhost);

	LYHandStrength* hs = new LYHandStrength(cards, pad, min_threeOfAKindHS);
	ASSERT_EQ(hs->hasGhost(), true);
	hs->reset();
	ASSERT_EQ(hs->rank.size(), 0);
	pad->isTwoPair(hs, min_threeOfAKindHS);
//	for (unsigned int i=0; i<hs->rank.size(); i++) {
//		std::cout << "RankCard: " << hs->rank[i].toString() << std::endl;
//	}
	ASSERT_EQ(hs->ranking, TwoPair);
	ASSERT_EQ(hs->topRankCard, s9);
	ASSERT_EQ(hs->rank[1], h9);
	delete hs;
}

//TEST_F(LYPineappleAlgorithmDelegate_tests, TwoPair_3)
//{
//	std::vector<LYCard> cards;
//	cards.push_back(d4);
//	cards.push_back(s4);
//	cards.push_back(s5);
//	cards.push_back(d5);
//	//  cards.push_back(sT);
//	cards.push_back(smallGhost);
//
//	LYHandStrength* hs = new LYHandStrength(cards, pad, min_threeOfAKindHS);
//	ASSERT_EQ(hs->hasGhost(), true);
//	hs->reset();
//	ASSERT_EQ(hs->rank.size(), 0);
//	pad->isFlush(hs, min_threeOfAKindHS);
//	for (unsigned int i=0; i<hs->rank.size(); i++) {
//		std::cout << "RankCard: " << hs->rank[i].toString() << std::endl;
//	}
//	ASSERT_EQ(hs->ranking, TwoPair);
//	delete hs;
//}

TEST_F(LYPineappleAlgorithmDelegate_tests, OnePair_1)
{
	std::vector<LYCard> cards;
	cards.push_back(d8);
	cards.push_back(s4);
	cards.push_back(s5);
	cards.push_back(d5);
	//  cards.push_back(sT);
	cards.push_back(smallGhost);

	LYHandStrength* hs = new LYHandStrength(cards, pad, min_twoPairHS);
	ASSERT_EQ(hs->hasGhost(), true);
	hs->reset();
	ASSERT_EQ(hs->rank.size(), 0);
	pad->isOnePair(hs, min_twoPairHS);
//	for (unsigned int i=0; i<hs->rank.size(); i++) {
//		std::cout << "RankCard: " << hs->rank[i].toString() << std::endl;
//	}
	ASSERT_EQ(hs->ranking, OnePair);
	ASSERT_EQ(hs->kicker[0], sA);
	delete hs;
}

TEST_F(LYPineappleAlgorithmDelegate_tests, OnePair_2)
{
	std::vector<LYCard> cards;
	cards.push_back(d8);
	cards.push_back(s4);
	cards.push_back(s5);
	cards.push_back(d9);
	//  cards.push_back(sT);
	cards.push_back(smallGhost);

	LYHandStrength* hs = new LYHandStrength(cards, pad, max_onePairHS);
	ASSERT_EQ(hs->hasGhost(), true);
	hs->reset();
	ASSERT_EQ(hs->rank.size(), 0);
	pad->isOnePair(hs, max_onePairHS);
//	for (unsigned int i=0; i<hs->rank.size(); i++) {
//		std::cout << "RankCard: " << hs->rank[i].toString() << std::endl;
//	}
	ASSERT_EQ(hs->ranking, OnePair);
	ASSERT_EQ(hs->rank[0], s9);
	delete hs;
}

TEST_F(LYPineappleAlgorithmDelegate_tests, OnePair_3)
{
	std::vector<LYCard> cards;
	cards.push_back(hA);
	cards.push_back(hK);
	cards.push_back(sJ);
	cards.push_back(dJ);
	cards.push_back(s8);
	LYHandStrength* capHs = new LYHandStrength(cards, pad);
	ASSERT_EQ(capHs->ranking, OnePair);

	cards.clear();
	cards.push_back(s6);
	cards.push_back(s2);
	cards.push_back(s5);
	cards.push_back(d4);
	cards.push_back(smallGhost);

	LYHandStrength* hs = new LYHandStrength(cards, pad, capHs);
	ASSERT_EQ(hs->hasGhost(), true);
	hs->reset();
	ASSERT_EQ(hs->rank.size(), 0);
	pad->isOnePair(hs, max_onePairHS);
	ASSERT_EQ(hs->ranking, OnePair);
	delete hs;
	delete capHs;
}

TEST_F(LYPineappleAlgorithmDelegate_tests, HighCard_1)
{
	std::vector<LYCard> cards;
	cards.push_back(d8);
	cards.push_back(s4);
	cards.push_back(s5);
	cards.push_back(d9);
	//  cards.push_back(sT);
	cards.push_back(smallGhost);

	LYHandStrength* hs = new LYHandStrength(cards, pad, min_onePairHS);
	ASSERT_EQ(hs->hasGhost(), true);
	hs->reset();
	ASSERT_EQ(hs->rank.size(), 0);
	pad->isHighCard(hs, min_onePairHS);
//	for (unsigned int i=0; i<hs->rank.size(); i++) {
//		std::cout << "RankCard: " << hs->rank[i].toString() << std::endl;
//	}
	ASSERT_EQ(hs->ranking, HighCard);
	ASSERT_EQ(hs->rank.size(), 1);
	ASSERT_EQ(hs->kicker.size(), 4);
	ASSERT_EQ(hs->topRankCard, sA);
	delete hs;
}

TEST_F(LYPineappleAlgorithmDelegate_tests, HighCard_2)
{
	std::vector<LYCard> cards;
	cards.push_back(d8);
	cards.push_back(s4);
	cards.push_back(s5);
	cards.push_back(d9);
	//  cards.push_back(sT);
	cards.push_back(smallGhost);

	LYHandStrength* hs = new LYHandStrength(cards, pad, min_highCardHS);
	ASSERT_EQ(hs->hasGhost(), true);
	hs->reset();
	ASSERT_EQ(hs->rank.size(), 0);
	pad->isHighCard(hs, min_highCardHS);
//	for (unsigned int i=0; i<hs->rank.size(); i++) {
//		std::cout << "RankCard: " << hs->rank[i].toString() << std::endl;
//	}
	ASSERT_EQ(hs->ranking, HighCard);
	ASSERT_EQ(hs->rank.size(), 1);
	ASSERT_EQ(hs->kicker.size(), 4);
	ASSERT_EQ(hs->topRankCard, d9);
	ASSERT_EQ(hs->kicker[3], s2);
	delete hs;
}

TEST_F(LYPineappleAlgorithmDelegate_tests, threeOfAKind_3_called_by_highcard)
{
    std::vector<LYCard> cards;
    cards.push_back(d4);
    cards.push_back(s2);
    cards.push_back(d3);
    cards.push_back(d2);
//  cards.push_back(sT);
    cards.push_back(smallGhost);

    LYHandStrength* hs = new LYHandStrength(cards, pad, min_threeOfAKindHS);
    ASSERT_EQ(hs->hasGhost(), true);
    hs->reset();
    ASSERT_EQ(hs->rank.size(), 0);
    pad->isHighCard(hs, min_threeOfAKindHS);
//    for (unsigned int i=0; i<hs->rank.size(); i++) {
//        std::cout << "==============RankCard: ============" << hs->rank[i].toString() << std::endl;
//    }
    ASSERT_EQ(hs->ranking, ThreeOfAKind);
    ASSERT_EQ(hs->rank[1], h2);
    delete hs;
}

