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

class LYPineappleSeat_tests : public ::testing::Test
{
protected:

    void SetUp()
    {
    	ps = new LYPineappleSeat(LYSeat1);
    	ps->status = LYSeatOccupied;
    }
    void TearDown()
    {
    	delete ps;
   }
public:
    LYPineappleSeat *ps;
};

TEST_F(LYPineappleSeat_tests, init)
{
	ASSERT_EQ(ps->score, 0);
	ASSERT_EQ(ps->finishGame(), false);
	ASSERT_EQ(ps->topBricks.size(), 0);
	ASSERT_EQ(ps->middleBricks.size(), 0);
	ASSERT_EQ(ps->bottomBricks.size(), 0);
}

TEST_F(LYPineappleSeat_tests, issueCards)
{
	LYPineappleBrick pb1(0, sA);
	LYPineappleBrick pb2(1, sK);
	LYPineappleBrick pb3(2, sQ);
	std::vector<LYPineappleBrick> pbs;
	pbs.push_back(pb1);
	pbs.push_back(pb2);
	pbs.push_back(pb3);

	int r = ps->issueCards(pbs);
	ASSERT_EQ(r, 0);
	ASSERT_EQ(ps->cardsOnHand.size(), 3);
	ASSERT_EQ(ps->cardsOnHand[0], sA);
	ASSERT_EQ(ps->cardsOnHand[1], sK);
	ASSERT_EQ(ps->cardsOnHand[2], sQ);
}

TEST_F(LYPineappleSeat_tests, pinup_1)
{
	LYPineappleBrick pb(1, sA);
	int r = ps->pinup(pb);
	ASSERT_EQ(r, 0);
	ASSERT_EQ(ps->allBricksStr(), "1:1,2:0,3:0,4:0,5:0,6:0,7:0,8:0,9:0,10:0,11:0,12:0,13:0,");

	r = ps->pinup(pb);
	ASSERT_EQ(r, -1);

	pb.brick = 12;
	r = ps->pinup(pb);
	ASSERT_EQ(r, -1);
	pb.brick = 9;
	r = ps->pinup(pb);
	ASSERT_EQ(r, 0);
	ASSERT_EQ(ps->allBricksStr(), "1:1,2:0,3:0,4:0,5:0,6:0,7:0,8:0,9:1,10:0,11:0,12:0,13:0,");

	pb.brick = 6;
	r = ps->pinup(pb);
	ASSERT_EQ(r, -1);
	pb.brick = 4;
	r = ps->pinup(pb);
	ASSERT_EQ(r, 0);
	ASSERT_EQ(ps->allBricksStr(), "1:1,2:0,3:0,4:1,5:0,6:0,7:0,8:0,9:1,10:0,11:0,12:0,13:0,");

	ps->cardsOnHand.push_back(sK);
	ASSERT_EQ(ps->allBricksStr(), "1:1,2:0,3:0,4:1,5:0,6:0,7:0,8:0,9:1,10:0,11:0,12:0,13:0,0:5,");
}

TEST_F(LYPineappleSeat_tests, pinup_2)
{
	LYPineappleBrick pb1(1, sA);
	LYPineappleBrick pb2(9, sK);
	LYPineappleBrick pb3(4, s3);
	std::vector<LYPineappleBrick> pbs;
	pbs.push_back(pb1);
	pbs.push_back(pb2);
	pbs.push_back(pb3);
	int r = ps->pinup(pbs);
	ASSERT_EQ(r, 0);

}

TEST_F(LYPineappleSeat_tests, finish_game)
{
	LYPineappleBrick pb1(1, sA);
	LYPineappleBrick pb2(2, sK);
	LYPineappleBrick pb3(3, sQ);
	LYPineappleBrick pb4(4, sJ);
	LYPineappleBrick pb5(5, sT);
	LYPineappleBrick pb6(6, s9);
	LYPineappleBrick pb7(7, s8);
	LYPineappleBrick pb8(8, s7);
	LYPineappleBrick pb9(9, s6);
	LYPineappleBrick pb10(10, s5);
	LYPineappleBrick pb11(11, s4);
	LYPineappleBrick pb12(12, s3);
	LYPineappleBrick pb13(13, s2);
	std::vector<LYPineappleBrick> pbs;
	pbs.push_back(pb1);
	pbs.push_back(pb2);
	pbs.push_back(pb3);
	pbs.push_back(pb4);
	pbs.push_back(pb5);
	pbs.push_back(pb6);
	pbs.push_back(pb7);
	pbs.push_back(pb8);
	pbs.push_back(pb9);
	pbs.push_back(pb10);
	pbs.push_back(pb11);
	pbs.push_back(pb12);
	pbs.push_back(pb13);
	int r = ps->pinup(pbs);
	ASSERT_EQ(r, 0);
	ASSERT_EQ(ps->finishGame(), true);

}
