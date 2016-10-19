//--------------------------------------------------------------------------------------------------
//
// MuonFiller
//
// Class to fill muon information in a TTree.
//
// MuonFiller.h created on Fri Oct 17 12:11:09 CEST 2014
//
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISBASE_ANALYZER_HTTFILLER_H
#define ANALYSISBASE_ANALYZER_HTTFILLER_H

#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/JetReco/interface/HTTTopJetTagInfo.h"
#include "AnalysisBase/Analyzer/interface/BaseFiller.h"
#include "TLorentzVector.h"

namespace ucsbsusy {

class QuarkGluonTaggingVariables;

  class HTTFiller : public BaseFiller {

  public :
    HTTFiller(const edm::ParameterSet& cfg, edm::ConsumesCollector && cc, const int options, const string branchName);
    ~HTTFiller() {}

    void load(const edm::Event& iEvent, const edm::EventSetup &iSetup);
    void fill();

    template<class Jet>
    float getCSV(const Jet& jet ){
      for(unsigned int iT = 0; iT < btags->size(); ++iT){
        if(btags->key(iT).get() != &jet  ) continue;
        return btags->value(iT);
      }
      return -1;
    }

    vector<int> getSDInd();
    vector<int> getSubjetsInd(const reco::BasicJet &fatjet);


  private :
    edm::EDGetTokenT<reco::BasicJetCollection>           fatJetToken;
    edm::EDGetTokenT<reco::HTTTopJetTagInfoCollection>   fatJetInfosToken;
    edm::EDGetTokenT<reco::JetTagCollection>             bTagsToken;
    edm::EDGetTokenT<reco::PFJetCollection>              httSubjetToken;

    edm::EDGetTokenT<reco::PFJetCollection>              sdJetToken;
    edm::EDGetTokenT<edm::ValueMap<float>>               tau1Token ;
    edm::EDGetTokenT<edm::ValueMap<float>>               tau2Token ;
    edm::EDGetTokenT<edm::ValueMap<float>>               tau3Token ;

    // Members to hold indices of tree data
    size i_pt       ;
    size i_eta      ;
    size i_phi      ;
    size i_mass     ;
    size i_csv      ;
    size i_ropt     ;
    size i_frec     ;
    size i_roptcalc ;
    size i_ptforropt;
    size i_ropt_pt  ;
    size i_ropt_eta ;
    size i_ropt_phi ;
    size i_ropt_mass;
    size i_ropt_tau1;
    size i_ropt_tau2;
    size i_ropt_tau3;
    size i_sd_mass  ;
    size i_sd_tau1  ;
    size i_sd_tau2  ;
    size i_sd_tau3  ;
    size i_nsubjets ;
    size i_w1_pt    ;
    size i_w1_eta   ;
    size i_w1_phi   ;
    size i_w1_mass  ;
    size i_w1_csv   ;
    size i_w1_ptD   ;
    size i_w1_axis1 ;
    size i_w1_axis2 ;
    size i_w1_mult  ;
    size i_w2_pt    ;
    size i_w2_eta   ;
    size i_w2_phi   ;
    size i_w2_mass  ;
    size i_w2_csv   ;
    size i_w2_ptD   ;
    size i_w2_axis1 ;
    size i_w2_axis2 ;
    size i_w2_mult  ;
    size i_b_pt     ;
    size i_b_eta    ;
    size i_b_phi    ;
    size i_b_mass   ;
    size i_b_csv    ;
    size i_b_ptD    ;
    size i_b_axis1  ;
    size i_b_axis2  ;
    size i_b_mult   ;

    QuarkGluonTaggingVariables* qgTaggingVar_;

  public :
    // Data members
    edm::Handle<reco::BasicJetCollection>             fatJets;
    edm::Handle<reco::HTTTopJetTagInfoCollection>     fatJetInfos;
    edm::Handle<reco::JetTagCollection>               btags;
    edm::Handle<reco::PFJetCollection>                sdFatJets;
    edm::Handle<reco::PFJetCollection>                httSubjets;
    edm::Handle<edm::ValueMap<float>>                 tau1s;
    edm::Handle<edm::ValueMap<float>>                 tau2s;
    edm::Handle<edm::ValueMap<float>>                 tau3s;

  };

}

#endif
