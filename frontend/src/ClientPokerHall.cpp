/*
 * ClientPokerHall.cpp
 *
 *  Created on: 2016-3-8
 *      Author: caiqingfeng
 */

#include "ClientPokerHall.h"
#include "libmrock/common/src/my_log.h"
#include <iostream>

ClientPokerHall::ClientPokerHall(LYPlayerMgr *pm) : LYHall(pm){
    // TODO Auto-generated constructor stub

}

ClientPokerHall::~ClientPokerHall() {
    // TODO Auto-generated destructor stub
}

void ClientPokerHall::leaveTable(const std::string &table_id, const std::string &uid)
{
	LYTablePtr table = this->fetchTableById(table_id);
	if (NULL == table.get()) {
		LY_LOG_ERR("user or table not exist, uid="<<string(uid) << " and tid=" << string(table_id));
		return;
	}
	table->leaveTable(uid);
}

void ClientPokerHall::enterTable(const std::string &table_id, const std::string &uid)
{
	LYTablePtr table = this->findOrCreateTableById(table_id);
	if (NULL == table.get()) {
		LY_LOG_ERR("not found table " << table_id );
		return;
	}
	if (table->havePlayer(uid)) {
		return;
	}
	table->enterTable(uid);
}
/*
 * Player申请坐下
 */
void ClientPokerHall::takeSeat(const std::string &table_id, enum LYApplicant seat_no,
							const std::string &uid, unsigned int buyin)
{
	if (seat_no > 9 || seat_no < 1) {
		LY_LOG_ERR("take seat failed! wrong seat no " << seat_no);
		return;
	}
	LYTablePtr table = this->fetchTableById(table_id);
	if (NULL == table.get() || !table->havePlayer(uid)) {
		LY_LOG_ERR("user or table not exist, or user must  have been in table first! uid="<<string(uid) << " and tid=" << string(table_id));
		//no message return!
		return;
	}

	LYSeatPtr prevSeat = table->fetchSeatByPlayerId(uid);
	if (NULL != prevSeat.get()) {
		LY_LOG_ERR("user seated already! uid="<<string(uid) << " and tid=" << string(table_id));
		return;
	}

	LYSeatPtr seat = table->fetchSeat(seat_no);
	if (NULL == seat.get() || LYSeatOpen != seat->status) {
		LY_LOG_ERR("seat must be open, uid="<<string(uid) << " and tid=" << string(table_id) << " and sea_no= " << seat_no);
		//return failure message, modified 20131216
//		handler->reply_take_seat(table_id, seat_no, buyin, uid, table->getAllPlayers(), false);
		return;
	}

	table->takeSeat(seat_no, buyin, uid);
}

/*
 * Player申请买入，前提是已经坐在该位子上
 * 在游戏中不能买入，只能是Fold或者GameWaiting状态可以买入
 * 客户端在游戏结束时，如果自己的筹码为0，即触发本条消息
 * 或者用户可以在游戏结束时买入
 */
void ClientPokerHall::buyin(const std::string &table_id, enum LYApplicant seat_no,
							const std::string &uid, unsigned int buyin)
{
	if (seat_no > 9 || seat_no < 1) {
		LY_LOG_ERR("apply buyin failed! wrong seat no " << seat_no);
		return;
	}
	LYTablePtr table = this->fetchTableById(table_id);
	if (NULL == table.get() || !table->havePlayer(uid)) {
		LY_LOG_ERR("user or table not exist, or user must  have been in table first! uid="<<string(uid) << " and tid=" << string(table_id));
		//no message return!
		return;
	}

	LYSeatPtr seat = table->fetchSeat(seat_no);
	if (NULL == seat.get() ||
		(LYSeatOpen != seat->status && seat->playerId != uid)) {
		LY_LOG_ERR("seat must be taken by the same user, uid="<<string(uid) << " and tid=" << string(table_id)
				<< ", sea_no= " << seat_no << " and taken by " << seat->playerId);
		return;
	}

	//seat->status == LYSeatOpen || seat->playerId == uid
	if (seat->status == LYSeatOpen) { //take seat
		this->takeSeat(table_id, seat_no, uid, buyin);
		return;
	}

	table->buyin(seat_no, buyin, uid);
	return;
}

/*
 * 首先fold，然后注册一个SeatChanged定时器
 */
void ClientPokerHall::leaveSeat(const std::string &table_id, enum LYApplicant seat_no, const std::string &uid,
		int cash_out_request)
{
	std::string uid_backup = uid; //虽然在本函数中没有修改uid，但在Table->LeaveSeat中将seat->playerId置成空，导致reply_leave_seat传递的参数UID为空
	LY_LOG_DBG("leaving uid="<<uid << " and tid=" << string(table_id) << "seat_no =" << seat_no);
	LYTablePtr table = this->fetchTableById(table_id);
	if (NULL == table.get()) {
		LY_LOG_ERR("user or table not exist, uid="<<string(uid) << " and tid=" << string(table_id));
		return;
	}

	int cash_out_approved = cash_out_request;
	table->leaveSeat(seat_no, cash_out_approved, uid);
}

