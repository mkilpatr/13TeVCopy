#ifndef OBJECTPRODUCERS_JETPRODUCERS_JETFLAVORASSOCIATOR_H
#define OBJECTPRODUCERS_JETPRODUCERS_JETFLAVORASSOCIATOR_H

#include <functional>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"


#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
#include "AnalysisTools/Utilities/interface/JetFlavorMatching.h"
#include "DataFormats/JetReco/interface/PFJet.h"
using namespace ucsbsusy;


class JetFlavorAssociator : public edm::EDProducer {
public:
  JetFlavorAssociator(const edm::ParameterSet& iConfig);
  virtual ~JetFlavorAssociator(){};
  virtual void produce(edm::Event& iEvent, const edm::EventSetup& iSetup);


protected:
  const edm::EDGetTokenT<reco::GenJetCollection>            genJetToken_       ;
  const edm::EDGetTokenT<reco::GenParticleCollection>       genMotherToken_    ;
  const edm::EDGetTokenT<pat::PackedGenParticleCollection>  genToken_          ;

  const edm::EDGetTokenT<reco::PFJetCollection>             recoJetToken_       ;
  const edm::EDGetTokenT<reco::GenJetCollection>            partonJetToken_     ;
  const edm::EDGetTokenT<edm::ValueMap<reco::CandidatePtr>> recoToGenToken_     ;
  const edm::EDGetTokenT<edm::ValueMap<reco::CandidatePtr>> recoToPartonToken_  ;

 };

//--------------------------------------------------------------------------------------------------
JetFlavorAssociator::JetFlavorAssociator(const edm::ParameterSet& iConfig)
        : genJetToken_       (consumes<reco::GenJetCollection>(iConfig.getParameter<edm::InputTag>("genJetsSrc")))
        , genMotherToken_    (consumes<reco::GenParticleCollection>(iConfig.getParameter<edm::InputTag>("genMotherSrc")))
        , genToken_          (consumes<pat::PackedGenParticleCollection>(iConfig.getParameter<edm::InputTag>("genSrc")))
        , recoJetToken_      (consumes<reco::PFJetCollection>(iConfig.getParameter<edm::InputTag>("recoJetSrc")))
        , partonJetToken_    (consumes<reco::GenJetCollection>(iConfig.getParameter<edm::InputTag>("partonJetSrc")))
        , recoToGenToken_    (consumes<edm::ValueMap<reco::CandidatePtr>>(iConfig.getParameter<edm::InputTag>("recoToGenTag")))
        , recoToPartonToken_ (consumes<edm::ValueMap<reco::CandidatePtr>>(iConfig.getParameter<edm::InputTag>("recoToPartonTag")))
    {

      produces< std::vector<size8> >("flavors");
      produces< std::vector<float> >("bHadronPTs");
      produces< std::vector<float> >("cHadronPTs");
    }

//--------------------------------------------------------------------------------------------------
void JetFlavorAssociator::produce(edm::Event& iEvent, const edm::EventSetup& iSetup){
  edm::Handle<reco::GenJetCollection>           genJets           ;
  edm::Handle<reco::GenParticleCollection>      genMotherParticles;
  edm::Handle<pat::PackedGenParticleCollection> packedGenParticles;

  edm::Handle<reco::PFJetCollection>              recoJets           ;
  edm::Handle<reco::GenJetCollection>             partonJets         ;
  edm::Handle<edm::ValueMap<reco::CandidatePtr> > recoToGenJetPtr    ;
  edm::Handle<edm::ValueMap<reco::CandidatePtr> > recoTopartonJetPtr ;

  iEvent.getByToken(genJetToken_   ,genJets           );
  iEvent.getByToken(genMotherToken_,genMotherParticles);
  iEvent.getByToken(genToken_      ,packedGenParticles);

  iEvent.getByToken(recoJetToken_       ,recoJets);
  iEvent.getByToken(partonJetToken_     ,partonJets);
  iEvent.getByToken(recoToGenToken_     ,recoToGenJetPtr);
  iEvent.getByToken(recoToPartonToken_  ,recoTopartonJetPtr);

  //start with heavy flavors
  std::vector<JetFlavorMatching::ParticleDecay> bHadrons;
  std::vector<JetFlavorMatching::ParticleDecay> cHadrons;

  JetFlavorMatching::getHadronDecays(genMotherParticles,bHadrons,cHadrons);

  JetFlavorMatching::associateDecayProducts(genMotherParticles,packedGenParticles,
      &bHadrons,&cHadrons,0,0);

  std::vector<JetFlavorMatching::ParticleContainments> mainBHadrons;
  std::vector<JetFlavorMatching::ParticleContainments> satelliteBHadrons;

  std::vector<JetFlavorMatching::ParticleContainments> mainCHadrons;
  std::vector<JetFlavorMatching::ParticleContainments> satelliteCHadrons;

  JetFlavorMatching::associateHadronsToJets(packedGenParticles,*genJets,bHadrons,mainBHadrons,satelliteBHadrons);
  JetFlavorMatching::associateHadronsToJets(packedGenParticles,*genJets,cHadrons,mainCHadrons,satelliteCHadrons);

  std::vector<JetFlavorInfo::JetFlavor>                          flavors          (genJets->size(),JetFlavorInfo::unmatched_jet);
  std::vector<std::vector<JetFlavorMatching::ParticleDecayRef> > matchedParticles (genJets->size());
  //std::auto_ptr<std::vector<float> >                             bHadronPTs       (new std::vector<float>(genJets->size(),0));
  //std::auto_ptr<std::vector<float> >                             cHadronPTs       (new std::vector<float>(genJets->size(),0));
  std::unique_ptr<std::vector<float> >                             bHadronPTs       (new std::vector<float>(genJets->size(),0));
  std::unique_ptr<std::vector<float> >                             cHadronPTs       (new std::vector<float>(genJets->size(),0));

  JetFlavorMatching::assignJetHadronFlavors(*genJets,JetFlavorInfo::b_jet, mainBHadrons,satelliteBHadrons,flavors,*bHadronPTs,matchedParticles);
  JetFlavorMatching::assignJetHadronFlavors(*genJets,JetFlavorInfo::c_jet, mainCHadrons,satelliteCHadrons,flavors,*cHadronPTs,matchedParticles);


  //Now do light flavors, start by filling the vector of pointers
  std::vector<const reco::GenJet *> partonJetPtrs(genJets->size(),0);
  assert(recoJets->size() == genJets->size());
  assert(recoJets->size() == partonJets->size());

  for(unsigned int iJ = 0; iJ < recoJets->size(); ++iJ){
    const reco::CandidatePtr&   genPtr  = (*recoToGenJetPtr)[reco::CandidatePtr(recoJets, iJ)];
    if(genPtr.isNull()) throw cms::Exception("JetFlavorAssociator.prodce()", "Could not find genJet!");
    if (genPtr.id() != genJets.id()) throw cms::Exception("JetFlavorAssociator.prodce()", "Inconsistent genJet and recoJet collection!");

    const reco::CandidatePtr&   partonPtr  = (*recoTopartonJetPtr)[reco::CandidatePtr(recoJets, iJ)];
    if(partonPtr.isNull()) throw cms::Exception("JetFlavorAssociator.prodce()", "Could not find partonJet!");
    if (partonPtr.id() != partonJets.id()) throw cms::Exception("JetFlavorAssociator.prodce()", "Inconsistent partonJet and recoJet collection!");

    partonJetPtrs[genPtr.key()] = &(*partonJets)[partonPtr.key()];
  }

  JetFlavorMatching::assignJetPartonFlavors(partonJetPtrs,*genJets,flavors,true);


  //std::auto_ptr<std::vector<size8> >   outFlavors(new std::vector<size8>(flavors.size()) );
  std::unique_ptr<std::vector<size8> >   outFlavors(new std::vector<size8>(flavors.size()) );
  for(unsigned int iF = 0; iF < flavors.size(); ++iF)
    outFlavors->at(iF) = flavors[iF];

  iEvent.put(move(outFlavors), "flavors");
  iEvent.put(move(bHadronPTs), "bHadronPTs");
  iEvent.put(move(cHadronPTs), "cHadronPTs");

}





DEFINE_FWK_MODULE(JetFlavorAssociator);
#endif
