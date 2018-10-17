/*
 * AppdHandlerImpl_tests.cpp
 *
 *  Created on: 2014-4-14
 *      Author: caiqingfeng
 */

#include "backend/src/PokerHandlerImpl.h"
#include "common/src/my_log.h"
#include "Fixture.h"
#include "MessageStubs.h"
#include "backend/src/ServerHandler.h"

#include <gtest/gtest.h>

//extern zmq::context_t context;
////extern zmq::socket_t g_socketLocalSvr;
////extern zmq::socket_t g_socketLocalClt;
//extern zmq::socket_t g_socketRemote;
//extern zmq::socket_t g_socketTimer;
//extern zmq::socket_t g_socketJson;
//extern com::luyun::handler::server::ServerHandler *g_serverHandler;
//
class PokerHandlerImpl_tests : public ::testing::Test{
public:

    void SetUp()
    {
    	fixture = new Fixture();
    	mockHandler = fixture->mockHandler;
    }

    void TearDown()
    {
        delete fixture;
    }

public:
    Fixture* fixture;
    MockMsgHandler* mockHandler;
};

TEST_F(PokerHandlerImpl_tests, sendTimer)
{
    LYTimerPtr t(new LYTimer());
    mockHandler->sendTimerMsg(t, com::luyun::timer::shared::LY_TIMER_REG);
}

