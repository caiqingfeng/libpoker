/*
 * LYHoldemAlgorithmDelegate.cpp
 *
 *  Created on: 2013-7-31
 *      Author: caiqingfeng
 */

#include <boost/random.hpp>
#include <boost/generator_iterator.hpp>
#include <boost/regex.hpp>

#include "LYHoldemAlgorithmDelegate.h"
#include "poker/src/LYDeck.h"
#include "poker/src/LYCardHelpers.h"

typedef boost::mt19937 LYRng; //random number generator

LYHoldemAlgorithmDelegate::LYHoldemAlgorithmDelegate() {
	// TODO Auto-generated constructor stub

}

LYHoldemAlgorithmDelegate::~LYHoldemAlgorithmDelegate() {
	// TODO Auto-generated destructor stub
}

std::vector<LYCard> LYHoldemAlgorithmDelegate::initDeck()
{
	std::vector<LYCard> deck;
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

	return deck;
}

bool LYHoldemAlgorithmDelegate::isStraightWithPattern(std::string const &cs, std::string const &ptn)
{
	boost::regex reg(ptn);
	boost::sregex_token_iterator it(cs.begin(), cs.end(), reg, 0);
	boost::sregex_token_iterator end;

	for (; it != end; ++it) { //只取第一个
		return true;
	}
	return false;
}

/*
 * 为了把Boost依赖从Lib中完全移除，设计这个代理模式
 * 但是直接操作LYHandStrength类的成员，比较Ugly
 * 后续再考虑修改20130731
 */
LYHandStrength * LYHoldemAlgorithmDelegate::isStraightFlush(LYHandStrength *hs, LYHandStrength* capHs)
{
    if (hs->rank.size() > 0) {
        if (hs->ranking == StraightFlush) return hs;
        return NULL;
    }

    const char *ptn =
"(Ac.*Kc.*Qc.*Jc.*Tc)|\
(Kc.*Qc.*Jc.*Tc.*9c)|\
(Qc.*Jc.*Tc.*9c.*8c)|\
(Jc.*Tc.*9c.*8c.*7c)|\
(Tc.*9c.*8c.*7c.*6c)|\
(9c.*8c.*7c.*6c.*5c)|\
(8c.*7c.*6c.*5c.*4c)|\
(7c.*6c.*5c.*4c.*3c)|\
(6c.*5c.*4c.*3c.*2c)|\
(Ac.*5c.*4c.*3c.*2c)|\
(Ad.*Kd.*Qd.*Jd.*Td)|\
(Kd.*Qd.*Jd.*Td.*9d)|\
(Qd.*Jd.*Td.*9d.*8d)|\
(Jd.*Td.*9d.*8d.*7d)|\
(Td.*9d.*8d.*7d.*6d)|\
(9d.*8d.*7d.*6d.*5d)|\
(8d.*7d.*6d.*5d.*4d)|\
(7d.*6d.*5d.*4d.*3d)|\
(6d.*5d.*4d.*3d.*2d)|\
(Ad.*5d.*4d.*3d.*2d)|\
(Ah.*Kh.*Qh.*Jh.*Th)|\
(Kh.*Qh.*Jh.*Th.*9h)|\
(Qh.*Jh.*Th.*9h.*8h)|\
(Jh.*Th.*9h.*8h.*7h)|\
(Th.*9h.*8h.*7h.*6h)|\
(9h.*8h.*7h.*6h.*5h)|\
(8h.*7h.*6h.*5h.*4h)|\
(7h.*6h.*5h.*4h.*3h)|\
(6h.*5h.*4h.*3h.*2h)|\
(Ah.*5h.*4h.*3h.*2h)|\
(As.*Ks.*Qs.*Js.*Ts)|\
(Ks.*Qs.*Js.*Ts.*9s)|\
(Qs.*Js.*Ts.*9s.*8s)|\
(Js.*Ts.*9s.*8s.*7s)|\
(Ts.*9s.*8s.*7s.*6s)|\
(9s.*8s.*7s.*6s.*5s)|\
(8s.*7s.*6s.*5s.*4s)|\
(7s.*6s.*5s.*4s.*3s)|\
(6s.*5s.*4s.*3s.*2s)|\
(As.*5s.*4s.*3s.*2s)" ;
    boost::regex reg(ptn);
    boost::sregex_token_iterator it(hs->cardString.begin(), hs->cardString.end(), reg, 0);
    boost::sregex_token_iterator end;

    for (; it != end; ++it) { //只取第一个
    	hs->topRankCard = hs->genTopRankCardForStraight(*it);
        std::string strMatched = *it;

        char suitSymbol = strMatched[1];
        for (unsigned int i=0; i<strMatched.size()/2; i++) {
            char f = strMatched[i*2];
            char s = strMatched[i*2+1];
            LYCard card(f, s);
            if (!hs->rankHasThisFace(card) && s==suitSymbol && hs->rank.size()<5) {
                //iOS会把Axx5432匹配进来，所以对A5432需要特别处理
                if (hs->topRankCard.face == FIVE) {
                    if (card.face <= hs->topRankCard.face || card.face == ACE) {
                    	hs->rank.push_back(card);
                    }
                } else if (card.face <= hs->topRankCard.face){
                	hs->rank.push_back(card);
                }
            }
        }
        hs->ranking = StraightFlush;
        hs->rankingString = StringifiedStraightFlush;
        hs->topRankCard = hs->genTopRankCard(); //之前的TopRankCard没有花色，所以更新一下
        return hs;
    }

    return NULL;
}

LYHandStrength * LYHoldemAlgorithmDelegate::isFourOfAKind(LYHandStrength *hs, LYHandStrength* capHs)
{
    if (NULL != LYHoldemAlgorithmDelegate::isStraightFlush(hs)) { //首先排除是StraightFlush
        return NULL;
    }

    if (hs->rank.size() > 0) { //have done checking it once.
        if (hs->ranking == FourOfAKind) return hs;
        return NULL;
    }

    const char *ptn =
"(AsAhAdAc)|\
(KsKhKdKc)|\
(QsQhQdQc)|\
(JsJhJdJc)|\
(TsThTdTc)|\
(9s9h9d9c)|\
(8s8h8d8c)|\
(7s7h7d7c)|\
(6s6h6d6c)|\
(5s5h5d5c)|\
(4s4h4d4c)|\
(3s3h3d3c)|\
(2s2h2d2c)";
    boost::regex reg(ptn);
    boost::cmatch matched;
    boost::sregex_token_iterator it(hs->cardString.begin(), hs->cardString.end(), reg, 0);
    boost::sregex_token_iterator end;

    for (; it != end; ++it) { //只取第一个
        std::string strMatched = *it;

        for (unsigned int i=0; i<strMatched.size()/2; i++) {
            char f = strMatched[i*2];
            char s = strMatched[i*2+1];
            LYCard card(f, s);
            if (!hs->rankHasThisCard(card) && hs->rank.size()<4) {
                hs->rank.push_back(card);
            }
        }
        hs->ranking = FourOfAKind;
        hs->rankingString = StringifiedFourOfAKind;
        hs->topRankCard = hs->genTopRankCard();

        //找出Kicker
        std::vector<LYCard>::iterator it = hs->cards.begin();
        for (; it!=hs->cards.end(); it++) {
        	LYCard cd = *it;
        	if (!hs->rankHasThisCard(cd)) {
        		hs->kicker.push_back(cd);
        		break; //因为kicker只可能有一个，所以取最大的一个即可！
        	}
        }
        return hs;
    }

    return NULL;
}

LYHandStrength * LYHoldemAlgorithmDelegate::isFullHouse(LYHandStrength *hs, LYHandStrength* capHs)
{
    if (LYHoldemAlgorithmDelegate::isFourOfAKind(hs)) { //首先排除是FourOfAKind
        return NULL;
    }

    if (hs->rank.size() > 0) { //have done checking it once.
        if (hs->ranking == FullHouse) return hs;
        return NULL;
    }

    const char *ptn =
"(A.{1}A.{1}A.{1})|\
(K.{1}K.{1}K.{1})|\
(Q.{1}Q.{1}Q.{1})|\
(J.{1}J.{1}J.{1})|\
(T.{1}T.{1}T.{1})|\
(9.{1}9.{1}9.{1})|\
(8.{1}8.{1}8.{1})|\
(7.{1}7.{1}7.{1})|\
(6.{1}6.{1}6.{1})|\
(5.{1}5.{1}5.{1})|\
(4.{1}4.{1}4.{1})|\
(3.{1}3.{1}3.{1})|\
(2.{1}2.{1}2.{1})";
    boost::regex reg(ptn);
    boost::sregex_token_iterator it(hs->cardString.begin(), hs->cardString.end(), reg, 0);
    boost::sregex_token_iterator end;

    for (; it != end; ++it) { //只取第一个
        std::string strMatched = *it;

        for (unsigned int i=0; i<strMatched.size()/2; i++) {
            char f = strMatched[i*2];
            char s = strMatched[i*2+1];
            LYCard card(f, s);
            if (!hs->rankHasThisCard(card) && hs->rank.size()<3) {
            	hs->rank.push_back(card);
            }
        }
        hs->ranking = ThreeOfAKind;
        hs->rankingString = StringifiedThreeOfAKind;
        hs->topRankCard = hs->genTopRankCard();

        //如果只有5张牌，可以快速的判断是否FullHouse
        if (hs->cards.size() == 5) {
            //找出Kicker
        	std::vector<LYCard>::iterator it = hs->cards.begin();
        	for (; it!=hs->cards.end(); it++) {
        		LYCard cd = *it;
        		if (!hs->rankHasThisCard(cd)) {
        			hs->kicker.push_back(cd);
        		}
        	}

            //如果kicker是一对，则是FullHouse
            LYCard k0 = hs->kicker[0];
            LYCard k1 = hs->kicker[1];
            if (k0.face == k1.face) {
            	hs->rank.push_back(k0);
            	hs->rank.push_back(k1);
            	hs->ranking = FullHouse;
            	hs->rankingString = StringifiedFullHouse;
                return hs;
            }
        }
        //当然也可以统一用下面这段代码实现，为效率起见，保留上段代码
        boost::regex regex_matched(strMatched);
        std::string strRest = boost::regex_replace(hs->cardString, regex_matched, std::string(""));
        const char *pattern_pair =
"(A.{1}A.{1})|\
(K.{1}K.{1})|\
(Q.{1}Q.{1})|\
(J.{1}J.{1})|\
(T.{1}T.{1})|\
(9.{1}9.{1})|\
(8.{1}8.{1})|\
(7.{1}7.{1})|\
(6.{1}6.{1})|\
(5.{1}5.{1})|\
(4.{1}4.{1})|\
(3.{1}3.{1})|\
(2.{1}2.{1})";

        boost::regex regex_pair(pattern_pair);
        boost::sregex_token_iterator newit(strRest.begin(), strRest.end(), regex_pair, 0);
        boost::sregex_token_iterator newend;
        for (; newit != newend; ++newit) { //只取第一个
            std::string strMatched_pair = *newit;
            for (unsigned int i=0; i<strMatched_pair.size()/2; i++) {
                char f = strMatched_pair[i*2];
                char s = strMatched_pair[i*2+1];
                LYCard card(f, s);
                if (!hs->rankHasThisCard(card) && hs->rank.size()<5) {
                	hs->rank.push_back(card);
                }
              }
            hs->ranking = FullHouse;
            hs->rankingString = StringifiedFullHouse;
            return hs;
        }
        //否则是ThreeOfAKind，找出Kicker
        std::vector<LYCard>::iterator it = hs->cards.begin();
        for (; it!=hs->cards.end(); it++) {
        	LYCard cd = *it;
        	if (!hs->rankHasThisCard(cd) && hs->kicker.size() < 2) {
        		hs->kicker.push_back(cd);
        	}
        }

        return NULL;
    }

    return NULL;
}

LYHandStrength * LYHoldemAlgorithmDelegate::isFlush(LYHandStrength *hs, LYHandStrength* capHs)
{
    if (NULL != LYHoldemAlgorithmDelegate::isStraightFlush(hs)) { //首先排除是StraightFlush
        return NULL;
    }

    if (hs->rank.size() > 0) {
        if (hs->ranking == Flush) return hs;
        return NULL;
    }

    const char *ptn =
"(.{1}c.*.{1}c.*.{1}c.*.{1}c.*.{1}c)|\
(.{1}d.*.d.*.{1}d.*.{1}d.*.{1}d)|\
(.{1}h.*.{1}h.*.{1}h.*.{1}h.*.{1}h)|\
(.{1}s.*.{1}s.*.{1}s.*.{1}s.*.{1}s)";

    boost::regex reg(ptn);
    boost::sregex_token_iterator it(hs->cardString.begin(), hs->cardString.end(), reg, 0);
    boost::sregex_token_iterator end;

    for (; it != end; ++it) { //只取第一个
        std::string strMatched = *it;
        char suitSymbol = strMatched[1];

        for (unsigned int i=0; i<strMatched.size()/2; i++) {
            char f = strMatched[i*2];
            char s = strMatched[i*2+1];
            LYCard card(f, s);
            if (!hs->rankHasThisCard(card) && s == suitSymbol && hs->rank.size()<5) {
            	hs->rank.push_back(card);
            }
        }
        hs->ranking = Flush;
        hs->rankingString = StringifiedFlush;
        hs->topRankCard = hs->genTopRankCard();
        return hs;
    }
    return NULL;
}

LYHandStrength * LYHoldemAlgorithmDelegate::isStraight(LYHandStrength *hs, LYHandStrength* capHs)
{
    if (NULL != LYHoldemAlgorithmDelegate::isStraightFlush(hs)) { //首先排除是StraightFlush
        return NULL;
    }

    if (NULL != LYHoldemAlgorithmDelegate::isFlush(hs)) { //首先排除是Flush
        return NULL;
    }

    if (hs->rank.size() > 0) {
        if (hs->ranking == Straight) return hs;
        return NULL;
    }

    const char *ptn =
"(A.{1}.*K.{1}.*Q.{1}.*J.{1}.*T.{1})|\
(K.{1}.*Q.{1}.*J.{1}.*T.{1}.*9.{1})|\
(Q.{1}.*J.{1}.*T.{1}.*9.{1}.*8.{1})|\
(J.{1}.*T.{1}.*9.{1}.*8.{1}.*7.{1})|\
(T.{1}.*9.{1}.*8.{1}.*7.{1}.*6.{1})|\
(9.{1}.*8.{1}.*7.{1}.*6.{1}.*5.{1})|\
(8.{1}.*7.{1}.*6.{1}.*5.{1}.*4.{1})|\
(7.{1}.*6.{1}.*5.{1}.*4.{1}.*3.{1})|\
(6.{1}.*5.{1}.*4.{1}.*3.{1}.*2.{1})|\
(A.{1}.*5.{1}.*4.{1}.*3.{1}.*2.{1})";
    boost::regex reg(ptn);
    boost::sregex_token_iterator it(hs->cardString.begin(), hs->cardString.end(), reg, 0);
    boost::sregex_token_iterator end;

    for (; it != end; ++it) { //只取第一个
        std::string strMatched = *it;
        hs->topRankCard = hs->genTopRankCardForStraight(strMatched);

        for (unsigned int i=0; i<strMatched.size()/2; i++) {
            char f = strMatched[i*2];
            char s = strMatched[i*2+1];
            LYCard card(f, s);
            if (!hs->rankHasThisFace(card) && hs->rank.size()<5) {
                if (hs->topRankCard.face == FIVE) {
                    if (card.face <= hs->topRankCard.face || card.face == ACE) {
                    	hs->rank.push_back(card);
                    }
                } else if (card.face <= hs->topRankCard.face){
                	hs->rank.push_back(card);
                }
            }
         }
        hs->ranking = Straight;
        hs->rankingString = StringifiedStraight;
        hs->topRankCard = hs->genTopRankCard();
        return hs;
    }
    return NULL;
}

LYHandStrength * LYHoldemAlgorithmDelegate::isThreeOfAKind(LYHandStrength *hs, LYHandStrength* capHs)
{
    if (NULL == LYHoldemAlgorithmDelegate::isFourOfAKind(hs)) { //not four of a kind
        if (NULL == LYHoldemAlgorithmDelegate::isFullHouse(hs)) { //not full house either
            if (hs->ranking == ThreeOfAKind) { //because we have checked it in isFullHouse
                return hs;
            }
        }
    }
    return NULL;
}

LYHandStrength * LYHoldemAlgorithmDelegate::isTwoPair(LYHandStrength *hs, LYHandStrength* capHs)
{
    if (NULL != LYHoldemAlgorithmDelegate::isFullHouse(hs)) { //首先排除是FullHouse
        return NULL;
    }

    if (hs->rank.size() > 0) {
        if (hs->ranking == TwoPair) return hs;
        return NULL;
    }

    const char *ptn =
"(A.{1}A.{1})|\
(K.{1}K.{1})|\
(Q.{1}Q.{1})|\
(J.{1}J.{1})|\
(T.{1}T.{1})|\
(9.{1}9.{1})|\
(8.{1}8.{1})|\
(7.{1}7.{1})|\
(6.{1}6.{1})|\
(5.{1}5.{1})|\
(4.{1}4.{1})|\
(3.{1}3.{1})|\
(2.{1}2.{1})";

    boost::regex reg(ptn);
    boost::sregex_token_iterator it(hs->cardString.begin(), hs->cardString.end(), reg, 0);
    boost::sregex_token_iterator end;

    for (; it != end; ++it) { //只取第一个
        std::string strMatched = *it;

        for (unsigned int i=0; i<strMatched.size()/2; i++) {
            char f = strMatched[i*2];
            char s = strMatched[i*2+1];
            LYCard card(f, s);
            if (!hs->rankHasThisCard(card) && hs->rank.size()<2) {
            	hs->rank.push_back(card);
            }
        }
        hs->ranking = OnePair;
        hs->rankingString = StringifiedOnePair;

        boost::regex regex_matched(strMatched);
        std::string strRest = boost::regex_replace(hs->cardString, regex_matched, std::string(""));

        boost::sregex_token_iterator newit(strRest.begin(), strRest.end(), reg, 0);
        boost::sregex_token_iterator newend;
        for (; newit != newend; ++newit) { //只取第一个
            std::string strMatched_pair2 = *newit;
            for (unsigned int i=0; i<strMatched_pair2.size()/2; i++) {
                char f = strMatched_pair2[i*2];
                char s = strMatched_pair2[i*2+1];
                LYCard card(f, s);
                if (!hs->rankHasThisCard(card) && hs->rank.size()<4) {
                	hs->rank.push_back(card);
                }
            }
            hs->ranking = TwoPair;
            hs->rankingString = StringifiedTwoPairs;
            //找出Kicker
            std::vector<LYCard>::iterator it = hs->cards.begin();
            for (; it!=hs->cards.end(); it++) {
            	LYCard cd = *it;
            	if (!hs->rankHasThisCard(cd)) {
            		hs->kicker.push_back(cd);
            		break; //因为kicker只可能有一个，所以取最大的一个即可！
            	}
            }
            hs->topRankCard = hs->genTopRankCard();
            return hs;
        }
        //找出Kicker
        std::vector<LYCard>::iterator it = hs->cards.begin();
        for (; it!=hs->cards.end(); it++) {
        	LYCard cd = *it;
        	if (!hs->rankHasThisCard(cd) && hs->kicker.size() < 3) {
        		hs->kicker.push_back(cd);
        	}
        }
        LYCardHelpers::sortCardsByFace(hs->kicker);
        hs->topRankCard = hs->genTopRankCard();
        return NULL;
    }

    return NULL;
}

LYHandStrength * LYHoldemAlgorithmDelegate::isOnePair(LYHandStrength *hs, LYHandStrength* capHs)
{
    if (NULL != LYHoldemAlgorithmDelegate::isStraight(hs)) { //首先排除是TwoPair以上牌力
        return NULL;
    }

    if (NULL != LYHoldemAlgorithmDelegate::isTwoPair(hs)) { //首先排除是TwoPair以上牌力
        return NULL;
    }

    if (hs->rank.size() > 0) {
        if (hs->ranking == OnePair) {
//        	LYCardHelpers::sortCardsByFace(hs->kicker);
        	return hs;
        }
        return NULL;
    }

    return NULL;
}

LYHandStrength * LYHoldemAlgorithmDelegate::isHighCard(LYHandStrength *hs, LYHandStrength* capHs)
{
    if (NULL != LYHoldemAlgorithmDelegate::isFlush(hs)) { //首先排除是TwoPair以上牌力
        return NULL;
    }

    if (NULL != LYHoldemAlgorithmDelegate::isStraight(hs)) { //首先排除是TwoPair以上牌力
        return NULL;
    }

    if (NULL != LYHoldemAlgorithmDelegate::isOnePair(hs)) { //首先排除是TwoPair以上牌力
        return NULL;
    }

    if (hs->rank.size() > 0) {
        if (hs->ranking == HighCard) return hs;
        return NULL;
    }

    //找出Rank
    std::vector<LYCard>::iterator it = hs->cards.begin();
    for (; it!=hs->cards.end(); it++) {
    	LYCard cd = *it;
    	if (!hs->rankHasThisCard(cd) && hs->rank.size() < 1) {
    		hs->rank.push_back(cd);
    		break;
    	}
    }
    //找出Kicker
    it = hs->cards.begin();
    for (; it!=hs->cards.end(); it++) {
    	LYCard cd = *it;
    	if (!hs->rankHasThisCard(cd) && hs->kicker.size() < 4) {
    		hs->kicker.push_back(cd);
    	}
    }

    hs->rankingString = StringifiedHighCard;
    hs->topRankCard = hs->genTopRankCard();
	LYCardHelpers::sortCardsByFace(hs->kicker);

    return NULL;
}

