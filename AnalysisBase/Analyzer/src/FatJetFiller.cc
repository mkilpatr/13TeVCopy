//--------------------------------------------------------------------------------------------------
// 
// FatJetFiller
// 
// Muon filler.
// 
// FatJetFiller.cc created on Fri Oct 17 12:11:09 CEST 2014 
// 
//--------------------------------------------------------------------------------------------------

#include "AnalysisBase/Analyzer/interface/FatJetFiller.h"
#include "AnalysisBase/Analyzer/interface/JetFiller.h"

using namespace ucsbsusy;

//--------------------------------------------------------------------------------------------------
FatJetFiller::FatJetFiller(const edm::ParameterSet& cfg, edm::ConsumesCollector && cc, const int options, const string branchName) :
  BaseFiller(options, branchName),
  fatJetToken_(cc.consumes<pat::JetCollection>(cfg.getParameter<edm::InputTag>("fatJets")))
{

  ifj_rawmass_       = data.addMulti<float>(branchName_,"fatjet_rawmass",0);
  ifj_trimmedmass_   = data.addMulti<float>(branchName_,"fatjet_trimmedmass",0);
  ifj_prunedmass_    = data.addMulti<float>(branchName_,"fatjet_prunedmass",0);
  ifj_softdropmass_  = data.addMulti<float>(branchName_,"fatjet_softdropmass",0);
  ifj_filteredmass_  = data.addMulti<float>(branchName_,"fatjet_filteredmass",0);
  ifj_cmstoptagmass_ = data.addMulti<float>(branchName_,"fatjet_cmstoptagmass",0);
  //ifj_massdropfilteredmass_ = data.addMulti<float>(branchName_,"fatjet_massdropfilteredmass",0);
  ifj_tau1_          = data.addMulti<float>(branchName_,"fatjet_tau1",-1.);
  ifj_tau2_          = data.addMulti<float>(branchName_,"fatjet_tau2",-1.);
  ifj_tau3_          = data.addMulti<float>(branchName_,"fatjet_tau3",-1.);
  ifj_pt_            = data.addMulti<float>(branchName_,"fatjet_pt",0);
  ifj_eta_           = data.addMulti<float>(branchName_,"fatjet_eta",0);
  ifj_phi_           = data.addMulti<float>(branchName_,"fatjet_phi",0);

}

//--------------------------------------------------------------------------------------------------
void FatJetFiller::load(const edm::Event& iEvent, const edm::EventSetup &iSetup)
{
  reset();
  iEvent.getByToken(fatJetToken_,fatJets_);
  isLoaded_ = true;
}

//--------------------------------------------------------------------------------------------------
void FatJetFiller::fill()
{

  for (const pat::Jet &fatjet : *fatJets_) {

    float cmstoptagmass_ = -1;
    const reco::CATopJetTagInfo * catopTag = dynamic_cast<reco::CATopJetTagInfo const *>(fatjet.tagInfo("caTop"));
    if (catopTag) { cmstoptagmass_ = catopTag->properties().topMass; }
    else          { cmstoptagmass_ = fatjet.mass(); }



    data.fillMulti<float>(ifj_rawmass_       , fatjet.mass());
    data.fillMulti<float>(ifj_trimmedmass_   , fatjet.userFloat("ak8PFJetsCHSTrimmedMass"));
    data.fillMulti<float>(ifj_prunedmass_    , fatjet.userFloat("ak8PFJetsCHSPrunedMass"));
    data.fillMulti<float>(ifj_softdropmass_  , fatjet.userFloat("ak8PFJetsCHSSoftDropMass"));
    data.fillMulti<float>(ifj_filteredmass_  , fatjet.userFloat("ak8PFJetsCHSFilteredMass"));
    data.fillMulti<float>(ifj_cmstoptagmass_ , cmstoptagmass_);
    //    data.fillMulti<float>(ifj_massdropfilteredmass_, fatjet.userFloat("ak8PFJetsCHSMassDropFilteredLinks"));
    data.fillMulti<float>(ifj_tau1_          , fatjet.userFloat("NjettinessAK8:tau1"));
    data.fillMulti<float>(ifj_tau2_          , fatjet.userFloat("NjettinessAK8:tau2"));
    data.fillMulti<float>(ifj_tau3_          , fatjet.userFloat("NjettinessAK8:tau3"));
    data.fillMulti<float>(ifj_pt_            , fatjet.pt());
    data.fillMulti<float>(ifj_eta_           , fatjet.eta());
    data.fillMulti<float>(ifj_phi_           , fatjet.phi());
    data.fillMulti<bool> (ifj_looseId_       , JetFunctions::passLooseJetId(fatjet));
    data.fillMulti<bool> (ifj_tightId_       , JetFunctions::passTightJetId(fatjet));
    
    isFilled_ = true;

  }

}

