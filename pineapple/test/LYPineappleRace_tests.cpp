/*
 * LYPineappleAction_tests.cpp
 *
 *  Created on: 2014-9-25
 *      Author: caiqingfeng
 */

#include <gtest/gtest.h>

#include "src/LYPineappleRace.h"
//#include "common/src/my_log.h"
#include "poker/src/LYDeck.h"

class LYPineappleRace_tests : public ::testing::Test
{
protected:

    void SetUp()
    {
    	pr = new LYPineappleRace;
    	pr->scoreAgainst = 100;
    	pr->player = 1;
    	pr->playerAgainst = 2;
    	pr->statusAgainst = LYBEATALL;
    	pr->scoreAgainstTop = 13;
    	pr->scoreAgainstMiddle = 23;
    	pr->scoreAgainstBottom = 32;
    }
    void TearDown()
    {
    	delete pr;
   }
public:
    LYPineappleRace *pr;
};

TEST_F(LYPineappleRace_tests, turnNegative)
{
	LYPineappleRace npr;
	npr = pr->turnNegative();
	ASSERT_EQ(npr.player, pr->playerAgainst);
	ASSERT_EQ(npr.playerAgainst, pr->player);
	ASSERT_EQ(npr.scoreAgainst, pr->scoreAgainst * (-1));
	ASSERT_EQ(npr.statusAgainst, LYSCOOPED);
	ASSERT_EQ(npr.scoreAgainstTop, pr->scoreAgainstTop * (-1));
	ASSERT_EQ(npr.scoreAgainstMiddle, pr->scoreAgainstMiddle * (-1));
	ASSERT_EQ(npr.scoreAgainstBottom, pr->scoreAgainstBottom * (-1));
}
