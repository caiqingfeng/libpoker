/*
 * LYHoldemAlgorithmDelegate.h
 *
 *  Created on: 2013-7-31
 *      Author: caiqingfeng
 */

#ifndef LYPINEAPPLEALGORITHMDELEGATE_H_
#define LYPINEAPPLEALGORITHMDELEGATE_H_

#include "poker/src/LYPokerAlgorithmDelegate.h"
#include <memory>
#include "holdem/src/LYHoldemAlgorithmDelegate.h"

class LYHandStrength;
class LYPineappleSeat;
typedef ::std::shared_ptr<LYHandStrength> LYHandStrengthPtr;

/*
 * 没有大小王的算法，就直接调用Holdem的
 * 这个算法是针对有大小王的（一幅牌里只能有一个）
 */
class LYPineappleAlgorithmDelegate : public LYHoldemAlgorithmDelegate{
public:
	virtual std::vector<LYCard> initDeck();

public:
	LYPineappleAlgorithmDelegate();
	virtual ~LYPineappleAlgorithmDelegate();

	virtual LYHandStrengthPtr genHandStrength(LYPineappleSeat *seat,
			const std::vector<LYCard> &communityCards) {return LYHandStrengthPtr();};

	virtual LYHandStrength * isStraightFlush(LYHandStrength *hs, LYHandStrength* catHs = NULL);
	virtual LYHandStrength * isFourOfAKind(LYHandStrength *hs, LYHandStrength* catHs = NULL);
	virtual LYHandStrength * isFullHouse(LYHandStrength *hs, LYHandStrength* catHs = NULL);
	virtual LYHandStrength * isFlush(LYHandStrength *hs, LYHandStrength* catHs = NULL);
	virtual LYHandStrength * isStraight(LYHandStrength *hs, LYHandStrength* catHs = NULL);
	virtual LYHandStrength * isThreeOfAKind(LYHandStrength *hs, LYHandStrength* catHs = NULL);
	virtual LYHandStrength * isTwoPair(LYHandStrength *hs, LYHandStrength* catHs = NULL);
	virtual LYHandStrength * isOnePair(LYHandStrength *hs, LYHandStrength* catHs = NULL);
	virtual LYHandStrength * isHighCard(LYHandStrength *hs, LYHandStrength* catHs = NULL);
};

#endif /* LYHOLDEMALGORITHMDELEGATE_H_ */
