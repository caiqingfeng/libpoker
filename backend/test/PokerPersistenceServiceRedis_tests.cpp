/*
 * AppdPersistenceServiceRedis_tests.cpp
 *
 *  Created on: 2014-4-14
 *      Author: caiqingfeng
 */

#include "backend/src/PokerPersistenceServiceRedis.h"
#include "common/src/my_log.h"
#include "libpoker/poker/src/LYSeat.h"
#include <gtest/gtest.h>
#include "Fixture.h"
#include "backend/src/SerializeHelper.h"

class PokerPersistenceServiceRedis_tests: public ::testing::Test {
public:

	void SetUp()
	{
		aps = new PokerPersistenceServiceRedis("luyunpoker");
	}

	void TearDown()
	{
		delete aps;
	}

public:
	PokerPersistenceServiceRedis* aps;
};

TEST_F(PokerPersistenceServiceRedis_tests, init)
{
	ASSERT_EQ(aps->poker_collection, "luyunpoker_test");
}

TEST_F(PokerPersistenceServiceRedis_tests, enter_quit_table)
{
	LYTable* table = new LYTable("kkk", 6, LYTableOpen, "first table");
	table->owner = "fffff";
	table->_id = "kkkk";

	aps->enter_table(table->_id, "kkkk");
	std::vector<std::string> list;
	std::string table_players_collection = aps->poker_collection + ".tables." + table->_id + ".players" ;

	aps->retrieve_index_to_vector(table_players_collection, list);

	ASSERT_EQ(list.size(), 1);

	aps->leave_table(table->_id, "kkkk");
	list.clear();
	table_players_collection = aps->poker_collection + ".tables." + table->_id + ".players" ;

	aps->retrieve_index_to_vector(table_players_collection, list);
	ASSERT_EQ(list.size(), 0);
	delete table;
}

TEST_F(PokerPersistenceServiceRedis_tests, create_table)
{
	LYTable* table = new LYTable("kkk", 6, LYTableOpen, "first table");
	std::vector < std::pair<std::string, std::string> > key_value_pairs =
			table->table2kvps(); // SerializeHelper::table2kvp(table);
	std::string table_collection = std::string("rhappyholdem_test") + ".tables";
	aps->store_kvps_to_hash(table_collection, table->_id, key_value_pairs);
	delete table;
}

TEST_F(PokerPersistenceServiceRedis_tests, user_chips)
{
	aps->save_user_chips_to_mb("kkk", "1", 1, 1000);
	int r = aps->get_user_chips_from_mb("kkk", "1");
	ASSERT_EQ(r, 1000);

	aps->increase_user_chips_to_mb("kkk", "1", 1, 1000);
	r = aps->get_user_chips_from_mb("kkk", "1");
	ASSERT_EQ(r, 2000);

	aps->drop_user_from_mb("kkk", "1");
	r = aps->get_user_chips_from_mb("kkk", "1");
	ASSERT_EQ(r, 0);
}
