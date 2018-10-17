/*
 * LYDevice.h
 *
 *  Created on: 2013-7-3
 *      Author: caiqingfeng
 */

#ifndef LYDEVICE_H_
#define LYDEVICE_H_

#include <string>

enum LYOsType {
    LY_ANDROID = 0,
    LY_IOS = 1,
    LY_WP = 2,
    LY_OS_TYPE_OTHER = 9
};

class LYDevice {
public:
	std::string device_model;
	enum LYOsType os_type;
	std::string os_version;
	int ly_major_release;
	int ly_minor_release;
	std::string download_url;
	std::string desc;

public:
	LYDevice();
	virtual ~LYDevice();
};

#endif /* LYDEVICE_H_ */
