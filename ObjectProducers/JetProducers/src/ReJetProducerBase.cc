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
        , producePartonJets(isRealData == 0 && iConfig.getParameter<bool           >("producePartonJets"                 ))
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
        , outputSuperJets (iConfig.getParameter<bool>("outputSuperJets"  ))
        , useTrimming     (iConfig.getParameter<bool  >("useTrimming"  ))
		    , useTrimmedSubjets(iConfig.getParameter<bool  >("useTrimmedSubjets"  ))
        , rFilt           (iConfig.getParameter<double>("rFilt"  ))
        , trimPtFracMin   (iConfig.getParameter<double>("trimPtFracMin"  ))
        , useSubjetCountingCA(iConfig.getParameter<bool>("useSubjetCountingCA"  ))
        , ptCut           (iConfig.getParameter<double>("ptCut"  ))
        , mCutoff         (iConfig.getParameter<double>("mCutoff"  ))
        , yCut            (iConfig.getParameter<double>("yCut"  ))
        , rMin            (iConfig.getParameter<double>("rMin"  ))
        , doPickyJets     (iConfig.getParameter<bool  >("doPickyJets"  ))
        , pickyMaxSplits  (iConfig.getParameter<int>("pickyMaxSplits"  ))
        , splitter        (doPickyJets ? new PickyJetSplitting(iConfig.getParameter<string>("pickyMVAFileName"),iConfig.getParameter<string>("pickyMVAName"),PickyJetSplitting::NOPUPPI_RECO,iConfig) : 0)
        , genMotherParticlesToken_(consumes<reco::GenParticleCollection>(genMotherSrc))
        , genParticlesToken_(consumes<pat::PackedGenParticleCollection>(genSrc))
    {

  produces< reco::PFJetCollection             >(        );
    if(produceGen){
      produces< reco::GenJetCollection            >("Gen"   );
      produces< edm::ValueMap<reco::CandidatePtr> >("GenPtr");
    }
    if(producePartonJets){
      produces< reco::GenJetCollection            >("parton"   );
      produces< edm::ValueMap<reco::CandidatePtr> >("partonPtr");
    }
    if(outputSuperJets){
      produces< reco::PFJetCollection           >("Super"   );
      if(useSubjetCountingCA) {
        produces< std::vector<int>              >("SuperJetIndex");
        produces< std::vector<unsigned int>     >("NCASubjets");
      }
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
    iEvent.getByToken(genParticlesToken_,genParticles);
  }
  if(producePartonJets || (produceGen && ignoreBosonInv) ) iEvent.getByToken(genMotherParticlesToken_,genMotherParticles);
}

void ReJetProducer::putJets(edm::Event& iEvent, std::auto_ptr<reco::PFJetCollection> recoJets, std::auto_ptr<reco::GenJetCollection> genJets,std::auto_ptr<reco::GenJetCollection> partonJets, std::auto_ptr<reco::PFJetCollection> puJets, std::auto_ptr<std::vector<int> > superJetIndices){
  edm::OrphanHandle<reco::PFJetCollection>            jetsHandle;
  edm::OrphanHandle<reco::PFJetCollection>            puJetsHandle;
  edm::OrphanHandle<reco::GenJetCollection>           genJetsHandle;
  edm::OrphanHandle<reco::GenJetCollection>           partonJetsHandle;

  std::auto_ptr<reco::PFJetCollection>                sortedRecoJets  (new reco::PFJetCollection);
  std::auto_ptr<reco::PFJetCollection>                sortedPuInJets  (new reco::PFJetCollection);
  std::auto_ptr<reco::GenJetCollection>               sortedPartonInJets  (new reco::GenJetCollection);
  std::auto_ptr<std::vector<int> >                    sortedSuperJetIndices  (new std::vector<int>);

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
    if(partonJets.get()) sortedPartonInJets->reserve(rankedGenJets.size());
    if(superJetIndices.get() && superJetIndices->size()) sortedSuperJetIndices->reserve(rankedGenJets.size());
    for(const auto& iR : rankedGenJets){
      sortedRecoJets->push_back((*recoJets)[iR.origIndex]);
      if(puJets.get()) sortedPuInJets->push_back((*puJets)[iR.origIndex]);
      if(partonJets.get()) sortedPartonInJets->push_back((*partonJets)[iR.origIndex]);
      if(superJetIndices.get() && superJetIndices->size()) sortedSuperJetIndices->push_back((*superJetIndices)[iR.origIndex]);
    }
    std::sort(rankedGenJets.begin(), rankedGenJets.end(), JetSorter());

    std::auto_ptr<reco::GenJetCollection>             sortedGen (new reco::GenJetCollection);
    sortedGen->reserve(rankedGenJets.size());

    for(const auto& iR : rankedGenJets)
      sortedGen->push_back((*genJets)[iR.origIndex]);

    genJetsHandle = iEvent.put(sortedGen, "Gen");
    jetsHandle = iEvent.put(sortedRecoJets);
    if(puJets.get()) puJetsHandle  = iEvent.put(sortedPuInJets, "PU");
    if(partonJets.get()) partonJetsHandle  = iEvent.put(sortedPartonInJets, "parton");
    if(superJetIndices.get() && superJetIndices->size()) iEvent.put(sortedSuperJetIndices, "SuperJetIndex");

    //Make pointers to reco jets
    putPointer(iEvent,jetsHandle,genJetsHandle,"GenPtr",&rankedGenJets);
  } else {
    jetsHandle  = iEvent.put(recoJets);
    if(puJets.get()) puJetsHandle  = iEvent.put(puJets, "PU");
    if(partonJets.get()) partonJetsHandle  = iEvent.put(partonJets, "parton");
    if(superJetIndices.get() && superJetIndices->size()) iEvent.put(superJetIndices, "SuperJetIndex");
  }

  if(puJets.get()){
    putPointer(iEvent,jetsHandle,puJetsHandle,"PUPtr");
  }
  if(partonJets.get()){
    putPointer(iEvent,jetsHandle,partonJetsHandle,"partonPtr");
  }

}
