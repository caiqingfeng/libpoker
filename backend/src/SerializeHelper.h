/*
 * SerializeHelper.h
 *
 *  Created on: 2014年10月16日
 *      Author: luyunstaff001
 */

#ifndef SERIALIZEHELPER_H_
#define SERIALIZEHELPER_H_
#include "libmrock/common/src/LYUtils.h"
#include "poker/src/LYSeat.h"
#include "poker/src/LYTable.h"

class SerializeHelper {
public:
	SerializeHelper();
	virtual ~SerializeHelper();
//	static std::vector<std::pair<std::string, std::string> > table2kvp(
//			LYTable *table);
	static void kvp2table(std::vector<std::pair<std::string, std::string> >& to,
			LYTable *table);
	static std::vector<std::pair<std::string, std::string> > tableStatus2kvp(
			LYTable *table);
	static std::vector<std::pair<std::string, std::string> > seatStatus2kvp(
			LYSeat* seat);
//	static std::vector<std::pair<std::string, std::string> > seat2kvp(
//		LYSeat* seat);
	static void kvp2seat(std::vector<std::pair<std::string, std::string> >& to,
		LYSeat* seat);

};

#endif /* SERIALIZEHELPER_H_ */
