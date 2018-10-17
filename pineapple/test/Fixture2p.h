/*
 * Fixture.h
 *
 *  Created on: 2014-4-17
 *      Author: caiqingfeng
 */

#ifndef FIXTURE_2P_H_
#define FIXTURE_2P_H_

#include "src/LYPineappleTable.h"

const std::string creator = "creator";

class Fixture2p
{
public:
	Fixture2p();
	~Fixture2p();

public:
	void setBricks(LYPineappleSeat* seat, std::string cardStr, unsigned int line);

public:
    LYPineappleTable* pt;
    LYPineappleSeat* seat1, *seat2;

public:

};

#endif


