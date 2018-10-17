#started with '#' are comments
include ../Makefile.inc

PROTOP=proto
SRCP=src
OBJP=obj
MKDIRS += $(OBJP)
LIBP=lib
MKDIRS += $(LIBP)

PROTOSRCS := $(wildcard $(PROTOP)/*.proto)
PROTOOBJS := $(patsubst $(PROTOP)/%.proto,$(SRCP)/%.pb.cc,$(PROTOSRCS))
CPPSRCS := $(wildcard $(SRCP)/*.cpp)
CPPOBJS := $(patsubst $(SRCP)/%.cpp,$(OBJP)/%.o,$(CPPSRCS)) 
CCSRCS := $(wildcard $(SRCP)/*.cc)
CCOBJS := $(patsubst $(SRCP)/%.cc,$(OBJP)/%.o,$(CCSRCS))

all: $(PROTOOBJS) result 

$(PROTOOBJS): $(SRCP)/%.pb.cc:$(PROTOP)/%.proto
	@echo "compile proto begin......"
	../protobuf/bin/protoc -I./proto --cpp_out=./src $<
	@echo "compile proto end......"

$(CPPOBJS): $(OBJP)/%.o:$(SRCP)/%.cpp | $(OBJP)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(CCOBJS): $(OBJP)/%.o:$(SRCP)/%.cc | $(OBJP)
	$(CXX) $(CXXFLAGS) -c $< -o $@
       	
objs: $(CPPOBJS) $(CCOBJS) 
	@echo "holdem"
	
result: objs
	@echo "holdem"

clean:
	@echo "Removing linked and compiled files......"
	rm -f $(CPPOBJS)
	rm -f $(CCOBJS)

$(sort $(MKDIRS)) :
	mkdir -p $@
