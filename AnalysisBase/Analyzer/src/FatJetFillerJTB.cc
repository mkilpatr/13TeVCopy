//--------------------------------------------------------------------------------------------------
// 
// Fatjetjtbfiller
// 
// Muon filler.
// 
// Fatjetjtbfiller.cc created on Fri Oct 17 12:11:09 CEST 2014 
// 
//--------------------------------------------------------------------------------------------------

#include "AnalysisBase/Analyzer/interface/FatJetJTBFiller.h"
#include "AnalysisBase/Analyzer/interface/JetFiller.h"

using namespace ucsbsusy;

//--------------------------------------------------------------------------------------------------
FatJetjtbFiller::FatJetjtbFiller(const edm::ParameterSet& cfg, edm::ConsumesCollector && cc, const int options, const string branchName) :
  BaseFiller(options, branchName),
  fatJetjtbToken_(cc.consumes<pat::JetCollection>(cfg.getParameter<edm::InputTag>("fatJets"))),
  fatJetSDjtbToken_(cc.consumes<pat::JetCollection>(cfg.getParameter<edm::InputTag>("fatJetsSD"))),
  fatJetSDSubjtbToken_(cc.consumes<pat::JetCollection>(cfg.getParameter<edm::InputTag>("fatJetsSDSub"))),
  puRemoval_(cfg.getUntrackedParameter<string>("puRemoval")),
  details_(cfg.getUntrackedParameter<string>("details"))
 {
 
  ifj_mass_        = data.addMulti<float>(branchName_,"fatjet_mass",0);
  ifj_pt_          = data.addMulti<float>(branchName_,"fatjet_pt",0);
  ifj_eta_         = data.addMulti<float>(branchName_,"fatjet_eta",0);
  ifj_phi_         = data.addMulti<float>(branchName_,"fatjet_phi",0);
  ifj_csv_         = data.addMulti<float>(branchName_,"fatjet_csv",0);
  ifj_tau1_        = data.addMulti<float>(branchName_,"fatjet_tau1",-1.);
  ifj_tau2_        = data.addMulti<float>(branchName_,"fatjet_tau2",-1.);
  ifj_tau3_        = data.addMulti<float>(branchName_,"fatjet_tau3",-1.);
  ifj_nsdsubjets_  = data.addMulti<int  >(branchName_,"fatjet_nsdsubjets",0);
  ifj_sdsub1_mass_ = data.addMulti<float>(branchName_,"fatjet_sdsub1_mass",0.);
  ifj_sdsub1_pt_   = data.addMulti<float>(branchName_,"fatjet_sdsub1_pt",0.);
  ifj_sdsub1_eta_  = data.addMulti<float>(branchName_,"fatjet_sdsub1_eta",0.);
  ifj_sdsub1_phi_  = data.addMulti<float>(branchName_,"fatjet_sdsub1_phi",0.);
  ifj_sdsub1_csv_  = data.addMulti<float>(branchName_,"fatjet_sdsub1_csv",0.);
  ifj_sdsub2_mass_ = data.addMulti<float>(branchName_,"fatjet_sdsub2_mass",0.);
  ifj_sdsub2_pt_   = data.addMulti<float>(branchName_,"fatjet_sdsub2_pt",0.);
  ifj_sdsub2_eta_  = data.addMulti<float>(branchName_,"fatjet_sdsub2_eta",0.);
  ifj_sdsub2_phi_  = data.addMulti<float>(branchName_,"fatjet_sdsub2_phi",0.);
  ifj_sdsub2_csv_  = data.addMulti<float>(branchName_,"fatjet_sdsub2_csv",0.);

}

//--------------------------------------------------------------------------------------------------
void FatJetjtbFiller::load(const edm::Event& iEvent, const edm::EventSetup &iSetup)
{
  reset();
  iEvent.getByToken(fatJetjtbToken_,fatJetsjtb_);
  iEvent.getByToken(fatJetSDjtbToken_,fatJetsSDjtb_);
  iEvent.getByToken(fatJetSDSubjtbToken_,fatJetsSDSubjtb_);
  isLoaded_ = true;
}

//--------------------------------------------------------------------------------------------------
void FatJetjtbFiller::fill()
{

  std::string detailsU_ = boost::to_upper_copy<std::string>(details_);


  for (const pat::Jet &fatjet : *fatJetsSDjtb_) {

    if (fatjet.pt()<=200.) { continue; }
    data.fillMulti<float>(ifj_mass_ , fatjet.mass());
    data.fillMulti<float>(ifj_pt_   , fatjet.pt());
    data.fillMulti<float>(ifj_eta_  , fatjet.eta());
    data.fillMulti<float>(ifj_phi_  , fatjet.phi());

    // get variables from the ungroomed fatjet
    unsigned int rawjetindx_ = matchedjet(fatJetsjtb_,fatjet); 
    data.fillMulti<float>(ifj_csv_  , fatJetsjtb_->at(rawjetindx_).bDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags"));
    data.fillMulti<float>(ifj_tau1_ , fatJetsjtb_->at(rawjetindx_).userFloat("Njettiness"+detailsU_+puRemoval_+":tau1"));
    data.fillMulti<float>(ifj_tau2_ , fatJetsjtb_->at(rawjetindx_).userFloat("Njettiness"+detailsU_+puRemoval_+":tau2"));
    data.fillMulti<float>(ifj_tau3_ , fatJetsjtb_->at(rawjetindx_).userFloat("Njettiness"+detailsU_+puRemoval_+":tau3"));

    // get variables from the softdrop subjets
    std::vector<pat::Jet> sdsubjets_; sdsubjets_.clear();
    for (const pat::Jet &fatjetsub : *fatJetsSDSubjtb_) {
      if (drmatch(fatjetsub,fatjet)<0.79) { sdsubjets_.push_back(fatjetsub); }
    }
    data.fillMulti<int> (ifj_nsdsubjets_,sdsubjets_.size());

    if (sdsubjets_.size()==0) {
      data.fillMulti<float>(ifj_sdsub1_mass_ ,-9.);
      data.fillMulti<float>(ifj_sdsub1_pt_   ,-9.);
      data.fillMulti<float>(ifj_sdsub1_eta_  ,-9.);
      data.fillMulti<float>(ifj_sdsub1_phi_  ,-9.);
      data.fillMulti<float>(ifj_sdsub1_csv_  ,-9.);
      data.fillMulti<float>(ifj_sdsub2_mass_ ,-9.);
      data.fillMulti<float>(ifj_sdsub2_pt_   ,-9.);
      data.fillMulti<float>(ifj_sdsub2_eta_  ,-9.);
      data.fillMulti<float>(ifj_sdsub2_phi_  ,-9.);
      data.fillMulti<float>(ifj_sdsub2_csv_  ,-9.);
    }

    if (sdsubjets_.size()==1) {
      data.fillMulti<float>(ifj_sdsub1_mass_ , sdsubjets_[0].mass());
      data.fillMulti<float>(ifj_sdsub1_pt_   , sdsubjets_[0].pt());
      data.fillMulti<float>(ifj_sdsub1_eta_  , sdsubjets_[0].eta());
      data.fillMulti<float>(ifj_sdsub1_phi_  , sdsubjets_[0].phi());
      data.fillMulti<float>(ifj_sdsub1_csv_  , sdsubjets_[0].bDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags"));
      data.fillMulti<float>(ifj_sdsub2_mass_ ,-9.);
      data.fillMulti<float>(ifj_sdsub2_pt_   ,-9.);
      data.fillMulti<float>(ifj_sdsub2_eta_  ,-9.);
      data.fillMulti<float>(ifj_sdsub2_phi_  ,-9.);
      data.fillMulti<float>(ifj_sdsub2_csv_  ,-9.);
    }

    if (sdsubjets_.size()>=2) {
      data.fillMulti<float>(ifj_sdsub1_mass_ , sdsubjets_[0].mass());
      data.fillMulti<float>(ifj_sdsub1_pt_   , sdsubjets_[0].pt());
      data.fillMulti<float>(ifj_sdsub1_eta_  , sdsubjets_[0].eta());
      data.fillMulti<float>(ifj_sdsub1_phi_  , sdsubjets_[0].phi());
      data.fillMulti<float>(ifj_sdsub1_csv_  , sdsubjets_[0].bDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags"));
      data.fillMulti<float>(ifj_sdsub2_mass_ , sdsubjets_[1].mass());
      data.fillMulti<float>(ifj_sdsub2_pt_   , sdsubjets_[1].pt());
      data.fillMulti<float>(ifj_sdsub2_eta_  , sdsubjets_[1].eta());
      data.fillMulti<float>(ifj_sdsub2_phi_  , sdsubjets_[1].phi());
      data.fillMulti<float>(ifj_sdsub2_csv_  , sdsubjets_[1].bDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags"));
    }

    isFilled_ = true;
  }

}


unsigned int FatJetjtbFiller::matchedjet(edm::Handle<std::vector<pat::Jet> >& rawjets, const pat::Jet sdjet) {
  
  float        drmin_      = 999.;
  unsigned int rawjetindx_ = -1;
  for (unsigned int i0=0; i0<rawjets->size(); ++i0) {
    float drtmp_ = drmatch(rawjets->at(i0),sdjet);
    if (drtmp_<drmin_) { 
      drmin_      = drtmp_;
      rawjetindx_ = i0;
    }
  }
  
  if (drmin_>=0.8) { rawjetindx_ = -1; } // here we shoud remove hardcoded r value
  return rawjetindx_;
}


float FatJetjtbFiller::drmatch(pat::Jet rawjet, pat::Jet sdjet) {
  float dr_ = PhysicsUtilities::deltaR(rawjet.eta(), rawjet.phi(), sdjet.eta(), sdjet.phi());
  return dr_;
}

