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

class PokerPersistenceService_tests: public ::testing::Test {
public:

	void SetUp()
	{
		fixture = new Fixture();
		aps = fixture->aps;
		apsm = fixture->apsm;

//		getchar();
	}

	void TearDown()
	{
		delete fixture;
	}

public:
	PokerPersistenceService* aps;
	PokerPersistenceServiceMysql* apsm;
	Fixture* fixture;
};

TEST_F(PokerPersistenceService_tests, enter_table)
{
	aps->enter_table("tableone", "1");
//	getchar();
}

TEST_F(PokerPersistenceService_tests, leave_table)
{
	unsigned int chips = aps->leave_table("tableone", "1");
	ASSERT_EQ(chips, 1000);

	aps->enter_table("tableone", "1");
	chips = apsm->get_user_chips("tableone", "1");
	ASSERT_EQ(chips, 1000);

	apsm->save_user_chips("tableone", "1", 2000);
	chips = apsm->get_user_chips("tableone", "1");
	ASSERT_EQ(chips, 2000);
	chips = aps->leave_table("tableone", "1");
	ASSERT_EQ(chips, 1000);
	chips = apsm->get_user_chips("tableone", "1");
	ASSERT_EQ(chips, 1000);
}
