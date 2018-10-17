/*
 * Fixture.h
 *
 *  Created on: 2014-4-17
 *      Author: caiqingfeng
 */

#ifndef FIXTURE_H_
#define FIXTURE_H_

#include "src/LYPineappleTable.h"

const std::string creator = "creator";

class Fixture
{
public:
	Fixture();
	~Fixture();

public:
	void setBricks(LYPineappleSeat* seat, std::string cardStr, unsigned int line);

public:
    LYPineappleTable* pt;
    LYPineappleSeat* seat1, *seat2, *seat3;

public:

};

#endif


