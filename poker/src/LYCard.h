//
//  LYCard.h
//  iBuddyHoldem
//
//  Created by 蔡 庆丰 on 13-2-25.
//  Copyright (c) 2013年 蔡 庆丰. All rights reserved.
//
#ifndef _LY_CARD_H
#define _LY_CARD_H

#include <string>
#include "LYPokerConstants.h"

extern char g_faces[];

class LYCard { 
	public:
		enum LYFace face;
		enum LYSuit suit;
		std::string image; //file name of iamge

	public:
		LYCard (LYFace fe, LYSuit st);
		LYCard (char fe, char st);
		LYCard();
		LYCard(unsigned int seq);
		const std::string toString();
		char getFaceSymbol() const;
		char getSuitSymbol() const;
		bool isFlipCard();
		bool isGhost();
		std::string getImage();

		bool operator < (const LYCard &right) const {
			if (this->face < right.face) return true;
			if (this->face == right.face) {
				if (this->suit < right.suit) {
					return true;
				}
				return false;
			}
			return false;
		}

		bool operator > (const LYCard &right) const {
			if (this->face > right.face) return true;
			if (this->face == right.face) {
				if (this->suit > right.suit) {
					return true;
				}
				return false;
			}
			return false;
		}

		bool operator == (const LYCard &right) const {
			if (this->face == right.face && this->suit == right.suit) {
				return true;
			}
			return false;
		}

		bool operator != (const LYCard &right) const {
			if (this->face != right.face || this->suit != right.suit) {
				return true;
			}
			return false;
		}

		LYCard& operator = (const LYCard &right)  {
			this->face = right.face;
			this->suit = right.suit;
			this->image = right.image;
			return *this;
		}

		unsigned int toInteger() const;
	public:
		static enum LYSuit char2suit(char s);
};

#endif
