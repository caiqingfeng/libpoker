/*
 * LYPokerAlgorithmDelegate.h
 *
 *  Created on: 2013-7-31
 *      Author: caiqingfeng
 */

#ifndef LYPOKERALGORITHMDELEGATE_H_
#define LYPOKERALGORITHMDELEGATE_H_

#include <vector>
#include  "LYCard.h"

/*
 * Server will implements this interface to do shuffle and generate handstrength for each seat
 */
class LYDeck;

class LYPokerAlgorithmDelegate {
public:
	LYPokerAlgorithmDelegate();
	virtual ~LYPokerAlgorithmDelegate();

public:
	virtual void doShuffle(std::vector<LYCard> &deck);
	virtual std::vector<LYCard> initDeck() {return std::vector<LYCard>();};
};

#endif /* LYPOKERALGORITHMDELEGATE_H_ */
