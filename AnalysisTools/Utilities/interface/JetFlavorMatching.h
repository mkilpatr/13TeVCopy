/*
 * JetFlavorMatching.h
 *
 *  Created on: Aug 27, 2014
 *      Author: nmccoll
 */

#ifndef JETFLAVORMATCHING_H_
#define JETFLAVORMATCHING_H_

#include "DataFormats/JetReco/interface/GenJet.h"
#include "SimDataFormats/JetMatching/interface/JetFlavourMatching.h"
#include "DataFormats/PatCandidates/interface/Jet.h"

#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"


namespace ucsbsusy {
//_____________________________________________________________________________
// Class to hold hadron decay info....used for jet flavor matching
//_____________________________________________________________________________
class HadronDecay
{
public:
  reco::GenParticleRef                hadron;       ///< The hadron of interest.
  std::vector<reco::GenParticleRef>   quark;        ///< All quarks that hadronize into this hadron.
  std::vector<reco::GenParticleRef>   decay;        ///< All decay products of this hadron.
  CartLorentzVector                   sumVisible;   ///< Vector sum momenta of visible (non-neutrino) decay products.
  CartLorentzVector                   sumCharged;   ///< Vector sum momenta of charged decay products.
  int                                 numVisible;   ///< Number of visible (non-neutrino) decay products.
  int                                 numCharged;   ///< Number of charged decay products.
  HadronDecay(const edm::Handle<reco::GenParticleCollection>& particles, int index)
    : hadron    (particles, index)
    , numVisible(0)
    , numCharged(0)
  { }
};

typedef   edm::Ref<std::vector<HadronDecay> >           HadronDecayRef;
typedef   std::pair<HadronDecayRef, CartLorentzVector>  HadronContainment;
typedef   std::vector<HadronContainment>                HadronContainments;


template<typename Hadron>
struct GreaterHadronPT : public std::binary_function<const Hadron&, const Hadron&, bool> {
  bool operator()(const Hadron& h1, const Hadron& h2) const
  { return h1.hadron->pt() > h2.hadron->pt(); }
};

template<typename Hadron>
struct GreaterHadronE : public std::binary_function<const Hadron&, const Hadron&, bool> {
  bool operator()(const Hadron& h1, const Hadron& h2) const
  { return h1.hadron->energy() > h2.hadron->energy(); }
};




class JetFlavorMatching {
public:
  JetFlavorMatching();
  virtual ~JetFlavorMatching();

  //_____________________________________________________________________________
  // How we define flavor for genjets
  // Called "taggable" because we say a jet is a b-jet if the gen jet is in pT
  // and eta acceptance
  //_____________________________________________________________________________
  enum TaggableType         { TAGGABLE_MONO_B, TAGGABLE_MULTI_B, TAGGABLE_CHARM, TAGGABLE_LIGHT, TAGGABLE_GLUON, TAGGABLE_NONB, numTaggableTypes };
  static const TString        TAGGABLE_NAME [numTaggableTypes+1];

  //_____________________________________________________________________________
  // Strings for storage in PAT user info
  //_____________________________________________________________________________
  static const std::string    STORED_BHAD_INFO;   ///< userInt label to whether or not bHadron info has been added (or attempted to be added) to the jet
  static const std::string    NUM_FROM_ME;        ///< userInt label for number of matrix element sources
  static const std::string    MAIN_B_P4;          ///< userCand label for p4 contained in various main-associated B hadrons.
  static const std::string    SATELLITE_B_P4;     ///< userCand label for p4 contained in various satellite B hadrons.
  static const std::string    MAIN_B_INDEX;       ///< userCand label for indices of various main-associated B hadrons.
  static const std::string    SATELLITE_B_INDEX;  ///< userCand label for indices of various satellite B hadrons.

  //_____________________________________________________________________________
  /**
    Match a given list of particles to the list of jets, by dR proximity, and
    allowing multiple matches in a second round.
  */
  //_____________________________________________________________________________
  template<typename Jet, typename Particle>
  static std::vector<std::vector<reco::GenParticleRef> > matchParticlesToJets ( const std::vector<Jet>& jets, const std::vector<Particle>& particles
                                                                    , double maxDR = 0.5, int pdgId = ParticleInfo::p_b, int status = ParticleInfo::status_decayed);

  //_____________________________________________________________________________
  /**
    Finds the constituents of a given object that are present in the given list of particles.
    Returns the number of such constituents and stores the vector sum (all and visible only)
    in the provided output objects.
  */
  //_____________________________________________________________________________
  template<typename Object, typename Particle>
  static int matchConstituents( const Object& object, const std::vector<edm::Ref<std::vector<Particle> > >& particles
                              , CartLorentzVector& sumMatched, CartLorentzVector& sumVisible, CartLorentzVector& sumCharged, int& numVisible, int& numCharged
                              );

  //_____________________________________________________________________________
  /// Traces and stores all status final daughters down the decay chain of the given particle.
  //_____________________________________________________________________________
  template<typename Particle>
  static void getDecayProducts( int index, const edm::Handle<std::vector<Particle> >& particles, std::vector<edm::Ref<std::vector<Particle> > >& decayProducts
                              , CartLorentzVector& sumVisible, CartLorentzVector& sumCharged, int& numVisible, int& numCharged
                              );

  //_____________________________________________________________________________
  /// Gets the list of B hadrons, their originating quarks, and their visible/invisible decay products.
  //_____________________________________________________________________________
  static std::vector<HadronDecay> getBHadronDecays(const edm::Handle<std::vector<reco::GenParticle> >& particles);

  //_____________________________________________________________________________
  /// Associate B hadrons to genJets by tracing constituents.
  //_____________________________________________________________________________
  static void associateBHadronsToJets ( const std::vector<reco::GenJet>&   jets
                                      , const std::vector<HadronDecay>&    bHadronDecays
                                      , std::vector<HadronContainments>&   mainBHadrons        ///< B hadrons whose max contained energy is in this indexed jet.
                                      , std::vector<HadronContainments>&   satelliteBHadrons   ///< B hadrons who have some sub-leading containment in this indexed jet.
                                      );
  //_____________________________________________________________________________
  /// Stores B hadron info into pat::Jets according to its corresponding genJet.
  //_____________________________________________________________________________
  static void storeBHadronInfo( const std::vector<pat::Jet>&             jets
                              , const std::vector<HadronContainments>&   mainBHadrons
                              , const std::vector<HadronContainments>&   satelliteBHadrons
                              , const std::string&                       genJetLabel     = "GenPtr"
                              );

  //_____________________________________________________________________________
  /// Automagically calls associateBHadronsToJets() first.
  //_____________________________________________________________________________
  static void storeBHadronInfo( const std::vector<pat::Jet>& recoJets, const std::vector<reco::GenJet>& genJets, const std::vector<HadronDecay>& bHadronDecays );


  //_____________________________________________________________________________
  // Get jet info based on stored decay information
  //_____________________________________________________________________________
  static TaggableType getTaggableType(double jetEta, int numMainBHadrons, int partonFlavor, double etaAcceptance = 2.4);

  static TaggableType getTaggableType(const pat::Jet& jet, int* numMEpartons = 0, double etaAcceptance = 2.4);

  static TaggableType getPATTaggableType(const pat::Jet& jet, int* numMEpartons = 0, double etaAcceptance = 2.4);

  template<typename JetRef>
  static TaggableType getTaggableType(const JetRef& jet, const HadronContainments&  mainBHadronsInJet, const reco::JetFlavourMatchingCollection& jetFlavorMatch, double etaAcceptance = 2.4);

  static reco::GenParticleRef getMainBQuark(const pat::Jet& jet, const edm::Handle<reco::GenParticleCollection>& genParticles, size which = 0);

};

#include "AnalysisTools/Utilities/src/JetFlavorMatching.icc"

} /* namespace ucsbsusy */

#endif /* JETFLAVORMATCHING_H_ */
