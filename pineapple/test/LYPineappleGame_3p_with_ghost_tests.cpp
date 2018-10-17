/*
 * LYPineappleGame_2p_with_ghost_tests.cpp
 *
 *  Created on: 2014-9-25
 *      Author: caiqingfeng
 */

#include <gtest/gtest.h>

#include "src/LYPineappleGame.h"
//#include "common/src/my_log.h"
#include "poker/src/LYDeck.h"
#include "Fixture.h"
#include <iostream>
#include "src/LYPineappleAlgorithmDelegate.h"
#include "libmrock/common/src/LYUtils.h"

class LYPineappleGame_3p_with_ghost_tests : public ::testing::Test
{
protected:

    void SetUp()
    {
    	pad = new LYPineappleAlgorithmDelegate();
    	fixture = new Fixture();
    	pt = fixture->pt;
    	LYSeatPtr btnSeat = pt->fetchSeat(LYSeat3);
    	pg = new LYPineappleGame("new game", pt->seats, btnSeat, pt, pad, false, "private_withking");
    }
    void TearDown()
    {
    	delete pg;
    	delete fixture;
    	delete pad;
   }
public:
    LYPineappleAlgorithmDelegate *pad;
    Fixture* fixture;
    LYPineappleGame* pg;
    LYPineappleTable* pt;
};

TEST_F(LYPineappleGame_3p_with_ghost_tests, init)
{
//	ASSERT_EQ(pg->gameType, 100);
	ASSERT_EQ(pg->deck.deck.size(), 53);
	ASSERT_EQ(pg->btnSeatNo, LYSeat3);
	ASSERT_EQ(pg->dutySeatNo, LYSeat1);
	ASSERT_EQ(pg->actionIndex, 0);
	LYSeatPtr seat = pg->sortedSeats[0];
	ASSERT_EQ(seat->seatNo, LYSeat1);
	seat = pg->sortedSeats[1];
	ASSERT_EQ(seat->seatNo, LYSeat2);
}

TEST_F(LYPineappleGame_3p_with_ghost_tests, gameOver1)
{
	fixture->setBricks(fixture->seat1, "Kh8cKs", 1);
	fixture->setBricks(fixture->seat1, "Jd3dJh7c3s", 2);
	fixture->setBricks(fixture->seat1, "2h2c6cXn6h", 3);

	fixture->setBricks(fixture->seat2, "Js8s5c", 1);
	fixture->setBricks(fixture->seat2, "3h3c7s6sQd", 2);
	fixture->setBricks(fixture->seat2, "TcAsQsJs8h", 3);

	fixture->setBricks(fixture->seat3, "Ac9c8d", 1);
	fixture->setBricks(fixture->seat3, "4h2dQh9sAd", 2);
	fixture->setBricks(fixture->seat3, "7h8hTdQsTh", 3);

	pg->gameOver();
//	for (int i=0; i<pg->seat1->handStrengthOfBottom->rank.size(); i++) {
//		std::cout << pg->seat1->handStrengthOfBottom->rank[i].toString() << std::endl;
//	}
//	for (int i=0; i<pg->seat1->handStrengthOfBottom->kicker.size(); i++) {
//		std::cout << pg->seat1->handStrengthOfBottom->kicker[i].toString() << std::endl;
//	}
	ASSERT_EQ(pg->seat1->handStrengthOfTop->ranking, OnePair);
	ASSERT_EQ(pg->seat1->handStrengthOfMiddle->ranking, TwoPair);
	ASSERT_EQ(pg->seat1->handStrengthOfBottom->ranking, FullHouse);
	ASSERT_EQ(pg->seat2->handStrengthOfTop->ranking, HighCard);
	ASSERT_EQ(pg->seat2->handStrengthOfMiddle->ranking, OnePair);
	ASSERT_EQ(pg->seat2->handStrengthOfBottom->ranking, HighCard);
	ASSERT_EQ(pg->seat3->handStrengthOfTop->ranking, HighCard);
	ASSERT_EQ(pg->seat3->handStrengthOfMiddle->ranking, HighCard);
	ASSERT_EQ(pg->seat3->handStrengthOfBottom->ranking, OnePair);
//	std::cout << pg->seat1_vs_seat2.toString() << std::endl;
	LYHandStrength* hs1 = pg->seat1->handStrengthOfMiddle.get();
	LYHandStrength* hs2 = pg->seat1->handStrengthOfBottom.get();
	ASSERT_EQ(hs1->topRankCard, hJ);
	ASSERT_EQ(hs2->topRankCard, h6);
	ASSERT_EQ((*hs1 > *hs2), false);
	ASSERT_EQ(pg->seat1_vs_seat2.scoreAgainst, 20);
	ASSERT_EQ(pg->seat1_vs_seat2.statusAgainst, LYBEATALL);
	ASSERT_EQ(pg->seat1_vs_seat3.scoreAgainst, 20);
	ASSERT_EQ(pg->seat1_vs_seat3.statusAgainst, LYBEATALL);
}

TEST_F(LYPineappleGame_3p_with_ghost_tests, nextGame)
{
	fixture->setBricks(fixture->seat1, "Kh8cKs", 1);
	fixture->setBricks(fixture->seat1, "Jd3dJh7c3s", 2);
	fixture->setBricks(fixture->seat1, "2h2c6cXn6h", 3);

	fixture->setBricks(fixture->seat2, "Js8s5c", 1);
	fixture->setBricks(fixture->seat2, "3h3c7s6sQd", 2);
	fixture->setBricks(fixture->seat2, "TcAsQsJs8h", 3);

	fixture->setBricks(fixture->seat3, "Ac9c8d", 1);
	fixture->setBricks(fixture->seat3, "4h2dQh9sAd", 2);
	fixture->setBricks(fixture->seat3, "7h8hTdQsTh", 3);

	pg->gameOver();

	pt->createGame(LYUtils::genId(), pad, "private_withking");
	LYPineappleGame* newpg = (LYPineappleGame*)(pt->getCurrentGame());
//	ASSERT_EQ(newpg->gameType, 100);
	ASSERT_EQ(newpg->deck.deck.size(), 53);
}
