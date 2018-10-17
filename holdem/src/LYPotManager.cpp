/*
 * LYPotManager.cpp
 *
 *  Created on: 2013-3-25
 *      Author: caiqingfeng
 */
//#include "boost/foreach.hpp"

#include <cstdlib>
#include "LYPotManager.h"
//#include "common/src/my_log.h"
#include "LYHoldemGame.h"
#include <iostream>

LYPotManager::LYPotManager(LYHoldemGame *gm) {
	// TODO Auto-generated constructor stub
	topLevel = LYMainPot;
	mainPot = LYPotPtr(new LYPot());
	pots.push_back(mainPot);
	game = gm;
}

LYPotManager::~LYPotManager() {
	// TODO Auto-generated destructor stub
	pots.clear();
}

void LYPotManager::createPots(unsigned int pot_size)
{
	if (pot_size < 2) return;
	for (unsigned int i=1; i<pot_size; i++) {
		createSidePot(0, std::vector<LYSeatPtr>());
	}
}

LYPotPtr LYPotManager::fetchPot(enum LYPotLevel pl)
{
	std::vector<LYPotPtr >::iterator it = pots.begin();
	for (; it!=pots.end(); it++) {
		if ((*it)->potLevel == pl) return *it;
	}
	return LYPotPtr();
}

void LYPotManager::createSidePot(unsigned int chips, std::vector<LYSeatPtr>seats)
{
    LYPotPtr lastPot = mainPot;
    if (pots.size() > 1) {
        lastPot = pots.back();
    }
    LYPotPtr newPot = LYPotPtr(new LYPot(chips));
	newPot->potLevel = (LYPotLevel)(this->topPot()->potLevel + 1);
//    newPot->potLevel = (enum LYPotLevel)(((int)lastPot->potLevel)+1);
    pots.push_back(newPot);
}

void LYPotManager::addPot(LYPot *pot)
{
	pots.push_back(LYPotPtr(pot));
	if (pots.size() == 1) {
		mainPot = pots[0];
	}
}
/*
 * 如何分Pot是最复杂的一个函数
 * toBet = (sit.chipsAtHand>chips)?chips:sit.chipsAtHand
 * 遍历pots, chipsToBet = sum of (所有参与的pot.unitPrice)
 * 如果当前pot.unitPrice > chipsToBet, 表明是第一个Bet，即是preflop的smallBlind bet
 * 如果pot.unitPrice < chipsToBet，说明这个Pot要溢出，例如seat1:50, seat2:60，则溢出创建一个新的Pot
 */
void LYPotManager::betBySeat(unsigned int chips, LYSeatPtr st) //自动加入正确的Pot
{
//	LY_LOG_DBG("enter betBySeat, chips=" << chips << ", seat=" << st->seatNo << ", chipsAtHand=" << st->chipsAtHand << ", lastBet=" << st->lastBet);
	    unsigned int chipsToBet = ((st->chipsAtHand+st->lastBet)>chips)?chips:(st->chipsAtHand+st->lastBet); //二者取小
//	    unsigned int chipsToBet = (st->chipsAtHand>chips)?chips:st->chipsAtHand; //二者取小
	    //st.lastBet > 0 ，说明是第二次以上Call/Raise，最复杂的一种情况，前面可能已经分了好几次Pot了
	    //要么分裂最上层一个Pot，要么分裂底下某个Pot
	    unsigned int  basePrice = 0;
	    int potIndex = -1;
	    /* CASE1 ---
	     * seat1: 100, seat2: 100
	     * seat1: bet 10, mainpot=10, mainpot.unitprice=10，进入分支G21
	     * seat2: bet 20, chipsToBet=20, chipsToPot=20, contribute=-10,进入分支G32, mainpot=30, mainpot.unitprice=20
	     * seat1: bet 20, chipsToBet=20, chipsToPot=20, contribute=-10，进入分支E2
	     * CASE2 ---
	     * seat1: 25, seat2: 100
	     * seat1: bet 10, mainpot=10, mainpot.unitprice=10，进入分支G21
	     * seat2: bet 30, 进入分支G32，mainpot=30, mainpot.unitprice=30
	     * seat1: bet 30, chipsToBet=25, chipsToPot=25，contribute=10-0-30=-20,导致分Pot，进入分支L2－L21，
	     * CASE3 ---
	     * seat1: 25, seat2: 20, seat3:15
	     * seat1: bet 10, 进入分支G21, mainpot=10, mainpot.unitprice=10
	     * seat2: bet 20, 进入分支G32，mainpot=30, mainpot.unitprice=20
	     * seat3: bet 20, chipsToBet=15, chipsToPot=15导致分Pot, mainpot=10+15+15, mainpot.unitprice=15, newpot=5, newpot.unitprice=5，进入分支TAG:
	     * seat1: bet 20
	     *          循环到mainpot时，chipsToBet=20, chipsToPot=20, contribute=-10,，进入分支TAG:S21
	     *          循环到secondpot时，chipsToBet=20, chipsToPot=20-15, contribute=15-15-5,进入分支TAG:E1
	     * CASE4 ---
	     * Seat1=25, seat2=10, seat3=200, seat4=30
	     * seat1 bet 20, mainpot=20
	     * seat2 call 10, mainpot=20, sidepot1=10
	     * seat3 bet 40, mainpot=30, sidepot1=40
	     * seat4 call 40, mainpot=40, sidepot1=50, sidepot2=10
	     * seat1 call 40, mainpot=40, sidepot1=45, sidepot2=10, sidepot3=10
	     *          循环到secondpot时，chipsToBet=5, chipsToPot=-5, contribute=-10,进入分支TAG:L4
	     */
		std::vector<LYPotPtr>::iterator it = pots.begin();
		for(; it != pots.end(); it++) {
			LYPotPtr p = *it;
//LY_LOG_DBG("looping pots ... level =" << p->potLevel << " chips=" << p->chipsInPot);
	        potIndex++;
	        if (!p->potOpen) continue;
	        int chipsToPot = chipsToBet - basePrice;	    // >0 意味着筹码足够将要投入到这个Pot <0意味着不够
	        if (potIndex == 0 && st->chipsAtHand+st->lastBet<=chips) {
	            chipsToPot -= st->lastBet;
	        }
	//        if (chipsToBet < basePrice) chipsToPot = basePrice - chipsToBet;
	        int contribute = st->lastBet - basePrice - p->unitPrice; //对当前这个Pot的贡献，>0 意味着已经投入筹码到这个Pot <0意味着还没有投入筹码
	        basePrice += p->unitPrice;
	        LYSeatPtr sit = this->findSeatInPot(st, p);
//LY_LOG_DBG("chipsToBet=" << chipsToBet << ", unit price=" << p->unitPrice << ", chipsToPot=" << chipsToPot << ", contribute=" << contribute << ", basePrice=" << basePrice);
	        if (NULL != sit.get()) {
	            //分支TAG:S
	//            if (p.priceFixed || contribute >= 0) {
	            if (contribute >= 0 && st->lastBet > 0) {
	                //分支TAG:S1
	            //如果已经在这级Pot里，并且该Seat对该级pot 已经作出UnitPrice的贡献，直接跳过
	            	if (p->priceFixed == true || chipsToPot == 0) continue;
	            }else if (contribute < 0 && p->priceFixed) {
	                //分支TAG:S2
	                int i = ((int)st->chipsAtHand)+contribute;
	                if (i>=0) { //这是要补足的情况
	                    //要补足本级Pot，分两种情况，一是该Seat有足够筹码，二是该Seat无足够筹码，又重新分Pot
	                    p->chipsInPot = p->chipsInPot-contribute;
	                    st->chipsAtHand = st->chipsAtHand+contribute;
	                    st->lastBet = st->lastBet-contribute;
	                    continue;
	                }
	            }
	        }
	        if (abs(chipsToPot) < p->unitPrice) { //将要Bet比前面少的情况可能是本Seat要Allin，因此重新分Pot
//LY_LOG_DBG("for debug, unitPrice=" << p->unitPrice << ", chipsToPot=" << chipsToPot);
//std::cout << "for debug, unitPrice=" << p->unitPrice << ", chipsToPot=" << chipsToPot << std::endl;
//分支TAG:L
	            LYPotPtr newPot = LYPotPtr(new LYPot());
	        	newPot->potLevel = (LYPotLevel)(this->topPot()->potLevel + 1);
	            if (chipsToPot < 0) {
	                //分支TAG:L1
	                newPot->unitPrice = abs(chipsToPot);
	            } else {
	                //分支TAG:L2
	                newPot->unitPrice = p->unitPrice - chipsToPot;
	                if (p->priceFixed) {
	                	newPot->priceFixed = true;
	                }
	                if (chips >= (st->chipsAtHand+st->lastBet) && potIndex == 0) {
	                    //分支TAG:L21
	                	//第一次的时候需要扣减lastBet
	                	//this seat allin, chipsToPot only have chipsAtHand
	                	//if potIndex>0, chipsToPot already have minus st->lastBet, for basePrice includes st->lastBet
	                	newPot->unitPrice -= st->lastBet;
	                }
	            }
//	            LY_LOG_DBG("newPot unitPrice=" << newPot->unitPrice);
	        	std::vector<LYApplicant>::iterator it2 = p->seatsInPot.begin();
	        	for(; it2 != p->seatsInPot.end(); it2++) {
	        		LYApplicant sn = *it2;
	            	LYSeatPtr s = game->fetchSeat(sn);
	                if (s->lastBet-(basePrice-p->unitPrice)<p->unitPrice-newPot->unitPrice) continue;
	                newPot->addSeat(s);
	                newPot->chipsInPot += newPot->unitPrice;
	                p->chipsInPot -= newPot->unitPrice;;
	            }
	            pots.insert(pots.begin()+potIndex+1, newPot);
	            p->priceFixed = true;
	            p->unitPrice = p->unitPrice-newPot->unitPrice;
	            if (!sit) { //说明该Seat是第一次Bet，不在pot里
	                //分支TAG:L3
	                p->addSeat(st);
	                p->chipsInPot += p->unitPrice; //把该Seat加进来
	                st->lastBet += p->unitPrice;
	                st->chipsAtHand = 0;
	            } else if (contribute < 0) { //说明该Seat是第二次Bet，并且需要补足的情况
	                //分支TAG:L4
//	            	LY_LOG_DBG("for debug, chipsToPot=" << chipsToPot << ", chipsInPot=" << p->chipsInPot << ", unitPrice=" << p->unitPrice);
	            	p->chipsInPot += st->chipsAtHand; //把该Seat加进来
	                st->lastBet += st->chipsAtHand;
	                st->chipsAtHand = 0;
	            }
	            return;
	        }
	        if (abs(chipsToPot) == p->unitPrice) { //刚好Bet
	            //分支TAG:E
	            if (!sit) { //说明该Seat是第一次Bet，不在pot里
	                //分支TAG:E1
	                p->addSeat(st);
	                p->chipsInPot += p->unitPrice; //把该Seat加进来
	                st->lastBet += p->unitPrice;
	                st->chipsAtHand -= p->unitPrice;
	            } else if (contribute < 0) { //说明该Seat是第二次Bet，并且需要补足的情况
//LY_LOG_DBG("for debug, contribute=" << contribute);
//分支TAG:E2
	                p->chipsInPot += abs(contribute); //把该Seat加进来
	                st->lastBet += abs(contribute);
	                st->chipsAtHand -= abs(contribute);
	            }
	            if (st->chipsAtHand == 0) {
	                p->priceFixed = true;
	            }
	            return;
	        }
	        if (abs(chipsToPot) > p->unitPrice) { //本Seat Raise or Allin
	            //分支TAG:G
	            if (p->priceFixed) { //本Pot已经有人Allin
	                //分支TAG:G1
	                if (!sit) { //说明该Seat是第一次Bet，不在pot里
	                    //分支TAG:G11
	                    p->addSeat(st);
	                    p->chipsInPot += p->unitPrice; //把该Seat加进来
	                    st->chipsAtHand -= p->unitPrice;
	                    st->lastBet += p->unitPrice;
	                }
	                if (potIndex < (int)pots.size()-1) continue;
	                LYPotPtr newPot = LYPotPtr(new LYPot());
	        		newPot->potLevel = (LYPotLevel)(this->topPot()->potLevel + 1);
	                newPot->chipsInPot = chipsToPot - p->unitPrice;
	                newPot->priceFixed = false;
	                newPot->unitPrice = chipsToPot - p->unitPrice;
	                st->chipsAtHand -= newPot->unitPrice;
	                st->lastBet += newPot->unitPrice;
	                if (st->chipsAtHand == 0) {
	                    //分支TAG:G12
	                    newPot->priceFixed = true;
	                }
	                newPot->addSeat(st);
	                pots.insert(pots.begin()+potIndex+1, newPot);
	                return;
	            } else { //该Pot未固定
//LY_LOG_DBG("chipsAtHand=" << st->chipsAtHand << ", chipsToPot=" << chipsToPot);
//分支TAG:G2
	                p->unitPrice = chipsToPot;
	                if (!sit) { //说明该Seat是第一次Bet，不在此Pot里
	                    //分支TAG:G21
	                    p->addSeat(st); //把该Seat加进来
	                } else {
//LY_LOG_DBG("for debug, chipsToPot=" << chipsToPot);
//分支TAG:G3
	                	if (st->chipsAtHand>chips && potIndex == 0) {
	                        //分支TAG:G31
	                		chipsToPot -= st->lastBet; //扣除已经Bet的，例如大小盲
	                	} else {
	                        //分支TAG:G32
	                		p->unitPrice = chipsToPot+st->lastBet;
	                	}
	                }
	                p->chipsInPot += chipsToPot;
	                st->chipsAtHand -= chipsToPot;
	                st->lastBet += chipsToPot;
//LY_LOG_DBG("unitPrice="<<p->unitPrice<<",chipsAtHand=" << st->chipsAtHand << ", lastBet=" << st->lastBet);
	                if (st->chipsAtHand == 0) {
	                    p->priceFixed = true;
	                }
	                return;
	            }
	            return;
	        }
	    }
	    //added 20130531
	    if (st->chipsAtHand > 0 && chipsToBet > st->lastBet) {
            LYPotPtr newPot = LYPotPtr(new LYPot());
	        newPot->potLevel = (LYPotLevel)(this->topPot()->potLevel + 1);
            newPot->unitPrice = chipsToBet - st->lastBet;
            newPot->chipsInPot = chipsToBet - st->lastBet;
            newPot->priceFixed = false;
            newPot->addSeat(st);
            pots.push_back(newPot);
            st->chipsAtHand -= newPot->unitPrice;
            st->lastBet += newPot->unitPrice;
            if (st->chipsAtHand == 0) {
            	newPot->priceFixed = true;
            }
	    }
}

void LYPotManager::newRound()
{
	std::vector<LYPotPtr>::iterator it = pots.begin();
	for(; it != pots.end(); it++) {
		LYPotPtr p = *it;
        p->potOpen = false;
        p->unitPrice = 0;
    	std::vector<LYApplicant>::iterator it2 = p->seatsInPot.begin();
    	for(; it2 != p->seatsInPot.end(); it2++) {
    		LYApplicant sn = *it2;
        	LYSeatPtr st = game->fetchSeat(sn);
            st->lastBet = 0;
        }
    }

    LYPotPtr lastPot = pots.back();
    lastPot->potOpen = true;
}

LYSeatPtr LYPotManager::findSeatInPot(LYSeatPtr st, LYPotPtr pot)
{
    if (!pot || NULL == st.get()) return LYSeatPtr();

	std::vector<LYApplicant>::iterator it = pot->seatsInPot.begin();
	for(; it != pot->seatsInPot.end(); it++) {
		LYApplicant ss = *it;
        if (ss == st->seatNo) {
            return st;
        }
    }
    return LYSeatPtr();
}

void LYPotManager::fold(LYApplicant seat)
{
	std::vector<LYPotPtr>::iterator it = pots.begin();
	for (; it!=pots.end(); it++) {
		LYPotPtr pot = *it;
		pot->fold(seat);
	}
}

void LYPotManager::reset()
{
	std::vector<LYPotPtr>::iterator it = pots.begin();
	for(; it != pots.end(); it++) {
		LYPotPtr pot = *it;
		pot->reset();
	}
	pots.clear();
}

unsigned int LYPotManager::getChipsIfWin(enum LYApplicant player)
{
	unsigned int chips = 0;
	std::vector<LYPotPtr>::iterator it = pots.begin();
	for(; it != pots.end(); it++) {
		LYPotPtr pot = *it;
		if (pot->haveSeat(player)) {
			chips += pot->chipsInPot;
		}
	}
	return chips;
}

LYPotPtr& LYPotManager::topPot()
{
	return pots.back();
}

 //added 20160310
/*
 * 根据传过来的kvps创建pot
 */
void LYPotManager::buildPot(std::vector<std::pair<std::string, std::string>>& kvps)
{
	LYPot *pot = new LYPot(kvps);
	LYPotPtr p = fetchPot(pot->potLevel);
	if (p.get() != NULL) {
		p->setPot(pot);
		delete pot;
		return;
	}

	addPot(pot);
}

void LYPotManager::buyAssurance(unsigned int round, LYSeatPtr st, unsigned int assurance, const std::string& pot)
{
	LYPotLevel level = (LYPotLevel)std::stoi(pot);
	if (!(level < 10 && level > 0)) {
		return; //invalid pot level
	}
	LYPotPtr p = fetchPot(level);
	p->buyAssurance((LYGameRound)round, st, assurance);
}
