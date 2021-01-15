CXX         = g++
RM          = rm -f

#ROOT
ROOTFLAGS   = $(shell root-config --cflags)
ROOTLIBS    = $(shell root-config --libs)
ROOTGLIBS   = $(shell root-config --glibs)

#configuration
dst0_dir    = ../E16DAQ/E16DST
src_dir     = src
header_dir  = include
bin_dir     = bin
user_dir    = usr

SRCS        = $(wildcard $(src_dir)/*.cc $(dst0_dir)/*.cc)
HEADS       = $(wildcard $(header_dir)/*.hh $(dst0_dir)/*.hh)
objs        = $(SRCS:%.cc=%.o)

CXXFLAGS    = -O3 -g -Wall -pg
CXXFLAGS   += $(addprefix -I,$(header_dir))
#CXXFLAGS   += -L/usr/lib/gef -I/usr/include 
#CXXFLAGS   += -I/usr/include 
CXXFLAGS   += $(ROOTFLAGS)
CXXFLAGS   += -I/opt/FairSoft/jun19p2/include/root6
#CXXFLAGS   += -I/e16/u/ashikaga/work/E16DAQ/E16DAQ20200521
#CXXFLAGS   += -I/e16/u/ashikaga/work/E16DAQ/daq
CXXFLAGS   += -I../E16DAQ
CXXFLAGS    += -L../install/lib64
#CXXFLAGS    += -L/e16/u/ashikaga/work/E16DAQ/install/lib64
#CXXFLAGS    += -L/e16/u/ashikaga/work/E16DAQ/install20200529/lib64

#CXXLIBS     = -lpthread
CXXLIBS    += $(ROOTGLIBS)
CXXLIBS    += $(ROOTLIBS)
CXXFLAGS   += -I/opt/FairSoft/jun19p2/lib/root
CXXLIBS    += -lMinuit -lz
#CXXLIBS    += -L/e16/u/ashikaga/work/E16DAQ/install/lib64
CXXLIBS    += -L../install/lib64
#CXXLIBS    += -L/e16/u/ashikaga/work/E16DAQ/install20200529/lib64
CXXLIBS	   += -I../E16DAQ
#CXXLIBS	   += -I/e16/u/ashikaga/work/E16DAQ/daq
#CXXLIBS	   += -I/e16/u/ashikaga/work/E16DAQ/DAQ20200529

TSRCS = $(wildcard $(user_dir)/*.cc)
TARGETS = $(TSRCS:$(user_dir)/%.cc=%)

.PHONY: all clean distclean

all: $(TARGETS)

$(TARGETS): $(objs)
	@echo Linking $@
	@set -e;\
	$(CXX) $(CXXFLAGS) -o $(bin_dir)/$@ $(user_dir)/$@.cc $^ $(CXXLIBS)
	@if [ \(-f $(bin_dir)/$@ \) ]; then \
	echo ... Done!; fi
.cc.o:
	@echo Compiling $@ ...
	@set -e;\
	$(CXX) $(CXXFLAGS) -o $@ -MMD -c $<

-include $(SRCS:%.cc=%.d)

clean:
	@echo Cleaning up ...
	@$(RM) $(src_dir)/*.o $(src_dir)/*.d

distclean:
	@echo Full Cleaning up ...
	@$(RM) $(src_dir)/*.o $(src_dir)/*.d $(bin_dir)/$(TARGETS)
