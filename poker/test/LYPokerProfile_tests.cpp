/*
 * LYPlayer_tests.cpp
 *
 *  Created on: 2014-3-16
 *      Author: caiqingfeng
 */

#include <gtest/gtest.h>
//#include <gmock/gmock.h>
#include "src/LYPokerProfile.h"

class LYPokerProfile_tests: public ::testing::Test
{
protected:

    void SetUp()
    {
    	prof = new LYPokerProfile(1, 2);
    }

    void TearDown()
    {
    	delete prof;
    }

public:
    LYPokerProfile *prof;

};

TEST_F(LYPokerProfile_tests, isValidBuyin)
{
	prof->min_chips = 100;
	prof->max_chips = 500;
	ASSERT_EQ(prof->isValidBuyin(0, 400), true);

	ASSERT_EQ(prof->isValidBuyin(0, 105), false);
	ASSERT_EQ(prof->isValidBuyin(0, 600), false);
}

