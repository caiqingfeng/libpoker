/*
 * LYPineappleBrick_tests.cpp
 *
 *  Created on: 2014-9-25
 *      Author: caiqingfeng
 */

#include <gtest/gtest.h>

#include "src/LYPineappleBrick.h"
//#include "common/src/my_log.h"
#include "poker/src/LYDeck.h"
#include <boost/lexical_cast.hpp>

class LYPineappleBrick_tests : public ::testing::Test
{
protected:

    void SetUp()
    {
    }
    void TearDown()
    {
   }
public:

};

TEST_F(LYPineappleBrick_tests, init)
{
	LYPineappleBrick brick(LY_UNDEFINED_POSITION, sA);
	ASSERT_EQ(brick.card, sA);
	ASSERT_EQ(brick.brick, LY_UNDEFINED_POSITION);
}

TEST_F(LYPineappleBrick_tests, to_s)
{
	LYPineappleBrick brick(LY_UNDEFINED_POSITION, sA);
	ASSERT_EQ(brick.toString(), "0:"+boost::lexical_cast<std::string>(sA.toInteger()));
}
