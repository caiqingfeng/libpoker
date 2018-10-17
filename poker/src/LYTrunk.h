/*
 * LYTrunk.h
 *
 *  Created on: 2013-7-5
 *      Author: caiqingfeng
 */

#ifndef LYTRUNK_H_
#define LYTRUNK_H_

#include <memory>
#include "LYSeat.h"
#include "LYGame.h"
#include "LYPokerProfile.h"

class LYTable;

/*
 * Dealer和Trunk生命周期相同
 */
class LYTrunk {
public: //这些属性保存在数据库中
	std::string _id;
	std::string profile_id;
	std::string owner;
	std::string name;
	unsigned long long open_at; //stored in mongo as date_t
	unsigned long long close_at;//stored in mongo as date_t
	unsigned long surplus;

public:
	LYPokerProfile* profile;

protected:
	LYTable *table; //belongs to which table

	unsigned int &totalSeats; //reference of LYTable.totalSeats
	std::vector<LYSeatPtr> &seats; //reference of LYTable.seats;

protected:
	LYGame *currentGame;
	LYGame *lastGame;

public:
	LYTrunk(const std::string &trunk_id, const std::string &trunk_name,
				LYTable *tbl, unsigned int &ts, std::vector<LYSeatPtr> &all_seats,
				const std::string &player, const std::string &prof_id);

	virtual ~LYTrunk();
	void createGame(LYGame *gm);
	const std::string& getOwner() {return owner;}; //discarded 20160315
	const std::string& getId() { return _id; };
	void setId(const std::string &id) { _id = id; };
	void setOwner(const std::string &uid) {owner = uid;};
	virtual void activeProfile(){};
	void clearCurrentGame();

public: //游戏动作
	virtual void createGame(const std::string &game_id, LYPokerAlgorithmDelegate *sd=NULL) {}; //无other参数，让Trunk根据自身状态即可作出判断
	LYGame *getCurrentGame() {return currentGame;}
	LYGame *getLastGame() {return lastGame;}
	virtual bool ready2go() {return false;};
	virtual bool isGameOver() {return false;};
};

typedef ::std::shared_ptr<LYTrunk> LYTrunkPtr;

#endif /* LYTRUNK_H_ */
