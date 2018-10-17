/*
 * LYHandStrength.h
 *
 *  Created on: 2013-3-22
 *      Author: caiqingfeng
 */

#ifndef LYHANDSTRENGTH_H_
#define LYHANDSTRENGTH_H_

#include <vector>
#include <memory>

#include "poker/src/LYPokerConstants.h"
#include "LYHoldemConstants.h"
#include "poker/src/LYCard.h"
#include "LYHoldemAlgorithmDelegate.h"

/*
 从给定的5-7张牌里找出最大的成牌。
 Ranking          Rank-card(s)           Kicker-card(s)
 ------------------------------------------------------
 HighCard         Top card               Remaining 4
 OnePair          Pair card              Remaining 3
 TwoPair          1st & 2nd Pair card    Remaining 1
 ThreeOfAKind     Trips card             Remaining 2
 Straight         Top card               -
 Flush            Flush cards            -
 FullHouse        Trips & Pair card      -
 FourOfAKind      FourOfAKind card       Remaining 1
 StraightFlush    Top card               -
 */

extern const char * StringifiedStraightFlush;
extern const char * StringifiedFourOfAKind;
extern const char * StringifiedFullHouse;
extern const char * StringifiedFlush;
extern const char * StringifiedStraight;
extern const char * StringifiedThreeOfAKind;
extern const char * StringifiedTwoPairs;
extern const char * StringifiedOnePair;
extern const char * StringifiedHighCard;

class LYHoldemAlgorithmDelegate;

class LYHandStrength {
private:
	LYHoldemAlgorithmDelegate *had;

public:
	LYHandStrength(std::vector<LYCard > const &cds, LYHoldemAlgorithmDelegate *had=NULL, LYHandStrength* capHs=NULL);
	virtual ~LYHandStrength();
	void reset();
	
	LYCard genTopRankCard();
	LYCard genTopRankCardForStraight(std::string const &cs);
	bool isStraightWithPattern(std::string const &cs, std::string const &ptn);
	bool rankHasThisCard(LYCard const &card);
	bool rankHasThisFace(LYCard const &card);
	LYHandStrength * isStraightFlush(LYHandStrength* capHs=NULL);
	LYHandStrength * isFourOfAKind(LYHandStrength* capHs=NULL);
	LYHandStrength * isFullHouse(LYHandStrength* capHs=NULL);
	LYHandStrength * isFlush(LYHandStrength* capHs=NULL);
	LYHandStrength * isStraight(LYHandStrength* capHs=NULL);
	LYHandStrength * isThreeOfAKind(LYHandStrength* capHs=NULL);
	LYHandStrength * isTwoPair(LYHandStrength* capHs=NULL);
	LYHandStrength * isOnePair(LYHandStrength* capHs=NULL);
	LYHandStrength * isHighCard(LYHandStrength* capHs=NULL);
	bool hasGhost();
	std::string genCardStringWithoutGhost();

public:
	enum LYRanking ranking;
	std::string rankingString;
	std::vector<LYCard > cards;
	std::vector<LYCard > rank;
	std::vector<LYCard > kicker;
	LYCard topRankCard;
	std::string cardString;

	bool operator < (const LYHandStrength &right) const {
		if (this->ranking < right.ranking) return true;
		if (this->ranking == right.ranking) {
			LYCard card(topRankCard.face, right.topRankCard.suit);
			if (card < right.topRankCard) {
				return true;
			}
			if (card > right.topRankCard) {
				return false;
			}
			//assert rank.size() == right.rank.size()
			for (unsigned int i=0; i<rank.size(); i++) {
				LYCard rightCard = right.rank[i];
				LYCard cd(rank[i].face, rightCard.suit);
				if (cd < rightCard) {
					return true;
				}
				if (rightCard < cd) {
					return false;
				}
			}
			//asert kicker.size() == right.kicker()
			for (unsigned int i=0; i<kicker.size(); i++) {
				LYCard rightCard = right.kicker[i];
				LYCard cd(kicker[i].face, rightCard.suit);
				if (cd < rightCard) {
					return true;
				}
				if (rightCard < cd) {
					return false;
				}
			}
			return false;
		}
		return false;
	}

	bool operator == (const LYHandStrength &right) const {
		if (this->ranking != right.ranking) return false;
		LYCard card(topRankCard.face, right.topRankCard.suit);
		if (card != right.topRankCard) {
			return false;
		}
		//assert rank.size() == right.rank.size()
		for (unsigned int i=0; i<rank.size(); i++) {
			LYCard rightCard = right.rank[i];
			LYCard cd(rank[i].face, rightCard.suit);
			if (cd != rightCard) {
				return false;
			}
		}
		//asert kicker.size() == right.kicker()
		for (unsigned int i=0; i<kicker.size(); i++) {
			LYCard rightCard = right.kicker[i];
			LYCard cd(kicker[i].face, rightCard.suit);
			if (cd != rightCard) {
				return false;
			}
		}
		return true;
	}

	bool operator > (const LYHandStrength &right) const {
		if (this->ranking > right.ranking) return true;
		if (this->ranking == right.ranking) {
			LYCard card(topRankCard.face, right.topRankCard.suit);
			if (card > right.topRankCard) {
				return true;
			}
			if (card < right.topRankCard) {
				return false;
			}
			//assert rank.size() == right.rank.size()
			for (unsigned int i=0; i<rank.size(); i++) {
				LYCard rightCard = right.rank[i];
				LYCard cd(rank[i].face, rightCard.suit);
				if (cd > rightCard) {
					return true;
				}
				if (cd < rightCard) {
					return false;
				}
			}
			//asert kicker.size() == right.kicker()
			for (unsigned int i=0; i<kicker.size(); i++) {
				LYCard rightCard = right.kicker[i];
				LYCard cd(kicker[i].face, right.kicker[i].suit);
				if (cd > rightCard) {
					return true;
				}
				if (cd < rightCard) {
					return false;
				}
			}
			return false;
		}
		return false;
	}


};

typedef ::std::shared_ptr<LYHandStrength> LYHandStrengthPtr;

#endif /* LYHANDSTRENGTH_H_ */
