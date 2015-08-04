//--------------------------------------------------------------------------------------------------
// 
// MuonFiller
// 
// Class to fill muon information in a TTree.
// 
// MuonFiller.h created on Fri Oct 17 12:11:09 CEST 2014 
// 
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISBASE_ANALYZER_FATJETFILLER_H
#define ANALYSISBASE_ANALYZER_FATJETFILLER_H

#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/BTauReco/interface/CATopJetTagInfo.h"
#include "AnalysisBase/Analyzer/interface/BaseFiller.h"

namespace ucsbsusy {

  class FatJetFiller : public BaseFiller {

  public :
    FatJetFiller(const edm::ParameterSet& cfg, edm::ConsumesCollector && cc, const int options, const string branchName);
    ~FatJetFiller() {}

    void load(const edm::Event& iEvent, const edm::EventSetup &iSetup);
    void fill();

  private :
    edm::EDGetTokenT<pat::JetCollection> fatJetToken_;

    // Members to hold indices of tree data
    size ifj_rawmass_;
    size ifj_trimmedmass_;
    size ifj_prunedmass_;
    size ifj_softdropmass_;
    size ifj_filteredmass_;
    size ifj_cmstoptagmass_;
    //size ifj_massdropfilteredmass_;
    size ifj_tau1_;
    size ifj_tau2_;
    size ifj_tau3_;
    size ifj_pt_;
    size ifj_eta_;
    size ifj_phi_;


  public :
    // Data members
    edm::Handle<pat::JetCollection> fatJets_;
  };

}

#endif
