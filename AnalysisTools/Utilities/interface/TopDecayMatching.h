/*
 * JetFlavorMatching.h
 *
 *  Created on: Aug 27, 2014
 *      Author: nmccoll
 */

#ifndef TOPDECAYMATCHING_H_
#define TOPDECAYMATCHING_H_

#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/PatCandidates/interface/PackedGenParticle.h"
#include "AnalysisTools/Utilities/interface/Types.h"


namespace TopDecayMatching {

//_____________________________________________________________________________
// Class to hold matching partons
//_____________________________________________________________________________
class Parton
{
public:
  reco::GenParticleRef                parton;       ///< The particle of interest
  pat::PackedGenParticleRefVector     finalPrts;    ///< All final decay products of this hadron.
  ucsbsusy::CartLorentzVector         sumFinal;     ///< Vector sum momenta of all decay products.
  bool                                hasSinglet;   ///< has at least one singlet
  std::vector<std::pair<double,int>>  tempFinal;     ///< Temporary list of particles up for adding.
  Parton(const edm::Handle<reco::GenParticleCollection>& particles, int index)
    : parton    (particles, index)
    , hasSinglet(false)
  { }
  void addFinalParticle(pat::PackedGenParticleRef p){
      finalPrts.push_back(p);
      sumFinal += p->p4();
    }
  //helper functions for the DR matching
  double pt() const {return parton->pt();}
  double eta() const {return parton->eta();}
  double phi() const {return parton->phi();}
};

typedef   std::vector<Parton>         Partons;
typedef   edm::Ref<Partons>           PartonRef;
typedef edm::RefVector<Partons>       PartonRefVector;


template<typename Particle>
struct GreaterPartonPT : public std::binary_function<const Particle&, const Particle&, bool> {
  bool operator()(const Particle& h1, const Particle& h2) const
  { return h1.parton->pt() > h2.parton->pt(); }
};

//_____________________________________________________________________________
// Class to hold decay group info...usually just color singlets
//_____________________________________________________________________________
class ColorSinglet
{
public:
  PartonRefVector                             inPartons;          ///< All particles created in the singlet
  PartonRefVector                             incomingParticles; ///< All singlet members without a parton mother
  reco::GenParticleRefVector                  singletParticles;   ///< All particles created in the singlet
  reco::GenParticleRefVector                  decayProducts;    ///< Direct decay products of the group....the unique identifitier
  std::vector<int>                            matchIdxs;        //vector of particles to be checked against for matching

  ColorSinglet(Partons& partons,Partons& incomingPartons,const edm::Handle<reco::GenParticleCollection>& particles, int index) {
    addSingletPrt(partons,incomingPartons,particles,index);
    decayProducts.reserve(singletParticles[0]->numberOfDaughters());
    for(unsigned int iD = 0; iD < singletParticles[0]->numberOfDaughters(); ++iD)
      decayProducts.push_back(singletParticles[0]->daughterRef(iD));
  }

  bool equivDecays(const ColorSinglet& other) const {
    //never combine empty singlets
    if(!decayProducts.size() || !other.decayProducts.size()) return false;
    if(decayProducts.size() != other.decayProducts.size()) return false;
    for(unsigned int iD = 0; iD < decayProducts.size(); ++iD){
      if(decayProducts[iD].key() != other.decayProducts[iD].key() ) return false;
    }
    return true;
  }

  bool equivInputs(const ColorSinglet& other) const {
    if(incomingParticles.size() || other.incomingParticles.size()) return false;
    if(inPartons.size() != other.inPartons.size()) return false;
    for(unsigned int iD = 0; iD < inPartons.size(); ++iD){
      if(inPartons[iD].key() != other.inPartons[iD].key() ) return false;
    }
    return true;
  }

  // count total number of incomming and parton inputs
  static void getSingletMotherCompositon( const edm::Handle<reco::GenParticleCollection>& particles, const int idx, std::vector<int>& partonIDXs, std::vector<int>& incommingIDxs);
  //add a singlet particle to the group
  void addSingletPrt( Partons& partons, Partons& incomingPartons, const edm::Handle<reco::GenParticleCollection>& particles, int index);

  //remove duplicate inPartons
  void cleanupInPartons();

  // get all possible matching particles
  void addDaughterKeys(reco::GenParticleRef p);
  void fillMatchIdxs();


};

typedef   std::vector<ColorSinglet>         ColorSinglets;


//_____________________________________________________________________________
/// Choose the outgoing doc q/g's (not tops)
//_____________________________________________________________________________
void getMatchingPartons(const edm::Handle<reco::GenParticleCollection>& particles, Partons& partons);

//_____________________________________________________________________________
/// Get color singlets for matching
//_____________________________________________________________________________
bool intermediateMother(const reco::GenParticle * p);
void getColorSinglets(const edm::Handle<reco::GenParticleCollection>& particles, Partons& partons,
    ColorSinglets& colorSinglets, Partons& incomingPartons
);

//_____________________________________________________________________________
/// assign final decay products to singlets or empty vector
//_____________________________________________________________________________
bool outgoingLeptonDaughter(const reco::GenParticle * p);
void associateFinalParticles(const edm::Handle<reco::GenParticleCollection>& particles, const edm::Handle<pat::PackedGenParticleCollection>& finalParticles,
    const ColorSinglets& colorSinglets, std::vector<std::vector<pat::PackedGenParticleRef>>& assocToColorSinglets, std::vector<pat::PackedGenParticleRef>& nonAssoc);

////_____________________________________________________________________________
/////Match final particles to nearest neighbor
////Assumes that the tempFinal particles is already cleared
/// Will fill it with the nearest neighbors and remove it from the finalParticles list
////_____________________________________________________________________________
void matchDecayProducts(std::vector<Parton*>& matchingPartons, std::vector<pat::PackedGenParticleRef>& finalParticles, const double maxDR);
////_____________________________________________________________________________
// add all temporary particles to the parton
// if relPT >= 0, add the particles (sorted by DR) if their (sumFinal + newPart).pt() > relPT*partonPT
// if you do not add the particle you can then fill it into the some vector
// does sorting and cleaning for you
////_____________________________________________________________________________
void addParticlesToParton(Parton& parton, double maxRelPT, std::vector<pat::PackedGenParticleRef>& particleDump,
    const edm::Handle<pat::PackedGenParticleCollection>& finalParticles  );

//_____________________________________________________________________________
/// pass 1: assign final particles with nearest neighbor partons (when match is less than some value)
// ---partons without singlets get to run on the nonAssoc list by themselves
//_____________________________________________________________________________
void associateToNearestPartons(Partons& partons, const ColorSinglets& colorSinglets, const double maxDR,const edm::Handle<pat::PackedGenParticleCollection>& finalParticles,
    std::vector<std::vector<pat::PackedGenParticleRef>>& assocToColorSinglets, std::vector<pat::PackedGenParticleRef>& nonAssoc);
//_____________________________________________________________________________
/// pass 2: assign final particles to nearest neighbors within maxDR and maxDPT for singlets only
//_____________________________________________________________________________
void associateSingletsLoosely(Partons& partons,Partons& incomingPartons, const ColorSinglets& colorSinglets, const double maxDR,const double maxRelPT,
    const edm::Handle<pat::PackedGenParticleCollection>& finalParticles,std::vector<std::vector<pat::PackedGenParticleRef>>& assocToColorSinglets);

//_____________________________________________________________________________
/// pass 3: associate the rest
//_____________________________________________________________________________
void associateRemaining(Partons& partons,Partons& incomingPartons, const ColorSinglets& colorSinglets, const double maxDR,const double maxRelPT,
    const edm::Handle<pat::PackedGenParticleCollection>& finalParticles,std::vector<std::vector<pat::PackedGenParticleRef>>& assocToColorSinglets,
    std::vector<pat::PackedGenParticleRef>& nonAssoc
    );

//Class to organize the decay
class TopDecay {
public:
  reco::GenParticleRef top;
  reco::GenParticleRef b;
  reco::GenParticleRef W;
  reco::GenParticleRef W_dau1;
  reco::GenParticleRef W_dau2;


  bool isLeptonic;

  PartonRef b_decay;
  PartonRef w1_decay;
  PartonRef w2_decay;

  TopDecay(const edm::Handle<reco::GenParticleCollection>& particles, const int idx) :top(particles,idx),isLeptonic(false) {};
};

typedef std::vector<TopDecay> TopDecays;
void fillTopDecays(const edm::Handle<reco::GenParticleCollection>& particles,const Partons& partons, TopDecays& topDecays);



//

};

//#include "AnalysisTools/Utilities/src/JetFlavorMatching.icc"

#endif /* JETFLAVORMATCHING_H_ */
