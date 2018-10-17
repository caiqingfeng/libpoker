/*
 * LYPineappleGame_tests.cpp
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

class LYPineappleGame_tests : public ::testing::Test
{
protected:

    void SetUp()
    {
    	fixture = new Fixture();
    	pt = fixture->pt;
    	LYSeatPtr btnSeat = pt->fetchSeat(LYSeat3);
    	pg = new LYPineappleGame("new game", pt->seats, btnSeat, pt, NULL);
    }
    void TearDown()
    {
    	delete pg;
   }
public:
    Fixture* fixture;
    LYPineappleGame* pg;
    LYPineappleTable* pt;
};

TEST_F(LYPineappleGame_tests, init)
{
	ASSERT_EQ(pg->btnSeatNo, LYSeat3);
	ASSERT_EQ(pg->dutySeatNo, LYSeat1);
	ASSERT_EQ(pg->actionIndex, 0);
	LYSeatPtr seat = pg->sortedSeats[0];
	ASSERT_EQ(seat->seatNo, LYSeat1);
	seat = pg->sortedSeats[1];
	ASSERT_EQ(seat->seatNo, LYSeat2);
	seat = pg->sortedSeats[2];
	ASSERT_EQ(seat->seatNo, LYSeat3);

	ASSERT_EQ(pg->seat1->seatNo, LYSeat1);
	ASSERT_EQ(pg->seat2->seatNo, LYSeat2);
	ASSERT_EQ(pg->seat3->seatNo, LYSeat3);
}

TEST_F(LYPineappleGame_tests, action)
{
	ASSERT_EQ(pg->actions.size(), 15);
	LYPineappleAction& action = pg->popAction();
	ASSERT_EQ(action.bricks.size(), 5);
	ASSERT_EQ(action.applicant, LYSEAT1);
	action = pg->popAction();
	ASSERT_EQ(action.bricks.size(), 5);
	ASSERT_EQ(action.applicant, LYSEAT2);
	action = pg->popAction();
	ASSERT_EQ(action.bricks.size(), 5);
	action = pg->popAction();
	ASSERT_EQ(action.bricks.size(), 3);
	ASSERT_EQ(action.applicant, LYSEAT1);
	action = pg->popAction();
	ASSERT_EQ(action.bricks.size(), 3);
	ASSERT_EQ(action.applicant, LYSEAT2);
	action = pg->popAction();
	ASSERT_EQ(action.bricks.size(), 3);
	ASSERT_EQ(action.applicant, LYSEAT3);
	action = pg->popAction();
	ASSERT_EQ(action.bricks.size(), 3);
	ASSERT_EQ(action.applicant, LYSEAT1);
	action = pg->popAction();
	ASSERT_EQ(action.bricks.size(), 3);
	ASSERT_EQ(action.applicant, LYSEAT2);
	action = pg->popAction();
	ASSERT_EQ(action.bricks.size(), 3);
	ASSERT_EQ(action.applicant, LYSEAT3);
	action = pg->popAction();
	ASSERT_EQ(action.applicant, LYSEAT1);
	ASSERT_EQ(action.bricks.size(), 3);
	action = pg->popAction();
	ASSERT_EQ(action.bricks.size(), 3);
	ASSERT_EQ(action.applicant, LYSEAT2);
	action = pg->popAction();
	ASSERT_EQ(action.bricks.size(), 3);
	ASSERT_EQ(action.applicant, LYSEAT3);
	action = pg->popAction();
	ASSERT_EQ(action.bricks.size(), 3);
	ASSERT_EQ(action.applicant, LYSEAT1);
	action = pg->popAction();
	ASSERT_EQ(action.bricks.size(), 3);
	ASSERT_EQ(action.applicant, LYSEAT2);
	action = pg->popAction();
	ASSERT_EQ(action.bricks.size(), 3);
	ASSERT_EQ(action.applicant, LYSEAT3);
	ASSERT_EQ(pg->actionIndex, 15);
}

TEST_F(LYPineappleGame_tests, fantasy)
{
	fixture->seat1->gameStatus = LYFANTASY;
	LYSeatPtr btnSeat = pt->fetchSeat(LYSeat1);
	LYPineappleGame* newgame = new LYPineappleGame("new game", pt->seats, btnSeat, pt, NULL);
	ASSERT_EQ(fixture->seat1->gameStatus, LYFANTASY);
	ASSERT_EQ(newgame->actions.size(), 11);
	LYPineappleAction& action = newgame->popAction();
	ASSERT_EQ(action.bricks.size(), 5);
	ASSERT_EQ(action.applicant, LYSEAT2);
	action = newgame->popAction();
	ASSERT_EQ(action.bricks.size(), 5);
	ASSERT_EQ(action.applicant, LYSEAT3);
	action = newgame->popAction();
	ASSERT_EQ(action.bricks.size(), 14);
	ASSERT_EQ(action.applicant, LYSEAT1);
	ASSERT_EQ(newgame->dutySeatNo, action.applicant);
	action = newgame->popAction();
	ASSERT_EQ(action.bricks.size(), 3);
	ASSERT_EQ(action.applicant, LYSEAT2);
	ASSERT_EQ(newgame->dutySeatNo, action.applicant);
	action = newgame->popAction();
	ASSERT_EQ(action.bricks.size(), 3);
	ASSERT_EQ(action.applicant, LYSEAT3);
	ASSERT_EQ(newgame->dutySeatNo, action.applicant);
	action = newgame->popAction();
	ASSERT_EQ(action.bricks.size(), 3);
	ASSERT_EQ(action.applicant, LYSEAT2);
	ASSERT_EQ(newgame->dutySeatNo, action.applicant);
	action = newgame->popAction();
	ASSERT_EQ(action.bricks.size(), 3);
	ASSERT_EQ(action.applicant, LYSEAT3);
	ASSERT_EQ(newgame->dutySeatNo, action.applicant);
	action = newgame->popAction();
	ASSERT_EQ(action.bricks.size(), 3);
	ASSERT_EQ(action.applicant, LYSEAT2);
	ASSERT_EQ(newgame->dutySeatNo, action.applicant);
	action = newgame->popAction();
	ASSERT_EQ(action.bricks.size(), 3);
	ASSERT_EQ(action.applicant, LYSEAT3);
	ASSERT_EQ(newgame->dutySeatNo, action.applicant);
	delete newgame;
}

TEST_F(LYPineappleGame_tests, fantasy_issuecards)
{
	fixture->seat1->gameStatus = LYFANTASY;
	fixture->seat2->gameStatus = LYFANTASY;
	fixture->seat3->gameStatus = LYFANTASY;
	LYSeatPtr btnSeat = pt->fetchSeat(LYSeat1);
	LYPineappleGame* newgame = new LYPineappleGame("new game", pt->seats, btnSeat, pt, NULL);
	ASSERT_EQ(fixture->seat1->gameStatus, LYFANTASY);
	ASSERT_EQ(fixture->seat2->gameStatus, LYFANTASY);
	ASSERT_EQ(fixture->seat3->gameStatus, LYFANTASY);
	ASSERT_EQ(newgame->actions.size(), 3);

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

TEST_F(LYPineappleGame_tests, gameOver)
{
	fixture->setBricks(fixture->seat1, "QsQh8d", 1);
	fixture->setBricks(fixture->seat1, "KsKhTd7c8c", 2);
	fixture->setBricks(fixture->seat1, "As3s5s8s9s", 3);

	fixture->setBricks(fixture->seat2, "6s6h2d", 1);
	fixture->setBricks(fixture->seat2, "7s7h4d4c8h", 2);
	fixture->setBricks(fixture->seat2, "9h9d9cJsQc", 3);

	fixture->setBricks(fixture->seat3, "AdAhAc", 1);
	fixture->setBricks(fixture->seat3, "Jh2h3h4h5h", 2);
	fixture->setBricks(fixture->seat3, "TcThTs3c3d", 3);

	pg->gameOver();
	ASSERT_EQ(pg->seat1_vs_seat2.scoreAgainst, 11);
	ASSERT_EQ(pg->seat1_vs_seat2.statusAgainst, LYEVEN);
	ASSERT_EQ(pg->seat1_vs_seat3.scoreAgainst, -31);
	ASSERT_EQ(pg->seat1_vs_seat3.statusAgainst, LYSCOOPED);
	ASSERT_EQ(pg->seat2_vs_seat3.scoreAgainst, -41);
	ASSERT_EQ(pg->seat2_vs_seat3.statusAgainst, LYSCOOPED);
}

TEST_F(LYPineappleGame_tests, validAction)
{
	LYPineappleAction& action = pg->popAction();
	ASSERT_EQ(pg->isValidAction(action), true);
	action = pg->popAction();
	ASSERT_EQ(pg->isValidAction(action), true);
}
