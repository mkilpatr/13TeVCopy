#ifndef ANALYSISTOOLS_DATAFORMATS_GENPARTICLE_H
#define ANALYSISTOOLS_DATAFORMATS_GENPARTICLE_H


#define FLVRECOASSOC

#include <vector>
#include <iostream>

#include "AnalysisTools/DataFormats/interface/Momentum.h"

namespace ucsbsusy {

template <class Type> class CandidateRef;

template <class CoordSystem>
class GenParticle : public Momentum<CoordSystem>
{
public :
  typedef size16 stor;

  GenParticle() : status_(-1), pdgId_(-1), nMoms_(0), firstMom_(0),nDaus_(0),firstDau_(0),assocList_(0),genParticles_(0) {}

  template <class InputCoordSystem>
  GenParticle(ROOT::Math::LorentzVector<InputCoordSystem> inMomentum,std::vector<GenParticle<CoordSystem>> * genParticles = 0) : Momentum<CoordSystem>(inMomentum),
  status_(-1), pdgId_(-1), nMoms_(0), firstMom_(0),nDaus_(0),firstDau_(0),assocList_(0),genParticles_(genParticles) {}
  ~GenParticle(){}

  void setGenPrtPtr(std::vector<GenParticle<CoordSystem>> * genParticles) {genParticles_ = genParticles;}
  void setStorage(const size8 status, const int pdgId,
      const stor nMoms, const stor firstMom, const stor nDaus, const stor firstDau, const std::vector<stor> * assocList){
    status_       = status      ;
    pdgId_        = pdgId       ;
    nMoms_        = nMoms       ;
    firstMom_     = firstMom    ;
    nDaus_        = nDaus       ;
    firstDau_     = firstDau    ;
    assocList_    = assocList   ;
  }

  int status()       const { return status_;}
  int pdgId()       const { return pdgId_; }

  stor numberOfMothers() const {return nMoms_;}
  const GenParticle<CoordSystem> * mother(const stor idx)const{
    if(assocList_ == 0) throw ("GenParticle::mother() -> storage not loaded!");
    if(idx >= nMoms_ ) throw ("GenParticle::mother() -> invalid mother index");
    return &genParticles_->at(assocList_->at(firstMom_ + idx));
  }
  CandidateRef<GenParticle<CoordSystem> > motherRef(const stor idx) const {
    return CandidateRef<GenParticle<CoordSystem> >(mother(idx),assocList_->at(firstMom_ + idx));
  }

  stor numberOfDaughters() const {return nDaus_;}
  const GenParticle<CoordSystem> * daughter(const stor idx)const{
    if(assocList_ == 0) throw ("GenParticle::daughter() -> storage not loaded!");
    if(idx >= nDaus_ ) throw ("GenParticle::daughter() -> invalid mother index");
    return &genParticles_->at(assocList_->at(firstDau_ + idx));
  }
  CandidateRef<GenParticle<CoordSystem> > daughterRef(const stor idx) const {
    return CandidateRef<GenParticle<CoordSystem> >(daughter(idx),assocList_->at(firstDau_ + idx));
  }


  //Dummy functions used to add compatibility with reco::GenParticles
  int charge() const {return 0;}


protected :
  size8  status_  ;
  int    pdgId_   ;
  stor   nMoms_   ;
  stor   firstMom_;
  stor   nDaus_   ;
  stor   firstDau_;
  const std::vector<stor> * assocList_;
  const std::vector<GenParticle<CoordSystem>> * genParticles_;


};
typedef GenParticle<CylLorentzCoordF> GenParticleF;
typedef std::vector<GenParticleF>     GenParticleFCollection;


//dummy class to add compatibility with GenParticle
template <class Type>
class CandidateRef {
public:
  CandidateRef(const Type * inptr, const int inidx) : ptr(inptr), idx(inidx) {}
  const Type& operator->() const {return *ptr;}
  const Type& operator* () const {return *ptr;}
  int key() const {return idx;}

private:
  const Type * ptr;
  const int idx;
};

}

#endif
