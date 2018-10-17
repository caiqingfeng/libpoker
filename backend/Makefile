#started with '#' are comments
include ../Makefile.inc

SRCP=src
TSTP=test
OBJP=obj
MKDIRS += $(OBJP)
LIBP=lib
MKDIRS += $(LIBP)
BINP=bin
MKDIRS += $(BINP)

CURP=$(CURDIR)

#src
CPPSRCS := $(wildcard $(SRCP)/*.cpp)
CPPOBJS := $(patsubst $(SRCP)/%.cpp,$(OBJP)/%.o,$(CPPSRCS)) 
TSTSRCS := $(wildcard $(TSTP)/*.cpp)
TSTOBJS := $(patsubst $(TSTP)/%.cpp,$(OBJP)/%.o,$(TSTSRCS))

DEPFILES := $(patsubst %.cpp,$(OBJP)/%.d,$(CPPSRCS))
-include $(DEPFILES)

#src depend dirs
POKERP=../poker
DPDIRS += $(POKERP)
HOLDEMP=../holdem
DPDIRS += $(HOLDEMP)
PINEAPPLEP=../pineapple
DPDIRS += $(PINEAPPLEP)

#poker src depend objs and libs
DPCPPSRCS := $(wildcard $(POKERP)/src/*.cpp)
DPOBJS := $(patsubst %.cpp,$(POKERP)/obj/%.o,$(notdir $(DPCPPSRCS)))
DPCCSRCS := $(wildcard $(POKERP)/src/*.cc)
DPOBJS += $(patsubst %.cc,$(POKERP)/obj/%.o,$(notdir $(DPCCSRCS)))

#holdem src depend objs and libs
HOLDEMCPPSRCS := $(wildcard $(HOLDEMP)/src/*.cpp)
HOLDEMOBJS := $(patsubst %.cpp,$(HOLDEMP)/obj/%.o,$(notdir $(HOLDEMCPPSRCS)))
HOLDEMCCSRCS := $(wildcard $(HOLDEMP)/src/*.cc)
HOLDEMOBJS += $(patsubst %.cc,$(HOLDEMP)/obj/%.o,$(notdir $(HOLDEMCCSRCS)))
DPOBJS += $(HOLDEMOBJS)

#pineapple src depend objs and libs
PINEAPPLECPPSRCS := $(wildcard $(PINEAPPLEP)/src/*.cpp)
PINEAPPLEOBJS := $(patsubst %.cpp,$(PINEAPPLEP)/obj/%.o,$(notdir $(PINEAPPLECPPSRCS)))
PINEAPPLECCSRCS := $(wildcard $(PINEAPPLEP)/src/*.cc)
PINEAPPLEOBJS += $(patsubst %.cc,$(PINEAPPLEP)/obj/%.o,$(notdir $(PINEAPPLECCSRCS)))
DPOBJS += $(PINEAPPLEOBJS)

#test depend objs
TSTDPOBJS=$(DPOBJS)
TSTDPSRCS=$(filter-out $(SRCP)/appd.cpp,$(wildcard $(SRCP)/*.cpp))
TSTDPOBJS += $(patsubst %.cpp,$(OBJP)/%.o,$(notdir $(TSTDPSRCS)))
TSTDPCCSRCS := $(wildcard $(SRCP)/*.cc)
TSTDPOBJS += $(patsubst %.cc,$(OBJP)/%.o,$(notdir $(TSTDPCCSRCS)))
TSTDPOBJS += $(DPOBJS)

LIB_SERVER += -L./lib -lallpoker-backend

all: backend backend-tests

objs: $(CPPOBJS) $(CCOBJS) $(TSTOBJS)
	@echo "make backend depend objs"
	-for d in $(DPDIRS); do (cd $$d; $(MAKE) objs || exit "$$?");done
	cd $(CURP)

$(CPPOBJS): $(OBJP)/%.o:$(SRCP)/%.cpp | $(OBJP)
	$(CXX) $(CC_FLAG) -c $< -MMD -o $@

$(CCOBJS): $(OBJP)/%.o:$(SRCP)/%.cc | $(OBJP)
	$(CXX) $(CC_FLAG) -c $< -MMD -o $@

$(TSTOBJS): $(OBJP)/%.o:$(TSTP)/%.cpp | $(OBJP)
	$(CXX) $(CC_FLAG) -c $< -MMD -o $@

$(DPOBJS): $(DPSRCS)
	cd $(COMMONP); make

backend: $(CCOBJS) $(CPPOBJS) $(DPOBJS) $(DPLIBS) | $(LIBP)
	@echo "Link backend begin......"
	$(ECHO) $(AR) $(ARFLAGS) $(LIBP)/liballpoker-backend.a $(CPPOBJS) $(CCOBJS) $(DPOBJS)
	$(AR) $(ARFLAGS) $(LIBP)/liballpoker-backend.a $(CPPOBJS) $(CCOBJS)  $(DPOBJS)
	$(RANLIB) $(LIBP)/liballpoker-backend.a
	@echo "Link backend end......"

backend-tests: $(TSTOBJS) $(TSTDPOBJS) $(DPLIBS) | $(BINP)
	@echo "Link backend-tests: begin......"
	#$(CXX) $(CC_FLAG) -o $(BINP)/$@ $^ $(LIB_SERVER) $(LIB) $(LIB_BASE) $(LIB_REDIS) $(LIB_MYSQL) $(LIB_BOOST) $(LIB_CRYP) $(LIB_JSON) $(LIB_TEST)
	$(CC) $(CC_FLAG) -o $(BINP)/$@ $^  $(LIB) $(LIB_TST) $(LIB_EXT) -lzmq -lprotobuf -lmysqlcppconn -ljson_linux-gcc-4.x_libmt
	@echo "Link backend-tests end......"

test: backend-tests | $(BINP)
	@echo "runnning backend-tests......"
	#RAILS_ENV=test $(BINP)/backend-tests

clean:
	@echo "Removing linked and compiled files......"
	rm -f $(CPPOBJS)
	rm -f $(CCOBJS)
	rm -f $(TSTOBJS)
	rm -f $(BINP)/*

.phony: clean

$(sort $(MKDIRS)):
	mkdir -p $@
