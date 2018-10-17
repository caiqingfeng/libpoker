/*
 * LYPokerAlgorithmDelegate.cpp
 *
 *  Created on: 2013-7-31
 *      Author: caiqingfeng
 */

#include <boost/random.hpp>
#include <boost/generator_iterator.hpp>
#include <boost/regex.hpp>

#include "LYPokerAlgorithmDelegate.h"

typedef boost::mt19937 LYRng; //random number generator

LYPokerAlgorithmDelegate::LYPokerAlgorithmDelegate() {
	// TODO Auto-generated constructor stub

}

LYPokerAlgorithmDelegate::~LYPokerAlgorithmDelegate() {
	// TODO Auto-generated destructor stub
}

void LYPokerAlgorithmDelegate::doShuffle(std::vector<LYCard> &deck)
{
	LYRng rng(std::time(0)); //must set a seed, otherwise always get the same result
	boost::uniform_int<> deckRange(1, deck.size());
	boost::variate_generator<LYRng, boost::uniform_int<> > oneDeck(rng, deckRange);
	std::random_shuffle(deck.begin(), deck.end(), oneDeck);
}
