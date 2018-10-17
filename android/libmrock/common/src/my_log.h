//
//
#ifndef MY_LOG_H_
#define MY_LOG_H_

#ifndef _MOBILE_

#include <sys/syscall.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <iomanip>
#include <log4cplus/logger.h>
#include <log4cplus/fileappender.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>
#include <log4cplus/ndc.h>

// just for eclipse auto format
#ifdef __CDT_PARSER__
    #undef BOOST_FOREACH
    #define BOOST_FOREACH(a, b) for(a; ; )
#endif

using namespace log4cplus;
using std::string;

#define LY_LOG_ERR(inf) LOG4CPLUS_ERROR(logger, inf)
#define LY_LOG_INF(inf) LOG4CPLUS_INFO(logger, inf)
#define LY_LOG_DBG(inf) LOG4CPLUS_DEBUG(logger, inf)

extern log4cplus::Logger logger;

const log4cplus::Logger getLogger (char *argv0);
void LogHexDump (Logger& log, std::string& str);

//public utility header
extern const string LY_MCAST;
extern const string LY_BCAST;
extern const string LY_FEED;

#else //_MOBILE_

#include <iostream>

#define LY_LOG_ERR(inf) std::cout<<inf<<std::endl
#define LY_LOG_INF(inf) std::cout<<inf<<std::endl
#define LY_LOG_DBG(inf) std::cout<<inf<<std::endl

#endif //_MOBILE_

#endif
