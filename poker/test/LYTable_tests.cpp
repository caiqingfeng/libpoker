/*
 * LYPlayer_tests.cpp
 *
 *  Created on: 2014-3-16
 *      Author: caiqingfeng
 */

#include <gtest/gtest.h>
//#include <gmock/gmock.h>
#include "src/LYTable.h"

class LYTable_tests: public ::testing::Test
{
protected:

    void SetUp()
    {
    	table = new LYTable("tableid", 6, LYTableOpen, "tabletest");
    }

    void TearDown()
    {
    	delete table;
    }

public:
    LYTable *table;

};

TEST_F(LYTable_tests, table2kvps)
{
	std::vector < std::pair<std::string, std::string> > kvps = table->table2kvps();
	std::vector < std::pair<std::string, std::string> >::iterator it = kvps.begin();
	for (; it!=kvps.end(); it++) {
		std::pair<std::string, std::string> kvp = *it;
		if (kvp.first == "_id") {
			ASSERT_EQ(kvp.second, "tableid");
		}
	}
}

TEST_F(LYTable_tests, isTableOpen)
{
	table->started_at = time(NULL);
	table->last_duration = 60;
	ASSERT_EQ(table->isTableOpen(), true);
}

TEST_F(LYTable_tests, getTimeLeft)
{
	table->started_at = time(NULL) - 30*60;
	table->last_duration = 60;
	ASSERT_EQ(table->getTimeLeft(), 30*60);
}

TEST_F(LYTable_tests, getTotalBuyin)
{
	ASSERT_EQ(table->getTotalBuyin(), 0);
	table->takeSeat(LYSeat1, 1000, "1");
	ASSERT_EQ(table->getTotalBuyin(), 1000);
	table->takeSeat(LYSeat2, 2000, "2");
	ASSERT_EQ(table->getTotalBuyin(), 3000);
}

