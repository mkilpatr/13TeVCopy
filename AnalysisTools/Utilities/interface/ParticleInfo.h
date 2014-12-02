/**
  @file         ParticleInfo.h
  @author       Sue Ann Koay (sakoay@cern.ch)
*/


#ifndef PARTICLEINFO_H
#define PARTICLEINFO_H

#include <vector>
#include <TString.h>

namespace ParticleInfo
{
enum JetFlavor      { unmatched_jet, uds_jet, c_jet, b_jet, g_jet, hf_jet, lf_jet, photon_jet, tau_jet, numJetFlavors };

enum  HadronType  { OTHER
                  , /*PI0,*/ PIPLUS, ETA0, KAON, DIQUARK, LIGHT_MESON, LIGHT_BARYON
                  , DPLUS, D0, DSPLUS, LAMBDACPLUS, C_MESON, C_BARYON
                  , BPLUS, B0, BSPLUS, LAMBDAB0, B_MESON, B_BARYON
                  , numDetailedHadronTypes
                  };
enum ParticleStatus { FINAL,INTERMEDIATE, DOC_INTERMEDIATE, DOC_ALTERED, DOC_OUTGOING, INCOMING, UNKNOWN};
//enum ParticleStatus { status_unknown, status_final, status_decayed, status_doc };
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
// process status information
//_____________________________________________________________________________
//// is a final, non-decayed particle
bool isFinal(const int status);
//// is a non-final decayed particle
bool isIntermediate(const int status);
//Doc particle that will be altered before becoming outgoing
bool isDocIntermediate(const int status);
//An altered version of a doc particle in prep for becoming outgoing
bool isDocAltered(const int status);
//Doc particle that is outgoing
bool isDocOutgoing(const int status);
// Is incoming particle
bool isIncoming(const int status);

// is a documentaiton particle, a part of the original particles that define the event
bool isDoc(const int status);
ParticleStatus getStatus(const int status);

//_____________________________________________________________________________
// Check particle type from PDGID
//_____________________________________________________________________________

bool isJetSource(int particleID);
bool isLightQuark(int particleID);
bool isHeavyQuark(int particleID);
bool isQuark(int pdgId);
bool isEWKBoson(int particleID);
bool isHadron(int pdgId);
bool isQuarkOrGluon(int pdgId);
bool isDecayProduct(int pdgId);
bool isThirdGeneration(int pdgId);
bool isANeutrino(int particleID);
bool isInvisible(int particleID);
bool isVisible(int particleID);
bool isLepton(int particleID);
bool isLeptonOrNeutrino(int particleID);
bool isPion(int particleID);
bool isKaon(int particleID);
bool isBSM(int particleID);
bool isSquark(int pdgId);
bool isSquarkOrGluino(int pdgId);
bool isLSP(int pdgId);
bool isHadronizationModel(int particleID);

//_____________________________________________________________________________
// check if particle matches a given pdgId
template<typename Particle>
bool isA(int particleID, const Particle* p, bool checkCharge=false);

//_____________________________________________________________________________
// Return the parton name and information as a string
//_____________________________________________________________________________

TString        smPartonName(int particleID, bool separateL = false, bool separateU = false, bool separateD = false, bool separateB = false, bool separateAnti = false);
TString        smPartonTitle(int particleID, bool separateL = false, bool separateU = false, bool separateD = false, bool separateB = false, bool separateAnti = false);
TString        bsmPartonName(int particleID, bool separateU = false, bool separateB = true, bool separateLR = false);
TString        flavorName(int particleID);
TString        shortFlavorName(int particleID);
JetFlavor      jetFlavor(int particleID);
bool           isLightFlavorJet(JetFlavor flavor);
bool           isHeavyFlavorJet(JetFlavor flavor);
const TString* pfTypeNames();
const TString* pfTypeTitles();
const TString* jetFlavorNames();
const TString& jetFlavorName(JetFlavor flavor);
const TString& jetFlavorTag(JetFlavor flavor);
const TString* jetFlavorTags();
const char*    specialJetFlavor(JetFlavor flavor, JetFlavor special);
TString        multiply(int count, const char label[]);
TString        formatFlavors(const std::vector<int>& counts);
TString        formatFlavors(const std::vector<int>& counts, const std::vector<int>& antiCounts);
TString        nameFor(int pdgId, int charge);
TString        nameFor(int pdgId);

template<typename Object>
TString nameFor(const Object& object);

TString titleFor(int pdgId, int charge);
TString titleFor(int pdgId);

template<typename Object>
TString titleFor(const Object& object);

TString shortTitleFor(int pdgId, bool ignoreLR = false);
TString toMathematica(TString text);

//_____________________________________________________________________________
// Hadron type information
//_____________________________________________________________________________

JetFlavor jetFlavor(HadronType type);
bool isHeavyFlavor(HadronType type);
bool isBHadron(HadronType type);
HadronType typeOfHadron(int pdgId, int* numBQuarks = 0, int* numCQuarks = 0);
HadronType detailedTypeOfHadron(int pdgId);
const TString& hadronTypeName(HadronType type);
const TString& hadronTypeTitle(HadronType type);
TString hadronTypeName(HadronType type, int pdgId);
TString hadronTypeTitle(HadronType type, int pdgId);
};  // end class ParticleInfo


#include "AnalysisTools/Utilities/src/ParticleInfo.icc"

#endif //PARTICLEINFO_H
