#ifndef OBJECTPRODUCERS_JETPRODUCERS_REDEFINEDJETPRODUCERS_H
#define OBJECTPRODUCERS_JETPRODUCERS_REDEFINEDJETPRODUCERS_H

#include <functional>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include <fastjet/JetDefinition.hh>

#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "DataFormats/PatCandidates/interface/PackedGenParticle.h"

class RedefinedJetProducer : public edm::EDProducer {
public:
  RedefinedJetProducer(const edm::ParameterSet& iConfig);
  virtual ~RedefinedJetProducer(){};
  virtual void produce(edm::Event& iEvent, const edm::EventSetup& iSetup);
  virtual void load(edm::Event& iEvent, const edm::EventSetup& iSetup);

   static bool isPUParticle(const pat::PackedCandidate& p) {return p.fromPV() == 0; }
   static bool isNonPUParticle(const pat::PackedCandidate& p) {return !isPUParticle(p); }

protected:
  const int isRealData;
  const bool produceGen;
  const bool producePU;
  const edm::InputTag src;
  const edm::InputTag genSrc;
  const std::string jetAlgorithmName;
  const double rParameter;
  const double jetPtMin;
  const double minParticlePT;
  const double maxParticleEta;
  const double ghostArea;

  fastjet::JetAlgorithm jetAlgo;
  edm::Handle<pat::PackedCandidateCollection> pfCandidates;
  edm::Handle<pat::PackedGenParticleCollection> genParticles;
  };

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

DEFINE_FWK_MODULE(RedefinedJetProducer);
#endif
