/*
 * LYPineappleGame_2p_tests.cpp
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

class LYPineappleGame_2p_tests : public ::testing::Test
{
protected:

    void SetUp()
    {
    	fixture = new Fixture2p();
    	pt = fixture->pt;
    	LYSeatPtr btnSeat = pt->fetchSeat(LYSeat2);
    	pg = new LYPineappleGame("new game", pt->seats, btnSeat, pt, NULL);
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
};

TEST_F(LYPineappleGame_2p_tests, init)
{
	ASSERT_EQ(pg->btnSeatNo, LYSeat2);
	ASSERT_EQ(pg->dutySeatNo, LYSeat1);
	ASSERT_EQ(pg->actionIndex, 0);
	LYSeatPtr seat = pg->sortedSeats[0];
	ASSERT_EQ(seat->seatNo, LYSeat1);
	seat = pg->sortedSeats[1];
	ASSERT_EQ(seat->seatNo, LYSeat2);
}

TEST_F(LYPineappleGame_2p_tests, fantasy)
{
	LYPineappleAction pinup;

	fixture->seat1->gameStatus = LYFANTASY;
	LYSeatPtr btnSeat = pt->fetchSeat(LYSeat1);
	LYPineappleGame* newgame = new LYPineappleGame("new game", pt->seats, btnSeat, pt, NULL);
	ASSERT_EQ(fixture->seat1->gameStatus, LYFANTASY);
	ASSERT_EQ(newgame->actions.size(), 6);

	LYPineappleAction& action = newgame->popAction();
	ASSERT_EQ(action.bricks.size(), 5);
	ASSERT_EQ(action.applicant, LYSEAT2);
	ASSERT_EQ(newgame->dutySeatNo, action.applicant);
	bool ret = newgame->onAction(action);
	ASSERT_EQ(ret, true);
	pinup.pinup(action.table_id, action.applicant, action.bricks);
	std::vector<LYPineappleBrick>::iterator it = pinup.bricks.begin();
	unsigned int i=1;
	for (; it!=pinup.bricks.end(); it++) {
		(*it).brick = i++;
	}
	ret = newgame->onAction(pinup);
//	std::cout << pinup.toString() << std::endl;
	ASSERT_EQ(ret, true);

	action = newgame->popAction();
	ASSERT_EQ(action.bricks.size(), 14);
	ASSERT_EQ(action.applicant, LYSEAT1);
	ASSERT_EQ(newgame->dutySeatNo, action.applicant);
	ret = newgame->onAction(action);
	ASSERT_EQ(ret, true);
	pinup.pinup(action.table_id, action.applicant, action.bricks);
	it = pinup.bricks.begin();
	for (unsigned j=1; it!=pinup.bricks.end(); it++) {
		if (j>13) break;
		(*it).brick = j++;
	}
	pinup.bricks.pop_back();
	ret = newgame->onAction(pinup);
//	std::cout << pinup.toString() << std::endl;
	ASSERT_EQ(ret, true);

	action = newgame->popAction();
	ASSERT_EQ(action.bricks.size(), 3);
	ASSERT_EQ(action.applicant, LYSEAT2);
	ASSERT_EQ(newgame->dutySeatNo, action.applicant);
	ret = newgame->onAction(action);
	ASSERT_EQ(ret, true);
	pinup.pinup(action.table_id, action.applicant, action.bricks);
	it = pinup.bricks.begin();
	for (unsigned j=1; it!=pinup.bricks.end(); it++, j++) {
		if (j>2) break;
		(*it).brick = i++;
	}
	pinup.bricks.pop_back();
	ret = newgame->onAction(pinup);
//	std::cout << pinup.toString() << std::endl;
	ASSERT_EQ(ret, true);

	action = newgame->popAction();
	ASSERT_EQ(action.bricks.size(), 3);
	ASSERT_EQ(action.applicant, LYSEAT2);
	ASSERT_EQ(newgame->dutySeatNo, action.applicant);
	ret = newgame->onAction(action);
	ASSERT_EQ(ret, true);
	pinup.pinup(action.table_id, action.applicant, action.bricks);
	it = pinup.bricks.begin();
	for (unsigned j=1; it!=pinup.bricks.end(); it++, j++) {
		if (j>2) break;
		(*it).brick = i++;
	}
	pinup.bricks.pop_back();
	ret = newgame->onAction(pinup);
//	std::cout << pinup.toString() << std::endl;
	ASSERT_EQ(ret, true);

	action = newgame->popAction();
	ASSERT_EQ(action.bricks.size(), 3);
	ASSERT_EQ(action.applicant, LYSEAT2);
	ASSERT_EQ(newgame->dutySeatNo, action.applicant);
	ret = newgame->onAction(action);
	ASSERT_EQ(ret, true);
	pinup.pinup(action.table_id, action.applicant, action.bricks);
	it = pinup.bricks.begin();
	for (unsigned j=1; it!=pinup.bricks.end(); it++, j++) {
		if (j>2) break;
		(*it).brick = i++;
	}
	pinup.bricks.pop_back();
	ret = newgame->onAction(pinup);
//	std::cout << pinup.toString() << std::endl;
	ASSERT_EQ(ret, true);

	action = newgame->popAction();
	ASSERT_EQ(action.bricks.size(), 3);
	ASSERT_EQ(action.applicant, LYSEAT2);
	ASSERT_EQ(newgame->dutySeatNo, action.applicant);
	ret = newgame->onAction(action);
	ASSERT_EQ(ret, true);
	pinup.pinup(action.table_id, action.applicant, action.bricks);
	it = pinup.bricks.begin();
	for (unsigned j=1; it!=pinup.bricks.end(); it++, j++) {
		if (j>2) break;
		(*it).brick = i++;
	}
	pinup.bricks.pop_back();
	ret = newgame->onAction(pinup);
//	std::cout << pinup.toString() << std::endl;
	ASSERT_EQ(ret, true);

	delete newgame;
}

TEST_F(LYPineappleGame_2p_tests, fantasy_issuecards)
{
	fixture->seat1->gameStatus = LYFANTASY;
	fixture->seat2->gameStatus = LYFANTASY;
	LYSeatPtr btnSeat = pt->fetchSeat(LYSeat1);
	LYPineappleGame* newgame = new LYPineappleGame("new game", pt->seats, btnSeat, pt, NULL);
	ASSERT_EQ(fixture->seat1->gameStatus, LYFANTASY);
	ASSERT_EQ(fixture->seat2->gameStatus, LYFANTASY);
	ASSERT_EQ(newgame->actions.size(), 2);

	LYPineappleAction& action = newgame->popAction();
	ASSERT_EQ(action.applicant, LYSeat2);
	ASSERT_EQ(newgame->seat2->cardsOnHand.size(), 0);
	newgame->onAction(action);
	ASSERT_EQ(newgame->seat2->cardsOnHand.size(), 14);
	LYPineappleAction pinupAction;
	std::vector<LYPineappleBrick> bricks;
	std::vector<LYPineappleBrick>::iterator it = action.bricks.begin();
	unsigned int i = 1;
	for (; it!=action.bricks.end(); it++) {
		if (i > 13) break;
		LYPineappleBrick brk = *it;
		brk.brick = i++;
		bricks.push_back(brk);
	}
	ASSERT_EQ(i, 14);
	pinupAction.pinup("newtable", LYSEAT2, bricks);
	ASSERT_EQ(newgame->seat2->finishGame(), false);
	newgame->onAction(pinupAction);
	ASSERT_EQ(newgame->seat2->finishGame(), true);
	delete newgame;

}

TEST_F(LYPineappleGame_2p_tests, gameOver)
{
	fixture->setBricks(fixture->seat1, "QsQh8d", 1);
	fixture->setBricks(fixture->seat1, "KsKhTd7c8c", 2);
	fixture->setBricks(fixture->seat1, "As3s5s8s9s", 3);

	fixture->setBricks(fixture->seat2, "6s6h2d", 1);
	fixture->setBricks(fixture->seat2, "7s7h4d4c8h", 2);
	fixture->setBricks(fixture->seat2, "9h9d9cJsQc", 3);

	pg->gameOver();
	ASSERT_EQ(pg->seat1_vs_seat2.scoreAgainst, 11);
	ASSERT_EQ(pg->seat1_vs_seat2.statusAgainst, LYEVEN);
}

TEST_F(LYPineappleGame_2p_tests, validAction)
{
	LYPineappleAction& action = pg->popAction();
	ASSERT_EQ(pg->isValidAction(action), true);
	action = pg->popAction();
	ASSERT_EQ(pg->isValidAction(action), true);
}
