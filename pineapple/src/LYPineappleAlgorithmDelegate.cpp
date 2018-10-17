/*
 * LYPineappleAlgorithmDelegate.cpp
 *
 *  Created on: 2013-7-31
 *      Author: caiqingfeng
 */

#include <boost/random.hpp>
#include <boost/generator_iterator.hpp>
#include <boost/regex.hpp>

#include "poker/src/LYCardHelpers.h"
#include "poker/src/LYDeck.h"

#include "LYPineappleAlgorithmDelegate.h"
#include "libmrock/common/src/my_log.h"

const char* OpenEndedStraightFlushDraw =
"(Kc.*Qc.*Jc.*Tc)|\
(Qc.*Jc.*Tc.*9c)|\
(Jc.*Tc.*9c.*8c)|\
(Tc.*9c.*8c.*7c)|\
(9c.*8c.*7c.*6c)|\
(8c.*7c.*6c.*5c)|\
(7c.*6c.*5c.*4c)|\
(6c.*5c.*4c.*3c)|\
(5c.*4c.*3c.*2c)|\
(Kd.*Qd.*Jd.*Td)|\
(Qd.*Jd.*Td.*9d)|\
(Jd.*Td.*9d.*8d)|\
(Td.*9d.*8d.*7d)|\
(9d.*8d.*7d.*6d)|\
(8d.*7d.*6d.*5d)|\
(7d.*6d.*5d.*4d)|\
(6d.*5d.*4d.*3d)|\
(5d.*4d.*3d.*2d)|\
(Kh.*Qh.*Jh.*Th)|\
(Qh.*Jh.*Th.*9h)|\
(Jh.*Th.*9h.*8h)|\
(Th.*9h.*8h.*7h)|\
(9h.*8h.*7h.*6h)|\
(8h.*7h.*6h.*5h)|\
(7h.*6h.*5h.*4h)|\
(6h.*5h.*4h.*3h)|\
(5h.*4h.*3h.*2h)|\
(Ks.*Qs.*Js.*Ts)|\
(Qs.*Js.*Ts.*9s)|\
(Js.*Ts.*9s.*8s)|\
(Ts.*9s.*8s.*7s)|\
(9s.*8s.*7s.*6s)|\
(8s.*7s.*6s.*5s)|\
(7s.*6s.*5s.*4s)|\
(6s.*5s.*4s.*3s)|\
(5s.*4s.*3s.*2s)" ;

const char* GushotStraightFlushDraw =
"(Ac.*Kc.*Qc.*Jc)|\
(Ac.*Kc.*Qc.*Tc)|\
(Ac.*Kc.*Jc.*Tc)|\
(Ac.*Qc.*Jc.*Tc)|\
(Kc.*Qc.*Jc.*9c)|\
(Kc.*Qc.*Tc.*9c)|\
(Kc.*Jc.*Tc.*9c)|\
(Qc.*Jc.*Tc.*8c)|\
(Qc.*Jc.*9c.*8c)|\
(Qc.*Tc.*9c.*8c)|\
(Jc.*Tc.*9c.*7c)|\
(Jc.*Tc.*8c.*7c)|\
(Jc.*9c.*8c.*7c)|\
(Tc.*9c.*8c.*6c)|\
(Tc.*9c.*7c.*6c)|\
(Tc.*8c.*7c.*6c)|\
(9c.*8c.*7c.*5c)|\
(9c.*8c.*6c.*5c)|\
(9c.*7c.*6c.*5c)|\
(8c.*7c.*6c.*4c)|\
(8c.*7c.*5c.*4c)|\
(8c.*6c.*5c.*4c)|\
(7c.*6c.*5c.*3c)|\
(7c.*6c.*4c.*3c)|\
(7c.*5c.*4c.*3c)|\
(6c.*5c.*4c.*2c)|\
(6c.*5c.*3c.*2c)|\
(6c.*4c.*3c.*2c)|\
(Ac.*5c.*4c.*3c)|\
(Ac.*5c.*4c.*2c)|\
(Ac.*5c.*3c.*2c)|\
(Ac.*4c.*3c.*2c)|\
(Ad.*Kd.*Qd.*Jd)|\
(Ad.*Kd.*Qd.*Td)|\
(Ad.*Kd.*Jd.*Td)|\
(Ad.*Qd.*Jd.*Td)|\
(Kd.*Qd.*Jd.*9d)|\
(Kd.*Qd.*Td.*9d)|\
(Kd.*Jd.*Td.*9d)|\
(Qd.*Jd.*Td.*8d)|\
(Qd.*Jd.*9d.*8d)|\
(Qd.*Td.*9d.*8d)|\
(Jd.*Td.*9d.*7d)|\
(Jd.*Td.*8d.*7d)|\
(Jd.*9d.*8d.*7d)|\
(Td.*9d.*8d.*6d)|\
(Td.*9d.*7d.*6d)|\
(Td.*8d.*7d.*6d)|\
(9d.*8d.*7d.*5d)|\
(9d.*8d.*6d.*5d)|\
(9d.*7d.*6d.*5d)|\
(8d.*7d.*6d.*4d)|\
(8d.*7d.*5d.*4d)|\
(8d.*6d.*5d.*4d)|\
(7d.*6d.*5d.*3d)|\
(7d.*6d.*4d.*3d)|\
(7d.*5d.*4d.*3d)|\
(6d.*5d.*4d.*2d)|\
(6d.*5d.*3d.*2d)|\
(6d.*4d.*3d.*2d)|\
(Ad.*5d.*4d.*3d)|\
(Ad.*5d.*4d.*2d)|\
(Ad.*5d.*3d.*2d)|\
(Ad.*4d.*3d.*2d)|\
(Ah.*Kh.*Qh.*Jh)|\
(Ah.*Kh.*Qh.*Th)|\
(Ah.*Kh.*Jh.*Th)|\
(Ah.*Qh.*Jh.*Th)|\
(Kh.*Qh.*Jh.*9h)|\
(Kh.*Qh.*Th.*9h)|\
(Kh.*Jh.*Th.*9h)|\
(Qh.*Jh.*Th.*8h)|\
(Qh.*Jh.*9h.*8h)|\
(Qh.*Th.*9h.*8h)|\
(Jh.*Th.*9h.*7h)|\
(Jh.*Th.*8h.*7h)|\
(Jh.*9h.*8h.*7h)|\
(Th.*9h.*8h.*6h)|\
(Th.*9h.*7h.*6h)|\
(Th.*8h.*7h.*6h)|\
(9h.*8h.*7h.*5h)|\
(9h.*8h.*6h.*5h)|\
(9h.*7h.*6h.*5h)|\
(8h.*7h.*6h.*4h)|\
(8h.*7h.*5h.*4h)|\
(8h.*6h.*5h.*4h)|\
(7h.*6h.*5h.*3h)|\
(7h.*6h.*4h.*3h)|\
(7h.*5h.*4h.*3h)|\
(6h.*5h.*4h.*2h)|\
(6h.*5h.*3h.*2h)|\
(6h.*4h.*3h.*2h)|\
(Ah.*5h.*4h.*3h)|\
(Ah.*5h.*4h.*2h)|\
(Ah.*5h.*3h.*2h)|\
(Ah.*4h.*3h.*2h)|\
(As.*Ks.*Qs.*Js)|\
(As.*Ks.*Qs.*Ts)|\
(As.*Ks.*Js.*Ts)|\
(As.*Qs.*Js.*Ts)|\
(Ks.*Qs.*Js.*9s)|\
(Ks.*Qs.*Ts.*9s)|\
(Ks.*Js.*Ts.*9s)|\
(Qs.*Js.*Ts.*8s)|\
(Qs.*Js.*9s.*8s)|\
(Qs.*Ts.*9s.*8s)|\
(Js.*Ts.*9s.*7s)|\
(Js.*Ts.*8s.*7s)|\
(Js.*9s.*8s.*7s)|\
(Ts.*9s.*8s.*6s)|\
(Ts.*9s.*7s.*6s)|\
(Ts.*8s.*7s.*6s)|\
(9s.*8s.*7s.*5s)|\
(9s.*8s.*6s.*5s)|\
(9s.*7s.*6s.*5s)|\
(8s.*7s.*6s.*4s)|\
(8s.*7s.*5s.*4s)|\
(8s.*6s.*5s.*4s)|\
(7s.*6s.*5s.*3s)|\
(7s.*6s.*4s.*3s)|\
(7s.*5s.*4s.*3s)|\
(6s.*5s.*4s.*2s)|\
(6s.*5s.*3s.*2s)|\
(6s.*4s.*3s.*2s)|\
(As.*5s.*4s.*3s)|\
(As.*5s.*4s.*2s)|\
(As.*5s.*3s.*2s)|\
(As.*4s.*3s.*2s)";

const char* GushotAceToFiveStraightFlush =
"(Ac.*5c.*4c.*3c)|\
(Ac.*5c.*4c.*2c)|\
(Ac.*5c.*3c.*2c)|\
(Ac.*4c.*3c.*2c)|\
(Ad.*5d.*4d.*3d)|\
(Ad.*5d.*4d.*2d)|\
(Ad.*5d.*3d.*2d)|\
(Ad.*4d.*3d.*2d)|\
(Ah.*5h.*4h.*3h)|\
(Ah.*5h.*4h.*2h)|\
(Ah.*5h.*3h.*2h)|\
(Ah.*4h.*3h.*2h)|\
(As.*5s.*4s.*3s)|\
(As.*5s.*4s.*2s)|\
(As.*5s.*3s.*2s)|\
(As.*4s.*3s.*2s)";

const char *ThreeOfAKindPtn =
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

const char *OnePairPtn =
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

const char* FlushDrawPtn =
"(.{1}c.*.{1}c.*.{1}c.*.{1}c)|\
(.{1}d.*.{1}d.*.{1}d.*.{1}d)|\
(.{1}h.*.{1}h.*.{1}h.*.{1}h)|\
(.{1}s.*.{1}s.*.{1}s.*.{1}s)";

const char* OpenEndedStraightDrawPtn =
"(K.{1}.*Q.{1}.*J.{1}.*T.{1})|\
(Q.{1}.*J.{1}.*T.{1}.*9.{1})|\
(J.{1}.*T.{1}.*9.{1}.*8.{1})|\
(T.{1}.*9.{1}.*8.{1}.*7.{1})|\
(9.{1}.*8.{1}.*7.{1}.*6.{1})|\
(8.{1}.*7.{1}.*6.{1}.*5.{1})|\
(7.{1}.*6.{1}.*5.{1}.*4.{1})|\
(6.{1}.*5.{1}.*4.{1}.*3.{1})|\
(5.{1}.*4.{1}.*3.{1}.*2.{1})"
;

const char* GushotStraightDrawPtn =
"(A.{1}.*K.{1}.*Q.{1}.*J.{1})|\
(A.{1}.*K.{1}.*Q.{1}.*T.{1})|\
(A.{1}.*K.{1}.*J.{1}.*T.{1})|\
(A.{1}.*Q.{1}.*J.{1}.*T.{1})|\
(K.{1}.*Q.{1}.*J.{1}.*9.{1})|\
(K.{1}.*Q.{1}.*T.{1}.*9.{1})|\
(K.{1}.*J.{1}.*T.{1}.*9.{1})|\
(Q.{1}.*J.{1}.*T.{1}.*8.{1})|\
(Q.{1}.*J.{1}.*9.{1}.*8.{1})|\
(Q.{1}.*T.{1}.*9.{1}.*8.{1})|\
(J.{1}.*T.{1}.*9.{1}.*7.{1})|\
(J.{1}.*T.{1}.*8.{1}.*7.{1})|\
(J.{1}.*9.{1}.*8.{1}.*7.{1})|\
(T.{1}.*9.{1}.*8.{1}.*6.{1})|\
(T.{1}.*9.{1}.*7.{1}.*6.{1})|\
(T.{1}.*8.{1}.*7.{1}.*6.{1})|\
(9.{1}.*8.{1}.*7.{1}.*5.{1})|\
(9.{1}.*8.{1}.*6.{1}.*5.{1})|\
(9.{1}.*7.{1}.*6.{1}.*5.{1})|\
(8.{1}.*7.{1}.*6.{1}.*4.{1})|\
(8.{1}.*7.{1}.*5.{1}.*4.{1})|\
(8.{1}.*6.{1}.*5.{1}.*4.{1})|\
(7.{1}.*6.{1}.*5.{1}.*3.{1})|\
(7.{1}.*6.{1}.*4.{1}.*3.{1})|\
(7.{1}.*5.{1}.*4.{1}.*3.{1})|\
(6.{1}.*5.{1}.*4.{1}.*2.{1})|\
(6.{1}.*5.{1}.*3.{1}.*2.{1})|\
(6.{1}.*4.{1}.*3.{1}.*2.{1})|\
(A.{1}.*5.{1}.*4.{1}.*3.{1})|\
(A.{1}.*5.{1}.*4.{1}.*2.{1})|\
(A.{1}.*5.{1}.*3.{1}.*2.{1})|\
(A.{1}.*4.{1}.*3.{1}.*2.{1})"
;

const char* GushotAceToFiveStraightDrawPtn =
"(A.{1}.*5.{1}.*4.{1}.*3.{1})|\
(A.{1}.*5.{1}.*4.{1}.*2.{1})|\
(A.{1}.*5.{1}.*3.{1}.*2.{1})|\
(A.{1}.*4.{1}.*3.{1}.*2.{1})"
;

LYPineappleAlgorithmDelegate::LYPineappleAlgorithmDelegate() {
	// TODO Auto-generated constructor stub

}

LYPineappleAlgorithmDelegate::~LYPineappleAlgorithmDelegate() {
	// TODO Auto-generated destructor stub
}

std::vector<LYCard> LYPineappleAlgorithmDelegate::initDeck()
{
	std::vector<LYCard> deck;
	deck.push_back(smallGhost);
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

/*
 * capHs: 不能超过这个强度，例如当底道是两对时，中道就不能是三条
 */
LYHandStrength * LYPineappleAlgorithmDelegate::isStraightFlush(LYHandStrength *hs, LYHandStrength* capHs)
{
	if (!hs->hasGhost()) return LYHoldemAlgorithmDelegate::isStraightFlush(hs);

	if (capHs && capHs->ranking != StraightFlush) return NULL;
	LYCard capTopRankCard = flipCard;
	if (capHs) capTopRankCard = capHs->topRankCard;

	std::string cs = hs->genCardStringWithoutGhost();

	//首先判断OpenEndedStraightFlush，比较复杂，王可以当两张牌
//	std::cout << OpenEndedStraightFlushDraw << std::endl;
    boost::regex reg(OpenEndedStraightFlushDraw);
    boost::sregex_token_iterator it(cs.begin(), cs.end(), reg, 0);
    boost::sregex_token_iterator end;

    for (; it != end; ++it) { //只取第一个
    	std::string strMatched = *it;
        for (unsigned int i=0; i<strMatched.size()/2; i++) {
            char f = strMatched[i*2];
            char s = strMatched[i*2+1];
            LYCard card(f, s);
            hs->rank.push_back(card);
            if (i == 0) {
            	LYCard ghostCard((enum LYFace)(card.face+1), card.suit);
            	hs->rank.push_back(ghostCard);
            }
        }
    	hs->ranking = StraightFlush;
        hs->rankingString = StringifiedStraightFlush;
        hs->topRankCard = hs->rank[0];
    	if (!capHs || hs->topRankCard > capTopRankCard) {
        	if (capHs && hs->rank[1].face > capTopRankCard.face) {
        		hs->reset();
        		return NULL;
        	}
        	hs->topRankCard = hs->rank[1];
        	if (hs->rank[1].face > FIVE) {
        		LYCard ghostCard(hs->rank[1].face-4, hs->rank[1].suit);
        		hs->rank[0] = ghostCard;
        	} else {
        		LYCard ghostCard(ACE, hs->rank[1].suit);
        		hs->rank[0] = ghostCard;
        	}
        	LYCardHelpers::sortCardsByFace(hs->rank);
        }
    	return hs;
    }

    //其次判断A－5的Gushot
//	std::cout << GushotAceToFiveStraightFlush << std::endl;
    boost::regex reg2(GushotAceToFiveStraightFlush);
    boost::sregex_token_iterator it2(cs.begin(), cs.end(), reg2, 0);

    for (; it2 != end; ++it2) { //只取第一个
        std::string strMatched = *it2;
//        std::cout << "matched:" << strMatched << std::endl;
        char s = strMatched[1];
        for (unsigned int i=0; i<4; i++) {
        	LYCard cd(strMatched[0], s);
        	cd.face = (enum LYFace)(TWO + i);
        	hs->rank.push_back(cd);
        }
        LYCard aceCard(ACE, hs->rank[0].suit);
        LYCard fiveCard(FIVE, hs->rank[0].suit);
        hs->rank.push_back(aceCard);
        LYCardHelpers::sortCardsByFace(hs->rank);
        hs->rankingString = StringifiedStraightFlush;
        hs->ranking = StraightFlush;
        hs->topRankCard = fiveCard;
        return hs;
    }

    //最后判断其它的Gushot
    boost::regex reg3(GushotStraightFlushDraw);
    boost::sregex_token_iterator it3(cs.begin(), cs.end(), reg3, 0);

    for (; it3 != end; ++it3) { //只取第一个
        std::string strMatched = *it3;
//        std::cout << "matched:" << strMatched << std::endl;
        char s = strMatched[1];
        char f = strMatched[0];
    	LYCard topCard(f, s);
//    	std::cout << "topCard:" << topCard.toString() << std::endl;
        if (capHs && capTopRankCard.face < topCard.face) return NULL;
        hs->rank.push_back(topCard);
        for (unsigned int i=0; i<4; i++) {
        	LYCard cd(f, s);
        	cd.face = (enum LYFace)(topCard.face - i - 1);
        	hs->rank.push_back(cd);
//        	std::cout << cd.toString() << std::endl;
        }
        LYCardHelpers::sortCardsByFace(hs->rank);
        hs->rankingString = StringifiedStraightFlush;
        hs->ranking = StraightFlush;
        hs->topRankCard = topCard;
        return hs;
    }

    hs->reset();
    return NULL;
}

LYHandStrength * LYPineappleAlgorithmDelegate::isFourOfAKind(LYHandStrength *hs, LYHandStrength* capHs)
{
	if (!hs->hasGhost()) return LYHoldemAlgorithmDelegate::isFourOfAKind(hs);
	//其次判断是否已经具备四条，无论有无Ghost都已经是四条
	LYHandStrength* orgHs = LYHoldemAlgorithmDelegate::isFourOfAKind(hs);
	if (orgHs != NULL) {
		//只需要设置Kicker
		LYCard kicker = hs->topRankCard;
		if (kicker.face != ACE) {
			kicker.face = ACE;
		} else {
			kicker.face = KING;
		}
		hs->kicker.push_back(kicker);
		return orgHs;
	}
	//此处不能调!!!! 20141104
//	hs->reset();

    if (NULL != this->isStraightFlush(hs, capHs)) { //首先排除是StraightFlush
        return NULL;
    }

    if (hs->rank.size() > 0) { //have done checking it once.
        if (hs->ranking == FourOfAKind) return hs;
        return NULL;
    }

	if (capHs && capHs->ranking != StraightFlush && capHs->ranking != FourOfAKind) return NULL;
	LYCard capTopRankCard = flipCard;
	if (capHs) capTopRankCard = capHs->topRankCard;

    boost::regex reg(ThreeOfAKindPtn);
    boost::cmatch matched;
    boost::sregex_token_iterator it(hs->cardString.begin(), hs->cardString.end(), reg, 0);
    boost::sregex_token_iterator end;

    for (; it != end; ++it) { //只取第一个
        std::string strMatched = *it;
//        std::cout << "matched:" << strMatched << std::endl;
        LYCard topRankCard(strMatched[0], strMatched[1]);
        if (capHs && capHs->ranking == FourOfAKind && topRankCard.face > capTopRankCard.face) return NULL;
//        std::cout << "hahaah:" << strMatched << std::endl;
        topRankCard.suit = Spades;
        hs->rank.push_back(topRankCard);
        hs->topRankCard = topRankCard;
        topRankCard.suit = Hearts;
        hs->rank.push_back(topRankCard);
        topRankCard.suit = Diamonds;
        hs->rank.push_back(topRankCard);
        topRankCard.suit = Clubs;
        hs->rank.push_back(topRankCard);

        hs->ranking = FourOfAKind;
        hs->rankingString = StringifiedFourOfAKind;

        //找出Kicker
        std::vector<LYCard>::iterator it = hs->cards.begin();
        for (; it!=hs->cards.end(); it++) {
        	LYCard cd = *it;
        	if (!cd.isGhost() && !hs->rankHasThisCard(cd)) {
        		hs->kicker.push_back(cd);
        		break; //因为kicker只可能有一个，所以取最大的一个即可！
        	}
        }
        return hs;
    }

    return NULL;
}

LYHandStrength * LYPineappleAlgorithmDelegate::isFullHouse(LYHandStrength *hs, LYHandStrength* capHs)
{
//LY_LOG_DBG("enter fullhouse");
	if (!hs->hasGhost()) return LYHoldemAlgorithmDelegate::isFullHouse(hs);

	if (this->isFourOfAKind(hs, capHs)) { //首先排除是FourOfAKind
        return NULL;
    }

    if (hs->rank.size() > 0) { //have done checking it once.
        if (hs->ranking == FullHouse) return hs;
        return NULL;
    }

	if (capHs && capHs->ranking != StraightFlush && capHs->ranking != FourOfAKind &&
			capHs->ranking != FullHouse) return NULL;
	LYCard capTopRankCard = flipCard;
	if (capHs) capTopRankCard = capHs->topRankCard;

    //首先来判断是否有三条
	boost::regex reg(ThreeOfAKindPtn);
    boost::sregex_token_iterator it(hs->cardString.begin(), hs->cardString.end(), reg, 0);
    boost::sregex_token_iterator end;

    for (; it != end; ++it) { //只取第一个
    	//说明有三条，但已经排除是四条
        std::string strMatched = *it;
        LYCard topRankCard(strMatched[0], strMatched[1]);
        if (capHs && capHs->ranking == FullHouse && topRankCard.face > capTopRankCard.face) return NULL;

        for (unsigned int i=0; i<hs->cardString.size()/2; i++) {
            char f = hs->cardString[i*2];
            char s = hs->cardString[i*2+1];
            LYCard card(f, s);
            if (!card.isGhost() && !hs->rankHasThisCard(card) && hs->rank.size() < 5) {
            	hs->rank.push_back(card);
            	if (card.face != topRankCard.face) {
            		LYCard ghostCard(card.face, Spades);
            		hs->rank.push_back(ghostCard);
            	}
            }
        }
        hs->ranking = FullHouse;
        hs->rankingString = StringifiedFullHouse;
        hs->topRankCard = topRankCard;
        return hs;
    }

    //其次判断是否有两对
    if (!LYHoldemAlgorithmDelegate::isTwoPair(hs)) return NULL;
//    LY_LOG_DBG("two pairs:" << hs->cardString);
    //此时hs已经设置成two pairs
    LYCard topPairCard = hs->rank[0];
    LYCard secondPairCard = hs->rank[2];
    if (capHs && capHs->ranking == FullHouse && secondPairCard.face > capTopRankCard.face) return NULL; //最小的对也比Cap的三条大，自然不可能是FullHouse
	hs->kicker.clear();
	hs->ranking = FullHouse;
    hs->rankingString = StringifiedFullHouse;
    if (!capHs || //在底道
    		capHs->ranking != FullHouse || //如果不是FullHouse，肯定比FullHouse大
    		topPairCard.face < capTopRankCard.face) {
    	hs->rank.push_back(topPairCard);
    } else {
    	hs->rank.push_back(secondPairCard);
    }
    LYCardHelpers::sortCardsByFace(hs->rank);
    return hs;
}

LYHandStrength * LYPineappleAlgorithmDelegate::isFlush(LYHandStrength *hs, LYHandStrength* capHs)
{
	if (!hs->hasGhost()) return LYHoldemAlgorithmDelegate::isFlush(hs);

	if (this->isStraightFlush(hs, capHs)) { //首先排除是StraightFlush
        return NULL;
    }

    if (hs->rank.size() > 0) { //have done checking it once.
        if (hs->ranking == Flush) return hs;
        hs->reset();
        return NULL;
    }

	if (capHs && capHs->ranking != StraightFlush && capHs->ranking != FourOfAKind &&
			capHs->ranking != FullHouse && capHs->ranking != Flush) return NULL;
	LYCard capTopRankCard = flipCard;
	if (capHs) capTopRankCard = capHs->topRankCard;

    boost::regex reg(FlushDrawPtn);
    boost::sregex_token_iterator it(hs->cardString.begin(), hs->cardString.end(), reg, 0);
    boost::sregex_token_iterator end;

    for (; it != end; ++it) { //只取第一个
        std::string strMatched = *it;
        char suitSymbol = strMatched[1];
//        std::cout << strMatched << std::endl;
        for (unsigned int i=0; i<strMatched.size()/2; i++) {
            char f = strMatched[i*2];
            char s = strMatched[i*2+1];
            LYCard card(f, s);
            if (!hs->rankHasThisCard(card) && s == suitSymbol && hs->rank.size()<4) {
            	hs->rank.push_back(card);
            }
        }
    	LYHoldemAlgorithmDelegate had;
        if (!capHs || capHs->ranking != Flush) {
        	//此时要么capHs为Null，要么capHs大于Flush
        	//不能简单的让Ghost＝A，因为需要排除一种可能性，本道＝A Hight Flush，可能已经有一个A了，此时有两个A，导致跟对手的AK Flush相比时比别人大
            for (int face=ACE; face>=TWO; face--) {
            	LYCard ghostCard((enum LYFace)face, LYCard::char2suit(suitSymbol));
            	if (hs->rankHasThisCard(ghostCard)) continue;
            	std::vector<LYCard> cds = hs->rank;
            	cds.push_back(ghostCard);
            	LYHandStrength tmpHs(cds, &had);
            	if (tmpHs.ranking != Flush) continue; //需要排除是StraightFlush

            	hs->rank.push_back(ghostCard);
            	hs->ranking = Flush;
            	hs->rankingString = StringifiedFlush;
            	LYCardHelpers::sortCardsByFace(hs->rank);
            	hs->topRankCard = hs->rank[0];
            	return hs;
            }
        }
        for (int face=ACE; face>=TWO; face--) {
        	LYCard ghostCard((enum LYFace)face, LYCard::char2suit(suitSymbol));
        	if (hs->rankHasThisCard(ghostCard)) continue;
        	std::vector<LYCard> cds = hs->rank;
//        	std::cout << "ghost:" << ghostCard.toString() << std::endl;
        	cds.push_back(ghostCard);
        	LYHandStrength tmpHs(cds, &had);
        	if (tmpHs.ranking != Flush) continue;
        	if ((capHs && !(tmpHs > *capHs)) ||
        			!capHs) {
            	hs->rank.push_back(ghostCard);
            	hs->ranking = Flush;
            	hs->rankingString = StringifiedFlush;
            	hs->topRankCard = tmpHs.topRankCard;
            	return hs;
        	}
        }
        hs->reset();
        return NULL;
    }
    hs->reset();
    return NULL;
}

LYHandStrength * LYPineappleAlgorithmDelegate::isStraight(LYHandStrength *hs, LYHandStrength* capHs)
{
	if (!hs->hasGhost()) return LYHoldemAlgorithmDelegate::isStraight(hs);

    if (NULL != this->isStraightFlush(hs, capHs)) { //首先排除是StraightFlush
        return NULL;
    }

    if (NULL != this->isFlush(hs, capHs)) { //首先排除是Flush
        return NULL;
    }

    if (hs->rank.size() > 0) {
//    	std::cout << "kkkkk" << std::endl;
        if (hs->ranking == Straight) return hs;
        return NULL;
    }

    if (capHs && capHs->ranking != StraightFlush && capHs->ranking != FourOfAKind &&
			capHs->ranking != FullHouse && capHs->ranking != Flush && capHs->ranking != Straight) return NULL;
	LYCard capTopRankCard = flipCard;
	if (capHs) capTopRankCard = capHs->topRankCard;

	std::string gushot_ptn(GushotStraightDrawPtn);
	std::string ptn(OpenEndedStraightDrawPtn);
	ptn = ptn + "|" + gushot_ptn;
    boost::regex reg(ptn.c_str());
    boost::sregex_token_iterator it(hs->cardString.begin(), hs->cardString.end(), reg, 0);
    boost::sregex_token_iterator end;

    for (; it != end; ++it) { //只取第一个
        std::string strMatched = *it;
//    	std::cout << strMatched << std::endl;
        for (unsigned int i=0; i<strMatched.size()/2; i++) {
            char f = strMatched[i*2];
            char s = strMatched[i*2+1];
            LYCard card(f, s);
            if (!hs->rankHasThisFace(card) && hs->rank.size()<4) {
            	hs->rank.push_back(card);
            }
        }
    }

    //然后判断是否A－5的GushotStraight，此时A和5肯定已经在hs->rank中
    boost::regex reg2(GushotAceToFiveStraightDrawPtn);
    boost::sregex_token_iterator it2(hs->cardString.begin(), hs->cardString.end(), reg2, 0);

    for (; it2 != end; ++it2) { //只取第一个
        std::string strMatched = *it2;
//        std::cout << strMatched << std::endl;
    	//因为是最小的顺子，所以肯定成立
    	for (unsigned int s=Spades; s>=Clubs; s--) {
    		if (s == hs->rank[0].suit) continue;
    		//任意取一个非同花色的2－4即可，保证不是同花
    		for (unsigned int f=FIVE; f>=TWO; f--) {
    			LYCard cd((enum LYFace)f, (enum LYSuit)s);
    			if (hs->rankHasThisFace(cd)) continue;
    			hs->rank.push_back(cd);
    			hs->ranking = Straight;
            	hs->rankingString = StringifiedStraight;
            	LYCardHelpers::sortCardsByFace(hs->rank);
            	hs->topRankCard = hs->rank[1];
            	return hs;
    		}
    	}
    }

    //其次判断是否其它的GushotStraight，此时头和尾肯定已经在hs->rank中
    boost::regex reg3(GushotStraightDrawPtn);
    boost::sregex_token_iterator it3(hs->cardString.begin(), hs->cardString.end(), reg3, 0);

    for (; it3 != end; ++it3) { //只取第一个
    	//需要判断此时的顺子是否比capHs大
        std::string strMatched = *it3;
//    	std::cout << strMatched << std::endl;
    	if (capHs && capHs->ranking == Straight && hs->rank[0].face > capTopRankCard.face) {
    		return NULL;
    	}

    	for (unsigned int s=Spades; s>=Clubs; s--) {
    		if (s == hs->rank[0].suit) continue;
    		//任意取一个非同花色的牌即可，保证不是同花
    		for (unsigned int f=(unsigned int)(hs->rank[0].face)-1; f>=hs->rank[3].face; f--) {
    			LYCard cd((LYFace)f, (LYSuit)s);
    			if (hs->rankHasThisFace(cd)) continue;
    			hs->rank.push_back(cd);
    			hs->ranking = Straight;
            	hs->rankingString = StringifiedStraight;
            	LYCardHelpers::sortCardsByFace(hs->rank);
            	hs->topRankCard = hs->rank[0];
            	return hs;
    		}
    	}
    }

    //最后判断是否OpenEndedStraight
    boost::regex reg4(OpenEndedStraightDrawPtn);
    boost::sregex_token_iterator it4(hs->cardString.begin(), hs->cardString.end(), reg4, 0);

    for (; it4 != end; ++it4) { //只取第一个
//    	std::cout << "here!" << capTopRankCard.face << std::endl;
    	//需要判断此时的顺子是否比capHs大
    	LYFace f = (LYFace)(hs->rank[0].face+1);
    	if (capHs && capHs->ranking == Straight && f >= capTopRankCard.face) {
    		f = (LYFace)(hs->rank[3].face - 1);
    	}
    	for (unsigned int s=Spades; s>=Clubs; s--) {
    		if (s == hs->rank[0].suit) continue;
    		LYCard cd((LYFace)f, (LYSuit)s);
    		hs->rank.push_back(cd);
    		hs->ranking = Straight;
    		hs->rankingString = StringifiedStraight;
    		LYCardHelpers::sortCardsByFace(hs->rank);
    		hs->topRankCard = hs->rank[0];
    		if (hs->topRankCard.face == ACE && hs->rank[1].face == FIVE) hs->topRankCard = hs->rank[1];
        	if (!capHs ||
        			(capHs->ranking != Straight ||
        					hs->topRankCard.face <= capTopRankCard.face)) {
            	return hs;
        	}
        	hs->reset();
        	return NULL;
    	}
    }
    return NULL;
}

/*
 * 以上除了FourOfAKind，其它的都是需要五张牌才能成
 * 以下三张、二张、或1张都可以成牌
 */
LYHandStrength * LYPineappleAlgorithmDelegate::isThreeOfAKind(LYHandStrength *hs, LYHandStrength* capHs)
{
//	std::cout << "3t" << std::endl;
	if (!hs->hasGhost()) return LYHoldemAlgorithmDelegate::isThreeOfAKind(hs);
    if (NULL != this->isStraightFlush(hs, capHs)) { //首先排除是StraightFlush
        return NULL;
    }
    if (NULL != this->isFourOfAKind(hs, capHs)) { //not four of a kind
    	return NULL;
    }
    if (NULL != this->isFlush(hs, capHs)) { //排除是Flush
        return NULL;
    }
    if (NULL != this->isStraight(hs, capHs)) { //排除是Straight
        return NULL;
    }

	//如果已经是三条了自然必须得返回三条了
	if (LYHoldemAlgorithmDelegate::isThreeOfAKind(hs)) {
//		std::cout << "kkkk" << std::endl;
		//设置一下Ghost即可
		unsigned int kicker_face = 0;
		unsigned int kicker_suit = 0;
		for (unsigned int i=0; i<2; i++) {
			if (!hs->kicker[i].isGhost()) {
				kicker_face = hs->kicker[i].face;
				kicker_suit = hs->kicker[i].suit;
				break;
			}
		}
		LYCard k((LYFace)kicker_face, (LYSuit)kicker_suit);
		unsigned int f = ACE;
		for (; f>=TWO; f--) {
			if (!LYCardHelpers::hasThisFace(hs->cards, (LYFace)f)) break;
		}
		hs->kicker.clear();
		hs->kicker.push_back(k);
		LYCard ghostCard((LYFace)f, Spades);
//		std::cout << ghostCard.toString() << std::endl;
		hs->kicker.push_back(ghostCard);
//		LYCardHelpers::sortCardsByFace(hs->rank);
		LYCardHelpers::sortCardsByFace(hs->kicker);
		return hs;
	}
	hs->reset();
	//如果是两对，就不可能是三条
	if (LYHoldemAlgorithmDelegate::isTwoPair(hs)) return NULL;
	hs->reset();
	//如果一对都没有，当然不可能是三条
	if (!LYHoldemAlgorithmDelegate::isOnePair(hs)) return NULL;

	if (capHs && capHs->ranking != StraightFlush && capHs->ranking != FourOfAKind &&
			capHs->ranking != FullHouse && capHs->ranking != Flush && capHs->ranking != Straight
			&& capHs->ranking != ThreeOfAKind) return NULL;

	if (capHs && capHs->ranking == ThreeOfAKind && capHs->topRankCard.face < hs->topRankCard.face) {
		hs->reset();
		return NULL;
	}

	for (unsigned int s=Spades; s>=Clubs; s--) {
		LYCard ghostCard(hs->topRankCard.face, (LYSuit)s);
		if (!hs->rankHasThisCard(ghostCard)) {
			hs->rank.push_back(ghostCard);
			break;
		}
	}

	std::vector<LYCard>::iterator cit = hs->kicker.begin();
	for (; cit!=hs->kicker.end(); cit++) {
		if ((*cit).isGhost()) {
			hs->kicker.erase(cit);
			break;
		}
	}
	hs->ranking = ThreeOfAKind;
	hs->rankingString = StringifiedThreeOfAKind;
	LYCardHelpers::sortCardsByFace(hs->rank);
	LYCardHelpers::sortCardsByFace(hs->kicker);
	return hs;
}

LYHandStrength * LYPineappleAlgorithmDelegate::isTwoPair(LYHandStrength *hs, LYHandStrength* capHs)
{
//	std::cout << "2p" << std::endl;
	if (!hs->hasGhost()) return LYHoldemAlgorithmDelegate::isTwoPair(hs);

    if (NULL != this->isStraightFlush(hs, capHs)) { //首先排除是StraightFlush
        return NULL;
    }
    if (NULL != this->isFourOfAKind(hs, capHs)) { //not four of a kind
    	return NULL;
    }
    if (NULL != this->isFlush(hs, capHs)) { //排除是Flush
        return NULL;
    }
    if (NULL != this->isStraight(hs, capHs)) { //排除是Straight
        return NULL;
    }
    if (NULL != this->isThreeOfAKind(hs, capHs)) { //排除是ThreeOfAKind
        return NULL;
    }

	//如果已经是2P了自然必须得返回2P了
	if (LYHoldemAlgorithmDelegate::isTwoPair(hs)) {
		//设置一下Ghost即可
		//有非常小的概率是底下两对和中间两对完全一样，只是Kicker不同，基本无需考虑这种情况！！！！
		//反人类的摆法才可能出现
		for (unsigned int f=ACE; f>=TWO; f--) {
			LYCard ghostCard((LYFace)f, Spades);
			if (!hs->rankHasThisFace(ghostCard)) {
				hs->kicker.clear();
				hs->kicker.push_back(ghostCard);
				break;
			}
		}
		return hs;
	}
	hs->reset();
	//如果一对都没有，当然不可能是两对
	if (!LYHoldemAlgorithmDelegate::isOnePair(hs)) return NULL;

	LYHoldemAlgorithmDelegate had;
	std::vector<LYCard> newCards = hs->rank;
    for (unsigned int i=0; i<hs->kicker.size(); i++) {
    	if (hs->kicker[i].isGhost()) continue;
    	newCards.push_back(hs->kicker[i]);
    }
//	std::cout << newCards.size() << std::endl;

    for (unsigned int i=0; i<hs->kicker.size(); i++) {
    	if (hs->kicker[i].isGhost()) continue;
    	unsigned int s = hs->kicker[i].suit;
    	if (s == Spades) {
    		s = Hearts;
    	} else {
    		s = Spades;
    	}
    	LYCard ghostCard(hs->kicker[i].face, (LYSuit)s);
//    	std::cout << "ghost=" << ghostCard.toString() << std::endl;
    	newCards.push_back(ghostCard);
    	LYHandStrength tmpHs(newCards, &had);
    	if (tmpHs.ranking != TwoPair) {
    		hs->reset();
    		return NULL;
    	}
    	if (!capHs ||
    			!(tmpHs > *capHs)) {
        	hs->rank = tmpHs.rank;
        	hs->kicker = tmpHs.kicker;
        	hs->ranking = TwoPair;
        	hs->rankingString = StringifiedTwoPairs;
        	hs->topRankCard = tmpHs.topRankCard;
        	return hs;
    	}
    	newCards.pop_back();
    }

    hs->reset();
    return NULL;
}

LYHandStrength * LYPineappleAlgorithmDelegate::isOnePair(LYHandStrength *hs, LYHandStrength* capHs)
{
//	std::cout << "onep" << std::endl;
	LYHoldemAlgorithmDelegate had;
	if (!hs->hasGhost()) return LYHoldemAlgorithmDelegate::isOnePair(hs);
    if (NULL != this->isStraight(hs, capHs)) { //首先排除是TwoPair以上牌力
        return NULL;
    }

    if (NULL != this->isTwoPair(hs, capHs)) { //首先排除是TwoPair以上牌力
        return NULL;
    }

	std::vector<LYCard> cards = hs->cards;
	std::vector<LYCard>::iterator it = cards.begin();
	for (; it!=cards.end(); it++) {
		if ((*it).isGhost()) {
			cards.erase(it);
			break;
		}
	}
	//如果已经是1P了自然必须得返回1P了
	if (LYHoldemAlgorithmDelegate::isOnePair(hs)) {
		//设置一下Ghost即可
		for (unsigned int f=ACE; f>=TWO; f--) {
			LYCard ghostCard((LYFace)f, Spades);
			if (!hs->rankHasThisFace(ghostCard)) {
				cards.push_back(ghostCard);
		    	LYHandStrength tmpHs(cards, &had);
		    	if (tmpHs.ranking != OnePair) {
		    		cards.pop_back();
		    		hs->reset();
		    		continue;
		    	}
		    	if (!capHs ||
		    			!(tmpHs > *capHs)) {
		        	hs->rank = tmpHs.rank;
		        	hs->kicker = tmpHs.kicker;
		        	hs->ranking = OnePair;
		        	hs->rankingString = StringifiedOnePair;
		        	hs->topRankCard = tmpHs.topRankCard;
		        	return hs;
		    	}

		    	cards.pop_back();
		    	hs->reset();
			}
		}
		return hs;
	}
	hs->reset();
//	std::cout << "kkkk" << std::endl;

    for (unsigned int i=0; i<hs->cards.size(); i++) {
    	if (hs->cards[i].isGhost()) continue;
    	unsigned int s = hs->cards[i].suit;
    	if (s == Spades) {
    		s = Hearts;
    	} else {
    		s = Spades;
    	}
    	LYCard ghostCard(hs->cards[i].face, (LYSuit)s);
    	cards.push_back(ghostCard);
//    	std::cout << "ghost:" << ghostCard.toString() << std::endl;
    	LYHandStrength tmpHs(cards, &had);
    	if (tmpHs.ranking != OnePair) return NULL;
    	if (!capHs ||
    			!(tmpHs > *capHs)){
        	hs->rank = tmpHs.rank;
        	hs->kicker = tmpHs.kicker;
        	hs->ranking = OnePair;
        	hs->rankingString = StringifiedOnePair;
        	hs->topRankCard = tmpHs.topRankCard;
        	return hs;
    	}
    	cards.pop_back();
    }

    return NULL;
}

LYHandStrength * LYPineappleAlgorithmDelegate::isHighCard(LYHandStrength *hs, LYHandStrength* capHs)
{
//LY_LOG_DBG("isHighCard?" << hs->cardString);
	if (!hs->hasGhost()) return LYHoldemAlgorithmDelegate::isHighCard(hs);
    if (NULL != this->isStraightFlush(hs, capHs)) { //首先排除是TwoPair以上牌力
        return NULL;
    }

    if (NULL != this->isFourOfAKind(hs, capHs)) { //首先排除是TwoPair以上牌力
        return NULL;
    }

    if (NULL != this->isFullHouse(hs, capHs)) { //首先排除是TwoPair以上牌力
        return NULL;
    }

    if (NULL != this->isFlush(hs, capHs)) { //首先排除是TwoPair以上牌力
        return NULL;
    }

    if (NULL != this->isStraight(hs, capHs)) { //首先排除是TwoPair以上牌力
        return NULL;
    }

    if (NULL != this->isThreeOfAKind(hs, capHs)) { //排除是ThreeOfAKind
        return NULL;
    }

    if (NULL != this->isTwoPair(hs, capHs)) { //首先排除是OnePair以上牌力
        return NULL;
    }

    if (NULL != this->isOnePair(hs, capHs)) { //首先排除是OnePair以上牌力
        return NULL;
    }
//std::cout << "high card!!!" << std::endl;
	std::vector<LYCard> cards = hs->cards;
	std::vector<LYCard>::iterator it = cards.begin();
	for (; it!=cards.end(); it++) {
		if ((*it).isGhost()) {
			cards.erase(it);
			break;
		}
	}

	LYCardHelpers::sortCardsByFace(cards);
    enum LYFace f = cards[0].face;
    enum LYSuit s = cards[0].suit;
    if (s == Spades) {
    	s = Hearts;
    } else {
    	s = Spades;
    }
	LYHoldemAlgorithmDelegate had;
	enum LYFace min_face = ACE;
    for (unsigned int i=ACE; i>=TWO; i--) {
    	LYCard cd((LYFace)i, s);
//    	std::cout << cd.toString() << std::endl;
    	if (LYCardHelpers::hasThisFace(cards, cd)) continue;
    	min_face = (LYFace)i;
    	cards.push_back(cd);
    	LYHandStrength tmpHs(cards, &had);
    	if (tmpHs.ranking != HighCard) return NULL;
    	if ((capHs && !(tmpHs > *capHs)) ||
    			!capHs) {
        	hs->rank = tmpHs.rank;
        	hs->kicker = tmpHs.kicker;
        	hs->ranking = HighCard;
        	hs->rankingString = StringifiedOnePair;
        	hs->topRankCard = tmpHs.topRankCard;
        	return hs;
    	}
    	cards.pop_back();
    }
    //最后即使不比Cap大，也得是一个HighCard，最小的一个HighCard
    hs->reset();
    LYCard cd(min_face, s);
	cards.push_back(cd);
	LYHandStrength tmpHs(cards, &had);

	hs->rank = tmpHs.rank;
	hs->kicker = tmpHs.kicker;
	hs->ranking = HighCard;
	hs->rankingString = StringifiedOnePair;
	hs->topRankCard = tmpHs.topRankCard;
    return hs;
}

