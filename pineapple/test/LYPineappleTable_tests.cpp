/*
 * LYPineappleAction_tests.cpp
 *
 *  Created on: 2014-9-25
 *      Author: caiqingfeng
 */

#include <gtest/gtest.h>

#include "src/LYPineappleTable.h"
//#include "common/src/my_log.h"
#include "poker/src/LYDeck.h"
#include "Fixture.h"

class LYPineappleTable_tests : public ::testing::Test
{
protected:

    void SetUp()
    {
    	fixture = new Fixture();
    	std::string creator = "creator";
    	pt = fixture->pt;
    }
    void TearDown()
    {
    	delete fixture;
   }
public:
    Fixture* fixture;
    LYPineappleTable *pt;
};

TEST_F(LYPineappleTable_tests, init)
{
	ASSERT_EQ(pt->owner, "creator");
	ASSERT_EQ(pt->total_seats, 3);
}


