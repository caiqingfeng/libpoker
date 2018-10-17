/*
 * ClientPokerAlgorithmDelegate.h
 *
 *  Created on: 2013-7-31
 *      Author: caiqingfeng
 */

#ifndef CLIENTPOKERALGORITHMDELEGATE_H_
#define CLIENTPOKERALGORITHMDELEGATE_H_

#include "holdem/src/LYHoldemAlgorithmDelegate.h"

class ClientPokerAlgorithmDelegate : public LYHoldemAlgorithmDelegate {
protected:
	ClientPokerAlgorithmDelegate();

public:
    static ClientPokerAlgorithmDelegate *Instance();
	virtual ~ClientPokerAlgorithmDelegate();
	virtual void doShuffle(std::vector<LYCard> &deck);

private:
	static ClientPokerAlgorithmDelegate *_instance;
};

#endif /* ServerPokerAlgorithmDelegate_H_ */
