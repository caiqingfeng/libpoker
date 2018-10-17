/*
 * LYProfileMgr_tests.cpp
 *
 *  Created on: 2013-7-13
 *      Author: caiqingfeng
 */

#include <gtest/gtest.h>

#include "src/LYProfileMgr.h"

class LYProfileMgr_tests : public ::testing::Test
{
protected:

    void SetUp()
    {
    	profileMgr = new LYProfileMgr();
    }

    void TearDown()
    {
    	delete profileMgr;
    }

public:
    LYProfileMgr *profileMgr;
};

TEST_F(LYProfileMgr_tests, init)
{
	ASSERT_EQ(true, true);
}

TEST_F(LYProfileMgr_tests, isValidBuyin)
{
	ASSERT_EQ(false, profileMgr->isValidBuyin("not_exist_profile", 1000, 100));
	LYHoldemProfile profile;
	ASSERT_EQ(true, profile.isValidBuyin(0, 5000));
	ASSERT_EQ(true, profile.isValidBuyin(0, 1000));
	ASSERT_EQ(false, profile.isValidBuyin(1, 5000));
	ASSERT_EQ(false, profile.isValidBuyin(0, 999));
}

