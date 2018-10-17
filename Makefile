#started with '#' are comments
include ./Makefile.inc

#OBJS := $(wildcard poker/obj/*.o)
POKERSRCS=$(wildcard poker/src/*.cpp)
POKEROBJS := $(patsubst %.cpp,poker/obj/%.o,$(notdir $(POKERSRCS)))
HOLDEMSRCS=$(wildcard holdem/src/*.cpp)
HOLDEMOBJS := $(patsubst %.cpp,holdem/obj/%.o,$(notdir $(HOLDEMSRCS)))
PINEAPPLESRCS=$(wildcard pineapple/src/*.cpp)
PINEAPPLEOBJS := $(patsubst %.cpp,pineapple/obj/%.o,$(notdir $(PINEAPPLESRCS)))
OBJS=$(POKEROBJS)
OBJS += $(HOLDEMOBJS)
OBJS += $(PINEAPPLEOBJS)

DIRS=poker holdem pineapple backend
LIBP := lib
MKDIRS := $(LIBP)

#all: src test 
all: poker holdem pineapple backend frontend
test: poker-tests holdem-tests pineapple-tests backend-tests frontend-tests

backend: force_look
	@echo "make allpoker"
	cd backend; $(MAKE)
	@echo "build allpoker end"

backend-tests: force_look
	@echo "test allpoker"
	cd backend; $(MAKE) test
	@echo "test allpoker end"

frontend: force_look
	@echo "make allpoker"
	cd frontend; $(MAKE)
	@echo "build allpoker-frontend end"

frontend-tests: force_look
	@echo "test allpoker"
	cd frontend; $(MAKE) test
	@echo "test allpoker-frontend end"

poker: force_look
	@echo "make poker"
	cd poker; $(MAKE)
	@echo "build poker end"

poker-tests: force_look
	cd poker; $(MAKE) test

holdem: force_look
	@echo "make holdem"
	cd holdem; $(MAKE)
	@echo "build holdem end"

holdem-tests: force_look
	cd holdem; $(MAKE) test

pineapple: force_look
	@echo "make pineapple"
	cd pineapple; $(MAKE)
	@echo "build pineapple end"

pineapple-tests: force_look
	cd pineapple; $(MAKE) test

clean:
	@echo "clean everything......"
	find . -name "core" -exec rm {} \;
	-for d in $(DIRS); do (cd $$d; make clean; cd ..); done

rebuild:
	@echo "run protoc to compile protobuf......"
	./holdem_proto_rebuild.sh
	./timer_proto_rebuild.sh
	make clean all

force_look :
	true

.phony: clean

$(sort $(MKDIRS)) :
	mkdir -p $@
