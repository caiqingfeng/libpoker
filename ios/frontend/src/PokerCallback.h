/*
 * Callback.h
 *
 *  Created on: 2013-7-25
 *      Author: chenfeng
 */

#ifndef POKER_CALLBACK_H_
#define POKER_CALLBACK_H_

#include "libmrock/frontend/src/Callback.h"
#include <vector>
#include <string>

/*
 * 所有的Poker类游戏都需要继承本类，实现Callback方法
 */
class PokerCallback : public com::luyun::msg::client::Callback {
public:
	PokerCallback();
    virtual ~PokerCallback();

public: //游戏动作部分
    /*
     * 以下是将原来实现在onScene中的逻辑分解了之后，需要实现的方法
     */
    virtual void onResetTable(const string& table_id, const string&  name,
                            unsigned int sbprice, unsigned int bbprice, unsigned int game_round,
                            unsigned int assurance_flag, int assurance_income) {};
    virtual void onRebuy(const string& table_id, unsigned int seat) {}; //触发Rebuy
    virtual void onBuyin(const string& uid, const string& table_id, unsigned int seat, unsigned int buyin, int chips) {}; //收到Buyin成功消息
    //在onSetCommunity的实现里，要将每个Seat的Bet清空，相当于实现View的onNewRound()
    virtual void onSetCommunity(const string& table_id, unsigned int round,
    		unsigned int flop1, unsigned int flop2, unsigned int flop3,
    		unsigned int turn, unsigned int river) {};
    virtual void onNewRound(const string& table_id) {};
    virtual void onSetPot(const string& table_id,
    		unsigned int potLevel, unsigned int chips) {};
    //以下把SeatView的设置分解
    virtual void onSetSeatOpen(const string& table_id, unsigned int seat) {};
    virtual void onSetSeatBasic(const string& table_id, unsigned int seat, unsigned int status,
    		unsigned int game_status, std::string playerId, int chipsAtHand, unsigned int lastBet,
    		unsigned int firstCard, unsigned int secondCard) {};
    virtual void onSetSeatStatus(const string& table_id, unsigned int seat,
    		unsigned int gameStatus, int chips, unsigned int ranking) {};
    virtual void onSetSeatBet(const string& table_id, unsigned int seat,
    		unsigned int lastBet) {};

    virtual void onSetSeatDealer(const string& table_id, unsigned int seat) {};
    virtual void onSetSeatSmall(const string& table_id, unsigned int seat, unsigned int small) {};
    virtual void onSetSeatBig(const string& table_id, unsigned int seat, unsigned int big) {};
    virtual void onStartTimer(const string& table_id, unsigned int seat, unsigned int seconds_left) {};
    virtual void onStopTimer(const string& table_id, unsigned int seat) {};

    virtual void onSetSeatWinner(const string& table_id, unsigned int pot,
            unsigned int seat, int chips_at_hand, int chips_at_beginning,
    		unsigned int ranking, unsigned int firstCard, unsigned int secondCard) {};

    virtual void onApproveSeat(const string& table_id, unsigned int seat_no) {};
    virtual void onRefuseSeat(const string& table_id, unsigned int seat_no) {};
    virtual void onApproveBuyin(const string& table_id, unsigned int seat_no) {};
    virtual void onRefuseBuyin(const string& table_id, unsigned int seat_no) {};

    virtual void onEnterTable(const string& table_id, const string& uid, int chips_at_hand) {};
    virtual void onLeaveTable(const string& table_id, const string& uid) {};
    virtual void onTakeSeat(const string& table_id, unsigned int seat_no, unsigned int buyin, const string& uid) {};
    virtual void onLeaveSeat(const string& table_id, unsigned int seat_no, unsigned int cashout, const string& uid) {};

//for 保险20160320
    virtual void onAskAssurance(const string& table_id, unsigned int seat_no, unsigned int maxassure,
                    unsigned int potlevel, const string& outs) {};
    virtual void onBuyAssurance(const string& table_id, unsigned int seat_no, unsigned int buyassure,
                    unsigned int potlevel, unsigned int expected = 0) {};
    virtual void onPayAssurance(const string& table_id, unsigned int seat_no, unsigned int payassure,
                    unsigned int potlevel) {};
    virtual void onAssuranceSummary(const string& table_id, int incomeOfThisGame, int totalIncome) {};
};

#endif /* CALLBACK_H_ */
