/*
 * LYPotManager_test.cpp
 *
 *  Created on: 2013-3-25
 *      Author: caiqingfeng
 */

#include <gtest/gtest.h>
#include <boost/foreach.hpp>

#include "src/LYHoldemTable.h"
#include "src/LYPotManager.h"
#include "src/LYHoldemGame.h"
//#include "common/src/my_log.h"

#include <string>
using std::string;

class LYPotManager_tests : public ::testing::Test
{
protected:

    void SetUp()
    {
	table = new LYHoldemTable("table_id_is_me", 9, LYTableOpen, 
        		"table_name");
	openSeats = table->seats;
	for (int i=0; i<9; i++) {
            LYSeatPtr seat = openSeats[i];
            seat->chipsAtBeginning = 5000;
            seat->chipsAtHand = 5000;
        }

	game = new LYHoldemGame("game_id", table->seats, table);

    	pm = &game->pm;
    }
    void TearDown()
    {
	delete table;
    	//delete pm;
    	openSeats.clear();
   }
public:
    LYHoldemTable *table;
    LYHoldemGame *game;
    LYPotManager *pm;
    std::vector<LYSeatPtr > openSeats;
};

TEST_F(LYPotManager_tests, init)
{
	ASSERT_TRUE(NULL != pm->mainPot);
	ASSERT_EQ(pm->mainPot->chipsInPot, 0);
}

TEST_F(LYPotManager_tests, create)
{
    std::vector<LYSeatPtr > seats;
    for (int i=1; i<8; i++) {
        LYSeatPtr seat = openSeats[i];
        seats.push_back(seat);
    }
    pm->createSidePot(100, seats);
    ASSERT_TRUE(pm->pots.size() == 2);

    seats.clear();
    for (int i=1; i<8; i++) {
        LYSeatPtr seat = openSeats[i];
        seats.push_back(seat);
    }
    pm->createSidePot(100, seats);
    ASSERT_EQ(pm->pots.size(), 3);

    seats.clear();
    for (int i=1; i<5; i++) {
        LYSeatPtr seat = openSeats[i];
        seats.push_back(seat);
    }
    pm->createSidePot(100, seats);
    ASSERT_EQ(pm->pots.size(), 4);

    seats.clear();
    for (int i=1; i<3; i++) {
        LYSeatPtr seat = openSeats[i];
        seats.push_back(seat);
    }
    seats.push_back(openSeats[6]);
    pm->createSidePot(100, seats);
    ASSERT_EQ(pm->pots.size(), 5);
}

TEST_F(LYPotManager_tests, addChips)
{
    ASSERT_EQ(pm->mainPot->chipsInPot, 0);
    LYSeatPtr st = openSeats[1];
    pm->betBySeat(100,st);
    ASSERT_EQ(pm->mainPot->chipsInPot, 100);
    ASSERT_EQ(st->lastBet, 100);
    ASSERT_EQ(st->chipsAtBeginning, 5000);
    ASSERT_EQ(st->chipsAtHand, 4900);
}

TEST_F(LYPotManager_tests, continuousBet)
{
    ASSERT_EQ(pm->mainPot->chipsInPot, 0);
    LYSeatPtr st1 = openSeats[1];
    st1->chipsAtHand = 1000;
    st1->chipsAtBeginning = 1000;
    pm->betBySeat(10,st1);
    ASSERT_EQ(pm->mainPot->chipsInPot, 10);
    ASSERT_EQ(st1->lastBet, 10);
    ASSERT_EQ(st1->chipsAtBeginning, 1000);
    ASSERT_EQ(st1->chipsAtHand, 990);
    LYSeatPtr st2 = openSeats[2];
    st2->chipsAtHand = 1000;
    st2->chipsAtBeginning = 1000;
    pm->betBySeat(20,st2);
    ASSERT_EQ(pm->mainPot->chipsInPot, 30);
    ASSERT_EQ(st2->lastBet, 20);
    ASSERT_EQ(st2->chipsAtBeginning, 1000);
    ASSERT_EQ(st2->chipsAtHand, 980);

    pm->betBySeat(1000,st1);
    ASSERT_EQ(pm->mainPot->chipsInPot, 1020);
    ASSERT_EQ(st1->lastBet, 1000);
    ASSERT_EQ(st1->chipsAtBeginning, 1000);
    ASSERT_EQ(st1->chipsAtHand, 0);
    pm->betBySeat(1000,st2);
    ASSERT_EQ(pm->mainPot->chipsInPot, 2000);
    ASSERT_EQ(st2->lastBet, 1000);
    ASSERT_EQ(st2->chipsAtBeginning, 1000);
    ASSERT_EQ(st2->chipsAtHand, 0);
}

TEST_F(LYPotManager_tests, twoSimplePots)
{
    ASSERT_EQ(pm->mainPot->chipsInPot, 0);
    LYSeatPtr st1 = openSeats[1];
    st1->chipsAtHand = 1020;
    st1->chipsAtBeginning = 1020;
    pm->betBySeat(10,st1);
    ASSERT_EQ(pm->mainPot->chipsInPot, 10);
    ASSERT_EQ(st1->lastBet, 10);
    ASSERT_EQ(st1->chipsAtBeginning, 1020);
    ASSERT_EQ(st1->chipsAtHand, 1010);
    LYSeatPtr st2 = openSeats[2];
    st2->chipsAtHand = 980;
    st2->chipsAtBeginning = 980;
    pm->betBySeat(20,st2);
    ASSERT_EQ(pm->mainPot->chipsInPot, 30);
    ASSERT_EQ(st2->lastBet, 20);
    ASSERT_EQ(st2->chipsAtBeginning, 980);
    ASSERT_EQ(st2->chipsAtHand, 960);

    pm->betBySeat(1020,st1);
    ASSERT_EQ(pm->mainPot->chipsInPot, 1040);
    ASSERT_EQ(st1->lastBet, 1020);
    ASSERT_EQ(st1->chipsAtHand, 0);
    pm->betBySeat(1020,st2);
    ASSERT_EQ(pm->mainPot->chipsInPot, 1960);
    ASSERT_EQ(st2->lastBet, 980);
    ASSERT_EQ(st2->chipsAtHand, 0);
}

/*
 * Seat1=100, seat2=10, seat3=200, seat4=30
 * seat1 bet 20, mainpot=20
 * seat2 call 10, mainpot=20, sidepot1=10
 * seat3 bet 40, mainpot=30, sidepot1=40
 * seat4 call 30, mainpot=40, sidepot1=50, sidepot2=10
 * seat1 call 40, mainpot=40, sidepot1=60, sidepot2=20
 */
TEST_F(LYPotManager_tests, complicatedPot)
{
    ASSERT_EQ(pm->mainPot->chipsInPot, 0);
    LYSeatPtr st1 = openSeats[0];
    st1->chipsAtHand = 100;
    st1->chipsAtBeginning = 100;
    pm->betBySeat(20,st1);

    ASSERT_EQ(pm->mainPot->chipsInPot, 20);
    ASSERT_EQ(st1->lastBet, 20);
    ASSERT_EQ(st1->chipsAtBeginning, 100);
    ASSERT_EQ(st1->chipsAtHand, 80);
    LYSeatPtr st2 = openSeats[1];
    st2->chipsAtHand = 10;
    st2->chipsAtBeginning = 10;
    pm->betBySeat(20,st2);

    ASSERT_EQ(pm->mainPot->chipsInPot, 20);
    ASSERT_EQ(pm->pots.size(), 2);
    LYPotPtr secondPot = pm->pots[1];
    ASSERT_EQ(secondPot->chipsInPot, 10);
    ASSERT_EQ(st2->lastBet, 10);
    ASSERT_EQ(st2->chipsAtHand, 0);

    LYSeatPtr st3 = openSeats[2];
    st3->chipsAtHand = 200;
    st3->chipsAtBeginning = 200;
    pm->betBySeat(40,st3);
    ASSERT_EQ(pm->mainPot->chipsInPot, 30);
    ASSERT_EQ(st3->lastBet, 40);
    ASSERT_EQ(pm->pots.size(), 2);
    ASSERT_EQ(secondPot->chipsInPot, 40);
    ASSERT_EQ(st3->chipsAtBeginning, 200);
    ASSERT_EQ(st3->chipsAtHand, 160);

    LYSeatPtr st4 = openSeats[3];
    st4->chipsAtHand = 30;
    st4->chipsAtBeginning = 30;
//LY_LOG_DBG("for debug");
    pm->betBySeat(40,st4);
    ASSERT_EQ(pm->mainPot->chipsInPot, 40);
    ASSERT_EQ(pm->pots.size(), 3);
    LYPotPtr lastPot = pm->pots[2];
    ASSERT_EQ(secondPot->chipsInPot, 50);
    ASSERT_EQ(lastPot->chipsInPot, 10);
    ASSERT_EQ(st4->lastBet, 30);
    ASSERT_EQ(st4->chipsAtHand, 0);

    pm->betBySeat(40, st1);
    ASSERT_EQ(pm->mainPot->chipsInPot, 40);
    ASSERT_EQ(pm->pots.size(), 3);
    ASSERT_EQ(secondPot->chipsInPot, 60);
    ASSERT_EQ(lastPot->chipsInPot, 20);
    ASSERT_EQ(st1->lastBet, 40);
    ASSERT_EQ(st1->chipsAtHand, 60);
}

/*
 * Seat1=25, seat2=10, seat3=200, seat4=30
 * seat1 bet 20, mainpot=20
 * seat2 call 10, mainpot=20, sidepot1=10
 * seat3 bet 40, mainpot=30, sidepot1=40
 * seat4 call 40, mainpot=40, sidepot1=50, sidepot2=10
 * seat1 call 40, mainpot=40, sidepot1=45, sidepot2=10, sidepot3=10
 */
TEST_F(LYPotManager_tests, complicatedPot2)
{
    ASSERT_EQ(pm->mainPot->chipsInPot, 0);
    LYSeatPtr st1 = openSeats[0];
    st1->chipsAtHand = 25;
    st1->chipsAtBeginning = 25;
    pm->betBySeat(20,st1);

    ASSERT_EQ(pm->mainPot->chipsInPot, 20);
    ASSERT_EQ(st1->lastBet, 20);
    ASSERT_EQ(st1->chipsAtBeginning, 25);
    ASSERT_EQ(st1->chipsAtHand, 5);
    LYSeatPtr st2 = openSeats[1];
    st2->chipsAtHand = 10;
    st2->chipsAtBeginning = 10;
    pm->betBySeat(20,st2);

    ASSERT_EQ(pm->mainPot->chipsInPot, 20);
    ASSERT_EQ(pm->pots.size(), 2);
    LYPotPtr secondPot = pm->pots[1];
    ASSERT_EQ(secondPot->chipsInPot, 10);
    ASSERT_EQ(st2->lastBet, 10);
    ASSERT_EQ(st2->chipsAtHand, 0);

    LYSeatPtr st3 = openSeats[2];
    st3->chipsAtHand = 200;
    st3->chipsAtBeginning = 200;
    pm->betBySeat(40,st3);
    ASSERT_EQ(pm->mainPot->chipsInPot, 30);
    ASSERT_EQ(st3->lastBet, 40);
    ASSERT_EQ(pm->pots.size(), 2);
    ASSERT_EQ(secondPot->chipsInPot, 40);
    ASSERT_EQ(st3->chipsAtBeginning, 200);
    ASSERT_EQ(st3->chipsAtHand, 160);

    LYSeatPtr st4 = openSeats[3];
    st4->chipsAtHand = 30;
    st4->chipsAtBeginning = 30;
//LY_LOG_DBG("for debug");
    pm->betBySeat(40,st4);
    ASSERT_EQ(pm->mainPot->chipsInPot, 40);
    ASSERT_EQ(pm->pots.size(), 3);
    LYPotPtr thirdPot = pm->pots[2];
    ASSERT_EQ(secondPot->chipsInPot, 50);
    ASSERT_EQ(thirdPot->chipsInPot, 10);
    ASSERT_EQ(st4->lastBet, 30);
    ASSERT_EQ(st4->chipsAtHand, 0);

    pm->betBySeat(40, st1);
    ASSERT_EQ(pm->mainPot->chipsInPot, 40);
    ASSERT_EQ(pm->pots.size(), 4);
    ASSERT_EQ(secondPot->chipsInPot, 45);
    ASSERT_EQ(thirdPot->chipsInPot, 10);
    LYPotPtr lastPot = pm->pots[3];
    ASSERT_EQ(lastPot->chipsInPot, 10);
    ASSERT_EQ(st1->lastBet, 25);
    ASSERT_EQ(st1->chipsAtHand, 0);
}

/*
 * Seat1=4701, seat2=4020, seat3=5929, seat4=2869
 * seat1 bet 20, mainpot=20
 * seat2 call 20, mainpot=40
 * seat3 bet 20, mainpot=60
 * seat4 call 20, mainpot=80
 * seat1 bet 4615, mainpot=4695, mainpot.unitprice=4615, mainpot.deadmoney=80
 * seat2 bet 4000, mainpot=80+4000*2, mainpot.deadmoney=80, mainpot.unitprice=4000, sidepot1=615
 * seat3 bet 5909, mainpot=80+4000*3, sidepot1=615+1909, total chips=14604
 * seat4 call 2849, mainpot=80+2849*4, sidepot1=(4000-2849)*3, sidepot2=615+1909, total chips=17453
 * seat1 call 4681, mainpot=80+2849*4, sidepot1=(4000-2849)*3, sidepot2=681*2, sidepot3=1909-681, total chips=17519
 */
TEST_F(LYPotManager_tests, realCase)
{
    ASSERT_EQ(pm->mainPot->chipsInPot, 0);
    LYPotPtr secondPot, thirdPot, lastPot;
    LYSeatPtr st1 = openSeats[0];
    st1->chipsAtHand = 4701;
    st1->chipsAtBeginning = 4701;
    pm->betBySeat(20,st1);

    ASSERT_EQ(pm->mainPot->chipsInPot, 20);
    ASSERT_EQ(st1->lastBet, 20);
    ASSERT_EQ(st1->chipsAtBeginning, 4701);
    ASSERT_EQ(st1->chipsAtHand, 4681);
    LYSeatPtr st2 = openSeats[1];
    st2->chipsAtHand = 4020;
    st2->chipsAtBeginning = 4020;
    pm->betBySeat(20,st2);

    ASSERT_EQ(pm->mainPot->chipsInPot, 40);
    ASSERT_EQ(pm->pots.size(), 1);
    ASSERT_EQ(st2->lastBet, 20);
    ASSERT_EQ(st2->chipsAtHand, 4000);

    LYSeatPtr st3 = openSeats[2];
    st3->chipsAtHand = 5929;
    st3->chipsAtBeginning = 5929;
    pm->betBySeat(20,st3);
    ASSERT_EQ(pm->mainPot->chipsInPot, 60);
    ASSERT_EQ(st3->lastBet, 20);
    ASSERT_EQ(pm->pots.size(), 1);
    ASSERT_EQ(st3->chipsAtBeginning, 5929);
    ASSERT_EQ(st3->chipsAtHand, 5909);

    LYSeatPtr st4 = openSeats[3];
    st4->chipsAtHand = 2869;
    st4->chipsAtBeginning = 2869;
//LY_LOG_DBG("for debug");
    pm->betBySeat(20,st4);
    ASSERT_EQ(pm->mainPot->chipsInPot, 80);
    ASSERT_EQ(pm->pots.size(), 1);
    ASSERT_EQ(st4->lastBet, 20);
    ASSERT_EQ(st4->chipsAtBeginning, 2869);
    ASSERT_EQ(st4->chipsAtHand, 2849);

    pm->newRound();
    
    pm->betBySeat(4615, st1);
    ASSERT_EQ(pm->mainPot->chipsInPot, 4695);
    ASSERT_EQ(pm->pots.size(), 1);
    ASSERT_EQ(pm->mainPot->priceFixed, false);
    ASSERT_EQ(pm->mainPot->unitPrice, 4615);
    ASSERT_EQ(st1->lastBet, 4615);
    ASSERT_EQ(st1->chipsAtHand, 66);

    pm->betBySeat(4000, st2);
    ASSERT_EQ(pm->mainPot->chipsInPot, 8080);
    ASSERT_EQ(pm->pots.size(), 2);
    ASSERT_EQ(pm->mainPot->priceFixed, true);
    ASSERT_EQ(pm->mainPot->unitPrice, 4000);
    secondPot = pm->pots[1];
    ASSERT_EQ(secondPot->chipsInPot, 615);
    ASSERT_EQ(secondPot->priceFixed, false);
    ASSERT_EQ(st1->lastBet, 4615);
    ASSERT_EQ(st1->chipsAtHand, 66);
    ASSERT_EQ(st2->lastBet, 4000);
    ASSERT_EQ(st2->chipsAtHand, 0);

    pm->betBySeat(5909, st3);
    ASSERT_EQ(pm->mainPot->chipsInPot, 12080);
    ASSERT_EQ(pm->pots.size(), 2);
    ASSERT_EQ(pm->mainPot->priceFixed, true);
    ASSERT_EQ(pm->mainPot->unitPrice, 4000);
    secondPot = pm->pots[1];
    ASSERT_EQ(secondPot->chipsInPot, 615+1909);
    ASSERT_EQ(secondPot->priceFixed, true);
    ASSERT_EQ(st1->lastBet, 4615);
    ASSERT_EQ(st1->chipsAtHand, 66);
    ASSERT_EQ(st2->lastBet, 4000);
    ASSERT_EQ(st2->chipsAtHand, 0);
    ASSERT_EQ(st3->lastBet, 5909);
    ASSERT_EQ(st3->chipsAtHand, 0);
    
    pm->betBySeat(2849, st4);
    ASSERT_EQ(pm->mainPot->chipsInPot, 80+2849*4);
    ASSERT_EQ(pm->pots.size(), 3);
    ASSERT_EQ(pm->mainPot->priceFixed, true);
    ASSERT_EQ(pm->mainPot->unitPrice, 2849);
    secondPot = pm->pots[1];
    ASSERT_EQ(secondPot->chipsInPot, (4000-2849)*3);
    ASSERT_EQ(secondPot->priceFixed, true);
    thirdPot = pm->pots[2];
    ASSERT_EQ(thirdPot->chipsInPot, 615+1909);
    ASSERT_EQ(thirdPot->priceFixed, true);
    ASSERT_EQ(st1->lastBet, 4615);
    ASSERT_EQ(st1->chipsAtHand, 66);
    ASSERT_EQ(st2->lastBet, 4000);
    ASSERT_EQ(st2->chipsAtHand, 0);
    ASSERT_EQ(st3->lastBet, 5909);
    ASSERT_EQ(st3->chipsAtHand, 0);
    ASSERT_EQ(st4->lastBet, 2849);
    ASSERT_EQ(st4->chipsAtHand, 0);

    pm->betBySeat(4681, st1);
    ASSERT_EQ(pm->mainPot->chipsInPot, 80+2849*4);
    ASSERT_EQ(pm->pots.size(), 4);
    ASSERT_EQ(pm->mainPot->priceFixed, true);
    ASSERT_EQ(pm->mainPot->unitPrice, 2849);
    secondPot = pm->pots[1];
    ASSERT_EQ(secondPot->chipsInPot, (4000-2849)*3);
    ASSERT_EQ(secondPot->unitPrice, 4000-2849);
    ASSERT_EQ(secondPot->priceFixed, true);
    thirdPot = pm->pots[2];
    ASSERT_EQ(thirdPot->chipsInPot, 681*2);
    ASSERT_EQ(thirdPot->unitPrice, 681);
    ASSERT_EQ(thirdPot->priceFixed, true);
    lastPot = pm->pots[3];
    ASSERT_EQ(lastPot->chipsInPot, 1909-681);
    ASSERT_EQ(lastPot->priceFixed, true);
    ASSERT_EQ(st1->lastBet, 4681);
    ASSERT_EQ(st1->chipsAtHand, 0);
    ASSERT_EQ(st2->lastBet, 4000);
    ASSERT_EQ(st2->chipsAtHand, 0);
    ASSERT_EQ(st3->lastBet, 5909);
    ASSERT_EQ(st3->chipsAtHand, 0);
    ASSERT_EQ(st4->lastBet, 2849);
    ASSERT_EQ(st4->chipsAtHand, 0);
}

TEST_F(LYPotManager_tests, newPot)
{
	ASSERT_EQ(pm->mainPot->chipsInPot, 0);
    int i = 0;
    BOOST_FOREACH(LYSeatPtr st, openSeats)
    {
        st->chipsAtHand = 10+i*10;
        st->chipsAtBeginning = 10+i*10;
        i++;
    }
    LYSeatPtr seat1 = openSeats[0];
    pm->betBySeat(10, seat1);
    ASSERT_EQ(pm->mainPot->chipsInPot, 10);
    ASSERT_TRUE(pm->mainPot->priceFixed);
    ASSERT_EQ(pm->pots.size(), 1);
    ASSERT_EQ(seat1->lastBet, 10);
    ASSERT_EQ(seat1->chipsAtBeginning, 10);
    ASSERT_EQ(seat1->chipsAtHand, 0);

    LYSeatPtr seat2 = openSeats[1];
    pm->betBySeat(20, seat2);
    ASSERT_EQ(pm->mainPot->chipsInPot, 20);
    ASSERT_EQ(pm->pots.size(), 2);
    LYPotPtr lastPot = pm->pots.back();
    ASSERT_EQ(lastPot->chipsInPot, 10);
    ASSERT_EQ(seat2->lastBet, 20);
    ASSERT_EQ(seat2->chipsAtBeginning, 20);
    ASSERT_EQ(seat2->chipsAtHand, 0);

    LYSeatPtr seat3 = openSeats[2];
    pm->betBySeat(20, seat3);
    ASSERT_EQ(pm->mainPot->chipsInPot, 30);
    ASSERT_EQ(pm->pots.size(), 2);
    lastPot = pm->pots.back();
    ASSERT_EQ(lastPot->chipsInPot, 20);
    ASSERT_EQ(seat3->lastBet, 20);
    ASSERT_EQ(seat3->chipsAtBeginning, 30);
    ASSERT_EQ(seat3->chipsAtHand, 10);

    LYSeatPtr seat4 = openSeats[3];
    pm->betBySeat(40, seat4);
    ASSERT_EQ(pm->mainPot->chipsInPot, 40);
    ASSERT_EQ(pm->pots.size(), 3);
    lastPot = pm->pots.back();
    ASSERT_EQ(lastPot->chipsInPot, 20);
    LYPotPtr secondPot = pm->pots[1];
    ASSERT_EQ(secondPot->chipsInPot, 30);
    ASSERT_EQ(seat4->lastBet, 40);
    ASSERT_EQ(seat4->chipsAtBeginning, 40);
    ASSERT_EQ(seat4->chipsAtHand, 0);

}

TEST_F(LYPotManager_tests, a_real_hand)
{
    LYSeatPtr seat1 = openSeats[0];
    seat1->chipsAtHand = 14700;
    seat1->chipsAtBeginning = 14700;
    LYSeatPtr seat2 = openSeats[1];
    seat2->chipsAtHand = 9600;
    seat2->chipsAtBeginning = 9600;
    LYSeatPtr seat3 = openSeats[2];
    seat3->chipsAtHand = 10700;
    seat3->chipsAtBeginning = 10700;

    pm->betBySeat(10, seat3);
    pm->betBySeat(25, seat1);
    pm->betBySeat(25, seat2);
    pm->betBySeat(25, seat3);

    ASSERT_EQ(pm->mainPot->chipsInPot, 25*3);
    ASSERT_EQ(pm->pots.size(), 1);
    ASSERT_EQ(seat1->lastBet, 25);

    pm->betBySeat(10700, seat1);
    ASSERT_EQ(pm->mainPot->chipsInPot, 25*2 + 10700);
    ASSERT_EQ(pm->pots.size(), 1);
    ASSERT_EQ(seat1->lastBet, 10700);

    pm->betBySeat(9600, seat2);
    ASSERT_EQ(pm->mainPot->chipsInPot, 25 + 9600*2);
    ASSERT_EQ(pm->pots.size(), 2);
    ASSERT_EQ(seat2->lastBet, 9600);
    LYPotPtr lastPot = pm->pots.back();
    ASSERT_EQ(lastPot->chipsInPot, 1100);
    ASSERT_EQ(lastPot->potLevel, 2);
    ASSERT_EQ(lastPot->seatsInPot.size(), 1);
    ASSERT_EQ(lastPot->seatsInPot[0], LYSeat1);

    pm->betBySeat(10700, seat3);
    ASSERT_EQ(pm->mainPot->chipsInPot, 9600*3);
    ASSERT_EQ(pm->pots.size(), 2);
    ASSERT_EQ(seat3->lastBet, 10700);
    ASSERT_EQ(lastPot->chipsInPot, 1100*2);
    ASSERT_EQ(lastPot->seatsInPot.size(), 2);
    ASSERT_EQ(lastPot->seatsInPot[1], LYSeat3);
}
