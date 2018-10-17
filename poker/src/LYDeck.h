/*
 * LYDeck.h
 *
 *  Created on: 2013-3-27
 *      Author: caiqingfeng
 */

#ifndef LYDECK_H_
#define LYDECK_H_

#include <vector>

#include "LYPokerConstants.h"
#include "LYCard.h"
#include "LYPokerAlgorithmDelegate.h"

//class LYCard;

extern LYCard cA, cK, cQ, cJ, cT, c9, c8, c7, c6, c5, c4, c3, c2;
extern LYCard dA, dK, dQ, dJ, dT, d9, d8, d7, d6, d5, d4, d3, d2;
extern LYCard hA, hK, hQ, hJ, hT, h9, h8, h7, h6, h5, h4, h3, h2;
extern LYCard sA, sK, sQ, sJ, sT, s9, s8, s7, s6, s5, s4, s3, s2;
extern LYCard flipCard, smallGhost, bigGhost;
extern LYCard sortedDeck[];

class LYDeck {
public:
	LYDeck(LYPokerAlgorithmDelegate *sd=NULL);
	LYDeck(const std::vector<LYCard> &dk, LYPokerAlgorithmDelegate *sd=NULL);
	virtual ~LYDeck();

public:
	std::vector<LYCard> deck;
	LYPokerAlgorithmDelegate *shuffleDelegate; // delegating the task of shuffle

private:
	unsigned int index;

public:
	void test();
	void shuffle(std::vector<int> & vec);
	std::vector<LYCard> shuffle();
	LYCard pop();
	void reset();
};

#endif /* LYDECK_H_ */
