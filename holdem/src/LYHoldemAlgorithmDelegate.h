/*
 * LYHoldemAlgorithmDelegate.h
 *
 *  Created on: 2013-7-31
 *      Author: caiqingfeng
 */

#ifndef LYHOLDEMALGORITHMDELEGATE_H_
#define LYHOLDEMALGORITHMDELEGATE_H_

//#include "LYHandStrength.h"
//#include "LYHoldemSeat.h"

#include "poker/src/LYPokerAlgorithmDelegate.h"
#include <memory>
#include "holdem/src/LYHandStrength.h"
#include "holdem/src/LYHoldemConstants.h"

/*
 * 20141110 为了支持Pineapple带王的强度算法，参数中加多一个CapHs
 * Holdem不影响
 */
class LYHoldemSeat;
class LYHandStrength;
typedef ::std::shared_ptr<LYHandStrength> LYHandStrengthPtr;

class LYHoldemAlgorithmDelegate : public LYPokerAlgorithmDelegate{
public:
	LYHoldemAlgorithmDelegate();
	virtual ~LYHoldemAlgorithmDelegate();

public:
	virtual std::vector<LYCard> initDeck();

public:
	virtual LYHandStrengthPtr genHandStrength(LYHoldemSeat *seat,
			const std::vector<LYCard> &communityCards) {return LYHandStrengthPtr();};

//	virtual bool isStraightWithPattern(std::string const &cs, std::string const &ptn){return false;};
//	virtual LYHandStrength * isStraightFlush(LYHandStrength *hs){return NULL;};
//	virtual LYHandStrength * isFourOfAKind(LYHandStrength *hs){return NULL;};
//	virtual LYHandStrength * isFullHouse(LYHandStrength *hs){return NULL;};
//	virtual LYHandStrength * isFlush(LYHandStrength *hs){return NULL;};
//	virtual LYHandStrength * isStraight(LYHandStrength *hs){return NULL;};
//	virtual LYHandStrength * isThreeOfAKind(LYHandStrength *hs){return NULL;};
//	virtual LYHandStrength * isTwoPair(LYHandStrength *hs){return NULL;};
//	virtual LYHandStrength * isOnePair(LYHandStrength *hs){return NULL;};
//	virtual LYHandStrength * isHighCard(LYHandStrength *hs){return NULL;};
	virtual bool isStraightWithPattern(std::string const &cs, std::string const &ptn);
	virtual LYHandStrength * isStraightFlush(LYHandStrength *hs, LYHandStrength* capHs=NULL);
	virtual LYHandStrength * isFourOfAKind(LYHandStrength *hs, LYHandStrength* capHs=NULL);
	virtual LYHandStrength * isFullHouse(LYHandStrength *hs, LYHandStrength* capHs=NULL);
	virtual LYHandStrength * isFlush(LYHandStrength *hs, LYHandStrength* capHs=NULL);
	virtual LYHandStrength * isStraight(LYHandStrength *hs, LYHandStrength* capHs=NULL);
	virtual LYHandStrength * isThreeOfAKind(LYHandStrength *hs, LYHandStrength* capHs=NULL);
	virtual LYHandStrength * isTwoPair(LYHandStrength *hs, LYHandStrength* capHs=NULL);
	virtual LYHandStrength * isOnePair(LYHandStrength *hs, LYHandStrength* capHs=NULL);
	virtual LYHandStrength * isHighCard(LYHandStrength *hs, LYHandStrength* capHs=NULL);
};

#endif /* LYHOLDEMALGORITHMDELEGATE_H_ */
