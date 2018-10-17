//
//  LYCommonDefine.h
//  iBuddyHoldem
//
//  Created by 蔡 庆丰 on 13-2-28.
//  Copyright (c) 2013年 蔡 庆丰. All rights reserved.
//

#ifndef LYPOKER_CONSTANTS_H_
#define LYPOKER_CONSTANTS_H_

#include <string>

const std::string LUYUN_HOUSE = "luyun_house"; //table's owner default is house但如果支持用户占桌子成为桌主的话，会改变

enum LYFace
{
    NOFACE = 0,
    ACE = 14,
    TWO = 2,
    THREE = 3,
    FOUR = 4,
    FIVE = 5,
    SIX = 6,
    SEVEN = 7,
    EIGHT = 8,
    NINE = 9,
    TEN = 10,
    JACK = 11,
    QUEEN = 12,
    KING = 13,
    SMALL_GHOST = 24,
    BIG_GHOST = 25
};

enum LYSuit
{
    Nosuit = 0, Clubs = 1, Diamonds = 2, Hearts = 3, Spades = 4
};

enum LYApplicant
{
    LYHouse = 0, //保留
    LYVirtualDealer = 99,
    LYSeat1 = 1,
    LYSeat2 = 2,
    LYSeat3 = 3,
    LYSeat4 = 4,
    LYSeat5 = 5,
    LYSeat6 = 6,
    LYSeat7 = 7,
    LYSeat8 = 8,
    LYSeat9 = 9,
    LYSeat10 = 10,

    LYOutOfRoom = 40,
    LYWatcher = 50, //旁观者

    LYButtonSeat = 80,
    LYSmallBlindSeat = 81,
    LYBigBlindSeat = 82
};

enum LYTableStatus
{
    LYTableOpen = 0,
    LYTableInGame = 1,
    LYTableClose = 2,
    LYTableShutingDown = 3 //added 20140313 为了关闭一个桌子
};

enum LYSeatStatus
{
    LYSeatOpen = 0,
    LYSeatOccupied = 1,
    LYSeatInGame = 2,
	LYSeatReserved = 99 //added 20150209，留座离桌状态，此座位为空，但仅有该用户可以占用
};

#endif
