#started with '#' are comments
include ./Makefile.inc

DIRS=frontend poker holdem pineapple
OBJS := $(wildcard frontend/obj/*.o)
OBJS += $(wildcard poker/obj/*.o)
OBJS += $(wildcard holdem/obj/*.o)
OBJS += $(wildcard pineapple/obj/*.o)

all: allpoker

allpoker: force_look
	@echo "make allpoker"
	cd poker; $(MAKE)
	cd holdem; $(MAKE)
	cd pineapple; $(MAKE)
	cd frontend; $(MAKE)
	@echo "build allpoker end"
	
install:
	mkdir -p lib
	$(AR) $(ARFLAGS) lib/liballpoker.a $(OBJS)
	$(RANLIB) lib/liballpoker.a
	-for d in $(DIRS); do (mkdir -p include/$$d/src; cp $$d/src/*.h include/$$d/src; cp $$d/src/*.hpp include/$$d/src); done

init:
	-rm -rf include/*
	-rm -rf holdem/src/*
	-rm -rf poker/src/*
	-rm -rf pineapple/src/*
	-rm -rf lib/*
	-rm -rf frontend/src/*
	-for d in $(DIRS); do (mkdir -p $$d; rm -rf $$d/src; cp -r ../$$d/src $$d/src;); done

clean:
	@echo "clean everything......"
	find . -name "core" -exec rm {} \;
	for d in $(DIRS); do (cd $$d; make clean; cd ..); done
	
finalize:
	-rm -r include/*
	-rm lib/*
	-for d in $(DIRS); do (rm -r $$d/src); done

force_look :
	true

.phony: init clean install
