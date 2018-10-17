/*
 * LYPlayer.h
 *
 *  Created on: 2013-7-2
 *      Author: caiqingfeng
 */

#ifndef LYPLAYER_H_
#define LYPLAYER_H_

#include <string>
#include <vector>
#include <memory>

#include "LYDevice.h"

using std::string;

const unsigned int MAX_BONUS_SEQ = 6; //6天之后的Bonus都一样

enum UserRole
{
    ROLE_PLAYER = 0,                    //  player
    ROLE_OWNER  = 1                    // owner
};

class LYPlayerPosition
{
public:
    string table_id;
    int chips_at_hand;

public:
    LYPlayerPosition()
    {
        chips_at_hand = 0;
    };
    LYPlayerPosition(const std::string& t, int chips)
    {
        table_id = t;
        chips_at_hand = chips;
    };
};

class LYPlayer {
public:
	LYPlayer(const string& uid, const string& device_id);
	LYPlayer();
	virtual ~LYPlayer();

public:
    string _id;
    string nick_name;
    string icon_url;
    unsigned int balance;
//    string position; //"hall" means in hall, otherwise should be a table id
    std::vector<LYPlayerPosition> positions;
    string dev_id;

    UserRole role;
    bool activing;

    //added 20140316 for daily bonus
    time_t last_lucky_time;
    unsigned int day_in_sequence; //0 means not in the sequence, 1: first day

public:
    bool list_daily_bonus(unsigned int& seq, std::vector<unsigned int>& result);
    unsigned int apply_daily_bonus(unsigned int& seq); //0 means no more bonus today

    void leaveTable(const string& table_id);
    void enterTable(const string& table_id);

public:

    bool operator==(const LYPlayer & other) const
    {
        return this->_id == other._id;
    }

//    void set_position(string pos)
//    {
//        this->position = pos;
//    }
//    string& get_position()
//    {
//        return this->position;
//    }

    const string& get_uid()
    {
        return this->_id;
    }

    string& get_nick_name()
    {
        return this->nick_name;
    }
    void set_nick_name(string nick_name)
    {
        this->nick_name = nick_name;
    }

    void set_icon_url(string icon_url)
    {
        this->icon_url = icon_url;
    }
    string& get_icon_url()
    {
        return this->icon_url;
    }

    int get_balance()
    {
        return this->balance;
    }

    void set_balance(int balance_)
    {
        this->balance = balance_;
    }

    bool is_activing()
    {
        return this->activing;
    }

    void set_active()
    {
        this->activing = true;
    }

    void de_active()
    {
        this->activing = false;
    }

    virtual bool is_game_owner()
    {
        return this->role == ROLE_OWNER;
    }

    virtual bool can_take_seat(const std::string& table_id, int buyin)
    {
        return balance >= buyin;
    }

//    void set_game_owner(string& tb_id, string& tbl_name)
//    {
//        this->position = tb_id;
//        set_role(ROLE_OWNER);
//    }

    void set_role(UserRole rol)
    {
        this->role = rol;
    }

    UserRole get_role()
    {
        return this->role;
    }

    virtual void onTableDelete()
    {
        this->role = ROLE_PLAYER;
//        this->position = "hall";
    }

//    virtual void onGameClose()
//    {
//        if(!this->is_game_owner())
//        {
//            this->table_name = "";
//            this->table_id = "";
//        }
//
//        this->status = ST_IN_HALL;
//    }
//
//    void takeSeat(LYApplicant sn, unsigned int buyin){
//        this->seat_no = sn;
//        this->status = ST_SIT_DOWN;
//        //TODO: not atomic operation here,
//        balance = balance - buyin;
//    }
//
//    void leaveSeat(unsigned int cashout){
//        this->seat_no = LYWatcher;
//        this->status = ST_IN_HALL;
//        balance = balance + cashout;
//    }
//
//    void play_game()
//    {
//        this->status = ST_PLAYING;
//    }

};

typedef ::std::shared_ptr<LYPlayer> LYPlayerPtr;

#endif /* LYPLAYER_H_ */
