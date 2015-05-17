# Setup

# End Setup

BINS          = NNRunner
MYLIB         = libNNRunner

SOURCE_C      = NeuralNetReader.cc \
				NNRunner.cc \
				HptEventOld.cc \
				IncEvent.cc \
				NNRunnerDict.cc

SOURCE_BIN    = Main.cc

OBJS_C        = $(SOURCE_C:.cc=.o)
OBJS_BIN      = $(SOURCE_BIN:.cc=.o)
DEPS          = $(OBJS_BIN:%.o=%.P) $(OBJS_C:%.o=%.P)
DICT_HDRS     = HptEventOld.h \
				IncEvent.h

CXX           = g++
LD            = g++
CXXFLAGS      = -pthread -std=c++11 -Wno-deprecated-declarations -m64 -I/afs/cern.ch/sw/lcg/app/releases/ROOT/5.34.10/x86_64-slc6-gcc47-opt/root/include -I/afs/cern.ch/sw/lcg/external/Boost/1.53.0_python2.7/x86_64-slc6-gcc47-opt/include/boost-1_53 -I/afs/cern.ch/user/c/conrad/usr/include   \
				-fPIC
LDFLAGS       = -m64 -L/afs/cern.ch/sw/lcg/external/Boost/1.53.0_python2.7/x86_64-slc6-gcc47-opt/lib \
				-fPIC
LIBS          = -L/afs/cern.ch/sw/lcg/app/releases/ROOT/5.34.10/x86_64-slc6-gcc47-opt/root/lib -lGui -lCore -lCint -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lTree -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -pthread -lm -ldl -rdynamic -L/afs/cern.ch/user/c/conrad/usr/lib -lmuparser   -lXMLParser -lboost_filesystem-gcc47-mt-1_53 \
				-lboost_system-gcc47-mt-1_53 \
				-lboost_regex-gcc47-mt-1_53

CXX          += -MMD

MAKEDEPEND = cp $*.d $*.P; sed -e 's/\#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' -e '/^$$/ d' -e 's/$$/ :/' < $*.d >> $*.P; rm -f $*.d 

all: $(BINS)
	@echo \*\* Finished NNRunner

NNRunner: $(MYLIB).so $(OBJS_BIN)
	@echo \*\* Compilation: $@
	@$(LD) $(LDFLAGS) $^ $(LIBS) -o $@

$(MYLIB).so : $(OBJS_C)
	@echo \*\* Creating Library: $@
	@$(LD) -shared $(LDFLAGS) $^ $(LIBS) -o $@

%.o : %.cc
	@echo \*\* Compilation: $@
	@$(CXX) $(CXXFLAGS) -c $<
	@$(MAKEDEPEND)

NNRunnerDict.cc NNRunnerDict.h : $(DICT_HDRS)
	@echo \*\* Generating ROOT Dictionary...
	@$(ROOTSYS)/bin/rootcint -f NNRunnerDict.cc -c $(DICT_HDRS) LinkDef.h

clean :
	@echo \*\* Cleaning...
	@rm -f $(BINS) $(OBJS_C) $(OBJS_BIN) $(MYLIB).so NNRunnerDict.cc \
		NNRunnerDict.h $(DEPS)

distclean : clean
	@echo \*\* Distribution Cleaning...
	@rm -rf Makefile aclocal.m4 autom4te.cache config.status	setup.sh *.log

-include $(DEPS)

