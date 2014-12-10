/*
 * JetFlavorMatching.h
 *
 *  Created on: Aug 27, 2014
 *      Author: nmccoll
 */

#ifndef JETFLAVORMATCHING_H_
#define JETFLAVORMATCHING_H_

#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/PatCandidates/interface/PackedGenParticle.h"


#include "AnalysisTools/Utilities/interface/Types.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"
#include "AnalysisTools/Utilities/interface/JetFlavorInfo.h"

namespace JetFlavorMatching {

//Matching info


//_____________________________________________________________________________
// Class to hold hadron decay info....used for jet flavor matching
//_____________________________________________________________________________
class ParticleDecay
{
public:
  reco::GenParticleRef                particle;     ///< The particle of interest.
  std::vector<size>                   decayInts;    ///< All decay products of this hadron.
  CartLorentzVector                   sumVisible;   ///< Vector sum momenta of visible (non-neutrino) decay products.
  CartLorentzVector                   sumCharged;   ///< Vector sum momenta of charged decay products.
  int                                 numVisible;   ///< Number of visible (non-neutrino) decay products.
  int                                 numCharged;   ///< Number of charged decay products.
  ParticleDecay(const edm::Handle<reco::GenParticleCollection>& particles, int index)
    : particle    (particles, index)
    , numVisible(0)
    , numCharged(0)
  { }
};

typedef   edm::Ref<std::vector<ParticleDecay> >           ParticleDecayRef;
typedef   std::pair<ParticleDecayRef, CartLorentzVector>  ParticleContainment;
typedef   std::vector<ParticleContainment>                ParticleContainments;


template<typename Particle>
struct GreaterParticlePT : public std::binary_function<const Particle&, const Particle&, bool> {
  bool operator()(const Particle& h1, const Particle& h2) const
  { return h1.particle->pt() > h2.particle->pt(); }
};

template<typename Particle>
struct GreaterParticleE : public std::binary_function<const Particle&, const Particle&, bool> {
  bool operator()(const Particle& h1, const Particle& h2) const
  { return h1.particle->energy() > h2.particle->energy(); }
};

  //_____________________________________________________________________________
  /// Gets the list of B+C hadrons
  //_____________________________________________________________________________
  void getHadronDecays(const edm::Handle<std::vector<reco::GenParticle> >& particles,
      std::vector<ParticleDecay>& bDecays, std::vector<ParticleDecay>& cDecays );
  //_____________________________________________________________________________
  /// Gets the list of partons (with the option to skip heavy partons)
  //_____________________________________________________________________________
  template<typename Particle>
  bool isMatchingParton( const Particle& particle);
  void getPartons(const edm::Handle<std::vector<reco::GenParticle> >& particles,
      std::vector<ParticleDecay>& partonDecays, const bool skipHeavyFlavor);
  //_____________________________________________________________________________
  /// collect all final decay products
  //_____________________________________________________________________________
  void tagDecays(const int tag,reco::GenParticleRef  particle, std::vector<int>& bHadronMatches);
  template<typename Particle>
  void addDecayProduct(ParticleDecay& decay, Particle& decayProduct, const int index);
  //
  // Gives prefecence to b hadrons then c hadrons, then partons (sorted by pT) when the decay product can be
  // associated to multiple sources (if a vector is set to 0, skip that set)
  // the last piece stores a vector of associations between partons and genparticles for faster flavor tagging
  void associateDecayProducts(const edm::Handle<std::vector<reco::GenParticle> > & particles, const edm::Handle<pat::PackedGenParticleCollection>& finalParticles,
      std::vector<ParticleDecay>* bDecays,std::vector<ParticleDecay>* cDecays, std::vector<ParticleDecay>* partonDecays
      , std::vector<int>* partonParticleAssoc);
  //_____________________________________________________________________________
  /// Associate hadrons to genJets by tracing constituents.
  //_____________________________________________________________________________
  template<typename Object, typename Particle>
  void associateHadronsToJets        (
                                        const edm::Handle<std::vector<Particle> >& particles
                                      , const std::vector<Object>&   jets
                                      , const std::vector<ParticleDecay>&    hadronDecays
                                      , std::vector<ParticleContainments>&   mainHadrons        ///< B hadrons whose max contained energy is in this indexed jet.
                                      , std::vector<ParticleContainments>&   satelliteHadrons   ///< B hadrons who have some sub-leading containment in this indexed jet.
                                      );
  //_____________________________________________________________________________
  /**
      Finds the constituents of a given object that are present in the given list of particles.
      Returns the number of such constituents and stores the vector sum (all and visible only)
      in the provided output objects.
   */
  //_____________________________________________________________________________
  template<typename Object, typename Particle>
  int matchConstituents(const std::vector<Particle>& particles,const Object& object, const std::vector<size>& particleInts
      , CartLorentzVector& sumMatched, CartLorentzVector& sumVisible, CartLorentzVector& sumCharged, int& numVisible, int& numCharged
  );

  //_____________________________________________________________________________
  /// Match hadrons to jets, based on the mainHadron, and set flavor to setFlavor
  // output total hadron pT of that type
  // assumes that the outputs: flavors, matched particles, hadron PTs is correct in size
  // WILL NOT OVERRIDE A MATCHED JET
  //_____________________________________________________________________________
  template<typename Object>
  void assignJetHadronFlavors( const std::vector<Object>& jets, const JetFlavorInfo::JetFlavor setFlavor,
      const std::vector<ParticleContainments>&   mainHadrons, std::vector<ParticleContainments>&   satteliteHadrons,
      std::vector<JetFlavorInfo::JetFlavor>& flavors, std::vector<float>& hadronPTs,std::vector<std::vector<ParticleDecayRef> > & matchedParticles
  );
  //_____________________________________________________________________________
  /// Match partons to jets, based on the one with the greatest pT in some radius cone. (squared)
  //  if cusRad2 != 0 then use that radius for each jet (usefull for variable cone jets)
  //_____________________________________________________________________________
  template<typename Object>
  void assignJetPartonFlavors( const std::vector<Object>& jets, std::vector<ParticleDecay>& partonDecays,
      std::vector<JetFlavorInfo::JetFlavor>& flavors, std::vector<std::vector<ParticleDecayRef> > & matchedParticles,
      double maxDR2, std::vector<double>* cusRad2 = 0
  );

  //_____________________________________________________________________________
  /// Match partons to jets, based on the one with the largest contribution
  // assumes that the outputs: flavors, matched particles, is correct in size
  // WILL NOT OVERRIDE A MATCHED JET
  //_____________________________________________________________________________
  //Version that doesnt work because mini aod links only to the first gen mother...
  //this means that color singlets dont really make sense when tracing
  template<typename Object>
  void assignJetPartonFlavors( const std::vector<Object>& jets,
      std::vector<ParticleDecay>& partonDecays, std::vector<int>& partonParticleAssoc,
      std::vector<JetFlavorInfo::JetFlavor>& flavors, std::vector<std::vector<ParticleDecayRef> > & matchedParticles,
      std::vector<ParticleContainments> & partonContainments
  );
}

#include "AnalysisTools/Utilities/src/JetFlavorMatching.icc"

#endif /* JETFLAVORMATCHING_H_ */
