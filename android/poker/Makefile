#started with '#' are comments
include ../Makefile.inc

PROTOP=proto
SRCP=src
OBJP=obj
MKDIRS += $(OBJP)
LIBP=lib
MKDIRS += $(LIBP)
BINP=bin
MKDIRS += $(BINP)

CURP=$(CURDIR)

#src
PROTOSRCS := $(wildcard $(PROTOP)/*.proto)
PROTOOBJS := $(patsubst $(PROTOP)/%.proto,$(SRCP)/%.pb.cc,$(PROTOSRCS))
CPPSRCS := $(wildcard $(SRCP)/*.cpp)
CPPOBJS := $(patsubst $(SRCP)/%.cpp,$(OBJP)/%.o,$(CPPSRCS)) 
CCSRCS := $(wildcard $(SRCP)/*.cc)
CCOBJS := $(patsubst $(SRCP)/%.cc,$(OBJP)/%.o,$(CCSRCS))

DEPFILES := $(patsubst %.cpp,$(OBJP)/%.d,$(CPPSRCS))
-include $(DEPFILES)

#src depend dirs
DPDIRS += $(COMMONP)

all: $(PROTOOBJS) poker

$(PROTOOBJS): $(SRCP)/%.pb.cc:$(PROTOP)/%.proto
	@echo "compile proto begin......"
	protoc -I./proto --cpp_out=./src $<
	@echo "compile proto end......"

objs: $(CPPOBJS) $(CCOBJS) $(TSTOBJS)
	@echo "make handler depend objs"
	-for d in $(DPDIRS); do (cd $$d; $(MAKE) objs || exit "$$?");done
	cd $(CURP)
        
$(CPPOBJS): $(OBJP)/%.o:$(SRCP)/%.cpp | $(OBJP)
	$(CXX) $(CXXFLAGS) -c $< -MMD -o $@

$(CCOBJS): $(OBJP)/%.o:$(SRCP)/%.cc | $(OBJP)
	$(CXX) $(CXXFLAGS) -c $< -MMD -o $@

$(DPOBJS): $(DPSRCS)
	cd $(COMMONP); make

poker: $(CCOBJS) $(CPPOBJS) $(DPOBJS) | $(LIBP)

clean:
	@echo "Removing linked and compiled files......"
	rm -f $(CPPOBJS)
	rm -f $(CCOBJS)
	rm -f $(TSTOBJS)

.phony: clean

$(sort $(MKDIRS)) :
	mkdir -p $@
