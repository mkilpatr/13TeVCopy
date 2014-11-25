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
    FastJetClusterer& clusterer = clusterJets(&(*recoCandidates));

    std::auto_ptr<reco::PFJetCollection>                recoJets  (new reco::PFJetCollection);
    clusterer.distillJets<reco::PFCandidate, reco::PFJet>(recoCandidates  , *recoJets, iSetup,0, true);

    std::auto_ptr<reco::GenJetCollection>               genJets ;
    if (produceGen){
      genJets.reset(new reco::GenJetCollection);
      clusterer.distillJets<pat::PackedGenParticle, reco::GenJet>(genParticles, *genJets , iSetup,0, true);
    }

    putJets(iEvent,recoJets,genJets, std::auto_ptr<reco::PFJetCollection>(0));
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

    FastJetClusterer& clusterer =  producePU ? clusterJets(&(*recoCandidates), &isNonPUParticle, 0 , &isPUParticle)
                                             : clusterJets(&(*recoCandidates), &isNonPUParticle);

    std::auto_ptr<reco::PFJetCollection>                recoJets  (new reco::PFJetCollection);
    clusterer.distillJets<pat::PackedCandidate, reco::PFJet>(recoCandidates  , *recoJets, iSetup,&isNonPUParticle, true);

    std::auto_ptr<reco::GenJetCollection>               genJets ;
    if (produceGen){
      genJets.reset(new reco::GenJetCollection);
      clusterer.distillJets<pat::PackedGenParticle, reco::GenJet>(genParticles, *genJets , iSetup,0, true);
    }

      std::auto_ptr<reco::PFJetCollection>                puInJets;
      if (producePU){
        puInJets.reset(new reco::PFJetCollection);
        clusterer.distillJets(recoCandidates  , *puInJets, iSetup,&isPUParticle, true);
      }

    putJets(iEvent,recoJets,genJets,puInJets);
  }

  static bool isPUParticle(const pat::PackedCandidate& p) {return p.fromPV() == 0; }
  static bool isNonPUParticle(const pat::PackedCandidate& p) {return !isPUParticle(p); }

protected:
  const bool producePU;
  edm::Handle<pat::PackedCandidateCollection >           recoCandidates;

};

DEFINE_FWK_MODULE(PFReJetProducer);
DEFINE_FWK_MODULE(PackedReJetProducer);
