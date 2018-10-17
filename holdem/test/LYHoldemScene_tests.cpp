/*
 * LYHoldemScene_tests.cpp
 *
 *  Created on: 2013-7-6
 *      Author: caiqingfeng
 */

#include "src/LYHoldemScene.h"
#include "src/LYHoldemGame.h"
#include "src/LYHoldemTable.h"
#include "src/LYHoldemTrunk.h"
//#include "common/src/my_log.h"

#include <gtest/gtest.h>

#include <string>
using std::string;

class LYHoldemScene_tests : public ::testing::Test
{
protected:

    void SetUp()
    {
    	table = new LYHoldemTable(string("this_is_table_id"), 9, LYTableOpen, string("this_is_table_name"));
    	table->openTrunk("999", "trunk_name", "player01", "888");
    	trunk = (LYHoldemTrunk *)table->getTrunk();
    	table->takeSeat(LYSeat1, 1000, "player01");
    	table->takeSeat(LYSeat2, 1000, "player02");
    	trunk->createGame("", NULL);
    	game = (LYHoldemGame *)trunk->getCurrentGame();
    	scene = game->getLastScene();
    }
    void TearDown()
    {
    	delete table;
   }
public:
    LYHoldemTable *table;
    LYHoldemTrunk  *trunk;
    LYHoldemGame *game;
    LYHoldemScenePtr scene;
};

TEST_F(LYHoldemScene_tests, init)
{
    ASSERT_EQ(scene->pots.size(), 1);
    LYPot mainPot = scene->pots.back();
    ASSERT_EQ(mainPot.chipsInPot, 35);
    ASSERT_EQ(mainPot.seatsInPot.size(), 2);
    ASSERT_EQ(mainPot.winnersInPot.size(), 0);
    ASSERT_EQ(scene->seats.size(), 2);
    LYSeatDynamics sd1 = scene->seats.front();
    ASSERT_EQ(sd1.status, LYSeatInGame);
    ASSERT_EQ(sd1.chipsAtHand, 975);
    LYSeatDynamics sd2 = scene->seats.back();
    ASSERT_EQ(sd2.status, LYSeatInGame);
    ASSERT_EQ(sd2.chipsAtHand, 990);

    LYSeatPtr st1 = table->fetchSeat(LYSeat1);
    ASSERT_EQ(st1->seatNo, LYSeat1);
    ASSERT_EQ(st1->status, LYSeatInGame);
    LYSeatPtr st2 = table->fetchSeat(LYSeat2);
    ASSERT_EQ(st2->seatNo, LYSeat2);
    ASSERT_EQ(st2->status, LYSeatInGame);
}


