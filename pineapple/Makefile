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

all: pineapple pineapple-tests 

CPPSRCS := $(wildcard $(SRCP)/*.cpp)
CPPOBJS := $(patsubst $(SRCP)/%.cpp,$(OBJP)/%.o,$(CPPSRCS)) 
CCSRCS := $(wildcard $(SRCP)/*.cc)
CCOBJS := $(patsubst $(SRCP)/%.cc,$(OBJP)/%.o,$(CCSRCS))
TSTSRCS := $(wildcard $(TSTP)/*.cpp)
TSTOBJS := $(patsubst $(TSTP)/%.cpp,$(OBJP)/%.o,$(TSTSRCS)) 
LIBPOKER= -L$(LIBP) -lpineapple -L../holdem/lib -lholdem -L../poker/lib -lpoker 
POKERTGT=../poker/lib/libpoker.a
HOLDEMTGT=../holdem/lib/libholdem.a

objs: $(CPPOBJS) $(CCOBJS) | $(OBJP)
	@echo "make pineapple depend objs"
	-for d in $(DPDIRS); do (cd $$d; make objs);done  
	cd $(CURP)
	#lib
	make pineapple

$(CPPOBJS): $(OBJP)/%.o:$(SRCP)/%.cpp | $(OBJP)
	$(CC) $(CC_FLAG) -c $< -o $@

$(CCOBJS): $(OBJP)/%.o:$(SRCP)/%.cc | $(OBJP)
	$(CC) $(CC_FLAG) -c $< -o $@
       	
$(TSTOBJS): $(OBJP)/%.o:$(TSTP)/%.cpp | $(OBJP)
	$(CC) $(CC_FLAG) -c $< -o $@

$(DPOBJS): 
	cd $(COMMON); make

pineapple: $(CPPOBJS) $(CCOBJS) $(DPOBJS) | $(LIBP)
	$(ECHO) $(AR) $(ARFLAGS) $(LIBP)/libpineapple.a $(CPPOBJS) $(CCOBJS)
	$(AR) $(ARFLAGS) $(LIBP)/libpineapple.a $(CPPOBJS) $(CCOBJS) 
	$(RANLIB) $(LIBP)/libpineapple.a

$(POKERTGT): 
	cd ../poker; make

pineapple-tests: $(TSTOBJS) $(DPOBJS) pineapple $(HOLDEMTGT) $(POKERTGT) | $(BIN_PATH)
	@echo "Link pineapple-test begin......"
	$(CC) $(CC_FLAG) -o $(BIN_PATH)/$@ $(TSTOBJS) $(DPOBJS)  $(LIBPOKER) $(LIB) $(LIB_TST) $(LIB_EXT) -lpthread
	@echo "Link pineapple-tests end......"

test: pineapple-tests | $(BIN_PATH)
	@echo "runnning test......"
	RAILS_ENV=test $(BIN_PATH)/pineapple-tests 

clean:
	@echo "Removing pineapple linked and compiled files......"
	rm -f $(CPPOBJS)
	rm -f $(CCOBJS)
	rm -f $(TSTOBJS)
	rm -f $(LIBP)/libpineapple.a

rebuild:
	@echo "run protoc to compile protobuf......"
	make clean all

$(sort $(MKDIRS)) :
	mkdir -p $@
