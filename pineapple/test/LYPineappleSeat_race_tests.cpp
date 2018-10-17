/*
 * LYPineappleAction_tests.cpp
 *
 *  Created on: 2014-9-25
 *      Author: caiqingfeng
 */

#include <gtest/gtest.h>

#include "src/LYPineappleSeat.h"
//#include "common/src/my_log.h"
#include "poker/src/LYDeck.h"

class LYPineappleSeat_race_tests : public ::testing::Test
{
protected:

    void SetUp()
    {
    	ps1 = new LYPineappleSeat(LYSeat1);
    	ps1->topBricks.push_back(hQ);
    	ps1->topBricks.push_back(sQ);
    	ps1->topBricks.push_back(h6);
    	ps1->middleBricks.push_back(h2);
    	ps1->middleBricks.push_back(c2);
    	ps1->middleBricks.push_back(h3);
    	ps1->middleBricks.push_back(c3);
    	ps1->middleBricks.push_back(h5);
    	ps1->bottomBricks.push_back(dA);
    	ps1->bottomBricks.push_back(d6);
    	ps1->bottomBricks.push_back(d9);
    	ps1->bottomBricks.push_back(dT);
    	ps1->bottomBricks.push_back(dJ);
    	ps2 = new LYPineappleSeat(LYSeat2);
    	ps2->topBricks.push_back(h8);
    	ps2->topBricks.push_back(s8);
    	ps2->topBricks.push_back(h7);
    	ps2->middleBricks.push_back(h9);
    	ps2->middleBricks.push_back(c9);
    	ps2->middleBricks.push_back(hK);
    	ps2->middleBricks.push_back(cK);
    	ps2->middleBricks.push_back(d5);
    	ps2->bottomBricks.push_back(dQ);
    	ps2->bottomBricks.push_back(sQ);
    	ps2->bottomBricks.push_back(cQ);
    	ps2->bottomBricks.push_back(d4);
    	ps2->bottomBricks.push_back(c4);
    	ps3 = new LYPineappleSeat(LYSeat3);
    	ps3->topBricks.push_back(hJ);
    	ps3->topBricks.push_back(sJ);
    	ps3->topBricks.push_back(h3);
    	ps3->middleBricks.push_back(d2);
    	ps3->middleBricks.push_back(s2);
    	ps3->middleBricks.push_back(hT);
    	ps3->middleBricks.push_back(dK);
    	ps3->middleBricks.push_back(sA);
    	ps3->bottomBricks.push_back(d3);
    	ps3->bottomBricks.push_back(s4);
    	ps3->bottomBricks.push_back(c5);
    	ps3->bottomBricks.push_back(d6);
    	ps3->bottomBricks.push_back(c7);

    	ps1->prepareForRace();
    	ps2->prepareForRace();
    	ps3->prepareForRace();
    }
    void TearDown()
    {
    	delete ps1;
    	delete ps2;
    	delete ps3;
   }
public:
    LYPineappleSeat *ps1, *ps2, *ps3;
};

TEST_F(LYPineappleSeat_race_tests, init)
{
	ASSERT_EQ(ps1->gameStatus, LYFANTASY);
	ASSERT_EQ(ps2->gameStatus, LYNORMAL);
	ASSERT_EQ(ps3->gameStatus, LYFOUL);
	ps1->resetForNewGame();
	ASSERT_EQ(ps1->gameStatus, LYFANTASY);
	ps2->resetForNewGame();
	ASSERT_EQ(ps2->gameStatus, LYNORMAL);
	ps3->resetForNewGame();
	ASSERT_EQ(ps3->gameStatus, LYNORMAL);
}

TEST_F(LYPineappleSeat_race_tests, bonus)
{
	ASSERT_EQ(ps1->bonusOfTop(), 7);
	ASSERT_EQ(ps1->bonusOfMiddle(), 0);
	ASSERT_EQ(ps1->bonusOfBottom(), 4);
	ASSERT_EQ(ps2->bonusOfTop(), 3);
	ASSERT_EQ(ps2->bonusOfMiddle(), 0);
	ASSERT_EQ(ps2->bonusOfBottom(), 6);
	ASSERT_EQ(ps3->bonusOfTop(), 0);
	ASSERT_EQ(ps3->bonusOfMiddle(), 0);
	ASSERT_EQ(ps3->bonusOfBottom(), 0);
}

TEST_F(LYPineappleSeat_race_tests, raceAgainst)
{
	LYPineappleRace pr;
	ps1->raceAgainst(*ps2, pr);
	ASSERT_EQ(pr.scoreAgainst, 1);
	ASSERT_EQ(pr.statusAgainst, LYEVEN);
	LYPineappleRace pr2;
	ps1->raceAgainst(*ps3, pr2);
	ASSERT_EQ(pr2.scoreAgainst, 17);
	ASSERT_EQ(pr2.statusAgainst, LYBEATALL);
	LYPineappleRace pr3;
	ps2->raceAgainst(*ps3, pr3);
	ASSERT_EQ(pr3.scoreAgainst, 15);
	ASSERT_EQ(pr3.statusAgainst, LYBEATALL);
}


