/*
 * Fixture2p.cpp
 *
 *  Created on: 2014-4-10
 *      Author: caiqingfeng
 */

#include <string>
#include "Fixture2p.h"
#include "libmrock/common/src/LYUtils.h"

Fixture2p::Fixture2p()
{
	pt = new LYPineappleTable(creator);
	unsigned int seat = LYSEAT1;
	pt->takeSeat((enum LYApplicant)seat++, 10000, creator);
	pt->takeSeat((enum LYApplicant)seat++, 10000, "player2");
	seat1 = (LYPineappleSeat*)pt->fetchSeat(LYSeat1).get();
	seat2 = (LYPineappleSeat*)pt->fetchSeat(LYSeat2).get();
	pt->openTrunk(LYUtils::genId(), //trunk id, not used yet
			LYUtils::genId(),  //trunk name, not used yet
			"lyhouse", "no profile");
}

Fixture2p::~Fixture2p()
{
	delete pt;
}

void Fixture2p::setBricks(LYPineappleSeat* seat, std::string cardStr, unsigned int line)
{
	unsigned int cardNum = cardStr.size()/2;
	for (unsigned int i=0; i<cardNum; i++) {
		char face = cardStr[i*2];
		char suit = cardStr[i*2+1];
		LYCard card(face, suit);
		switch (line)
		{
		case 1:
			seat->topBricks.push_back(card);
			break;
		case 2:
			seat->middleBricks.push_back(card);
			break;
		case 3:
			seat->bottomBricks.push_back(card);
			break;
		default:
			break;
		}
	}
}
