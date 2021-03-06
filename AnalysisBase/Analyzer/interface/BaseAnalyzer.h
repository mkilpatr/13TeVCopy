//--------------------------------------------------------------------------------------------------
// 
// BaseAnalyzer
// 
// Base class to define interface for analyzer classes. Implementation of a CMS EDFilter.
// 
// BaseAnalyzer.h created on Fri Aug 8 15:50:43 CEST 2014 
// 
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISBASE_ANALYZER_BASEANALYZER_H
#define ANALYSISBASE_ANALYZER_BASEANALYZER_H

#include <memory>
#include <TRandom3.h>
#include <TSystem.h>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "AnalysisTools/Utilities/interface/TreeWriter.h"

namespace ucsbsusy {

  class BaseAnalyzer : public edm::EDFilter {

    public:
      BaseAnalyzer(const edm::ParameterSet&);
      virtual ~BaseAnalyzer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

      virtual void beginJob() override;

      // Event processing

      /**
        Calls load() to load collections from the event. If that does not fail, calls
        analyze() for user code to analyze the event, and then the optional produce()
        [which is provided in case the user needs to output products].

        Increments numProcessed after all of the above.
        DO NOT OVERRIDE unless you know what you're doing.
      */
      virtual bool filter(edm::Event& iEvent, const edm::EventSetup& iSetup) override;

      /**
        Called to load products before filter(), analyze(), and produce(). Will not proceed
        to filter() etc. if false is returned, but still counts as numProcessed.
      */
      virtual bool load(const edm::Event& iEvent, const edm::EventSetup& iSetup) = 0;

      /// Called to process an event, after load(). Return value acts as a filter.
      virtual bool filter() = 0;
      /// Called to process an event, after analyze().
      virtual void analyze() = 0;
      /// Called to put EDM products into the event, after analyze(). Return value acts as a filter.
      virtual bool produce(edm::Event& iEvent, const edm::EventSetup& iSetup) = 0;

      // For convenience                                                          

      // Place in your analysis class declaration to define an analyze() that does nothing.
      #define DUMMY_ANALYZE  \
        virtual void analyze()						{ }

      // Place in your analysis class declaration to define a filter() that always returns true.
      #define DUMMY_FILTER   \
        virtual bool filter ()						{ return true;	}

      // Place in your analysis class declaration to define a produce() that does nothing.
      #define DUMMY_PRODUCE  \
        virtual bool produce(edm::Event&, const edm::EventSetup&)	{ return true;	}

      // Place in your analysis class declaration to only require analyze().
      #define ANALYZER_MODE  \
        DUMMY_FILTER         \
        DUMMY_PRODUCE

      // Place in your analysis class declaration to only require produce().
      #define PRODUCER_MODE  \
        DUMMY_FILTER         \
        DUMMY_ANALYZE

      // Place in your analysis class declaration to only require filter().
      #define FILTER_MODE    \
        DUMMY_ANALYZE        \
        DUMMY_PRODUCE

    public :
      // Functions to return data members
      int 				numProcessed()    const	{ return numProcessed_;	}   // Total number of events processed in this job -- incremented after load/filter/analyze/produce for every event
      int 				numAnalyzed ()	  const	{ return numAnalyzed_;	}   // Total number of events analyzed -- incremented after load/filter/analyze/produce if all of them returned true
      edm::RunNumber_t			runNumber()	  const	{ return runNumber_;	}
      edm::LuminosityBlockNumber_t	lumiBlock()	  const	{ return lumiBlock_;	}
      edm::EventNumber_t		eventNumber()	  const	{ return eventNumber_;	}
      void 				printEventCoordinates(std::ostream& out = std::cout) const;
      TString				eventCoordinates() const { return TString::Format("%d:%d:%llu", runNumber(), lumiBlock(), eventNumber());	}
      TreeWriter*			treeWriter()		{ return treeWriter_;	}

      // Basic members to run jobs
      TString				outputPath;           // Output file that is produced by plotter
      const int				outputInterval;       // Frequency (in terms of number of events) in which to output the currently filled plots
      mutable TRandom*			randomGenerator;      // TRandom3 initialized with the given seed (configurable)

    private :
      // Event and procesing information
      TTree*				eventTree_;
      TreeWriter*			treeWriter_;
      int				numProcessed_;        // Total number of events processed in this job -- incremented after load/filter/analyze/produce for every event
      int				numAnalyzed_;         // Total number of events analyzed -- incremented after load/filter/analyze/produce if all of them returned true
      edm::RunNumber_t			runNumber_;
      edm::LuminosityBlockNumber_t	lumiBlock_;
      edm::EventNumber_t		eventNumber_;

  };

}

#endif
