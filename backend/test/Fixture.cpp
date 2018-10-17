/*
 * Fixture.cpp
 *
 *  Created on: 2014-4-10
 *      Author: caiqingfeng
 */

#include <string>
#include "Fixture.h"

zmq::context_t z_context(1);
zmq::socket_t mock_zmq(z_context, ZMQ_DEALER);

Fixture::Fixture()
{
	pm = new ServerPlayerMgr_instance();
	hall = new ServerHall_instance(pm);
	mockHandler = new MockMsgHandler(mock_zmq);

	aps = new PokerPersistenceService("rhappypoker");
	apsr = new PokerPersistenceServiceRedis("rhappypoker");
	apsm = new PokerPersistenceServiceMysql("rhappypoker");
	aps->_memStorage = apsr;
	aps->_diskStorage = apsm;

	drop_tables();
	create_tables();
}

Fixture::~Fixture()
{
	drop_tables();

	delete pm;
	delete hall;
	delete mockHandler;

	delete apsr;
	delete apsm;
	delete aps;
}

void Fixture::create_tables()
{
	std::string create_test_sql =
				"create database if not exists rhappypoker_test DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;";
	std::map<std::string, int> fileds;
	std::vector < std::map<std::string, std::string> > results;
	apsm->getDataBySql(create_test_sql, fileds, results);

	create_test_sql =
				std::string("create table if not exists holdem_tables") +
				std::string("(tid varchar(255) not null primary key, name varchar(20), total_seats int, occupied_seat_number int, ") +
				std::string(" holdem_profile_id int, creator_id int, status int, handler varchar(20), ") +
				std::string("holdem_club_id int, last_duration int, created_at datetime, insurance_active int, insurance_income int, total_games int, total_buyin int);");
	apsm->getDataBySql(create_test_sql, fileds, results);

	create_test_sql =
				std::string("create table if not exists holdem_profiles") +
				std::string("(id int not null primary key auto_increment, small_blind int, big_blind int, game_type int);");
	apsm->getDataBySql(create_test_sql, fileds, results);

	create_test_sql =
				std::string("create table if not exists users") +
				std::string("(id int not null primary key auto_increment, show_name varchar(255), coins int, avatar varchar(255));");
	apsm->getDataBySql(create_test_sql, fileds, results);

	create_test_sql =
				std::string("insert into holdem_profiles (small_blind, big_blind, game_type) values ") +
				std::string("(25, 50, 1);");
	apsm->getDataBySql(create_test_sql, fileds, results);

	int now = time(NULL);
	std::string ts;
	API_TimeToString(ts, now);

	create_test_sql =
				std::string("insert into holdem_tables (tid, name, total_seats, status, handler, holdem_profile_id, holdem_club_id, last_duration, created_at, insurance_active, insurance_income, total_games, total_buyin, occupied_seat_number) values ") +
				std::string("('tableone', 'tableone', 6, 1, 'hqueend', 1, 1, 360, '") +
				ts +
				std::string("', 1, 0, 0, 0, 0);");
//	std::cout <<  "sql=" << create_test_sql << std::endl;
	apsm->getDataBySql(create_test_sql, fileds, results);

	create_test_sql =
				std::string("insert into users (show_name, coins) values ") +
				std::string("('userone', 1000);");
	apsm->getDataBySql(create_test_sql, fileds, results);

	create_test_sql =
				std::string("create table if not exists table_users") +
				std::string("(id int not null primary key auto_increment, tid varchar(255), user_id int, chips_at_hand int, total_buyin int, total_income int, total_games int, inpot int, winning_games int);");
	apsm->getDataBySql(create_test_sql, fileds, results);
	create_test_sql =
				std::string("insert into table_users (tid, user_id, chips_at_hand, total_buyin, total_income, total_games, inpot, winning_games) values ") +
				std::string("('tableone', 1, 1000, 0, 0, 0, 0, 0);");
	apsm->getDataBySql(create_test_sql, fileds, results);

	create_test_sql =
				std::string("create table if not exists holdem_clubs") +
				std::string("(id int not null primary key auto_increment, total_games int);");
	apsm->getDataBySql(create_test_sql, fileds, results);
	create_test_sql =
				std::string("insert into holdem_clubs (total_games) values ") +
				std::string("(0);");
	apsm->getDataBySql(create_test_sql, fileds, results);

	create_test_sql =
				std::string("create table if not exists club_users") +
				std::string("(id int not null primary key auto_increment, user_id int, holdem_club_id int, total_income int, total_games int, inpot int, winning_games int);");
	apsm->getDataBySql(create_test_sql, fileds, results);
	create_test_sql =
				std::string("insert into club_users (holdem_club_id, user_id, total_income, total_games, inpot, winning_games) values ") +
				std::string("(1, 1, 0, 0, 0, 0);");
	apsm->getDataBySql(create_test_sql, fileds, results);
}

void Fixture::drop_tables()
{
	std::vector < std::map<std::string, std::string> > results;
	std::string drop_test_sql = "drop table if exists holdem_tables;";
	std::map<std::string, int> fileds;
	apsm->getDataBySql(drop_test_sql, fileds, results);

	drop_test_sql = "drop table if exists holdem_profiles;";
	apsm->getDataBySql(drop_test_sql, fileds, results);

	drop_test_sql = "drop table if exists users;";
	apsm->getDataBySql(drop_test_sql, fileds, results);

	drop_test_sql = "drop table if exists table_users;";
	apsm->getDataBySql(drop_test_sql, fileds, results);

	drop_test_sql = "drop table if exists club_users;";
	apsm->getDataBySql(drop_test_sql, fileds, results);

	drop_test_sql = "drop table if exists holdem_clubs;";
	apsm->getDataBySql(drop_test_sql, fileds, results);
}

LYTablePtr ServerHall_instance::load_new_table(const string &table_id)
{
	LYTable* table = new LYTable(table_id, 9, LYTableOpen, "first_table");
	table->started_at = time(NULL);
	table->last_duration = 360;
	LYTablePtr p = LYTablePtr(table);
	createTable(table_id, p);

	return p;
}

bool ServerHall_instance::isOpen(const std::string& table_id) //判断是否关闭桌子
{
	return true;
}

bool ServerHall_instance::isValidBuyin(const std::string& profile, unsigned int chipsAtHand, unsigned int buyin) //买入是否在允许范围内
{
	return true;
}

void ServerPlayerMgr_instance::load_user_from_db(const std::string& uid, const std::string &device_id, LYPlayer& player)
{
	player.balance = 1000;
	player._id = uid;
}
