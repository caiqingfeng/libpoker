/*
 * LYTuringStrategy_tests.cpp
 *
 *  Created on: 2014-9-25
 *      Author: caiqingfeng
 */

#include <gtest/gtest.h>

#include "src/LYTuringStrategy.h"
//#include "common/src/my_log.h"
#include "poker/src/LYDeck.h"
#include "Fixture2p.h"
#include <string>
#include "src/LYPineappleGame.h"

class LYTuringStrategy_tests: public ::testing::Test {
protected:

    void SetUp()
    {
    	fixture = new Fixture2p();
    	pt = fixture->pt;
    	btnSeat = pt->fetchSeat(LYSeat2);
    	ps = (LYPineappleSeat *)btnSeat.get();
    	pg = new LYPineappleGame("new game", pt->seats, btnSeat, pt, NULL);
    	//add cards on hand
    	ps->cardsOnHand.push_back(c5);
    	ps->cardsOnHand.push_back(c8);
    	ps->cardsOnHand.push_back(d5);
    	ps->cardsOnHand.push_back(s5);
    	ps->cardsOnHand.push_back(hA);
    }
    void TearDown()
    {
    	delete pg;
    	delete fixture;
   }

public:
    Fixture2p* fixture;
    LYPineappleGame* pg;
    LYPineappleTable* pt;
    LYSeatPtr btnSeat;
	LYPineappleSeat* ps;
};

TEST_F(LYTuringStrategy_tests, makeAction_firstHand)
{
	LYPineappleAction action;
	LYTuringStrategy::Instance()->makeAction(pt, btnSeat->seatNo, action);
	ASSERT_EQ(action.bricks.size(), 5);
	for (unsigned int i=0; i<5; i++) {
		ASSERT_EQ(action.bricks[i].brick, i+1);
	}
}

TEST_F(LYTuringStrategy_tests, makeAction_otherHand)
{
	ps->cardsOnHand.pop_back();
	ps->cardsOnHand.pop_back();
	LYPineappleAction action;
	LYTuringStrategy::Instance()->makeAction(pt, btnSeat->seatNo, action);
	ASSERT_EQ(action.bricks.size(), 2);
}

TEST_F(LYTuringStrategy_tests, makeAction_middleLineHasCards)
{
	ps->middleBricks.push_back(sA);
	LYPineappleAction action;
	LYTuringStrategy::Instance()->makeAction(pt, btnSeat->seatNo, action);
	ASSERT_EQ(action.bricks.size(), 5);
	for (unsigned int i=0; i<5; i++) {
		if (i >= 3) {
			ASSERT_EQ(action.bricks[i].brick, i+2);
		} else {
			ASSERT_EQ(action.bricks[i].brick, i+1);
		}
	}
}

TEST_F(LYTuringStrategy_tests,makeAction_allLineHaveCards_MB)
{
	ps->topBricks.push_back(cT);
	ps->topBricks.push_back(s8);
	ps->topBricks.push_back(h6);
	ps->middleBricks.push_back(d9);
	ps->middleBricks.push_back(h3);
	ps->middleBricks.push_back(d2);
	ps->middleBricks.push_back(d6);
	ps->bottomBricks.push_back(c6);
	ps->bottomBricks.push_back(h8);
	ps->bottomBricks.push_back(sT);
	ps->bottomBricks.push_back(hT);
	LYPineappleAction action;
	LYTuringStrategy::Instance()->makeAction(pt, btnSeat->seatNo, action);
	ASSERT_EQ(action.bricks.size(), 2);
	for (unsigned int i=0; i<2; i++) {
		if (i == 0) {
			ASSERT_EQ(action.bricks[i].brick, i+8);
		} else if (i == 1) {
			ASSERT_EQ(action.bricks[i].brick, i+12);
		}
	}
}

TEST_F(LYTuringStrategy_tests,makeAction_allLineHaveCards_BB)
{
	ps->topBricks.push_back(cT);
	ps->topBricks.push_back(s8);
	ps->topBricks.push_back(h6);
	ps->middleBricks.push_back(d9);
	ps->middleBricks.push_back(h3);
	ps->middleBricks.push_back(d2);
	ps->middleBricks.push_back(d6);
	ps->middleBricks.push_back(c6);
	ps->bottomBricks.push_back(h8);
	ps->bottomBricks.push_back(sT);
	ps->bottomBricks.push_back(hT);
	LYPineappleAction action;
	LYTuringStrategy::Instance()->makeAction(pt, btnSeat->seatNo, action);
	ASSERT_EQ(action.bricks.size(), 2);
	for (unsigned int i=0; i<2; i++) {
		if (i == 0) {
			ASSERT_EQ(action.bricks[i].brick, i+12);
		} else if (i == 1) {
			ASSERT_EQ(action.bricks[i].brick, i+12);
		}
	}
}

TEST_F(LYTuringStrategy_tests,makeAction_allLineHaveCards_TM)
{
	ps->topBricks.push_back(cT);
	ps->topBricks.push_back(s8);
	ps->middleBricks.push_back(d9);
	ps->middleBricks.push_back(d6);
	ps->middleBricks.push_back(c6);
	LYPineappleAction action;
	LYTuringStrategy::Instance()->makeAction(pt, btnSeat->seatNo, action);
	ASSERT_EQ(action.bricks.size(), 5);
	for (unsigned int i=0; i<2; i++) {
		if (i == 0) {
			ASSERT_EQ(action.bricks[i].brick, i+3);
		} else if (i == 1) {
			ASSERT_EQ(action.bricks[i].brick, i+6);
		}
	}
}

TEST_F(LYTuringStrategy_tests,makeAction_allLineHaveCards_TB)
{
	ps->topBricks.push_back(cT);
	ps->topBricks.push_back(s8);
	ps->middleBricks.push_back(d9);
	ps->middleBricks.push_back(h3);
	ps->middleBricks.push_back(d2);
	ps->middleBricks.push_back(h6);
	ps->middleBricks.push_back(d6);
	ps->bottomBricks.push_back(h8);
	ps->bottomBricks.push_back(sT);
	LYPineappleAction action;
	LYTuringStrategy::Instance()->makeAction(pt, btnSeat->seatNo, action);
	ASSERT_EQ(action.bricks.size(), 4);
	for (unsigned int i=0; i<2; i++) {
		if (i == 0) {
			ASSERT_EQ(action.bricks[i].brick, i+3);
		} else if (i == 1) {
			ASSERT_EQ(action.bricks[i].brick, i+10);
		}
	}
}
