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
  fatJetToken_(cc.consumes<pat::JetCollection>(cfg.getParameter<edm::InputTag>("fatJets"))),
  puRemoval_(cfg.getUntrackedParameter<string>("puRemoval"))
{

  ifj_rawmass_       = data.addMulti<float>(branchName_,"fatjet_rawmass",0);
  ifj_trimmedmass_   = data.addMulti<float>(branchName_,"fatjet_trimmedmass",0);
  ifj_prunedmass_    = data.addMulti<float>(branchName_,"fatjet_prunedmass",0);
  ifj_softdropmass_  = data.addMulti<float>(branchName_,"fatjet_softdropmass",0);
  ifj_filteredmass_  = data.addMulti<float>(branchName_,"fatjet_filteredmass",0);
  ifj_cmstoptagmass_ = data.addMulti<float>(branchName_,"fatjet_cmstoptagmass",0);
  ifj_cmstoptagminmass_   = data.addMulti<float>(branchName_,"fatjet_cmstoptagminmass",0);
  ifj_cmstoptagnsubjets_  = data.addMulti<float>(branchName_,"fatjet_cmstoptagnsubjets",0);
  //ifj_massdropfilteredmass_ = data.addMulti<float>(branchName_,"fatjet_massdropfilteredmass",0);
  ifj_tau1_          = data.addMulti<float>(branchName_,"fatjet_tau1",-1.);
  ifj_tau2_          = data.addMulti<float>(branchName_,"fatjet_tau2",-1.);
  ifj_tau3_          = data.addMulti<float>(branchName_,"fatjet_tau3",-1.);
  ifj_pt_            = data.addMulti<float>(branchName_,"fatjet_pt",0);
  ifj_eta_           = data.addMulti<float>(branchName_,"fatjet_eta",0);
  ifj_phi_           = data.addMulti<float>(branchName_,"fatjet_phi",0);
  ifj_looseId_       = data.addMulti<bool> (branchName_,"fatjet_looseId" ,0);
  ifj_tightId_       = data.addMulti<bool> (branchName_,"fatjet_tightId" ,0);
  ifj_muEnFrac_      = data.addMulti<bool> (branchName_,"fatjet_muEnFrac",0);

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

    if ((fatjet.pt()<=50.) || (abs(fatjet.eta())<2.5) ) { continue; } 

    float cmstoptagmass = -1;
    float cmstoptagminmass = -1;
    int   cmstoptagnsubjets = 0;
    const reco::CATopJetTagInfo * catopTag = dynamic_cast<reco::CATopJetTagInfo const *>(fatjet.tagInfo("caTop"));
    if (catopTag) {
      cmstoptagmass     = catopTag->properties().topMass;
      cmstoptagminmass  = catopTag->properties().minMass;
      cmstoptagnsubjets = catopTag->properties().nSubJets;
    }
    else          { cmstoptagmass = fatjet.mass(); }

    data.fillMulti<float>(ifj_rawmass_       , fatjet.mass());
    data.fillMulti<float>(ifj_trimmedmass_   , fatjet.userFloat("ak8PFJets"+puRemoval_+"TrimmedMass"));
    data.fillMulti<float>(ifj_prunedmass_    , fatjet.userFloat("ak8PFJets"+puRemoval_+"PrunedMass"));
    data.fillMulti<float>(ifj_softdropmass_  , fatjet.userFloat("ak8PFJets"+puRemoval_+"SoftDropMass"));
    data.fillMulti<float>(ifj_filteredmass_  , fatjet.userFloat("ak8PFJets"+puRemoval_+"FilteredMass"));
    data.fillMulti<float>(ifj_cmstoptagmass_ , cmstoptagmass);
    data.fillMulti<float>(ifj_cmstoptagminmass_ , cmstoptagminmass);
    data.fillMulti<int  >(ifj_cmstoptagnsubjets_, cmstoptagnsubjets);
    //    data.fillMulti<float>(ifj_massdropfilteredmass_, fatjet.userFloat("ak8PFJets"+puRemoval_+"MassDropFilteredLinks"));
    if (puRemoval_=="Puppi") { 
      data.fillMulti<float>(ifj_tau1_          , fatjet.userFloat("NjettinessAK8"+puRemoval_+":tau1"));
      data.fillMulti<float>(ifj_tau2_          , fatjet.userFloat("NjettinessAK8"+puRemoval_+":tau2"));
      data.fillMulti<float>(ifj_tau3_          , fatjet.userFloat("NjettinessAK8"+puRemoval_+":tau3"));
    }
    else {
      data.fillMulti<float>(ifj_tau1_          , fatjet.userFloat("NjettinessAK8:tau1"));
      data.fillMulti<float>(ifj_tau2_          , fatjet.userFloat("NjettinessAK8:tau2"));
      data.fillMulti<float>(ifj_tau3_          , fatjet.userFloat("NjettinessAK8:tau3"));
    } 
    data.fillMulti<float>(ifj_pt_            , fatjet.pt());
    data.fillMulti<float>(ifj_eta_           , fatjet.eta());
    data.fillMulti<float>(ifj_phi_           , fatjet.phi());
    data.fillMulti<bool> (ifj_looseId_       , JetFunctions::passLooseJetId(fatjet));
    data.fillMulti<bool> (ifj_tightId_       , JetFunctions::passTightJetId(fatjet));
    data.fillMulti<bool> (ifj_muEnFrac_      , JetFunctions::passMuonEnergyFraction(fatjet));
    
    isFilled_ = true;

  }

}

