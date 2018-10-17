/*
 * ClientPokerAlgorithmDelegate.cpp
 *
 *  Created on: 2014-10-31
 *      Author: caiqingfeng
 */
#include "ClientPokerAlgorithmDelegate.h"
#include "holdem/src/LYHandStrength.h"

ClientPokerAlgorithmDelegate *ClientPokerAlgorithmDelegate::_instance = NULL;

ClientPokerAlgorithmDelegate::ClientPokerAlgorithmDelegate() {
	// TODO Auto-generated constructor stub

}

ClientPokerAlgorithmDelegate::~ClientPokerAlgorithmDelegate() {
	// TODO Auto-generated destructor stub
}

ClientPokerAlgorithmDelegate *ClientPokerAlgorithmDelegate::Instance() {
	// TODO Auto-generated constructor stub
	if (NULL == _instance) {
		_instance = new ClientPokerAlgorithmDelegate();
	}
	return _instance;
}

void ClientPokerAlgorithmDelegate::doShuffle(std::vector<LYCard> &deck)
{
}


