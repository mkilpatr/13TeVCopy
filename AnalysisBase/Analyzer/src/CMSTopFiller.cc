//--------------------------------------------------------------------------------------------------
// 
// CMSTopFiller
// 
// Muon filler.
// 
// CMSTopFiller.cc created on Fri Oct 17 12:11:09 CEST 2014 
// 
//--------------------------------------------------------------------------------------------------

#include "AnalysisBase/Analyzer/interface/CMSTopFiller.h"

using namespace ucsbsusy;

//--------------------------------------------------------------------------------------------------
CMSTopFiller::CMSTopFiller(const edm::ParameterSet& cfg, edm::ConsumesCollector && cc, const int options, const string branchName) :
  BaseFiller(options, branchName),
  fatJetToken_(cc.consumes<pat::JetCollection>(cfg.getParameter<edm::InputTag>("fatJets")))
{

  ictt_rawmass_       = data.addMulti<float>(branchName_,"top_rawmass",0);
  ictt_trimmedmass_   = data.addMulti<float>(branchName_,"top_trimmedmass",0);
  ictt_prunedmass_    = data.addMulti<float>(branchName_,"top_prunedmass",0);
  ictt_softdropmass_  = data.addMulti<float>(branchName_,"top_softdropmass",0);
  ictt_filteredmass_  = data.addMulti<float>(branchName_,"top_filteredmass",0);
  ictt_cmstoptagmass_ = data.addMulti<float>(branchName_,"top_cmstoptagmass",0);
  //  ictt_massdropfilteredmass_ = data.addMulti<float>(branchName_,"top_massdropfilteredmass",0);
  ictt_tau1_          = data.addMulti<float>(branchName_,"top_tau1",-1.);
  ictt_tau2_          = data.addMulti<float>(branchName_,"top_tau2",-1.);
  ictt_tau3_          = data.addMulti<float>(branchName_,"top_tau3",-1.);
  ictt_top_pt_        = data.addMulti<float>(branchName_,"top_pt",0);
  ictt_top_eta_       = data.addMulti<float>(branchName_,"top_eta",0);
  ictt_top_phi_       = data.addMulti<float>(branchName_,"top_phi",0);
  ictt_top_wmass_     = data.addMulti<float>(branchName_,"top_wmass",0);
  ictt_top_minmass_   = data.addMulti<float>(branchName_,"top_minmass",0);
  ictt_top_nsubjets_  = data.addMulti<float>(branchName_,"top_nsubjets",0);

}

//--------------------------------------------------------------------------------------------------
void CMSTopFiller::load(const edm::Event& iEvent)
{
  reset();
  iEvent.getByToken(fatJetToken_,fatJets_);
  isLoaded_ = true;
}

//--------------------------------------------------------------------------------------------------
void CMSTopFiller::fill()
{

  for (const pat::Jet &fatjet : *fatJets_) {

    const reco::CATopJetTagInfo * catopTag = dynamic_cast<reco::CATopJetTagInfo const *>(fatjet.tagInfo("caTop"));
    if (!catopTag) { continue; }

    data.fillMulti<float>(ictt_rawmass_      , fatjet.mass());
    data.fillMulti<float>(ictt_trimmedmass_  , fatjet.userFloat("ak8PFJetsCHSTrimmedMass"));
    data.fillMulti<float>(ictt_prunedmass_   , fatjet.userFloat("ak8PFJetsCHSPrunedMass"));
    data.fillMulti<float>(ictt_softdropmass_ , fatjet.userFloat("ak8PFJetsCHSSoftDropMass"));
    data.fillMulti<float>(ictt_filteredmass_ , fatjet.userFloat("ak8PFJetsCHSFilteredMass"));
    data.fillMulti<float>(ictt_cmstoptagmass_, catopTag->properties().topMass);
    //    data.fillMulti<float>(ictt_massdropfilteredmass_, fatjet.userFloat("ak8PFJetsCHSMassDropFilteredLinks"));
    data.fillMulti<float>(ictt_tau1_         , fatjet.userFloat("NjettinessAK8:tau1"));
    data.fillMulti<float>(ictt_tau2_         , fatjet.userFloat("NjettinessAK8:tau2"));
    data.fillMulti<float>(ictt_tau3_         , fatjet.userFloat("NjettinessAK8:tau3"));
    data.fillMulti<float>(ictt_top_pt_       , fatjet.pt());
    data.fillMulti<float>(ictt_top_eta_      , fatjet.eta());
    data.fillMulti<float>(ictt_top_phi_      , fatjet.phi());
    data.fillMulti<float>(ictt_top_wmass_    , catopTag->properties().wMass);
    data.fillMulti<float>(ictt_top_minmass_  , catopTag->properties().minMass);
    data.fillMulti<float>(ictt_top_nsubjets_ , catopTag->properties().nSubJets);
    
    isFilled_ = true;
  }

}

