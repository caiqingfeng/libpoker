/*
 * LYHoldemTable_tests.cpp
 *
 *  Created on: 2013-7-6
 *      Author: caiqingfeng
 */

#include "src/LYHoldemTable.h"
//#include "common/src/my_log.h"

#include <gtest/gtest.h>
#include <string>
using std::string;

class LYHoldemTable_tests : public ::testing::Test
{
protected:

    void SetUp()
    {
    	table = new LYHoldemTable(string("this_is_table_id"), 9, LYTableOpen, string("this_is_table_name"));
    }
    void TearDown()
    {
    	delete table;
   }
public:
    LYHoldemTable *table;
};

TEST_F(LYHoldemTable_tests, init)
{
    ASSERT_EQ(table->getStatus(), LYTableOpen);
    LYSeatPtr st1 = table->fetchSeat(LYSeat1);
    ASSERT_EQ(st1.get() == NULL, false);
    LYHoldemSeat *seat1 = (LYHoldemSeat *)st1.get();
    ASSERT_EQ(seat1->firstCard(), flipCard);
    ASSERT_EQ(seat1->secondCard(), flipCard);

    LYSeatPtr st2 = table->fetchNextSeatInGame(LYSeat1);
    ASSERT_EQ(st2.get() == NULL, true);
}

TEST_F(LYHoldemTable_tests, openTrunk)
{
	table->openTrunk("999", "trunk_name", "player01", "888");
	LYTrunk *trunk = table->getTrunk();
	ASSERT_EQ(trunk != NULL, true);
}

TEST_F(LYHoldemTable_tests, closeTrunk)
{
	table->closeTrunk();
	LYTrunk *trunk = table->getTrunk();
	ASSERT_EQ(trunk == NULL, true);
}

