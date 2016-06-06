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

  ictt_cmstoptagmass_ = data.addMulti<float>(branchName_,"top_cmstoptagmass",0);
  ictt_allsubjetmass_ = data.addMulti<float>(branchName_,"top_allsubjetmass",0);

  ictt_top_pt_        = data.addMulti<float>(branchName_,"top_pt",0);
  ictt_top_eta_       = data.addMulti<float>(branchName_,"top_eta",0);
  ictt_top_phi_       = data.addMulti<float>(branchName_,"top_phi",0);
  ictt_top_minmass_   = data.addMulti<float>(branchName_,"top_minmass",0);
  ictt_top_nsubjets_  = data.addMulti<float>(branchName_,"top_nsubjets",0);

}

//--------------------------------------------------------------------------------------------------
void CMSTopFiller::load(const edm::Event& iEvent, const edm::EventSetup &iSetup)
{
  reset();
  iEvent.getByToken(fatJetToken_,fatJets_);
  isLoaded_ = true;
}

//--------------------------------------------------------------------------------------------------
void CMSTopFiller::fill()
{

  for (const pat::Jet &fatjet : *fatJets_) {
    
    std::vector<TLorentzVector> subjets; subjets.clear();
    TLorentzVector top; 
    for (unsigned int idau =0; idau<fatjet.numberOfDaughters(); ++idau) {
      const reco::Candidate * dau = fatjet.daughter(idau);
      TLorentzVector tmp; tmp.SetPtEtaPhiM(dau->pt(),dau->eta(),dau->phi(),dau->mass());
      subjets.push_back(tmp);
      top += tmp;
    }

    data.fillMulti<float>(ictt_top_pt_        , fatjet.pt());
    data.fillMulti<float>(ictt_top_eta_       , fatjet.eta());
    data.fillMulti<float>(ictt_top_phi_       , fatjet.phi());
    data.fillMulti<float>(ictt_cmstoptagmass_ , fatjet.mass());
    data.fillMulti<float>(ictt_allsubjetmass_ , top.M());
    data.fillMulti<float>(ictt_top_nsubjets_  , fatjet.numberOfDaughters());
    data.fillMulti<float>(ictt_top_minmass_   , getMinMass(subjets));

    isFilled_ = true;
  }

}


float CMSTopFiller::getMinMass(std::vector<TLorentzVector> subjetslv) {

  float minmass_ = 999.;

  for (unsigned int i0=0; i0<subjetslv.size(); ++i0) {
    for (unsigned int i1=0; i1<subjetslv.size(); ++i1) {
      if (i1<=i0) { continue; }
      float minmasstmp_ = (subjetslv[i0]+subjetslv[i1]).M();
      if (minmasstmp_< minmass_) { minmass_ = minmasstmp_; } 

    }
  }

  return minmass_;
}

