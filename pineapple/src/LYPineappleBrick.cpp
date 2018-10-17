/*
 * LYPineappleBrick.cpp
 *
 *  Created on: 2014-9-22
 *      Author: caiqingfeng
 */

#include "LYPineappleBrick.h"
#include <boost/lexical_cast.hpp>

LYPineappleBrick::LYPineappleBrick()
{
	// TODO Auto-generated constructor stub

}

LYPineappleBrick::~LYPineappleBrick()
{
	// TODO Auto-generated destructor stub
}

LYPineappleBrick::LYPineappleBrick(unsigned int brk, const LYCard& cd)
{
	// TODO Auto-generated constructor stub
	card = cd;
	brick = brk;
}

std::string LYPineappleBrick::toString()
{
	std::string spb;
	spb = boost::lexical_cast<std::string>(brick) + ":" + boost::lexical_cast<std::string>(card.toInteger());
	return spb;
}
