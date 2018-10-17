#include <gtest/gtest.h>
#include <iostream>

#include "../src/ServerHall.h"
#include "Fixture.h"

class ServerHall_test: public ::testing::Test {
public:
	void SetUp() {
		fixture = new Fixture();
		pm = fixture->pm;
		hall = fixture->hall;
	}

	void TearDown() {
		delete fixture;
	}

public:
	ServerHall_instance* hall;
	ServerPlayerMgr_instance* pm;
	Fixture* fixture;
};

TEST_F(ServerHall_test, registerTimer)
{
	LYPokerTimerPtr t = hall->getTimer(std::string("helloworld"));
	ASSERT_EQ(t.get()!=NULL, false);

	hall->registerTimer(std::string("helloworld"), TIMER_ID_REQ_NEW_GAME, 0);
	t = hall->getTimer(std::string("helloworld"));
	ASSERT_EQ(t.get()!=NULL, true);
}

TEST_F(ServerHall_test, registerPollingTimer)
{
	hall->registerTimer(std::string("helloworld"), TIMER_ID_POLL_SEAT1, 0);
	LYPokerTimerPtr t = hall->getTimer(std::string("helloworld"));
	ASSERT_EQ(t.get()!=NULL, true);

	ASSERT_EQ(t->tableTimers.size(), 1);
	ASSERT_EQ(t->tableTimers.find(TIMER_ID_POLL_SEAT1) != t->tableTimers.end(), true);
}

TEST_F(ServerHall_test, clearTimer)
{
	hall->registerTimer(std::string("helloworld"), TIMER_ID_REQ_NEW_GAME, 0);
	LYPokerTimerPtr t = hall->getTimer(std::string("helloworld"));
	ASSERT_EQ(t.get()!=NULL, true);
	ASSERT_EQ(t->tableTimers.size(), 1);

	hall->clearTimer(std::string("helloworld"), TIMER_ID_REQ_NEW_GAME);
	t = hall->getTimer(std::string("helloworld"));
	ASSERT_EQ(t.get()!=NULL, true);
	ASSERT_EQ(t->tableTimers.size(), 0);
}

TEST_F(ServerHall_test, enterTable_leave)
{
	hall->enterTable("1", "user");
	LYTablePtr table = hall->fetchTableById("1");
//	getchar();
	ASSERT_EQ(table.get()!=NULL, true);
	ASSERT_EQ(table->total_seats, 9);
	ASSERT_EQ(table->last_duration, 360);
	ASSERT_EQ(table->havePlayer("user"), true);

	hall->leaveTable("1", "user");
	ASSERT_EQ(table->havePlayer("user"), false);

}

TEST_F(ServerHall_test, takeSeat_leave)
{
	hall->enterTable("1", "user");
	hall->takeSeat("1", LYSeat1, "user", 100);
	LYTablePtr table = hall->fetchTableById("1");
	LYSeatPtr seat = table->fetchSeat(LYSeat1);
	ASSERT_EQ(seat->status, LYSeatOccupied);
	ASSERT_EQ(seat->playerId, "user");
	ASSERT_EQ(table->getSeatedPlayers(), 1);

	seat->status = LYSeatInGame;
	hall->enterTable("1", "2");
	hall->takeSeat("1", LYSeat2, "2", 100);
	seat = table->fetchSeat(LYSeat2);
	ASSERT_EQ(seat->status, LYSeatOccupied);
	ASSERT_EQ(seat->playerId, "2");
	ASSERT_EQ(table->getSeatedPlayers(), 2);

	hall->leaveSeat("1", LYSeat1, "user", 100);
	seat = table->fetchSeat(LYSeat1);
	ASSERT_EQ(seat->status, LYSeatOpen);
}

TEST_F(ServerHall_test, buyin)
{
	hall->enterTable("1", "user");
	hall->takeSeat("1", LYSeat1, "user", 100);
	LYTablePtr table = hall->fetchTableById("1");
	LYSeatPtr seat = table->fetchSeat(LYSeat1);
	ASSERT_EQ(seat->status, LYSeatOccupied);
	ASSERT_EQ(seat->playerId, "user");
	ASSERT_EQ(seat->chipsAtHand, 100);
	ASSERT_EQ(seat->newBuyin, 0);
}

TEST_F(ServerHall_test, onTimeout_closeTable)
{
	hall->enterTable("1", "user");
	hall->takeSeat("1", LYSeat1, "user", 100);
	LYTablePtr table = hall->fetchTableById("1");
	ASSERT_EQ(table->status, LYTableOpen);
	LYTimer t("1", 1, 1);
	t.set_client_timer_id("1");
	t.set_second_client_timer_id(TIMER_ID_CLOSE_TABLE);
	t.set_action(com::luyun::timer::shared::LY_TIMER_FIRE);
	hall->onTimeout(t);

	ASSERT_EQ(table->status, LYTableClose);
}
