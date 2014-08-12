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
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

namespace ucsbsusy {
  class BaseAnalyzer : public edm::EDAnalyzer {
    public:
      BaseAnalyzer(const edm::ParameterSet&);
      virtual ~BaseAnalyzer() {}

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


    private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

  };
}

#endif
