#include "LYPineappleProfile.h"

LYPineappleProfile::LYPineappleProfile()
{

}

LYPineappleProfile::~LYPineappleProfile()
{

}

bool LYPineappleProfile::isValidBuyin(unsigned int chips)
{
	if (chips > min_chips) return true;
	return false;
}
