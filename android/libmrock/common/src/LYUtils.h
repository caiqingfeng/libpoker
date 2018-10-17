#ifndef LYUTILS_H_
#define LYUTILS_H_

#include <sstream>
#include <algorithm>
#include <unistd.h>
#include <string>
#include "md5.h"
#include "my_log.h"

#include <iostream>
#include <iomanip>

#include <sys/time.h>
#include <time.h>
#include <assert.h>
#include <pthread.h>
#include <stdlib.h>        // random()  RAND_MAX
#include <stdio.h>
#include <stdarg.h>
#include <signal.h>

class LYUtils {
public:
	static std::string intToHex(int i, unsigned int length);
	static std::string genId();
	static void genDigest(std::string& msg, std::string& key, std::string& digest);
	static void printTextOrBinary(const std::string& tag, const std::string& str);
};

#endif
