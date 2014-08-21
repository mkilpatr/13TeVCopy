//--------------------------------------------------------------------------------------------------
// 
// JetFiller
// 
// Class to fill some basic jet information for testing. To be developed as needed.
// 
// JetFiller.h created on Thu Aug 14 11:10:55 CEST 2014 
// 
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISBASE_ANALYZER_JETFILLER_H
#define ANALYSISBASE_ANALYZER_JETFILLER_H

#include "DataFormats/PatCandidates/interface/Jet.h"
#include "AnalysisTools/DataFormats/interface/Jet.h"
#include "AnalysisBase/Analyzer/interface/BaseAnalyzer.h"


namespace ucsbsusy {
  class JetFiller : public BaseAnalyzer {
    public:
      JetFiller(const edm::ParameterSet &cfg);
      ~JetFiller();

      void book(TTree &t);
      void fill(const edm::Event &e);

    private:
      double jptMin_;
      unsigned int maxNjets_;
      edm::InputTag jetTag_;
      JetCollection *jetCol_;

  };
}

#endif
