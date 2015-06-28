#ifndef OBJECTPRODUCERS_JETPRODUCERS_PFREJETPRODUCER_H
#define OBJECTPRODUCERS_JETPRODUCERS_PFREJETPRODUCER_H

#ifndef OBJECTPRODUCERS_JETPRODUCERS_PACKEDREJETPRODUCER_H
#define OBJECTPRODUCERS_JETPRODUCERS_PACKEDREJETPRODUCER_H

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"

#include "ObjectProducers/JetProducers/interface/ReJetProducerBase.h"

using namespace std;
using namespace ucsbsusy;

class PFReJetProducer : public ReJetProducer{
public:
  PFReJetProducer(const edm::ParameterSet& iConfig) : ReJetProducer(iConfig)
 {};
  virtual ~PFReJetProducer(){};

  virtual void load(edm::Event& iEvent, const edm::EventSetup& iSetup){
    iEvent.getByLabel(src,recoCandidates);
    ReJetProducer::load(iEvent,iSetup);
  }

  void produce(edm::Event& iEvent, const edm::EventSetup& iSetup){
    load(iEvent,iSetup);
    FastJetClusterer                                    clusterer(true, true);
    clusterer.setDeterministicSeed(iEvent.id().run(),iEvent.id().event());

    clusterJets(clusterer,recoCandidates,edm::Handle<reco::PFCandidateCollection >());

    std::auto_ptr<reco::PFJetCollection>                recoJets  (new reco::PFJetCollection);
    std::auto_ptr<std::vector<int> > superJetIndices (new std::vector<int> );
    if(useSubjetCountingCA) {
      if(outputSuperJets) {
        superJetIndices.reset(new std::vector<int>(clusterer.getCompoundJets().size(), -1));
         clusterer.distillJets<reco::PFCandidate, reco::PFJet>(recoCandidates, clusterer.getCompoundJets(), *recoJets, iSetup, 0, true, true, FastJetClusterer::DEFAULT_VERTEX, *superJetIndices);
      } else {
        clusterer.distillJets<reco::PFCandidate, FastJetClusterer::CompoundPseudoJet, reco::PFJet>(recoCandidates, clusterer.getCompoundJets(), *recoJets, iSetup, 0, true);
      }
    }
    else clusterer.distillJets<reco::PFCandidate, reco::PFJet>(recoCandidates  , *recoJets, iSetup, 0, true);

    std::auto_ptr<reco::GenJetCollection>               genJets ;
    if (produceGen){
      genJets.reset(new reco::GenJetCollection);
      if(useSubjetCountingCA) clusterer.distillJets<pat::PackedGenParticle, FastJetClusterer::CompoundPseudoJet, reco::GenJet>(genParticles, clusterer.getCompoundJets(), *genJets, iSetup, 0, true);
      else clusterer.distillJets<pat::PackedGenParticle, reco::GenJet>(genParticles, *genJets , iSetup,0, true);
    }
    std::auto_ptr<reco::GenJetCollection>               partonJets ;
    if(producePartonJets){
      partonJets.reset(new reco::GenJetCollection);
      if(useSubjetCountingCA) clusterer.distillJets<reco::GenParticle, FastJetClusterer::CompoundPseudoJet, reco::GenJet>(genMotherParticles, clusterer.getCompoundJets(), *partonJets, iSetup, 0, true);
      else clusterer.distillJets<reco::GenParticle, reco::GenJet>(genMotherParticles, *partonJets , iSetup,0, true);
      std::auto_ptr<reco::GenJetCollection>                partonJets  (new reco::GenJetCollection);
    }

    if(outputSuperJets){
      std::auto_ptr<reco::PFJetCollection>                superJets  (new reco::PFJetCollection);
      clusterer.distillSuperJets<reco::PFCandidate, reco::PFJet>(recoCandidates  , *superJets, iSetup,0, true);
      iEvent.put(superJets, "Super");
      if(useSubjetCountingCA) {
        std::auto_ptr<std::vector<unsigned int> > nSubjetsCA(new std::vector<unsigned int>(clusterer.getSuperJets().size(), 0));
        std::vector<unsigned int> nsubjets = clusterer.getNSubjetsCA();
        for(unsigned int iJ = 0; iJ < nsubjets.size(); ++iJ) {
          nSubjetsCA->at(iJ) = nsubjets[iJ];
        }
        iEvent.put(nSubjetsCA, "NCASubjets");
      }
    }

    putJets(iEvent,recoJets,genJets,partonJets, std::auto_ptr<reco::PFJetCollection>(0), superJetIndices);
  }

protected:
  edm::Handle<reco::PFCandidateCollection >           recoCandidates;

};

class PackedReJetProducer : public ReJetProducer{
public:
  PackedReJetProducer(const edm::ParameterSet& iConfig) : ReJetProducer(iConfig), producePU       (iConfig.getParameter<bool            >("producePU"                 ))
 {
    if(producePU){
      produces<reco::PFJetCollection>("PU");
      produces< edm::ValueMap<reco::CandidatePtr> >("PUPtr" );
    }
 };
  virtual ~PackedReJetProducer(){};

  virtual void load(edm::Event& iEvent, const edm::EventSetup& iSetup){
    iEvent.getByLabel(src,recoCandidates);
    ReJetProducer::load(iEvent,iSetup);
  }

  void produce(edm::Event& iEvent, const edm::EventSetup& iSetup){
    load(iEvent,iSetup);
    FastJetClusterer                                    clusterer(true, true);
    clusterer.setDeterministicSeed(iEvent.id().run(),iEvent.id().event());

    producePU ? clusterJets(clusterer,recoCandidates,edm::Handle<pat::PackedCandidateCollection >(0),  &isNonPUParticle, &isPUParticle)
              : clusterJets(clusterer,recoCandidates,edm::Handle<pat::PackedCandidateCollection >(0),  &isNonPUParticle);

    std::auto_ptr<reco::PFJetCollection>                recoJets  (new reco::PFJetCollection);
    std::auto_ptr<std::vector<int> > superJetIndices (new std::vector<int> );
    if(useSubjetCountingCA) {
      if(outputSuperJets) {
        superJetIndices.reset(new std::vector<int>(clusterer.getCompoundJets().size(), -1));
        clusterer.distillJets<pat::PackedCandidate, reco::PFJet>(recoCandidates, clusterer.getCompoundJets(), *recoJets, iSetup,&isNonPUParticle, true, true, FastJetClusterer::DEFAULT_VERTEX, *superJetIndices);
      } else {
        clusterer.distillJets<pat::PackedCandidate, FastJetClusterer::CompoundPseudoJet, reco::PFJet>(recoCandidates, clusterer.getCompoundJets(), *recoJets, iSetup,&isNonPUParticle, true);
      }
    }
    else clusterer.distillJets<pat::PackedCandidate, reco::PFJet>(recoCandidates  , *recoJets, iSetup,&isNonPUParticle, true);

    std::auto_ptr<reco::GenJetCollection>               genJets ;
    if (produceGen){
      genJets.reset(new reco::GenJetCollection);
      if(useSubjetCountingCA) clusterer.distillJets<pat::PackedGenParticle, FastJetClusterer::CompoundPseudoJet, reco::GenJet>(genParticles, clusterer.getCompoundJets(), *genJets, iSetup, 0, true);
      else clusterer.distillJets<pat::PackedGenParticle, reco::GenJet>(genParticles, *genJets , iSetup,0, true);
    }
    std::auto_ptr<reco::GenJetCollection>               partonJets ;
    if(producePartonJets){
      partonJets.reset(new reco::GenJetCollection);
      if(useSubjetCountingCA) clusterer.distillJets<reco::GenParticle, FastJetClusterer::CompoundPseudoJet, reco::GenJet>(genMotherParticles, clusterer.getCompoundJets(), *partonJets, iSetup, 0, true);
      else clusterer.distillJets<reco::GenParticle, reco::GenJet>(genMotherParticles, *partonJets , iSetup,0, true);
      std::auto_ptr<reco::GenJetCollection>                partonJets  (new reco::GenJetCollection);
    }

      std::auto_ptr<reco::PFJetCollection>                puInJets;
      if (producePU){
        puInJets.reset(new reco::PFJetCollection);
        clusterer.distillJets(recoCandidates  , *puInJets, iSetup,&isPUParticle, true);
      }

     if(outputSuperJets){
       std::auto_ptr<reco::PFJetCollection>                superJets  (new reco::PFJetCollection);
       clusterer.distillSuperJets<pat::PackedCandidate, reco::PFJet>(recoCandidates  , *superJets, iSetup,&isNonPUParticle, true);
       iEvent.put(superJets, "Super");
       if(useSubjetCountingCA) {
         std::auto_ptr<std::vector<unsigned int> > nSubjetsCA(new std::vector<unsigned int>(clusterer.getSuperJets().size(), 0));
         std::vector<unsigned int> nsubjets = clusterer.getNSubjetsCA();
         for(unsigned int iJ = 0; iJ < nsubjets.size(); ++iJ) {
           nSubjetsCA->at(iJ) = nsubjets[iJ];
         }
         iEvent.put(nSubjetsCA, "NCASubjets");
       }
     }

    putJets(iEvent,recoJets,genJets,partonJets,puInJets,superJetIndices);
  }

  static bool isPUParticle(const pat::PackedCandidate& p) {return p.fromPV() == 0; }
  static bool isNonPUParticle(const pat::PackedCandidate& p) {return !isPUParticle(p); }

protected:
  const bool producePU;
  edm::Handle<pat::PackedCandidateCollection >           recoCandidates;

};

DEFINE_FWK_MODULE(PFReJetProducer);
DEFINE_FWK_MODULE(PackedReJetProducer);
#endif
#endif
