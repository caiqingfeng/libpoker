/*
 * LYBsonBuilder.h
 *
 *  Created on: 2013-6-26
 *      Author: caiqingfeng
 */

#ifndef OTASERIALIZEHELPER_H_
#define OTASERIALIZEHELPER_H_

#include "timer.pb.h"
#include "LYTimer.h"

using namespace com::luyun::timer::shared;

/*
 * helper class to build bson object for LYGame/LYSeat/LYPot/LYHandStrength
 * and helper class to build OtaMessage
 * to be divided into one common part shared by server and client both
 */
class OtaSerializeHelper {
public:
	OtaSerializeHelper();
	
public:
	virtual ~OtaSerializeHelper();

	static LYTimer ota2timer(LYTimerMsg &tmsg);
    static void timer2ota(LYTimer timer, LYTimerMsg *tmsg);
};

#endif /* LYBSONBUILDER_H_ */
