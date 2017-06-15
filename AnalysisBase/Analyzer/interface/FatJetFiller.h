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

class QuarkGluonTaggingVariables;
class SoftdropTopMVA;
class SoftdropWTagMVA;

  class FatJetFiller : public BaseFiller {

  public :
    FatJetFiller(const edm::ParameterSet& cfg, edm::ConsumesCollector && cc, const int options, const string branchName);
    ~FatJetFiller() {}

    enum  Options           {
                              NULLOPT         = 0
                            , LOADJETSHAPE    = (1 <<  0)   ///< load jet shape variables
                            , LOADSUBJETCTAG  = (1 <<  1)   ///< load subjet c-tagging
                            , LOADTOPMVA      = (1 <<  2)   ///< load softdrop top-tagging MVA
                            , LOADWTAGMVA     = (1 <<  3)   ///< load softdrop W-tagging MVA
    };
    static const int defaultOptions = NULLOPT;

    void load(const edm::Event& iEvent, const edm::EventSetup &iSetup);
    void fill();

  private :
    edm::EDGetTokenT<pat::JetCollection> fatJetToken_;
    edm::EDGetTokenT<pat::JetCollection> sdSubjetToken_;
    edm::EDGetTokenT<pat::JetCollection> puppiSubjetToken_;
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
    size ifj_csvboosted_;
    size ifj_nsoftdropsubjets_;
    size ifj_sdsubjet1_pt_;
    size ifj_sdsubjet1_eta_;
    size ifj_sdsubjet1_phi_;
    size ifj_sdsubjet1_mass_;
    size ifj_sdsubjet1_csv_;
    size ifj_sdsubjet1_cmva_;
    size ifj_sdsubjet1_cvsl_;
    size ifj_sdsubjet1_cvsb_;
    size ifj_sdsubjet1_ptD_;
    size ifj_sdsubjet1_axis1_;
    size ifj_sdsubjet1_axis2_;
    size ifj_sdsubjet1_jetMult_;
    size ifj_sdsubjet2_pt_;
    size ifj_sdsubjet2_eta_;
    size ifj_sdsubjet2_phi_;
    size ifj_sdsubjet2_mass_;
    size ifj_sdsubjet2_csv_;
    size ifj_sdsubjet2_cmva_;
    size ifj_sdsubjet2_cvsl_;
    size ifj_sdsubjet2_cvsb_;
    size ifj_sdsubjet2_ptD_;
    size ifj_sdsubjet2_axis1_;
    size ifj_sdsubjet2_axis2_;
    size ifj_sdsubjet2_jetMult_;
    size ifj_topmva;
    size ifj_wmva;

    size ifj_puppi_pt_;
    size ifj_puppi_eta_;
    size ifj_puppi_phi_;
    size ifj_puppi_mass_;
    size ifj_puppi_tau1_;
    size ifj_puppi_tau2_;
    size ifj_puppi_tau3_;
    //    size ifj_puppi_csv_;
    //    size ifj_puppi_cvsl_;
    //    size ifj_puppi_cvsb_;
    size ifj_puppi_nsoftdropsubjets_;
    size ifj_puppi_sdsubjet1_pt_;
    size ifj_puppi_sdsubjet1_eta_;
    size ifj_puppi_sdsubjet1_phi_;
    size ifj_puppi_sdsubjet1_mass_;
    size ifj_puppi_sdsubjet1_csv_;
    size ifj_puppi_sdsubjet1_cmva_;
    size ifj_puppi_sdsubjet1_cvsl_;
    size ifj_puppi_sdsubjet1_cvsb_;
    size ifj_puppi_sdsubjet2_pt_;
    size ifj_puppi_sdsubjet2_eta_;
    size ifj_puppi_sdsubjet2_phi_;
    size ifj_puppi_sdsubjet2_mass_;
    size ifj_puppi_sdsubjet2_csv_;
    size ifj_puppi_sdsubjet2_cmva_;
    size ifj_puppi_sdsubjet2_cvsl_;
    size ifj_puppi_sdsubjet2_cvsb_;
    size ifj_puppi_sdmass_;

  public :
    // Data members
    edm::Handle<pat::JetCollection> fatJets_;
    edm::Handle<pat::JetCollection> sdCollectionSubjets_;
    edm::Handle<pat::JetCollection> puppiCollectionSubjets_;

  protected:
    QuarkGluonTaggingVariables* qgTaggingVar_;
    SoftdropTopMVA * sdTopMVA = nullptr;
    SoftdropWTagMVA * sdWTagMVA = nullptr;

  };

}

#endif
