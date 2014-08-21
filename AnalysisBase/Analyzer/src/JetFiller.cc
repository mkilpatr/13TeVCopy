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
  BaseAnalyzer(cfg),
  jptMin_(cfg.getUntrackedParameter<double>("minjetpt")),
  maxNjets_(cfg.getUntrackedParameter<unsigned int>("maxnjets")),
  jetTag_(cfg.getParameter<edm::InputTag>("jets")),
  jetCol_(new JetCollection())
{
  // Constructor

  ucsbsusy::Jet::Class()->IgnoreTObjectStreamer();

}

//--------------------------------------------------------------------------------------------------
JetFiller::~JetFiller()
{
  // Destructor

  delete jetCol_;

}

//--------------------------------------------------------------------------------------------------
void JetFiller::book(TTree &t)
{
  // Add branch for this filler

  t.Branch("Jets", &jetCol_);

}

//--------------------------------------------------------------------------------------------------
void JetFiller::fill(const edm::Event &event)
{
  // Called for each event

  jetCol_->clear();

  edm::Handle<pat::JetCollection> jets;
  event.getByLabel(jetTag_, jets);

  unsigned int njets = 0;

  for(const pat::Jet &j : *jets) {
    if(j.pt() < jptMin_) continue;
    if(njets >= maxNjets_) continue;
    ucsbsusy::Jet *jet = new ucsbsusy::Jet();
    jet->setPtEtaPhiM(j.pt(), j.eta(), j.phi(), j.mass());
    jet->setPtRaw(j.pt()*j.jecFactor("Uncorrected"));
    jet->setCSV(j.bDiscriminator("combinedSecondaryVertexBJetTags"));
    jet->setJetArea(j.jetArea());
    jet->setPUJetId(j.userFloat("pileupJetId:fullDiscriminant"));
    jet->setNChargedHadron(j.chargedHadronMultiplicity());
    jet->setNNeutralHadron(j.neutralHadronMultiplicity());
    jet->setNElectron(j.electronMultiplicity());
    jet->setNMuon(j.muonMultiplicity());
    jet->setNPhoton(j.photonMultiplicity());
    jet->setNeutralHadFrac(j.neutralHadronEnergyFraction());
    jet->setChargedHadFrac(j.chargedHadronEnergyFraction());
    jet->setNeutralEMFrac(j.neutralEmEnergyFraction());
    jet->setChargedEMFrac(j.chargedEmEnergyFraction());
    njets++;
    jetCol_->push_back(*jet);
  }

}
