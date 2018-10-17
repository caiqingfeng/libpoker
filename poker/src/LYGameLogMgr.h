/*
 * LYGame.h
 *
 *  Created on: 2013-7-5
 *      Author: caiqingfeng
 */

#ifndef LYGAME_LOG_MGR_H_
#define LYGAME_LOG_MGR_H_

#include <memory>
#include <string>
#include <vector>

class LYGameLogMgr {
public:
	std::vector<std::string> logs;

public:
	LYGameLogMgr();
	virtual ~LYGameLogMgr();
	void reset();

public:
	void appendLog(const std::string& log);
	std::vector<std::string> getLogs();
};

#endif /* LYGAME_H_ */
