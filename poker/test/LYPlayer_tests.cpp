/*
 * LYPlayer_tests.cpp
 *
 *  Created on: 2014-3-16
 *      Author: caiqingfeng
 */

#include <gtest/gtest.h>
//#include <gmock/gmock.h>
#include "src/LYPlayer.h"

class LYPlayer_tests: public ::testing::Test
{
protected:

    void SetUp()
    {
    	player = new LYPlayer("player01", "device01");
    }

    void TearDown()
    {
    	delete player;
    }

public:
    LYPlayer *player;

};

TEST_F(LYPlayer_tests, init)
{
	ASSERT_EQ(player->_id, "player01");
	ASSERT_EQ(player->day_in_sequence, 0);
	ASSERT_EQ(player->balance, 0);
}

TEST_F(LYPlayer_tests, get_daily_bonus)
{
	unsigned int seq = 0;
	std::vector<unsigned int> result;
	bool today_applicable = player->list_daily_bonus(seq, result);
	ASSERT_EQ(seq, 1);
	ASSERT_EQ(today_applicable, true);
	ASSERT_EQ(result.size(), 6);

	unsigned int bonus = player->apply_daily_bonus(seq);
	ASSERT_EQ(seq, 1);
	ASSERT_EQ(bonus, 500);
	ASSERT_EQ(player->balance, 500);
	bonus = player->apply_daily_bonus(seq);
	ASSERT_EQ(seq, 1);
	ASSERT_EQ(bonus, 0);
	ASSERT_EQ(player->balance, 500);
	today_applicable = player->list_daily_bonus(seq, result);
	ASSERT_EQ(today_applicable, false);

	player->last_lucky_time -= 86400;
	bonus = player->apply_daily_bonus(seq);
	ASSERT_EQ(seq, 2);
	ASSERT_EQ(bonus, 800);
	ASSERT_EQ(player->balance, 500+800);
	today_applicable = player->list_daily_bonus(seq, result);
	ASSERT_EQ(today_applicable, false);
	ASSERT_EQ(seq, 2);

	player->last_lucky_time -= 86400;
	bonus = player->apply_daily_bonus(seq);
	ASSERT_EQ(seq, 3);
	ASSERT_EQ(bonus, 1000);
	ASSERT_EQ(player->balance, 500+800+1000);

	player->last_lucky_time -= 86400;
	bonus = player->apply_daily_bonus(seq);
	ASSERT_EQ(seq, 4);
	ASSERT_EQ(bonus, 1200);
	ASSERT_EQ(player->balance, 500+800+1000+1200);

	player->last_lucky_time -= 86400;
	bonus = player->apply_daily_bonus(seq);
	ASSERT_EQ(seq, 5);
	ASSERT_EQ(bonus, 1500);
	ASSERT_EQ(player->balance, 500+800+1000+1200+1500);

	player->last_lucky_time -= 86400;
	bonus = player->apply_daily_bonus(seq);
	ASSERT_EQ(seq, 6);
	ASSERT_EQ(bonus, 3000);
	ASSERT_EQ(player->balance, 500+800+1000+1200+1500+3000);

	player->last_lucky_time -= 86400;
	bonus = player->apply_daily_bonus(seq);
	ASSERT_EQ(seq, 7);
	ASSERT_EQ(bonus, 3000);
	ASSERT_EQ(player->balance, 500+800+1000+1200+1500+3000+3000);

	bonus = player->apply_daily_bonus(seq);
	ASSERT_EQ(seq, 7);
	ASSERT_EQ(bonus, 0);
	ASSERT_EQ(player->balance, 500+800+1000+1200+1500+3000+3000);
}
