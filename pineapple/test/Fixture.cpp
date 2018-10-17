/*
 * Fixture.cpp
 *
 *  Created on: 2014-4-10
 *      Author: caiqingfeng
 */

#include <string>
#include "Fixture.h"
#include "libmrock/common/src/LYUtils.h"

Fixture::Fixture()
{
	pt = new LYPineappleTable(creator);
	pt->openTrunk(LYUtils::genId(), "first turnk", "lyhouse", "profile");
	unsigned int seat = LYSEAT1;
	pt->takeSeat((enum LYApplicant)seat++, 10000, creator);
	pt->takeSeat((enum LYApplicant)seat++, 10000, "player2");
	pt->takeSeat((enum LYApplicant)seat++, 10000, "player3");
	seat1 = (LYPineappleSeat*)pt->fetchSeat(LYSeat1).get();
	seat2 = (LYPineappleSeat*)pt->fetchSeat(LYSeat2).get();
	seat3 = (LYPineappleSeat*)pt->fetchSeat(LYSeat3).get();
}

Fixture::~Fixture()
{
	delete pt;
}

void Fixture::setBricks(LYPineappleSeat* seat, std::string cardStr, unsigned int line)
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
