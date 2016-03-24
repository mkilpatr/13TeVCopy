#ifndef OBJECTPRODUCERS_JETPRODUCERS_REJETPRODUCERBASE_H
#define OBJECTPRODUCERS_JETPRODUCERS_REJETPRODUCERBASE_H

#include <functional>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include <fastjet/JetDefinition.hh>
#include "DataFormats/PatCandidates/interface/PackedGenParticle.h"
#include "ObjectProducers/JetProducers/interface/FastJetClusterer.h"
#include "ObjectProducers/JetProducers/interface/PickyJetSplitting.h"

namespace ucsbsusy {
/// Compares two pairs by their fields.
/**
  Orders two objects by a vector of fields. The first field is compared, then the second, and so on,
  to make a decision. If one has more fields than another, it is ordered first by default.
*/
struct JetSorter {
public:
  JetSorter() : genPT(0), recoPT(0), origIndex(0), recoIndex(0) {};
  double genPT    ;
  double recoPT   ;
  int    origIndex;
  int    recoIndex;

  JetSorter(const double genPT_,const double recoPT_,const int origIndex_,const int recoIndex_) :
    genPT     (genPT_),
    recoPT    (recoPT_),
    origIndex (origIndex_),
    recoIndex (recoIndex_)
    {}

  Bool_t operator()( const JetSorter& x, const JetSorter& y) const {
    if (x.genPT > y.genPT) return true;
    if (x.genPT < y.genPT) return false;

    if (x.recoPT > y.recoPT) return true;
    if (x.recoPT < y.recoPT) return false;

    return true;
  }
};

class ReJetProducer : public edm::EDProducer {
public:
  ReJetProducer(const edm::ParameterSet& iConfig);
  virtual ~ReJetProducer(){delete splitter;};
  virtual void produce(edm::Event& iEvent, const edm::EventSetup& iSetup) = 0;
  virtual void load(edm::Event& iEvent, const edm::EventSetup& iSetup);

  /* clusters the jets together....two different ways to add PU particles
   *
   * If puParts != 0, then these are filled into the PU particles
   * If selectPU != 0, then this is used to fill the PU particles
   * Where, only the case of selectPU == 0 and puParts == 0 results in no PU jets being prouced
   */
  template<typename RecoPart>
  void clusterJets(FastJetClusterer& clusterer, const edm::Handle<std::vector<RecoPart> >& recoParts, const edm::Handle<std::vector<RecoPart> >& puParts, bool (*selectReco)(const RecoPart&) = 0, bool (*selectPU)(const RecoPart&) = 0  ) {
    clusterer.addParticles<RecoPart>(recoParts,FastJetClusterer::RECO, -1, minParticlePT, maxParticleEta,selectReco);
    if(produceGen){
      std::vector<bool>* vetoes = 0;
      if(ignoreBosonInv || ignoreBSMInv){
        vetoes = new std::vector<bool>(genParticles->size(), false);
        vetoGenPart(*vetoes);
      }
      clusterer.addParticles<pat::PackedGenParticle>( genParticles,FastJetClusterer::GEN, -1, minParticlePT, maxParticleEta, 0,vetoes, 1e-50 );
      delete vetoes;
    }
    if(producePartonJets){
      clusterer.addParticles<reco::GenParticle>( genMotherParticles,FastJetClusterer::PARTON, -1, 0, maxParticleEta, 0,0, 1e-50 );
    }
    if(puParts.isValid() || selectPU)
      clusterer.addParticles<RecoPart>( (puParts.isValid() ? puParts : recoParts) ,FastJetClusterer::PU, -1 , minParticlePT, maxParticleEta,selectPU,0, 1e-50);

    clusterer.clusterJets   ( jetAlgo, rParameter, produceGen ? 0 : jetPtMin , maxParticleEta, ghostArea );
    if(outputSuperJets) clusterer.storeSuperJets();
    if(doPickyJets)clusterer.pickySubjets(splitter,pickyMaxSplits);
    if(useTrimming)clusterer.trimJets(rFilt,trimPtFracMin,useTrimmedSubjets);
    if(useSubjetCountingCA)clusterer.applySubjetCountingCA(mCutoff, yCut, rMin, ptCut);
  }

  void putJets(edm::Event& iEvent, std::auto_ptr<reco::PFJetCollection> recoJets, std::auto_ptr<reco::GenJetCollection> genJets,std::auto_ptr<reco::GenJetCollection> partonJets, std::auto_ptr<reco::PFJetCollection> puJets, std::auto_ptr<std::vector<int> > superJetIndices);
  template<typename recoJetCol, typename ptrJetCol >
  void putPointer(edm::Event& iEvent, edm::OrphanHandle<recoJetCol> recoJetHandle,edm::OrphanHandle<ptrJetCol> ptrJetHandle, std::string const& label, const std::vector<JetSorter> * sortedJets = 0){
    std::vector<reco::CandidatePtr>                     vPUPtr(ptrJetHandle->size());
    if(sortedJets){
      for (unsigned int iPU = 0; iPU < vPUPtr.size(); ++iPU)
        vPUPtr[(*sortedJets)[iPU].recoIndex]                                       = reco::CandidatePtr(ptrJetHandle, iPU);
    } else{
      for (unsigned int iPU = 0; iPU < vPUPtr.size(); ++iPU)
        vPUPtr[iPU]                                       = reco::CandidatePtr(ptrJetHandle, iPU);
    }
    std::auto_ptr<edm::ValueMap<reco::CandidatePtr> >   puJetPtr(new edm::ValueMap<reco::CandidatePtr>);
    edm::ValueMap<reco::CandidatePtr>::Filler           fPUPtr  (*puJetPtr);
    fPUPtr.insert(recoJetHandle, vPUPtr.begin(), vPUPtr.end());
    fPUPtr.fill();
    iEvent.put(puJetPtr,label);
  }
  void vetoGenPart(std::vector<bool>& vetoes) const;

protected:
  const int isRealData;
  const bool produceGen;
  const bool producePartonJets;
  const bool ignoreBosonInv;
  const bool ignoreBSMInv;

  const edm::InputTag src;
  const edm::InputTag genMotherSrc;
  const edm::InputTag genSrc;
  const std::string jetAlgorithmName;
  const double rParameter;
  const double jetPtMin;
  const double minParticlePT;
  const double maxParticleEta;
  const double ghostArea;

  const bool   outputSuperJets;
  const bool   useTrimming;
  const bool   useTrimmedSubjets;
  const double rFilt;
  const double trimPtFracMin;
  const bool   useSubjetCountingCA;
  const double ptCut;
  const double mCutoff;
  const double yCut;
  const double rMin;

  const bool doPickyJets;
  const int  pickyMaxSplits;
  PickyJetSplitting * splitter;

  fastjet::JetAlgorithm jetAlgo;
  edm::EDGetTokenT<reco::GenParticleCollection> genMotherParticlesToken_;
  edm::EDGetTokenT<pat::PackedGenParticleCollection> genParticlesToken_;
  edm::Handle<reco::GenParticleCollection>      genMotherParticles;
  edm::Handle<pat::PackedGenParticleCollection> genParticles;
  };

}
#endif
