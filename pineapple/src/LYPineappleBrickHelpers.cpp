#include "LYPineappleBrickHelpers.h"

#include <vector>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

void LYPineappleBrickHelpers::bricksToStringifiedInt(std::vector<LYPineappleBrick>& bricks, std::string& cs)
{
	cs = "";
	std::vector<LYPineappleBrick>::iterator it = bricks.begin();
	for (; it!=bricks.end(); it++) {
		LYPineappleBrick brick = *it;
		cs = cs + boost::lexical_cast<std::string>(brick.brick) + ":" +
				boost::lexical_cast<std::string>(brick.card.toInteger()) + ",";
	}
}

void LYPineappleBrickHelpers::stringifiedIntToBricks(const std::string& cs, std::vector<LYPineappleBrick>& bricks)
{
	bricks.clear();
	std::vector<std::string> cds;
	boost::split(cds, cs, boost::is_any_of(","));
	std::vector<std::string>::iterator it = cds.begin();
	for (; it!=cds.end(); it++) {
		if (*it == "") continue;
		std::vector<std::string> brk;
		boost::split(brk, *it, boost::is_any_of(":"));
		if (brk.size() != 2) continue;
		int brick = boost::lexical_cast<int>(brk[0]);
		int cd = boost::lexical_cast<int>(brk[1]);
		LYCard card(cd);
		LYPineappleBrick pb(brick, card);
		bricks.push_back(pb);
	}
}
