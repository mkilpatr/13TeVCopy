#include <iostream>

#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/GenJet.h"


#include "ObjectProducers/JetProducers/interface/ReJetProducerBase.h"
#include "AnalysisTools/Utilities/interface/ParticleUtilities.h"


using namespace ucsbsusy;
using namespace std;
//--------------------------------------------------------------------------------------------------
ReJetProducer::ReJetProducer(const edm::ParameterSet& iConfig) :
          isRealData      (iConfig.getParameter<int             >("isRealData"                 ))
        , produceGen      (isRealData == 0 && iConfig.getParameter<bool            >("produceGen"                 ))
        , ignoreBosonInv  (isRealData == 0 && iConfig.getParameter<bool            >("ignoreBosonInv"                 ))
        , ignoreBSMInv    (isRealData == 0 && iConfig.getParameter<bool            >("ignoreBSMInv"                 ))
        , src             (iConfig.getParameter<edm::InputTag>("src"))
        , genMotherSrc    (iConfig.getParameter<edm::InputTag>("genMotherSrc"))
        , genSrc          (iConfig.getParameter<edm::InputTag>("genSrc"))
        , jetAlgorithmName(iConfig.getParameter<string>       ("jetAlgorithm"))
        , rParameter      (iConfig.getParameter<double>("rParameter"      ))
        , jetPtMin        (iConfig.getParameter<double>("jetPtMin"   ))
        , minParticlePT   (iConfig.getParameter<double>("minParticlePT"   ))
        , maxParticleEta  (iConfig.getParameter<double>("maxParticleEta"  ))
        , ghostArea       (iConfig.getParameter<double>("ghostArea"  ))
        , useTrimming     (iConfig.getParameter<bool  >("useTrimming"  ))
        , rFilt           (iConfig.getParameter<double>("rFilt"  ))
        , trimPtFracMin   (iConfig.getParameter<double>("trimPtFracMin"  ))

    {

  produces< reco::PFJetCollection             >(        );
    if(produceGen){
      produces< reco::GenJetCollection            >("Gen"   );
      produces< edm::ValueMap<reco::CandidatePtr> >("GenPtr");
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
void ReJetProducer::vetoGenPart(std::vector<bool>& vetoes) const {
  for(unsigned int iP = 0; iP < genParticles->size(); ++iP){
    const pat::PackedGenParticle& p = genParticles->at(iP);
    if(ignoreBSMInv && ParticleInfo::isLSP(p.pdgId())) {vetoes[iP] = true; continue;}
    if(!ignoreBosonInv || ParticleInfo::isVisible(p.pdgId())) continue;

    const reco::Candidate * pruneMom = p.mother(0);
    if(pruneMom == nullptr) continue;

    //if it is the same particle, let's get the original and see if it's mother is a boson
    if(pruneMom->pdgId() == p.pdgId()){
      pruneMom = ParticleUtilities::getOriginal(pruneMom);
      for(unsigned int iM = 0; iM < pruneMom->numberOfMothers(); ++iM){
        const reco::Candidate * mom = pruneMom->mother(iM);
        if(ParticleInfo::isDoc(mom->status()) && ParticleInfo::isEWKBoson(mom->pdgId()))
          vetoes[iP] = true;
      }
    }
    //otherwise it's direct mother must be a boson
    else if(ParticleInfo::isDoc(pruneMom->status()) && ParticleInfo::isEWKBoson(pruneMom->pdgId()) ){
      vetoes[iP] = true;
    }
  }
}

//--------------------------------------------------------------------------------------------------
void ReJetProducer::load(edm::Event& iEvent, const edm::EventSetup& iSetup){
  if(produceGen){
    iEvent.getByLabel(genSrc,genParticles);
    if(ignoreBosonInv) iEvent.getByLabel(genMotherSrc,genMotherParticles);
  }
}

void ReJetProducer::putJets(edm::Event& iEvent, std::auto_ptr<reco::PFJetCollection> recoJets, std::auto_ptr<reco::GenJetCollection> genJets, std::auto_ptr<reco::PFJetCollection> puJets){
  edm::OrphanHandle<reco::PFJetCollection>            jetsHandle;
  edm::OrphanHandle<reco::PFJetCollection>            puJetsHandle;
  edm::OrphanHandle<reco::GenJetCollection>           genJetsHandle;

  std::auto_ptr<reco::PFJetCollection>                sortedRecoJets  (new reco::PFJetCollection);
  std::auto_ptr<reco::PFJetCollection>                sortedPuInJets  (new reco::PFJetCollection);

  //If we produce gen jets we need to make sure to drop jets that fall below the pT threshold
  //of both collections
  if(genJets.get() != 0){
    //-- Sort genJet collection -----------------------------------------------
    std::vector<JetSorter>  rankedGenJets; rankedGenJets.reserve(genJets->size());
    for (unsigned int iGen = 0; iGen < genJets->size(); ++iGen) {
      if((*genJets )[iGen].pt() < jetPtMin && (*recoJets)[iGen].pt() < jetPtMin ) continue;
      rankedGenJets.emplace_back((*genJets )[iGen].pt(),(*recoJets)[iGen].pt(),iGen, rankedGenJets.size());
    } // end loop over genJets

    std::vector<int> recoIndices; recoIndices.reserve(rankedGenJets.size());
    if(puJets.get()) sortedPuInJets->reserve(rankedGenJets.size());
    for(const auto& iR : rankedGenJets){
      sortedRecoJets->push_back((*recoJets)[iR.origIndex]);
      if(puJets.get()) sortedPuInJets->push_back((*puJets)[iR.origIndex]);
    }
    std::sort(rankedGenJets.begin(), rankedGenJets.end(), JetSorter());

    std::auto_ptr<reco::GenJetCollection>             sortedGen (new reco::GenJetCollection);
    sortedGen->reserve(rankedGenJets.size());

    for(const auto& iR : rankedGenJets)
      sortedGen->push_back((*genJets)[iR.origIndex]);

    genJetsHandle = iEvent.put(sortedGen, "Gen");
    jetsHandle = iEvent.put(sortedRecoJets);
    if(puJets.get()) puJetsHandle  = iEvent.put(sortedPuInJets, "PU");

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
    if(puJets.get()) puJetsHandle  = iEvent.put(puJets, "PU");
  }

  if(puJets.get()){
    //-- Associate PU in recoJets -----------------------------------------------
    std::vector<reco::CandidatePtr>                     vPUPtr(puJetsHandle->size());
    for (unsigned int iPU = 0; iPU < vPUPtr.size(); ++iPU)
      vPUPtr[iPU]                                       = reco::CandidatePtr(puJetsHandle, iPU);

    std::auto_ptr<edm::ValueMap<reco::CandidatePtr> >   puJetPtr(new edm::ValueMap<reco::CandidatePtr>);
    edm::ValueMap<reco::CandidatePtr>::Filler           fPUPtr  (*puJetPtr);
    fPUPtr.insert(jetsHandle, vPUPtr.begin(), vPUPtr.end());
    fPUPtr.fill();
    iEvent.put(puJetPtr, "PUPtr");
  }


}
