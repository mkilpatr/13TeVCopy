//--------------------------------------------------------------------------------------------------
// 
// MuonFiller
// 
// Class to fill muon information in a TTree.
// 
// MuonFiller.h created on Fri Oct 17 12:11:09 CEST 2014 
// 
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISBASE_ANALYZER_FATJETJTBFILLER_H
#define ANALYSISBASE_ANALYZER_FATJETJTBFILLER_H

#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/BTauReco/interface/CATopJetTagInfo.h"
#include "AnalysisBase/Analyzer/interface/BaseFiller.h"
#include "TLorentzVector.h"
#include <boost/algorithm/string.hpp>
#include <string>


namespace ucsbsusy {

  class FatJetjtbFiller : public BaseFiller {

  public :
    FatJetjtbFiller(const edm::ParameterSet& cfg, edm::ConsumesCollector && cc, const int options, const string branchName);
    ~FatJetjtbFiller() {}

    void load(const edm::Event& iEvent, const edm::EventSetup &iSetup);
    void fill();

  private :
    edm::EDGetTokenT<pat::JetCollection> fatJetjtbToken_;
    edm::EDGetTokenT<pat::JetCollection> fatJetSDjtbToken_;
    edm::EDGetTokenT<pat::JetCollection> fatJetSDSubjtbToken_;
    std::string puRemoval_;
    std::string details_;

    // helper functions
    unsigned int matchedjet(edm::Handle<std::vector<pat::Jet> >& rawjets, const pat::Jet sdjet);
    float drmatch(pat::Jet rawjet, pat::Jet sdjet);

    // Members to hold indices of tree data
    size ifj_mass_;
    size ifj_pt_;
    size ifj_eta_;
    size ifj_phi_;
    size ifj_csv_;
    size ifj_tau1_;
    size ifj_tau2_;
    size ifj_tau3_;
    size ifj_nsdsubjets_;
    size ifj_sdsub1_mass_;
    size ifj_sdsub1_pt_;
    size ifj_sdsub1_eta_;
    size ifj_sdsub1_phi_;
    size ifj_sdsub1_csv_;
    size ifj_sdsub2_mass_;
    size ifj_sdsub2_pt_;
    size ifj_sdsub2_eta_;
    size ifj_sdsub2_phi_;
    size ifj_sdsub2_csv_;


  public :
    // Data members
    edm::Handle<pat::JetCollection> fatJetsjtb_;
    edm::Handle<pat::JetCollection> fatJetsSDjtb_;
    edm::Handle<pat::JetCollection> fatJetsSDSubjtb_;
   
  };

}

#endif
