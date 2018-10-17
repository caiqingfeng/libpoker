//
//  LYCard.m
//  iBuddyHoldem
//
//  Created by 蔡 庆丰 on 13-2-25.
//  Copyright (c) 2013年 蔡 庆丰. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include "LYCard.h"

using namespace std;
extern LYCard sortedDeck[];
char g_faces[] = {'X', 'Y', '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A'};

LYCard::LYCard(LYFace fe,  LYSuit st)
{
    face = fe;
    suit = st;
    //image = [[NSString stringWithFormat:@"%c_%c.png", [self getSuitSymbol], [self getFaceSymbol]] lowercaseString];
    char buf[100];
    sprintf(buf, "%c_%c.png", this->getSuitSymbol(), this->getFaceSymbol());
    image = buf;
//    std::cout << "kkkk =" << buf<< std::endl;
}

LYCard::LYCard()
{
    face = NOFACE;
    suit = Nosuit;
    image = "";
}

LYCard::LYCard(unsigned int seq)
{
	*this = sortedDeck[seq];
	switch (seq)
	{
	case 53:
		image = "small_ghost.png";
		break;
	case 54:
		image = "big_ghost.png";
		break;
	case 0:
		image = "flipcard.png";
		break;
	default:
		image = "flipcard.png";
		break;
	}

	if (seq > 0 && seq < 53) {
		char buf[100];
		sprintf(buf, "%c_%c.png", this->getSuitSymbol(), this->getFaceSymbol());
		image = buf;
	}
}

LYCard::LYCard(char fe,
                        char st)
{
//    std::cout << "ffff" << std::endl;
    switch (fe)  {
		case '1':
		case 'A':
			face = ACE;
			break;
		case '2':
			face = TWO;
			break;
		case '3':
			face = THREE;
			break;
		case '4':
			face = FOUR;
			break;
		case '5':
			face = FIVE;
			break;
		case '6':
			face = SIX;
			break;
		case '7':
			face = SEVEN;
			break;
		case '8':
			face = EIGHT;
			break;
		case '9':
			face = NINE;
			break;
		case 'T':
			face = TEN;
			break;
		case 'J':
			face = JACK;
			break;
		case 'Q':
			face = QUEEN;
			break;
		case 'K':
			face = KING;
			break;
		case 'X':
			face = SMALL_GHOST;
			break;
		case 'Y':
			face = BIG_GHOST;
			break;
    }
    switch (st) {
		case 's':
			suit = Spades;
			break;
		case 'h':
			suit = Hearts;
			break;
		case 'd':
			suit = Diamonds;
			break;
		case 'c':
			suit = Clubs;
			break;
		case 'n':
			suit = Nosuit;
			break;
    }
    char buf[100];
    sprintf(buf, "%c_%c.png", this->getSuitSymbol(), this->getFaceSymbol());
    image = buf;
}

const std::string LYCard::toString()
{
    char buf[100];
    sprintf(buf, "%c%c", this->getFaceSymbol(), this->getSuitSymbol());
    std::string str = buf;
    return str;
}

unsigned int LYCard::toInteger() const
{
	if (face == SMALL_GHOST) return 53;
	if (face == BIG_GHOST) return 54;
    if (face == NOFACE || suit == Nosuit) return 0;
	char fe = getFaceSymbol();
	char st = getSuitSymbol();

	unsigned int cursor;
	switch (fe) {
	case '2':
		cursor = 12;
		break;
	case '3':
		cursor = 11;
		break;
	case '4':
		cursor = 10;
		break;
	case '5':
		cursor = 9;
		break;
	case '6':
		cursor = 8;
		break;
	case '7':
		cursor = 7;
		break;
	case '8':
		cursor = 6;
		break;
	case '9':
		cursor = 5;
		break;
	case 'T':
		cursor = 4;
		break;
	case 'J':
		cursor = 3;
		break;
	case 'Q':
		cursor = 2;
		break;
	case 'K':
		cursor = 1;
		break;
	case 'A':
		cursor = 0;
		break;
	default :
		cursor = 0;
	}
	cursor = cursor*4;
    switch (st) {
		case 's':
			cursor += 0;
			break;
		case 'h':
			cursor += 1;
			break;
		case 'd':
			cursor += 2;
			break;
		case 'c':
			cursor += 3;
			break;
		default:
			cursor = 0;
    }
    cursor++;
	return cursor;
}

char LYCard::getFaceSymbol() const
{
    switch (face) {
		case SMALL_GHOST:
			return 'X';
		case BIG_GHOST:
			return 'Y';
		case ACE:
			return 'A';
		case TEN:
			return 'T';
		case JACK:
			return 'J';
		case QUEEN:
			return 'Q';
		case KING:
			return 'K';
		case TWO:
			return '2';
		case THREE:
			return '3';
		case FOUR:
			return '4';
		case FIVE:
			return '5';
		case SIX:
			return '6';
		case SEVEN:
			return '7';
		case EIGHT:
			return '8';
		case NINE:
			return '9';
        default:
            return '0';
    }
    return '0';
}

char LYCard::getSuitSymbol() const
{
    switch (suit) {
		case Clubs:
			return 'c';
		case Diamonds:
			return 'd';
		case Hearts:
			return 'h';
		case Spades:
			return 's';
        default:
            return 'n';
    }
    return 'n';
}

bool LYCard::isFlipCard()
{
    if (face == NOFACE && suit == Nosuit) {
        return true;
    }
    
    return false;
}


bool LYCard::isGhost()
{
    if (face == SMALL_GHOST || face == BIG_GHOST) {
        return true;
    }

    return false;
}

enum LYSuit LYCard::char2suit(char s)
{
    switch (s) {
		case 'c':
			return Clubs;
		case 'd':
			return Diamonds;
		case 'h':
			return Hearts;
		case 's':
			return Spades;
        default:
            return Spades;
    }
    return Spades;
}

std::string LYCard::getImage()
{
	return image;
}