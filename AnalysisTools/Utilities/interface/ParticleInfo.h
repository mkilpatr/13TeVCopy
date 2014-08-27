/**
  @file         ParticleInfo.h
  @author       Sue Ann Koay (sakoay@cern.ch)
*/


#ifndef PARTICLEINFO_H
#define PARTICLEINFO_H

#include <vector>
#include <TString.h>

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

namespace ucsbsusy{

class ParticleInfo
{
protected:
  ParticleInfo()  { }

public:


enum JetFlavor      { unmatched_jet, uds_jet, c_jet, b_jet, g_jet, hf_jet, lf_jet, photon_jet, tau_jet, numJetFlavors };

enum  HadronType  { OTHER
                  , /*PI0,*/ PIPLUS, ETA0, KAON, DIQUARK, LIGHT_MESON, LIGHT_BARYON
                  , DPLUS, D0, DSPLUS, LAMBDACPLUS, C_MESON, C_BARYON
                  , BPLUS, B0, BSPLUS, LAMBDAB0, B_MESON, B_BARYON
                  , numDetailedHadronTypes
                  };

enum ParticleStatus { status_unknown, status_final, status_decayed, status_doc };
enum ParticleID     { p_unknown, p_d, p_u, p_s, p_c, p_b, p_t, p_bprime, p_tprime,
                      p_eminus = 11, p_nu_e, p_muminus, p_nu_mu, p_tauminus, p_nu_tau,
                      p_tauprimeminus, p_nu_tauprime, p_g = 21, p_gamma, p_Z0,
                      p_Wplus, p_h0, p_Zprime0 = 32, p_Zpprime0, p_Wprimeplus, p_H0,
                      p_A0, p_Hplus, p_G = 39, p_R0 = 41, p_H30 = 45, p_A20 = 46,
                      p_LQ, p_cluster = 91, p_string,
                      p_pi0 = 111, p_rho0 = 113, p_klong = 130, p_piplus = 211, p_rhoplus = 213, p_eta = 221, p_omega = 223,
                      p_kshort = 310, p_k0, p_kstar0 = 313, p_kplus = 321, p_kstarplus = 323, p_phi = 333,
                      p_dplus = 411, p_d0 = 421, p_dsplus = 431, p_bplus = 511, p_b0 = 521,
                      p_bsplus = 531, p_bcplus = 541, 
                      p_neutron = 2112, p_proton = 2212,
                      p_sigmaminus = 3112, p_lambda0 = 3122,
                      p_sigma0 = 3212, p_sigmaplus = 3222, p_ximinus = 3312, p_xi0 = 3322, p_omegaminus = 3334,
                      p_sigmac0 = 4112, p_lambdacplus = 4122, p_xic0 = 4132,
                      p_sigmacplus = 4212, p_sigmacpp = 4222, p_xicplus = 4232, p_omegac0 = 4332,
                      p_sigmabminus = 5112, p_lambdab0 = 5122, p_xibminus = 5132, p_sigmab0 = 5212, p_sigmabplus = 5222,
                      p_xib0 = 5232, p_omegabminus = 5332,
                      p_sdownL = 1000001, p_supL, p_sstrangeL, p_scharmL, p_sbottom1, p_stop1,
                      p_selectronL = 1000011, p_snu_eL, p_smuonL, p_snu_muL, p_stau1, p_snu_tauL, 
                      p_gluino = 1000021, p_chi10, p_chi20, p_chi1plus, p_chi30, 
                      p_chi40 = 1000035, p_chi2plus = 1000037, p_gravitino = 1000039, p_chi50 = 1000045,
                      p_sdownR = 2000001, p_supR, p_sstrangeR, p_scharmR, p_sbottom2, p_stop2,
                      p_selectronR = 2000011, p_snu_eR, p_smuonR, p_snu_muR, p_stau2, p_snu_tauR
                    };
enum  { p_LSP = 1000022 };


//_____________________________________________________________________________
// Check particle type from PDGID
//_____________________________________________________________________________

static bool isJetSource(int particleID);
static bool isLightQuark(int particleID);
static bool isHeavyQuark(int particleID);
static bool isQuark(int pdgId);
static bool isEWKBoson(int particleID);
static bool isHadron(int pdgId);
static bool isQuarkOrGluon(int pdgId);
static bool isDecayProduct(int pdgId);
static bool isThirdGeneration(int pdgId);
static bool isANeutrino(int particleID);
static bool isInvisible(int particleID);
static bool isVisible(int particleID);
static bool isLepton(int particleID);
static bool isLeptonOrNeutrino(int particleID);
static bool isPion(int particleID);
static bool isKaon(int particleID);
static bool isBSM(int particleID);
static bool isSquark(int pdgId);
static bool isSquarkOrGluino(int pdgId);
static bool isLSP(int pdgId);
static bool isHadronizationModel(int particleID);

//_____________________________________________________________________________
//check particle status
template<typename Particle>
static bool isOutgoing(const Particle* particle);

//_____________________________________________________________________________
// Original/Final version of the particle
//_____________________________________________________________________________

/// Traces particle back up the chain of radiation vertices to the first instance of the same particle.
template<typename Particle>
static const Particle* getOriginal(const Particle* particle);

/// Traces particle back up the chain of radiation vertices to the first instance of the same particle.
template<typename ParticleRef, typename Container>
static ParticleRef getOriginal(ParticleRef particle, const Container& particles);

/// Traces particle down the chain of radiation vertices to the last instance of the same particle.
template<typename Particle>
static const Particle* getFinal(const Particle* particle, int maxNumDaughters = -1);

/// Traces particle down the chain of radiation vertices to the last instance of the same particle.
template<typename ParticleRef, typename Container>
static ParticleRef getFinal(ParticleRef particle, const Container& particles, int maxNumDaughters = -1);

template<typename Particle, typename OutParticle>
static int getOutgoing(const Particle* particle, std::vector<OutParticle>& output, bool (*vetoID)(int) = 0);

template<typename Particle, typename OutParticle>
static int getDecayProducts(const Particle* particle, std::vector<OutParticle>& output, bool (*vetoID)(int) = 0, std::vector<OutParticle>* vetoed = 0);

template<typename Particle>
static bool isLastInChain(const Particle* particle);

template<typename Particle>
static std::vector<const Particle*> getProgenitors(const std::vector<Particle>& particles);

template<typename Particle>
static std::vector<const Particle*> getDecayProducts(const std::vector<const Particle*>& progenitors);


//_____________________________________________________________________________
// Return the parton name and information as a string
//_____________________________________________________________________________

static TString        smPartonName(int particleID, bool separateL = false, bool separateU = false, bool separateD = false, bool separateB = false, bool separateAnti = false);
static TString        smPartonTitle(int particleID, bool separateL = false, bool separateU = false, bool separateD = false, bool separateB = false, bool separateAnti = false);
static TString        bsmPartonName(int particleID, bool separateU = false, bool separateB = true, bool separateLR = false);
static TString        flavorName(int particleID);
static TString        shortFlavorName(int particleID);
static JetFlavor      jetFlavor(int particleID);
static bool           isLightFlavorJet(JetFlavor flavor);
static bool           isHeavyFlavorJet(JetFlavor flavor);
static const TString* pfTypeNames();
static const TString* pfTypeTitles();
static const TString* jetFlavorNames();
static const TString& jetFlavorName(JetFlavor flavor);
static const TString& jetFlavorTag(JetFlavor flavor);
static const TString* jetFlavorTags();
static const char*    specialJetFlavor(JetFlavor flavor, JetFlavor special);
static TString        multiply(int count, const char label[]);
static TString        formatFlavors(const std::vector<int>& counts);
static TString        formatFlavors(const std::vector<int>& counts, const std::vector<int>& antiCounts);
static TString        nameFor(int pdgId, int charge);
static TString        nameFor(int pdgId);

template<typename Object>
static TString nameFor(const Object& object);

static TString titleFor(int pdgId, int charge);
static TString titleFor(int pdgId);

template<typename Object>
static TString titleFor(const Object& object);

static TString shortTitleFor(int pdgId, bool ignoreLR = false);
static TString toMathematica(TString text);

//_____________________________________________________________________________
// Hadron type information
//_____________________________________________________________________________

static JetFlavor jetFlavor(HadronType type);
static bool isHeavyFlavor(HadronType type);
static bool isBHadron(HadronType type);
static HadronType typeOfHadron(int pdgId, int* numBQuarks = 0, int* numCQuarks = 0);
static HadronType detailedTypeOfHadron(int pdgId);
static const TString& hadronTypeName(HadronType type);
static const TString& hadronTypeTitle(HadronType type);
static TString hadronTypeName(HadronType type, int pdgId);
static TString hadronTypeTitle(HadronType type, int pdgId);


//_____________________________________________________________________________
// Classify particles
//_____________________________________________________________________________

struct LesserIDorGreaterPT {
  template<typename ObjectRef>
  bool operator()(const ObjectRef& x, const ObjectRef& y) const 
  {
    const int         xID   = TMath::Abs(x->pdgId());
    const int         yID   = TMath::Abs(y->pdgId());
    if (xID < yID)    return true ;
    if (xID > yID)    return false;
    return x->pt() >= y->pt();
  }
};

template<typename Particle>
static TString classifyInitialFlavors(const std::vector<Particle>& genParticles);

template<typename Particle>
static TString classifyIncomingFlavors(const std::vector<Particle>& genParticles);

template<typename Particle>
static TString classifyOutgoingFlavors(const std::vector<Particle>& genParticles, const std::vector<int>& mePartonIndex, const unsigned int numMEPartons);

template<typename Particle>
static TString classifyProduction(const std::vector<Particle>& genParticles, JetFlavor special);

template<typename Particle>
static int countWithStatus(const std::vector<Particle>& particles, int status, bool (*test)(int) = 0, bool shortCircuit = false);

template<typename Particle>
static int countWithID(const std::vector<Particle>& particles, int pdgId, int status = -1, bool checkCharge = false, bool shortCircuit = false);

template<typename Particle>
static int countProducedID(const std::vector<Particle>& particles, int pdgId, bool checkCharge = false, bool shortCircuit = false, unsigned int firstProduced = 6, unsigned int minNumParents = 2);


//_____________________________________________________________________________
//    Particle Information
//_____________________________________________________________________________

static double poleMass(int pdgId);


//_____________________________________________________________________________
//    Print GenParticle history
//_____________________________________________________________________________

/// Prints the history (ancestors and their decays) of the indexed genParticle.
static std::ostream& printGenHistory(const std::vector<reco::GenParticle>& genParticles, const unsigned int particleIndex);

/// Prints the entire particle creation/decay history, for the first genBound number of genParticles.
static void printGenInfo(const std::vector<reco::GenParticle>& genParticles, int genBound = 30);

};  // end class ParticleInfo

}

#include "AnalysisTools/Utilities/src/ParticleInfo.icc"

#endif //PARTICLEINFO_H
