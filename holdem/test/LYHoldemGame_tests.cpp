/*
 * LYHoldemGame_tests.cpp
 *
 *  Created on: 2013-7-6
 *      Author: caiqingfeng
 */

#include "src/LYHoldemGame.h"
#include "src/LYHoldemTable.h"
#include "src/LYHoldemTrunk.h"
//#include "common/src/my_log.h"
#include "src/LYHoldemActionView.h"
#include <string>

#include <gtest/gtest.h>

using std::string;

class LYHoldemGame_tests : public ::testing::Test
{
protected:

    void SetUp()
    {
    	table = new LYHoldemTable(string("this_is_table_id"), 9, LYTableOpen, string("this_is_table_name"));
    	table->openTrunk("999", "trunk_name", "player01", "888");
    	trunk = (LYHoldemTrunk *)table->getTrunk();
    	table->takeSeat(LYSeat1, 1000, "player01");
    	table->takeSeat(LYSeat2, 1000, "player02");
    	trunk->createGame("", NULL);
    	game = (LYHoldemGame *)trunk->getCurrentGame();

    	init_seats();

    }
    void TearDown()
    {
    	delete table;
    }

    void init_seats()
    {
    	st1 = table->fetchSeat(LYSeat1);
    	seat1 = (LYHoldemSeat *)st1.get();
    	st2 = table->fetchSeat(LYSeat2);
    	seat2 = (LYHoldemSeat *)st2.get();
    	st3 = table->fetchSeat(LYSeat3);
    	seat3 = (LYHoldemSeat *)st3.get();
    	st4 = table->fetchSeat(LYSeat4);
    	seat4 = (LYHoldemSeat *)st4.get();
    	st5 = table->fetchSeat(LYSeat5);
    	seat5 = (LYHoldemSeat *)st5.get();
    	st6 = table->fetchSeat(LYSeat6);
    	seat6 = (LYHoldemSeat *)st6.get();
    	st7 = table->fetchSeat(LYSeat7);
    	seat7 = (LYHoldemSeat *)st7.get();
    	st8 = table->fetchSeat(LYSeat8);
    	seat8 = (LYHoldemSeat *)st8.get();
    	st9 = table->fetchSeat(LYSeat9);
    	seat9 = (LYHoldemSeat *)st9.get();
    }
public:
    LYHoldemTable *table;
    LYHoldemTrunk  *trunk;
    LYHoldemGame *game;
    LYSeatPtr st1, st2, st3, st4, st5, st6, st7, st8, st9;
    LYHoldemSeat *seat1, *seat2, *seat3, *seat4, *seat5, *seat6, *seat7, *seat8, *seat9;
};

TEST_F(LYHoldemGame_tests, init)
{
    ASSERT_EQ(game->seatsOfTable.size(), 9);
    ASSERT_EQ(game->sortedSeats.size(), 2);
}

TEST_F(LYHoldemGame_tests, onCall)
{
	LYSeatPtr sorted_st1 = game->sortedSeats.back();
	LYSeatPtr sorted_st2 = game->sortedSeats.front();
	ASSERT_EQ(sorted_st1->status, LYSeatInGame);
	ASSERT_EQ(sorted_st2->status, LYSeatInGame);
	ASSERT_EQ(sorted_st1->lastBet, 10);
	ASSERT_EQ(game->getMaxChipsLeftOfOthers(sorted_st1->seatNo), 1000);
	ASSERT_EQ(sorted_st2->lastBet, 25);
	ASSERT_EQ(game->getMaxChipsLeftOfOthers(sorted_st2->seatNo), 1000);
	LYPotPtr mainPot = game->mainPot();
	ASSERT_EQ(mainPot->chipsInPot, 35);
	game->onCall(st1, 25);
	game->onCall(st2, 25);
	ASSERT_EQ(st1->lastBet, 25);
	ASSERT_EQ(seat1->gameStatus, LYSeatHoldemInCalled);
	ASSERT_EQ(st2->lastBet, 25);
	ASSERT_EQ(seat2->gameStatus, LYSeatHoldemInCalled);
	ASSERT_EQ(mainPot->chipsInPot, game->pm.mainPot->chipsInPot);
	ASSERT_EQ(mainPot->chipsInPot, 50);
}

TEST_F(LYHoldemGame_tests, onAction)
{
	LYSeatPtr sorted_st1 = game->sortedSeats.back();
	LYSeatPtr sorted_st2 = game->sortedSeats.front();
    ASSERT_EQ(game->sortedSeats.size(), 2);
	LYPotPtr mainPot = game->mainPot();
	LYHoldemAction action;
	action.pollingDecision(sorted_st1->seatNo, game->bigBlindPrice);
	bool validAction = game->onAction(action);
	ASSERT_EQ(validAction, true);
	action.call(sorted_st1->seatNo, game->bigBlindPrice);
    ASSERT_EQ(game->sortedSeats.size(), 2);
	validAction = game->onAction(action);
	ASSERT_EQ(validAction, true);
	ASSERT_EQ(mainPot->chipsInPot, game->pm.mainPot->chipsInPot);
	ASSERT_EQ(mainPot->chipsInPot, 50);
	ASSERT_EQ(game->continueThisRound, LYHoldemRoundCont);
}

TEST_F(LYHoldemGame_tests, 2p_allin)
{
	LYSeatPtr sorted_st1 = game->sortedSeats.back();
	LYSeatPtr sorted_st2 = game->sortedSeats.front();
    ASSERT_EQ(game->sortedSeats.size(), 2);
	LYPotPtr mainPot = game->mainPot();
	LYHoldemAction action;
	action.pollingDecision(sorted_st1->seatNo, game->bigBlindPrice);
	bool validAction = game->onAction(action);
	ASSERT_EQ(validAction, true);
	action.allin(sorted_st1->seatNo);
	validAction = game->onAction(action);
	ASSERT_EQ(validAction, true);
	ASSERT_EQ(game->continueThisRound, LYHoldemRoundCont);
	ASSERT_EQ(mainPot->chipsInPot, 1025);

	game->dutyMoveToNextSeat();
	action.pollingDecision(sorted_st2->seatNo, game->betIfCall);
	validAction = game->onAction(action);
	ASSERT_EQ(validAction, true);
	action.allin(sorted_st2->seatNo);
	validAction = game->onAction(action);
	ASSERT_EQ(validAction, true);
	ASSERT_EQ(game->continueThisRound, LYHoldemRoundDown);
	ASSERT_EQ(mainPot->chipsInPot, 2000);
}

TEST_F(LYHoldemGame_tests, gameActionView)
{
	LYSeatPtr sorted_st1 = game->sortedSeats.back();
	LYSeatPtr sorted_st2 = game->sortedSeats.front();
    ASSERT_EQ(game->sortedSeats.size(), 2);
    ASSERT_EQ(game->round, LYGamePreflop);
	LYPotPtr mainPot = game->mainPot();
	LYHoldemAction action;
	action.pollingDecision(sorted_st1->seatNo, game->bigBlindPrice);
	bool validAction = game->onAction(action);
	ASSERT_EQ(validAction, true);
	action.call(sorted_st1->seatNo, game->bigBlindPrice);
	validAction = game->onAction(action);

	game->dutyMoveToNextSeat();
	LYHoldemActionView actionView;
	game->genActionView(sorted_st1->seatNo, actionView);
//	std::cout << actionView.toString();
	ASSERT_EQ(actionView.onDuty, false);
	ASSERT_EQ(actionView.allinIfCall, false);
	ASSERT_EQ(2*game->bigBlindPrice, actionView.raiseMin);
	ASSERT_EQ(((LYHoldemSeat *)sorted_st1.get())->getChipsLeft(), actionView.raiseMax);

	LYHoldemActionView actionView2;
	game->genActionView(sorted_st2->seatNo, actionView2);
//	std::cout << actionView2.toString();
	ASSERT_EQ(actionView2.onDuty, true);
	ASSERT_EQ(actionView2.allinIfCall, false);
	ASSERT_EQ(2*game->bigBlindPrice, actionView2.raiseMin);
	ASSERT_EQ(((LYHoldemSeat *)sorted_st2.get())->getChipsLeft(), actionView2.raiseMax);

	//大盲Call，Preflop结束
	LYHoldemAction action2;
	action2.pollingDecision(sorted_st2->seatNo, game->bigBlindPrice);
	validAction = game->onAction(action2);
	ASSERT_EQ(validAction, true);
	action2.call(sorted_st2->seatNo, game->bigBlindPrice);
	validAction = game->onAction(action2);

	//应该下一轮
	ASSERT_EQ(game->continueThisRound, LYHoldemRoundNext);
	game->onAction(game->flop_action);
	ASSERT_EQ(game->dutySeatNo, sorted_st2->seatNo);
	ASSERT_EQ(game->betIfCall, 0);
	ASSERT_EQ(game->minBetIfRaise, game->bigBlindPrice);
    ASSERT_EQ(game->round, LYGameFlop);
    ASSERT_EQ(game->continueThisRound, LYHoldemRoundCont);

	LYHoldemActionView actionView3;
	game->genActionView(sorted_st2->seatNo, actionView3);
//	std::cout << actionView3.toString();
	ASSERT_EQ(actionView3.onDuty, true);
	ASSERT_EQ(actionView3.allinIfCall, false);
	ASSERT_EQ(game->bigBlindPrice, actionView3.raiseMin);
	ASSERT_EQ(((LYHoldemSeat *)sorted_st1.get())->getChipsLeft(), actionView3.raiseMax);

	LYHoldemAction action5;
	action5.pollingDecision(sorted_st2->seatNo, game->bigBlindPrice);
	validAction = game->onAction(action5);
	ASSERT_EQ(validAction, true);
//std::cout << "before calling raise" << std::endl;
	action5.raise(sorted_st2->seatNo, game->bigBlindPrice);
	validAction = game->onAction(action5);
    ASSERT_EQ(game->round, LYGameFlop);
	ASSERT_EQ(game->continueThisRound, LYHoldemRoundCont);
	ASSERT_EQ(validAction, true);
	ASSERT_EQ(game->dutySeatNo, sorted_st2->seatNo);
	ASSERT_EQ(game->betIfCall, game->bigBlindPrice);
	ASSERT_EQ(game->minBetIfRaise, 2*game->bigBlindPrice);
	LYHoldemActionView actionView5;
	game->genActionView(sorted_st2->seatNo, actionView5);
//	std::cout << actionView5.toString();
	ASSERT_EQ(actionView5.onDuty, true);
	ASSERT_EQ(actionView5.allinIfCall, false);
	ASSERT_EQ(2*game->bigBlindPrice, actionView5.raiseMin);
	ASSERT_EQ(((LYHoldemSeat *)sorted_st2.get())->getChipsLeft(), actionView5.raiseMax);

	game->dutyMoveToNextSeat();
	ASSERT_EQ(game->dutySeatNo, sorted_st1->seatNo);
	LYHoldemActionView actionView6;
	game->genActionView(sorted_st1->seatNo, actionView6);
	//std::cout << actionView6.toString();
	ASSERT_EQ(actionView6.onDuty, true);
	ASSERT_EQ(actionView6.allinIfCall, false);
	ASSERT_EQ(2*game->bigBlindPrice, actionView6.raiseMin);
	ASSERT_EQ(((LYHoldemSeat *)sorted_st1.get())->getChipsLeft(), actionView6.raiseMax);
}

TEST_F(LYHoldemGame_tests, gameActionView2)
{
	LYSeatPtr sorted_st1 = game->sortedSeats.back();
	LYSeatPtr sorted_st2 = game->sortedSeats.front();
	sorted_st1->chipsAtHand = 1000;
	sorted_st2->chipsAtHand = 500;
	LYHoldemActionView actionView;
	game->genActionView(sorted_st1->seatNo, actionView);
	ASSERT_EQ(actionView.onDuty, true);
	ASSERT_EQ(actionView.allinIfCall, false);
	ASSERT_EQ(2*game->bigBlindPrice, actionView.raiseMin);
	ASSERT_EQ(actionView.raiseMax, 525);

	LYHoldemActionView actionView2;
	game->genActionView(sorted_st2->seatNo, actionView2);
	ASSERT_EQ(actionView2.onDuty, false);
	ASSERT_EQ(actionView2.allinIfCall, false);
	ASSERT_EQ(2*game->bigBlindPrice, actionView2.raiseMin);
	ASSERT_EQ(actionView2.raiseMax, 525);
}

TEST_F(LYHoldemGame_tests, caculateSeatRanking)
{
	game->round = LYGameRiver;
	game->flopCards.clear();
	game->flopCards.push_back(sA);
	game->flopCards.push_back(hA);
	game->flopCards.push_back(sK);
	game->turnCard = d8;
	game->riverCard = s9;

	seat1->setFirst(s7);
	seat1->setSecond(s2);
	seat2->setFirst(dA);
	seat2->setSecond(c8);

	ASSERT_EQ(game->caculateSeatRanking(LYSeat1), Flush);
	ASSERT_EQ(game->caculateSeatRanking(LYSeat2), FullHouse);
//	std::cout << game->toString() << std::endl;
}

TEST_F(LYHoldemGame_tests, big_leave_seat_to_move_duty)
{
	LYSeatPtr sorted_st1 = game->sortedSeats.back();
	LYSeatPtr sorted_st2 = game->sortedSeats.front();
    ASSERT_EQ(game->sortedSeats.size(), 2);
	LYHoldemAction action;
	action.pollingDecision(sorted_st1->seatNo, game->bigBlindPrice);
	bool validAction = game->onAction(action);
	ASSERT_EQ(validAction, true);
	action.fold(sorted_st2->seatNo, "byleave");
	ASSERT_EQ(game->continueThisRound, LYHoldemRoundCont);
	validAction = game->onAction(action);
	ASSERT_EQ(validAction, true);
	ASSERT_EQ(game->continueThisRound, LYHoldemRoundEnd);

	ASSERT_EQ(game->dutySeatNo, sorted_st1->seatNo);
	//game->dutyMoveToNextSeat(); // not allowed
	ASSERT_EQ(game->dutySeatNo, sorted_st1->seatNo);
}

TEST_F(LYHoldemGame_tests, small_leave_seat_to_move_duty)
{
	LYSeatPtr sorted_st1 = game->sortedSeats.back();
	LYSeatPtr sorted_st2 = game->sortedSeats.front();
    ASSERT_EQ(game->sortedSeats.size(), 2);
	LYHoldemAction action;
	action.pollingDecision(sorted_st1->seatNo, game->bigBlindPrice);
	bool validAction = game->onAction(action);
	ASSERT_EQ(validAction, true);
	action.fold(sorted_st1->seatNo, "byleave");
	ASSERT_EQ(game->continueThisRound, LYHoldemRoundCont);
	validAction = game->onAction(action);
	ASSERT_EQ(validAction, true);
	ASSERT_EQ(game->continueThisRound, LYHoldemRoundEnd);

	ASSERT_EQ(game->dutySeatNo, sorted_st1->seatNo);
	game->dutyMoveToNextSeat();
	ASSERT_EQ(game->dutySeatNo, sorted_st2->seatNo);
}

TEST_F(LYHoldemGame_tests, prepare_match_result)
{
	LYHoldemAlgorithmDelegate had;
	std::vector<LYCard> cards;
	LYCardHelpers::stringToCards("8s,9d,Ts,", cards);
	game->onFlop(cards);
	game->pad = (LYHoldemAlgorithmDelegate *)&had;

	st1->handCards.clear();
	st1->handCards.push_back(sA);
	st1->handCards.push_back(sK);
	st1->status = LYSeatInGame;
	seat1->gameStatus = LYSeatHoldemInAllin;
	st2->handCards.clear();
	st2->handCards.push_back(dQ);
	st2->handCards.push_back(hQ);
	st2->status = LYSeatInGame;
	seat2->gameStatus = LYSeatHoldemInAllin;
//	st3->handCards.push_back(d6);
//	st3->handCards.push_back(d7);

	LYPotPtr pot = game->mainPot();
	pot->prepareMatchResult(game);
	LYHandStrength *hs = pot->winningHand;
	ASSERT_EQ(hs!=NULL, true);
//	std::cout << hs->cardString << std::endl;
	ASSERT_EQ(hs->ranking, OnePair);
	pot->caculateOuts(game);
	ASSERT_EQ(pot->chasingOuts.size(), 6+9);
}

TEST_F(LYHoldemGame_tests, gen_assure_actions)
{
	LYHoldemAlgorithmDelegate had;
	std::vector<LYCard> cards;
	LYCardHelpers::stringifiedIntToCards("17,23,42,", cards);
	game->onFlop(cards);
	game->pad = (LYHoldemAlgorithmDelegate *)&had;

	st1->handCards.clear();
	LYCardHelpers::stringifiedIntToCards("4,44,", cards);
	st1->handCards = cards;
	st1->status = LYSeatInGame;
	seat1->gameStatus = LYSeatHoldemInAllin;
	st2->handCards.clear();
	LYCardHelpers::stringifiedIntToCards("14,10,", cards);
	st2->handCards = cards;
	st2->status = LYSeatInGame;
	seat2->gameStatus = LYSeatHoldemInAllin;
//	st3->handCards.push_back(d6);
//	st3->handCards.push_back(d7);

	LYPotPtr pot = game->mainPot();
	pot->prepareMatchResult(game);
	LYHandStrength *hs = pot->winningHand;
	ASSERT_EQ(hs!=NULL, true);
//	std::cout << hs->cardString << std::endl;
//	std::cout << seat2->handStrength->cardString << std::endl;
	ASSERT_EQ(hs->ranking, OnePair);
	pot->caculateOuts(game);
	ASSERT_EQ(pot->chasingOuts.size(), 6+8);

	std::vector<LYHoldemAction> actions;
	game->genAssureActions(actions);
	game->dutySeatNo = LYHouse;
	ASSERT_EQ(game->finishAssurance(), false);
	ASSERT_EQ(actions.size(), 1);
	ASSERT_EQ(game->assureActionNumbers, 1);
	LYHoldemAction action(game->table->_id);
	action.assure(actions[0].applicant, 100);
	game->onAction(action);
	ASSERT_EQ(game->assureActionNumbers, 0);
	ASSERT_EQ(game->finishAssurance(), true);
}

TEST_F(LYHoldemGame_tests, gen_assure_actions2)
{
	LYHoldemAlgorithmDelegate had;
	std::vector<LYCard> cards;
	LYCardHelpers::stringifiedIntToCards("13,43,32,", cards);
	game->onFlop(cards);
	game->pad = (LYHoldemAlgorithmDelegate *)&had;

	st1->handCards.clear();
	LYCardHelpers::stringifiedIntToCards("48,9,", cards);
	st1->handCards = cards;
	st1->status = LYSeatInGame;
	seat1->gameStatus = LYSeatHoldemInAllin;
	st2->handCards.clear();
	LYCardHelpers::stringifiedIntToCards("11,10,", cards);
	st2->handCards = cards;
	st2->status = LYSeatInGame;
	seat2->gameStatus = LYSeatHoldemInAllin;
//	st3->handCards.push_back(d6);
//	st3->handCards.push_back(d7);

	LYPotPtr pot = game->mainPot();
	pot->prepareMatchResult(game);
	LYHandStrength *hs = pot->winningHand;
	ASSERT_EQ(hs!=NULL, true);
//	std::cout << seat1->handStrength->cardString << std::endl;
//	std::cout << seat2->handStrength->cardString << std::endl;
	ASSERT_EQ(hs->ranking, OnePair);
	pot->caculateOuts(game);
	ASSERT_EQ(pot->chasingOuts.size(), 0);

	std::vector<LYHoldemAction> actions;
	game->genAssureActions(actions);
	game->dutySeatNo = LYHouse;
	ASSERT_EQ(game->finishAssurance(), true);
	ASSERT_EQ(game->assureActionNumbers, 0);
	LYHoldemAction action(game->table->_id);
	action.assure(LYSeat2, 100);
	game->onAction(action);
	ASSERT_EQ(game->assureActionNumbers, 0);
	ASSERT_EQ(game->finishAssurance(), true);
}

TEST_F(LYHoldemGame_tests, gen_assure_actions4)
{
	LYHoldemAlgorithmDelegate had;
	std::vector<LYCard> cards;
	LYCardHelpers::stringifiedIntToCards("36,3,49,", cards);
	game->onFlop(cards);
	game->pad = (LYHoldemAlgorithmDelegate *)&had;

	st1->handCards.clear();
	LYCardHelpers::stringifiedIntToCards("51,16,", cards);
	st1->handCards = cards;
	st1->status = LYSeatInGame;
	seat1->gameStatus = LYSeatHoldemInAllin;
	st2->handCards.clear();
	LYCardHelpers::stringifiedIntToCards("20,24,", cards);
	st2->handCards = cards;
	st2->status = LYSeatInGame;
	seat2->gameStatus = LYSeatHoldemInAllin;
//	st3->handCards.push_back(d6);
//	st3->handCards.push_back(d7);

	LYPotPtr pot = game->mainPot();
	pot->prepareMatchResult(game);
	LYHandStrength *hs = pot->winningHand;
	ASSERT_EQ(hs!=NULL, true);
//	std::cout << seat1->handStrength->cardString << std::endl;
//	std::cout << seat2->handStrength->cardString << std::endl;
	ASSERT_EQ(hs->ranking, OnePair);
	pot->caculateOuts(game);
	ASSERT_EQ(pot->chasingOuts.size(), 6);
}

TEST_F(LYHoldemGame_tests, gen_assure_actions3)
{
	LYHoldemAlgorithmDelegate had;
	std::vector<LYCard> cards;
	LYCardHelpers::stringifiedIntToCards("22,39,10,", cards);
	game->onFlop(cards);
	game->pad = (LYHoldemAlgorithmDelegate *)&had;

	st1->handCards.clear();
	LYCardHelpers::stringifiedIntToCards("48,41,", cards);
	st1->handCards = cards;
	st1->status = LYSeatInGame;
	seat1->gameStatus = LYSeatHoldemInAllin;
	st2->handCards.clear();
	LYCardHelpers::stringifiedIntToCards("34,24,", cards);
	st2->handCards = cards;
	st2->status = LYSeatInGame;
	seat2->gameStatus = LYSeatHoldemInAllin;
//	st3->handCards.push_back(d6);
//	st3->handCards.push_back(d7);

	LYPotPtr pot = game->mainPot();
	pot->prepareMatchResult(game);
	LYHandStrength *hs = pot->winningHand;
	ASSERT_EQ(hs!=NULL, true);
//	std::cout << seat1->handStrength->cardString << std::endl;
//	std::cout << seat2->handStrength->cardString << std::endl;
	ASSERT_EQ(hs->ranking, OnePair);
	pot->caculateOuts(game);
	ASSERT_EQ(pot->chasingOuts.size(), 0);

	std::vector<LYHoldemAction> actions;
	game->genAssureActions(actions);
	game->dutySeatNo = LYHouse;
	ASSERT_EQ(game->finishAssurance(), true);
	ASSERT_EQ(game->assureActionNumbers, 0);
	LYHoldemAction action(game->table->_id);
	action.assure(LYSeat2, 100);
	game->onAction(action);
	ASSERT_EQ(game->assureActionNumbers, 0);
	ASSERT_EQ(game->finishAssurance(), true);
}

//20160412 added
TEST_F(LYHoldemGame_tests, isValidAction)
{
	LYHoldemAlgorithmDelegate had;
	game->pad = (LYHoldemAlgorithmDelegate *)&had;

	st1->handCards.clear();
	st1->handCards.push_back(sA);
	st1->handCards.push_back(sK);
	st1->status = LYSeatInGame;
	st1->chipsAtHand = 975;
	st1->lastBet = 25;
	seat1->gameStatus = LYSeatHoldemMakingDecision;
	st2->handCards.clear();
	st2->handCards.push_back(dQ);
	st2->handCards.push_back(hQ);
	st2->status = LYSeatInGame;
	st2->chipsAtHand = 950;
	st2->lastBet = 25;
	seat2->gameStatus = LYSeatHoldemBetBig;

	game->dutySeatNo = LYSeat1;
	game->round = LYGamePreflop;
	game->betIfCall = 25;
	LYHoldemAction action(table->_id);
	action.check(LYSeat1);
	ASSERT_EQ(game->isValidAction(action), true);
	action.fold(LYSeat1);
	ASSERT_EQ(game->isValidAction(action), true);
	action.call(LYSeat1, 50);
	ASSERT_EQ(game->isValidAction(action), true);
	action.doBet(LYSeat1, 100);
	ASSERT_EQ(game->isValidAction(action), true);
	action.raise(LYSeat1, 100);
	ASSERT_EQ(game->isValidAction(action), true);
	bool r = game->onAction(action);
	ASSERT_EQ(r, true);
}

//20160321 added
TEST_F(LYHoldemGame_tests, game_over)
{
	LYHoldemAlgorithmDelegate had;
	std::vector<LYCard> cards;
	LYCardHelpers::stringToCards("8s,9d,Ts,", cards);
	game->onFlop(cards);
	game->pad = (LYHoldemAlgorithmDelegate *)&had;

	st1->handCards.clear();
	st1->handCards.push_back(sA);
	st1->handCards.push_back(sK);
	st1->status = LYSeatInGame;
	st1->chipsAtHand = 950;
	seat1->gameStatus = LYSeatHoldemInAllin;
	st2->handCards.clear();
	st2->handCards.push_back(dQ);
	st2->handCards.push_back(hQ);
	st2->status = LYSeatInGame;
	st2->chipsAtHand = 950;
	seat2->gameStatus = LYSeatHoldemInAllin;

	LYPotPtr pot = game->mainPot();
	pot->chipsInPot = 100;
	game->gameOver();
	ASSERT_EQ(game->sortedSeats.size(), 2);
	ASSERT_EQ(game->totalSeatsInGame(), 2);
	std::vector<std::string> white_list = game->getWhiteList();
	ASSERT_EQ(white_list.size(), 2);

	ASSERT_EQ(pot->chasingOuts.size(), 0);
	ASSERT_EQ(st1->chipsAtHand, 950);
	ASSERT_EQ(st2->chipsAtHand, 1050);
	LYHandStrength *hs = pot->winningHand;
	ASSERT_EQ(hs!=NULL, true);
//	std::cout << hs->cardString << std::endl;
	ASSERT_EQ(hs->ranking, OnePair);
	pot->caculateOuts(game);
	ASSERT_EQ(pot->chasingOuts.size(), 6+9);
}

//20160321 added
TEST_F(LYHoldemGame_tests, game_over_with_assurance)
{
	LYHoldemAlgorithmDelegate had;
	std::vector<LYCard> cards;
	LYCardHelpers::stringToCards("8s,9d,Ts,", cards);
	game->onFlop(cards);
	game->pad = (LYHoldemAlgorithmDelegate *)&had;

	st1->handCards.clear();
	st1->handCards.push_back(sA);
	st1->handCards.push_back(sK);
	st1->status = LYSeatInGame;
	st1->chipsAtHand = 0;
	seat1->gameStatus = LYSeatHoldemInAllin;
	st2->handCards.clear();
	st2->handCards.push_back(dQ);
	st2->handCards.push_back(hQ);
	st2->status = LYSeatInGame;
	st2->chipsAtHand = 0;
	seat2->gameStatus = LYSeatHoldemInAllin;

	LYPotPtr pot = game->mainPot();
	pot->chipsInPot = 2000;
	pot->prepareMatchResult(game);

	pot->caculateOuts(game);
	pot->buyAssurance(LYGameFlop, st2, 100);
	pot->checkAssurance(LYGameFlop, cA);

	ASSERT_EQ(pot->chasingOuts.size(), 6+9);

	game->onTurn(cA);
	game->gameOver();

	ASSERT_EQ(st1->chipsAtHand, 2000);
	ASSERT_EQ(st2->chipsAtHand, 160);
	ASSERT_EQ(table->assurance_income, -160);

}

//20160321 added
TEST_F(LYHoldemGame_tests, game_over_with_double_assurance)
{
	LYHoldemAlgorithmDelegate had;
	std::vector<LYCard> cards;
	LYCardHelpers::stringToCards("8s,9d,Ts,", cards);
	game->onFlop(cards);
	game->pad = (LYHoldemAlgorithmDelegate *)&had;

	st1->handCards.clear();
	st1->handCards.push_back(sA);
	st1->handCards.push_back(sK);
	st1->status = LYSeatInGame;
	st1->chipsAtHand = 0;
	seat1->gameStatus = LYSeatHoldemInAllin;
	st2->handCards.clear();
	st2->handCards.push_back(dQ);
	st2->handCards.push_back(hQ);
	st2->status = LYSeatInGame;
	st2->chipsAtHand = 0;
	seat2->gameStatus = LYSeatHoldemInAllin;

	LYPotPtr pot = game->mainPot();
	pot->chipsInPot = 2000;
	pot->prepareMatchResult(game);
	pot->caculateOuts(game);
	pot->buyAssurance(LYGameFlop, st2, 100);
	pot->checkAssurance(LYGameFlop, cA);

	ASSERT_EQ(pot->chasingOuts.size(), 6+9);
	game->onTurn(cA);
	pot->prepareMatchResult(game);
	pot->caculateOuts(game);
	pot->buyAssurance(LYGameTurn, st1, 100);
	pot->checkAssurance(LYGameTurn, cQ);
	game->onRiver(cQ);
	std::vector<LYHoldemAction> actions;
	pot->makePaidAssuranceActions(game, cA, actions);
	ASSERT_EQ(actions.size(), 1);

	game->gameOver();

	ASSERT_EQ(st1->chipsAtHand, 750);
	ASSERT_EQ(st2->chipsAtHand, 2160);
	ASSERT_EQ(table->assurance_income, -160-750);

}

//20160321 added
TEST_F(LYHoldemGame_tests, game_over_with_double_assurance2)
{
	LYHoldemAlgorithmDelegate had;
	std::vector<LYCard> cards;
	LYCardHelpers::stringToCards("8s,9d,Ts,", cards);
	game->onFlop(cards);
	game->pad = (LYHoldemAlgorithmDelegate *)&had;

	st1->handCards.clear();
	st1->handCards.push_back(sA);
	st1->handCards.push_back(sK);
	st1->status = LYSeatInGame;
	st1->chipsAtHand = 0;
	seat1->gameStatus = LYSeatHoldemInAllin;
	st2->handCards.clear();
	st2->handCards.push_back(dQ);
	st2->handCards.push_back(hQ);
	st2->status = LYSeatInGame;
	st2->chipsAtHand = 0;
	seat2->gameStatus = LYSeatHoldemInAllin;

	std::vector<LYHoldemAction> actions;
	LYPotPtr pot = game->mainPot();
	pot->chipsInPot = 2000;
	pot->prepareMatchResult(game);
	pot->caculateOuts(game);
	pot->buyAssurance(LYGameFlop, st2, 100);
	pot->checkAssurance(LYGameFlop, c2);
	ASSERT_EQ(pot->chasingOuts.size(), 6+9);
	game->onTurn(c2);
//	game->debugGame();

	pot->makePaidAssuranceActions(game, c2, actions);
	ASSERT_EQ(actions.size(), 1);

	pot->prepareMatchResult(game);
	pot->caculateOuts(game);
//	pot->buyAssurance(LYGameTurn, st1, 100);
	actions.clear();
	pot->checkAssurance(LYGameTurn, cA);
	game->onRiver(cA);

	game->gameOver();
	pot->makePaidAssuranceActions(game, cA, actions);
	ASSERT_EQ(actions.size(), 0);

	ASSERT_EQ(st1->chipsAtHand, 2000);
	ASSERT_EQ(st2->chipsAtHand, -100);
	ASSERT_EQ(table->assurance_income, 100);

	ASSERT_EQ(game->assurance_summary_action.verb, LYSummarizeAssurance);
	ASSERT_EQ(game->assurance_summary_action.bet, 100);
	ASSERT_EQ(game->assurance_summary_action.desc, "0");

}
