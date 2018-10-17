/*
 * LYTable.h
 *
 *  Created on: 2013-7-5
 *      Author: caiqingfeng
 */

#ifndef LYTABLE_H_
#define LYTABLE_H_

#include <memory>
#include <vector>
#include <string>

#include "LYPokerConstants.h"
#include "LYSeat.h"
#include "LYTrunk.h"
#include "LYPokerProfile.h"

/*
 * 一个Table在某一段时间里，仅可能有一个Trunk
 * Table属于系统所有, created by system in advance
 * Trunk可能属于某个用户
 */
class LYTable
{
public: //为了不写一大堆的setter/getter还是将其设为Public
	std::string _id; //stored in db
	std::string name; //stored in db
	std::string profile_id;
	unsigned int no; //用于分页查找
	std::string server_handler; //处理的服务器标识，用于客户端发回来 20150205新增
	std::string owner; //属于哪个，luyun_house，或者club_id
	std::string creator; //创建者id
	unsigned int total_seats; //stored in db
	unsigned int game_round; //当前轮次进行了多少局游戏
	std::string current_game_id; //当前Game ID， 20141203增加
	unsigned int timeout; //快速桌=20s，一般桌子=40s
	enum LYTableStatus status; //stored in db
	unsigned int assurance_flag; //added 20160321, 1: active, 0: normal
	int assurance_income; //added 20160329
	unsigned int total_buyin;
	//未创建Game之前，从服务器取得；createGame/AnnouceWinner，takeSeat/leaveSeat时要更新这个数
	int occupied_seat_number;
	int last_duration; //added 20160409
	int started_at; //added 20160409
//	int last_polling_at; //added 20160709,上次polling的时间
	std::vector<std::string> players; //all players in this table
	std::vector<LYSeatPtr> seats; //
	LYTrunk *trunk; //for the sake of inheritence, not use shared_ptr

public:
	LYTable();
	LYTable(const std::string &tid, unsigned int ts, enum LYTableStatus sta,
			const std::string &nm, std::string creator="lyhouse");
	~LYTable();

	virtual void openTrunk(const std::string &trunk_id, const std::string &trunk_name,
			const std::string &player, const std::string &prof_id);
	virtual void openTrunk(const std::string &trunk_id);
	virtual void openTrunk(const std::string &trunk_id, const std::string &trunk_name,
			const std::string &player, LYPokerProfile *prof);

	void openTrunk(LYTrunk *trk); //set trunk by pointer
	void closeTrunk();
	LYTrunk *getTrunk() {return trunk;}
	void setProfile(const std::string &prof);
	std::string getProfile();
	void setServerHandler(const std::string& hdl);
	bool isTableOpen();
	int getTimeLeft();

	bool isThisName(const std::string &nm) { return name == nm; };
	const std::string& getOwner();
	void setOwner(const std::string &uid);
	const std::string& getId() {return _id;};
	const unsigned int getTotalSeats() { return total_seats; };
	const std::string &getName() {return name; };
	const enum LYTableStatus getStatus() { return status; };
	const unsigned int getSeatedPlayers();
	const unsigned int totalSeatsInGame();

public: //所有的Table都有相同的操作:enter table/leave seat/take seat/leave seat
	void enterTable(const std::string &uid);
	void leaveTable(const std::string &uid); //leave table
	void takeSeat(enum LYApplicant seat_no, unsigned int buyin, const std::string &uid); //有人坐在某个位子上
	virtual void buyin(enum LYApplicant seat_no, unsigned int buyin, const std::string &uid); //买入
	void leaveSeat(enum LYApplicant seat_no, int &cash_out_approved, const std::string &uid); //放弃某个座位
	void leaveSeatButReserve(enum LYApplicant seat_no); //保留某个座位
	void forceLeaveSeat(enum LYApplicant seat_no); //强制清空某个座位

public:
	std::vector<std::string> getNeighbors(const std::string &uid);
	bool playerInTable(const std::string &uid);
	std::vector<std::string> getAllPlayers();
	bool havePlayer(const std::string &player);
	LYSeatPtr fetchSeat(enum LYApplicant player);
	bool isOpen(enum LYApplicant player);
	std::vector<LYSeatPtr> getAllSeats();
	LYSeatPtr fetchNextSeat(enum LYApplicant player);
//	LYSeatPtr fetchDutySeat();
	LYSeatPtr fetchFirstOccupiedSeat();
	LYSeatPtr fetchNextOccupiedSeat(enum LYApplicant player);
	LYSeatPtr fetchSeatByPlayerId(const std::string &player);
	std::string getSeatedPlayerId(enum LYApplicant player);

public:
	LYSeatPtr fetchNextSeatInGame(enum LYApplicant player);
	std::vector<LYSeatPtr> findReservedSeats();
	virtual bool isGameOngoing() {return false;};
	bool isAssuranceActive() {return assurance_flag == 1;}

public:
	LYGame *getCurrentGame()
	{
		if (trunk == NULL) {
			return NULL;
		}
		return trunk->getCurrentGame();
	}

	LYGame *getLastGame()
	{
		if (trunk == NULL) {
			return NULL;
		}
		return trunk->getLastGame();
	}

	void clearCurrentGame();
	/*
	 * 在反序列化时，先生成Game实例，然后通过本createGame方法将二者关联
	 */
	void createGame(LYGame *gm)
	{
		if (trunk == NULL) {
			return ;
		}
		return trunk->createGame(gm);
	}

public: //通用游戏功能，在各子类中重载
	virtual bool ready2go() {return false;};
	virtual bool isGameOver() {return false;};
	virtual void createGame() {return;};
	virtual std::vector < std::pair<std::string, std::string> > table2kvps();
	virtual unsigned int getTotalBuyin();
	void resetAllSeatsForNewGame();
	void resetAllSeats();

};

typedef ::std::shared_ptr<LYTable> LYTablePtr;

#endif /* LYTABLE_H_ */
