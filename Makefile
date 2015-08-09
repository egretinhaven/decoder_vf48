# Makefile
CC = c99

ROOTFLAGS   =  $(shell root-config --cflags)
ROOTLIBPATH =  $(shell root-config --libdir)
#CFLAGS   =  -DOS_LINUX -m64 -mcmodel=medium -fPIC -g -Wall -I. $(ROOTFLAGS)
CFLAGS   =  -DOS_LINUX -m64 -fPIC -g -Wall -I. $(ROOTFLAGS)
CXXFLAGS = -std=c++0x $(CFLAGS)


#LIBS = -lm -lz -lutil -lnsl -lpthread
LIBS= $(shell root-config --libs)

all:: raw2hdw_vf48

raw2hdw_vf48::raw2hdw_vf48.o vf48_unpack.o vf48_tree_hdw.o dict.o hdw2det_map.o
	$(CXX) -o $@ $(CXXFLAGS) $^ $(LIBS) -lrt -lutil
raw2hdw_vf48.o::raw2hdw_vf48.cxx raw2hdw_vf48.h
	$(CXX) $(OSFLAGS) $(CXXFLAGS) -c $<
hdw2det_map.o::hdw2det_map.cxx hdw2det_map.h
	$(CXX) $(OSFLAGS) $(CXXFLAGS) -c $<
vf48_unpack.o::vf48_unpack.cxx vf48_unpack.h
	$(CXX) $(OSFLAGS) $(CXXFLAGS) -c $<
vf48_tree_hdw.o::vf48_tree_hdw.cxx vf48_tree_hdw.h
	$(CXX) $(OSFLAGS) $(CXXFLAGS) -c $<
dict.o::dict.cc
	$(CXX) $(OSFLAGS) $(CXXFLAGS) -c $<
dict.cc::vf48TreeLinkDef.h
	LD_LIBRARY_PATH=$(ROOTLIBPATH) rootcint dict.cc -c -p $<
clean::
	-rm -f *.o

clean::
	-rm -f *.exe
	-rm -f raw2hdw_vf48
clean::
	-rm -f dict.h
	-rm -f dict.cc
# end
