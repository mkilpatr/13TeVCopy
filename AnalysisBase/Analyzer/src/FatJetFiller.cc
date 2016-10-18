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
#include "DataFormats/Math/interface/deltaR.h"

#include "AnalysisTools/JetShapeVariables/interface/QuarkGluonTaggingVariables.h"

using namespace ucsbsusy;

//--------------------------------------------------------------------------------------------------
FatJetFiller::FatJetFiller(const edm::ParameterSet& cfg, edm::ConsumesCollector && cc, const int options, const string branchName) :
  BaseFiller(options, branchName),
  fatJetToken_(cc.consumes<pat::JetCollection>(cfg.getParameter<edm::InputTag>("fatJets"))),
  sdSubjetToken_(cc.consumes<pat::JetCollection>(cfg.getParameter<edm::InputTag>("sdSubjets"))),
  puppiSubjetToken_(cc.consumes<pat::JetCollection>(cfg.getParameter<edm::InputTag>("puppiSubjets"))),
  puRemoval_(cfg.getUntrackedParameter<string>("puRemoval")),
  qgTaggingVar_(new QuarkGluonTaggingVariables)
{

  ifj_rawmass_       = data.addMulti<float>(branchName_,"fatjet_rawmass",0);
  ifj_prunedmass_    = data.addMulti<float>(branchName_,"fatjet_prunedmass",0);
  ifj_softdropmass_  = data.addMulti<float>(branchName_,"fatjet_softdropmass",0);
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
  ifj_puppi_sdmass_           = data.addMulti<float> (branchName_,"fatjet_puppi_sdmass",0);

  if(options_ & LOADSUBJETCTAG){
    ifj_sdsubjet1_cmva_   = data.addMulti<float> (branchName_,"fatjet_sdsubjet1_cmva",0);
    ifj_sdsubjet1_cvsl_   = data.addMulti<float> (branchName_,"fatjet_sdsubjet1_cvsl",0);
    ifj_sdsubjet1_cvsb_   = data.addMulti<float> (branchName_,"fatjet_sdsubjet1_cvsb",0);
    ifj_sdsubjet2_cmva_   = data.addMulti<float> (branchName_,"fatjet_sdsubjet2_cmva",0);
    ifj_sdsubjet2_cvsl_   = data.addMulti<float> (branchName_,"fatjet_sdsubjet2_cvsl",0);
    ifj_sdsubjet2_cvsb_   = data.addMulti<float> (branchName_,"fatjet_sdsubjet2_cvsb",0);

    ifj_puppi_sdsubjet1_cmva_   = data.addMulti<float> (branchName_,"fatjet_puppi_sdsubjet1_cmva",0);
    ifj_puppi_sdsubjet1_cvsl_   = data.addMulti<float> (branchName_,"fatjet_puppi_sdsubjet1_cvsl",0);
    ifj_puppi_sdsubjet1_cvsb_   = data.addMulti<float> (branchName_,"fatjet_puppi_sdsubjet1_cvsb",0);
    ifj_puppi_sdsubjet2_cmva_   = data.addMulti<float> (branchName_,"fatjet_puppi_sdsubjet2_cmva",0);
    ifj_puppi_sdsubjet2_cvsl_   = data.addMulti<float> (branchName_,"fatjet_puppi_sdsubjet2_cvsl",0);
    ifj_puppi_sdsubjet2_cvsb_   = data.addMulti<float> (branchName_,"fatjet_puppi_sdsubjet2_cvsb",0);
  }

  if(options_ & LOADJETSHAPE){
    ifj_sdsubjet1_ptD_        = data.addMulti<float> (branchName_,"fatjet_sdsubjet1_ptD",0);
    ifj_sdsubjet1_axis1_      = data.addMulti<float> (branchName_,"fatjet_sdsubjet1_axis1",0);
    ifj_sdsubjet1_axis2_      = data.addMulti<float> (branchName_,"fatjet_sdsubjet1_axis2",0);
    ifj_sdsubjet1_jetMult_    = data.addMulti<int>   (branchName_,"fatjet_sdsubjet1_jetMult",0);

    ifj_sdsubjet2_ptD_        = data.addMulti<float> (branchName_,"fatjet_sdsubjet2_ptD",0);
    ifj_sdsubjet2_axis1_      = data.addMulti<float> (branchName_,"fatjet_sdsubjet2_axis1",0);
    ifj_sdsubjet2_axis2_      = data.addMulti<float> (branchName_,"fatjet_sdsubjet2_axis2",0);
    ifj_sdsubjet2_jetMult_    = data.addMulti<int>   (branchName_,"fatjet_sdsubjet2_jetMult",0);
  }

}

//--------------------------------------------------------------------------------------------------
void FatJetFiller::load(const edm::Event& iEvent, const edm::EventSetup &iSetup)
{
  reset();
  iEvent.getByToken(fatJetToken_,fatJets_);
  if(options_ & LOADSUBJETCTAG){
    iEvent.getByToken(sdSubjetToken_,sdCollectionSubjets_);
    iEvent.getByToken(puppiSubjetToken_,puppiCollectionSubjets_);
  }
  isLoaded_ = true;
}

//--------------------------------------------------------------------------------------------------
void FatJetFiller::fill()
{

  for (const pat::Jet &fatjet : *fatJets_) {

    if (fatjet.pt()<=50.) { continue; }

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
    // soft-drop gives up to two subjets
    const auto &sdsubjets = fatjet.subjets("SoftDrop");
    data.fillMulti<int>  (ifj_nsoftdropsubjets_ , sdsubjets.size());
    data.fillMulti<float>(ifj_sdsubjet1_pt_  ,sdsubjets.size()>0 ? sdsubjets[0]->pt()  : -9);
    data.fillMulti<float>(ifj_sdsubjet1_eta_ ,sdsubjets.size()>0 ? sdsubjets[0]->eta() : -9);
    data.fillMulti<float>(ifj_sdsubjet1_phi_ ,sdsubjets.size()>0 ? sdsubjets[0]->phi() : -9);
    data.fillMulti<float>(ifj_sdsubjet1_mass_,sdsubjets.size()>0 ? sdsubjets[0]->mass(): -9);
    data.fillMulti<float>(ifj_sdsubjet1_csv_ ,sdsubjets.size()>0 ? sdsubjets[0]->bDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags") : -9);
    data.fillMulti<float>(ifj_sdsubjet2_pt_  ,sdsubjets.size()>1 ? sdsubjets[1]->pt()  : -9);
    data.fillMulti<float>(ifj_sdsubjet2_eta_ ,sdsubjets.size()>1 ? sdsubjets[1]->eta() : -9);
    data.fillMulti<float>(ifj_sdsubjet2_phi_ ,sdsubjets.size()>1 ? sdsubjets[1]->phi() : -9);
    data.fillMulti<float>(ifj_sdsubjet2_mass_,sdsubjets.size()>1 ? sdsubjets[1]->mass(): -9);
    data.fillMulti<float>(ifj_sdsubjet2_csv_ ,sdsubjets.size()>1 ? sdsubjets[1]->bDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags") : -9);

    if (options_ & LOADJETSHAPE){
      if (sdsubjets.size()>0) qgTaggingVar_->compute(&(*sdsubjets[0]),true);
      data.fillMulti<float>(ifj_sdsubjet1_ptD_    ,sdsubjets.size()>0 ? qgTaggingVar_->getPtD()       : -9);
      data.fillMulti<float>(ifj_sdsubjet1_axis1_  ,sdsubjets.size()>0 ? qgTaggingVar_->getAxis1()     : -9);
      data.fillMulti<float>(ifj_sdsubjet1_axis2_  ,sdsubjets.size()>0 ? qgTaggingVar_->getAxis2()     : -9);
      data.fillMulti<int>  (ifj_sdsubjet1_jetMult_,sdsubjets.size()>0 ? qgTaggingVar_->getTotalMult() : -9);

      if (sdsubjets.size()>1) qgTaggingVar_->compute(&(*sdsubjets[1]),true);
      data.fillMulti<float>(ifj_sdsubjet2_ptD_    ,sdsubjets.size()>1 ? qgTaggingVar_->getPtD()       : -9);
      data.fillMulti<float>(ifj_sdsubjet2_axis1_  ,sdsubjets.size()>1 ? qgTaggingVar_->getAxis1()     : -9);
      data.fillMulti<float>(ifj_sdsubjet2_axis2_  ,sdsubjets.size()>1 ? qgTaggingVar_->getAxis2()     : -9);
      data.fillMulti<int>  (ifj_sdsubjet2_jetMult_,sdsubjets.size()>1 ? qgTaggingVar_->getTotalMult() : -9);
    }

    if (options_ & LOADSUBJETCTAG){
      pat::JetPtrCollection subjets;
      for (const pat::Jet &sj : *sdCollectionSubjets_) {
        // sdCollectionSubjets_ stores the soft-drop AK8 jets, with the actual subjets stored as daughters
        // PhysicsTools/PatAlgos/python/slimming/applySubstructure_cff.py
        // PhysicsTools/PatUtils/plugins/JetSubstructurePacker.cc
        if (reco::deltaR(sj, fatjet) < 0.8) {
          for ( size_t ida = 0; ida < sj.numberOfDaughters(); ++ida ) {
            auto candPtr =  sj.daughterPtr(ida);
            subjets.push_back( edm::Ptr<pat::Jet>(candPtr) );
          }
          break;
        }
      }

      data.fillMulti<float>(ifj_sdsubjet1_cmva_,subjets.size()>0 ? subjets[0]->bDiscriminator("pfCombinedMVAV2BJetTags") : -9);
      data.fillMulti<float>(ifj_sdsubjet1_cvsl_,subjets.size()>0 ? subjets[0]->bDiscriminator("pfCombinedCvsLJetTags")   : -9);
      data.fillMulti<float>(ifj_sdsubjet1_cvsb_,subjets.size()>0 ? subjets[0]->bDiscriminator("pfCombinedCvsBJetTags")   : -9);
      data.fillMulti<float>(ifj_sdsubjet2_cmva_,subjets.size()>1 ? subjets[1]->bDiscriminator("pfCombinedMVAV2BJetTags") : -9);
      data.fillMulti<float>(ifj_sdsubjet2_cvsl_,subjets.size()>1 ? subjets[1]->bDiscriminator("pfCombinedCvsLJetTags")   : -9);
      data.fillMulti<float>(ifj_sdsubjet2_cvsb_,subjets.size()>1 ? subjets[1]->bDiscriminator("pfCombinedCvsBJetTags")   : -9);
    }


    // for puppi
    const auto &sdpuppisubjets = fatjet.subjets("SoftDropPuppi");
    data.fillMulti<int>  (ifj_puppi_nsoftdropsubjets_ , sdpuppisubjets.size());
    data.fillMulti<float>(ifj_puppi_sdsubjet1_pt_  ,sdpuppisubjets.size()>0 ? sdpuppisubjets[0]->pt()   : -9);
    data.fillMulti<float>(ifj_puppi_sdsubjet1_eta_ ,sdpuppisubjets.size()>0 ? sdpuppisubjets[0]->eta()  : -9);
    data.fillMulti<float>(ifj_puppi_sdsubjet1_phi_ ,sdpuppisubjets.size()>0 ? sdpuppisubjets[0]->phi()  : -9);
    data.fillMulti<float>(ifj_puppi_sdsubjet1_mass_,sdpuppisubjets.size()>0 ? sdpuppisubjets[0]->mass() : -9);
    data.fillMulti<float>(ifj_puppi_sdsubjet1_csv_ ,sdpuppisubjets.size()>0 ? sdpuppisubjets[0]->bDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags") : -9);
    data.fillMulti<float>(ifj_puppi_sdsubjet2_pt_  ,sdpuppisubjets.size()>1 ? sdpuppisubjets[1]->pt()   : -9);
    data.fillMulti<float>(ifj_puppi_sdsubjet2_eta_ ,sdpuppisubjets.size()>1 ? sdpuppisubjets[1]->eta()  : -9);
    data.fillMulti<float>(ifj_puppi_sdsubjet2_phi_ ,sdpuppisubjets.size()>1 ? sdpuppisubjets[1]->phi()  : -9);
    data.fillMulti<float>(ifj_puppi_sdsubjet2_mass_,sdpuppisubjets.size()>1 ? sdpuppisubjets[1]->mass() : -9);
    data.fillMulti<float>(ifj_puppi_sdsubjet2_csv_ ,sdpuppisubjets.size()>1 ? sdpuppisubjets[1]->bDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags") : -9);

    // calculate the puppi softdrop mass
    if (sdpuppisubjets.size()==0) { data.fillMulti<float>(ifj_puppi_sdmass_, 0.);                      }
    if (sdpuppisubjets.size()==1) { data.fillMulti<float>(ifj_puppi_sdmass_, sdpuppisubjets[0]->mass()); }
    if (sdpuppisubjets.size()>=2) {
      float sdpuppimass_ = (sdpuppisubjets[0]->p4()+sdpuppisubjets[1]->p4()).mass();
      data.fillMulti<float>(ifj_puppi_sdmass_, sdpuppimass_);
    }

    if(options_ & LOADSUBJETCTAG){
      pat::JetPtrCollection puppisubjets;
      for (const pat::Jet &sj : *puppiCollectionSubjets_) {
        // puppiCollectionSubjets_ stores the puppi soft-drop AK8 jets, with the actual subjets stored as daughters
        if (reco::deltaR(sj, fatjet) < 0.8) {
          for ( size_t ida = 0; ida < sj.numberOfDaughters(); ++ida ) {
            auto candPtr =  sj.daughterPtr(ida);
            puppisubjets.push_back( edm::Ptr<pat::Jet>(candPtr) );
          }
          break;
        }
      }

      data.fillMulti<float>(ifj_puppi_sdsubjet1_cmva_,puppisubjets.size()>0 ? puppisubjets[0]->bDiscriminator("pfCombinedMVAV2BJetTags") : -9);
      data.fillMulti<float>(ifj_puppi_sdsubjet1_cvsl_,puppisubjets.size()>0 ? puppisubjets[0]->bDiscriminator("pfCombinedCvsLJetTags")   : -9);
      data.fillMulti<float>(ifj_puppi_sdsubjet1_cvsb_,puppisubjets.size()>0 ? puppisubjets[0]->bDiscriminator("pfCombinedCvsBJetTags")   : -9);
      data.fillMulti<float>(ifj_puppi_sdsubjet2_cmva_,puppisubjets.size()>1 ? puppisubjets[1]->bDiscriminator("pfCombinedMVAV2BJetTags") : -9);
      data.fillMulti<float>(ifj_puppi_sdsubjet2_cvsl_,puppisubjets.size()>1 ? puppisubjets[1]->bDiscriminator("pfCombinedCvsLJetTags")   : -9);
      data.fillMulti<float>(ifj_puppi_sdsubjet2_cvsb_,puppisubjets.size()>1 ? puppisubjets[1]->bDiscriminator("pfCombinedCvsBJetTags")   : -9);
    }

    isFilled_ = true;

  }

}

