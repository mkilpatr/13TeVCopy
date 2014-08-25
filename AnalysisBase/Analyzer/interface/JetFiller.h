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

#include "AnalysisBase/Analyzer/interface/BaseFiller.h"
#include "AnalysisTools/DataFormats/interface/Jet.h"

namespace ucsbsusy {
  class JetFiller : public BaseFiller {
    public:
      JetFiller(const edm::ParameterSet &cfg);
      ~JetFiller();

      void load(edm::Event& iEvent, bool storeOnlyPtr = false);
      void fill(Planter& plant, int& bookMark);
      JetCollection& operator->(){return *obj;}

    private:
      //--------------------------------------------------------------------------------------------------
      // Input from the config file
      //--------------------------------------------------------------------------------------------------
      edm::InputTag jetTag_;
      double jptMin_;
      unsigned int maxNjets_;

      //--------------------------------------------------------------------------------------------------
      // Data Members
      //--------------------------------------------------------------------------------------------------
      JetCollection            * obj;
      const pat::JetCollection * jets_;

  };
}

#endif
