# =============================== 
# Makefile for QuickRefold 
# =============================== 

LIBDIR        = lib
SRCDIR        = src
INTDIR        = interface
CINTDIR       = cint

MKARCH := $(wildcard $(shell root-config --etcdir)/Makefile.arch)
include $(ROOTSYS)/etc/Makefile.arch

CXXFLAGS += -I../.. -I.

ifeq ($(PLATFORM),macosx)
CXXFLAGS += -std=c++11
endif

OBJS = $(LIBDIR)/Plot.o $(LIBDIR)/PlotROCCurves.o $(LIBDIR)/PlotStuff.o $(LIBDIR)/StyleTools.o $(LIBDIR)/PlotUtilsDict.o $(LIBDIR)/EffPlotTools.o $(LIBDIR)/PlotTools.o $(LIBDIR)/SFGetter.o $(LIBDIR)/HistogramGetter.o
# INTS = $(INTDIR)/Plot.hh $(INTDIR)/PlotROCCurves.h $(INTDIR)/PlotStuff.h $(INTDIR)/StyleTools.hh
INTS = $(INTDIR)/Plot.hh $(INTDIR)/PlotROCCurves.h  $(INTDIR)/StyleTools.hh $(INTDIR)/EffPlotTools.hh $(INTDIR)/PlotStuff.h $(INTDIR)/PlotTools.hh $(INTDIR)/SFGetter.hh $(INTDIR)/Sample.hh $(INTDIR)/HistogramGetter.hh

libPlotUtils.so: $(OBJS)
ifeq ($(PLATFORM),macosx)
	$(LD) $(SOFLAGS)$@ $(LDFLAGS) $^ $(OutPutOpt) $@ $(EXPLLINKLIBS) -lTreePlayer
else
	$(LD) $(SOFLAGS) $(LDFLAGS) $^ $(OutPutOpt) $@ $(EXPLLINKLIBS) -lTreePlayer
endif

$(CINTDIR)/PlotUtilsDict.cc: $(INTS) $(SRCDIR)/PlotUtils_LinkDef.h
	@echo "Generating dictionary $@..."
	rootcint -v -l -f $@ -c -I../.. $^ 
$(LIBDIR)/PlotUtilsDict.o: $(CINTDIR)/PlotUtilsDict.cc
	$(CXX)  $(CXXFLAGS) -c $<  -o $@	

$(LIBDIR)/%.o: $(SRCDIR)/%.cc
	$(CXX) $(CXXFLAGS) -c $<  -o $@

	



	
# =============================== 
clean: 
	rm -f $(LIBDIR)/* $(CINTDIR)/* libPlotUtils.so
# =============================== # DO NOT DELETE
# DO NOT DELETE
