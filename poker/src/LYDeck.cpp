/*
 * LYDeck.cpp
 *
 *  Created on: 2013-3-27
 *      Author: caiqingfeng
 */
#include <iostream>

#include "LYDeck.h"
#include "LYCard.h"

LYCard cA(ACE, Clubs);
LYCard cK(KING, Clubs);
LYCard cQ(QUEEN, Clubs);
LYCard cJ(JACK, Clubs);
LYCard cT(TEN, Clubs);
LYCard c9(NINE, Clubs);
LYCard c8(EIGHT, Clubs);
LYCard c7(SEVEN, Clubs);
LYCard c6(SIX, Clubs);
LYCard c5(FIVE, Clubs);
LYCard c4(FOUR, Clubs);
LYCard c3(THREE, Clubs);
LYCard c2(TWO, Clubs);
LYCard dA(ACE, Diamonds);
LYCard dK(KING, Diamonds);
LYCard dQ(QUEEN, Diamonds);
LYCard dJ(JACK, Diamonds);
LYCard dT(TEN, Diamonds);
LYCard d9(NINE, Diamonds);
LYCard d8(EIGHT, Diamonds);
LYCard d7(SEVEN, Diamonds);
LYCard d6(SIX, Diamonds);
LYCard d5(FIVE, Diamonds);
LYCard d4(FOUR, Diamonds);
LYCard d3(THREE, Diamonds);
LYCard d2(TWO, Diamonds);
LYCard hA(ACE, Hearts);
LYCard hK(KING, Hearts);
LYCard hQ(QUEEN, Hearts);
LYCard hJ(JACK, Hearts);
LYCard hT(TEN, Hearts);
LYCard h9(NINE, Hearts);
LYCard h8(EIGHT, Hearts);
LYCard h7(SEVEN, Hearts);
LYCard h6(SIX, Hearts);
LYCard h5(FIVE, Hearts);
LYCard h4(FOUR, Hearts);
LYCard h3(THREE, Hearts);
LYCard h2(TWO, Hearts);
LYCard sA(ACE, Spades);
LYCard sK(KING, Spades);
LYCard sQ(QUEEN, Spades);
LYCard sJ(JACK, Spades);
LYCard sT(TEN, Spades);
LYCard s9(NINE, Spades);
LYCard s8(EIGHT, Spades);
LYCard s7(SEVEN, Spades);
LYCard s6(SIX, Spades);
LYCard s5(FIVE, Spades);
LYCard s4(FOUR, Spades);
LYCard s3(THREE, Spades);
LYCard s2(TWO, Spades);
LYCard flipCard(NOFACE, Nosuit);
LYCard smallGhost(SMALL_GHOST, Nosuit);
LYCard bigGhost(BIG_GHOST, Nosuit);
LYCard sortedDeck[55] = {flipCard, sA, hA, dA, cA, sK, hK, dK, cK, sQ, hQ, dQ, cQ, sJ, hJ, dJ, cJ,
                         sT, hT, dT, cT, s9, h9, d9, c9, s8, h8, d8, c8, s7, h7, d7, c7, s6, h6, d6, c6,
                         s5, h5, d5, c5, s4, h4, d4, c4, s3, h3, d3, c3, s2, h2, d2, c2,
							smallGhost, bigGhost};

LYDeck::LYDeck(LYPokerAlgorithmDelegate *sd) {
	// TODO Auto-generated constructor stub
	deck.clear();
	index = 0;
	if (sd == NULL) {
		deck.push_back(cA);
		deck.push_back(cK);
		deck.push_back(cQ);
		deck.push_back(cJ);
		deck.push_back(cT);
		deck.push_back(c9);
		deck.push_back(c8);
		deck.push_back(c7);
		deck.push_back(c6);
		deck.push_back(c5);
		deck.push_back(c4);
		deck.push_back(c3);
		deck.push_back(c2);
		deck.push_back(dA);
		deck.push_back(dK);
		deck.push_back(dQ);
		deck.push_back(dJ);
		deck.push_back(dT);
		deck.push_back(d9);
		deck.push_back(d8);
		deck.push_back(d7);
		deck.push_back(d6);
		deck.push_back(d5);
		deck.push_back(d4);
		deck.push_back(d3);
		deck.push_back(d2);
		deck.push_back(hA);
		deck.push_back(hK);
		deck.push_back(hQ);
		deck.push_back(hJ);
		deck.push_back(hT);
		deck.push_back(h9);
		deck.push_back(h8);
		deck.push_back(h7);
		deck.push_back(h6);
		deck.push_back(h5);
		deck.push_back(h4);
		deck.push_back(h3);
		deck.push_back(h2);
		deck.push_back(sA);
		deck.push_back(sK);
		deck.push_back(sQ);
		deck.push_back(sJ);
		deck.push_back(sT);
		deck.push_back(s9);
		deck.push_back(s8);
		deck.push_back(s7);
		deck.push_back(s6);
		deck.push_back(s5);
		deck.push_back(s4);
		deck.push_back(s3);
		deck.push_back(s2);
	} else {
		deck = sd->initDeck();
	}

	shuffleDelegate = sd;

	this->shuffle();
}

LYDeck::LYDeck(const std::vector<LYCard> &dk, LYPokerAlgorithmDelegate *sd) {
	// TODO Auto-generated destructor stub
	deck = dk;
	index = 0;
	shuffleDelegate = sd;
	this->shuffle();
}

LYDeck::~LYDeck() {
	// TODO Auto-generated destructor stub
	deck.clear();
}

void  LYDeck::test()
{
//	LYRng rng(std::time(0));
//	boost::uniform_int<> deckRange(1, 52);
//	boost::variate_generator<LYRng, boost::uniform_int<> > oneDeck(rng, deckRange);
//	for (int i=0; i<52; i++) {
//		int n = oneDeck();
//		std::cout << n << std::endl;
//	}
}

void LYDeck::shuffle(std::vector<int> & vec)
{
//	LYRng rng(std::time(0)); //must set a seed, otherwise always get the same result
//	boost::uniform_int<> deckRange(1, 52);
//	boost::variate_generator<LYRng, boost::uniform_int<> > oneDeck(rng, deckRange);
//	std::random_shuffle(vec.begin(), vec.end(), oneDeck);
}

std::vector<LYCard> LYDeck::shuffle()
{
	if (NULL == shuffleDelegate) {
		return deck;
	}
	shuffleDelegate->doShuffle(deck);
	return deck;
}

LYCard LYDeck::pop()
{
	return this->deck[index++];
}

void LYDeck::reset()
{
	index = 0;
}
