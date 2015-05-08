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
CMSTopFiller::CMSTopFiller(const int options,
			   const string branchName,
			   const EventInfoFiller * evtInfoFiller,
			   const edm::InputTag fatJetTag) :
  BaseFiller(options, branchName),
  evtInfoFiller_(evtInfoFiller),
  fatJetTag_(fatJetTag)

{

  ictt_fatjet_mass_                 = data.addMulti<float>(branchName_,"fatjet_mass",0);
  ictt_fatjet_trimmedmass_          = data.addMulti<float>(branchName_,"fatjet_trimmedmass",0);
  ictt_fatjet_prunedmass_           = data.addMulti<float>(branchName_,"fatjet_prunedmass",0);
  ictt_fatjet_softdropmass_         = data.addMulti<float>(branchName_,"fatjet_softdropmass",0);
  ictt_fatjet_filteredmass_         = data.addMulti<float>(branchName_,"fatjet_filteredmass",0);
  ictt_fatjet_massdropfilteredmass_ = data.addMulti<float>(branchName_,"fatjet_massdropfilteredmass",0);
  ictt_fatjet_tau1_                 = data.addMulti<float>(branchName_,"fatjet_tau1",-1.);
  ictt_fatjet_tau2_                 = data.addMulti<float>(branchName_,"fatjet_tau2",-1.);
  ictt_fatjet_tau3_                 = data.addMulti<float>(branchName_,"fatjet_tau3",-1.);
  ictt_top_pt_                      = data.addMulti<float>(branchName_,"top_pt",0);
  ictt_top_eta_                     = data.addMulti<float>(branchName_,"top_eta",0);
  ictt_top_phi_                     = data.addMulti<float>(branchName_,"top_phi",0);
  ictt_top_topmass_                 = data.addMulti<float>(branchName_,"top_topmass",0);
  ictt_top_wmass_                   = data.addMulti<float>(branchName_,"top_wmass",0);
  ictt_top_minmass_                 = data.addMulti<float>(branchName_,"top_minmass",0);
  ictt_top_nsubjets_                = data.addMulti<float>(branchName_,"top_nsubjets",0);

}

//--------------------------------------------------------------------------------------------------
void CMSTopFiller::load(const edm::Event& iEvent)
{
  reset();
  FileUtilities::enforceGet(iEvent, fatJetTag_,fatJets_,true);
  isLoaded_ = true;
}

//--------------------------------------------------------------------------------------------------
void CMSTopFiller::fill()
{

  for (const pat::Jet &fatjet : *fatJets_) {

    const reco::CATopJetTagInfo * catopTag = dynamic_cast<reco::CATopJetTagInfo const *>(fatjet.tagInfo("caTop"));
    if (!catopTag) { continue; }

    data.fillMulti<float>(ictt_fatjet_mass_                , fatjet.mass());
    data.fillMulti<float>(ictt_fatjet_trimmedmass_         , fatjet.userFloat("ak8PFJetsCHSTrimmedLinks"));
    data.fillMulti<float>(ictt_fatjet_prunedmass_          , fatjet.userFloat("ak8PFJetsCHSPrunedLinks"));
    data.fillMulti<float>(ictt_fatjet_softdropmass_        , fatjet.userFloat("ak8PFJetsCHSSoftDropLinks"));
    data.fillMulti<float>(ictt_fatjet_filteredmass_        , fatjet.userFloat("ak8PFJetsCHSFilteredLinks"));
    data.fillMulti<float>(ictt_fatjet_massdropfilteredmass_, fatjet.userFloat("ak8PFJetsCHSMassDropFilteredLinks"));
    data.fillMulti<float>(ictt_fatjet_tau1_                , fatjet.userFloat("NjettinessAK8:tau1"));
    data.fillMulti<float>(ictt_fatjet_tau2_                , fatjet.userFloat("NjettinessAK8:tau2"));
    data.fillMulti<float>(ictt_fatjet_tau3_                , fatjet.userFloat("NjettinessAK8:tau3"));
    data.fillMulti<float>(ictt_top_pt_                     , fatjet.pt());
    data.fillMulti<float>(ictt_top_eta_                    , fatjet.eta());
    data.fillMulti<float>(ictt_top_phi_                    , fatjet.phi());
    data.fillMulti<float>(ictt_top_topmass_                , catopTag->properties().topMass);
    data.fillMulti<float>(ictt_top_wmass_                  , catopTag->properties().wMass);
    data.fillMulti<float>(ictt_top_minmass_                , catopTag->properties().minMass);
    data.fillMulti<float>(ictt_top_nsubjets_               , catopTag->properties().nSubJets);
    
    isFilled_ = true;

  }

}

