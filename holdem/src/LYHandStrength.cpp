/*
 * LYHandStrength.cpp
 *
 *  Created on: 2013-3-22
 *      Author: caiqingfeng
 */
#include <algorithm>
//#include <regex>
//#include <boost/foreach.hpp>

#include "LYHandStrength.h"
#include "poker/src/LYDeck.h"
#include "LYHoldemAlgorithmDelegate.h"
#include "poker/src/LYCardHelpers.h"

const char * StringifiedStraightFlush = "同花顺";
const char * StringifiedFourOfAKind = "四条";
const char * StringifiedFullHouse = "葫芦";
const char * StringifiedFlush = "同花";
const char * StringifiedStraight = "顺子";
const char * StringifiedThreeOfAKind = "三条";
const char * StringifiedTwoPairs = "两对";
const char * StringifiedOnePair = "一对";
const char * StringifiedHighCard = "高牌";

LYHandStrength::LYHandStrength(std::vector<LYCard > const &cds, LYHoldemAlgorithmDelegate *had_,
		LYHandStrength* capHs)
{
	// TODO Auto-generated constructor stub
	cards = cds;
	std::sort(cards.rbegin(), cards.rend());
	reset();

	had = had_;
	this->isHighCard(capHs);
}

void LYHandStrength::reset()
{
	cardString = "";
	ranking = HighCard;
	for (std::vector<LYCard>::iterator it=cards.begin(); it!=cards.end(); ++it) {
		cardString += (*it).toString();
	}
	rank.clear();
}

LYHandStrength::~LYHandStrength() {
	// TODO Auto-generated destructor stub
}

LYCard LYHandStrength::genTopRankCard() {
    if (rank.size() != 0) {
		if (ranking != Straight && ranking != StraightFlush) return (rank[0]);
		
		LYCardHelpers::sortCardsByFace(rank);
		if (rank[0].face == ACE && rank[1].face == FIVE) {
			return (rank[1]);
		}
		
		return (rank[0]);
    }
    return flipCard;
}

/*
 2013.02.28 蔡庆丰注：此处实际上引入了一个限制，最多只处理7张牌
 */
LYCard  LYHandStrength::genTopRankCardForStraight(std::string const &cs)
{
    std::string pattern_ace = "A.{1}.*5.{1}.*4.{1}.*3.{1}.*2.{1}";
    std::string pattern_six = "6.{1}.*5.{1}.*4.{1}.*3.{1}.*2.{1}";
    std::string pattern_seven = "7.{1}.*6.{1}.*5.{1}.*4.{1}.*3.{1}";
    if (this->isStraightWithPattern(cs, pattern_ace)) {
    	if (this->isStraightWithPattern(cs, pattern_seven)) {
    		LYCard card(SEVEN, Nosuit);
    		return card;
        }
    	if (this->isStraightWithPattern(cs, pattern_six)) { //是6***
    		LYCard card(SIX, Nosuit);
    		return card;
        }
		LYCard card(FIVE, Nosuit);
		return card;
    }
    
    char f = cs[0];
    char s = cs[1];
    LYCard card(f, s);
    return card;
}

bool LYHandStrength::isStraightWithPattern(std::string const &cs, std::string const &ptn)
{
	if (NULL == had) {
		return false;
	}

	return had->isStraightWithPattern(cs, ptn);
//    boost::regex reg(ptn);
//    boost::sregex_token_iterator it(cs.begin(), cs.end(), reg, 0);
//    boost::sregex_token_iterator end;
//
//    for (; it != end; ++it) { //只取第一个
//    	return true;
//    }
//    return false;
    //boost::regex_match doesnot work as expected to give a substring match result
    //it treats as the whole matching
}

bool LYHandStrength::rankHasThisCard(LYCard const &card)
{
    if (rank.size() == 0) return false;
	for (std::vector<LYCard>::iterator it=rank.begin(); it!=rank.end(); ++it) {
		if (card == (*it)) {
			return true;
		}
	}
    return false;
}

bool LYHandStrength::rankHasThisFace(LYCard const &card)
{
    if (rank.size() == 0) return false;
	for (std::vector<LYCard>::iterator it=rank.begin(); it!=rank.end(); ++it) {
		if (card.face == (*it).face) {
			return true;
		}
	}
    return false;
}

/*
 * 从objc移植过来
 * 2013.03.24
 */
LYHandStrength * LYHandStrength::isStraightFlush(LYHandStrength* capHs)
{
	if (NULL == had) {
		return NULL;
	}

	return had->isStraightFlush(this, capHs);

//    if (rank.size() > 0) {
//        if (ranking == StraightFlush) return this;
//        return NULL;
//    }
//
//    const char *ptn =
//"(Ac.*Kc.*Qc.*Jc.*Tc)|\
//(Kc.*Qc.*Jc.*Tc.*9c)|\
//(Qc.*Jc.*Tc.*9c.*8c)|\
//(Jc.*Tc.*9c.*8c.*7c)|\
//(Tc.*9c.*8c.*7c.*6c)|\
//(9c.*8c.*7c.*6c.*5c)|\
//(8c.*7c.*6c.*5c.*4c)|\
//(7c.*6c.*5c.*4c.*3c)|\
//(6c.*5c.*4c.*3c.*2c)|\
//(Ac.*5c.*4c.*3c.*2c)|\
//(Ad.*Kd.*Qd.*Jd.*Td)|\
//(Kd.*Qd.*Jd.*Td.*9d)|\
//(Qd.*Jd.*Td.*9d.*8d)|\
//(Jd.*Td.*9d.*8d.*7d)|\
//(Td.*9d.*8d.*7d.*6d)|\
//(9d.*8d.*7d.*6d.*5d)|\
//(8d.*7d.*6d.*5d.*4d)|\
//(7d.*6d.*5d.*4d.*3d)|\
//(6d.*5d.*4d.*3d.*2d)|\
//(Ad.*5d.*4d.*3d.*2d)|\
//(Ah.*Kh.*Qh.*Jh.*Th)|\
//(Kh.*Qh.*Jh.*Th.*9h)|\
//(Qh.*Jh.*Th.*9h.*8h)|\
//(Jh.*Th.*9h.*8h.*7h)|\
//(Th.*9h.*8h.*7h.*6h)|\
//(9h.*8h.*7h.*6h.*5h)|\
//(8h.*7h.*6h.*5h.*4h)|\
//(7h.*6h.*5h.*4h.*3h)|\
//(6h.*5h.*4h.*3h.*2h)|\
//(Ah.*5h.*4h.*3h.*2h)|\
//(As.*Ks.*Qs.*Js.*Ts)|\
//(Ks.*Qs.*Js.*Ts.*9s)|\
//(Qs.*Js.*Ts.*9s.*8s)|\
//(Js.*Ts.*9s.*8s.*7s)|\
//(Ts.*9s.*8s.*7s.*6s)|\
//(9s.*8s.*7s.*6s.*5s)|\
//(8s.*7s.*6s.*5s.*4s)|\
//(7s.*6s.*5s.*4s.*3s)|\
//(6s.*5s.*4s.*3s.*2s)|\
//(As.*5s.*4s.*3s.*2s)" ;
//    boost::regex reg(ptn);
//    boost::sregex_token_iterator it(cardString.begin(), cardString.end(), reg, 0);
//    boost::sregex_token_iterator end;
//
//    for (; it != end; ++it) { //只取第一个
//        topRankCard = this->genTopRankCardForStraight(*it);
//        std::string strMatched = *it;
//
//        char suitSymbol = strMatched[1];
//        for (int i=0; i<strMatched.size()/2; i++) {
//            char f = strMatched[i*2];
//            char s = strMatched[i*2+1];
//            LYCard card(f, s);
//            if (!rankHasThisFace(card) && s==suitSymbol && rank.size()<5) {
//                //iOS会把Axx5432匹配进来，所以对A5432需要特别处理
//                if (topRankCard.face == FIVE) {
//                    if (card.face <= topRankCard.face || card.face == ACE) {
//                        rank.push_back(card);
//                    }
//                } else if (card.face <= topRankCard.face){
//                    rank.push_back(card);
//                }
//            }
//        }
//        ranking = StraightFlush;
//        rankingString = StringifiedStraightFlush;
//        topRankCard = this->genTopRankCard(); //之前的TopRankCard没有花色，所以更新一下
//        return this;
//    }
//
//    return NULL;
}

LYHandStrength * LYHandStrength::isFourOfAKind(LYHandStrength* capHs)
{
	if (NULL == had) {
		return NULL;
	}

	return had->isFourOfAKind(this, capHs);
//    if (NULL != this->isStraightFlush()) { //首先排除是StraightFlush
//        return NULL;
//    }
//
//    if (rank.size() > 0) { //have done checking it once.
//        if (ranking == FourOfAKind) return this;
//        return NULL;
//    }
//
//    const char *ptn =
//"(AsAhAdAc)|\
//(KsKhKdKc)|\
//(QsQhQdQc)|\
//(JsJhJdJc)|\
//(TsThTdTc)|\
//(9s9h9d9c)|\
//(8s8h8d8c)|\
//(7s7h7d7c)|\
//(6s6h6d6c)|\
//(5s5h5d5c)|\
//(4s4h4d4c)|\
//(3s3h3d3c)|\
//(2s2h2d2c)";
//    boost::regex reg(ptn);
//    boost::cmatch matched;
//    boost::sregex_token_iterator it(cardString.begin(), cardString.end(), reg, 0);
//    boost::sregex_token_iterator end;
//
//    for (; it != end; ++it) { //只取第一个
//        std::string strMatched = *it;
//
//        for (int i=0; i<strMatched.size()/2; i++) {
//            char f = strMatched[i*2];
//            char s = strMatched[i*2+1];
//            LYCard card(f, s);
//            if (!this->rankHasThisCard(card) && rank.size()<4) {
//                rank.push_back(card);
//            }
//        }
//        ranking = FourOfAKind;
//        rankingString = StringifiedFourOfAKind;
//        topRankCard = this->genTopRankCard();
//
//        //找出Kicker
//        std::vector<LYCard>::iterator it = cards.begin();
//        for (; it!=cards.end(); it++) {
//        	LYCard cd = *it;
//        	if (!this->rankHasThisCard(cd)) {
//        		kicker.push_back(cd);
//        		break; //因为kicker只可能有一个，所以取最大的一个即可！
//        	}
//        }
//        return this;
//    }
//
//    return NULL;
}

LYHandStrength * LYHandStrength::isFullHouse(LYHandStrength* capHs)
{
	if (NULL == had) {
		return NULL;
	}

	return had->isFullHouse(this, capHs);
//    if (this->isFourOfAKind()) { //首先排除是FourOfAKind
//        return NULL;
//    }
//
//    if (rank.size() > 0) { //have done checking it once.
//        if (ranking == FullHouse) return this;
//        return NULL;
//    }
//
//    const char *ptn =
//"(A.{1}A.{1}A.{1})|\
//(K.{1}K.{1}K.{1})|\
//(Q.{1}Q.{1}Q.{1})|\
//(J.{1}J.{1}J.{1})|\
//(T.{1}T.{1}T.{1})|\
//(9.{1}9.{1}9.{1})|\
//(8.{1}8.{1}8.{1})|\
//(7.{1}7.{1}7.{1})|\
//(6.{1}6.{1}6.{1})|\
//(5.{1}5.{1}5.{1})|\
//(4.{1}4.{1}4.{1})|\
//(3.{1}3.{1}3.{1})|\
//(2.{1}2.{1}2.{1})";
//    boost::regex reg(ptn);
//    boost::sregex_token_iterator it(cardString.begin(), cardString.end(), reg, 0);
//    boost::sregex_token_iterator end;
//
//    for (; it != end; ++it) { //只取第一个
//        std::string strMatched = *it;
//
//        for (int i=0; i<strMatched.size()/2; i++) {
//            char f = strMatched[i*2];
//            char s = strMatched[i*2+1];
//            LYCard card(f, s);
//            if (!this->rankHasThisCard(card) && rank.size()<3) {
//                rank.push_back(card);
//            }
//        }
//        ranking = ThreeOfAKind;
//        rankingString = StringifiedThreeOfAKind;
//        topRankCard = this->genTopRankCard();
//
//        //如果只有5张牌，可以快速的判断是否FullHouse
//        if (cards.size() == 5) {
//            //找出Kicker
//        	std::vector<LYCard>::iterator it = cards.begin();
//        	for (; it!=cards.end(); it++) {
//        		LYCard cd = *it;
//        		if (!this->rankHasThisCard(cd)) {
//        			kicker.push_back(cd);
//        		}
//        	}
//
//            //如果kicker是一对，则是FullHouse
//            LYCard k0 = kicker[0];
//            LYCard k1 = kicker[1];
//            if (k0.face == k1.face) {
//                rank.push_back(k0);
//                rank.push_back(k1);
//                ranking = FullHouse;
//                rankingString = StringifiedFullHouse;
//                return this;
//            }
//        }
//        //当然也可以统一用下面这段代码实现，为效率起见，保留上段代码
//        boost::regex regex_matched(strMatched);
//        std::string strRest = boost::regex_replace(cardString, regex_matched, string(""));
//        const char *pattern_pair =
//"(A.{1}A.{1})|\
//(K.{1}K.{1})|\
//(Q.{1}Q.{1})|\
//(J.{1}J.{1})|\
//(T.{1}T.{1})|\
//(9.{1}9.{1})|\
//(8.{1}8.{1})|\
//(7.{1}7.{1})|\
//(6.{1}6.{1})|\
//(5.{1}5.{1})|\
//(4.{1}4.{1})|\
//(3.{1}3.{1})|\
//(2.{1}2.{1})";
//
//        boost::regex regex_pair(pattern_pair);
//        boost::sregex_token_iterator newit(strRest.begin(), strRest.end(), regex_pair, 0);
//        boost::sregex_token_iterator newend;
//        for (; newit != newend; ++newit) { //只取第一个
//            std::string strMatched_pair = *newit;
//            for (int i=0; i<strMatched_pair.size()/2; i++) {
//                char f = strMatched_pair[i*2];
//                char s = strMatched_pair[i*2+1];
//                LYCard card(f, s);
//                if (!this->rankHasThisCard(card) && rank.size()<5) {
//                    rank.push_back(card);
//                }
//              }
//            ranking = FullHouse;
//            rankingString = StringifiedFullHouse;
//            return this;
//        }
//        //否则是ThreeOfAKind，找出Kicker
//        std::vector<LYCard>::iterator it = cards.begin();
//        for (; it!=cards.end(); it++) {
//        	LYCard cd = *it;
//        	if (!this->rankHasThisCard(cd) && kicker.size() < 2) {
//        		kicker.push_back(cd);
//        	}
//        }
//
//        return NULL;
//    }
//
//    return NULL;
}

LYHandStrength * LYHandStrength::isFlush(LYHandStrength* capHs)
{
	if (NULL == had) {
		return NULL;
	}

	return had->isFlush(this, capHs);
//    if (NULL != this->isStraightFlush()) { //首先排除是StraightFlush
//        return NULL;
//    }
//
//    if (rank.size() > 0) {
//        if (ranking == Flush) return this;
//        return NULL;
//    }
//
//    const char *ptn =
//"(.{1}c.*.{1}c.*.{1}c.*.{1}c.*.{1}c)|\
//(.{1}d.*.d.*.{1}d.*.{1}d.*.{1}d)|\
//(.{1}h.*.{1}h.*.{1}h.*.{1}h.*.{1}h)|\
//(.{1}s.*.{1}s.*.{1}s.*.{1}s.*.{1}s)";
//
//    boost::regex reg(ptn);
//    boost::sregex_token_iterator it(cardString.begin(), cardString.end(), reg, 0);
//    boost::sregex_token_iterator end;
//
//    for (; it != end; ++it) { //只取第一个
//        std::string strMatched = *it;
//        char suitSymbol = strMatched[1];
//
//        for (int i=0; i<strMatched.size()/2; i++) {
//            char f = strMatched[i*2];
//            char s = strMatched[i*2+1];
//            LYCard card(f, s);
//            if (!this->rankHasThisCard(card) && s == suitSymbol && rank.size()<5) {
//                rank.push_back(card);
//            }
//        }
//        ranking = Flush;
//        rankingString = StringifiedFlush;
//        topRankCard = this->genTopRankCard();
//        return this;
//    }
//    return NULL;
}

LYHandStrength * LYHandStrength::isStraight(LYHandStrength* capHs)
{
	if (NULL == had) {
		return NULL;
	}

	return had->isStraight(this, capHs);
//    if (NULL != this->isStraightFlush()) { //首先排除是StraightFlush
//        return NULL;
//    }
//
//    if (NULL != this->isFlush()) { //首先排除是Flush
//        return NULL;
//    }
//
//    if (rank.size() > 0) {
//        if (ranking == Straight) return this;
//        return NULL;
//    }
//
//    const char *ptn =
//"(A.{1}.*K.{1}.*Q.{1}.*J.{1}.*T.{1})|\
//(K.{1}.*Q.{1}.*J.{1}.*T.{1}.*9.{1})|\
//(Q.{1}.*J.{1}.*T.{1}.*9.{1}.*8.{1})|\
//(J.{1}.*T.{1}.*9.{1}.*8.{1}.*7.{1})|\
//(T.{1}.*9.{1}.*8.{1}.*7.{1}.*6.{1})|\
//(9.{1}.*8.{1}.*7.{1}.*6.{1}.*5.{1})|\
//(8.{1}.*7.{1}.*6.{1}.*5.{1}.*4.{1})|\
//(7.{1}.*6.{1}.*5.{1}.*4.{1}.*3.{1})|\
//(6.{1}.*5.{1}.*4.{1}.*3.{1}.*2.{1})|\
//(A.{1}.*5.{1}.*4.{1}.*3.{1}.*2.{1})";
//    boost::regex reg(ptn);
//    boost::sregex_token_iterator it(cardString.begin(), cardString.end(), reg, 0);
//    boost::sregex_token_iterator end;
//
//    for (; it != end; ++it) { //只取第一个
//        std::string strMatched = *it;
//        topRankCard = this->genTopRankCardForStraight(strMatched);
//
//        for (int i=0; i<strMatched.size()/2; i++) {
//            char f = strMatched[i*2];
//            char s = strMatched[i*2+1];
//            LYCard card(f, s);
//            if (!this->rankHasThisFace(card) && rank.size()<5) {
//                if (topRankCard.face == FIVE) {
//                    if (card.face <= topRankCard.face || card.face == ACE) {
//                        rank.push_back(card);
//                    }
//                } else if (card.face <= topRankCard.face){
//                    rank.push_back(card);
//                }
//            }
//         }
//        ranking = Straight;
//        rankingString = StringifiedStraight;
//        topRankCard = this->genTopRankCard();
//        return this;
//    }
//    return NULL;
}

LYHandStrength * LYHandStrength::isThreeOfAKind(LYHandStrength* capHs)
{
	if (NULL == had) {
		return NULL;
	}

	return had->isThreeOfAKind(this, capHs);
//    if (NULL == this->isFourOfAKind()) { //not four of a kind
//        if (NULL == this->isFullHouse()) { //not full house either
//            if (ranking == ThreeOfAKind) { //because we have checked it in isFullHouse
//                return this;
//            }
//        }
//    }
//    return NULL;
}

LYHandStrength * LYHandStrength::isTwoPair(LYHandStrength* capHs)
{
	if (NULL == had) {
		return NULL;
	}

	return had->isTwoPair(this, capHs);
//    if (NULL != this->isFullHouse()) { //首先排除是FullHouse
//        return NULL;
//    }
//
//    if (rank.size() > 0) {
//        if (ranking == TwoPair) return this;
//        return NULL;
//    }
//
//    const char *ptn =
//"(A.{1}A.{1})|\
//(K.{1}K.{1})|\
//(Q.{1}Q.{1})|\
//(J.{1}J.{1})|\
//(T.{1}T.{1})|\
//(9.{1}9.{1})|\
//(8.{1}8.{1})|\
//(7.{1}7.{1})|\
//(6.{1}6.{1})|\
//(5.{1}5.{1})|\
//(4.{1}4.{1})|\
//(3.{1}3.{1})|\
//(2.{1}2.{1})";
//
//    boost::regex reg(ptn);
//    boost::sregex_token_iterator it(cardString.begin(), cardString.end(), reg, 0);
//    boost::sregex_token_iterator end;
//
//    for (; it != end; ++it) { //只取第一个
//        std::string strMatched = *it;
//
//        for (int i=0; i<strMatched.size()/2; i++) {
//            char f = strMatched[i*2];
//            char s = strMatched[i*2+1];
//            LYCard card(f, s);
//            if (!this->rankHasThisCard(card) && rank.size()<2) {
//                rank.push_back(card);
//            }
//        }
//        ranking = OnePair;
//        rankingString = StringifiedOnePair;
//
//        boost::regex regex_matched(strMatched);
//        std::string strRest = boost::regex_replace(cardString, regex_matched, string(""));
//
//        boost::sregex_token_iterator newit(strRest.begin(), strRest.end(), reg, 0);
//        boost::sregex_token_iterator newend;
//        for (; newit != newend; ++newit) { //只取第一个
//            std::string strMatched_pair2 = *newit;
//            for (int i=0; i<strMatched_pair2.size()/2; i++) {
//                char f = strMatched_pair2[i*2];
//                char s = strMatched_pair2[i*2+1];
//                LYCard card(f, s);
//                if (!this->rankHasThisCard(card) && rank.size()<4) {
//                    rank.push_back(card);
//                }
//            }
//            ranking = TwoPair;
//            rankingString = StringifiedTwoPairs;
//            //找出Kicker
//            std::vector<LYCard>::iterator it = cards.begin();
//            for (; it!=cards.end(); it++) {
//            	LYCard cd = *it;
//            	if (!this->rankHasThisCard(cd)) {
//            		kicker.push_back(cd);
//            		break; //因为kicker只可能有一个，所以取最大的一个即可！
//            	}
//            }
//            topRankCard = this->genTopRankCard();
//            return this;
//        }
//        //找出Kicker
//        std::vector<LYCard>::iterator it = cards.begin();
//        for (; it!=cards.end(); it++) {
//        	LYCard cd = *it;
//        	if (!this->rankHasThisCard(cd) && kicker.size() < 3) {
//        		kicker.push_back(cd);
//        	}
//        }
//        topRankCard = this->genTopRankCard();
//        return NULL;
//    }
//
//    return NULL;
}

LYHandStrength * LYHandStrength::isOnePair(LYHandStrength* capHs)
{
	if (NULL == had) {
		return NULL;
	}

	return had->isOnePair(this, capHs);
//    if (NULL != this->isStraight()) { //首先排除是TwoPair以上牌力
//        return NULL;
//    }
//
//    if (NULL != this->isTwoPair()) { //首先排除是TwoPair以上牌力
//        return NULL;
//    }
//
//    if (rank.size() > 0) {
//        if (ranking == OnePair) return this;
//        return NULL;
//    }
//
//    return NULL;
}

LYHandStrength * LYHandStrength::isHighCard(LYHandStrength* capHs)
{
	if (NULL == had) {
		return NULL;
	}

	return had->isHighCard(this, capHs);
//    if (NULL != this->isFlush()) { //首先排除是TwoPair以上牌力
//        return NULL;
//    }
//
//    if (NULL != this->isStraight()) { //首先排除是TwoPair以上牌力
//        return NULL;
//    }
//
//    if (NULL != this->isOnePair()) { //首先排除是TwoPair以上牌力
//        return NULL;
//    }
//
//    if (rank.size() > 0) {
//        if (ranking == HighCard) return this;
//        return NULL;
//    }
//
//    //找出Rank
//    std::vector<LYCard>::iterator it = cards.begin();
//    for (; it!=cards.end(); it++) {
//    	LYCard cd = *it;
//    	if (!this->rankHasThisCard(cd) && rank.size() < 1) {
//    		rank.push_back(cd);
//    		break;
//    	}
//    }
//    //找出Kicker
//    it = cards.begin();
//    for (; it!=cards.end(); it++) {
//    	LYCard cd = *it;
//    	if (!this->rankHasThisCard(cd) && kicker.size() < 4) {
//    		kicker.push_back(cd);
//    	}
//    }
//
//    rankingString = StringifiedHighCard;
//    topRankCard = this->genTopRankCard();
//
//    return NULL;
}

bool LYHandStrength::hasGhost()
{
	std::vector<LYCard >::iterator it = cards.begin();
	for (; it!=cards.end(); it++) {
		if ((*it).isGhost()) return true;
	}
	return false;
}

std::string LYHandStrength::genCardStringWithoutGhost()
{
	std::string cs;
	std::vector<LYCard >::iterator it = cards.begin();
	for (; it!=cards.end(); it++) {
		if ((*it).isGhost()) continue;
		cs = cs + (*it).toString();
	}
	return cs;
}
