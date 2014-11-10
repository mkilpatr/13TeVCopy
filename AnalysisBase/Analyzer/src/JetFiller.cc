//--------------------------------------------------------------------------------------------------
// 
// JetFiller
// 
// Class to fill some basic jet information for testing. To be developed as needed.
// 
// JetFiller.cc created on Thu Aug 14 11:10:55 CEST 2014 
// 
//--------------------------------------------------------------------------------------------------

#include "AnalysisBase/Analyzer/interface/JetFiller.h"
#include "AnalysisTools/Utilities/interface/JetFlavorMatching.h"
#include "AnalysisTools/Utilities/interface/Types.h"

#include "AnalysisBase/Analyzer/interface/EventInfoFiller.h"
#include "AnalysisTools/JetShapeVariables/interface/QuarkGluonTagInterface.h"
#include "AnalysisTools/JetShapeVariables/interface/QuarkGluonTaggingVariables.h"

using namespace ucsbsusy;

#define TAGGABLE_TYPE_HACK
#define REDEFINED_GENJET_HACK

//--------------------------------------------------------------------------------------------------
JetFiller::JetFiller(const edm::ParameterSet &cfg, const bool isMC, const EventInfoFiller * evtInfoFiller) :
  fillGenInfo_     (isMC && cfg.getUntrackedParameter<bool>("fillJetGenInfo",false)),
  fillJetShapeInfo_(cfg.getUntrackedParameter<bool>("fillJetShapeInfo",false)),
  jetTag_          (cfg.getParameter<edm::InputTag>("jets")),
  reGenJetTag_     (fillGenInfo_      ? cfg.getParameter<edm::InputTag>("reGenJets")  : edm::InputTag()),
  stdGenJetTag_    (fillGenInfo_      ? cfg.getParameter<edm::InputTag>("stdGenJets")  : edm::InputTag()),
  genParticleTag_  (fillGenInfo_      ? cfg.getParameter<edm::InputTag>("genParticles") : edm::InputTag()),
  jptMin_          (cfg.getUntrackedParameter<double>("minJetPt")),
  jetsName_        (cfg.getUntrackedParameter<string>("jetsType")),
  evtInfofiller_   (evtInfoFiller),
  qglInterface_    (fillJetShapeInfo_ ? new QuarkGluonTagInterface : 0),
  qgTaggingVar_    (fillJetShapeInfo_ ? new QuarkGluonTaggingVariables : 0),
  jets_            (0),
  reGenJets_       (0),
  stdGenJets_      (0)
{}

//--------------------------------------------------------------------------------------------------
reco::GenJetRef JetFiller::getReGenJet(const pat::Jet& jet) const
{
  if (!reGenJets_.isValid())
    throw cms::Exception("JetFiller.getReGenJet()", "genJets have not been loaded.");

  const reco::CandidatePtr&   genPtr  = jet.userCand(REGENJET);
  if (genPtr.id() != reGenJets_.id())
    throw cms::Exception("JetFiller.getReGenJet()", "Inconsistent reGenJets collection with pat::Jet::userCand(REGENJET).");

  return reco::GenJetRef(reGenJets_, genPtr.key());
}

//--------------------------------------------------------------------------------------------------
reco::GenJetRef JetFiller::getStdGenJet(const pat::Jet& jet) const
{
  return jet.genJetFwdRef().backRef();
}

//--------------------------------------------------------------------------------------------------
void JetFiller::book(TreeWriter& tW)
{
  tW.book((jetsName_+"_jet_pt").c_str(), jetpt_);
  tW.book((jetsName_+"_jet_eta").c_str(), jeteta_);
  tW.book((jetsName_+"_jet_phi").c_str(), jetphi_);
  tW.book((jetsName_+"_jet_mass").c_str(), jetmass_);
  tW.book((jetsName_+"_jet_ptraw").c_str(), jetptraw_);
  tW.book((jetsName_+"_jet_puId").c_str(), jetpuId_);
  tW.book((jetsName_+"_jet_csv").c_str(), jetcsv_);
  tW.book((jetsName_+"_jet_flavor").c_str(), jetflavor_);
  if(fillGenInfo_) {
    tW.book((jetsName_+"_matchedgenjet_pt").c_str(), genjetpt_);
    tW.book((jetsName_+"_matchedgenjet_eta").c_str(), genjeteta_);
    tW.book((jetsName_+"_matchedgenjet_phi").c_str(), genjetphi_);
    tW.book((jetsName_+"_matchedgenjet_mass").c_str(), genjetmass_);
    tW.book((jetsName_+"_matchedgenjet_flavor").c_str(), genjetflavor_);
  }
  if(fillJetShapeInfo_) {
    tW.book((jetsName_+"_jet_betaStar").c_str(),jetbetaStar_);
    tW.book((jetsName_+"_jet_qgl").c_str(),     jetqgl_);
    tW.book((jetsName_+"_jet_ptD").c_str(),     jetptD_);
    tW.book((jetsName_+"_jet_axis1").c_str(),   jetaxis1_);
    tW.book((jetsName_+"_jet_axis2").c_str(),   jetaxis2_);
    tW.book((jetsName_+"_jet_jetMult").c_str(), jetMult_);
    if(fillGenInfo_) {
      tW.book((jetsName_+"_matchedgenjet_ptD").c_str(),     genjetptD_);
      tW.book((jetsName_+"_matchedgenjet_axis1").c_str(),   genjetaxis1_);
      tW.book((jetsName_+"_matchedgenjet_axis2").c_str(),   genjetaxis2_);
      tW.book((jetsName_+"_matchedgenjet_jetMult").c_str(), genjetMult_);
    }
  }
}

//--------------------------------------------------------------------------------------------------
void JetFiller::reset()
{
  isLoaded_ = false;
  isFilled_ = false;

  jetpt_.resize(0);
  jeteta_.resize(0);
  jetphi_.resize(0);
  jetmass_.resize(0);
  jetptraw_.resize(0);
  jetpuId_.resize(0);
  jetcsv_.resize(0);
  jetflavor_.resize(0);
  if(fillGenInfo_) {
    genjetpt_.resize(0);
    genjeteta_.resize(0);
    genjetphi_.resize(0);
    genjetmass_.resize(0);
    genjetflavor_.resize(0);
  }
  if(fillJetShapeInfo_) {
    jetbetaStar_.resize(0);
    jetqgl_     .resize(0);
    jetptD_     .resize(0);
    jetaxis1_   .resize(0);
    jetaxis2_   .resize(0);
    jetMult_    .resize(0);
    if(fillGenInfo_){
      genjetptD_  .resize(0);
      genjetaxis1_.resize(0);
      genjetaxis2_.resize(0);
      genjetMult_ .resize(0);
    }
  }
}

//--------------------------------------------------------------------------------------------------
void JetFiller::reserve()
{
const int nJ = PhysicsUtilities::countObjects(*jets_.product(),jptMin_);
  jetpt_.reserve(nJ);
  jeteta_.reserve(nJ);
  jetphi_.reserve(nJ);
  jetmass_.reserve(nJ);
  jetptraw_.reserve(nJ);
  jetpuId_.reserve(nJ);
  jetcsv_.reserve(nJ);
  jetflavor_.reserve(nJ);
  if(fillGenInfo_) {
    genjetpt_.reserve(nJ);
    genjeteta_.reserve(nJ);
    genjetphi_.reserve(nJ);
    genjetmass_.reserve(nJ);
    genjetflavor_.reserve(nJ);
  }
  if(fillJetShapeInfo_) {
    jetbetaStar_.reserve(nJ);
    jetqgl_     .reserve(nJ);
    jetptD_     .reserve(nJ);
    jetaxis1_   .reserve(nJ);
    jetaxis2_   .reserve(nJ);
    jetMult_    .reserve(nJ);
    if(fillGenInfo_){
      genjetptD_  .reserve(nJ);
      genjetaxis1_.reserve(nJ);
      genjetaxis2_.reserve(nJ);
      genjetMult_ .reserve(nJ);
    }
  }
}

//--------------------------------------------------------------------------------------------------
void JetFiller::load(edm::Event& iEvent)
{
  reset();
  FileUtilities::enforceGet(iEvent,jetTag_,jets_,true);

  if(fillGenInfo_) {
    FileUtilities::enforceGet(iEvent,reGenJetTag_,reGenJets_,true);
    FileUtilities::enforceGet(iEvent,stdGenJetTag_,stdGenJets_,true);
//    edm::Handle<reco::GenParticleCollection> genParticles_;
//    FileUtilities::enforceGet(iEvent,genParticleTag_,genParticles_,true);

#ifndef TAGGABLE_TYPE_HACK
    std::vector<HadronDecay> bHadrons = JetFlavorMatching::getBHadronDecays(genParticles_);
    JetFlavorMatching::storeBHadronInfo(*jets_,*reGenJets_,bHadrons);
#endif
  }

  isLoaded_ = true;

}

//--------------------------------------------------------------------------------------------------
void JetFiller::fill(TreeWriter& tW, const int& numAnalyzed)
{
  reserve();
  for (const pat::Jet &j : *jets_) {

#ifdef REDEFINED_GENJET_HACK
    const reco::GenJetRef gJ = getStdGenJet(j);
    if(j.pt() < jptMin_ && (fillGenInfo_ ? (gJ.isNonnull() ? gJ->pt() : 0) < jptMin_ : true)) continue;
#else
    const reco::GenJet * gJ = fillGenInfo_ ? &(*getReGenJet(j)) : 0;
    if(j.pt() < jptMin_ && (fillGenInfo_ ? gJ->pt() : 0) < jptMin_ : true) continue;
#endif

    jetpt_.push_back(j.pt());
    jeteta_.push_back(j.eta());
    jetphi_.push_back(j.phi());
    jetmass_.push_back(j.mass());
    jetptraw_.push_back(j.pt()*j.jecFactor("Uncorrected"));
    jetpuId_.push_back(j.userFloat("pileupJetId:fullDiscriminant"));
    jetcsv_.push_back(j.bDiscriminator("combinedInclusiveSecondaryVertexBJetTags"));

    int index = jetpt_.size()-1;

    if(fillGenInfo_) {
      jetflavor_.push_back(j.partonFlavour());
#ifdef REDEFINED_GENJET_HACK
      if(gJ.isNonnull()) {
//#ifdef TAGGABLE_TYPE_HACK
//	jetflavor_.push_back(JetFlavorMatching::getPATTaggableType(j));
//#else
//	jetflavor_.push_back(JetFlavorMatching::getTaggableType(j));
//#endif
	genjetpt_.push_back(gJ->pt());
	genjeteta_.push_back(gJ->eta());
	genjetphi_.push_back(gJ->phi());
	genjetmass_.push_back(gJ->mass());
	genjetflavor_.push_back(jetflavor_[index]);
      } else {
//	jetflavor_.push_back(numTaggableTypes);
	genjetpt_.push_back(-99.);
	genjeteta_.push_back(-99.);
	genjetphi_.push_back(-99.);
	genjetmass_.push_back(-99.);
	genjetflavor_.push_back(-99.);
      }
#else
//#ifdef TAGGABLE_TYPE_HACK
//      jetflavor_.push_back(JetFlavorMatching::getPATTaggableType(j));
//#else
//      jetflavor_.push_back(JetFlavorMatching::getTaggableType(j));
//#endif
      genjetpt_.push_back(gJ->pt());
      genjeteta_.push_back(gJ->eta());
      genjetphi_.push_back(gJ->phi());
      genjetmass_.push_back(gJ->mass());
      genjetflavor_.push_back(jetflavor_[index]);
#endif
    }
    if(fillJetShapeInfo_){
      assert(evtInfofiller_->isLoaded());
      jetqgl_.push_back(qglInterface_->getDiscriminator(j,*evtInfofiller_->rhoHandle_));
      qgTaggingVar_->compute(&j);
      jetbetaStar_.push_back(qgTaggingVar_->getBetaStar(&j,*evtInfofiller_->vertices_,evtInfofiller_->primaryVertexIndex_));
      jetptD_     .push_back(qgTaggingVar_->getPtD());
      jetaxis1_   .push_back(qgTaggingVar_->getAxis1());
      jetaxis2_   .push_back(qgTaggingVar_->getAxis2());
      jetMult_    .push_back(qgTaggingVar_->getTotalMult());

      if(fillGenInfo_){
        if(gJ.isNonnull()) {
          qgTaggingVar_->compute(gJ.get());
          genjetptD_     .push_back(qgTaggingVar_->getPtD());
          genjetaxis1_   .push_back(qgTaggingVar_->getAxis1());
          genjetaxis2_   .push_back(qgTaggingVar_->getAxis2());
          genjetMult_    .push_back(qgTaggingVar_->getTotalMult());
        } else {
          genjetptD_     .push_back(0);
          genjetaxis1_   .push_back(0);
          genjetaxis2_   .push_back(0);
          genjetMult_    .push_back(0);
        }
      }
    }
  }
  isFilled_ = true;
}

//--------------------------------------------------------------------------------------------------
const std::string   JetFiller::REGENJET           = "GenPtr";
