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
#include "Fixture2p.h"
#include <iostream>
#include "src/LYPineappleAlgorithmDelegate.h"
#include "libmrock/common/src/LYUtils.h"

class LYPineappleGame_2p_with_ghost_tests : public ::testing::Test
{
protected:

    void SetUp()
    {
    	pad = new LYPineappleAlgorithmDelegate();
    	fixture = new Fixture2p();
    	pt = fixture->pt;
    	LYSeatPtr btnSeat = pt->fetchSeat(LYSeat2);
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
    Fixture2p* fixture;
    LYPineappleGame* pg;
    LYPineappleTable* pt;
};

TEST_F(LYPineappleGame_2p_with_ghost_tests, init)
{
//	ASSERT_EQ(pg->gameType, 100);
	ASSERT_EQ(pg->deck.deck.size(), 53);
	ASSERT_EQ(pg->btnSeatNo, LYSeat2);
	ASSERT_EQ(pg->dutySeatNo, LYSeat1);
	ASSERT_EQ(pg->actionIndex, 0);
	LYSeatPtr seat = pg->sortedSeats[0];
	ASSERT_EQ(seat->seatNo, LYSeat1);
	seat = pg->sortedSeats[1];
	ASSERT_EQ(seat->seatNo, LYSeat2);
}

TEST_F(LYPineappleGame_2p_with_ghost_tests, gameOver)
{
	fixture->setBricks(fixture->seat1, "QsQh8d", 1);
	fixture->setBricks(fixture->seat1, "KsKhTd7c8c", 2);
	fixture->setBricks(fixture->seat1, "As3s5s8sXn", 3);

	fixture->setBricks(fixture->seat2, "6s6h2d", 1);
	fixture->setBricks(fixture->seat2, "7s7h4d4c8h", 2);
	fixture->setBricks(fixture->seat2, "9h9d9cJsQc", 3);

	pg->gameOver();
	ASSERT_EQ(pg->seat1_vs_seat2.scoreAgainst, 11);
	ASSERT_EQ(pg->seat1_vs_seat2.statusAgainst, LYEVEN);
}

TEST_F(LYPineappleGame_2p_with_ghost_tests, gameOver2)
{
	fixture->setBricks(fixture->seat1, "QsQhXn", 1);
	fixture->setBricks(fixture->seat1, "KsKhKd7c8c", 2);
	fixture->setBricks(fixture->seat1, "As3s5s8s2s", 3);

	fixture->setBricks(fixture->seat2, "6s6h2d", 1);
	fixture->setBricks(fixture->seat2, "7s7h4d4c8h", 2);
	fixture->setBricks(fixture->seat2, "9h9d9cJsQc", 3);

	pg->gameOver();
	ASSERT_EQ(pg->seat1_vs_seat2.scoreAgainst, 31);
	ASSERT_EQ(pg->seat1_vs_seat2.statusAgainst, LYBEATALL);
}

TEST_F(LYPineappleGame_2p_with_ghost_tests, gameOver3)
{
	fixture->setBricks(fixture->seat1, "JhTs7d", 1);
	fixture->setBricks(fixture->seat1, "6s2s5s4dXn", 2);
	fixture->setBricks(fixture->seat1, "AhKhJsJd8s", 3);

	fixture->setBricks(fixture->seat2, "Qc9d6d", 1);
	fixture->setBricks(fixture->seat2, "5h4s9c6cAc", 2);
	fixture->setBricks(fixture->seat2, "KsAdQdTh9h", 3);

	pg->gameOver();
	ASSERT_EQ(pg->seat1->handStrengthOfTop->ranking, HighCard);
//	for (int i=0; i<pg->seat1->handStrengthOfMiddle->rank.size(); i++) {
//		std::cout << pg->seat1->handStrengthOfMiddle->rank[i].toString() << std::endl;
//	}
//	for (int i=0; i<pg->seat1->handStrengthOfMiddle->kicker.size(); i++) {
//		std::cout << pg->seat1->handStrengthOfMiddle->kicker[i].toString() << std::endl;
//	}
	ASSERT_EQ(pg->seat1->handStrengthOfBottom->ranking, OnePair);
	ASSERT_EQ(pg->seat1->handStrengthOfMiddle->ranking, OnePair);
	ASSERT_EQ(pg->seat1_vs_seat2.scoreAgainst, 1);
	ASSERT_EQ(pg->seat1_vs_seat2.statusAgainst, LYEVEN);
}

TEST_F(LYPineappleGame_2p_with_ghost_tests, gameOver4)
{
	fixture->setBricks(fixture->seat1, "XnJs8h", 1);
	fixture->setBricks(fixture->seat1, "3h2hKhTsTh", 2);
	fixture->setBricks(fixture->seat1, "8cQcJhAhAd", 3);

	fixture->setBricks(fixture->seat2, "Ac9c8d", 1);
	fixture->setBricks(fixture->seat2, "6d4c8s4hJd", 2);
	fixture->setBricks(fixture->seat2, "7s9d4dJc9h", 3);

	pg->gameOver();
//	for (int i=0; i<pg->seat1->handStrengthOfTop->rank.size(); i++) {
//		std::cout << pg->seat1->handStrengthOfTop->rank[i].toString() << std::endl;
//	}
//	for (int i=0; i<pg->seat1->handStrengthOfTop->kicker.size(); i++) {
//		std::cout << pg->seat1->handStrengthOfTop->kicker[i].toString() << std::endl;
//	}
	ASSERT_EQ(pg->seat1->handStrengthOfTop->ranking, OnePair);
	ASSERT_EQ(pg->seat1->handStrengthOfBottom->ranking, OnePair);
	ASSERT_EQ(pg->seat1->handStrengthOfMiddle->ranking, OnePair);
	ASSERT_EQ(pg->seat1_vs_seat2.scoreAgainst, 9);
	ASSERT_EQ(pg->seat1_vs_seat2.statusAgainst, LYBEATALL);
}

TEST_F(LYPineappleGame_2p_with_ghost_tests, gameOver5)
{
	fixture->setBricks(fixture->seat1, "Jh9cQc", 1);
	fixture->setBricks(fixture->seat1, "2c5h6dAc7d", 2);
	fixture->setBricks(fixture->seat1, "As5s4s2sXn", 3);

	fixture->setBricks(fixture->seat2, "Ac9c8d", 1);
	fixture->setBricks(fixture->seat2, "6d4c8s4hJd", 2);
	fixture->setBricks(fixture->seat2, "7s9d4dJc9h", 3);

	pg->gameOver();
//	for (int i=0; i<pg->seat1->handStrengthOfBottom->rank.size(); i++) {
//		std::cout << pg->seat1->handStrengthOfBottom->rank[i].toString() << std::endl;
//	}
//	for (int i=0; i<pg->seat1->handStrengthOfBottom->kicker.size(); i++) {
//		std::cout << pg->seat1->handStrengthOfBottom->kicker[i].toString() << std::endl;
//	}
	ASSERT_EQ(pg->seat1->handStrengthOfTop->ranking, HighCard);
	ASSERT_EQ(pg->seat1->handStrengthOfMiddle->ranking, HighCard);
	ASSERT_EQ(pg->seat1->handStrengthOfBottom->ranking, StraightFlush);
	ASSERT_EQ(pg->seat1_vs_seat2.scoreAgainst, 14);
	ASSERT_EQ(pg->seat1_vs_seat2.statusAgainst, LYEVEN);
}

TEST_F(LYPineappleGame_2p_with_ghost_tests, gameOver6)
{
	fixture->setBricks(fixture->seat1, "KhKcAh", 1);
	fixture->setBricks(fixture->seat1, "6h6s7h7s9s", 2);
	fixture->setBricks(fixture->seat1, "QcQdQhXnTc", 3);

	fixture->setBricks(fixture->seat2, "Ac9c8d", 1);
	fixture->setBricks(fixture->seat2, "6d4c8s4hJd", 2);
	fixture->setBricks(fixture->seat2, "7s9d4dJc9h", 3);

	pg->gameOver();
//	for (int i=0; i<pg->seat1->handStrengthOfBottom->rank.size(); i++) {
//		std::cout << pg->seat1->handStrengthOfBottom->rank[i].toString() << std::endl;
//	}
//	for (int i=0; i<pg->seat1->handStrengthOfBottom->kicker.size(); i++) {
//		std::cout << pg->seat1->handStrengthOfBottom->kicker[i].toString() << std::endl;
//	}
	ASSERT_EQ(pg->seat1->handStrengthOfTop->ranking, OnePair);
	ASSERT_EQ(pg->seat1->handStrengthOfMiddle->ranking, TwoPair);
	ASSERT_EQ(pg->seat1->handStrengthOfBottom->ranking, FourOfAKind);
	ASSERT_EQ(pg->seat1_vs_seat2.scoreAgainst, 24);
	ASSERT_EQ(pg->seat1_vs_seat2.statusAgainst, LYBEATALL);
}

TEST_F(LYPineappleGame_2p_with_ghost_tests, gameOver7)
{
	fixture->setBricks(fixture->seat1, "AdKdTs", 1);
	fixture->setBricks(fixture->seat1, "8c4hJdTcJh", 2);
	fixture->setBricks(fixture->seat1, "4s7s7dXnQc", 3);

	fixture->setBricks(fixture->seat2, "KcKsAs", 1);
	fixture->setBricks(fixture->seat2, "AcAhQh8s9d", 2);
	fixture->setBricks(fixture->seat2, "6s4d3h5c7h", 3);

	pg->gameOver();
//	for (int i=0; i<pg->seat1->handStrengthOfBottom->rank.size(); i++) {
//		std::cout << pg->seat1->handStrengthOfBottom->rank[i].toString() << std::endl;
//	}
//	for (int i=0; i<pg->seat1->handStrengthOfBottom->kicker.size(); i++) {
//		std::cout << pg->seat1->handStrengthOfBottom->kicker[i].toString() << std::endl;
//	}
	ASSERT_EQ(pg->seat1->handStrengthOfTop->ranking, HighCard);
	ASSERT_EQ(pg->seat1->handStrengthOfMiddle->ranking, OnePair);
	ASSERT_EQ(pg->seat1->handStrengthOfBottom->ranking, ThreeOfAKind);
	ASSERT_EQ(pg->seat1_vs_seat2.scoreAgainst, -16);
	ASSERT_EQ(pg->seat1_vs_seat2.statusAgainst, LYSCOOPED);
}

TEST_F(LYPineappleGame_2p_with_ghost_tests, gameOver8)
{
	fixture->setBricks(fixture->seat1, "AhTs6h", 1);
	fixture->setBricks(fixture->seat1, "5s3cJd9sAd", 2);
	fixture->setBricks(fixture->seat1, "QcKsThKd6d", 3);

	fixture->setBricks(fixture->seat2, "QsTd4d", 1);
	fixture->setBricks(fixture->seat2, "3h2dAsJs4c", 2);
	fixture->setBricks(fixture->seat2, "2hXnJcKcQh", 3);

	pg->gameOver();
//	for (int i=0; i<pg->seat1->handStrengthOfBottom->rank.size(); i++) {
//		std::cout << pg->seat1->handStrengthOfBottom->rank[i].toString() << std::endl;
//	}
//	for (int i=0; i<pg->seat1->handStrengthOfBottom->kicker.size(); i++) {
//		std::cout << pg->seat1->handStrengthOfBottom->kicker[i].toString() << std::endl;
//	}
	ASSERT_EQ(pg->seat1->handStrengthOfTop->ranking, HighCard);
	ASSERT_EQ(pg->seat1->handStrengthOfMiddle->ranking, HighCard);
	ASSERT_EQ(pg->seat1->handStrengthOfBottom->ranking, OnePair);
	ASSERT_EQ(pg->seat2->handStrengthOfTop->ranking, HighCard);
	ASSERT_EQ(pg->seat2->handStrengthOfMiddle->ranking, HighCard);
	ASSERT_EQ(pg->seat2->handStrengthOfBottom->ranking, OnePair);
	ASSERT_EQ(pg->seat1_vs_seat2.scoreAgainst, 1);
	ASSERT_EQ(pg->seat1_vs_seat2.statusAgainst, LYEVEN);
}

TEST_F(LYPineappleGame_2p_with_ghost_tests, gameOver9)
{
	fixture->setBricks(fixture->seat1, "AhTs6h", 1);
	fixture->setBricks(fixture->seat1, "5s3cJd9sAd", 2);
	fixture->setBricks(fixture->seat1, "QcKsThKd6d", 3);

	fixture->setBricks(fixture->seat2, "KhQd7s", 1);
	fixture->setBricks(fixture->seat2, "4sAc5d2s3d", 2);
	fixture->setBricks(fixture->seat2, "6c8s7c5c9d", 3);

	pg->gameOver();
//	for (int i=0; i<pg->seat1->handStrengthOfBottom->rank.size(); i++) {
//		std::cout << pg->seat1->handStrengthOfBottom->rank[i].toString() << std::endl;
//	}
//	for (int i=0; i<pg->seat1->handStrengthOfBottom->kicker.size(); i++) {
//		std::cout << pg->seat1->handStrengthOfBottom->kicker[i].toString() << std::endl;
//	}
	ASSERT_EQ(pg->seat1->handStrengthOfTop->ranking, HighCard);
	ASSERT_EQ(pg->seat1->handStrengthOfMiddle->ranking, HighCard);
	ASSERT_EQ(pg->seat1->handStrengthOfBottom->ranking, OnePair);
	ASSERT_EQ(pg->seat2->handStrengthOfTop->ranking, HighCard);
	ASSERT_EQ(pg->seat2->handStrengthOfMiddle->ranking, Straight);
	ASSERT_EQ(pg->seat2->handStrengthOfBottom->ranking, Straight);
//	std::cout << pg->seat1_vs_seat2.toString() << std::endl;
	LYHandStrength* hs1 = pg->seat2->handStrengthOfMiddle.get();
	LYHandStrength* hs2 = pg->seat2->handStrengthOfBottom.get();
	ASSERT_EQ(hs1->topRankCard, d5);
	ASSERT_EQ(hs2->topRankCard, d9);
	ASSERT_EQ((*hs1 > *hs2), false);
	ASSERT_EQ(pg->seat1_vs_seat2.scoreAgainst, -7);
	ASSERT_EQ(pg->seat1_vs_seat2.statusAgainst, LYEVEN);
}

TEST_F(LYPineappleGame_2p_with_ghost_tests, gameOver10)
{
	fixture->setBricks(fixture->seat1, "Qd9h7d", 1);
	fixture->setBricks(fixture->seat1, "7c5sKsKcQh", 2);
	fixture->setBricks(fixture->seat1, "TsXn4hTh8h", 3);

	fixture->setBricks(fixture->seat2, "QcJd5c", 1);
	fixture->setBricks(fixture->seat2, "Ad3h8c9d4d", 2);
	fixture->setBricks(fixture->seat2, "Js4s3s6s9s", 3);

	pg->gameOver();
//	for (int i=0; i<pg->seat1->handStrengthOfBottom->rank.size(); i++) {
//		std::cout << pg->seat1->handStrengthOfBottom->rank[i].toString() << std::endl;
//	}
//	for (int i=0; i<pg->seat1->handStrengthOfBottom->kicker.size(); i++) {
//		std::cout << pg->seat1->handStrengthOfBottom->kicker[i].toString() << std::endl;
//	}
	ASSERT_EQ(pg->seat1->handStrengthOfTop->ranking, HighCard);
	ASSERT_EQ(pg->seat1->handStrengthOfMiddle->ranking, OnePair);
	ASSERT_EQ(pg->seat1->handStrengthOfBottom->ranking, ThreeOfAKind);
	ASSERT_EQ(pg->seat2->handStrengthOfTop->ranking, HighCard);
	ASSERT_EQ(pg->seat2->handStrengthOfMiddle->ranking, HighCard);
	ASSERT_EQ(pg->seat2->handStrengthOfBottom->ranking, Flush);
//	std::cout << pg->seat1_vs_seat2.toString() << std::endl;
	LYHandStrength* hs1 = pg->seat1->handStrengthOfMiddle.get();
	LYHandStrength* hs2 = pg->seat1->handStrengthOfBottom.get();
	ASSERT_EQ(hs1->topRankCard, sK);
	ASSERT_EQ(hs2->topRankCard, sT);
	ASSERT_EQ((*hs1 > *hs2), false);
	ASSERT_EQ(pg->seat1_vs_seat2.scoreAgainst, -5);
	ASSERT_EQ(pg->seat1_vs_seat2.statusAgainst, LYEVEN);
}

TEST_F(LYPineappleGame_2p_with_ghost_tests, gameOver11)
{
	fixture->setBricks(fixture->seat1, "AsKsJc", 1);
	fixture->setBricks(fixture->seat1, "9s4d7s4hJs", 2);
	fixture->setBricks(fixture->seat1, "Ts9c8hAhAd", 3);

	fixture->setBricks(fixture->seat2, "AcJhTh", 1);
	fixture->setBricks(fixture->seat2, "8d4cQd7c8c", 2);
	fixture->setBricks(fixture->seat2, "6sXnQcQh5s", 3);

	pg->gameOver();
//	for (int i=0; i<pg->seat1->handStrengthOfBottom->rank.size(); i++) {
//		std::cout << pg->seat1->handStrengthOfBottom->rank[i].toString() << std::endl;
//	}
//	for (int i=0; i<pg->seat1->handStrengthOfBottom->kicker.size(); i++) {
//		std::cout << pg->seat1->handStrengthOfBottom->kicker[i].toString() << std::endl;
//	}
	ASSERT_EQ(pg->seat1->handStrengthOfTop->ranking, HighCard);
	ASSERT_EQ(pg->seat1->handStrengthOfMiddle->ranking, OnePair);
	ASSERT_EQ(pg->seat1->handStrengthOfBottom->ranking, OnePair);
	ASSERT_EQ(pg->seat2->handStrengthOfTop->ranking, HighCard);
	ASSERT_EQ(pg->seat2->handStrengthOfMiddle->ranking, OnePair);
	ASSERT_EQ(pg->seat2->handStrengthOfBottom->ranking, ThreeOfAKind);
//	std::cout << pg->seat1_vs_seat2.toString() << std::endl;
	ASSERT_EQ(pg->seat1_vs_seat2.scoreAgainst, -1);
	ASSERT_EQ(pg->seat1_vs_seat2.statusAgainst, LYEVEN);
}

TEST_F(LYPineappleGame_2p_with_ghost_tests, gameOver12)
{
	fixture->setBricks(fixture->seat1, "QdKd6h", 1);
	fixture->setBricks(fixture->seat1, "Xn5cAc4c2s", 2);
	fixture->setBricks(fixture->seat1, "Jh9hTs7c8d", 3);

	fixture->setBricks(fixture->seat2, "AcJhTh", 1);
	fixture->setBricks(fixture->seat2, "8d4cQd7c8c", 2);
	fixture->setBricks(fixture->seat2, "6sXnQcQh5s", 3);

	pg->gameOver();
//	for (int i=0; i<pg->seat1->handStrengthOfBottom->rank.size(); i++) {
//		std::cout << pg->seat1->handStrengthOfBottom->rank[i].toString() << std::endl;
//	}
//	for (int i=0; i<pg->seat1->handStrengthOfBottom->kicker.size(); i++) {
//		std::cout << pg->seat1->handStrengthOfBottom->kicker[i].toString() << std::endl;
//	}
	ASSERT_EQ(pg->seat1->handStrengthOfTop->ranking, HighCard);
	ASSERT_EQ(pg->seat1->handStrengthOfMiddle->ranking, Straight);
	ASSERT_EQ(pg->seat1->handStrengthOfBottom->ranking, Straight);
	ASSERT_EQ(pg->seat2->handStrengthOfTop->ranking, HighCard);
	ASSERT_EQ(pg->seat2->handStrengthOfMiddle->ranking, OnePair);
	ASSERT_EQ(pg->seat2->handStrengthOfBottom->ranking, ThreeOfAKind);
//	std::cout << pg->seat1_vs_seat2.toString() << std::endl;
	LYHandStrength* hs1 = pg->seat1->handStrengthOfMiddle.get();
	LYHandStrength* hs2 = pg->seat1->handStrengthOfBottom.get();
	ASSERT_EQ(hs1->topRankCard, c5);
	ASSERT_EQ(hs2->topRankCard, hJ);
	ASSERT_EQ((*hs1 > *hs2), false);
	ASSERT_EQ(pg->seat1_vs_seat2.scoreAgainst, 7);
	ASSERT_EQ(pg->seat1_vs_seat2.statusAgainst, LYEVEN);
}

TEST_F(LYPineappleGame_2p_with_ghost_tests, gameOver13)
{
	fixture->setBricks(fixture->seat1, "Jh9dKc", 1);
	fixture->setBricks(fixture->seat1, "TdThQdQs6d", 2);
	fixture->setBricks(fixture->seat1, "8c8d8h4c4s", 3);

	fixture->setBricks(fixture->seat2, "QhJc9h", 1);
	fixture->setBricks(fixture->seat2, "7h3d3c3h9s", 2);
	fixture->setBricks(fixture->seat2, "AhAsKhKdXn", 3);

	pg->gameOver();
//	for (int i=0; i<pg->seat1->handStrengthOfBottom->rank.size(); i++) {
//		std::cout << pg->seat1->handStrengthOfBottom->rank[i].toString() << std::endl;
//	}
//	for (int i=0; i<pg->seat1->handStrengthOfBottom->kicker.size(); i++) {
//		std::cout << pg->seat1->handStrengthOfBottom->kicker[i].toString() << std::endl;
//	}
	ASSERT_EQ(pg->seat1->handStrengthOfTop->ranking, HighCard);
	ASSERT_EQ(pg->seat1->handStrengthOfMiddle->ranking, TwoPair);
	ASSERT_EQ(pg->seat1->handStrengthOfBottom->ranking, FullHouse);
	ASSERT_EQ(pg->seat2->handStrengthOfTop->ranking, HighCard);
	ASSERT_EQ(pg->seat2->handStrengthOfMiddle->ranking, ThreeOfAKind);
	ASSERT_EQ(pg->seat2->handStrengthOfBottom->ranking, FullHouse);
//	std::cout << pg->seat1_vs_seat2.toString() << std::endl;
	LYHandStrength* hs1 = pg->seat2->handStrengthOfMiddle.get();
	LYHandStrength* hs2 = pg->seat2->handStrengthOfBottom.get();
	ASSERT_EQ(hs1->topRankCard, h3);
	ASSERT_EQ(hs2->topRankCard, sA);
	ASSERT_EQ((*hs1 > *hs2), false);
	ASSERT_EQ(pg->seat1_vs_seat2.scoreAgainst, -3);
	ASSERT_EQ(pg->seat1_vs_seat2.statusAgainst, LYEVEN);
}

TEST_F(LYPineappleGame_2p_with_ghost_tests, gameOver14)
{
	fixture->setBricks(fixture->seat1, "Kh8cKs", 1);
	fixture->setBricks(fixture->seat1, "Jd3dJh7c3s", 2);
	fixture->setBricks(fixture->seat1, "2h2c6cXn6h", 3);

	fixture->setBricks(fixture->seat2, "Js8s5c", 1);
	fixture->setBricks(fixture->seat2, "3h3c7s6sQd", 2);
	fixture->setBricks(fixture->seat2, "TcAsQsJs8h", 3);

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
//	std::cout << pg->seat1_vs_seat2.toString() << std::endl;
	LYHandStrength* hs1 = pg->seat1->handStrengthOfMiddle.get();
	LYHandStrength* hs2 = pg->seat1->handStrengthOfBottom.get();
	ASSERT_EQ(hs1->topRankCard, hJ);
	ASSERT_EQ(hs2->topRankCard, h6);
	ASSERT_EQ((*hs1 > *hs2), false);
	ASSERT_EQ(pg->seat1_vs_seat2.scoreAgainst, 20);
	ASSERT_EQ(pg->seat1_vs_seat2.statusAgainst, LYBEATALL);
}

TEST_F(LYPineappleGame_2p_with_ghost_tests, gameOver15)
{
	fixture->setBricks(fixture->seat1, "9dAcAs", 1);
	fixture->setBricks(fixture->seat1, "JsTdJcXn9h", 2);
	fixture->setBricks(fixture->seat1, "6h7s8c9cTc", 3);

	fixture->setBricks(fixture->seat2, "KhKcJc", 1);
	fixture->setBricks(fixture->seat2, "7h5d6s6cQs", 2);
	fixture->setBricks(fixture->seat2, "8s8hAdKc3d", 3);

	pg->gameOver();
//	for (int i=0; i<pg->seat1->handStrengthOfMiddle->rank.size(); i++) {
//		std::cout << pg->seat1->handStrengthOfMiddle->rank[i].toString() << std::endl;
//	}
//	for (int i=0; i<pg->seat1->handStrengthOfMiddle->kicker.size(); i++) {
//		std::cout << pg->seat1->handStrengthOfMiddle->kicker[i].toString() << std::endl;
//	}
	ASSERT_EQ(pg->seat1->handStrengthOfTop->ranking, OnePair);
//	ASSERT_EQ(pg->seat1->handStrengthOfMiddle->ranking, ThreeOfAKind);
	ASSERT_EQ(pg->seat1->handStrengthOfBottom->ranking, Straight);
	ASSERT_EQ(pg->seat2->handStrengthOfTop->ranking, OnePair);
	ASSERT_EQ(pg->seat2->handStrengthOfMiddle->ranking, OnePair);
	ASSERT_EQ(pg->seat2->handStrengthOfBottom->ranking, OnePair);
//	std::cout << pg->seat1_vs_seat2.toString() << std::endl;
	LYHandStrength* hs1 = pg->seat1->handStrengthOfMiddle.get();
	LYHandStrength* hs2 = pg->seat1->handStrengthOfBottom.get();
	ASSERT_EQ(hs1->topRankCard, sJ);
	ASSERT_EQ((*hs1 > *hs2), false);
	ASSERT_EQ(pg->seat1_vs_seat2.scoreAgainst, 19);
	ASSERT_EQ(pg->seat1_vs_seat2.statusAgainst, LYBEATALL);
}

TEST_F(LYPineappleGame_2p_with_ghost_tests, gameOver16)
{
	fixture->setBricks(fixture->seat1, "QsThQc", 1);
	fixture->setBricks(fixture->seat1, "AdJsJh6dXn", 2);
	fixture->setBricks(fixture->seat1, "2h2sKsKh7h", 3);

	fixture->setBricks(fixture->seat2, "QdTcTd", 1);
	fixture->setBricks(fixture->seat2, "Jc3c3s3d9d", 2);
	fixture->setBricks(fixture->seat2, "Ks4s4dAcAs", 3);

	pg->gameOver();
//	for (int i=0; i<pg->seat1->handStrengthOfMiddle->rank.size(); i++) {
//		std::cout << pg->seat1->handStrengthOfMiddle->rank[i].toString() << std::endl;
//	}
//	for (int i=0; i<pg->seat1->handStrengthOfMiddle->kicker.size(); i++) {
//		std::cout << pg->seat1->handStrengthOfMiddle->kicker[i].toString() << std::endl;
//	}
	ASSERT_EQ(pg->seat1->handStrengthOfTop->ranking, OnePair);
	ASSERT_EQ(pg->seat1->handStrengthOfMiddle->ranking, TwoPair);
	ASSERT_EQ(pg->seat1->handStrengthOfBottom->ranking, TwoPair);
	ASSERT_EQ(pg->seat2->handStrengthOfTop->ranking, OnePair);
	ASSERT_EQ(pg->seat2->handStrengthOfMiddle->ranking, ThreeOfAKind);
	ASSERT_EQ(pg->seat2->handStrengthOfBottom->ranking, TwoPair);
//	std::cout << pg->seat1_vs_seat2.toString() << std::endl;
	LYHandStrength* hs1 = pg->seat1->handStrengthOfMiddle.get();
	LYHandStrength* hs2 = pg->seat1->handStrengthOfBottom.get();
	ASSERT_EQ(hs1->topRankCard, sJ);
	ASSERT_EQ((*hs1 > *hs2), false);
	ASSERT_EQ(pg->seat1_vs_seat2.scoreAgainst, 13);
	ASSERT_EQ(pg->seat1_vs_seat2.statusAgainst, LYBEATALL);
}

TEST_F(LYPineappleGame_2p_with_ghost_tests, gameOver17)
{
	fixture->setBricks(fixture->seat1, "JdQh6d", 1);
	fixture->setBricks(fixture->seat1, "9s6c9hXn9d", 2);
	fixture->setBricks(fixture->seat1, "KhKdKsKc5d", 3);

	fixture->setBricks(fixture->seat2, "QsQd7s", 1);
	fixture->setBricks(fixture->seat2, "Jh8cTc8h2c", 2);
	fixture->setBricks(fixture->seat2, "5c5s3s7h6h", 3);

	pg->gameOver();
//	for (int i=0; i<pg->seat1->handStrengthOfMiddle->rank.size(); i++) {
//		std::cout << pg->seat1->handStrengthOfMiddle->rank[i].toString() << std::endl;
//	}
//	for (int i=0; i<pg->seat1->handStrengthOfMiddle->kicker.size(); i++) {
//		std::cout << pg->seat1->handStrengthOfMiddle->kicker[i].toString() << std::endl;
//	}
	ASSERT_EQ(pg->seat1->handStrengthOfTop->ranking, HighCard);
	ASSERT_EQ(pg->seat1->handStrengthOfMiddle->ranking, FourOfAKind);
	ASSERT_EQ(pg->seat1->handStrengthOfBottom->ranking, FourOfAKind);
	ASSERT_EQ(pg->seat2->handStrengthOfTop->ranking, OnePair);
	ASSERT_EQ(pg->seat2->handStrengthOfMiddle->ranking, OnePair);
	ASSERT_EQ(pg->seat2->handStrengthOfBottom->ranking, OnePair);
//	std::cout << pg->seat1_vs_seat2.toString() << std::endl;
	LYHandStrength* hs1 = pg->seat1->handStrengthOfMiddle.get();
	LYHandStrength* hs2 = pg->seat1->handStrengthOfBottom.get();
	ASSERT_EQ(hs1->topRankCard, s9);
	ASSERT_EQ((*hs1 > *hs2), false);
	ASSERT_EQ(pg->seat1_vs_seat2.scoreAgainst, 36);
	ASSERT_EQ(pg->seat1_vs_seat2.statusAgainst, LYBEATALL);
}

TEST_F(LYPineappleGame_2p_with_ghost_tests, gameOver18)
{
	fixture->setBricks(fixture->seat1, "KhQd9d", 1);
	fixture->setBricks(fixture->seat1, "Th4s6dTcTd", 2);
	fixture->setBricks(fixture->seat1, "7s7dXnAh7h", 3);

	fixture->setBricks(fixture->seat2, "KsAdKd", 1);
	fixture->setBricks(fixture->seat2, "6c5c8s9c7c", 2);
	fixture->setBricks(fixture->seat2, "Jh4h3h2h6h", 3);

	pg->gameOver();
//	for (int i=0; i<pg->seat1->handStrengthOfMiddle->rank.size(); i++) {
//		std::cout << pg->seat1->handStrengthOfMiddle->rank[i].toString() << std::endl;
//	}
//	for (int i=0; i<pg->seat1->handStrengthOfMiddle->kicker.size(); i++) {
//		std::cout << pg->seat1->handStrengthOfMiddle->kicker[i].toString() << std::endl;
//	}
	ASSERT_EQ(pg->seat1->handStrengthOfTop->ranking, HighCard);
	ASSERT_EQ(pg->seat1->handStrengthOfMiddle->ranking, ThreeOfAKind);
	ASSERT_EQ(pg->seat1->handStrengthOfBottom->ranking, FourOfAKind);
	ASSERT_EQ(pg->seat2->handStrengthOfTop->ranking, OnePair);
	ASSERT_EQ(pg->seat2->handStrengthOfMiddle->ranking, Straight);
	ASSERT_EQ(pg->seat2->handStrengthOfBottom->ranking, Flush);
//	std::cout << pg->seat1_vs_seat2.toString() << std::endl;
	LYHandStrength* hs1 = pg->seat1->handStrengthOfMiddle.get();
	LYHandStrength* hs2 = pg->seat1->handStrengthOfBottom.get();
	ASSERT_EQ((*hs1 > *hs2), false);
	ASSERT_EQ(pg->seat1_vs_seat2.scoreAgainst, -5);
	ASSERT_EQ(pg->seat1_vs_seat2.statusAgainst, LYEVEN);
}

TEST_F(LYPineappleGame_2p_with_ghost_tests, gameOver19)
{
	fixture->setBricks(fixture->seat1, "JdAsAc", 1);
	fixture->setBricks(fixture->seat1, "8h8s9s9c6s", 2);
	fixture->setBricks(fixture->seat1, "3d3sXnAhKd", 3);

	fixture->setBricks(fixture->seat2, "KsAdKd", 1);
	fixture->setBricks(fixture->seat2, "6c5c8s9c7c", 2);
	fixture->setBricks(fixture->seat2, "Jh4h3h2h6h", 3);

	pg->gameOver();
//	for (int i=0; i<pg->seat1->handStrengthOfMiddle->rank.size(); i++) {
//		std::cout << pg->seat1->handStrengthOfMiddle->rank[i].toString() << std::endl;
//	}
//	for (int i=0; i<pg->seat1->handStrengthOfMiddle->kicker.size(); i++) {
//		std::cout << pg->seat1->handStrengthOfMiddle->kicker[i].toString() << std::endl;
//	}
	ASSERT_EQ(pg->seat1->handStrengthOfTop->ranking, OnePair);
	ASSERT_EQ(pg->seat1->handStrengthOfMiddle->ranking, TwoPair);
	ASSERT_EQ(pg->seat1->handStrengthOfBottom->ranking, ThreeOfAKind);
	ASSERT_EQ(pg->seat2->handStrengthOfTop->ranking, OnePair);
	ASSERT_EQ(pg->seat2->handStrengthOfMiddle->ranking, Straight);
	ASSERT_EQ(pg->seat2->handStrengthOfBottom->ranking, Flush);
//	std::cout << pg->seat1_vs_seat2.toString() << std::endl;
	LYHandStrength* hs1 = pg->seat1->handStrengthOfMiddle.get();
	LYHandStrength* hs2 = pg->seat1->handStrengthOfBottom.get();
	ASSERT_EQ((*hs1 > *hs2), false);
	ASSERT_EQ(pg->seat1_vs_seat2.scoreAgainst, -8);
	ASSERT_EQ(pg->seat1_vs_seat2.statusAgainst, LYEVEN);
}

TEST_F(LYPineappleGame_2p_with_ghost_tests, gameOver20)
{
	fixture->setBricks(fixture->seat1, "JdAsAc", 1);
	fixture->setBricks(fixture->seat1, "AcQcJc8c2c", 2);
	fixture->setBricks(fixture->seat1, "6s7s8s9sXn", 3);

	fixture->setBricks(fixture->seat2, "KsAdKd", 1);
	fixture->setBricks(fixture->seat2, "6c5c8s9c7c", 2);
	fixture->setBricks(fixture->seat2, "Jh4h3h2h6h", 3);

	pg->gameOver();
//	for (int i=0; i<pg->seat1->handStrengthOfMiddle->rank.size(); i++) {
//		std::cout << pg->seat1->handStrengthOfMiddle->rank[i].toString() << std::endl;
//	}
//	for (int i=0; i<pg->seat1->handStrengthOfMiddle->kicker.size(); i++) {
//		std::cout << pg->seat1->handStrengthOfMiddle->kicker[i].toString() << std::endl;
//	}
	ASSERT_EQ(pg->seat1->handStrengthOfTop->ranking, OnePair);
	ASSERT_EQ(pg->seat1->handStrengthOfMiddle->ranking, Flush);
	ASSERT_EQ(pg->seat1->handStrengthOfBottom->ranking, StraightFlush);
	ASSERT_EQ(pg->seat2->handStrengthOfTop->ranking, OnePair);
	ASSERT_EQ(pg->seat2->handStrengthOfMiddle->ranking, Straight);
	ASSERT_EQ(pg->seat2->handStrengthOfBottom->ranking, Flush);
//	std::cout << pg->seat1_vs_seat2.toString() << std::endl;
	LYHandStrength* hs1 = pg->seat1->handStrengthOfMiddle.get();
	LYHandStrength* hs2 = pg->seat1->handStrengthOfBottom.get();
	ASSERT_EQ((*hs1 > *hs2), false);
	ASSERT_EQ(pg->seat1_vs_seat2.scoreAgainst, 22);
	ASSERT_EQ(pg->seat1_vs_seat2.statusAgainst, LYBEATALL);
}

TEST_F(LYPineappleGame_2p_with_ghost_tests, nextGame)
{
	fixture->setBricks(fixture->seat1, "QsQhXn", 1);
	fixture->setBricks(fixture->seat1, "KsKhKd7c8c", 2);
	fixture->setBricks(fixture->seat1, "As3s5s8s2s", 3);

	fixture->setBricks(fixture->seat2, "6s6h2d", 1);
	fixture->setBricks(fixture->seat2, "7s7h4d4c8h", 2);
	fixture->setBricks(fixture->seat2, "9h9d9cJsQc", 3);

	pg->gameOver();

	pt->createGame(LYUtils::genId(), pad, "private_withking");
	LYPineappleGame* newpg = (LYPineappleGame*)(pt->getCurrentGame());
//	ASSERT_EQ(newpg->gameType, 100);
	ASSERT_EQ(newpg->deck.deck.size(), 53);
	ASSERT_EQ(pg->seat1->gameStatus, LYFANTASY);
}

TEST_F(LYPineappleGame_2p_with_ghost_tests, nextGame2)
{
	fixture->setBricks(fixture->seat1, "QsQhXn", 1);
	fixture->setBricks(fixture->seat1, "KsKhKd7c8c", 2);
	fixture->setBricks(fixture->seat1, "As3s5s8s2s", 3);

	fixture->setBricks(fixture->seat2, "6s6h2d", 1);
	fixture->setBricks(fixture->seat2, "7s7h4d4c8h", 2);
	fixture->setBricks(fixture->seat2, "9h9d9cJsQc", 3);

	pg->gameOver();

	pt->createGame(LYUtils::genId(), pad, "private_withking");
	LYPineappleGame* newpg = (LYPineappleGame*)(pt->getCurrentGame());
//	ASSERT_EQ(newpg->gameType, 100);
	ASSERT_EQ(newpg->deck.deck.size(), 53);
	ASSERT_EQ(pg->seat1->gameStatus, LYFANTASY);

	fixture->setBricks(fixture->seat1, "KsKcXn", 1);
	fixture->setBricks(fixture->seat1, "Jd3d3c7c7h", 2);
	fixture->setBricks(fixture->seat1, "ThTcAsTd5s", 3);

	fixture->setBricks(fixture->seat2, "6s6h2d", 1);
	fixture->setBricks(fixture->seat2, "7s7h4d4c8h", 2);
	fixture->setBricks(fixture->seat2, "9h9d9cJsQc", 3);

	newpg->gameOver();
	ASSERT_EQ(pg->seat1->gameStatus, LYNORMAL);

}

TEST_F(LYPineappleGame_2p_with_ghost_tests, nextGame3)
{
	fixture->setBricks(fixture->seat1, "QsQhXn", 1);
	fixture->setBricks(fixture->seat1, "KsKhKd7c8c", 2);
	fixture->setBricks(fixture->seat1, "As3s5s8s2s", 3);

	fixture->setBricks(fixture->seat2, "6s6h2d", 1);
	fixture->setBricks(fixture->seat2, "7s7h4d4c8h", 2);
	fixture->setBricks(fixture->seat2, "9h9d9cJsQc", 3);

	pg->gameOver();

	pt->createGame(LYUtils::genId(), pad, "private_withking");
	LYPineappleGame* newpg = (LYPineappleGame*)(pt->getCurrentGame());
//	ASSERT_EQ(newpg->gameType, 100);
	ASSERT_EQ(newpg->deck.deck.size(), 53);
	ASSERT_EQ(pg->seat1->gameStatus, LYFANTASY);

	fixture->setBricks(fixture->seat1, "KsKc5s", 1);
	fixture->setBricks(fixture->seat1, "Jd3d3c7c7h", 2);
	fixture->setBricks(fixture->seat1, "ThTcAsTdXn", 3);

	fixture->setBricks(fixture->seat2, "6s6h2d", 1);
	fixture->setBricks(fixture->seat2, "7s7h4d4c8h", 2);
	fixture->setBricks(fixture->seat2, "9h9d9cJsQc", 3);

	newpg->gameOver();
	ASSERT_EQ(pg->seat1->gameStatus, LYFANTASY);

}
