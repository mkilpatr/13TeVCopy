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

using namespace ucsbsusy;

//--------------------------------------------------------------------------------------------------
JetFiller::JetFiller(const edm::ParameterSet &cfg) :
    jetTag_(cfg.getParameter<edm::InputTag>("jets")),
    jptMin_(cfg.getUntrackedParameter<double>("minjetpt")),
    maxNjets_(cfg.getUntrackedParameter<unsigned int>("maxnjets")),
    obj(0),
    jets_(0)
{
  ucsbsusy::Jet::Class()->IgnoreTObjectStreamer();
}

//--------------------------------------------------------------------------------------------------
JetFiller::~JetFiller() {delete obj;}

//--------------------------------------------------------------------------------------------------
void JetFiller::load(edm::Event& iEvent, bool storeOnlyPtr ){
  enforceGet(iEvent,jetTag_,jets_,true);

  if(storeOnlyPtr) return;
  if(!obj) obj = new JetCollection;
  else obj->clear();
  obj->reserve(jets_->size());

  unsigned int njets = 0;
  for(const pat::Jet &j : *jets_) {
    if(j.pt() < jptMin_) continue;
    if(njets >= maxNjets_) break;

    obj->emplace_back();
    Jet& jet = obj->back();

    jet.setP4(j.polarP4());
    jet.setJECUncorrection(j.jecFactor("Uncorrected"));
    jet.setCSV(j.bDiscriminator("combinedSecondaryVertexBJetTags"));
    jet.setJetArea(j.jetArea());
    jet.setPUJetId(j.userFloat("pileupJetId:fullDiscriminant"));
    jet.setNChargedHadron(j.chargedHadronMultiplicity());
    jet.setNNeutralHadron(j.neutralHadronMultiplicity());
    jet.setNElectron(j.electronMultiplicity());
    jet.setNMuon(j.muonMultiplicity());
    jet.setNPhoton(j.photonMultiplicity());
    jet.setNeutralHadFrac(j.neutralHadronEnergyFraction());
    jet.setChargedHadFrac(j.chargedHadronEnergyFraction());
    jet.setNeutralEMFrac(j.neutralEmEnergyFraction());
    jet.setChargedEMFrac(j.chargedEmEnergyFraction());
    njets++;
  }

  std::sort(obj->begin(),obj->end(), greaterPT<Jet>());
}

//--------------------------------------------------------------------------------------------------
void JetFiller::fill(Planter& plant, int& bookMark)
{
  plant.fill(*obj,"Jets");
  bookMark++;
}
