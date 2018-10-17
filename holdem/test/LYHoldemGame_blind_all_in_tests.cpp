/*
 * LYHoldemGame_tests.cpp
 *
 *  Created on: 2013-7-6
 *      Author: caiqingfeng
 */

#include "src/LYHoldemGame.h"
#include "src/LYHoldemTable.h"
#include "src/LYHoldemTrunk.h"
//#include "common/src/my_log.h"
#include <string>

#include <gtest/gtest.h>

using std::string;

class LYHoldemGame_blind_allin_tests : public ::testing::Test
{
protected:

    void SetUp()
    {
    	table = new LYHoldemTable(string("this_is_table_id"), 9, LYTableOpen, string("this_is_table_name"));
    	init_seats();
    	table->openTrunk("999", "trunk_name", "player01", "888");
    	trunk = (LYHoldemTrunk *)table->getTrunk();
    	table->takeSeat(LYSeat1, 1000, "player01");
    	table->takeSeat(LYSeat2, 1000, "player02");
    	st1->chipsAtHand = 5;
    	st2->chipsAtHand = 500;
    	trunk->createGame("", NULL);
    	game = (LYHoldemGame *)trunk->getCurrentGame();
    }
    void TearDown()
    {
    	delete table;
    }

    void init_seats()
    {
    	st1 = table->fetchSeat(LYSeat1);
    	seat1 = (LYHoldemSeat *)st1.get();
    	st2 = table->fetchSeat(LYSeat2);
    	seat2 = (LYHoldemSeat *)st2.get();
    	st3 = table->fetchSeat(LYSeat3);
    	seat3 = (LYHoldemSeat *)st3.get();
    	st4 = table->fetchSeat(LYSeat4);
    	seat4 = (LYHoldemSeat *)st4.get();
    	st5 = table->fetchSeat(LYSeat5);
    	seat5 = (LYHoldemSeat *)st5.get();
    	st6 = table->fetchSeat(LYSeat6);
    	seat6 = (LYHoldemSeat *)st6.get();
    	st7 = table->fetchSeat(LYSeat7);
    	seat7 = (LYHoldemSeat *)st7.get();
    	st8 = table->fetchSeat(LYSeat8);
    	seat8 = (LYHoldemSeat *)st8.get();
    	st9 = table->fetchSeat(LYSeat9);
    	seat9 = (LYHoldemSeat *)st9.get();
    }
public:
    LYHoldemTable *table;
    LYHoldemTrunk  *trunk;
    LYHoldemGame *game;
    LYSeatPtr st1, st2, st3, st4, st5, st6, st7, st8, st9;
    LYHoldemSeat *seat1, *seat2, *seat3, *seat4, *seat5, *seat6, *seat7, *seat8, *seat9;
};

TEST_F(LYHoldemGame_blind_allin_tests, init)
{
//	LY_LOG_DBG("LYHoldemGame_blind_allin_tests::init");
    ASSERT_EQ(game->seatsOfTable.size(), 9);
    ASSERT_EQ(game->sortedSeats.size(), 2);
	ASSERT_EQ(st1->lastBet, 5);
	ASSERT_EQ(seat1->gameStatus, LYSeatHoldemInAllin);
	ASSERT_EQ(game->continueThisRound, LYHoldemRoundDown);
}

