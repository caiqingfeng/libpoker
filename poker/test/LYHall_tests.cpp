/*
 * LYHoldemHall_tests.cpp
 *
 *  Created on: 2013-6-27
 *      Author: caiqingfeng
 */

#include <gtest/gtest.h>
//#include <gmock/gmock.h>
#include "src/LYHall.h"

class LYHall_tests: public ::testing::Test
{
protected:

    void SetUp()
    {
    	LYPlayerMgr *pm = new LYPlayerMgr();
        hall = new LYHall(pm);
        hall->createTable("table_id_is_me", 9, LYTableOpen, "first table");
        table = hall->fetchTableById("table_id_is_me");
    }

    void TearDown()
    {
        delete hall;
    }

private:
    LYHall *hall;
    
public:
    LYTablePtr table;
};

TEST_F(LYHall_tests, init)
{
	ASSERT_EQ(NULL != table.get(), true);
	ASSERT_EQ(table->getTotalSeats(), 9);
	ASSERT_EQ(table->getStatus(), LYTableOpen);
}
