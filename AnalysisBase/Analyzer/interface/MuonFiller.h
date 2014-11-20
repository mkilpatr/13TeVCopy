//--------------------------------------------------------------------------------------------------
// 
// MuonFiller
// 
// Class to fill muon information in a TTree.
// 
// MuonFiller.h created on Fri Oct 17 12:11:09 CEST 2014 
// 
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISBASE_ANALYZER_MUONFILLER_H
#define ANALYSISBASE_ANALYZER_MUONFILLER_H

#include "DataFormats/PatCandidates/interface/Muon.h"

#include "AnalysisBase/Analyzer/interface/BaseFiller.h"
#include "AnalysisBase/Analyzer/interface/EventInfoFiller.h"
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"

namespace ucsbsusy {

  class MuonFiller : public BaseFiller {

    public :
      MuonFiller(const int options,
                 const string branchName,
                 const EventInfoFiller * evtInfoFiller,
                 const edm::InputTag muonTag,
                 const bool requireLoose,
                 const double muptMin);
      ~MuonFiller() {}

      enum Options {
                    NULLOPT     = 0,
                    LOADGEN     = (1 << 0),
                    FILLIDVARS  = (1 << 1), // fill ID variables
                    FILLISOVARS = (1 << 2)  // fill isolation variables
                   };
      static const int defaultOptions = NULLOPT;

      void load(const edm::Event& iEvent);
      void fill();

    private :
      const EventInfoFiller * evtInfoFiller_;
      // Input from the config file
      const edm::InputTag muonTag_;
      const bool          requireLoose_;    // only store muons which pass "Loose" ID
      const double        muptMin_;

      // Members to hold indices of tree data
      size ipt_;
      size ieta_;
      size iphi_;
      size imass_;
      size iq_;
      size id0_;
      size idz_;
      size ipfdbetaiso_;
      size iisloose_;
      size iistight_;
      size iispf_;
      size iisglobal_;
      size iistracker_;
      size iisstandalone_;
      size inChi2_;
      size inValidHits_;
      size inMatch_;
      size inPixHits_;
      size inTrackLayers_;
      size itrackiso_;
      size iecaliso_;
      size ihcaliso_;
      size ipfchargediso_;
      size ipfneutraliso_;
      size ipfphotoniso_;
      size ipfpuiso_;

    public :
      // Data members
      edm::Handle<pat::MuonCollection>  muons_;

  };

}

#endif
