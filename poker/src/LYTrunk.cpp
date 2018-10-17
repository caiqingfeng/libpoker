/*
 * LYTrunk.cpp
 *
 *  Created on: 2013-7-5
 *      Author: caiqingfeng
 */

#include "LYTrunk.h"

LYTrunk::LYTrunk(const std::string &trunk_id, const std::string &trunk_name,
			LYTable *tbl, unsigned int &ts, std::vector<LYSeatPtr> &all_seats,
			const std::string &player, const std::string &prof_id) :
			totalSeats(ts), seats(all_seats)
{
	// TODO Auto-generated constructor stub
	_id = trunk_id;
	owner = player;
	name = trunk_name;
	table = tbl;
	profile_id = prof_id;
	currentGame = NULL;
	lastGame = NULL;
	open_at = 0;
	close_at = 0;
	surplus = 0;
	profile = NULL;
}

LYTrunk::~LYTrunk() {
	// TODO Auto-generated destructor stub
	if (currentGame != NULL) delete currentGame;
	if (lastGame != NULL) delete lastGame;
	lastGame = NULL;
	currentGame = NULL;
}

void LYTrunk::createGame(LYGame *gm)
{
	lastGame = currentGame;
	currentGame = gm;
	currentGame->seatsOfTable = seats;
	currentGame->table = table;
}

void LYTrunk::clearCurrentGame()
{
	if (currentGame == NULL) return;
	if (lastGame != NULL) delete lastGame;
	lastGame = currentGame;
	currentGame = NULL;
}
