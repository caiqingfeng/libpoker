/*
 * LYPineappleGame.cpp
 *
 *  Created on: 2014-10-11
 *      Author: caiqingfeng
 */

#include "LYPineappleGame.h"
#include <iostream>
#include "libmrock/common/src/my_log.h"

void LYPineappleGame::reset()
{
	_id = "";
	btnSeatNo = ::LYHouse;
//	gameType = 0;
	seqNo = 0;
	dutySeatNo = ::LYHouse;
	actionIndex = 0;
	gameStatus = LYPINEAPPLE_GAME_ONGOING;
	seat1 = (LYPineappleSeat*)(this->fetchSeat(LYSeat1)).get();
	seat2 = (LYPineappleSeat*)(this->fetchSeat(LYSeat2)).get();
	if (table->total_seats > 2) {
		seat3 = (LYPineappleSeat*)(this->fetchSeat(LYSeat3)).get();
	}
}

/*
 * sortSeats: sort all seats according to duty_button, ..., button
 */
void LYPineappleGame::sortSeats()
{
    this->sortedSeats.clear();
    if (this->seatsOfTable.size() < 2) return;
    LYSeatPtr seat = this->table->fetchNextOccupiedSeat(btnSeatNo);
    while (seat.get()!= NULL && seat.get() != NULL && seat->seatNo != this->btnSeatNo) {
//    	LYPineappleSeat *ps = (LYPineappleSeat *)seat.get();
    	sortedSeats.push_back(seat);
    	seat = table->fetchNextOccupiedSeat(seat->seatNo);
    	if (NULL == seat.get()) return;
    }
    sortedSeats.push_back(this->buttonSeat());
}

LYSeatPtr LYPineappleGame::buttonSeat()
{
	return table->fetchSeat(this->btnSeatNo);
}

LYPineappleGame::~LYPineappleGame()
{

}

/*
 * 服务器CreateGame时调用本构造函数，将会自动生成后续的每手牌的Action
 */
LYPineappleGame::LYPineappleGame(const std::string &game_id,
		std::vector<LYSeatPtr> &all_seats,
		LYSeatPtr btnSeat,
		LYPineappleTable *tbl, LYHoldemAlgorithmDelegate *had, bool pendingForRestore, std::string prof) :
		LYGame(game_id, all_seats, (LYTable *)tbl, (LYPokerAlgorithmDelegate *)had)
{
	this->reset();
	_id = game_id;
	// TODO Auto-generated constructor stub
	btnSeatNo = LYHouse;
	seqNo = 0;
//	gameType = game_type;
	profile = prof;
	//reset seats
	if (!pendingForRestore) {
		btnSeatNo = btnSeat->seatNo;
		sortSeats();
		//init dynamics
		dutySeatNo = this->table->fetchNextOccupiedSeat(btnSeatNo)->seatNo;

		resetSeatsForNextGame();
		//gen actions
		//客户端也调用genActions(),只在于生成Action栈，用于移动Duty
		genActions();
	}
}

void LYPineappleGame::resetSeatsForNextGame()
{
	std::vector<LYSeatPtr>::iterator it = sortedSeats.begin();
	for (; it!=sortedSeats.end(); it++) {
		LYPineappleSeat* seat = (LYPineappleSeat*)(*it).get();
		seat->resetForNewGame();
	}
}

void LYPineappleGame::genActions()
{
	if (actions.size() > 0) return;

	deck.reset();
	for (unsigned int i = 0; i<5; i++) {
		std::vector<LYSeatPtr>::iterator it = sortedSeats.begin();
		for (; it!=sortedSeats.end(); it++) {
			LYPineappleSeat* seat = (LYPineappleSeat*)(*it).get();
			LYPineappleAction action;
			if (seat->gameStatus == LYFANTASY) {
				if (i == 0) {
					action = genFantasyAction(seat->playerId, seat->seatNo);
				} else {
					continue;
				}
			} else {
				action = genHandAction(seat->playerId, seat->seatNo, i+1);
			}
			actions.push_back(action);
		}
	}
}

LYPineappleAction& LYPineappleGame::popAction()
{
	LYPineappleAction& action = actions[actionIndex++];
	dutySeatNo = (enum LYApplicant)(action.applicant);
	return action;
}

LYPineappleAction LYPineappleGame::genFantasyAction(const std::string& player, enum LYApplicant seat)
{
	std::vector<LYCard> cards;
	for (unsigned int i=0; i<14; i++) {
		LYCard card = deck.pop();
		cards.push_back(card);
	}

	LYPineappleAction action;
	action.issueFirstHand(cards, this->table->_id, player, seat);
	return action;
}

LYPineappleAction LYPineappleGame::genHandAction(const std::string& player, enum LYApplicant seat, unsigned int round)
{
	LYPineappleAction action;
	if (round > 5) return action;
	unsigned int numbers = 3;
	if (round == 1) numbers = 5;

	std::vector<LYCard> cards;
	for (unsigned int i=0; i<numbers; i++) {
		LYCard card = deck.pop();
		cards.push_back(card);
	}

	action.issueHand(cards, this->table->_id, player, seat, round);
	return action;
}

/*
 * 判断Action是否有效
 * 根据不同的Action调用不同的函数处理 onCall/onFold/...
 * 成功后再生成Scene
 */
bool LYPineappleGame::onAction(LYPineappleAction &action)
{
	bool ret = true;
	if (!isValidAction(action)) {
		return false;
	}
	LYSeatPtr seatPtr = table->fetchSeat((enum LYApplicant)action.applicant);
	if (seatPtr.get() == NULL) {
		return false;
	}
	LYPineappleSeat* seat = (LYPineappleSeat *)seatPtr.get();
//	if (action.verb == LYHousePollingDecision) {
//		this->onPolling(seat);
//		return true;
//	}
	switch (action.verb)
	{
	case LYCMD_ISSUE_FIRST_HAND:
	case LYCMD_ISSUE_SECOND_HAND:
	case LYCMD_ISSUE_THIRD_HAND:
	case LYCMD_ISSUE_FOUTH_HAND:
	case LYCMD_ISSUE_LAST_HAND:
//		std::cout << "issuing cards ...." << action.verb << std::endl;
		this->onIssueCards(seat, action.bricks);
		break;
	case LYCMD_PINUP:
	{
		ret = this->onPinup(seat, action.bricks);
		if (action.player == "timeout") {
			seat->timeoutCount = 1;
		} else {
			seat->timeoutCount = 0;
		}
		if (action.player == "leaveseat") {
			seat->timeoutCount = 0;
			seat->status = LYSeatReserved;
			seat->gameStatus = LYABORT;
		}
	}
		break;

	default:
		return false;
	}

//	this->genNewScene();

	return ret;
}

void LYPineappleGame::onIssueCards(LYPineappleSeat* seat, std::vector<LYPineappleBrick>& bricks)
{
	seat->issueCards(bricks);
}

bool LYPineappleGame::onPinup(LYPineappleSeat* seat, std::vector<LYPineappleBrick>& bricks)
{
	int rc = seat->pinup(bricks);
	if (rc == -1) return false;
	return true;
}

/*
 * 判断是否合法的动作
 */
bool LYPineappleGame::isValidAction(LYPineappleAction &action)
{
	if (this->dutySeatNo != action.applicant) {
		return false;
	}
	LYSeatPtr seatPtr = table->fetchSeat((enum LYApplicant) action.applicant);
	if (seatPtr.get() == NULL) {
		return false;
	}
	LYPineappleSeat* seat = (LYPineappleSeat *)seatPtr.get();

	switch (action.verb)
	{
	case LYCMD_ISSUE_FIRST_HAND:
		if (action.bricks.size() == 14) {
			if (seat->gameStatus != LYFANTASY) {
				return false;
			}
			return true;
		}
		if (action.bricks.size() != 5) {
			return false;
		}
		return true;
		break;
	case LYCMD_ISSUE_SECOND_HAND:
	case LYCMD_ISSUE_THIRD_HAND:
	case LYCMD_ISSUE_FOUTH_HAND:
	case LYCMD_ISSUE_LAST_HAND:
		if (action.bricks.size() != 3) {
			return false;
		}
		return true;
		break;
	case LYCMD_PINUP:
	{
		//cards必须是之前发的牌
		std::vector<LYPineappleBrick>::iterator it = action.bricks.begin();
		for (; it!=action.bricks.end(); it++) {
			std::vector<LYCard>::iterator it2 = seat->cardsOnHand.begin();
			bool hasThisCard = false;
			for (; it2!=seat->cardsOnHand.end(); it2++) {
				if ((*it2) == flipCard) return true; // added 20141126，如果是客户端，则判断是否被过滤后的牌，此时允许
				if (*it2 == (*it).card) {
					hasThisCard = true;
					break;
				}
			}
			if (!hasThisCard) return false;
		}
	}
		break;

	default:
		return false;
	}
	return true;
}

void LYPineappleGame::dutyMoveToNextSeat()
{
//    if (LYHouse == dutySeatNo) return;
//    unsigned int i = 0;
//    while (i<sortedSeats.size()) {
//    	i++;
//    	LYSeatPtr seat = this->fetchNextSeatInGame(dutySeatNo);
//    	if (NULL == seat.get()) {
//    		//just one seat left in game
//    		return;
//    	}
//        dutySeatNo = seat->seatNo;
//        LYPineappleSeat *pineappleSeat = (LYPineappleSeat *)dutySeat().get();
//        if (pineappleSeat && pineappleSeat->gameStatus != LYFANTASY) {
//            break;
//        }
//    }
	return; //20141111 do nothing
}

void LYPineappleGame::gameOver()
{
//LY_LOG_DBG("enter gameover:" << this->sortedSeats.size());

	if (this->sortedSeats.size() > 2) {
//LY_LOG_DBG("seats > 2 " );
		if (!(seat1->finishGame() && seat2->finishGame()) && seat3->finishGame()) {
			return;
		}
	} else  {
//LY_LOG_DBG("seats = 2 " );
		if (!(seat1->finishGame() && seat2->finishGame())) {
//LY_LOG_DBG("seats = 2 not finished" );
			return;
		}
	}

	gameStatus = LYPINEAPPLE_GAME_CLOSED;
//LY_LOG_DBG("gametype:" << gameType);

	seat1->prepareForRace((LYHoldemAlgorithmDelegate*)pad);
	seat2->prepareForRace((LYHoldemAlgorithmDelegate*)pad);
	if (this->sortedSeats.size() > 2) {
		seat3->prepareForRace((LYHoldemAlgorithmDelegate*)pad);
	}

	seat1->score_race = 0;
	seat2->score_race = 0;
	int score = seat1->raceAgainst(*seat2, this->seat1_vs_seat2);
	seat1->score += score;
	seat2->score -= score;
	seat1->score_race += score;
	seat2->score_race -= score;
	if (this->sortedSeats.size() > 2) {
		seat3->score_race = 0;
		score = seat1->raceAgainst(*seat3, this->seat1_vs_seat3);
		seat1->score += score;
		seat3->score -= score;
		seat1->score_race += score;
		seat3->score_race -= score;
		score = seat2->raceAgainst(*seat3, this->seat2_vs_seat3);
		seat2->score += score;
		seat3->score -= score;
		seat2->score_race += score;
		seat3->score_race -= score;
	}
}

LYSeatPtr LYPineappleGame::dutySeat()
{
	return table->fetchSeat(dutySeatNo);
}

LYSeatPtr LYPineappleGame::fetchNextSeatInGame(LYApplicant seat_no)
{
	if ((unsigned int)seat_no < 1 || (unsigned int)seat_no > 3) {
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
		LYPineappleSeat *seat = (LYPineappleSeat *)((*it).get());
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

bool LYPineappleGame::hasFantasy()
{
	std::vector<LYSeatPtr>::iterator it = sortedSeats.begin();
	bool has_fantasy = false;
	for (; it!=sortedSeats.end(); it++) {
		LYSeatPtr cur = *it;
		LYPineappleSeat *seat = (LYPineappleSeat *)((*it).get());
		if (seat->isFantasy()) {
			return true;
		}
	}
	return has_fantasy;
}
