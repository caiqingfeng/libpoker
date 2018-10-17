//
//  LYCommonDefine.h
//  iBuddyHoldem
//
//  Created by 蔡 庆丰 on 13-2-28.
//  Copyright (c) 2013年 蔡 庆丰. All rights reserved.
//

#ifndef LYHOLDEMCONSTANTS_H_
#define LYHOLDEMCONSTANTS_H_

#include <string>

enum LYRanking
{
    HighCard = 0,
    OnePair = 1,
    TwoPair = 2,
    ThreeOfAKind = 3,
    Straight = 4,
    Flush = 5,
    FullHouse = 6,
    FourOfAKind = 7,
    StraightFlush = 8
};

enum LYGameRound
{
    LYGameWaiting = 0, //等待Game，在Table刚开始，或者人数不够等人的时候发生
    LYGameInit = 1, // new game
    LYGameFirstCard = 2, //first card, 20130416 cqf: first/second card not used any more
    LYGameSecondCard = 3, //second card
    LYGamePreflop = 4,
    LYGameFlop = 5,
    LYGameTurn = 6,
    LYGameRiver = 7,
//    LYGameNextRound = 77, // added 20130527 next round , deprecated 20130710
//    LYGameSameRound = 78, //added 20130527 same round
//    LYGameCheckdown = 79, //added 20130528
    LYGameClosed = 99 //一局游戏结束，立即过渡到下一个GameInit状态
};

enum LYPotLevel
{
    LYMainPot = 1,
    LYSidePot1 = 2,
    LYSidePot2 = 3,
    LYSidePot3 = 4,
    LYSidePot4 = 5,
    LYSidePot5 = 6,
    LYSidePot6 = 7,
    LYSidePot7 = 8,
    LYSidePot8 = 9
};

enum LYVerb
{
//    LYHouseOpenTable = 0,  //House通知Dealer开桌
//    LYHouseCreateGame = 1,  //新一轮游戏开始
//    LYPlayerSynchronizeTable = 2, //当一个玩家加入Table或者旁观该Table，需要跟服务器同步Table状态
//    LYPlayerTakeSeat = 3, //申请坐下，需要得到管理员的批准和购买筹码
//    LYPlayerLeaveSeat = 4, //Seat Open
//    LYPlayerJoinTable = 5, //旁观一个Table，需要得到管理员的批准，当其曾经进来这张桌子，则无需得到批准
//
//    LYFirstMove = 10,
    LYHouseIssueFirstCard = 13,
    LYHouseIssueSecondCard = 14,

    LYHouseIssueFlop = 15,
    LYHouseIssueTurn = 16,
    LYHouseIssueRiver = 17,

//    LYHouseShowdownAll = 66,
//    LYHouseAnnounceWinner = 77,
//
//    LYHouseForceSmallBlind = 88, //reserved,实际上应该在createGame的时候就指定了大小盲注
//    LYHouseForceBigBlind = 89,

    LYHousePollingDecision = 90,
    LYBet = 91,
    LYCall = 92,
    LYCheck = 93,
    LYFold = 94,
    LYRaise = 95,
    LYAllin = 96,
    LYBetSmallBlind = 97,
    LYBetBigBlind = 98,
    LYAskAssurance = 99,
    LYBuyAssurance = 100,
    LYPayAssurance = 101,
    LYSummarizeAssurance = 102
};

enum LYSeatHoldemStatus
{
	LYSeatHoldemWaiting = 0,
	LYSeatHoldemBetSmall = 1,
	LYSeatHoldemBetBig = 2,
    LYSeatHoldemMakingDecision = 3,
    LYSeatHoldemInBet = 7,
    LYSeatHoldemInCalled = 8,
    LYSeatHoldemInFold = 9,
    LYSeatHoldemInChecked = 10,
    LYSeatHoldemInRaised = 11,
    LYSeatHoldemInAllin = 12
};

/*
 * 20130710新加，用于每个动作之后判断下一个要进行的动作
 */
enum LYHoldemRoundContinue {
	LYHoldemRoundCont = 1, //本轮继续
	LYHoldemRoundNext = 2, //下一轮
	LYHoldemRoundDown = 3, //Checkdown
	LYHoldemRoundEnd = 4
};

enum LYGameType
{
    LYRingGame = 1, LYSngGame = 2, LYMttGame = 3
};

enum LYHandsOrder //best hand or ...
{
	LYNUTS = 1,
	LYSECOND_NUTS = 2,
	LYTHIRD_NUTS = 3,
	LYGUTSHOT_ODDS = 60,
	LYOUTSIDE_ODDS = 61,
	LYFLUSH_ODDS = 62,
	LYFULLHOUSE_ODDS = 63,
	LYFOUROFAKIND_ODDS = 64,
	LYSTRAIGHTFLUSH_ODDS = 65,
	LYON_DRAW = 88
};

const float LY_POLLING_TIMEOUT = 30.0; //timeout to wait for response from clients
const float LY_DELAYED_ACTION = 8.0; //interval between two new game
const float LY_FANTASY_TIMEOUT = 300.0; //范特西
const float LY_SHAKEHAND_TIMEOUT = 60.0; //握手1分钟
const float LY_RESERVE_SEAT_TIMEOUT = 120.0; //座位保留2分钟
const float LY_ASSURE_SEAT_TIMEOUT = 60.0; //保险的超时时间1分钟
const unsigned int LY_TIMEOUT_ALLOWED = 2; //client time out happen allowed
const unsigned int MAX_PLAYERS_ALLOWED_IN_ONE_TABLE = 30;

#endif
