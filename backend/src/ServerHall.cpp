/*
 * ServerHandler.cpp
 *
 *  Created on: 2013-8-23
 *      Author: chenfeng
 */

#include "ServerHall.h"
#include "libmrock/common/src/my_log.h"
#include <iostream>

ServerHall::ServerHall(ServerPlayerMgr *pm) : LYHall(pm){
    // TODO Auto-generated constructor stub

}

ServerHall::~ServerHall() {
    // TODO Auto-generated destructor stub
}

void ServerHall::registerTimer(const string &table_id, const string &timer_id, int time_passed, int reserved1)
{
	std::map<std::string, LYPokerTimerPtr>::iterator it = timers.find(table_id);
	enum LYTimerAction cmd = com::luyun::timer::shared::LY_TIMER_REG;
	LYTimerPtr t;

	//设置after时间
	float after = LY_SHAKEHAND_TIMEOUT;
//	const std::vector<std::string>::iterator res = find(TIMER_ID_POLLING_vector.begin(), TIMER_ID_POLLING_vector.end(), timer_id);
	std::vector<std::string>::const_iterator res = find(TIMER_ID_POLLING_vector.begin(), TIMER_ID_POLLING_vector.end(), timer_id);
	if (res != TIMER_ID_POLLING_vector.end()) {
		if (reserved1 == 1) {
		//范特西,3分钟
			after = LY_FANTASY_TIMEOUT - time_passed;
		} else {
			after = LY_POLLING_TIMEOUT - time_passed;
		}
	} else {
		if (timer_id == TIMER_ID_REQ_NEW_GAME) {
			after = LY_DELAYED_ACTION - time_passed;
		} else if (timer_id == TIMER_ID_CLOSE_TABLE) {
			after = reserved1;
		} else if (timer_id == TIMER_ID_NEXT_ROUND) {
			after = LY_DELAYED_ACTION - time_passed;
		} else {
			res = find(TIMER_ID_RESERVE_SEAT_vector.begin(), TIMER_ID_RESERVE_SEAT_vector.end(), timer_id);
			if (res != TIMER_ID_RESERVE_SEAT_vector.end()) {
				after = LY_RESERVE_SEAT_TIMEOUT - time_passed;
			} else {
				res = find(TIMER_ID_ASSURE_SEAT_vector.begin(), TIMER_ID_ASSURE_SEAT_vector.end(), timer_id);
				if (res != TIMER_ID_RESERVE_SEAT_vector.end()) {
					after = LY_ASSURE_SEAT_TIMEOUT - time_passed;
				}
			}
		}
	}

	if (after < 0) {
		srand(time(NULL));
		after = 1.0 + rand()%5;
	}

	if (it == timers.end()) { // create a brand new timers for this table
		LYPokerTimerPtr ht = LYPokerTimerPtr(new LYPokerTimer());
		t = LYTimerPtr(new LYTimer());
		t->set_client_timer_id(table_id);
		t->set_second_client_timer_id(timer_id);
		t->set_after(after);
		ht->tableTimers[timer_id] = t;
		timers[table_id] = ht;
	} else {
		LYPokerTimerPtr ht = it->second;
		std::map<std::string, LYTimerPtr>::iterator it2 = ht->tableTimers.find(timer_id);
		if (it2 == ht->tableTimers.end()) { //create a new timer for this timer
			t = LYTimerPtr(new LYTimer());
			t->set_client_timer_id(table_id);
			t->set_second_client_timer_id(timer_id);
			t->set_after(after);
			ht->tableTimers[timer_id] = t;
		} else {
			t = it2->second;
			t->set_client_reserved1(reserved1);
			//如果两个人同时坐下来，第二个人触发的Timer将覆盖第一个人的，因此要RESET
			cmd = com::luyun::timer::shared::LY_TIMER_RESET;
			//如果上一个还未处理完，不注册新的定时器, 20160701
			return;
		}
	}
//	((AppdHandlerImpl*) (g_serverHandler->getHandlerImp()))->sendTimerMsg(t, cmd);
	sendTimerMsg(t, cmd);
}

void ServerHall::clearTimer(const string &table_id, const string &timer_id)
{
	std::map<std::string, LYPokerTimerPtr>::iterator it = timers.find(table_id);
	LYTimerPtr t;
	if (it == timers.end()) {
		LY_LOG_ERR("something wrong, table should exist " << table_id);
		return;
	}
	LYPokerTimerPtr ht = it->second;
	std::map<std::string, LYTimerPtr>::iterator it2 = ht->tableTimers.find(timer_id);
	if (it2 == ht->tableTimers.end()) {
		LY_LOG_ERR("something wrong, timer should exist " << timer_id);
		return;
	}
	t = it2->second;
	enum LYTimerAction cmd = com::luyun::timer::shared::LY_TIMER_CAN;
	sendTimerMsg(t, cmd);
	ht->tableTimers.erase(it2);
}

/*
 * 20160427 added，清除所有的定时器，但保留close_table定时器
 *
 */
void ServerHall::clearTimers(const string &table_id) //清理所有的定时器
{
	std::map<std::string, LYPokerTimerPtr>::iterator it = timers.find(table_id);
	LYTimerPtr t;
	if (it == timers.end()) {
		LY_LOG_ERR("something wrong, table should exist " << table_id);
		return;
	}
	LYPokerTimerPtr ht = it->second;
	ht->clearTimersExceptCloseTable();

	std::map<std::string, LYTimerPtr>::iterator it2 = ht->tableTimers.begin();
	for (; it2 != ht->tableTimers.end(); it2++) {
		LYTimerPtr t = it2->second;
		if (t->get_second_client_timer_id() == TIMER_ID_CLOSE_TABLE) continue;

		enum LYTimerAction cmd = com::luyun::timer::shared::LY_TIMER_CAN;
		sendTimerMsg(t, cmd);
	}
	ht->clearTimersExceptCloseTable();
	//don't erase
//	timers.erase();
}

LYPokerTimerPtr ServerHall::getTimer(const std::string &table_id)
{
	std::map<std::string, LYPokerTimerPtr>::iterator it = timers.find(table_id);
	LYPokerTimerPtr t;
	if (it == timers.end()) { // create a brand new timers for this table
		return t;
	}
	return it->second;
}

/*
 * called by interface->onTimeout
 * cl
 *
 */
void ServerHall::onTimeout(LYTimer &timer)
{
//LY_LOG_DBG("trigger timeout");
	std::string table_id = timer.get_client_timer_id();
	std::map<std::string, LYPokerTimerPtr>::iterator it = this->timers.find(table_id);
	LYTablePtr table = this->fetchTableById(table_id);
	LYTable* pokerTbl = table.get();
	if (it == timers.end() || NULL == pokerTbl) {
		LY_LOG_ERR("not found table with id " << table_id);
		return;
	}
	LYPokerTimerPtr ht = it->second;
	std::string server_id = timer.get_server_timer_id();
	std::string timer_id = timer.get_second_client_timer_id();
	std::map<std::string, LYTimerPtr>::iterator it2 = ht->tableTimers.find(timer_id);
	if (it2 == ht->tableTimers.end()) {
		LY_LOG_ERR("not found timer " << timer_id);
		return;
	}
	LYTimerPtr t = it2->second;
	if (timer.get_action() == com::luyun::timer::shared::LY_TIMER_CONF) {
		t->set_server_timer_id(server_id);
		return;
	}
	if (timer.get_action() != com::luyun::timer::shared::LY_TIMER_FIRE) {
		LY_LOG_ERR("timer_message from timerd should be confirmed or fired!");
		return;
	}
	if (timer_id == TIMER_ID_REQ_NEW_GAME) {
LY_LOG_DBG("trigger timeout event, new game");
		checkToStartGame(pokerTbl);
		return;
	} else 	if (timer_id == TIMER_ID_CLOSE_TABLE) {
		LY_LOG_DBG("receiving timer event to close table!");
        closeTable(pokerTbl);
        return;
	} else 	if (timer_id == TIMER_ID_NEXT_ROUND) {
        nextRound(pokerTbl);
        return;
    }

	enum LYApplicant seat = LYHouse;
	for(unsigned int i=0; i<table->getTotalSeats(); i++)
	{
		if (TIMER_ID_POLLING[i] == timer_id) {
			seat = (enum LYApplicant)(i+1);
			break;
		}
	}
	if (seat != LYHouse) {
		LYSeatPtr st = table->fetchSeat(seat);
		if (NULL == st.get()) {
			LY_LOG_ERR("wrong happend, not found seat with " << seat);
			return;
		}
		if (st->timeoutCount < 1) {
LY_LOG_DBG("trigger timeout event, seat=" << seat);
			this->seatOnPollingTimeout(table_id, seat);
		} else {
			this->leaveSeat(table_id, seat, st->playerId);
		}
		return;
	}

	/*
	 * 20150209新增，ReserveSeat Timers
	 */
	for(unsigned int i=0; i<table->getTotalSeats(); i++)
	{
		if (TIMER_ID_RESERVE_SEAT[i] == timer_id) {
			seat = (enum LYApplicant)(i+1);
			break;
		}
	}
	if (seat != LYHouse) {
		LYSeatPtr st = table->fetchSeat(seat);
		if (NULL == st.get()) {
			LY_LOG_ERR("something wrong happend, not found seat with " << seat);
			return;
		}
//		LYHoldemSeat* ps = (LYHoldemSeat *)st.get();
		//强制离开座位
		this->forceLeaveSeat(pokerTbl, st.get());
		return;
	}
	/*
	 * 20160321新增，保险定时器
	 */
	for(unsigned int i=0; i<table->getTotalSeats(); i++)
	{
		if (TIMER_ID_ASSURE_SEAT[i] == timer_id) {
			seat = (enum LYApplicant)(i+1);
			break;
		}
	}
	if (seat != LYHouse) {
		//无保险
		this->seatOnAssuranceTimeout(table_id, seat);
		return;
	}
		//shake hand timer
	/*
	 * 第一次收到该用户Timer的Timeout消息是为监控该用户启动定时器
	 * 接下来调用ShakeHand，其中再次注册该用户Timer
	 * 如果收到ShakeHand消息则CancelTimer，否则根据送回的Reserved1判断已经超时
	 * 为了最大限度重用原来的Register和Timeout代码，这一段有点难理解，特别是第二次注册时，要设置Reserved1
	 */
//	LY_LOG_DBG("shake hand with user " << timer_id);
	if (timer.get_client_reserved1() == 0) {
		this->shakeHand(table_id, timer_id);
	} else {
		this->leaveTable(table_id, timer_id);
	}
	return;
}

/*
 * 发下行ShakeHand消息请求
 */
void ServerHall::shakeHand(const string &table_id, const string &uid)
{
	LY_LOG_DBG("shanking hand with " << uid);
	this->registerTimer(table_id, uid, 0, 1);

	sendShakeHandMsg(table_id, uid);
}

/*
 * 接收shakeHand响应
 */
void ServerHall::onShakeHand(const string &table_id, const string &uid)
{
	LY_LOG_DBG("onShanking hand with " << uid);
	//继续注册定时服务器
	this->clearTimer(table_id, uid);
	this->registerTimer(table_id, uid, 0);
}

void ServerHall::leaveTable(const std::string &table_id, const std::string &uid)
{
	LYPlayer player = ((ServerPlayerMgr *)playerMgr)->getPlayerById(uid);
	LYTablePtr table = this->fetchTableById(table_id);
	if ("" == player._id || NULL == table.get()) {
		LY_LOG_ERR("user or table not exist, uid="<<string(uid) << " and tid=" << string(table_id));
		return;
	}
	table->leaveTable(uid);
	//20140130修改，每次用户的动作都写数据库
	((ServerPlayerMgr *)playerMgr)->leaveTable(player, table->_id);
//	player->position = "hall";
	this->clearTimer(table_id, uid);
	sendReplyLeaveTableMsg(table.get(), uid);
}

/*
 *取桌子列表是从Web中通过Rest接口，不是通过ZMQ
 *用户进入桌子的时候，有可能该桌子刚刚创建在Mysql中，并未加载到内存
 *首先需要加载该Table
 *用户如果是第一次进入该桌子，则需要注册定时器，定时握手
 *用户如果是断线重连，需要获取该桌子的Scene
 */
void ServerHall::enterTable(const std::string &table_id, const std::string &uid)
{
	LYPlayer player = ((ServerPlayerMgr *)playerMgr)->getPlayerById(uid);
	if (player._id == "") {
		LY_LOG_ERR("user not found  " << uid );
		return;
	}
	LYTablePtr table = this->fetchTableById(table_id);
	if (NULL == table.get()) {
		LY_LOG_ERR("table id not found in memory " << table_id << " , loading table from db");
		table = load_new_table(table_id);
		if (table.get() == NULL) {
			LY_LOG_ERR("not found table in mysql " << table_id );
			return;
		}
		LYPokerProfile *prof = getProfileById(table->profile_id);
		if (prof == NULL) {
			LY_LOG_ERR("profile should not be null, id=" << table->profile_id);
			return;
		}
		table->openTrunk("last_trunk", "trunk_name", "player01", prof);
		table->status = LYTableOpen;
		int time_left = table->getTimeLeft();
		if (time_left <= 0) {
			LY_LOG_ERR("table closed already!!!!" << time_left);
			return;
		}
		LY_LOG_DBG("register timer for this table " << time_left);
		registerTimer(table_id, TIMER_ID_CLOSE_TABLE, 0, time_left);
	}
	int chips_at_hand = 0;
	if (table->havePlayer(uid)) {//如果用户已经在桌子里，则不从mysql取其chips_at_hand，此用户必须在redis中有记录
		bool reentry = true;
		((ServerPlayerMgr *)playerMgr)->enterTable(player, table->_id, chips_at_hand, reentry);
		sendReplyEnterTableMsg(table.get(), uid, chips_at_hand);
		return;
	}

	if (!allowUser(table, uid)) {
		LY_LOG_ERR("not allowed for this user " << table_id << ", " << uid);
		return;
	}

	table->enterTable(uid);
	//	player->position = table->_id;
	//20140126修改，每次用户的动作都写数据库
	((ServerPlayerMgr *)playerMgr)->enterTable(player, table->_id, chips_at_hand);

	//注册定时器，到时间如果没有收到客户的shake_hand响应，则踢出该用户
	this->registerTimer(table_id, uid, 0);
	sendReplyEnterTableMsg(table.get(), uid, chips_at_hand);
}

/*
 * Player申请坐下
 */
void ServerHall::takeSeat(const std::string &table_id, enum LYApplicant seat_no,
							const std::string &uid, unsigned int buyin)
{
	if (seat_no > 9 || seat_no < 1) {
		LY_LOG_ERR("take seat failed! wrong seat no " << seat_no);
		return;
	}
	LYPlayer player = ((ServerPlayerMgr *)playerMgr)->getPlayerById(uid);
	LYTablePtr table = this->fetchTableById(table_id);
	if ("" == player._id || NULL == table.get() || !table->havePlayer(uid)) {
		LY_LOG_ERR("user or table not exist, or user must  have been in table first! uid="<<string(uid) << " and tid=" << string(table_id));
		//no message return!
		return;
	}

	if (!isOpen(table_id)) {
		LY_LOG_ERR("table not exist, closed or shutting down:" << string(table_id));
		//return failure message, modified 20131216
//		handler->reply_take_seat(table_id, seat_no, buyin, uid, table->getAllPlayers(), false);
		return;
	}

	LYSeatPtr prevSeat = table->fetchSeatByPlayerId(uid);
	if (NULL != prevSeat.get()) {
		LY_LOG_ERR("user seated already! uid="<<string(uid) << " and tid=" << string(table_id));
		//return failure message, modified 20131216
//		handler->reply_take_seat(table_id, seat_no, buyin, uid, table->getAllPlayers(), false);
		return;
	}

	if (!canBuyinFromBank(table, player, buyin)) { //player->balance < buyin) {
		LY_LOG_ERR("not enough balance " << player.balance << ", buyin:" << buyin);
		return;
	}

	LYSeatPtr seat = table->fetchSeat(seat_no);
	if (NULL == seat.get() || LYSeatOpen != seat->status) {
//		std::cout << "seat must be open, uid="<<string(uid) << " and tid=" << string(table_id) << " and sea_no= " << seat_no;
		LY_LOG_ERR("seat must be open, uid="<<string(uid) << " and tid=" << string(table_id) << " and sea_no= " << seat_no);
		return;
	}

	int chips = getUserChips(table_id, uid);
	bool reentry = false;
	int buyin_result = buyin;
	if (chips > 0) {
		if (chips != buyin) {
			LY_LOG_ERR("user re-entry table and take seat with different chips:" << chips << ", buyin:" << buyin);
			return;
		}
		reentry = true;
	} else {
		string profile = table->getProfile();
		if (!this->isValidBuyin(profile, seat->chipsAtHand, buyin)) {
	//		std::cout << "buyin exceeds range allowed" << player.balance << ", buyin:" << buyin << std::endl;
			LY_LOG_ERR("buyin exceeds range allowed, balance:" << player.balance << ", buyin:" << buyin);
			return;
		}
		buyin_result = buyin + chips; //chips可能为负，此处需要扣减掉
	}

	table->takeSeat(seat_no, buyin_result, uid);
//	if (!reentry) {
//	//扣减余额
//		updateBalance(table, player, -buyin);
//	}
	//set seat/player chips_at_hand, status for the memory DB
	((ServerPlayerMgr *)playerMgr)->buyin(table_id, uid, seat_no, buyin, reentry);

	changeOccupiedSeats(table_id, table->occupied_seat_number);
	saveGame(table.get());
	//清除掉Standby定时器
	this->clearTimer(table_id, uid);
	LYGame *game = (LYGame *)table->getCurrentGame();
	if (game == NULL || game->readyForNext()) {
			//game->round == LYGameClosed || game->round == LYGameInit || game->round == LYGameWaiting) {
		if (table->getSeatedPlayers() == 2) {
		LY_LOG_DBG("registering a new game timer... ");
			this->clearTimer(table_id, TIMER_ID_REQ_NEW_GAME);
			this->registerTimer(table_id, TIMER_ID_REQ_NEW_GAME, 0);
		}
	} else {
		LY_LOG_DBG("after clearTimer, a game is ongoing");
	}
	sendReplyTakeSeatMsg(table_id, seat_no, buyin, uid, table->getAllPlayers());
	return;
}

/*
 * 一般是余额超过Buyin即可
 * 但可以被重载，由Bank来控制，例如不能超过6个Buyin
 */
bool ServerHall::canBuyinFromBank(LYTablePtr table, LYPlayer& player, unsigned int buyin)
{
	return player.balance >= buyin;
}

/*
 * 扣减掉用户的balance
 * 但可以被重载，例如累加某个桌子上的Buyin
 * discarded 20160701
 */
void ServerHall::updateBalance(LYTablePtr table, LYPlayer& player, unsigned int seat_no, int buyin)
{
	player.balance = player.balance - buyin;
	int delta = -buyin;
	((ServerPlayerMgr *)playerMgr)->updateBalance(player._id, "takeseat", delta);
	//added 20160701
}

/*
 * Player申请买入，前提是已经坐在该位子上
 * 在游戏中不能买入，只能是Fold或者GameWaiting状态可以买入
 * 客户端在游戏结束时，如果自己的筹码为0，即触发本条消息
 * 或者用户可以在游戏结束时买入
 */
void ServerHall::applyBuyin(const std::string &table_id, enum LYApplicant seat_no,
							const std::string &uid, unsigned int buyin)
{
	if (seat_no > 9 || seat_no < 1) {
		LY_LOG_ERR("apply buyin failed! wrong seat no " << seat_no);
		return;
	}
	LYPlayer player = ((ServerPlayerMgr *)playerMgr)->getPlayerById(uid);
	LYTablePtr table = this->fetchTableById(table_id);
	if ("" == player._id || NULL == table.get() || !table->havePlayer(uid)) {
		LY_LOG_ERR("user or table not exist, or user must  have been in table first! uid="<<string(uid) << " and tid=" << string(table_id));
		//no message return!
		return;
	}

	if (!canBuyinFromBank(table, player, buyin)) { //player->balance < buyin) {
//	if (player->balance < buyin) {
		LY_LOG_ERR("not enough balance " << player.balance << ", buyin:" << buyin);
		//return failure message, modified 20131216
//		handler->reply_buyin(table_id, seat_no, buyin, uid, table->getAllPlayers(), false);
		return;
	}

	LYSeatPtr seat = table->fetchSeat(seat_no);
	if (NULL == seat.get() ||
		(LYSeatOpen != seat->status && seat->playerId != uid)) {
		LY_LOG_ERR("seat must be taken by the same user, uid="<<string(uid) << " and tid=" << string(table_id)
				<< ", sea_no= " << seat_no << " and taken by " << seat->playerId);
		//return failure message, modified 20131216
//		handler->reply_buyin(table_id, seat_no, buyin, uid, table->getAllPlayers(), false);
		return;
	}

	//seat->status == LYSeatOpen || seat->playerId == uid
	if (seat->status == LYSeatOpen) { //take seat
		this->takeSeat(table_id, seat_no, uid, buyin);
		return;
	}

	string profile = table->getProfile();
	if (!isValidBuyin(profile, seat->chipsAtHand, buyin)) {
		LY_LOG_ERR("buyin not allowed " << player.balance << ", buyin:" << buyin);
		//return failure message, modified 20131216
//		handler->reply_buyin(table_id, seat_no, buyin, uid, table->getAllPlayers(), false);
		return;
	}

	table->buyin(seat_no, buyin, uid);
//	updateBalance(table, player, -buyin); //discarded!!! 20160701
	((ServerPlayerMgr *)playerMgr)->buyin(table_id, uid, seat_no, buyin, false);

	saveGame(table.get());

	sendReplyBuyinMsg(table_id, seat_no, seat->newBuyin, seat->chipsAtHand, uid, table->getAllPlayers());
	return;
}

/*
 * 首先fold，然后注册一个SeatChanged定时器
 */
void ServerHall::leaveSeat(const std::string &table_id, enum LYApplicant seat_no, const std::string &uid,
		int cash_out_request)
{
	// if timeout twice, player will be forced to leave
	//this function will be called as uid=seat->playerId;
	//it will be set to ""
	//marked 20130816
	std::string uid_backup = uid; //虽然在本函数中没有修改uid，但在Table->LeaveSeat中将seat->playerId置成空，导致reply_leave_seat传递的参数UID为空
	LYPlayer player = ((ServerPlayerMgr *)playerMgr)->getPlayerById(uid);
	LY_LOG_DBG("leaving uid="<<uid << " and tid=" << string(table_id) << "seat_no =" << seat_no);
	LYTablePtr table = this->fetchTableById(table_id);
	if (NULL == table.get() || "" == player._id) {
		LY_LOG_ERR("user or table not exist, uid="<<string(uid) << " and tid=" << string(table_id));
//		std::cout << "user or table not exist, uid="<<string(uid) << " and tid=" << string(table_id) << std::endl;
		return;
	}

	LYSeatPtr seat = table->fetchSeat(seat_no);
	if (NULL == seat.get() || uid != seat->playerId) {
		LY_LOG_ERR("user not seated at uid="<<string(uid) << " and seat=" << std::to_string(seat_no));
//		std::cout << "user not seated at uid="<<string(uid) << " and seat=" << std::to_string(seat_no) << std::endl;
		return;
	}
	LYTable *pokerTable = (LYTable *)table.get();
	LYSeat *pokerSeat = (LYSeat *)seat.get();
//	std::cout << "fold ?" << std::endl;
	if (!pokerTable->isGameOver() && NULL != seat.get()) {
		if (!pokerSeat->allowLeave()) {
			return;
		}
		if (pokerSeat->isInGame()) {
			this->foldBeforeLeave(pokerTable, pokerSeat);
		}
	}

	int cash_out_approved = cash_out_request;
	table->leaveSeat(seat_no, cash_out_approved, uid);
	updateBalance(table, player, seat_no, (int)cash_out_approved);
	changeOccupiedSeats(table_id, table->occupied_seat_number);
	saveGame(pokerTable);
	//that user will still in table
	//加上Standby定时器
//	LY_LOG_DBG(string(uid) << " leaving seat " << seat_no << " of table " << table_id);
	//Marked 20140322此处uid可能已经被修改成空！因此必须要用uid_backup
	this->registerTimer(table_id, uid_backup, 0);
	LYGame *game = (LYGame *)table->getCurrentGame();
	if (game == NULL || game->readyForNext()) {
			//game->round == LYGameClosed || game->round == LYGameInit || game->round == LYGameWaiting) {
		if (table->getSeatedPlayers() == 2) {
			this->registerTimer(table_id, TIMER_ID_REQ_NEW_GAME, 0);
		}
	}
	sendReplyLeaveSeatMsg(table_id, seat_no, cash_out_approved, uid_backup, table->getAllPlayers());
}

void ServerHall::kickoutPlayer(LYTable* table, LYSeat* seat)
{
	seat->status = LYSeatReserved;
	sendKickoutPlayerMsg(table->_id,
			seat->seatNo, seat->playerId);
	registerTimer(table->_id, TIMER_ID_RESERVE_SEAT[(int)seat->seatNo - 1], 0);
}

bool ServerHall::allowUser(LYTablePtr table, const std::string& uid)
{
	return true;
}

/*
 * 根据seat->gameStatus
 * 用定时器监控
 * 或者直接生成一个BlindAction
 */
void ServerHall::monitorPlayer(LYTable* table, unsigned int seat,
				LYHoldemRoundContinue roundCont) //added 20160321 for assurance
{
	switch (roundCont) {
	case LYHoldemRoundDown:
	case LYHoldemRoundEnd:
		return;
	case LYHoldemRoundNext:
	case LYHoldemRoundCont:
	default:
		break;
	}
	registerTimer(table->_id, TIMER_ID_POLLING[(int)seat - 1], 0);
}

void ServerHall::seatOnPollingTimeout(const std::string& table_id, LYApplicant seat)
{
}

void ServerHall::seatOnAssuranceTimeout(const std::string& table_id, LYApplicant seat)
{
}