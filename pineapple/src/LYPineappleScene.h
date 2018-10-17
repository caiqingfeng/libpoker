/*
 * LYPineappleScene.h
 *
 *  Created on: 2014-9-16
 *      Author: caiqingfeng
 */

#ifndef LYPINEAPPLECENE_H_
#define LYPINEAPPLECENE_H_

#include <string>
#include <vector>
#include "poker/src/LYPokerConstants.h"
#include "poker/src/LYSeat.h"
#include "LYPineappleSeat.h"

/*
 * PineappleScene保存到内存数据库中
 * 每个动作影响到Duty，Action等
 */
class LYPineappleStatic {
public:
//	unsigned int gameType; //Game类型
	std::string profile; //Game profile
	LYApplicant btnSeatNo; //每局的button不变
};

const unsigned int LYPINEAPPLE_GAME_ONGOING = 0;
const unsigned int LYPINEAPPLE_GAME_CLOSED = 1;

class LYPineappleDynamics {
public:
	unsigned int gameStatus; //0:正在进行，1：结束
	unsigned int seqNo; //sequence of game scene, for synchronize
	LYApplicant dutySeatNo;
};

class LYPineappleScene : public LYPineappleDynamics{
public:
	std::string _id; //唯一标识桌子
	std::vector<LYPineappleSeatDynamics> seats; //所有Seats的情况，只处只存Seat的动态信息

public:
	LYPineappleScene(std::vector<LYSeatPtr> &all_seats,
			enum LYApplicant duty_seat,
			unsigned int seq);
	virtual ~LYPineappleScene();
	std::string toString();
};

typedef std::shared_ptr<LYPineappleScene> LYPineappleScenePtr;

#endif /* LYHOLDEMSCENE_H_ */
