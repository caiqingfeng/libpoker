/*
 * LYTable.cpp
 *
 *  Created on: 2013-7-5
 *      Author: caiqingfeng
 */
#include "LYTable.h"
//#include <boost/foreach.hpp>
//#include "common/src/my_log.h"
#include "libmrock/common/src/LYUtils.h"

LYTable::LYTable(const std::string &tid, unsigned int ts, enum LYTableStatus sta,
		const std::string &nm, std::string creator)
{
	_id = tid;
	status = sta;
	name = nm;
	total_seats = ts;
	owner = creator;
	game_round = 0;
	assurance_flag = 0;
	assurance_income = 0;
    for (unsigned int i = 0; i < total_seats; i++)
    {
        LYSeat *seat = new LYSeat((enum LYApplicant) (i + 1));
        LYSeatPtr sp(seat);
        seats.push_back(sp);
    }
    trunk = NULL;
    last_duration = 0;
    started_at = 0;
    total_buyin = 0;
    occupied_seat_number = 0;
//    last_polling_at = 0;
}

LYTable::LYTable()
{
	status = LYTableOpen;
	total_seats = 0;
	game_round = 0;
	trunk = NULL;
	assurance_flag = 0;
	assurance_income = 0;
    last_duration = 0;
    started_at = 0;
    total_buyin = 0;
    occupied_seat_number = 0;
//    last_polling_at = 0;
}

LYTable::~LYTable()
{
}

const std::string& LYTable::getOwner()
{
	return owner;
}

void LYTable::setOwner(const std::string &uid)
{
	owner = uid;
}

void LYTable::setServerHandler(const std::string& hdl)
{
	server_handler = hdl;
}

//Trunk管理
/*
 * 创建一个新Trunk，如果当前Trunk的状态是正在运行，则返回
 */
void LYTable::openTrunk(const std::string &trunk_id, const std::string &trunk_name,
		const std::string &player, const std::string &prof_id)
{

}

/*
 * 创建一个新Trunk，如果当前Trunk的状态是正在运行，则返回
 */
void LYTable::openTrunk(const std::string &trunk_id)
{

}

void LYTable::openTrunk(const std::string &trunk_id, const std::string &trunk_name,
			const std::string &player, LYPokerProfile *prof)
{
}

/*
 * 创建一个新Trunk，如果当前Trunk的状态是正在运行，则返回
 */
void LYTable::openTrunk(LYTrunk *trk)
{
	trunk = trk;
	//set profile of trunk
	trunk->activeProfile();
}

/*
 * 关闭current_trunk，如果当前Trunk还有一个Game在运行，则设置Trunk状态为closing
 * NextGame时将会检查Trunk状态，是否为准备关闭或者已经关闭状态
 * 现阶段假设Trunk会一直运行，跟桌子的生命周期完全一样
 */
void LYTable::closeTrunk()
{
//	LY_LOG_DBG("closeTrunk not implemented yet!");
	delete trunk;
	trunk = NULL;
}

bool LYTable::isTableOpen()
{
	int now = time(NULL);
	if (last_duration > 0 && now > (started_at+last_duration*60)) {
		return false;
	}

	return (this->status == LYTableOpen || this->status == LYTableInGame);
}

int LYTable::getTimeLeft()
{
	int now = time(NULL);
	return (started_at+last_duration*60) - now;
}

//-----------玩家动作 begin
void LYTable::enterTable(const std::string &uid)
{
//    std::cout << "player join table, uid=" << uid << std::endl;
    std::vector<std::string>::iterator it = players.begin();
    for (; it!=players.end(); it++) {
//    std::cout << "111111player join table, uid=" << *it << std::endl;
    	if (*it == uid) {
    		return;
    	}
    }

    players.push_back(uid);
//    this->addStandby(player);
}

void LYTable::leaveTable(const std::string &uid)
{
//    LY_LOG_DBG("player leave table, uid=" << uid);
    std::vector<std::string>::iterator it = players.begin();
    for (; it != players.end(); it++) {
    	if (*it == uid) {
    		players.erase(it);
    		break;
    	}
    }
}

/*
 * 20150209修改，增加一个状态SeatReserved
 */
void LYTable::takeSeat(enum LYApplicant seat_no, unsigned int buyin, const std::string &uid) //有人坐在某个位子上
{
	LYSeatPtr seat = fetchSeat(seat_no);
	if (NULL == seat.get()) return;

	bool can_take_this_seat = false;
	switch (seat->status)
	{
	case LYSeatOccupied:
	case LYSeatInGame:
		return;
	case LYSeatOpen:
		can_take_this_seat = true;
		break;
	case LYSeatReserved:
		if (seat->playerId == uid) can_take_this_seat = true;
		break;
	default:
		break;
	}

	if (can_take_this_seat) {
		seat->occupy(buyin, uid);
		occupied_seat_number ++;
	}

	total_buyin += buyin;
    return;
}

void LYTable::buyin(enum LYApplicant seat_no, unsigned int buyin, const std::string &uid) //有人坐在某个位子上
{
	LYSeatPtr seat = fetchSeat(seat_no);
	if (NULL == seat.get() || seat->getStatus() == LYSeatOpen) {
//		LY_LOG_ERR("status should be not occupied first! " << seat_no);
		return;
	}
	seat->buyin(buyin, uid);
	total_buyin += buyin;
    return;
}

void LYTable::leaveSeat(enum LYApplicant seat_no, int &cash_out_req, const std::string &uid) //有人坐在某个位子上
{
	LYSeatPtr seat = fetchSeat(seat_no);
//	if (!this->isGameOver() && (NULL == seat.get() || seat->isInGame())) {
	if (NULL == seat.get()) {
//		LY_LOG_ERR("cannot leave an empty seat or a seat in game! " << seat_no);
		return;
	}
    cash_out_req = seat->getChipsAtHand();
    seat->reset();
    occupied_seat_number --;
    return;
}

void LYTable::leaveSeatButReserve(enum LYApplicant seat_no)
{
	LYSeatPtr seat = fetchSeat(seat_no);
	if (!this->isGameOver() && (NULL == seat.get() || seat->isInGame())) {
//		LY_LOG_ERR("cannot leave an empty seat or a seat in game! " << seat_no);
		return;
	}
    seat->reset();
    seat->status = LYSeatReserved;
    occupied_seat_number --;
    return;

}

void LYTable::forceLeaveSeat(enum LYApplicant seat_no)
{
	LYSeatPtr seat = fetchSeat(seat_no);
    seat->reset();
    if (occupied_seat_number > 0) {
    	occupied_seat_number --;
    }
    return;

}

//-----------玩家动作 end

std::vector<std::string> LYTable::getNeighbors(const std::string &uid)
{
	std::vector<std::string> neighbors;
	if (!this->playerInTable(uid)) {
		return neighbors;
	}
	std::vector<std::string>::iterator it = players.begin();
	for (; it != players.end(); it++) {
		if (*it != uid) {
			neighbors.push_back(*it);
		}
	}
	return neighbors;
}

bool LYTable::playerInTable(const std::string &uid)
{
	std::vector<std::string>::iterator it = players.begin();
	for (; it != players.end(); it++) {
		if (*it == uid) {
			return true;
		}
	}
	return false;
}
std::vector<std::string> LYTable::getAllPlayers()
{
	return players;
}

std::vector<LYSeatPtr> LYTable::getAllSeats()
{
    return seats;
}

LYSeatPtr LYTable::fetchSeat(enum LYApplicant player)
{
	//caiqingfeng 20130418, 不能调用Game的接口，因为需要在没有Game的时候调用
//    if (NULL == dealer.get() || NULL == dealer->scene.get())
//        return LYSeatPtr();
//    return dealer->scene->fetchSeat(player);
	if ((unsigned int)player < 1 || (unsigned int)player > 9) {
//		LY_LOG_ERR("player must be between 1..9, " << (unsigned int)player);
		return LYSeatPtr();
	}
	std::vector<LYSeatPtr>::iterator it = seats.begin();
	for(; it != seats.end(); it++) {
		LYSeatPtr seat = *it;
        if (seat->isMe(player)) {
             return seat;
        }
    }
	return LYSeatPtr();
}

bool LYTable::isOpen(enum LYApplicant player)
{
	LYSeatPtr seat = fetchSeat(player);
	if (seat.get() == NULL) return true;
	return (seat->status == LYSeatOpen);
}

LYSeatPtr LYTable::fetchNextOccupiedSeat(enum LYApplicant player)
{
	//caiqingfeng 20130418, 不能调用Game的接口，因为需要在没有Game的时候调用
	std::vector<LYSeatPtr>::iterator it = seats.begin();
	for(; it != seats.end(); it++) {
		LYSeatPtr st = *it;
		if (st->isMe(player) || st->hasSmallerSeatNo(player) || st->getStatus() == LYSeatOpen) continue;

        if (st->getChipsAtHand() > 0 &&
        	(st->isOccupied())) {
             return st;
        }
    }
	it = seats.begin();
	for(; it != seats.end(); it++) {
		LYSeatPtr st = *it;
		if (st->isMe(player) || st->hasBiggerSeatNo(player) || st->getStatus() == LYSeatOpen) continue;
        if (st->getChipsAtHand() > 0 &&
        	(st->isOccupied())) {
             return st;
        }
    }
	return LYSeatPtr();
}

//caiqingfeng 20130418, 不能调用Game的接口，因为需要在没有Game的时候调用
LYSeatPtr LYTable::fetchNextSeatInGame(enum LYApplicant player)
{
	std::vector<LYSeatPtr>::iterator it = seats.begin();
	for(; it != seats.end(); it++) {
		LYSeatPtr st = *it;
		if (st->hasBiggerSeatNo(player) && st->isInGame()){
			return st;
		}
	}
	it = seats.begin();
	for(; it != seats.end(); it++) {
		LYSeatPtr st = *it;
		if (st->hasSmallerSeatNo(player) && st->isInGame()){
			return st;
		}
	}
	return LYSeatPtr();
}

LYSeatPtr LYTable::fetchFirstOccupiedSeat()
{
//	LY_LOG_DBG("entering fetchFirstOccupiedSeat");
	//caiqingfeng 20130418, 不能调用Game的接口，因为需要在没有Game的时候调用
	std::vector<LYSeatPtr>::iterator it = seats.begin();
	for(; it != seats.end(); it++) {
		LYSeatPtr st = *it;
//		LY_LOG_DBG("KKKK \n" << st->serialize().toString());
		if (st->getStatus() == LYSeatOpen) continue;
		if (st->getStatus() == LYSeatOccupied) {
			if (st->getChipsAtHand() > 0) return st;
			continue;
		}
		continue;
//		return st;
    }
	return LYSeatPtr();
}

//all seats occupied including in game
const unsigned int LYTable::totalSeatsInGame()
{
	unsigned int count = 0;
	//caiqingfeng 20130418, 不能调用Game的接口，因为需要在没有Game的时候调用
	std::vector<LYSeatPtr>::iterator it = seats.begin();
	for(; it != seats.end(); it++) {
		LYSeatPtr st = *it;
		if (st->getStatus() == LYSeatOpen) continue;
		if (st->getStatus() == LYSeatOccupied) {
			if (st->getChipsAtHand() > 0) count++;
			continue;
		}
		count++;
    }
	return count;
}

LYSeatPtr LYTable::fetchNextSeat(enum LYApplicant player)
{
	std::vector<LYSeatPtr>::iterator it = seats.begin();
	for(; it != seats.end(); it++) {
		LYSeatPtr st = *it;
        if (st->hasBiggerSeatNo(player)) {
             return st;
        }
    }
	it = seats.begin();
	for(; it != seats.end(); it++) {
		LYSeatPtr st = *it;
        if (st->hasSmallerSeatNo(player)) {
             return st;
        }
    }
	return LYSeatPtr();
}

const unsigned int LYTable::getSeatedPlayers()
{
	std::vector<LYSeatPtr>::iterator it = seats.begin();
	unsigned int seated = 0;
	for (; it != seats.end(); it++) {
		if ((*it)->isSeated()) {
			seated++;
		}
	}
	return seated;
}

bool LYTable::havePlayer(const std::string &player)
{
	std::vector<std::string>::iterator it = players.begin();
	for (; it!=players.end(); it++) {
		if (*it == player) return true;
	}
	return false;
}

LYSeatPtr LYTable::fetchSeatByPlayerId(const std::string &player)
{
	std::vector<LYSeatPtr>::iterator it = seats.begin();
	for(; it != seats.end(); it++) {
		LYSeatPtr seat = *it;
        if (seat.get() != NULL && seat->status != LYSeatOpen && seat->isMe(player)) {
             return seat;
        }
    }
	return LYSeatPtr();
}

void LYTable::setProfile(const std::string &prof)
{
	profile_id = prof;
	if (NULL == trunk || this->owner != LUYUN_HOUSE) {
		return;
	}
	trunk->profile_id = prof;
}

std::string LYTable::getProfile()
{
	if (this->profile_id != "") {
		return this->profile_id;
	}
	if (NULL == trunk) {
		return std::string("");
	}
	return trunk->profile_id;
}

std::string LYTable::getSeatedPlayerId(enum LYApplicant player)
{
	LYSeatPtr seat = fetchSeat(player);
	if (NULL == seat.get()) {
		return "";
	}
	return seat->playerId;
}

std::vector<LYSeatPtr> LYTable::findReservedSeats()
{
	std::vector<LYSeatPtr> sts;
	std::vector<LYSeatPtr>::iterator it = seats.begin();
	for (; it != seats.end(); it++) {
		if ((*it)->status == LYSeatReserved) {
			sts.push_back(*it);
		}
	}
	return sts;
}

std::vector < std::pair<std::string, std::string> > LYTable::table2kvps()
{
	std::vector < std::pair<std::string, std::string> > key_value_pairs;
	if (this->_id == "") {
		this->_id = LYUtils::genId();
	}

	std::pair < std::string, std::string > kvp;
	kvp.first = "_id";
	kvp.second = this->_id;
	key_value_pairs.push_back(kvp);

	kvp.first = "name";
	kvp.second = this->name;
	key_value_pairs.push_back(kvp);

	kvp.first = "owner";
	kvp.second = this->owner;
	key_value_pairs.push_back(kvp);

	kvp.first = "total_games";
	kvp.second = std::to_string(this->game_round);
	key_value_pairs.push_back(kvp);

	kvp.first = "total_buyin";
	kvp.second = std::to_string(this->total_buyin);
	key_value_pairs.push_back(kvp);

	kvp.first = "assurance_income";
	kvp.second = std::to_string(this->assurance_income);
	key_value_pairs.push_back(kvp);

	kvp.first = "assurance_flag";
	kvp.second = std::to_string(this->assurance_flag);
	key_value_pairs.push_back(kvp);

	kvp.first = "started_at";
	kvp.second = std::to_string(this->started_at);
	key_value_pairs.push_back(kvp);

	kvp.first = "last_duration";
	kvp.second = std::to_string(this->last_duration);
	key_value_pairs.push_back(kvp);

	/*
	 * integer should be processed to string
	 */
	kvp.first = "status";
	kvp.second = std::to_string(this->status);
//	LY_LOG_DBG("status:"+table->status);
	key_value_pairs.push_back(kvp);

	kvp.first = "total_seats";
	kvp.second = std::to_string(this->total_seats);
//	LY_LOG_DBG("total_seats:"+table->total_seats);
	key_value_pairs.push_back(kvp);

//	kvp.first = "timeout";
//	kvp.second = std::to_string(this->timeout);
////	LY_LOG_DBG("total_seats:"+table->total_seats);
//	key_value_pairs.push_back(kvp);
//
	kvp.first = "occupied_seat_number";
	kvp.second = std::to_string(this->occupied_seat_number);
//	LY_LOG_DBG("total_seats:"+table->total_seats);
	key_value_pairs.push_back(kvp);

	kvp.first = "profile_id";
	kvp.second = this->profile_id;
//	LY_LOG_DBG("total_seats:"+table->total_seats);
	key_value_pairs.push_back(kvp);

//	kvp.first = "timestamp";
//	kvp.second = std::to_string(time(NULL));
//	key_value_pairs.push_back(kvp);
//
	return key_value_pairs;
}

unsigned int LYTable::getTotalBuyin()
{
	return total_buyin;
}

/*
 * 服务器重置seat的游戏状态
 */
void LYTable::resetAllSeatsForNewGame()
{
	std::vector<LYSeatPtr>::iterator it = seats.begin();
	for (; it!=seats.end(); it++) {
		LYSeatPtr seat = *it;
		seat->resetForGame();
	}
}

/*
 * 客户端或者从DB中恢复table/seats，首先将其完全重置
 */
void LYTable::resetAllSeats()
{
	std::vector<LYSeatPtr>::iterator it = seats.begin();
	for (; it!=seats.end(); it++) {
		LYSeatPtr seat = *it;
		seat->reset();
	}
}

void LYTable::clearCurrentGame() {
	resetAllSeatsForNewGame();
	if (trunk == NULL) {
		return;
	}
	trunk->clearCurrentGame();
}
