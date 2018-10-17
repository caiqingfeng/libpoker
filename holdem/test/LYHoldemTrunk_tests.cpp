/*
 * LYHoldemTrunk_tests.cpp
 *
 *  Created on: 2013-7-5
 *      Author: caiqingfeng
 */

#include "src/LYHoldemTrunk.h"
#include "src/LYHoldemTable.h"
//#include "common/src/my_log.h"

#include <gtest/gtest.h>
#include <string>
using std::string;

class LYHoldemTrunk_tests : public ::testing::Test
{
protected:

    void SetUp()
    {
    	table = new LYHoldemTable(string("this_is_table_id"), 9, LYTableOpen, string("this_is_table_name"));
    	table->openTrunk("999", "trunk_name", "player01", "888");
    	trunk = (LYHoldemTrunk *)table->getTrunk();
    }
    void TearDown()
    {
    	delete table;
   }
public:
    LYHoldemTable *table;
    LYHoldemTrunk  *trunk;
};

TEST_F(LYHoldemTrunk_tests, init)
{
    ASSERT_EQ(table->getStatus(), LYTableOpen);
    ASSERT_EQ(trunk != NULL, true);
}

TEST_F(LYHoldemTrunk_tests, createGame)
{
	ASSERT_EQ(trunk->ready2go(), false);
	table->takeSeat(LYSeat1, 1000, "player01");
	table->takeSeat(LYSeat2, 1000, "player02");
	ASSERT_EQ(trunk->ready2go(), true);
	trunk->createGame("", NULL);
	ASSERT_EQ(trunk->getCurrentGame() != NULL, true);
}

