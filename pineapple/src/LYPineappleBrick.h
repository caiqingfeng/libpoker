/*
 * LYPineappleBrick.h
 *
 *  Created on: 2014-9-22
 *      Author: caiqingfeng
 */

#ifndef LYPINEAPPLEBRICK_H_
#define LYPINEAPPLEBRICK_H_

#include "poker/src/LYCard.h"

#include <memory>

const unsigned int LY_UNDEFINED_POSITION = 0;
const unsigned int LY_TOP_1 = 1;
const unsigned int LY_TOP_2 = 2;
const unsigned int LY_TOP_3 = 3;
const unsigned int LY_MIDDLE_1 = 4;
const unsigned int LY_MIDDLE_2 = 5;
const unsigned int LY_MIDDLE_3 = 6;
const unsigned int LY_MIDDLE_4 = 7;
const unsigned int LY_MIDDLE_5 = 8;
const unsigned int LY_BOTTOM_1 = 9;
const unsigned int LY_BOTTOM_2 = 10;
const unsigned int LY_BOTTOM_3 = 11;
const unsigned int LY_BOTTOM_4 = 12;
const unsigned int LY_BOTTOM_5 = 13;

class LYPineappleBrick {
public:
	unsigned int brick; //0: 未设置，从服务器传下来的都是Brick；1,2,3 top; 4, 5, 6, 7, 8 middle; 9, 10, 11, 12, 13 bottom
	LYCard card;
public:
	LYPineappleBrick();
	LYPineappleBrick(unsigned int brk, const LYCard& cd);
	virtual ~LYPineappleBrick();

public:
	std::string toString();
};

typedef ::std::shared_ptr<LYPineappleBrick> LYPineappleBrickPtr;

#endif /* LYPINEAPPLEBRICK_H_ */
