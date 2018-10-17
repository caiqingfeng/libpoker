/*
 * ServerPokerAlgorithmDelegate.cpp
 *
 *  Created on: 2013-7-31
 *      Author: caiqingfeng
 */
#include <boost/random.hpp>
#include <boost/generator_iterator.hpp>
#include <boost/regex.hpp>

#include "ServerPokerAlgorithmDelegate.h"
#include "holdem/src/LYHandStrength.h"

typedef boost::mt19937 LYRng; //random number generator

ServerPokerAlgorithmDelegate *ServerPokerAlgorithmDelegate::_instance = NULL;

ServerPokerAlgorithmDelegate::ServerPokerAlgorithmDelegate() {
	// TODO Auto-generated constructor stub

}

ServerPokerAlgorithmDelegate::~ServerPokerAlgorithmDelegate() {
	// TODO Auto-generated destructor stub
}

ServerPokerAlgorithmDelegate *ServerPokerAlgorithmDelegate::Instance() {
	// TODO Auto-generated constructor stub
	if (NULL == _instance) {
		_instance = new ServerPokerAlgorithmDelegate();
	}
	return _instance;
}

void ServerPokerAlgorithmDelegate::doShuffle(std::vector<LYCard> &deck)
{
	LYRng rng(std::time(0)); //must set a seed, otherwise always get the same result
	boost::uniform_int<> deckRange(1, 52);
	boost::variate_generator<LYRng, boost::uniform_int<> > oneDeck(rng, deckRange);
	std::random_shuffle(deck.begin(), deck.end(), oneDeck);
}

//LYHandStrengthPtr ServerPokerAlgorithmDelegate::genHandStrength(LYHoldemSeat *seat,
//			const std::vector<LYCard> &communityCards)
//{
//}
