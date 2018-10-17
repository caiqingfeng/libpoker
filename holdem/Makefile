#started with '#' are comments

include ../Makefile.inc

SRCP=src
MKDIRS += $(SRCP)
OBJP=obj
MKDIRS += $(OBJP)
LIBP=lib
MKDIRS += $(LIBP)
BIN_PATH=bin
MKDIRS += $(BIN_PATH)
TSTP=test

CURP=$(CURDIR)
#depend dirs
#depend dirs
COMMON=../common

#depend dirs
#DPDIRS += $(COMMON)
#depend objs
#DPOBJS=$(COMMON)/obj/holdem.pb.o $(COMMON)/obj/my_log.o

all: holdem

CPPSRCS := $(wildcard $(SRCP)/*.cpp)
CPPOBJS := $(patsubst $(SRCP)/%.cpp,$(OBJP)/%.o,$(CPPSRCS)) 
CCSRCS := $(wildcard $(SRCP)/*.cc)
CCOBJS := $(patsubst $(SRCP)/%.cc,$(OBJP)/%.o,$(CCSRCS))
TSTSRCS := $(wildcard $(TSTP)/*.cpp)
TSTOBJS := $(patsubst $(TSTP)/%.cpp,$(OBJP)/%.o,$(TSTSRCS)) 
LIBPOKER= -L$(LIBP) -lholdem -L../poker/lib -lpoker 
POKERTGT=../poker/lib/libpoker.a

objs: $(CPPOBJS) $(CCOBJS) | $(OBJP)
	@echo "make holdem depend objs"
	-for d in $(DPDIRS); do (cd $$d; make objs);done  
	cd $(CURP)
	#lib
	make holdem

$(CPPOBJS): $(OBJP)/%.o:$(SRCP)/%.cpp | $(OBJP)
	$(CC) $(CC_FLAG) -c $< -o $@

$(CCOBJS): $(OBJP)/%.o:$(SRCP)/%.cc | $(OBJP)
	$(CC) $(CC_FLAG) -c $< -o $@
       	
$(TSTOBJS): $(OBJP)/%.o:$(TSTP)/%.cpp | $(OBJP)
	$(CC) $(CC_FLAG) -c $< -o $@

$(DPOBJS): 
	cd $(COMMON); make

holdem: $(CPPOBJS) $(CCOBJS) $(DPOBJS) | $(LIBP)
	$(ECHO) $(AR) $(ARFLAGS) $(LIBP)/libholdem.a $(CPPOBJS) $(CCOBJS)
	$(AR) $(ARFLAGS) $(LIBP)/libholdem.a $(CPPOBJS) $(CCOBJS) 
	$(RANLIB) $(LIBP)/libholdem.a

$(POKERTGT): 
	cd ../poker; make

holdem-tests: $(TSTOBJS) $(DPOBJS) holdem $(POKERTGT) | $(BIN_PATH)
	@echo "Link holdem-test begin......"
	$(CC) $(CC_FLAG) -o $(BIN_PATH)/$@ $(TSTOBJS) $(DPOBJS)  $(LIBPOKER) $(LIB) $(LIB_TST) $(LIB_EXT) -lpthread
	@echo "Link holdem-tests end......"

test: holdem-tests | $(BIN_PATH)
	@echo "runnning test......"
	RAILS_ENV=test $(BIN_PATH)/holdem-tests 

clean:
	@echo "Removing holdem linked and compiled files......"
	rm -f $(CPPOBJS)
	rm -f $(CCOBJS)
	rm -f $(TSTOBJS)
	rm -f $(LIBP)/libholdem.a

rebuild:
	@echo "run protoc to compile protobuf......"
	make clean all

$(sort $(MKDIRS)) :
	mkdir -p $@
