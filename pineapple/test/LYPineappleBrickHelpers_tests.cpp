/*
 * LYPineappleBrick_tests.cpp
 *
 *  Created on: 2014-9-25
 *      Author: caiqingfeng
 */

#include <gtest/gtest.h>

#include "src/LYPineappleBrickHelpers.h"
#include <boost/lexical_cast.hpp>

class LYPineappleBrickHelpers_tests : public ::testing::Test
{
protected:

    void SetUp()
    {
    	LYPineappleBrick brick(1, LYCard(23));
    	LYPineappleBrick brick2(3, LYCard(21));
    	LYPineappleBrick brick3(6, LYCard(2));
    	LYPineappleBrick brick4(8, LYCard(3));
    	LYPineappleBrick brick5(15, LYCard(52));
    	bricks.push_back(brick);
    	bricks.push_back(brick2);
    	bricks.push_back(brick3);
    	bricks.push_back(brick4);
    	bricks.push_back(brick5);
    }
    void TearDown()
    {
   }
public:
	std::vector<LYPineappleBrick> bricks;

};

TEST_F(LYPineappleBrickHelpers_tests, to_s)
{
	std::string ps;
	LYPineappleBrickHelpers::bricksToStringifiedInt(bricks, ps);
	ASSERT_EQ(ps, "1:23,3:21,6:2,8:3,15:52,");
}

TEST_F(LYPineappleBrickHelpers_tests, to_bricks)
{
	std::string ps = "0:45,1:23,5:38,,,,";
	bricks.clear();
	LYPineappleBrickHelpers::stringifiedIntToBricks(ps, bricks);
	ASSERT_EQ(bricks.size(), 3);
	ASSERT_EQ(bricks[0].brick, 0);
	ASSERT_EQ(bricks[0].card.toInteger(), 45);
	ASSERT_EQ(bricks[1].brick, 1);
	ASSERT_EQ(bricks[1].card.toInteger(), 23);
	ASSERT_EQ(bricks[2].brick, 5);
	ASSERT_EQ(bricks[2].card.toInteger(), 38);
}

TEST_F(LYPineappleBrickHelpers_tests, to_bricks2)
{
	std::string ps = "1:51,2:38,3:2,4:5,5:34,6:26,7:29,8:37,9:40,10:22,11:25,12:32,13:24,";
	bricks.clear();
	LYPineappleBrickHelpers::stringifiedIntToBricks(ps, bricks);
	ASSERT_EQ(bricks.size(), 13);
//	for (unsigned int i=0; i<13; i++) {
//		std::cout << bricks[i].card.toString() << ",";
//		if (i==2 || i==7) std::cout << std::endl;
//	}
}

TEST_F(LYPineappleBrickHelpers_tests, to_bricks3)
{
	std::string ps = "1:9,2:10,3:30,4:23,5:39,6:3,7:13,8:1,9:17,10:42,11:45,12:46,13:41";
	bricks.clear();
	LYPineappleBrickHelpers::stringifiedIntToBricks(ps, bricks);
	ASSERT_EQ(bricks.size(), 13);
//	for (unsigned int i=0; i<13; i++) {
//		std::cout << bricks[i].card.toString() << ",";
//		if (i==2 || i==7) std::cout << std::endl;
//	}
}
