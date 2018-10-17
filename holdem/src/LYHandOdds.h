/*
 * LYHandOdds.h
 *
 *  Created on: 2013-6-5
 *      Author: caiqingfeng
 */

#ifndef LYHANDODDS_H_
#define LYHANDODDS_H_
#include <vector>
#include <map>
#include <memory>
#include "LYHandStrength.h"
#include "LYHoldemAlgorithmDelegate.h"

class LYHandOdds : public LYHandStrength{
public:
	LYHandOdds(std::vector<LYCard > const &cds, LYHoldemAlgorithmDelegate *had=NULL);
	virtual ~LYHandOdds();
	void putCardIntoMap(const LYCard &cd);
	std::vector<std::string> genIndex(const LYCard &cd);
	unsigned int flush_base();
	unsigned int straight_base();
	std::vector<LYCard> get_odds(std::string idx);
//	unsigned int gutshot_base();

private:
	/* index for oddsMap
	 * spades: for spades, clubs: for clubs, diamonds: for diamonds, hearts for hearts
	 * A25: for ace to five straight, ...
	 * sA25: for spades ace to five straight flush
	 * ASet: for three of aces, ...
	 * AQuad: for four of aces, ...
	 * AKFull: for AAAKK
	 * AK2P: for AAKK
	 * A1P: for AA
	 */
	std::map<std::string, std::vector<LYCard> > oddsMap;
	void clean_dup_straight();
	void clean_dup_fullhouse();
};

typedef ::std::shared_ptr<LYHandOdds> LYHandOddsPtr;

#endif /* LYHANDODDS_H_ */
