#ifndef OBJECTPRODUCERS_JETPRODUCERS_RECOGENJETASSOCIATOR_H
#define OBJECTPRODUCERS_JETPRODUCERS_RECOGENJETASSOCIATOR_H

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/Common/interface/ValueMap.h"

#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"

class RecoGenJetAssociator : public edm::EDProducer {

  public :
    RecoGenJetAssociator(const edm::ParameterSet& iConfig);
    virtual ~RecoGenJetAssociator() {}

    virtual void produce(edm::Event & event, const edm::EventSetup& eventSetup);
    virtual void load(edm::Event& iEvent, const edm::EventSetup& iSetup);

  protected :
    const edm::EDGetTokenT<reco::PFJetCollection>  recoJetsToken_;
    const edm::EDGetTokenT<reco::PFJetCollection>  redRecoJetsToken_;
    const edm::EDGetTokenT<reco::GenJetCollection> redGenJetsToken_;
    const edm::EDGetTokenT<edm::ValueMap<reco::CandidatePtr>> redGenJetsPtrToken_;
    const std::string   redGenJetTag;

    edm::Handle<reco::PFJetCollection>              recoJets;
    edm::Handle<reco::PFJetCollection>              redRecoJets;
    edm::Handle<reco::GenJetCollection>             redGenJets;
    edm::Handle<edm::ValueMap<reco::CandidatePtr> > redGenJetsPtr;
};

RecoGenJetAssociator::RecoGenJetAssociator(const edm::ParameterSet& iConfig)
  : recoJetsToken_          (consumes<reco::PFJetCollection>(iConfig.getParameter<edm::InputTag>("recoJetsSrc")))
  , redRecoJetsToken_       (consumes<reco::PFJetCollection>(iConfig.getParameter<edm::InputTag>("redRecoJetsSrc")))
  , redGenJetsToken_        (consumes<reco::GenJetCollection>(iConfig.getParameter<edm::InputTag>("redGenJetsSrc")))
  , redGenJetsPtrToken_     (consumes<edm::ValueMap<reco::CandidatePtr>>(iConfig.getParameter<edm::InputTag>("redGenJetsPtrSrc")))
  , redGenJetTag         (iConfig.getParameter<std::string>("redGenJetTag"))
{
  produces<edm::ValueMap<reco::CandidatePtr> >(redGenJetTag);
}

void RecoGenJetAssociator::load(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  iEvent.getByToken(recoJetsToken_     , recoJets     );
  iEvent.getByToken(redRecoJetsToken_  , redRecoJets  );
  iEvent.getByToken(redGenJetsToken_   , redGenJets   );
  iEvent.getByToken(redGenJetsPtrToken_, redGenJetsPtr);
}

void RecoGenJetAssociator::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  load(iEvent,iSetup);

  const double maxDeltaR = .1;
  std::vector<bool> matches(redRecoJets->size(),false);
  std::vector<reco::CandidatePtr> ptrs(recoJets->size());

  for(size_t ijet = 0; ijet < recoJets->size(); ijet++) {
    const reco::PFJet &j = (*recoJets)[ijet];

    double nearDR = .5;
    int nearInd = PhysicsUtilities::findNearestDR(j,(*redRecoJets.product()),nearDR,maxDeltaR,.1,&matches );

    if(nearInd > -1) {
      matches[nearInd] = true;
      ptrs[ijet] = (*redGenJetsPtr)[reco::CandidatePtr(redRecoJets, nearInd)];
    }
  }

  //std::auto_ptr<edm::ValueMap<reco::CandidatePtr> > valMap(new edm::ValueMap<reco::CandidatePtr>());
  std::unique_ptr<edm::ValueMap<reco::CandidatePtr> > valMap(new edm::ValueMap<reco::CandidatePtr>());
  edm::ValueMap<reco::CandidatePtr>::Filler valMapFiller(*valMap);

  valMapFiller.insert(recoJets, ptrs.begin(), ptrs.end());
  valMapFiller.fill();

  iEvent.put(move(valMap), redGenJetTag);

}

DEFINE_FWK_MODULE(RecoGenJetAssociator);
#endif
