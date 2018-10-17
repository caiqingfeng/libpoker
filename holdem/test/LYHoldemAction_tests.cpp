/*
 * LYHoldemGame_tests.cpp
 *
 *  Created on: 2013-7-6
 *      Author: caiqingfeng
 */

#include "src/LYHoldemAction.h"
#include <string>
#include "poker/src/LYDeck.h"

#include <gtest/gtest.h>

using std::string;

class LYHoldemAction_tests : public ::testing::Test
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

TEST_F(LYHoldemAction_tests, toString)
{
	LYHoldemAction action("default");
	action.call(LYSeat1, 1000);

    std::string s = action.toString();
//    std::cout << s << std::endl;
	LYHoldemAction action2 = LYHoldemAction::build_from_action_string(s);
    std::string s2 = action2.toString();
//    std::cout << s2 << std::endl;
	ASSERT_EQ(s == s2, true);
	ASSERT_EQ(action == action2, true);

	action.issueTurnCard(sA);

    s = action.toString();
    std::cout << s << std::endl;
	action2 = LYHoldemAction::build_from_action_string(s);
    s2 = action2.toString();
    std::cout << s2 << std::endl;
	ASSERT_EQ(s == s2, true);
	ASSERT_EQ(action == action2, true);
}

