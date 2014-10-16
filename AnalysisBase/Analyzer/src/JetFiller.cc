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

using namespace ucsbsusy;

#define TAGGABLE_TYPE_HACK
#define REDEFINED_GENJET_HACK

//--------------------------------------------------------------------------------------------------
JetFiller::JetFiller(const edm::ParameterSet &cfg) :
    jetTag_(cfg.getParameter<edm::InputTag>("jets")),
    reGenJetTag_(cfg.getParameter<edm::InputTag>("reGenJets")),
    stdGenJetTag_(cfg.getParameter<edm::InputTag>("stdGenJets")),
    genParticleTag_(cfg.getParameter<edm::InputTag>("genParticles")),
    jptMin_(cfg.getUntrackedParameter<double>("minJetPt")),
    fillGenInfo_(false),
    jets_(0),
    reGenJets_(0),
    stdGenJets_(0)
{

}

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
  tW.book("jetpt", jetpt_);
  tW.book("jeteta", jeteta_);
  tW.book("jetphi", jetphi_);
  tW.book("jetmass", jetmass_);
  tW.book("jetptraw", jetptraw_);
  tW.book("jetpuId", jetpuId_);
  tW.book("jetcsv", jetcsv_);
}

//--------------------------------------------------------------------------------------------------
void JetFiller::reset()
{
  jetpt_.resize(0);
  jeteta_.resize(0);
  jetphi_.resize(0);
  jetmass_.resize(0);
  jetptraw_.resize(0);
  jetpuId_.resize(0);
  jetcsv_.resize(0);
}

//--------------------------------------------------------------------------------------------------
void JetFiller::load(edm::Event& iEvent, bool storeOnlyPtr, bool isMC )
{
  reset();
  enforceGet(iEvent,jetTag_,jets_,true);

  if(isMC){
    fillGenInfo_ = true;
    enforceGet(iEvent,reGenJetTag_,reGenJets_,true);
    enforceGet(iEvent,stdGenJetTag_,stdGenJets_,true);
    edm::Handle<reco::GenParticleCollection> genParticles_;
    enforceGet(iEvent,genParticleTag_,genParticles_,true);

#ifndef TAGGABLE_TYPE_HACK
    std::vector<HadronDecay> bHadrons = JetFlavorMatching::getBHadronDecays(genParticles_);
    JetFlavorMatching::storeBHadronInfo(*jets_,*reGenJets_,bHadrons);
#endif
  }

  if(storeOnlyPtr) return;

  recoJets    .clear();
  genJets .clear();
  v_csv   .clear();
  v_flavor.clear();

  recoJets    .reserve(jets_->size());
  v_csv   .reserve(jets_->size());

  if(isMC) {
    genJets .reserve(jets_->size());
    v_flavor.reserve(jets_->size());
  }

  for(const pat::Jet &j : *jets_) {

#ifdef REDEFINED_GENJET_HACK
    const reco::GenJetRef gJ = getStdGenJet(j);
    if(j.pt() < jptMin_ && (fillGenInfo_ ? (gJ.isNonnull() ? gJ->pt() : 0) < jptMin_ : true)) continue;
#else
    const reco::GenJet * gJ = fillGenInfo_ ? &(*getReGenJet(j)) : 0;
    if(j.pt() < jptMin_ && (fillGenInfo_ ? gJ->pt() : 0) < jptMin_ : true) continue;
#endif

    int index = recoJets.size();

    GenJetF * genJet = 0;
    if(fillGenInfo_) {
#ifdef REDEFINED_GENJET_HACK
      if(gJ.isNonnull()){
#ifdef TAGGABLE_TYPE_HACK
	v_flavor.push_back(JetFlavorMatching::getPATTaggableType(j));
#else
	v_flavor.push_back(JetFlavorMatching::getTaggableType(j));
#endif
	genJets.emplace_back( gJ->polarP4() ,index,&v_flavor[index]);
      } else{
	v_flavor.push_back(JetFlavorMatching::numTaggableTypes );
	MomentumF nullVect;
	genJets.emplace_back(  nullVect.p4() , index,&v_flavor[index]);
      }
#else
#ifdef TAGGABLE_TYPE_HACK
      v_flavor.push_back(JetFlavorMatching::getPATTaggableType(j));
#else
      v_flavor.push_back(JetFlavorMatching::getTaggableType(j));
#endif
      genJets.emplace_back( gJ->polarP4() ,index,&v_flavor[index]);
#endif
      genJet = &genJets[index];
    }

    v_csv.push_back(j.bDiscriminator("combinedSecondaryVertexBJetTags"));
    recoJets.emplace_back(j.polarP4(),index,&v_csv[index],genJet);
  }

  std::sort(recoJets.begin(),recoJets.end(), greaterPT<RecoJetF>());
}

//--------------------------------------------------------------------------------------------------
void JetFiller::fill(TreeWriter& tW, const int& numAnalyzed)
{
  for (const pat::Jet &j : *jets_) {
    jetpt_.push_back(j.pt());
    jeteta_.push_back(j.eta());
    jetphi_.push_back(j.phi());
    jetmass_.push_back(j.mass());
    jetptraw_.push_back(j.pt()*j.jecFactor("Uncorrected"));
    jetpuId_.push_back(j.userFloat("pileupJetId:fullDiscriminant"));
    jetcsv_.push_back(j.bDiscriminator("combinedInclusiveSecondaryVertexBJetTags"));
  }
}

//--------------------------------------------------------------------------------------------------
const std::string   JetFiller::REGENJET           = "GenPtr";
