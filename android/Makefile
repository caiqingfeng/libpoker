#started with '#' are comments

DIRS= poker holdem pineapple

OBJS := $(wildcard poker/obj/*.o)
OBJS += $(wildcard holdem/obj/*.o)
OBJS += $(wildcard pineapple/obj/*.o)
LIBP := lib
MKDIRS := $(LIBP)
INCP := include
MKDIRS += $(INCP)

#for d in $(DIRS); do (OBJS += $(wildcard $$d/obj/*.o); done

all: poker holdem pineapple

common: force_look
	@echo "make common"
	cd common; $(MAKE)
	@echo "build common end"

poker: force_look
	@echo "make poker"
	cd poker; $(MAKE)
	@echo "build poker end"

holdem: force_look
	@echo "make holdem"
	cd holdem; $(MAKE)
	@echo "build holdem end"

pineapple: force_look
	@echo "make pineapple"
	cd pineapple; $(MAKE)
	@echo "build client end"
	
install: |$(LIBP) $(INCP)
	$(ECHO) $(AR) $(ARFLAGS) lib/liballpoker.a $(OBJS)
	$(AR) $(ARFLAGS) lib/liballpoker.a $(OBJS)
	$(RANLIB) lib/liballpoker.a
	-for d in $(DIRS); do (mkdir -p include/$$d/src; cp $$d/src/*.h include/$$d/src; cp $$d/src/*.hpp include/$$d/src); done 

init:
	-rm -r include/*
	-rm lib/*
	-for d in $(DIRS); do (mkdir -p $$d; rm -r $$d/src; cp -r ../$$d/src $$d); done

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

$(sort $(MKDIRS)) :
	mkdir -p $@

