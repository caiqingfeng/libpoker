#include "LYGameLogMgr.h"

LYGameLogMgr::LYGameLogMgr()
{
    reset();
}

LYGameLogMgr::~LYGameLogMgr()
{
}

void LYGameLogMgr::reset()
{
    logs.clear();
}

void LYGameLogMgr::appendLog(const std::string& log)
{
    logs.push_back(log);
}

std::vector<std::string> LYGameLogMgr::getLogs()
{
    return logs;
}
