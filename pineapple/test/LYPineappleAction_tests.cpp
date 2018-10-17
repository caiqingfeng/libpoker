/*
 * LYPineappleAction_tests.cpp
 *
 *  Created on: 2014-9-25
 *      Author: caiqingfeng
 */

#include <gtest/gtest.h>

#include "src/LYPineappleAction.h"
//#include "common/src/my_log.h"
#include "poker/src/LYDeck.h"
#include <string>

class LYPineappleAction_tests: public ::testing::Test {
protected:

	void SetUp() {
	}
	void TearDown() {
	}
public:

};

TEST_F(LYPineappleAction_tests, init)
{
	LYPineappleAction pa;
	ASSERT_EQ(pa.verb, LYCMD_RESET);
	ASSERT_EQ(pa.applicant, LYEMPTYSEAT);
	std::string table_id = "1";
	unsigned int applicant =1;
	std::string bricks = "0:8,0:22,0:40,0:34,0:45";
	unsigned int verb = 1;
	LYPineappleAction ta(table_id,applicant,bricks,verb);
	std::ostringstream ios;
	ios << "table:" << table_id << std::endl;
	ios << "applicant:" << applicant << std::endl;
	ios << "player:" << "" << std::endl;
	ios << "verb:" << verb << std::endl;
	ios<<"briks:0:8"<<std::endl;
	ios<<"briks:0:22"<<std::endl;
	ios<<"briks:0:40"<<std::endl;
	ios<<"briks:0:34"<<std::endl;
	ios<<"briks:0:45"<<std::endl;
	ASSERT_EQ(ios.str(), ta.toString());
}

TEST_F(LYPineappleAction_tests, sortedFace)
{
	LYPineappleAction pa;
	LYPineappleBrick brick(LY_UNDEFINED_POSITION, sA);
	pa.bricks.push_back(brick);
	LYPineappleBrick brick2(LY_UNDEFINED_POSITION, hA);
	pa.bricks.push_back(brick2);
	LYPineappleBrick brick3(LY_UNDEFINED_POSITION, d2);
	pa.bricks.push_back(brick3);
	LYPineappleBrick brick4(LY_UNDEFINED_POSITION, dT);
	pa.bricks.push_back(brick4);
	LYPineappleBrick brick5(LY_UNDEFINED_POSITION, cJ);
	pa.bricks.push_back(brick5);

	std::string sf = pa.sortedByFaceString();
	ASSERT_EQ(sf, "1,2,16,19,51,");
	std::string ss = pa.sortedBySuitString();
	ASSERT_EQ(ss, "1,2,19,51,16,");
}
