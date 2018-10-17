/*
 * ServerHandler.h
 *
 *  Created on: 2015-12-23
 *      Author: caiqingfeng
 */

#ifndef CLIENT_POKER_HALL_H_
#define CLIENT_POKER_HALL_H_

#include "poker/src/LYHall.h"

class ClientPokerHall: public LYHall {
public:
	ClientPokerHall(LYPlayerMgr *pm);
	virtual ~ClientPokerHall();

////
public:
    virtual void enterTable(const std::string& t, const std::string& uid);
//    virtual void leaveTable(LYTable* table, const std::string& uid);
    virtual void leaveTable(const std::string& t, const std::string& uid);
    virtual void takeSeat(const std::string& t, enum LYApplicant seat_no,
                  			const std::string& uid, unsigned int buyin);
    virtual void leaveSeat(const std::string& t, enum LYApplicant seat_no,
                  			const std::string& uid, int cashout=0);
	virtual void buyin(const std::string &table_id, enum LYApplicant seat_no,
							const std::string &uid, unsigned int buyin);
    virtual LYTablePtr findOrCreateTableById(const std::string& table_id) = 0;
};

#endif /* SERVERHANDLER_H_ */
