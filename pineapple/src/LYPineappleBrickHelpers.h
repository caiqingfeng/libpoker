//
//  LYPineappleBrickHelpers.h
//
//  Created by 蔡 庆丰 on 14-10-25.
//  Copyright (c) 2013年 蔡 庆丰. All rights reserved.
//
#ifndef _LY_PINEAPPLEBRICK_HELPERS_H
#define _LY_PINEAPPLEBRICK_HELPERS_H

#include "LYPineappleBrick.h"
#include <vector>
#include <string>

class LYPineappleBrickHelpers {
public:
	//一组Card，输出"1:As,2:Ah,..."
	static void bricksToString(std::vector<LYPineappleBrick>& bricks, std::string& cs);
	//一组Card，输出"1:42,2:5,4:6..."
	static void bricksToStringifiedInt(std::vector<LYPineappleBrick>& bricks, std::string& cs);

	//String to Cards
	static void stringifiedIntToBricks(const std::string& cs, std::vector<LYPineappleBrick>& bricks);
};

#endif
