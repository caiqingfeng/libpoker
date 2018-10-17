/*
 * LYPineappleTable.h
 *
 *  Created on: 2014-9-16
 *      Author: caiqingfeng
 */

#ifndef LYPINEAPPLETABLE_H_
#define LYPINEAPPLETABLE_H_

#include "poker/src/LYTable.h"
#include "LYPineappleSeat.h"
#include "LYPineappleAction.h"
#include "LYPineappleProfile.h"

class LYPineappleTable : public LYTable {
private:
	LYPineappleSeat* seat1, *seat2, *seat3;
public:

public:
	LYPineappleTable();
	LYPineappleTable(const std::string &creator, std::string prof="private_noking");
	LYPineappleTable(const std::string &table_id, const std::string &creator, unsigned int size,
			std::string prof="private_noking");
	virtual ~LYPineappleTable();

public:
	void createSeats();
	void openTrunk(const std::string &trunk_id, const std::string &trunk_name,
			const std::string &player, const std::string &prof_id);
	void resetSeatsForNewGame(); //reset seat to initial status
	void setSeat(const LYPineappleSeat &seat);
	bool playGame(LYPineappleAction &action);
	LYRanking caculateSeatRanking(LYApplicant seatNo);

public:
	virtual void createGame(const std::string &game_id="newgame",
			LYHoldemAlgorithmDelegate *had=NULL, std::string prof="private_noking");
	virtual bool ready2go(bool auto_start=false);
	void voteForNext(const std::string& uid);
	void createGameInstance(const std::string &game_id);
	void run(LYHoldemAlgorithmDelegate *had=NULL);
	bool canLeave(unsigned int seat_no);
	std::vector<LYSeatPtr> findUnqualifiedSeats(unsigned int min_chips);
	std::vector<LYSeatPtr> findReservedSeats();

public:
	virtual bool isGameOngoing();
};

typedef ::std::shared_ptr<LYPineappleTable> LYPineappleTablePtr;

#endif /* LYPINEAPPLETABLE_H_ */
