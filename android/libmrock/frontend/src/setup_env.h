/*
 * setup_env.h
 * 只能被MsgAdapter Include or test/main.cpp
 *
 *  Created on: 2014-4-10
 *      Author: caiqingfeng
 */

#ifndef SETUP_ENV_H_
#define SETUP_ENV_H_

#include "common/src/zhelpers.hpp"
#include "common/src/my_log.h"
#include "ClientHandler.h"

#ifndef _MOBILE_
log4cplus::Logger logger;
#endif

#endif
