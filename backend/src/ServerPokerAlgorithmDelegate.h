/*
 * ServerPokerAlgorithmDelegate.h
 *
 *  Created on: 2013-7-31
 *      Author: caiqingfeng
 */

#ifndef SERVERPOKERALGORITHMDELEGATE_H_
#define SERVERPOKERALGORITHMDELEGATE_H_

#include "holdem/src/LYHoldemAlgorithmDelegate.h"

class ServerPokerAlgorithmDelegate : public LYHoldemAlgorithmDelegate {
protected:
	ServerPokerAlgorithmDelegate();

public:
    static ServerPokerAlgorithmDelegate *Instance();
	virtual ~ServerPokerAlgorithmDelegate();
	virtual void doShuffle(std::vector<LYCard> &deck);
//	virtual LYHandStrengthPtr genHandStrength(LYHoldemSeat *seat,
//			const std::vector<LYCard> &communityCards);

public:
//	virtual bool isStraightWithPattern(std::string const &cs, std::string const &ptn);
//	virtual LYHandStrength * isStraightFlush(LYHandStrength *hs);
//	virtual LYHandStrength * isFourOfAKind(LYHandStrength *hs);
//	virtual LYHandStrength * isFullHouse(LYHandStrength *hs);
//	virtual LYHandStrength * isFlush(LYHandStrength *hs);
//	virtual LYHandStrength * isStraight(LYHandStrength *hs);
//	virtual LYHandStrength * isThreeOfAKind(LYHandStrength *hs);
//	virtual LYHandStrength * isTwoPair(LYHandStrength *hs);
//	virtual LYHandStrength * isOnePair(LYHandStrength *hs);
//	virtual LYHandStrength * isHighCard(LYHandStrength *hs);

private:
	static ServerPokerAlgorithmDelegate *_instance;
};

#endif /* ServerPokerAlgorithmDelegate_H_ */
