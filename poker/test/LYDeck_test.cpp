/*
 * LYDeck_test.cpp
 *
 *  Created on: 2013-3-27
 *      Author: caiqingfeng
 */
#include <gtest/gtest.h>
#include <boost/foreach.hpp>

#include "src/LYDeck.h"

class LYDeck_tests : public ::testing::Test
{
protected:

    void SetUp()
    {
    	deck = new LYDeck();
    }

    void TearDown()
    {
    	delete deck;
    }
public:
    LYDeck *deck;
};

TEST_F(LYDeck_tests, globalCards)
{
	ASSERT_EQ(cA.face, ACE);
	ASSERT_STREQ(cA.toString().c_str(), "Ac");
	ASSERT_EQ(deck->deck.size(), 52);
	int clubs = 0;
	int hearts = 0;
	int diamonds = 0;
	int spades = 0;
	int Ace = 0;
	int Two = 0;
	int Three = 0;
	int Four = 0;
	int Five = 0;
	int Six = 0;
	int Seven = 0;
	int Eight = 0;
	int Nine = 0;
	int Ten = 0;
	int Jack = 0;
	int Queen = 0;
	int King = 0;
	BOOST_FOREACH(LYCard cd, deck->deck)
	{
		switch (cd.face) {
		case ACE:
			Ace++;
			break;
		case TWO:
			Two++;
			break;
		case THREE:
			Three++;
			break;
		case FOUR:
			Four++;
			break;
		case FIVE:
			Five++;
			break;
		case SIX:
			Six++;
			break;
		case SEVEN:
			Seven++;
			break;
		case EIGHT:
			Eight++;
			break;
		case NINE:
			Nine++;
			break;
		case TEN:
			Ten++;
			break;
		case JACK:
			Jack++;
			break;
		case QUEEN:
			Queen++;
			break;
		case KING:
			King++;
			break;
		default:
			break;
		}
		switch (cd.suit) {
		case Clubs:
			clubs ++;
			break;
		case Diamonds:
			diamonds ++;
			break;
		case Hearts:
			hearts ++;
			break;
		case Spades:
			spades ++;
			break;
		default:
			break;
		}
	}
	ASSERT_EQ(clubs, 13);
	ASSERT_EQ(diamonds, 13);
	ASSERT_EQ(hearts, 13);
	ASSERT_EQ(spades, 13);
	ASSERT_EQ(Ace, 4);
	ASSERT_EQ(Two, 4);
	ASSERT_EQ(Three, 4);
	ASSERT_EQ(Four, 4);
	ASSERT_EQ(Five, 4);
	ASSERT_EQ(Six, 4);
	ASSERT_EQ(Seven, 4);
	ASSERT_EQ(Eight, 4);
	ASSERT_EQ(Nine, 4);
	ASSERT_EQ(Ten, 4);
	ASSERT_EQ(Jack, 4);
	ASSERT_EQ(Queen, 4);
	ASSERT_EQ(King, 4);
}

TEST_F(LYDeck_tests, shuffle)
{
	std::vector<int> vec;
	for (int i=0; i<52; i++) {
		vec.push_back(i+1);
	}
	deck->shuffle(vec);
//	BOOST_FOREACH(int i, vec)
//	{
//		//std::cout << i << std::endl;
//	}
	deck->shuffle(vec);
	//std::cout << "next shuffle" << std::endl;
//	BOOST_FOREACH(int i, vec)
//	{
//		//std::cout << i << std::endl;
//	}
}

TEST_F(LYDeck_tests, shuffleCard)
{
	std::vector<LYCard> cards;
	cards = deck->shuffle();
//std::cout << cards.size() << std::endl;
	BOOST_FOREACH(LYCard cd, cards)
	{
		//std::cout << cd.toString() << std::endl;
	}
	cards = deck->shuffle();
	//std::cout << "next shuffle" << std::endl;
	BOOST_FOREACH(LYCard cd, cards)
	{
		//std::cout << cd.toString() << std::endl;
	}
}

TEST_F(LYDeck_tests, newDeck)
{
	LYDeck newDeck;
	std::vector<LYCard> cards = newDeck.deck;
	ASSERT_EQ(cards.size(), 52);
	//cards = deck->shuffle();
//std::cout << cards.size() << std::endl;
	BOOST_FOREACH(LYCard cd, cards)
	{
		//std::cout << cd.toString() << std::endl;
	}
	LYDeck newDeck2(cards);
	cards = newDeck2.deck; 
	ASSERT_EQ(cards.size(), 52);
	//std::cout << "next shuffle" << std::endl;
	BOOST_FOREACH(LYCard cd, cards)
	{
		//std::cout << cd.toString() << std::endl;
	}
}
