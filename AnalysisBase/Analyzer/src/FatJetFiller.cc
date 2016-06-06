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
  ifj_prunedmass_    = data.addMulti<float>(branchName_,"fatjet_prunedmass",0);
  ifj_softdropmass_  = data.addMulti<float>(branchName_,"fatjet_softdropmass",0);
  //  ifj_cmstoptagmass_ = data.addMulti<float>(branchName_,"fatjet_cmstoptagmass",0);
  //  ifj_cmstoptagminmass_   = data.addMulti<float>(branchName_,"fatjet_cmstoptagminmass",0);
  //  ifj_cmstoptagnsubjets_  = data.addMulti<float>(branchName_,"fatjet_cmstoptagnsubjets",0);
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
  ifj_csv_           = data.addMulti<float> (branchName_,"fatjet_csv",0);
  ifj_cvsl_          = data.addMulti<float> (branchName_,"fatjet_cvsl",0);
  ifj_cvsb_          = data.addMulti<float> (branchName_,"fatjet_cvsb",0);
  ifj_nsoftdropsubjets_ = data.addMulti<int>(branchName_,"fatjet_nsoftdropsubjets",0);
  ifj_sdsubjet1_pt_     = data.addMulti<float> (branchName_,"fatjet_sdsubjet1_pt",0);
  ifj_sdsubjet1_eta_    = data.addMulti<float> (branchName_,"fatjet_sdsubjet1_eta",0);
  ifj_sdsubjet1_phi_    = data.addMulti<float> (branchName_,"fatjet_sdsubjet1_phi",0);
  ifj_sdsubjet1_mass_   = data.addMulti<float> (branchName_,"fatjet_sdsubjet1_mass",0);
  ifj_sdsubjet1_csv_    = data.addMulti<float> (branchName_,"fatjet_sdsubjet1_csv",0);
  ifj_sdsubjet2_pt_     = data.addMulti<float> (branchName_,"fatjet_sdsubjet2_pt",0);
  ifj_sdsubjet2_eta_    = data.addMulti<float> (branchName_,"fatjet_sdsubjet2_eta",0);
  ifj_sdsubjet2_phi_    = data.addMulti<float> (branchName_,"fatjet_sdsubjet2_phi",0);
  ifj_sdsubjet2_mass_   = data.addMulti<float> (branchName_,"fatjet_sdsubjet2_mass",0);
  ifj_sdsubjet2_csv_    = data.addMulti<float> (branchName_,"fatjet_sdsubjet2_csv",0);

  // puppi related from miniaod
  ifj_puppi_pt_               = data.addMulti<float>(branchName_,"fatjet_puppi_pt",0);
  ifj_puppi_eta_              = data.addMulti<float>(branchName_,"fatjet_puppi_eta",0);
  ifj_puppi_phi_              = data.addMulti<float>(branchName_,"fatjet_puppi_phi",0);
  ifj_puppi_mass_             = data.addMulti<float>(branchName_,"fatjet_puppi_mass",0);
  ifj_puppi_tau1_             = data.addMulti<float>(branchName_,"fatjet_puppi_tau1",-1.);
  ifj_puppi_tau2_             = data.addMulti<float>(branchName_,"fatjet_puppi_tau2",-1.);
  ifj_puppi_tau3_             = data.addMulti<float>(branchName_,"fatjet_puppi_tau3",-1.);
  //  ifj_puppi_csv_              = data.addMulti<float> (branchName_,"fatjet_puppi_csv",0);
  //  ifj_puppi_cvsl_             = data.addMulti<float> (branchName_,"fatjet_puppi_cvsl",0);
  //  ifj_puppi_cvsb_             = data.addMulti<float> (branchName_,"fatjet_puppi_cvsb",0);
  ifj_puppi_nsoftdropsubjets_ = data.addMulti<int>(branchName_,"fatjet_puppi_nsoftdropsubjets",0);
  ifj_puppi_sdsubjet1_pt_     = data.addMulti<float> (branchName_,"fatjet_puppi_sdsubjet1_pt",0);
  ifj_puppi_sdsubjet1_eta_    = data.addMulti<float> (branchName_,"fatjet_puppi_sdsubjet1_eta",0);
  ifj_puppi_sdsubjet1_phi_    = data.addMulti<float> (branchName_,"fatjet_puppi_sdsubjet1_phi",0);
  ifj_puppi_sdsubjet1_mass_   = data.addMulti<float> (branchName_,"fatjet_puppi_sdsubjet1_mass",0);
  ifj_puppi_sdsubjet1_csv_    = data.addMulti<float> (branchName_,"fatjet_puppi_sdsubjet1_csv",0);
  ifj_puppi_sdsubjet2_pt_     = data.addMulti<float> (branchName_,"fatjet_puppi_sdsubjet2_pt",0);
  ifj_puppi_sdsubjet2_eta_    = data.addMulti<float> (branchName_,"fatjet_puppi_sdsubjet2_eta",0);
  ifj_puppi_sdsubjet2_phi_    = data.addMulti<float> (branchName_,"fatjet_puppi_sdsubjet2_phi",0);
  ifj_puppi_sdsubjet2_mass_   = data.addMulti<float> (branchName_,"fatjet_puppi_sdsubjet2_mass",0);
  ifj_puppi_sdsubjet2_csv_    = data.addMulti<float> (branchName_,"fatjet_puppi_sdsubjet2_csv",0);
  ifj_puppi_sdmass_           = data.addMulti<float>(branchName_,"fatjet_puppi_sdmass",0);
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

    if (fatjet.pt()<=50.) { continue; }
    /*
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
    */
//    data.fillMulti<float>(ifj_trimmedmass_   , fatjet.userFloat("ak8PFJets"+puRemoval_+"TrimmedMass"));//FIXME_76X
//    data.fillMulti<float>(ifj_filteredmass_  , fatjet.userFloat("ak8PFJets"+puRemoval_+"FilteredMass"));
//    data.fillMulti<float>(ifj_cmstoptagmass_ , cmstoptagmass);
//    data.fillMulti<float>(ifj_cmstoptagminmass_ , cmstoptagminmass);
//    data.fillMulti<int  >(ifj_cmstoptagnsubjets_, cmstoptagnsubjets);
    //    data.fillMulti<float>(ifj_massdropfilteredmass_, fatjet.userFloat("ak8PFJets"+puRemoval_+"MassDropFilteredLinks"));

    data.fillMulti<float>(ifj_rawmass_         , fatjet.mass());
    data.fillMulti<float>(ifj_prunedmass_      , fatjet.userFloat("ak8PFJets"+puRemoval_+"PrunedMass"));
    data.fillMulti<float>(ifj_softdropmass_    , fatjet.userFloat("ak8PFJets"+puRemoval_+"SoftDropMass"));
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
    data.fillMulti<float>(ifj_pt_              , fatjet.pt());
    data.fillMulti<float>(ifj_eta_             , fatjet.eta());
    data.fillMulti<float>(ifj_phi_             , fatjet.phi());
    data.fillMulti<bool> (ifj_looseId_         , JetFunctions::passLooseJetId(fatjet));
    data.fillMulti<bool> (ifj_tightId_         , JetFunctions::passTightJetId(fatjet));
    data.fillMulti<bool> (ifj_muEnFrac_        , JetFunctions::passMuonEnergyFraction(fatjet));
    data.fillMulti<float>(ifj_csv_             , fatjet.bDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags"));
    data.fillMulti<float>(ifj_cvsl_            , fatjet.bDiscriminator("pfCombinedCvsLJetTags"));
    data.fillMulti<float>(ifj_cvsb_            , fatjet.bDiscriminator("pfCombinedCvsBJetTags"));

    // fill puppi related variables from miniaod
    data.fillMulti<float>(ifj_puppi_pt_   , fatjet.userFloat("ak8PFJetsPuppiValueMap:pt"));
    data.fillMulti<float>(ifj_puppi_eta_  , fatjet.userFloat("ak8PFJetsPuppiValueMap:eta"));
    data.fillMulti<float>(ifj_puppi_phi_  , fatjet.userFloat("ak8PFJetsPuppiValueMap:phi"));
    data.fillMulti<float>(ifj_puppi_mass_ , fatjet.userFloat("ak8PFJetsPuppiValueMap:mass"));
    data.fillMulti<float>(ifj_puppi_tau1_ , fatjet.userFloat("ak8PFJetsPuppiValueMap:NjettinessAK8PuppiTau1"));
    data.fillMulti<float>(ifj_puppi_tau2_ , fatjet.userFloat("ak8PFJetsPuppiValueMap:NjettinessAK8PuppiTau2"));
    data.fillMulti<float>(ifj_puppi_tau3_ , fatjet.userFloat("ak8PFJetsPuppiValueMap:NjettinessAK8PuppiTau3"));


    // get the soft drop subjets
    // soft-drop gives up to two subjets - fill the subjet variables in a dummy way now.
    auto const & sdsubjet_      = fatjet.subjets("SoftDrop");
    data.fillMulti<int>  (ifj_nsoftdropsubjets_ , sdsubjet_.size());

    std::vector<TLorentzVector> sdsubjetsp4;
    std::vector<float>          sdsubjetsCSV;
    for (auto const & it : sdsubjet_) {
      TLorentzVector tmpLV; tmpLV.SetPtEtaPhiM(it->pt(),it->eta(),it->phi(),it->mass());
      sdsubjetsp4.push_back(tmpLV);
      sdsubjetsCSV.push_back(it->bDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags"));
    }

    if (sdsubjetsp4.size()==0) {
      data.fillMulti<float>(ifj_sdsubjet1_pt_  ,-9.);
      data.fillMulti<float>(ifj_sdsubjet1_eta_ ,-9.);
      data.fillMulti<float>(ifj_sdsubjet1_phi_ ,-9.);
      data.fillMulti<float>(ifj_sdsubjet1_mass_,-9.);
      data.fillMulti<float>(ifj_sdsubjet1_csv_ ,-9.);
      data.fillMulti<float>(ifj_sdsubjet2_pt_  ,-9.);
      data.fillMulti<float>(ifj_sdsubjet2_eta_ ,-9.);
      data.fillMulti<float>(ifj_sdsubjet2_phi_ ,-9.);
      data.fillMulti<float>(ifj_sdsubjet2_mass_,-9.);
      data.fillMulti<float>(ifj_sdsubjet2_csv_ ,-9.);
    }

    if (sdsubjetsp4.size()==1) {
      data.fillMulti<float>(ifj_sdsubjet1_pt_  ,sdsubjetsp4[0].Pt());
      data.fillMulti<float>(ifj_sdsubjet1_eta_ ,sdsubjetsp4[0].Eta());
      data.fillMulti<float>(ifj_sdsubjet1_phi_ ,sdsubjetsp4[0].Phi());
      data.fillMulti<float>(ifj_sdsubjet1_mass_,sdsubjetsp4[0].M());
      data.fillMulti<float>(ifj_sdsubjet1_csv_ ,sdsubjetsCSV[0]);
      data.fillMulti<float>(ifj_sdsubjet2_pt_  ,-9.);
      data.fillMulti<float>(ifj_sdsubjet2_eta_ ,-9.);
      data.fillMulti<float>(ifj_sdsubjet2_phi_ ,-9.);
      data.fillMulti<float>(ifj_sdsubjet2_mass_,-9.);
      data.fillMulti<float>(ifj_sdsubjet2_csv_ ,-9.);
    }

    if (sdsubjetsp4.size()>=2) {
      data.fillMulti<float>(ifj_sdsubjet1_pt_  ,sdsubjetsp4[0].Pt());
      data.fillMulti<float>(ifj_sdsubjet1_eta_ ,sdsubjetsp4[0].Eta());
      data.fillMulti<float>(ifj_sdsubjet1_phi_ ,sdsubjetsp4[0].Phi());
      data.fillMulti<float>(ifj_sdsubjet1_mass_,sdsubjetsp4[0].M());
      data.fillMulti<float>(ifj_sdsubjet1_csv_ ,sdsubjetsCSV[0]);
      data.fillMulti<float>(ifj_sdsubjet2_pt_  ,sdsubjetsp4[1].Pt());
      data.fillMulti<float>(ifj_sdsubjet2_eta_ ,sdsubjetsp4[1].Eta());
      data.fillMulti<float>(ifj_sdsubjet2_phi_ ,sdsubjetsp4[1].Phi());
      data.fillMulti<float>(ifj_sdsubjet2_mass_,sdsubjetsp4[1].M());
      data.fillMulti<float>(ifj_sdsubjet2_csv_ ,sdsubjetsCSV[1]);
    }

    
    // for puppi
    auto const & sdpuppisubjet_ = fatjet.subjets("SoftDropPuppi");
    data.fillMulti<int>  (ifj_puppi_nsoftdropsubjets_ , sdpuppisubjet_.size());

    std::vector<TLorentzVector> sdpuppisubjetsp4;
    std::vector<float>          sdpuppisubjetsCSV; 
    for (auto const & it : sdpuppisubjet_) {
      TLorentzVector tmpLV; tmpLV.SetPtEtaPhiM(it->pt(),it->eta(),it->phi(),it->mass());
      sdpuppisubjetsp4.push_back(tmpLV);              
      sdpuppisubjetsCSV.push_back(it->bDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags"));
    }

    if (sdpuppisubjetsp4.size()==0) {
      data.fillMulti<float>(ifj_puppi_sdsubjet1_pt_  ,-9.);
      data.fillMulti<float>(ifj_puppi_sdsubjet1_eta_ ,-9.);
      data.fillMulti<float>(ifj_puppi_sdsubjet1_phi_ ,-9.);
      data.fillMulti<float>(ifj_puppi_sdsubjet1_mass_,-9.);
      data.fillMulti<float>(ifj_puppi_sdsubjet1_csv_ ,-9.);
      data.fillMulti<float>(ifj_puppi_sdsubjet2_pt_  ,-9.);
      data.fillMulti<float>(ifj_puppi_sdsubjet2_eta_ ,-9.);
      data.fillMulti<float>(ifj_puppi_sdsubjet2_phi_ ,-9.);
      data.fillMulti<float>(ifj_puppi_sdsubjet2_mass_,-9.);
      data.fillMulti<float>(ifj_puppi_sdsubjet2_csv_ ,-9.);
    }

    if (sdpuppisubjetsp4.size()==1) {
      data.fillMulti<float>(ifj_puppi_sdsubjet1_pt_  ,sdpuppisubjetsp4[0].Pt());
      data.fillMulti<float>(ifj_puppi_sdsubjet1_eta_ ,sdpuppisubjetsp4[0].Eta());
      data.fillMulti<float>(ifj_puppi_sdsubjet1_phi_ ,sdpuppisubjetsp4[0].Phi());
      data.fillMulti<float>(ifj_puppi_sdsubjet1_mass_,sdpuppisubjetsp4[0].M());
      data.fillMulti<float>(ifj_puppi_sdsubjet1_csv_ ,sdpuppisubjetsCSV[0]);
      data.fillMulti<float>(ifj_puppi_sdsubjet2_pt_  ,-9.);
      data.fillMulti<float>(ifj_puppi_sdsubjet2_eta_ ,-9.);
      data.fillMulti<float>(ifj_puppi_sdsubjet2_phi_ ,-9.);
      data.fillMulti<float>(ifj_puppi_sdsubjet2_mass_,-9.);
      data.fillMulti<float>(ifj_puppi_sdsubjet2_csv_ ,-9.);
    }

    if (sdpuppisubjetsp4.size()>=2) {
      data.fillMulti<float>(ifj_puppi_sdsubjet1_pt_  ,sdpuppisubjetsp4[0].Pt());
      data.fillMulti<float>(ifj_puppi_sdsubjet1_eta_ ,sdpuppisubjetsp4[0].Eta());
      data.fillMulti<float>(ifj_puppi_sdsubjet1_phi_ ,sdpuppisubjetsp4[0].Phi());
      data.fillMulti<float>(ifj_puppi_sdsubjet1_mass_,sdpuppisubjetsp4[0].M());
      data.fillMulti<float>(ifj_puppi_sdsubjet1_csv_ ,sdpuppisubjetsCSV[0]);
      data.fillMulti<float>(ifj_puppi_sdsubjet2_pt_  ,sdpuppisubjetsp4[1].Pt());
      data.fillMulti<float>(ifj_puppi_sdsubjet2_eta_ ,sdpuppisubjetsp4[1].Eta());
      data.fillMulti<float>(ifj_puppi_sdsubjet2_phi_ ,sdpuppisubjetsp4[1].Phi());
      data.fillMulti<float>(ifj_puppi_sdsubjet2_mass_,sdpuppisubjetsp4[1].M());
      data.fillMulti<float>(ifj_puppi_sdsubjet2_csv_ ,sdpuppisubjetsCSV[1]);
    }    

    // calculate the puppi softdrop mass
    if (sdpuppisubjetsp4.size()==0) { data.fillMulti<float>(ifj_puppi_sdmass_, 0.);                      }
    if (sdpuppisubjetsp4.size()==1) { data.fillMulti<float>(ifj_puppi_sdmass_, sdpuppisubjetsp4[0].M()); }
    if (sdpuppisubjetsp4.size()>=2) { 
      float sdpuppimass_ = (sdpuppisubjetsp4[0]+sdpuppisubjetsp4[1]).M(); 
      data.fillMulti<float>(ifj_puppi_sdmass_, sdpuppimass_); 
    }

    isFilled_ = true;

  }

}

