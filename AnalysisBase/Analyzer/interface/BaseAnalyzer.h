//--------------------------------------------------------------------------------------------------
// 
// BaseAnalyzer
// 
// Base class to define interface for analyzer classes. Implementation of a CMS EDAnalyzer.
// 
// BaseAnalyzer.h created on Fri Aug 8 15:50:43 CEST 2014 
// 
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISBASE_ANALYZER_BASEANALYZER_H
#define ANALYSISBASE_ANALYZER_BASEANALYZER_H

#include <memory>
#include <TRandom3.h>
#include <TString.h>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "AnalysisTools/Parang/interface/Plotter.h"

namespace ucsbsusy {
  class BaseAnalyzer : public edm::EDAnalyzer {
    public:
      BaseAnalyzer(const edm::ParameterSet&);
      virtual ~BaseAnalyzer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

    private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;


      //_____________________________________________________________________________
      //     Functions to return data members
      //_____________________________________________________________________________
    public:
      int numProcessed() const { return numProcessed_ ; }   ///< Total number of events processed in this job -- incremented after load/filter/analyze/produce for every event
      int numAnalyzed () const { return numAnalyzed_  ; }   ///< Total number of events analyzed -- incremented after load/filter/analyze/produce if all of them returned true
      //_____________________________________________________________________________
      edm::RunNumber_t              runNumber     () const        { return runNumber_  ;    }
      edm::LuminosityBlockNumber_t  lumiBlock     () const        { return lumiBlock_  ;    }
      edm::EventNumber_t            eventNumber   () const        { return eventNumber_;    }
      void printEventCoordinates(std::ostream& out = std::cout) const;
      TString eventCoordinates() const  { return TString::Format("%d:%d:%d", runNumber(), lumiBlock(), eventNumber()); }


      //_____________________________________________________________________________
      //     Basic members to run jobs
      //_____________________________________________________________________________
    public:
        mutable PlotterD              plotter;              ///< Object that stores histograms
        TString                       outputPath;           ///< Output file that is produced by plotter
        const int                     outputInterval;       ///< Frequency (in terms of number of events) in which to output the currently filled plots
        mutable TRandom*              randomGenerator;      ///< TRandom3 initialized with the given seed (configurable)

        //_____________________________________________________________________________
        //     Event and procesing information
        //_____________________________________________________________________________
    private:
        int                           numProcessed_ ;       ///< Total number of events processed in this job -- incremented after load/filter/analyze/produce for every event
        int                           numAnalyzed_  ;       ///< Total number of events analyzed -- incremented after load/filter/analyze/produce if all of them returned true
        edm::RunNumber_t              runNumber_;
        edm::LuminosityBlockNumber_t  lumiBlock_;
        edm::EventNumber_t            eventNumber_;

  };
}

#endif
