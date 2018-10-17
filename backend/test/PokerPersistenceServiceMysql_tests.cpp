/*
 * AppdPersistenceServiceMysql_tests.cpp
 *
 *  Created on: 2014-4-14
 *      Author: caiqingfeng
 */

#include "backend/src/PokerPersistenceServiceMysql.h"
#include "common/src/my_log.h"
#include "Fixture.h"

#include <gtest/gtest.h>

class PokerPersistenceServiceMysql_tests: public ::testing::Test {
public:

	void SetUp()
	{
		fixture = new Fixture();
		aps = fixture->apsm;
	}

	void TearDown()
	{
		delete fixture;
	}

public:
	PokerPersistenceServiceMysql* aps;
	Fixture* fixture;
};

TEST_F(PokerPersistenceServiceMysql_tests, init)
{
	ASSERT_EQ(true, true);
}

TEST_F(PokerPersistenceServiceMysql_tests, load_table)
{
	LYTable *t = new LYTable("tableone", 9, LYTableOpen, "kkkk");
	aps->load_table(t);
//	getchar();
	ASSERT_EQ(t->name, "tableone");
	ASSERT_EQ(t->total_seats, 6);
	ASSERT_EQ(t->getOwner(), "1");
	ASSERT_EQ(t->last_duration, 360);
	ASSERT_EQ(t->assurance_income, 0);
	ASSERT_EQ(t->assurance_flag, 1);
	int now = time(NULL);
	ASSERT_EQ((t->started_at <= now+100) && (t->started_at >= now-100), true);
	ASSERT_EQ(t->isTableOpen(), true);
	ASSERT_EQ(t->getTimeLeft() < 360*60 + 10, true);
	ASSERT_EQ(t->getTimeLeft() > 360*60 - 10, true);
	delete t;
}

TEST_F(PokerPersistenceServiceMysql_tests, fetch_player)
{
	LYPlayer player;
	aps->fetch_player("1", player);
	ASSERT_EQ(player.nick_name, "userone");
}

TEST_F(PokerPersistenceServiceMysql_tests, update_balance)
{
	int balance = 5000;
	aps->update_balance("1", "game1", balance);
	LYPlayer player;
	aps->fetch_player("1", player);
	ASSERT_EQ(player.balance, 6000);
}

TEST_F(PokerPersistenceServiceMysql_tests, get_user_chips)
{
	unsigned int balance = aps->get_user_chips("tableone", "1");
	ASSERT_EQ(balance, 1000);
}

TEST_F(PokerPersistenceServiceMysql_tests, user_chips)
{
	aps->save_user_chips("tableone", "1", 2000);
	int balance = aps->get_user_chips("tableone", "1");
	ASSERT_EQ(balance, 2000);
	aps->save_user_chips("tableone", "1", -2000);
	balance = aps->get_user_chips("tableone", "1");
	ASSERT_EQ(balance, -2000);
}

TEST_F(PokerPersistenceServiceMysql_tests, get_time_left)
{
	LYTable table("tableone", 9, LYTableOpen, "tablename", "1");
	ASSERT_EQ(table.last_duration, 0);
	aps->load_table(&table);
	ASSERT_EQ(table.last_duration, 360);
	ASSERT_EQ(table.getTimeLeft() > 0, true);
}

TEST_F(PokerPersistenceServiceMysql_tests, insurance_income)
{
	LYTable table("tableone", 9, LYTableOpen, "tablename", "1");
	aps->change_insurance_income(table._id, 1000);
	int chips = aps->get_insurance_income(table._id);
	ASSERT_EQ(chips, 1000);
	aps->change_insurance_income(table._id, -100);
	chips = aps->get_insurance_income(table._id);
	ASSERT_EQ(chips, 900);
}

TEST_F(PokerPersistenceServiceMysql_tests, total_buyin)
{
	LYTable table("tableone", 9, LYTableOpen, "tablename", "1");
	aps->increase_total_buyin(table._id, 1000);
	int chips = aps->get_total_buyin(table._id);
	ASSERT_EQ(chips, 1000);
	aps->increase_total_buyin(table._id, 20000);
	chips = aps->get_total_buyin(table._id);
	ASSERT_EQ(chips, 21000);
}

TEST_F(PokerPersistenceServiceMysql_tests, occupied_seats)
{
	LYTable table("tableone", 9, LYTableOpen, "tablename", "1");
	aps->change_occupied_seats(table._id, true);
	int seats = aps->get_occupied_seats(table._id);
	ASSERT_EQ(seats, 1);
	aps->change_occupied_seats(table._id, true);
	seats = aps->get_occupied_seats(table._id);
	ASSERT_EQ(seats, 2);
	aps->change_occupied_seats(table._id, false);
	seats = aps->get_occupied_seats(table._id);
	ASSERT_EQ(seats, 1);
}

TEST_F(PokerPersistenceServiceMysql_tests, user_buyin)
{
	LYTable table("tableone", 9, LYTableOpen, "tablename", "1");
	aps->increase_user_buyin(table._id, "1", 3000);
	int chips = aps->get_user_buyin(table._id, "1");
	ASSERT_EQ(chips, 3000);
	aps->increase_user_buyin(table._id, "1", 5000);
	chips = aps->get_user_buyin(table._id, "1");
	ASSERT_EQ(chips, 8000);
	chips = aps->get_user_chips(table._id, "1");
	ASSERT_EQ(chips, 1000);

	aps->increase_user_chips_buyin_to_db(table._id, "1", 5000);
	chips = aps->get_user_buyin(table._id, "1");
	ASSERT_EQ(chips, 13000);
	chips = aps->get_user_chips(table._id, "1");
	ASSERT_EQ(chips, 6000);
}

TEST_F(PokerPersistenceServiceMysql_tests, user_club_income)
{
	aps->change_user_club_income("1", "1", 3000);
	int r = aps->get_user_club_income("1", "1");
	ASSERT_EQ(r, 3000);
	aps->change_user_club_income("1", "1", -2000);
	r = aps->get_user_club_income("1", "1");
	ASSERT_EQ(r, 1000);
	aps->change_user_club_income("1", "1", -2000);
	r = aps->get_user_club_income("1", "1");
	ASSERT_EQ(r, -1000);
}

TEST_F(PokerPersistenceServiceMysql_tests, club_games)
{
	aps->increase_club_games("1");
	int r = aps->get_club_games("1");
	ASSERT_EQ(r, 1);
	aps->increase_club_games("1");
	r = aps->get_club_games("1");
	ASSERT_EQ(r, 2);
	aps->increase_club_games("1");
	r = aps->get_club_games("1");
	ASSERT_EQ(r, 3);
}

TEST_F(PokerPersistenceServiceMysql_tests, user_club_games)
{
	aps->increase_user_club_games("1", "1");
	int r = aps->get_user_club_games("1", "1");
	ASSERT_EQ(r, 1);
	aps->increase_user_club_games("1", "1");
	r = aps->get_user_club_games("1", "1");
	ASSERT_EQ(r, 2);
	aps->increase_user_club_games("1", "1");
	r = aps->get_user_club_games("1", "1");
	ASSERT_EQ(r, 3);
}

TEST_F(PokerPersistenceServiceMysql_tests, user_club_inpot)
{
	aps->increase_user_club_inpots("1", "1");
	int r = aps->get_user_club_inpots("1", "1");
	ASSERT_EQ(r, 1);
	aps->increase_user_club_inpots("1", "1");
	r = aps->get_user_club_inpots("1", "1");
	ASSERT_EQ(r, 2);
	aps->increase_user_club_inpots("1", "1");
	r = aps->get_user_club_inpots("1", "1");
	ASSERT_EQ(r, 3);
}

TEST_F(PokerPersistenceServiceMysql_tests, user_club_wins)
{
	aps->increase_user_club_wins("1", "1");
	int r = aps->get_user_club_wins("1", "1");
	ASSERT_EQ(r, 1);
	aps->increase_user_club_wins("1", "1");
	r = aps->get_user_club_wins("1", "1");
	ASSERT_EQ(r, 2);
	aps->increase_user_club_wins("1", "1");
	r = aps->get_user_club_wins("1", "1");
	ASSERT_EQ(r, 3);
}

TEST_F(PokerPersistenceServiceMysql_tests, user_table_income)
{
	int r;
	r = aps->get_user_chips("tableone", "1");
	ASSERT_EQ(r, 1000);
	aps->change_user_table_income("tableone", "1", 3000);
	r = aps->get_user_table_income("tableone", "1");
	ASSERT_EQ(r, 3000);
	r = aps->get_user_chips("tableone", "1");
	ASSERT_EQ(r, 4000);
	aps->change_user_table_income("tableone", "1", -2000);
	r = aps->get_user_table_income("tableone", "1");
	ASSERT_EQ(r, 1000);
	r = aps->get_user_chips("tableone", "1");
	ASSERT_EQ(r, 2000);
	aps->change_user_table_income("tableone", "1", -2000);
	r = aps->get_user_table_income("tableone", "1");
	ASSERT_EQ(r, -1000);
	r = aps->get_user_chips("tableone", "1");
	ASSERT_EQ(r, 0);
}

TEST_F(PokerPersistenceServiceMysql_tests, user_table_games)
{
	aps->increase_user_table_games("tableone", "1");
	int r = aps->get_user_table_games("tableone", "1");
	ASSERT_EQ(r, 1);
	aps->increase_user_table_games("tableone", "1");
	r = aps->get_user_table_games("tableone", "1");
	ASSERT_EQ(r, 2);
	aps->increase_user_table_games("tableone", "1");
	r = aps->get_user_table_games("tableone", "1");
	ASSERT_EQ(r, 3);
}

TEST_F(PokerPersistenceServiceMysql_tests, user_table_inpot)
{
	aps->increase_user_table_inpots("tableone", "1");
	int r = aps->get_user_table_inpots("tableone", "1");
	ASSERT_EQ(r, 1);
	aps->increase_user_table_inpots("tableone", "1");
	r = aps->get_user_table_inpots("tableone", "1");
	ASSERT_EQ(r, 2);
	aps->increase_user_table_inpots("tableone", "1");
	r = aps->get_user_table_inpots("tableone", "1");
	ASSERT_EQ(r, 3);
}

TEST_F(PokerPersistenceServiceMysql_tests, user_table_wins)
{
	aps->increase_user_table_wins("tableone", "1");
	int r = aps->get_user_table_wins("tableone", "1");
	ASSERT_EQ(r, 1);
	aps->increase_user_table_wins("tableone", "1");
	r = aps->get_user_table_wins("tableone", "1");
	ASSERT_EQ(r, 2);
	aps->increase_user_table_wins("tableone", "1");
	r = aps->get_user_table_wins("tableone", "1");
	ASSERT_EQ(r, 3);
}

