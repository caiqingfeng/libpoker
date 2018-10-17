/*
 * LYHoldemGame.cpp
 *
 *  Created on: 2013-7-6
 *      Author: caiqingfeng
 */

#include "LYHoldemGame.h"
#include "LYHoldemTable.h"
#include "common/src/my_log.h"
#include <boost/foreach.hpp>
#include "LYHoldemAlgorithmDelegate.h"
#include <iostream>
#include "poker/src/LYCardHelpers.h"

void LYHoldemGame::reset()
{
	_id = "";
	gameStatus = LY_HOLDEMGAME_OVER;
	smallBlindPrice = 0;
	bigBlindPrice = 0;
	btnSeatNo = ::LYHouse;
	sbSeatNo = ::LYHouse;
	bbSeatNo = ::LYHouse;
	roundOfTournament = 0;

	seqNo = 0;
	round = LYGameInit;
	betIfCall = 0;
	minBetIfRaise = 0;
	seqNo = 0;
	dutySeatNo = ::LYHouse;
	continueThisRound = LYHoldemRoundCont;
	flopCards.clear();
	turnCard = flipCard;
	riverCard = flipCard;

	flop_action.table_id = table->_id;
	turn_action.table_id = table->_id;
	river_action.table_id = table->_id;
	assurance_summary_action.table_id = table->_id;
	assurance_summary_action.summarizeAssure(0, table->assurance_income);

	assureActionNumbers = 0;
	assureActions.clear();
}
/*
 * 初始化，本构造函数只能用于反序列化，如从DB或者NW中收到Scene，先创建一个空的Game实例，然后再调用赋值
 * 仅用于从NW中收到Scene，之所以不在构造中传参，是因为Protobuf比较大，并且跟具体项目关系比较大，因此不从构造中设置
 */
LYHoldemGame::LYHoldemGame(const std::string &game_id, LYHoldemAlgorithmDelegate *had)
	:LYGame((LYPokerAlgorithmDelegate *)had), pm(this)
{
	this->reset();
	_id = game_id;
}

/*
 * 初始化，本构造函数用于反序列化，如从DB或者NW中收到Scene，先创建一个空的Game实例，然后再赋值
 */
LYHoldemGame::LYHoldemGame(const std::string &game_id,
		std::vector<LYSeatPtr> &all_seats, LYHoldemTable *tbl, LYHoldemAlgorithmDelegate *had) :
				LYGame(game_id, all_seats, (LYTable *)tbl, (LYPokerAlgorithmDelegate *)had), pm(this)
{
	this->reset();
	_id = game_id;

	sortSeats();
}

/*
 * 初始化，本构造函数用于反序列化，仅用于从DB中恢复Table/Game，直接构造函数传参
 * 20160320
 */
LYHoldemGame::LYHoldemGame(const std::string &game_id,
		std::vector<LYSeatPtr> &all_seats, LYHoldemTable *tbl,
		std::vector < std::pair<std::string, std::string> >& kvps,
		LYHoldemAlgorithmDelegate *had) :
				LYGame(game_id, all_seats, (LYTable *)tbl, (LYPokerAlgorithmDelegate *)had), pm(this)
{
	this->reset();
	_id = game_id;

	std::vector < std::pair<std::string, std::string> >::iterator it = kvps.begin();
	for (; it!=kvps.end(); it++) {
		std::pair<std::string, std::string> kvp = *it;
		if (kvp.first == "gameStatus") {
			this->gameStatus = std::stoi(kvp.second);
		} else if (kvp.first == "_id") {
			this->_id = kvp.second;
		} else if (kvp.first == "roundOfTournament") {
			this->roundOfTournament = std::stoi(kvp.second);
		} else if (kvp.first == "btnSeatNo") {
			this->btnSeatNo = (LYApplicant)std::stoi(kvp.second);
		} else if (kvp.first == "sbSeatNo") {
			this->sbSeatNo = (LYApplicant)std::stoi(kvp.second);
		} else if (kvp.first == "bbSeatNo") {
			this->bbSeatNo = (LYApplicant)std::stoi(kvp.second);
		} else if (kvp.first == "flopCards") {
			LYCardHelpers::stringifiedIntToCards(kvp.second, this->flopCards);
		} else if (kvp.first == "turnCard") {
			this->turnCard = LYCard(std::stoi(kvp.second));
		} else if (kvp.first == "riverCard") {
			this->riverCard = LYCard(std::stoi(kvp.second));
		} else if (kvp.first == "smallBlindPrice") {
			this->smallBlindPrice = std::stoi(kvp.second);
		} else if (kvp.first == "bigBlindPrice") {
			this->bigBlindPrice = std::stoi(kvp.second);
		} else if (kvp.first == "seqNo") {
			this->seqNo = std::stoi(kvp.second);
		} else if (kvp.first == "round") {
			this->round = (LYGameRound)std::stoi(kvp.second);
		} else if (kvp.first == "dutySeatNo") {
			this->dutySeatNo = (LYApplicant)std::stoi(kvp.second);
		} else if (kvp.first == "betIfCall") {
			this->betIfCall = std::stoi(kvp.second);
		} else if (kvp.first == "minBetIfRaise") {
			this->minBetIfRaise = std::stoi(kvp.second);
		} else if (kvp.first == "betBySeatNo") {
			this->betBySeatNo = (LYApplicant)std::stoi(kvp.second);
		} else if (kvp.first == "continueThisRound") {
			this->continueThisRound = (LYHoldemRoundContinue)(std::stoi(kvp.second));
		} else if (kvp.first == "assureActionNumbers") {
			this->assureActionNumbers = std::stoi(kvp.second);
		} else if (kvp.first == "lastPollingAt") {
			this->lastPollingAt = std::stoi(kvp.second);
		} else if (kvp.first == "lastPollingAction") {
			this->lastPollingAction = LYHoldemAction::build_from_action_string(kvp.second);
		}
	}

	sortSeatsWithoutChangeStatus();
	reverseCommunityCardActions();
	this->appendLog(this->toString());
}

LYHoldemGame::LYHoldemGame(const std::string &game_id,
		std::vector<LYSeatPtr> &all_seats, unsigned int sbp, unsigned int bbp,
		LYSeatPtr btnSeat, LYSeatPtr sbSeat, LYSeatPtr bbSeat,
		LYHoldemTable *tbl, LYHoldemAlgorithmDelegate *had, unsigned int rot) :
				LYGame(game_id, all_seats, (LYTable *)tbl, (LYPokerAlgorithmDelegate *)had), pm(this)
{
	this->reset();
	_id = game_id;
	// TODO Auto-generated constructor stub
	smallBlindPrice = sbp;
	bigBlindPrice = bbp;
	btnSeatNo = btnSeat->seatNo;
	sbSeatNo = sbSeat->seatNo;
	bbSeatNo = bbSeat->seatNo;
	roundOfTournament = rot;
//	LY_LOG_DBG("small="<<smallBlindPrice << ", big="<<bigBlindPrice);

	sortSeats();

	//init dynamics
	round = LYGameInit;
	betIfCall = 0;
	minBetIfRaise = 0;
	seqNo = 0;
	dutySeatNo = sbSeatNo;
	continueThisRound = LYHoldemRoundCont;
	LYHoldemAction action;
	action.betSmallBlind(sbSeatNo, smallBlindPrice);
	this->onAction(action);
//	seqNo++;
	dutySeatNo = bbSeatNo;
	action.betBigBlind(bbSeatNo, bigBlindPrice);
	this->onAction(action);
	//shuffle and set cards to seats
//    this->deck.shuffle();
	//set pocket cards
    std::vector<LYSeatPtr>::iterator it = sortedSeats.begin();
    for (; it!=sortedSeats.end(); it++) {
    	LYSeatPtr seat = *it;
		LYHoldemSeat *hs = (LYHoldemSeat *)seat.get();
		LYCard card = deck.pop();
//    	LY_LOG_DBG("first handcard:" << card.toInteger());
//		hs->setFirstCard(card);
		hs->setFirst(card);
//    	LY_LOG_DBG("first handcard:" << hs->firstCard().toInteger() << ", seat=" << hs->seatNo);
	}
    it = sortedSeats.begin();
    for (; it!=sortedSeats.end(); it++) {
    	LYSeatPtr seat = *it;
		LYHoldemSeat *hs = (LYHoldemSeat *)seat.get();
		LYCard card = deck.pop();
//    	LY_LOG_DBG("second handcard:" << card.toInteger());
//		hs->setSecondCard(card);
		hs->setSecond(card);
//    	LY_LOG_DBG("second handcard:" << card.toInteger() << ", seat=" << hs->seatNo);
	}
	//设置Community Actions
	this->genCommunityCardActions();

	//set dynamics
	betIfCall = bigBlindPrice;
	minBetIfRaise = bigBlindPrice * 2;
	seqNo = 0;
	dutySeatNo = this->fetchNextSeatInGame(bbSeatNo)->seatNo;
	round = LYGamePreflop;

	this->genNewScene();
	this->appendLog(this->toString());
}

LYHoldemGame::~LYHoldemGame() {
	// TODO Auto-generated destructor stub
	scenes.clear();
}

enum LYGameRound LYHoldemGame::getRound()
{
	return round;
}

LYHoldemScenePtr LYHoldemGame::getLastScene()
{
	if (scenes.size() < 1) return LYHoldemScenePtr();
	return scenes.back();
}

/*
 * sortSeats: sort all seats according to small_blind, big_blind, ..., button
 */
void LYHoldemGame::sortSeats()
{
    this->sortedSeats.clear();
	if (::LYHouse == this->sbSeatNo || ::LYHouse == this->btnSeatNo || ::LYHouse == this->bbSeatNo || this->seatsOfTable.size() < 2) return;
    LYSeatPtr seat = this->smallBlindSeat();
    if (table->occupied_seat_number == 2) seat = this->bigBlindSeat();

	int i = 0;
    while (seat->seatNo != this->btnSeatNo && i < 9) {
		i++;
    	seat->status = LYSeatInGame;
    	LYHoldemSeat *hs = (LYHoldemSeat *)seat.get();
    	hs->gameStatus = LYSeatHoldemWaiting;
    	sortedSeats.push_back(seat);
    	seat = table->fetchNextOccupiedSeat(seat->seatNo);
    	if (NULL == seat.get()) break;
    }
    buttonSeat()->status = LYSeatInGame;
	LYHoldemSeat *hs = (LYHoldemSeat *)buttonSeat().get();
	hs->gameStatus = LYSeatHoldemWaiting;
    sortedSeats.push_back(this->buttonSeat());
}

void LYHoldemGame::debugGame()
{
	std::cout << toString() << std::endl;
//	for (int i=0; i<table->seats.size(); i++) {
//		std::cout << ((LYHoldemSeat *)table->seats[i].get())->toString() << std::endl;
//	}
}

std::string LYHoldemGame::toString()
{
    std::string str;
    std::string cards;
    LYCardHelpers::cardsToStringifiedInt(flopCards, cards);
    str = "{";
    str =   str + "\"_id\":\"" + _id + "\"," +
			"\"gameStatus\":" + std::to_string(gameStatus) + "," +
    		"\"roundOfTournament\":" + std::to_string(roundOfTournament) + "," +
    		"\"btnSeatNo\":" + std::to_string(btnSeatNo) + "," +
    		"\"sbSeatNo\":" + std::to_string(sbSeatNo) + "," +
    		"\"bbSeatNo\":" + std::to_string(bbSeatNo) + "," +
    		"\"flopCards\":\"" + cards + "\"," +
    		"\"turnCard\":" + std::to_string(turnCard.toInteger()) + "," +
    		"\"riverCard\":" + std::to_string(riverCard.toInteger()) + "," +
    		"\"smallBlindPrice\":" + std::to_string(smallBlindPrice) + "," +
    		"\"bigBlindPrice\":" + std::to_string(bigBlindPrice) + "," +
    		"\"seqNo\":" + std::to_string(seqNo) + "," +
    		"\"round\":" + std::to_string(round) + "," +
    		"\"dutySeatNo\":" + std::to_string(dutySeatNo) + "," +
    		"\"betIfCall\":" + std::to_string(betIfCall) + "," +
    		"\"minBetIfRaise\":" + std::to_string(minBetIfRaise) + "," +
    		"\"betBySeatNo\":" + std::to_string(betBySeatNo) + "," +
    		"\"continueThisRound\":" + std::to_string(continueThisRound) + "," +
    		"\"assureActionNumbers\":" + std::to_string(assureActionNumbers) + "," +
    		"\"lastPollingAt\":" + std::to_string(lastPollingAt) + "," +
    		"\"lastPollingAction\":{" + lastPollingAction.toString() + "}"
    		;

	str = str + ",\"seats\":[";
	int s = 0;
	for (int i=0; i<table->seats.size(); i++) {
		if (((LYHoldemSeat *)table->seats[i].get())->status == LYSeatOpen) continue;
		s++;
		str = str + "{" +((LYHoldemSeat *)table->seats[i].get())->toString() + "},";
	}
	if (s > 0) str = str.substr(0, str.size()-1);
	str = str + "]";

	str = str + "}";
    return str;
}

/*
 * sortSeats: sort all seats according to small_blind, big_blind, ..., button
 */
void LYHoldemGame::sortSeatsWithoutChangeStatus()
{
//std::cout << "enter sortSeats" << std::endl;
	cleanZombies();

	this->sortedSeats.clear();
	if (::LYHouse == this->sbSeatNo || ::LYHouse == this->btnSeatNo || ::LYHouse == this->bbSeatNo || this->seatsOfTable.size() < 2) return;
//	debugGame();

    LYSeatPtr seat = this->smallBlindSeat();
    int i = 0;
    if (table->occupied_seat_number == 2) seat = this->bigBlindSeat();
	while (seat->seatNo != this->btnSeatNo && i < 9) {
//std::cout << "seat no:" << seat->seatNo << ", status:" << seat->status << std::endl;
		i++;
		if (seat->status != LYSeatOpen) {
			sortedSeats.push_back(seat);
		}
		seat = table->fetchNextOccupiedSeat(seat->seatNo);
		if (NULL == seat.get()) {
//			LY_LOG_ERR("something weired happend!");
			return;
		}
	}
	if (sortedSeats.size() < 1) {
		//means no seats or only one seat occupied here!
		this->reset();
		return;
	}
    sortedSeats.push_back(this->buttonSeat());
}

/*
 * sortSeatsInGame: Client calls sortSeatsInGame when get scene
 */
void LYHoldemGame::sortSeatsInGame()
{
	this->sortedSeats.clear();
	if (::LYHouse == this->sbSeatNo || ::LYHouse == this->btnSeatNo || ::LYHouse == this->bbSeatNo || this->seatsOfTable.size() < 2) return;

    LYSeatPtr seat = this->smallBlindSeat();
    int i = 0;
    if (table->occupied_seat_number == 2) seat = this->bigBlindSeat();
	while (seat->seatNo != this->btnSeatNo && i < 9) {
		i++;
		if (seat->status == LYSeatInGame) {
			sortedSeats.push_back(seat);
		}
		seat = table->fetchNextOccupiedSeat(seat->seatNo);
		if (NULL == seat.get()) {
			return;
		}
	}
	if (sortedSeats.size() < 1) {
		//means no seats or only one seat occupied here!
		return;
	}
    sortedSeats.push_back(this->buttonSeat());
}

void LYHoldemGame::cleanZombies()
{
	std::vector<LYSeatPtr>::iterator it = seatsOfTable.begin();
	for (; it!=seatsOfTable.end(); it++) {
		LYSeatPtr seat = *it;
		if (seat.get() == NULL) {
			continue;
		}
		if (seat->playerId == "NULL" || seat->playerId == "") {
			seat->reset();
		}
	}
}

/*
 * 判断Action是否有效
 * 根据不同的Action调用不同的函数处理 onCall/onFold/...
 * 成功后再生成Scene
 */
bool LYHoldemGame::onAction(LYHoldemAction &action)
{
//	LY_LOG_DBG("duty_seat=" << this->dutySeatNo);
	if (!isValidAction(action)) {
//		LY_LOG_ERR("invalid action" << action.toString());
		return false;
	}
	LYSeatPtr seat = table->fetchSeat(action.applicant);
	if (action.applicant > 0 && action.applicant < 9) {
		if (seat->status == LYSeatOpen ||
			((LYHoldemSeat *)seat.get())->gameStatus == LYSeatHoldemInFold) {
			return false;
		}
	}
	if (action.verb == LYHousePollingDecision) {
		lastPollingAction = action;
		lastPollingAt = time(NULL);
		this->onPolling(seat);
		return true;
	}
	switch (action.verb)
	{
	case LYCall:
		this->onCall(seat, action.bet);
		break;
	case LYBet:
		this->onBet(seat, action.bet);
		break;
	case LYBetSmallBlind:
		this->onBetSmallBlind(seat, action.bet);
		break;
	case LYBetBigBlind:
		this->onBetBigBlind(seat, action.bet);
		break;
	case LYRaise:
		this->onRaise(seat, action.bet);
		break;
	case LYCheck:
		this->onCheck(seat);
		break;
	case LYFold:
		this->onFold(seat, action.desc);
		break;
	case LYAllin:
		this->onAllin(seat, action.bet);
		break;
	case LYHouseIssueFlop:
		this->onFlop(action.cards);
		break;
	case LYHouseIssueTurn:
		this->onTurn(action.cards.front());
		break;
	case LYHouseIssueRiver:
		this->onRiver(action.cards.front());
		break;
	case LYAskAssurance: //对Game状态无影响
	case LYSummarizeAssurance: //对Game状态无影响
	case LYPayAssurance: //客户端收到PaidAssurance，不在Game里处理，只是View视图处理即可
		return true;
	case LYBuyAssurance: //服务器收到玩家的
	{
//	LY_LOG_DBG("on assurance");
		this->onAssure(round, seat, action.bet, action.desc);
		this->genNewScene();
		return true;
	}
		break;
	default:
//		LY_LOG_ERR("invalid action" << action.toString());
		return false;
	}

	this->updateHoldemRound(action);
	this->genNewScene();
	this->appendLog(action.toString());

	return true;
}

void LYHoldemGame::genNewScene()
{
	LYHoldemScene *nc = new LYHoldemScene(sortedSeats, this->pots(), this->dutySeatNo,
			this->betIfCall, this->betIfCall*2, this->betBySeatNo,
			this->round, this->continueThisRound,
			seqNo++);
	LYHoldemScenePtr scene(nc);
//	LY_LOG_DBG("after this action, scene = \n" << nc->toString());
	scenes.push_back(scene);
}

void LYHoldemGame::onFlop(const std::vector<LYCard> &cards)
{
//	LY_LOG_DBG("enter onFlop");
	if (cards.size() < 3) {
//		LY_LOG_ERR("cards size should be greater than 3");
		return;
	}
    this->flopCards = cards;
    while (this->flopCards.size() > 3) {
    	this->flopCards.pop_back();
    }
    this->round = LYGameFlop;
    this->newRound();
}

void LYHoldemGame::onTurn(const LYCard &card)
{
    this->turnCard = card;
    this->round = LYGameTurn;
    this->newRound();
    this->checkAssurance(card);
}

void LYHoldemGame::onRiver(const LYCard &card)
{
    this->riverCard = card;
    this->round = LYGameRiver;
    this->newRound();
    this->checkAssurance(card);
}

void LYHoldemGame::onCall(LYSeatPtr st, unsigned int bet)
{
	if (st.get() == NULL) return;
    LYHoldemSeat *seat = (LYHoldemSeat *)st.get();
    if (bet > this->betIfCall ||
    		(bet < this->betIfCall && bet < (seat->chipsAtHand+seat->lastBet)))
    { //既不相等，也没有Allin，那就认为出错了！
//    	LY_LOG_INF("on onCall bet error "<< bet);
    	return;
    }

    pm.betBySeat(bet, st);
    if (seat->chipsAtHand > 0) {
    	seat->gameStatus = LYSeatHoldemInCalled;
    } else {
    	seat->gameStatus = LYSeatHoldemInAllin;
    }
	seat->timeoutCount = 0;
    return;
}

void LYHoldemGame::onRaise(LYSeatPtr st, unsigned int bet)
{
	if (st.get() == NULL) return;
//    LY_LOG_INF("on raise");
    //verify action.bet > betIfCall * 2
    LYHoldemSeat *seat = (LYHoldemSeat *)st.get();
//std::cout << "kkkkk, bet=" << bet << std::endl;
    if (bet < this->minBetIfRaise) {
        if (bet < (seat->chipsAtHand+seat->lastBet)) { //既没有2倍，也没有Allin，那就认为出错了！
//            LY_LOG_INF("on raise bet error "<< bet);
            return;
        }
    }

    this->betIfCall = bet;
    //added by chenfeng 20130524
    this->minBetIfRaise = bet * 2;
    this->betBySeatNo = st->seatNo;
//std::cout << "minBetIfRase:" << minBetIfRaise << ", betIfCall:" << betIfCall<< std::endl;

    pm.betBySeat(bet, st);
    if (seat->chipsAtHand > 0) {
    	seat->gameStatus = LYSeatHoldemInRaised;
    } else {
    	seat->gameStatus = LYSeatHoldemInAllin;
    }
	seat->timeoutCount = 0;
    return;
}

void LYHoldemGame::onBet(LYSeatPtr st, unsigned int bet)
{
	if (st.get() == NULL) return;
	this->onRaise(st, bet);
    LYHoldemSeat *seat = (LYHoldemSeat *)st.get();
	if (seat->gameStatus != LYSeatHoldemInAllin) {
		seat->gameStatus = LYSeatHoldemInBet;
	}
	seat->timeoutCount = 0;
}

void LYHoldemGame::onBetSmallBlind(LYSeatPtr st, unsigned int bet)
{
	if (st.get() == NULL) return;
	unsigned int tc = st->timeoutCount;
	this->onRaise(st, bet);
    LYHoldemSeat *seat = (LYHoldemSeat *)st.get();
	if (seat->gameStatus != LYSeatHoldemInAllin) {
		seat->gameStatus = LYSeatHoldemBetSmall;
	}
	seat->timeoutCount = tc;
}

void LYHoldemGame::onBetBigBlind(LYSeatPtr st, unsigned int bet)
{
	if (st.get() == NULL) return;
	unsigned int tc = st->timeoutCount;
	this->onRaise(st, bet);
    LYHoldemSeat *seat = (LYHoldemSeat *)st.get();
	if (seat->gameStatus != LYSeatHoldemInAllin) {
		seat->gameStatus = LYSeatHoldemBetBig;
	}
	seat->timeoutCount = tc;
}

void LYHoldemGame::onAllin(LYSeatPtr st, unsigned int bet)
{
	if (st.get() == NULL) return;
    LYHoldemSeat *seat = (LYHoldemSeat *)st.get();
	seat->gameStatus = LYSeatHoldemInAllin;
    unsigned int toBet = (bet>0)?bet:(st->chipsAtHand+st->lastBet);
    pm.betBySeat(toBet, st);
    pm.topPot()->priceFixed = true;
    if (toBet > this->betIfCall) {
        this->betIfCall = toBet;
        //added by chenfeng 20130524
        this->minBetIfRaise = toBet * 2;
        this->betBySeatNo = st->seatNo;
    }
	seat->timeoutCount = 0;
    return;
}

/*
 * 20140113 fold时，将Pot的involvedSeats去掉
 */
void LYHoldemGame::onFold(LYSeatPtr st, const std::string &reason)
{
	if (st.get() == NULL) return;
//	LY_LOG_DBG("folded reason " << reason);
    LYHoldemSeat *seat = (LYHoldemSeat *)st.get();
	seat->gameStatus = LYSeatHoldemInFold;
	pm.fold(st->seatNo);
	seat->fold(reason);
}

void LYHoldemGame::onCheck(LYSeatPtr st)
{
	if (st.get() == NULL) return;
    LYHoldemSeat *seat = (LYHoldemSeat *)st.get();
	seat->gameStatus = LYSeatHoldemInChecked;
	seat->timeoutCount = 0;
}

void LYHoldemGame::onPolling(LYSeatPtr st)
{
	if (st.get() == NULL) return;
    LYHoldemSeat *seat = (LYHoldemSeat *)st.get();
	seat->gameStatus = LYSeatHoldemMakingDecision;
}

void LYHoldemGame::onAssure(unsigned int assurance_round, LYSeatPtr st, unsigned int assurance, const std::string& pot)
{
	if (st.get() == NULL) return;
//std::cout << "onAssure:" << assureActionNumbers << std::endl;
	if (assureActionNumbers == 0) return;
    LYHoldemSeat *seat = (LYHoldemSeat *)st.get();
    pm.buyAssurance(assurance_round, st, assurance, pot);
	assureActionNumbers--;
//std::cout << "leaving onAssure:" << assureActionNumbers << std::endl;
}

/*
 * 最复杂的方法之一，根据当前Action和状态机，更新continueThisRound
 * 而continueThisRound被用于后续决定是否发Polling还是NextRound等
 * HoldemRoundDown: 接下来Turn、River等发牌动作都要进行，并在最后决出胜负
 * HoldemRoundEnd：如果只余下一个玩家，宣布比赛结束
 * 游戏结束时根据round和continueThisRound决定是否Show赢家的牌
 */
void LYHoldemGame::updateHoldemRound(LYHoldemAction &action)
{
//	if (continueThisRound == LYHoldemRoundDown || continueThisRound == LYHoldemRoundEnd) {
//		return;
//	}
	if (continueThisRound != LYHoldemRoundDown)
		continueThisRound = LYHoldemRoundCont;
	switch (action.verb) {
	case LYHouseIssueFirstCard:
	case LYHouseIssueSecondCard:
//	case LYHouseIssueFlop:
//	case LYHouseIssueTurn:
//	case LYHouseIssueRiver:
		return; //do nothing
	default:
		break;
	}
	unsigned int seated = 0;
	unsigned int allinSeats = 0;
	LYSeatPtr waitingSeat = LYSeatPtr();
	std::vector<LYSeatPtr>::iterator it = sortedSeats.begin();
	for (; it!=sortedSeats.end(); it++)
	{
		LYSeatPtr st = *it;
		LYHoldemSeat *seat = (LYHoldemSeat *)st.get();
		if (seat->status == LYSeatOpen) continue;

		switch (seat->gameStatus) {
		case LYSeatHoldemWaiting:
		case LYSeatHoldemBetSmall:
		case LYSeatHoldemBetBig:
		case LYSeatHoldemMakingDecision:
		case LYSeatHoldemInCalled:
		case LYSeatHoldemInChecked:
		case LYSeatHoldemInRaised:
		case LYSeatHoldemInBet:
			if (seat->chipsAtHand == 0) {
				allinSeats++;
				seat->gameStatus = LYSeatHoldemInAllin;
			} else {
				seated++;
				waitingSeat = st;
			}
			break;
		case LYSeatHoldemInAllin:
			allinSeats++;
			break;
		default:
			break;
		}
	}
	if (seated == 1) {
//		LY_LOG_DBG("allinSeats=" << allinSeats << ", seated=" << seated << ", betIfCall=" << this->betIfCall << ", waitingSeat->lastBet=" << waitingSeat->lastBet);
	} else {
//		LY_LOG_DBG("allinSeats=" << allinSeats << ", seated=" << seated << ", betIfCall=" << this->betIfCall );
	}
	/*
	 * 如果没有任何座位在游戏状态，即所有座位都Fold或者Allin，则直接进入到HoldemRoundDown状态
	 */
	if (0 == seated) {
		if (allinSeats < 2) {
			continueThisRound = LYHoldemRoundEnd;
			return;
		}
		continueThisRound = LYHoldemRoundDown;
		return ;
	}
	if (1 == seated) {
		/*
		 * 如果仅有一个座位在游戏状态，而且Allinseats = 0说明，只有一个玩家
		 */
		if (0 == allinSeats) {
			continueThisRound = LYHoldemRoundEnd;
			return ;
		}
		/*
		 * 如果仅有一个座位在游戏状态，其它所有座位都Fold或者Allin，则直接进入到HoldemRoundDown状态
		 */
		if (this->betIfCall <= waitingSeat->lastBet) {
			//说明未Allin的Seat已经做出大于当前最大注的决定，因此无需再做决定
			((LYHoldemSeat *)waitingSeat.get())->gameStatus = LYSeatHoldemInAllin;
			continueThisRound = LYHoldemRoundDown;
			return ;
		}
		/*
		 * 其它情况AllinSeats>0，并且当前注大于未Allin座位的上次出注，因此该座位还有出注机会
		 * 但需要判断一种情况，就是大盲已经出注Allin且其筹码小于小盲
		 */
		if (round == LYGameInit && waitingSeat->seatNo == sbSeatNo
				&& bigBlindSeat()->chipsAtHand == 0 && bigBlindSeat()->lastBet < smallBlindPrice) {
			continueThisRound = LYHoldemRoundDown;
			return ;
		}
		continueThisRound = LYHoldemRoundCont;
		return ;
	}

	if (round == LYGameInit) { //说明是大小盲下注，检查完Seats之后即返回，不判断Action
		return;
	}

	/*
	 * 以下根据Action进行判断
	 */
	if (action.applicant == LYHouse) {
		if (continueThisRound != LYHoldemRoundDown)
			continueThisRound = LYHoldemRoundCont;
		return;
	}

	LYSeatPtr nextSeatInGame = table->fetchNextSeatInGame(action.applicant);
	LYSeatPtr firstSeatInGame = this->fetchFirstSeatInGame();
	if (nextSeatInGame.get() == NULL || firstSeatInGame.get() == NULL) {
		return;
	}
	if (action.bet == 0 && action.verb != LYFold) {
		if (firstSeatInGame->seatNo == nextSeatInGame->seatNo) {
			continueThisRound = LYHoldemRoundNext;
			return ;
		}
		continueThisRound = LYHoldemRoundCont;
		return ;
	}

	if (this->round == LYGamePreflop) {
		if (nextSeatInGame->seatNo == this->bbSeatNo) {
			if (action.bet == this->bigBlindPrice) {
				continueThisRound = LYHoldemRoundCont;
				return;
			}
		}
	}

	if (nextSeatInGame->lastBet > 0
			&& nextSeatInGame->lastBet == action.bet) {
		continueThisRound = LYHoldemRoundNext;
		return ;
	}

	if (continueThisRound != LYHoldemRoundDown)
		continueThisRound = LYHoldemRoundCont;
	return ;
}

/*
 * 在一个桌子已经开始的情况下，Dealer检验Action是否合法
 * 先设DutySeat，然后执行onAction
 *
 */
bool LYHoldemGame::isValidAction(LYHoldemAction &action)
{
	//20130713注：Fold可能来自于LeaveSeat导致的离席Fold
	if (round == LYGameInit &&
			(action.verb == LYBetSmallBlind || action.verb == LYBetBigBlind)) {
		return true;
	}
	if (action.verb == LYFold ){ //&& action.desc == "byleave") {
		//20160701 在任何位置都可以fold
		//20160730 但不可以对已经allin的位置再fold
		if (this->round == LYGameClosed || this->round == LYGameInit) return false;
		LYSeatPtr seat = table->fetchSeat(action.applicant);
		LYHoldemSeat *holdemSeat = (LYHoldemSeat *)seat.get();
		if (holdemSeat != NULL && holdemSeat->gameStatus == LYSeatHoldemInAllin) return false;

		return true;
	}
	if (::LYHouse == dutySeatNo) { //check down（所有玩家allin）的情况下，
		return true;
	}
	if (action.verb == LYSummarizeAssurance) {
		return true; //任何时候都允许获取保险统计
	}
	if (action.applicant != LYHouse && dutySeatNo != action.applicant) {
//	std::cout << "duty:" << dutySeatNo << ", action="<< action.toString() << std::endl;
		if (action.isAssuranceRelevant()) {
			return true;
		}
		return false;
	}
	if (action.applicant == LYHouse) {
		switch (this->round)
		{
		case LYGamePreflop: //发完第二张牌
			if (action.verb == LYHouseIssueFlop) {
				return true;
			}
			return false;
		case LYGameFlop: //发完Flop
			if (action.verb == LYHouseIssueTurn) {
				return true;
			}
			return false;
		case LYGameTurn: //发完Turn
			if (action.verb == LYHouseIssueRiver) {
				return true;
			}
			return false;

		case LYGameRiver: //发完River
			return false;

		default:
			return false;
		}
	}
	//到此,duty != NULL, duty.seatNo == action.applicant
	LYSeatPtr seat = table->fetchSeat(action.applicant);
	LYHoldemSeat *holdemSeat = (LYHoldemSeat *)seat.get();
	if (action.verb != LYHousePollingDecision &&
			action.verb != LYBuyAssurance && //added 20160321 for assurance
			holdemSeat->gameStatus != LYSeatHoldemMakingDecision) {
//		LY_LOG_ERR("invalid action");
		return false;
	}

	if (action.verb == LYBetSmallBlind || action.verb == LYBetBigBlind) {
		if (round != LYGameInit) {
			return false;
		}
		return true;
	}
	switch (this->round)
	{
	case LYGameWaiting: //跟这个Dealer和桌子相关的，下一个动作应该只允许新游戏开始，或者在上面就返回了
	case LYGameInit: //新游戏开始
	case LYGameClosed:
		return false;

	case LYGamePreflop: //发完第二张牌
		switch (action.verb)
		{
		case LYHouseIssueFlop: //发Flop
		case LYHousePollingDecision: //询问下一个做决定
			return true;
		case LYBet: //下一个做的决定
		case LYRaise: //下一个做的决定
			if (action.bet < this->minBetIfRaise) return false;
			return true;
		case LYCall: //下一个做的决定
			if (action.bet < this->betIfCall) return false;
			return true;
		case LYFold: //下一个做的决定
		case LYAllin: //下一个做的决定
//		case LYBuyAssurance:
		case LYAskAssurance:
//		case LYPayAssurance:
			return true;
		case LYCheck: //下一个做的决定
			if (0 == this->betIfCall || seat->lastBet == this->betIfCall) return true;
			return false;

		default:
			return false;
		}
		break;

	case LYGameFlop: //发完Flop
		switch (action.verb)
		{
		case LYHouseIssueTurn: //发Turn
		case LYHousePollingDecision: //询问下一个做决定
			return true;
		case LYBet: //下一个做的决定
		case LYRaise: //下一个做的决定
			if (action.bet < this->minBetIfRaise) return false;
			return true;
		case LYCall: //下一个做的决定
			if (action.bet < this->betIfCall) return false;
			return true;
		case LYFold: //下一个做的决定
		case LYAllin: //下一个做的决定
		case LYBuyAssurance:
		case LYAskAssurance:
//		case LYPayAssurance:
			return true;
		case LYCheck: //下一个做的决定
			if (0 == this->betIfCall || seat->lastBet == this->betIfCall) return true;
			return false;

		default:
			return false;
		}
		break;

	case LYGameTurn: //发完Turn
		switch (action.verb)
		{
		case LYHouseIssueRiver: //发Turn
		case LYHousePollingDecision: //询问下一个做决定
			return true;
		case LYBet: //下一个做的决定
		case LYRaise: //下一个做的决定
			if (action.bet < this->minBetIfRaise) return false;
			return true;
		case LYCall: //下一个做的决定
			if (action.bet < this->betIfCall) return false;
			return true;
		case LYFold: //下一个做的决定
		case LYAllin: //下一个做的决定
		case LYBuyAssurance:
		case LYAskAssurance:
		case LYPayAssurance:
			return true;
		case LYCheck: //下一个做的决定
			if (0 == this->betIfCall || seat->lastBet == this->betIfCall) return true;
			return false;

		default:
			return false;
		}
		break;

	case LYGameRiver: //发完River
		switch (action.verb)
		{
		case LYHousePollingDecision: //询问下一个做决定
			return true;
		case LYBet: //下一个做的决定
		case LYRaise: //下一个做的决定
			if (action.bet < this->minBetIfRaise) return false;
			return true;
		case LYCall: //下一个做的决定
			if (action.bet < this->betIfCall) return false;
			return true;
		case LYFold: //下一个做的决定
		case LYAllin: //下一个做的决定
		case LYPayAssurance:
			return true;
		case LYCheck: //下一个做的决定
			if (0 == this->betIfCall || seat->lastBet == this->betIfCall) return true;
			return false;

		default:
			return false;
		}
		break;

	default:
		break;
	}
	return false;
}

LYSeatPtr LYHoldemGame::fetchFirstSeatInGame()
{
	if (round == LYGameInit) {
		return this->smallBlindSeat();
	}
	if (round == LYGamePreflop) {
		return this->fetchNextSeatInGame(this->bbSeatNo);
	}
	return this->fetchNextSeatInGame(this->btnSeatNo);
}

LYSeatPtr LYHoldemGame::fetchNextSeatInGame(LYApplicant seat_no)
{
	if ((unsigned int)seat_no < 1 || (unsigned int)seat_no > 9) {
//		LY_LOG_ERR("fetchNextSeatInGame::seat_no must be between 1..9, " << (unsigned int)seat_no);
		return LYSeatPtr();
	}
	std::vector<LYSeatPtr>::iterator it = sortedSeats.begin();
	LYSeatPtr right_hand;
	LYSeatPtr left_hand;
	bool checking_right = true;
	for (; it!=sortedSeats.end(); it++) {
		LYSeatPtr cur = *it;
		if (cur->seatNo == seat_no) {
			checking_right = false;
			continue;
		}
		LYHoldemSeat *seat = (LYHoldemSeat *)((*it).get());
		if (checking_right && NULL == right_hand.get() && seat->isInGame()) {
			right_hand = *it;
		}
		if (!checking_right && NULL == left_hand.get() && seat->isInGame()) {
			left_hand = *it;
			return left_hand;
		}
	}
	return right_hand;
}

LYSeatPtr LYHoldemGame::fetchNextSeatInGameAndNotAllin(LYApplicant seat_no)
{
	if ((unsigned int)seat_no < 1 || (unsigned int)seat_no > 9) {
//		LY_LOG_ERR("fetchNextSeatInGameAndNotAllin::seat_no must be between 1..9, " << (unsigned int)seat_no);
		return LYSeatPtr();
	}
	std::vector<LYSeatPtr>::iterator it = sortedSeats.begin();
	LYSeatPtr right_hand;
	LYSeatPtr left_hand;
	bool checking_right = true;
	for (; it!=sortedSeats.end(); it++) {
		LYSeatPtr cur = *it;
		if (cur->seatNo == seat_no) {
			checking_right = false;
			continue;
		}
		LYHoldemSeat *seat = (LYHoldemSeat *)((*it).get());
		if (checking_right && NULL == right_hand.get() && seat->isInGameAndNotAllin()) {
			right_hand = *it;
		}
		if (!checking_right && NULL == left_hand.get() && seat->isInGameAndNotAllin()) {
			left_hand = *it;
			return left_hand;
		}
	}
	return right_hand;
}

/*
 * gen community card actions
 * flop
 * turn
 * river
 * called by startGame()
 */
void LYHoldemGame::genCommunityCardActions()
{
	this->deck.pop();
	std::vector<LYCard> cards;
	cards.push_back(this->deck.pop());
	cards.push_back(this->deck.pop());
	cards.push_back(this->deck.pop());
	flop_action.issueFlopCards(cards);
	flop_action.table_id = this->table->_id;

	this->deck.pop();
	LYCard cd = deck.pop();
	turn_action.issueTurnCard(cd);
	turn_action.table_id = this->table->_id;

	this->deck.pop(); cd = deck.pop();
	river_action.issueRiverCard(cd);
	river_action.table_id = this->table->_id;
}

/*
 * gen community card actions
 * flop
 * turn
 * river
 * called by startGame()
 */
void LYHoldemGame::reverseCommunityCardActions()
{
	flop_action.issueFlopCards(flopCards);
	flop_action.table_id = this->table->_id;

	turn_action.issueTurnCard(turnCard);
	turn_action.table_id = this->table->_id;

	river_action.issueRiverCard(riverCard);
	river_action.table_id = this->table->_id;
}

/*
 * 当continueThisRound == LYSeatHoldemRoundEnd时，调用本方法
 * 负责分出胜负
 * 20160329 added 要对保险进行计算
 */
void LYHoldemGame::gameOver()
{
	std::vector<LYPotPtr> allPots = this->pots();
	std::vector<LYPotPtr>::iterator it = allPots.begin();

	if (totalSeatsInGame() == 1) { //所有人都弃牌，只留下一个玩家时，他获得所有pot
		LYSeatPtr seat = findLastSeatInGame();
		if (seat.get() == NULL) {
			LY_LOG_ERR("weired, should not get over here!");
			return;
		}
    	for (; it!=allPots.end(); it++) {
    		LYPotPtr pot = *it;
    		seat->chipsAtHand += pot->chipsInPot;
    		pot->winnersInPot.clear();
    		pot->winnersInPot.push_back(seat->seatNo);
		}
	} else {
		std::vector<LYCard> communityCards = this->getCommunityCards();

    	for (; it!=allPots.end(); it++) {
    		LYPotPtr pot = *it;
    		pot->caculateMatchResult(this);
		}
	}
	this->round = LYGameClosed;
	this->genNewScene();
	this->appendLog(this->toString());
}

void LYHoldemGame::caculateAssuranceResult(LYAssurance *assure)
{
//std::cout << assure->toString() << std::endl;
//std::cout << "assure result:" << assure->result << std::endl;
	LYSeatPtr seat = fetchSeat((LYApplicant)assure->seat);
	if (assure->result > 0) { //击中保险，赔付
		seat->chipsAtHand += assure->result;
		table->assurance_income -= assure->result;
		assurance_summary_action.bet -= assure->result;
	} else {
		seat->chipsAtHand -= assure->assurance;
		table->assurance_income += assure->assurance;
		assurance_summary_action.bet += assure->assurance;
	}
}

/*
 * 20160320 added for refactoring
 */
std::vector<LYCard> LYHoldemGame::getCommunityCards()
{
	std::vector<LYCard> communityCards = this->flopCards;
//	LY_LOG_DBG("game round" << this->round);
	switch (this->round) {
	case LYGameRiver:
		communityCards.push_back(this->riverCard);
	case LYGameTurn:
		communityCards.push_back(this->turnCard);
		break;
	default:
		break;
	}

	return communityCards;
}

void LYHoldemGame::makePaidAssurances(std::vector<LYHoldemAction>& actions, const LYCard& card)
{
	std::vector<LYPotPtr>& allPots = pots();
	std::vector<LYPotPtr>::iterator it = allPots.begin();
	std::vector<LYHoldemAction> assures;
	//首先生成之前的保险结果
	for (; it!=allPots.end(); it++) {
    	LYPotPtr pot = *it;
    	if (card == flipCard) continue;
    	pot->makePaidAssuranceActions(this, card, assures);
    }
	actions.insert(actions.end(), assures.begin(), assures.end());
}

/*
 * 20160323 generate assurance actions
 * 20160710 只返回第一条买保险的请求，当来了买保险的动作时，再
 */
void LYHoldemGame::genAssureActions(std::vector<LYHoldemAction>& actions,
					const LYCard card, bool nomoreAssure)
{
//LY_LOG_DBG("card =" );
	std::vector<LYPotPtr>& allPots = pots();
	std::vector<LYPotPtr>::iterator it = allPots.begin();
	std::vector<LYHoldemAction> assures;
	//其次生成新的保险请求
	for (it=allPots.begin(); it!=allPots.end(); it++) {
		LYPotPtr pot = *it;
		pot->makeAskAssuranceActions(this, assures);
	}
	assureActionNumbers = assures.size();
//LY_LOG_DBG("generated assure actions:" << assureActionNumbers);
//	actions.insert(actions.end(), assures.begin(), assures.end());
	if (assureActionNumbers > 0) {
		actions.push_back(this->popAssureAction());
		//只保留最后一个询问保险的请求，如果一局中有多个玩家触发买保险的动作，在重入时只能看到最后一个
		//mark by cqf 20160630
		lastPollingAction = assures[assureActionNumbers-1];
		lastPollingAt = time(NULL);
	}
}

void LYHoldemGame::dutyMoveToNextSeat()
{
//	LY_LOG_DBG("before duty=" << dutySeatNo << ", sortedSeats=" << sortedSeats.size());
//	std::cout << "before duty=" << dutySeatNo << ", sortedSeats=" << sortedSeats.size() << std::endl;
    if (LYHouse == dutySeatNo) return;
    unsigned int i = 0;
    while (i<sortedSeats.size()) {
    	i++;
    	LYSeatPtr seat = this->fetchNextSeatInGame(dutySeatNo);
    	if (NULL == seat.get()) {
    		//just one seat left in game
    		return;
    	}
        dutySeatNo = seat->seatNo;
//    	LY_LOG_DBG("looping i=" << i << ", duty="<< dutySeatNo);
        LYHoldemSeat *holdemSeat = (LYHoldemSeat *)dutySeat().get();
        if (holdemSeat->isInGameAndNotAllin()){
            break;
        }
    }
//	LY_LOG_DBG("after duty=" << dutySeatNo);
}

void LYHoldemGame::newRound()
{
	std::vector<LYSeatPtr>::iterator it = sortedSeats.begin();
	for (; it!=sortedSeats.end(); it++) {
		LYHoldemSeat *seat = (LYHoldemSeat *)(*it).get();
		if (seat->status != LYSeatOpen ) {
			switch (seat->gameStatus)
			{
			case LYSeatHoldemWaiting:
			case LYSeatHoldemInBet:
			case LYSeatHoldemInCalled:
			case LYSeatHoldemInChecked:
			case LYSeatHoldemInRaised:
				seat->gameStatus = LYSeatHoldemWaiting;
				break;
			default:
				//do nothing
				break;
			}
		}
	}
    if (continueThisRound != LYHoldemRoundEnd && continueThisRound != LYHoldemRoundDown) {
    	continueThisRound = LYHoldemRoundCont;
    }

    if (round == LYGamePreflop) {
        dutySeatNo = this->fetchNextSeatInGameAndNotAllin(bbSeatNo)->seatNo;
        betIfCall = this->bigBlindPrice;
        minBetIfRaise = 2*this->bigBlindPrice;
        betBySeatNo = bbSeatNo;
        return;
    }
    LYSeatPtr ds = this->fetchNextSeatInGameAndNotAllin(btnSeatNo);
    dutySeatNo = LYHouse;
    if (NULL != ds.get()) {
    	dutySeatNo = ds->seatNo;
    }
    betIfCall = 0;
    minBetIfRaise = bigBlindPrice;
    betBySeatNo = LYHouse;

    lastPollingAt = 0;
    lastPollingAction.reset();
    assureActionNumbers = 0;
	assureActions.clear();

    pm.newRound();
}

LYSeatPtr LYHoldemGame::buttonSeat()
{
	return table->fetchSeat(this->btnSeatNo);
}
LYSeatPtr LYHoldemGame::smallBlindSeat()
{
	return table->fetchSeat(this->sbSeatNo);
}
LYSeatPtr LYHoldemGame::bigBlindSeat()
{
	return table->fetchSeat(this->bbSeatNo);
}
LYSeatPtr LYHoldemGame::dutySeat()
{
	return table->fetchSeat(this->dutySeatNo);
}
LYSeatPtr LYHoldemGame::betBy()
{
	return table->fetchSeat(this->betBySeatNo);
}

/*
 * added 20130605
 */
std::string LYHoldemGame::getHandCardFace(enum LYApplicant player)
{
	if ((unsigned int)player < 1 || (unsigned int)player > 9) {
//		LY_LOG_ERR("getHandCardFace::seat_no must be between 1..9, " << (unsigned int)player);
		return "00";
	}
	LYSeatPtr st = this->fetchSeat(player);
	std::string faces = "00";
	if (st->handCards.size() != 2) {
		return faces;
	}
    if (st->handCards[0].isFlipCard() ||
        st->handCards[1].isFlipCard()) {
        return faces;
    }
    if (st->handCards[0] > st->handCards[1]) {
    	faces[0] = st->handCards[0].getFaceSymbol();
    	faces[1] = st->handCards[1].getFaceSymbol();
    } else {
    	faces[0] = st->handCards[1].getFaceSymbol();
    	faces[1] = st->handCards[0].getFaceSymbol();
    }
    return faces;
}

bool LYHoldemGame::isHandCardFlush(enum LYApplicant player)
{
    LYSeatPtr st = this->fetchSeat(player);
	if (NULL == st.get() || st->handCards.size() != 2) {
		return false;
	}
    if (st->handCards[0].isFlipCard() ||
        st->handCards[1].isFlipCard()) {
        return false;
    }
    if (st->handCards[0].suit == st->handCards[1].suit) {
        return true;
    } else {
        return false;
    }
}

/*
 * added 20130605
 */
enum LYHandsOrder LYHoldemGame::judgeHand(enum LYApplicant player)
{
	if (this->round != LYGameFlop && this->round != LYGameTurn && this->round != LYGameRiver) {
		return LYON_DRAW;
	}
	LYSeatPtr st = this->fetchSeat(player);
	if (NULL == st.get() || st->handCards.size() != 2) {
		return LYON_DRAW;
	}
	LYHandStrengthPtr hs = LYHandStrengthPtr(this->getHandStrength(player));
	enum LYHandsOrder community = this->judgeCommunity();
	switch (hs->ranking)
	{
	case OnePair:
	case HighCard:
	case TwoPair:
		return LYON_DRAW;
	case ThreeOfAKind:
		if (st->handCards[0].face != hs->topRankCard.face) {
			if (st->handCards[1].face != hs->topRankCard.face) {
				//set for community, everyone has a set
				return LYSECOND_NUTS;
			}
			//community has a pair and it hits a set
			return LYSECOND_NUTS;
		}
		if (st->handCards[1].face != hs->topRankCard.face) {
			//set for community, everyone has a set
			return LYSECOND_NUTS;
		}
		//player has a pair and hits a set
		switch (community)
		{
		case LYFOUROFAKIND_ODDS:
		case LYFULLHOUSE_ODDS:
		case LYFLUSH_ODDS:
		case LYOUTSIDE_ODDS:
		case LYGUTSHOT_ODDS:
			return LYSECOND_NUTS;
		default:
			return LYNUTS;
		}
		return LYNUTS;
	default:
		break;
	}
    return LYON_DRAW;
}

/*
 * added 20130605
 */
enum LYHandsOrder LYHoldemGame::judgeCommunity()
{
	if (this->round != LYGameFlop && this->round != LYGameTurn && this->round != LYGameRiver) {
		return LYON_DRAW;
	}
    std::vector<LYCard> cards = flopCards;
    if (!turnCard.isFlipCard()) {
        cards.push_back(turnCard);
    } else {
    	cards.push_back(smallGhost);
    }
    if (!riverCard.isFlipCard()) {
        cards.push_back(riverCard);
    } else {
    	cards.push_back(bigGhost);
    }
    LYHoldemAlgorithmDelegate had; //把HandStrength算法从类里移到了外面，改来改去，这个Ugly的代码以后再改掉吧
    LYHandOdds *hsp = new LYHandOdds(cards, &had);
//    LYHandOdds *hsp = new LYHandOdds(cards);
    LYHandOddsPtr hs = LYHandOddsPtr(hsp);
    unsigned int sbase = 0;
    unsigned int baseCount = 2;
    if (this->round == LYGameRiver) baseCount = 3;
	switch (hs->ranking)
	{
	case OnePair:
	case TwoPair:
	case ThreeOfAKind:
		return LYFOUROFAKIND_ODDS;
	case Straight:
		if (hs->flush_base()>=baseCount) {
			return LYFLUSH_ODDS;
		}
		return LYOUTSIDE_ODDS;
	case Flush:
		return LYFLUSH_ODDS;
	case HighCard:
		if (hs->flush_base()>=baseCount) {
			return LYFLUSH_ODDS;
		}
		sbase = hs->straight_base();
		if (sbase >= baseCount) {
			if (sbase == 2) return LYGUTSHOT_ODDS; //not 100% correct here!
			return LYOUTSIDE_ODDS;
		}
		return LYFLUSH_ODDS;
	default:
		break;
	}
    return LYON_DRAW;
}

/*
 * added 20130605
 */
unsigned int LYHoldemGame::getOuts(enum LYApplicant player)
{
	if ((unsigned int)player < 1 || (unsigned int)player > 9) {
//		LY_LOG_ERR("getOuts::player must be between 1..9, " << (unsigned int)player);
		return 0;
	}
//	LY_LOG_DBG("enter getOuts, round=" << this->round);
	if (this->round == LYGameRiver || this->round == LYGamePreflop) {
		return 0;
	}
	//only process game in flop/turn
	LYHandOddsPtr hs = LYHandOddsPtr(this->getHandOdds(player));
	if (NULL == hs.get()) {
		return 0;
	}
	if (NULL != hs.get() && (hs->ranking == StraightFlush
							|| hs->ranking == FourOfAKind)) {
		//don't really think these two hands need any out
		return 0;
	}
	LYSeatPtr seat = this->fetchSeat(player);
	std::vector<std::string> indexs = hs->genIndex(seat->handCards[0]);
	std::vector<std::string> indexs2 = hs->genIndex(seat->handCards[1]);
	//merge two indexs
	std::vector<std::string> newIndex = indexs;
	BOOST_FOREACH(std::string idx1, indexs2)
	{
		bool dup = false;
		BOOST_FOREACH(std::string idx2, newIndex)
		{
			if (idx2 == idx1) {
				dup = true;
			}
		}
		if (!dup) {
			newIndex.push_back(idx1);
		}
	}
	unsigned int outs = 0;
	BOOST_FOREACH(std::string idx, newIndex)
	{
		std::vector<LYCard> cards = hs->get_odds(idx);
		unsigned int sz = cards.size();
		std::string flag = idx.substr(0, 3);
//		LY_LOG_DBG("idx=" <<idx << ", size=" << sz << ", outs=" << outs);
		if (flag == "Qua" && sz == 3) {
			outs++;
		}
		if (flag == "Set" && sz == 2) {
			outs += 2;
		}
		if (flag == "Flu" && sz == 4) {
			outs += 9;
		}
		if (flag == "Str" && sz == 4) {
			outs += 4;
		}
		if (flag == "Ful" && sz == 4) {
			outs += 3;
		}
//		if (flag == "SnF" && sz == 4) {
//			outs += 1;
//		}
//		if (flag == "2pr" && sz == 3) {
//			outs += 3;
//		}
//		if (flag == "1pr" && sz == 1) {
//			outs += 3;
//		}
	}
    return outs;
}

/*
 * added 20130605
 */
unsigned int LYHoldemGame::getChipsIfWin(enum LYApplicant player)
{
    return pm.getChipsIfWin(player);
}

LYHandOdds *LYHoldemGame::getHandOdds(enum LYApplicant player)
{
	LYSeatPtr st = this->fetchSeat(player);
	if (NULL == st.get() || st->handCards.size() != 2) {
		return NULL;
	}
    if (st->handCards[0].isFlipCard() ||
        st->handCards[1].isFlipCard()) {
        return NULL;
    }
    std::vector<LYCard> cards = flopCards;
    if (!turnCard.isFlipCard()) {
        cards.push_back(turnCard);
    }
    if (!riverCard.isFlipCard()) {
        cards.push_back(riverCard);
    }
    cards.push_back(st->handCards[0]);
    cards.push_back(st->handCards[1]);
    LYHoldemAlgorithmDelegate had; //把HandStrength算法从类里移到了外面，改来改去，这个Ugly的代码以后再改掉吧
    LYHandOdds *ho = new LYHandOdds(cards, &had);
    return ho;
}

LYHandStrength *LYHoldemGame::getHandStrength(enum LYApplicant player)
{
	LYSeatPtr st = this->fetchSeat(player);
	if (NULL == st.get() || st->handCards.size() != 2) {
		return NULL;
	}
    if (st->handCards[0].isFlipCard() ||
        st->handCards[1].isFlipCard()) {
        return NULL;
    }
    std::vector<LYCard> cards = flopCards;
    if (!turnCard.isFlipCard()) {
        cards.push_back(turnCard);
    }
    if (!riverCard.isFlipCard()) {
        cards.push_back(riverCard);
    }
    cards.push_back(st->handCards[0]);
    cards.push_back(st->handCards[1]);
    LYHoldemAlgorithmDelegate had;
    LYHandStrength *hs = new LYHandStrength(cards, &had);
    return hs;
}

LYHoldemScene *LYHoldemGame::lastScene()
{
	LYHoldemScene *scene = NULL;
	LYHoldemScenePtr scenePtr = scenes.back();
	if (scenes.size() > 0 && NULL != scenePtr.get()) {
		return scenePtr.get();
	}
	return scene;
}

LYRanking LYHoldemGame::caculateSeatRanking(LYApplicant seatNo)
{
	LYHandStrength *hs = this->getHandStrength(seatNo);
	if (NULL == hs) {
		return HighCard;
	}
	LYRanking ranking = hs->ranking;
	delete hs;
	return ranking;
}

void LYHoldemGame::genActionView(LYApplicant seatNo, LYHoldemActionView& actionView)
{
	LYHoldemSeat *seat = (LYHoldemSeat *)this->fetchSeat(seatNo).get();
	if (NULL == seat || seat->status == LYSeatOpen ||
//			LYSeatHoldemWaiting == seat->gameStatus ||
			LYSeatHoldemInFold == seat->gameStatus ||
			LYSeatHoldemInAllin == seat->gameStatus){
		//nothing to do
		return;
	}

	//now seat is in game
	if (continueThisRound != LYHoldemRoundCont) { //means ends this round
		//nothing to do as well
//		std::cout << "kkkkk" << std::endl;
//		LY_LOG_DBG("not continue this round! " << continueThisRound);
		return;
	}

	if (this->dutySeatNo == seatNo) {
		actionView.onDuty = true;
	}

	actionView.betIfCall = this->betIfCall;
	actionView.betToCall = 0;

	//get all involved users from the top pot
	unsigned int chipsLeft = seat->getChipsLeft();
	unsigned int maxLeft = this->getMaxChipsLeftOfOthers(seatNo); //其他人最大的筹码量
	//actionView.raiseMax：从chipsLeft, maxLeft二者中取小
	//actionView.raiseMin：从chipsLeft, this->minBetIfRaise二者中取小
	actionView.raiseMax = (chipsLeft<maxLeft)?chipsLeft:maxLeft;
	actionView.raiseMin = (this->minBetIfRaise < chipsLeft)?this->minBetIfRaise:chipsLeft;
	if (actionView.raiseMax < actionView.raiseMin) { //意味着除了自己，其它人的筹码都不够Raise
		actionView.raiseMin = actionView.raiseMax;
	}
//	raiseMax == raiseMin means allin if raise
	if (seat->lastBet == this->betIfCall) { //只有在Preflop时，一路Call到BigBlind
		//view显示：fold/check/raise, view判断是否allin根据raiseMin和raiseMax两个参数
		actionView.checkEnabledForBigBlind = true;
		return;
	}
	//只可能是seat->lastBet < this->betIfCall
	//view显示fold/call/raise，需要区分的是如果Call就Allin即显示另外一种情况fold/allin
	if (chipsLeft <= this->betIfCall) {
		actionView.allinIfCall = true;
		actionView.betToCall = seat->chipsAtHand;
	} else {
		actionView.betToCall = this->betIfCall - seat->lastBet;
	}
}

unsigned int LYHoldemGame::getMaxChipsLeftOfOthers(LYApplicant seatNo)
{
	LYPotPtr& topPot = pm.topPot();
	std::vector<LYApplicant>::iterator it = topPot->seatsInPot.begin();
	unsigned int max_chips = 0;
	for (; it!=topPot->seatsInPot.end(); it++) {
		if (*it == seatNo) continue;
		LYHoldemSeat* seat = (LYHoldemSeat*)(fetchSeat(*it).get());
		if (NULL == seat) continue;
		if (seat->getChipsLeft() > max_chips) {
			max_chips = seat->getChipsLeft();
		}
	}
	return max_chips;
}

bool LYHoldemGame::isWinner(LYApplicant seatNo)
{
	if ((unsigned int)seatNo < 1 || (unsigned int)seatNo > 9) {
//		LY_LOG_ERR("isWinner::seatNo must be between 1..9, " << (unsigned int)seatNo);
		return false;
	}
	std::vector<LYPotPtr>& _pots = this->pots();
	std::vector<LYPotPtr>::iterator it = _pots.begin();
	for (; it!=_pots.end(); it++) {
		LYPotPtr pot = *it;
		if (pot->isWinner(seatNo)) {
			return true;
		}
	}
	return false;
}

/*
 * 被takeSeat/leaveSeat调用，如果round是在游戏中，则返回false，否则返回true
 */
bool LYHoldemGame::readyForNext()
{
	if (this->round == LYGameClosed || this->round == LYGameInit || this->round == LYGameWaiting) {
		return true;
	}

//	std::vector<LYSeatPtr>::iterator it = seatsOfTable.begin();
//	int seated = 0;
//	for (; it != seatsOfTable.end(); it ++) {
//		LYSeatPtr seat = *it;
//		if (seat->status != LYSeatOpen) {
//			seated ++;
//		}
//		if (seated > 2) {
//			return false;
//		}
//	}
//
//	if (seated == 2) {
//		this->round = LYGameWaiting;
//		return true;
//	}
//
	return false;
}

/*
 * 之前的持久化/序列化跟数据库有关，因此把它放到了后台
 * 现在持久化到redis中，只需要key-value-pair，因此又在本类中实现。
 * 2016-03-10
 */
std::vector<std::pair<std::string, std::string> > LYHoldemGame::game2kvps()
{
	std::vector<std::pair<std::string, std::string> > kvps = static2kvps();

	std::vector<std::pair<std::string, std::string> > v = dynamic2kvps();
	kvps.insert(kvps.end(), v.begin(), v.end());

	std::pair < std::string, std::string > kvp;
	kvp.first = "flopCards";
	LYCardHelpers::cardsToStringifiedInt(this->flop_action.cards, kvp.second);
	kvps.push_back(kvp);

	kvp.first = "turnCard";
	kvp.second = std::to_string(this->turn_action.cards[0].toInteger());
	kvps.push_back(kvp);

	kvp.first = "riverCard";
	kvp.second = std::to_string(this->river_action.cards[0].toInteger());
	kvps.push_back(kvp);

	kvp.first = "_id";
	kvp.second = this->_id;
	kvps.push_back(kvp);

	return kvps;
}

void LYHoldemGame::buildPot(std::vector<std::pair<std::string, std::string>>& kvps)
{
	pm.buildPot(kvps);
}

std::vector<LYCard> LYHoldemGame::getAllVisibleCards()
{
	std::vector<LYCard> cards = this->getCommunityCards();
	std::vector<LYSeatPtr>::iterator it = seatsOfTable.begin();
	for (; it!=seatsOfTable.end(); it++) {
		LYHoldemSeat *seat = (LYHoldemSeat *)(*it).get();
		if (seat->status == LYSeatInGame && seat->gameStatus == LYSeatHoldemInAllin) {
			cards.insert(cards.end(), seat->handCards.begin(), seat->handCards.end());
		}
	}

	return cards;
}

void LYHoldemGame::checkAssurance(const LYCard &card)
{
	std::vector<LYPotPtr>& allpots = pots();
	std::vector<LYPotPtr>::iterator it = allpots.begin();
	for (; it!=allpots.end(); it++) {
		LYPotPtr pot = *it;
		pot->checkAssurance(this->round, card);
	}
}

bool LYHoldemGame::finishAssurance()
{
LY_LOG_DBG("assure:" << assureActionNumbers);
	return (assureActionNumbers == 0);
}

/*
 * added 20160701
 * for white list when game finished
 */
std::vector<std::string> LYHoldemGame::getWhiteList()
{
	std::vector<std::string> white_list;
	if (round != LYGameClosed || totalSeatsInGame() < 2) 	return white_list;
    std::vector<LYSeatPtr>::iterator it = sortedSeats.begin();
    for (; it!=sortedSeats.end(); it++) {
    	LYSeatPtr seat = *it;
		LYHoldemSeat *hs = (LYHoldemSeat *)seat.get();
		if (hs->gameStatus == LYSeatHoldemInAllin || this->isWinner(hs->seatNo)) {
			white_list.push_back(hs->playerId);
		}
	}

	return white_list;
}

/*
 * added 20160701
 * for white list when game finished
 */
unsigned int LYHoldemGame::totalSeatsInGame()
{
	unsigned int count = 0;
    std::vector<LYSeatPtr>::iterator it = sortedSeats.begin();
    for (; it!=sortedSeats.end(); it++) {
    	LYSeatPtr seat = *it;
		LYHoldemSeat *hs = (LYHoldemSeat *)seat.get();
		if (hs->gameStatus == LYSeatHoldemWaiting || hs->gameStatus == LYSeatHoldemInFold) continue;
		count ++;
	}
	return count;
}

LYSeatPtr LYHoldemGame::findLastSeatInGame()
{
	LYSeatPtr seat;
    std::vector<LYSeatPtr>::iterator it = sortedSeats.begin();
    for (; it!=sortedSeats.end(); it++) {
    	seat = *it;
		LYHoldemSeat *hs = (LYHoldemSeat *)seat.get();
		if (hs->gameStatus == LYSeatHoldemWaiting || hs->gameStatus == LYSeatHoldemInFold) continue;
		break;
	}
	return seat;
}

LYHoldemAction LYHoldemGame::popAssureAction()
{
	LYHoldemAction action;
	std::vector<LYPotPtr>::iterator it = pots().begin();
	for (; it != pots().end(); it++) {
		LYPotPtr pot = *it;
		if (pot->assureActions.size() > 0) {
			return pot->popAssureAction();
		}
	}
	return action;
}

void LYHoldemGame::appendLog(const std::string& log)
{
	logMgr.appendLog(log);
}

std::vector<std::string> LYHoldemGame::getGameLogs()
{
	return logMgr.getLogs();
}
