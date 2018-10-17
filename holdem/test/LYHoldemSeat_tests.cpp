/*
 * LYHoldemSeat_tests.cpp
 *
 *  Created on: 2013-7-5
 *      Author: caiqingfeng
 */

#include "src/LYHoldemSeat.h"
#include "poker/src/LYCardHelpers.h"
//#include "common/src/my_log.h"

#include <gtest/gtest.h>

class LYHoldemSeat_tests : public ::testing::Test
{
protected:

    void SetUp()
    {
        seat = new LYHoldemSeat(LYSeat1);
        communityCards.push_back(hA);
        communityCards.push_back(hK);
        communityCards.push_back(dA);
    }
    void TearDown()
    {
    	delete seat;
   }
public:
    LYHoldemSeat *seat;
    std::vector<LYCard> communityCards;

};

TEST_F(LYHoldemSeat_tests, init)
{
    ASSERT_EQ(seat->status, LYSeatOpen);
    ASSERT_EQ(seat->firstCard(), flipCard);
    ASSERT_EQ(seat->secondCard(), flipCard);
}

TEST_F(LYHoldemSeat_tests, fold)
{
    seat->chipsAtHand = 100;
    seat->newBuyin = 500;
    seat->fold("nothing");
    ASSERT_EQ(seat->chipsAtHand, 600);
    ASSERT_EQ(seat->newBuyin, 0);
}

TEST_F(LYHoldemSeat_tests, setCard)
{
	seat->setFirst(flipCard);
	seat->setSecond(flipCard);
	seat->setFirst(sA);
	seat->setSecond(sK);

    ASSERT_EQ(seat->firstCard(), sA);
    ASSERT_EQ(seat->secondCard(), sK);

	seat->setFirstCard(flipCard);
	seat->setSecondCard(flipCard);
    ASSERT_EQ(seat->firstCard(), flipCard);
    ASSERT_EQ(seat->secondCard(), flipCard);
	seat->setFirstCard(sA);
	seat->setSecondCard(sK);

    ASSERT_EQ(seat->firstCard(), sA);
    ASSERT_EQ(seat->secondCard(), sK);
}

TEST_F(LYHoldemSeat_tests, getHandStrength)
{
	seat->setFirst(sA);
	seat->setSecond(sK);
    ASSERT_EQ(seat->firstCard(), sA);
    ASSERT_EQ(seat->secondCard(), sK);

//    LYHandStrengthPtr hs = seat->genHandStrength(communityCards,
//    		(LYHoldemAlgorithmDelegate *)ServerPokerAlgorithmDelegate::Instance());
//    ASSERT_EQ(hs->ranking, FullHouse);
}

TEST_F(LYHoldemSeat_tests, caculateOuts_pair_against_2high)
{
    LYHoldemAlgorithmDelegate had;
	seat->setFirst(sA);
	seat->setSecond(sK);

    std::vector<LYCard> cards;
    LYCardHelpers::stringToCards("Qc,Qh,8s,2c,3d,", cards);
    LYHandStrength hs(cards, &had);
    ASSERT_EQ(hs.ranking, OnePair);

    std::vector<LYCard> communityCards;
    LYCardHelpers::stringToCards("8s,2c,3d,", communityCards);

    std::vector<LYCard> allVisibleCards;
    LYCardHelpers::stringToCards("Qc,Qh,As,Ks,8s,2c,3d,", allVisibleCards);
    std::vector<LYCard> outs = seat->caculateOuts(&hs, communityCards, allVisibleCards);
//    std::string str;
//    LYCardHelpers::cardsToString(outs, str);
//    std::cout << str << std::endl;
    ASSERT_EQ(outs.size(), 6);
}

TEST_F(LYHoldemSeat_tests, caculateOuts_pair_against_flushdraw_plus_2high)
{
    LYHoldemAlgorithmDelegate had;
	seat->setFirst(cA);
	seat->setSecond(cK);

    std::vector<LYCard> cards;
    LYCardHelpers::stringToCards("Qs,Qh,8s,2c,3c,", cards);
    LYHandStrength hs(cards, &had);
    ASSERT_EQ(hs.ranking, OnePair);

    std::vector<LYCard> communityCards;
    LYCardHelpers::stringToCards("8s,2c,3c,", communityCards);

    std::vector<LYCard> allVisibleCards;
    LYCardHelpers::stringToCards("Qs,Qh,Ac,Kc,8s,2c,3c,", allVisibleCards);
    std::vector<LYCard> outs = seat->caculateOuts(&hs, communityCards, allVisibleCards);
//    std::string str;
//    LYCardHelpers::cardsToString(outs, str);
//    std::cout << str << std::endl;
    ASSERT_EQ(outs.size(), 15);
}

TEST_F(LYHoldemSeat_tests, caculateOuts_straight_against_flushdraw)
{
    LYHoldemAlgorithmDelegate had;
	seat->setFirst(cA);
	seat->setSecond(cK);

    std::vector<LYCard> cards;
    LYCardHelpers::stringToCards("5s,6h,4s,2c,3c,", cards);
    LYHandStrength hs(cards, &had);
    ASSERT_EQ(hs.ranking, Straight);

    std::vector<LYCard> communityCards;
    LYCardHelpers::stringToCards("4s,2c,3c,", communityCards);

    std::vector<LYCard> allVisibleCards;
    LYCardHelpers::stringToCards("5s,6h,Ac,Kc,4s,2c,3c,", allVisibleCards);
    std::vector<LYCard> outs = seat->caculateOuts(&hs, communityCards, allVisibleCards);
//    std::string str;
//    LYCardHelpers::cardsToString(outs, str);
//    std::cout << str << std::endl;
    ASSERT_EQ(outs.size(), 9);
}

TEST_F(LYHoldemSeat_tests, caculateOuts_set_against_superdraw)
{
    LYHoldemAlgorithmDelegate had;
	seat->setFirst(cA);
	seat->setSecond(cK);

    std::vector<LYCard> cards;
    LYCardHelpers::stringToCards("4d,4h,4s,2c,3c,", cards);
    LYHandStrength hs(cards, &had);
    ASSERT_EQ(hs.ranking, ThreeOfAKind);

    std::vector<LYCard> communityCards;
    LYCardHelpers::stringToCards("4s,2c,3c,", communityCards);

    std::vector<LYCard> allVisibleCards;
    LYCardHelpers::stringToCards("4d,4h,Ac,Kc,4s,2c,3c,", allVisibleCards);
    std::vector<LYCard> outs = seat->caculateOuts(&hs, communityCards, allVisibleCards);
    std::string str;
    LYCardHelpers::cardsToString(outs, str);
//    std::cout << str << std::endl;
    ASSERT_EQ(outs.size(), 11);
}

TEST_F(LYHoldemSeat_tests, caculateOuts_pair_against_superdraw_with_2high)
{
    LYHoldemAlgorithmDelegate had;
	seat->setFirst(c5);
	seat->setSecond(cJ);

    std::vector<LYCard> cards;
    LYCardHelpers::stringToCards("2d,9h,4s,2c,3c,", cards);
    LYHandStrength hs(cards, &had);
    ASSERT_EQ(hs.ranking, OnePair);

    std::vector<LYCard> communityCards;
    LYCardHelpers::stringToCards("4s,2c,3c,", communityCards);

    std::vector<LYCard> allVisibleCards;
    LYCardHelpers::stringToCards("2d,9h,5c,Jc,4s,2c,3c,", allVisibleCards);
    std::vector<LYCard> outs = seat->caculateOuts(&hs, communityCards, allVisibleCards);
    std::string str;
    LYCardHelpers::cardsToString(outs, str);
//    std::cout << str << std::endl;
    ASSERT_EQ(outs.size(), 9+6+6);
}

TEST_F(LYHoldemSeat_tests, caculateOuts_high_against_superdraw_with_2high)
{
    LYHoldemAlgorithmDelegate had;
	seat->setFirst(c5);
	seat->setSecond(cJ);

    std::vector<LYCard> cards;
    LYCardHelpers::stringToCards("Ad,9h,4s,2c,3c,", cards);
    LYHandStrength hs(cards, &had);
    ASSERT_EQ(hs.ranking, HighCard);

    std::vector<LYCard> communityCards;
    LYCardHelpers::stringToCards("4s,2c,3c,", communityCards);

    std::vector<LYCard> allVisibleCards;
    LYCardHelpers::stringToCards("Ad,9h,5c,Jc,4s,2c,3c,", allVisibleCards);
    std::vector<LYCard> outs = seat->caculateOuts(&hs, communityCards, allVisibleCards);
    std::string str;
    LYCardHelpers::cardsToString(outs, str);
//    std::cout << str << std::endl;
    ASSERT_EQ(outs.size(), 9+5+3);
}
