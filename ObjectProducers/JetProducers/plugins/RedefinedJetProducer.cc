#include <iostream>

#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/GenJet.h"

#include "ObjectProducers/JetProducers/plugins/RedefinedJetProducer.h"
#include "ObjectProducers/JetProducers/interface/FastJetClusterer.h"


using namespace ucsbsusy;
using namespace std;
//--------------------------------------------------------------------------------------------------
RedefinedJetProducer::RedefinedJetProducer(const edm::ParameterSet& iConfig) :
          isRealData      (iConfig.getParameter<int             >("isRealData"                 ))
        , produceGen      (isRealData == 0 && iConfig.getParameter<bool            >("produceGen"                 ))
        , producePU       (iConfig.getParameter<bool            >("producePU"                 ))
        , src             (iConfig.getParameter<edm::InputTag>("src"))
        , genSrc          (produceGen ? iConfig.getParameter<edm::InputTag>("genSrc") : edm::InputTag())
        , jetAlgorithmName(iConfig.getParameter<string>       ("jetAlgorithm"))
        , rParameter      (iConfig.getParameter<double>("rParameter"      ))
        , jetPtMin        (iConfig.getParameter<double>("jetPtMin"   ))
        , minParticlePT   (iConfig.getParameter<double>("minParticlePT"   ))
        , maxParticleEta  (iConfig.getParameter<double>("maxParticleEta"  ))
        , ghostArea       (iConfig.getParameter<double>("ghostArea"  ))
    {

  produces< reco::PFJetCollection             >(        );
    if(produceGen){
      produces< reco::GenJetCollection            >("Gen"   );
      produces< edm::ValueMap<reco::CandidatePtr> >("GenPtr");
    }
    if(producePU){
      produces<reco::PFJetCollection>("PU");
      produces< edm::ValueMap<reco::CandidatePtr> >("PUPtr" );
    }

    if (jetAlgorithmName=="CambridgeAachen")
      jetAlgo = fastjet::cambridge_aachen_algorithm;
    else if (jetAlgorithmName=="AntiKt")
      jetAlgo= fastjet::antikt_algorithm;
    else if (jetAlgorithmName=="Kt")
      jetAlgo= fastjet::kt_algorithm;
    else
      throw cms::Exception("Invalid jetAlgorithm")
        <<"Jet algorithm for RedefinedJetProducer is invalid, Abort!\n";
    }

//--------------------------------------------------------------------------------------------------
void RedefinedJetProducer::load(edm::Event& iEvent, const edm::EventSetup& iSetup){
  iEvent.getByLabel(src,pfCandidates);
  iEvent.getByLabel(genSrc,genParticles);
}

//--------------------------------------------------------------------------------------------------
void RedefinedJetProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup){
  load(iEvent,iSetup);

  //-- Cluster jets -----------------------------------------------------------
  FastJetClusterer                                    clusterer(true, true);

  clusterer.addParticles<pat::PackedCandidate>(pfCandidates, -1, minParticlePT, maxParticleEta, &isNonPUParticle );
  if (produceGen) clusterer.addParticles<pat::PackedGenParticle>( genParticles, -1, minParticlePT, maxParticleEta, 0, 1e-50 );
  if (producePU)  clusterer.addParticles<pat::PackedCandidate>( pfCandidates , -1 , minParticlePT, maxParticleEta,&isPUParticle, 1e-50);

  clusterer.clusterJets   ( jetAlgo, rParameter, produceGen ? 0 : jetPtMin , maxParticleEta, ghostArea );

  std::auto_ptr<reco::PFJetCollection>                recoJets  (new reco::PFJetCollection);
  clusterer.distillJets(pfCandidates  , *recoJets, iSetup,&isNonPUParticle, true);

  std::auto_ptr<reco::GenJetCollection>               genJets ;
  if (produceGen){
    genJets.reset(new reco::GenJetCollection);
    clusterer.distillJets<pat::PackedGenParticle, reco::GenJet>(genParticles, *genJets , iSetup,0, true);
  }

  std::auto_ptr<reco::PFJetCollection>                puInJets;
  if (producePU){
    puInJets.reset(new reco::PFJetCollection);
    clusterer.distillJets(pfCandidates  , *puInJets, iSetup,&isPUParticle, true);
  }

  edm::OrphanHandle<reco::PFJetCollection>            jetsHandle;
  edm::OrphanHandle<reco::PFJetCollection>            puJetsHandle;
  edm::OrphanHandle<reco::GenJetCollection>           genJetsHandle;

  std::auto_ptr<reco::PFJetCollection>                sortedRecoJets  (new reco::PFJetCollection);
  std::auto_ptr<reco::PFJetCollection>                sortedPuInJets  (new reco::PFJetCollection);


  //If we produce gen jets we need to make sure to drop jets that fall below the pT threshold
  //of both collections
  if(produceGen){
    //-- Sort genJet collection -----------------------------------------------
    std::vector<JetSorter>  rankedGenJets; rankedGenJets.reserve(genJets->size());
    for (unsigned int iGen = 0; iGen < genJets->size(); ++iGen) {
      if((*genJets )[iGen].pt() < jetPtMin && (*recoJets)[iGen].pt() < jetPtMin ) continue;
      rankedGenJets.emplace_back((*genJets )[iGen].pt(),(*recoJets)[iGen].pt(),iGen, rankedGenJets.size());
    } // end loop over genJets

    std::vector<int> recoIndices; recoIndices.reserve(rankedGenJets.size());
    if(producePU) sortedPuInJets->reserve(rankedGenJets.size());
    for(const auto& iR : rankedGenJets){
      sortedRecoJets->push_back((*recoJets)[iR.origIndex]);
      if(producePU) sortedPuInJets->push_back((*puInJets)[iR.origIndex]);
    }
    std::sort(rankedGenJets.begin(), rankedGenJets.end(), JetSorter());

    std::auto_ptr<reco::GenJetCollection>             sortedGen (new reco::GenJetCollection);
    sortedGen->reserve(rankedGenJets.size());

    for(const auto& iR : rankedGenJets)
      sortedGen->push_back((*genJets)[iR.origIndex]);

    genJetsHandle = iEvent.put(sortedGen, "Gen");
    jetsHandle = iEvent.put(sortedRecoJets);
    if(producePU) puJetsHandle  = iEvent.put(sortedPuInJets, "PU");

    //Make pointers to reco jets
    std::vector<reco::CandidatePtr>                   vGenPtr   (rankedGenJets.size());
    for (unsigned int iGen = 0; iGen < rankedGenJets.size(); ++iGen)
      vGenPtr[rankedGenJets[iGen].recoIndex] = reco::CandidatePtr(genJetsHandle, iGen);

    std::auto_ptr<edm::ValueMap<reco::CandidatePtr> > genJetPtr (new edm::ValueMap<reco::CandidatePtr>);
    edm::ValueMap<reco::CandidatePtr>::Filler         fGenPtr   (*genJetPtr);


    fGenPtr.insert(jetsHandle, vGenPtr.begin(), vGenPtr.end());
    fGenPtr.fill();
    iEvent.put(genJetPtr, "GenPtr");
  } else {
    jetsHandle  = iEvent.put(recoJets);
    if(producePU)puJetsHandle  = iEvent.put(puInJets, "PU");
  }

  if(producePU){
    //-- Associate PU in recoJets -----------------------------------------------
    std::vector<reco::CandidatePtr>                     vPUPtr(puInJets->size());
    edm::OrphanHandle<reco::PFJetCollection>            puJetsHandle  = iEvent.put(puInJets, "PU");
    for (unsigned int iPU = 0; iPU < vPUPtr.size(); ++iPU)
      vPUPtr[iPU]                                       = reco::CandidatePtr(puJetsHandle, iPU);

    std::auto_ptr<edm::ValueMap<reco::CandidatePtr> >   puJetPtr(new edm::ValueMap<reco::CandidatePtr>);
    edm::ValueMap<reco::CandidatePtr>::Filler           fPUPtr  (*puJetPtr);
    fPUPtr.insert(jetsHandle, vPUPtr.begin(), vPUPtr.end());
    fPUPtr.fill();
    iEvent.put(puJetPtr, "PUPtr");
  }

}
