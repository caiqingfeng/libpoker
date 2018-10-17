#started with '#' are comments

#LIBMROCK=`pwd`/../../libmrock

LIB_GTEST=-L/usr/local/lib -L/usr/local/lib/gtest/lib -lgtest -lpthread

LIB_TST = $(LIB_GTEST) $(LIB_GMOCK) -lboost_regex -L$(LIBMROCK)/backend/lib/ -lmrock-backend -lcryptopp -llog4cplus
LIB_TST += -L$(LIBMROCK)/persistence/lib/ -lpersistence -lhiredis

LIB_FRONTEND_TST = $(LIB_GTEST) $(LIB_GMOCK) -lboost_regex -L$(LIBMROCK)/frontend/lib/ -lmrock-frontend -lcryptopp -llog4cplus

CPP_FLAGS_32 := -DBIT32
CPP_FLAGS_64 := -DBIT64
ARCH := $(shell getconf LONG_BIT)

CC=g++
CC_FLAG=$(CPP_FLAGS_$(ARCH)) -std=c++0x -fPIC -Wall -Wno-unused-function -g -I/usr/local/include -I../ -I. -I../../ -I$(LIBMROCK)/../
CC_FLAG += -I$(LIBMROCK)

AR  = ar
ARFLAGS = rv
RANLIB  = ranlib
RM  = rm
MAKE = make -S
