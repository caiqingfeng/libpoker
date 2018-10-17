/*
 * LYPotManager_test.cpp
 *
 *  Created on: 2013-3-25
 *      Author: caiqingfeng
 */

#include <gtest/gtest.h>
#include <boost/foreach.hpp>

#include "src/LYPot.h"
//#include "common/src/my_log.h"

#include <string>
using std::string;

class LYPot_tests : public ::testing::Test
{
protected:

    void SetUp()
    {
    }
    void TearDown()
    {
    }
public:
};

TEST_F(LYPot_tests, pot2kvps)
{
    LYPot pot(100);
    pot.addSeat(LYSeat1);
    pot.addSeat(LYSeat2);
    pot.addSeat(LYSeat3);

    pot.winnersInPot.push_back(LYSeat2);

    std::vector<std::pair<std::string, std::string> > kvps = pot.pot2kvps();
    std::vector<std::pair<std::string, std::string> >::iterator it = kvps.begin();
    for (; it!=kvps.end(); it++) {
        std::pair<std::string, std::string> kvp = *it;
//        std::cout << "kvp.first=" <<kvp.first << ", kvp.second=" << kvp.second << std::endl;
        if (kvp.first == "potLevel") {
            ASSERT_EQ(kvp.second, "1");
        }
        if (kvp.first == "potOpen") {
            ASSERT_EQ(kvp.second, "1");
        }
        if (kvp.first == "priceFixed") {
            ASSERT_EQ(kvp.second, "0");
        }
    }

    LYPot newPot(kvps);
    ASSERT_EQ(newPot.potLevel, 1);
    kvps = newPot.pot2kvps();
    it = kvps.begin();
    for (; it!=kvps.end(); it++) {
        std::pair<std::string, std::string> kvp = *it;
//        std::cout << "kvp.first=" <<kvp.first << ", kvp.second=" << kvp.second << std::endl;
        if (kvp.first == "potLevel") {
            ASSERT_EQ(kvp.second, "1");
        }
        if (kvp.first == "potOpen") {
            ASSERT_EQ(kvp.second, "1");
        }
        if (kvp.first == "priceFixed") {
            ASSERT_EQ(kvp.second, "0");
        }
    }
}

