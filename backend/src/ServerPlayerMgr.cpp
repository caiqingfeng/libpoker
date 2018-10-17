/*
 * ServerPlayerMgr.cpp
 *
 *  Created on: 2013-7-2
 *      Author: caiqingfeng
 */

#include "libmrock/common/src/zhelpers.hpp"
#include "ServerPlayerMgr.h"
#include "libmrock/common/src/my_log.h"

ServerPlayerMgr::ServerPlayerMgr() {
	// TODO Auto-generated constructor stub
	onlineUserCount = 0;
}

ServerPlayerMgr::~ServerPlayerMgr() {
	// TODO Auto-generated destructor stub
}

/*
 * 永远都触发从数据库中获取最新数据
 * 因此无需在内存中保存Map
 * 20160413 Mark: 未来可能需要优化，缓存一部分数据
 */
LYPlayer ServerPlayerMgr::getPlayerById(const std::string &uid)
{
	LY_LOG_DBG("enter ServerPlayerMgr::fetchPlayer");
	LYPlayer player(uid, "default_device");
	if (uid != "") {
		this->load_user_from_db(uid, "default_device", player);
	}
	return player;
}

void ServerPlayerMgr::updateBalance(const std::string& uid, const std::string& game, int& delta)
{
	this->update_balance_to_db(uid, game, delta);
}

//void ServerPlayerMgr::updatePosition(LYPlayerPtr player)
//discarded 20160701
void ServerPlayerMgr::updatePosition(LYPlayer& player, const std::string& tid, int& chips_at_hand, bool enter_table)
{
	this->update_position_to_db(player, tid, chips_at_hand, enter_table);
}

/*
 * 20140126新增，用户每次进入离开桌子要持久化
 * 20160315 如果用户离开桌子，其手上的筹码要保留。
 */
//void ServerPlayerMgr::enterTable(LYPlayerPtr player, const string& tid)
void ServerPlayerMgr::enterTable(LYPlayer& player, const string& tid, int& chips_at_hand, bool reentry)
{
//	if (NULL == player.get()) {
//		LY_LOG_ERR("player should be not null");
//		return;
//	}

//	player->position = tid;
//	player->enterTable(tid);
	player.enterTable(tid);
//	this->updatePosition(player, tid, chips_at_hand, true);
	if (reentry) {
		chips_at_hand = this->get_user_chips_from_mb(tid, player._id);	 // from redis
	} else {
		chips_at_hand = this->get_user_chips_from_db(tid, player._id); //from table_users
		this->save_user_chips_to_mb(tid, player._id, 0, chips_at_hand);
	}
}

/*
 * 20140130新增，用户每次进入离开桌子要持久化
 */
//void ServerPlayerMgr::leaveTable(LYPlayerPtr player, const string& tid)
void ServerPlayerMgr::leaveTable(LYPlayer& player, const string& tid)
{
//	if (NULL == player.get()) {
//		LY_LOG_ERR("player should be not null");
//		return;
//	}

//	player->position = "hall";
//	player->leaveTable(tid);
	player.leaveTable(tid);
//	int chips_at_hand;
//	this->updatePosition(player, tid, chips_at_hand, false);
	this->drop_user_from_mb(tid, player._id);
}

//added 20160701
//用户坐下时，如果是新的buyin，则在table_users表中记录，并且登记到redis中
void ServerPlayerMgr::buyin(const std::string& table_id, const std::string& uid,
					unsigned int seat_no, unsigned int chips, bool reentry)
{
	this->increase_user_chips_to_mb(table_id, uid, seat_no, chips);
	if (reentry) {
		return;
	}

	this->increase_user_chips_buyin_to_db(table_id, uid, chips);
}

//void ServerPlayerMgr::leaveSeat(LYTable *table, const std::string& uid, unsigned int seat_no, int chips)
//{
//}
