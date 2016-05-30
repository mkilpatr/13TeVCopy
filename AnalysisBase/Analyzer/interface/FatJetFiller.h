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
#include "TLorentzVector.h"

namespace ucsbsusy {

  class FatJetFiller : public BaseFiller {

  public :
    FatJetFiller(const edm::ParameterSet& cfg, edm::ConsumesCollector && cc, const int options, const string branchName);
    ~FatJetFiller() {}

    void load(const edm::Event& iEvent, const edm::EventSetup &iSetup);
    void fill();

  private :
    edm::EDGetTokenT<pat::JetCollection> fatJetToken_;
    std::string puRemoval_;

    // Members to hold indices of tree data
    size ifj_rawmass_;
    size ifj_prunedmass_;
    size ifj_softdropmass_;
    //    size ifj_cmstoptagmass_;
    //    size ifj_cmstoptagminmass_;
    //    size ifj_cmstoptagnsubjets_;
    //size ifj_massdropfilteredmass_;
    size ifj_tau1_;
    size ifj_tau2_;
    size ifj_tau3_;
    size ifj_pt_;
    size ifj_eta_;
    size ifj_phi_;
    size ifj_looseId_;
    size ifj_tightId_;
    size ifj_muEnFrac_;
    size ifj_csv_;
    size ifj_cvsl_;
    size ifj_cvsb_;
    size ifj_nsoftdropsubjets_;
    size ifj_sdsubjet1_pt_;
    size ifj_sdsubjet1_eta_;
    size ifj_sdsubjet1_phi_;
    size ifj_sdsubjet1_mass_;
    size ifj_sdsubjet1_csv_;
    size ifj_sdsubjet2_pt_;
    size ifj_sdsubjet2_eta_;
    size ifj_sdsubjet2_phi_;
    size ifj_sdsubjet2_mass_;
    size ifj_sdsubjet2_csv_;

    size ifj_puppi_pt_;
    size ifj_puppi_eta_;
    size ifj_puppi_phi_;
    size ifj_puppi_mass_;
    size ifj_puppi_tau1_;
    size ifj_puppi_tau2_;
    size ifj_puppi_tau3_;
    size ifj_puppi_csv_;
    size ifj_puppi_cvsl_;
    size ifj_puppi_cvsb_;
    size ifj_puppi_nsoftdropsubjets_;
    size ifj_puppi_sdsubjet1_pt_;
    size ifj_puppi_sdsubjet1_eta_;
    size ifj_puppi_sdsubjet1_phi_;
    size ifj_puppi_sdsubjet1_mass_;
    size ifj_puppi_sdsubjet1_csv_;
    size ifj_puppi_sdsubjet2_pt_;
    size ifj_puppi_sdsubjet2_eta_;
    size ifj_puppi_sdsubjet2_phi_;
    size ifj_puppi_sdsubjet2_mass_;
    size ifj_puppi_sdsubjet2_csv_;
    size ifj_puppi_sdmass_;

  public :
    // Data members
    edm::Handle<pat::JetCollection> fatJets_;
   
  };

}

#endif
