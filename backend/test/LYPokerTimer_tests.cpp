/*
 * AppdHandlerImpl_tests.cpp
 *
 *  Created on: 2014-4-14
 *      Author: caiqingfeng
 */

#include "backend/src/LYPokerTimer.h"
#include "common/src/my_log.h"
#include "Fixture.h"
#include "MessageStubs.h"
#include "backend/src/ServerHall.h"

#include <gtest/gtest.h>

class LYPokerTimer_tests : public ::testing::Test{
public:

    void SetUp()
    {
    	fixture = new Fixture();
    	mockHandler = fixture->mockHandler;
    	pt = new LYPokerTimer();
    }

    void TearDown()
    {
        delete fixture;
        delete pt;
    }

public:
    Fixture* fixture;
    MockMsgHandler* mockHandler;
    LYPokerTimer *pt;
};

TEST_F(LYPokerTimer_tests, clearTimersExceptCloseTable)
{
    LYTimerPtr t(new LYTimer());
	t->set_client_timer_id("table_id");
	t->set_second_client_timer_id(TIMER_ID_CLOSE_TABLE);
	t->set_after(30.0);

	pt->tableTimers[TIMER_ID_CLOSE_TABLE] = t;
	ASSERT_EQ(pt->tableTimers.size(), 1);
	pt->clearTimersExceptCloseTable();
	ASSERT_EQ(pt->tableTimers.size(), 1);
	pt->tableTimers["polling1"] = t;
	ASSERT_EQ(pt->tableTimers.size(), 2);
	pt->clearTimersExceptCloseTable();
	ASSERT_EQ(pt->tableTimers.size(), 1);

	std::map<std::string, LYTimerPtr>::iterator it = pt->tableTimers.find(TIMER_ID_CLOSE_TABLE);
	ASSERT_EQ(it!=pt->tableTimers.end(), true);
	LYTimerPtr nt = it->second;
	ASSERT_EQ(nt->get_after(), 30.0);
}

