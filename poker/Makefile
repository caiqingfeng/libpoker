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

all: poker poker-tests

CPPSRCS := $(wildcard $(SRCP)/*.cpp)
CPPOBJS := $(patsubst $(SRCP)/%.cpp,$(OBJP)/%.o,$(CPPSRCS)) 
CCSRCS := $(wildcard $(SRCP)/*.cc)
CCOBJS := $(patsubst $(SRCP)/%.cc,$(OBJP)/%.o,$(CCSRCS))
TSTSRCS := $(wildcard $(TSTP)/*.cpp)
TSTOBJS := $(patsubst $(TSTP)/%.cpp,$(OBJP)/%.o,$(TSTSRCS)) 
LIBPOKER := -L$(LIBP) -lpoker

objs: $(CPPOBJS) $(CCOBJS)
	@echo "make poker depend objs"
	-for d in $(DPDIRS); do (cd $$d; make objs);done  
	cd $(CURP)
	#lib
	make poker
	
$(CPPOBJS): $(OBJP)/%.o:$(SRCP)/%.cpp | $(OBJP)
	$(CC) $(CC_FLAG) -c $< -o $@

$(CCOBJS): $(OBJP)/%.o:$(SRCP)/%.cc | $(OBJP)
	$(CC) $(CC_FLAG) -c $< -o $@
       	
$(TSTOBJS): $(OBJP)/%.o:$(TSTP)/%.cpp | $(OBJP)
	$(CC) $(CC_FLAG) -c $< -o $@

$(DPOBJS): 
	cd $(COMMON); make

poker: $(CPPOBJS) $(CCOBJS) $(DPOBJS) | $(LIBP)
	$(ECHO) $(AR) $(ARFLAGS) $(LIBP)/libpoker.a $(CPPOBJS) $(CCOBJS)
	$(AR) $(ARFLAGS) $(LIBP)/libpoker.a $(CPPOBJS) $(CCOBJS) 
	$(RANLIB) $(LIBP)/libpoker.a

poker-tests: $(TSTOBJS) $(DPOBJS) poker | $(BIN_PATH)
	@echo "Link poker-tests begin......"
	$(CC) $(CC_FLAG) -o $(BIN_PATH)/$@ $(TSTOBJS) $(DPOBJS) $(LIBPOKER) $(LIB) $(LIB_TST) $(LIB_EXT) -lpthread
	@echo "Link poker-tests end......"

test: poker-tests | $(BIN_PATH)
	@echo "executing poker-tests"
	RAILS_ENV=test $(BIN_PATH)/poker-tests 

clean:
	@echo "Removing poker linked and compiled files......"
	rm -f $(CPPOBJS)
	rm -f $(CCOBJS)
	rm -f $(TSTOBJS)
	rm -f $(LIBP)/libpoker.a

rebuild:
	@echo "run protoc to compile protobuf......"
	make clean all

$(sort $(MKDIRS)) :
	mkdir -p $@
