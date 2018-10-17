/*
 * LYHandOdds_tests.cpp
 *
 *  Created on: 2013-6-5
 *      Author: caiqingfeng
 */

#include <gtest/gtest.h>

#include "src/LYHandOdds.h"
#include "poker/src/LYDeck.h"
//#include "common/src/my_log.h"

class LYHandOdds_tests : public ::testing::Test
{
protected:

    void SetUp()
    {
    	std::vector<LYCard> cards;
    	cards.push_back(sA);
    	cards.push_back(sK);
    	cards.push_back(hT);
    	cards.push_back(hA);
    	cards.push_back(s8);
    	cards.push_back(d2);
    	cards.push_back(c2);

    	ho = new LYHandOdds(cards);
    }
    void TearDown()
    {
    	delete ho;
   }
public:
    LYHandOdds *ho;

};

TEST_F(LYHandOdds_tests, init)
{
	ASSERT_EQ(ho->cardString, "AsAhKsTh8s2d2c");
	std::vector<std::string> indexs = ho->genIndex(sA);
	ASSERT_EQ(ho->flush_base(), 3);
	ASSERT_EQ(ho->straight_base(), 3);
}
