/**
  @file         ParticleInfo.cc
*/

#include <cassert>
#include <TPRegexp.h>
#include <TArrayI.h>

#include "AnalysisTools/Utilities/interface/ParticleUtilities.h"

//_____________________________________________________________________________
bool ParticleUtilities::findBosonDaughters(const std::vector<reco::GenParticle> &genParticles, const reco::GenParticle* &boson, const reco::GenParticle* &dau1, const reco::GenParticle* &dau2)
{
  for(const reco::GenParticle &p : genParticles) {
    const reco::Candidate* mom = p.mother(0);
    if(mom==nullptr) continue;               // not filled
    if(!isAncestor(boson, mom)) continue;    // not related
    if(p.pdgId()==boson->pdgId()) continue;  // skip the boson
    if(p.status()!=23 && p.status()!=2 && p.status()!=1) continue;
    if(!dau1) dau1 = &p;                     // fill daughter 1
    else if(!dau2) dau2 = &p;                // fill daughter 2
    else break;
  }

  if(dau1 && dau2) return true;

  // if didn't find daughters for input (status 22) boson, look for daughters of boson versions with higher status codes
  std::vector<const reco::GenParticle*> outbosons;
  for(const reco::GenParticle &p : genParticles) {
    if(p.pdgId()==boson->pdgId() && p.status()!=boson->status() && p.mass()==boson->mass()) outbosons.push_back(&p);
  }

  for(size_t i = 0; i < outbosons.size(); ++i) {
    dau1 = 0;
    dau2 = 0;
    if(findBosonDaughters(genParticles, outbosons[i], dau1, dau2)) {
      boson = outbosons[i];
      return true;
    }
  }

  // no luck!
  return false;
}
//_____________________________________________________________________________
void ParticleUtilities::findTauDaughter(const std::vector<reco::GenParticle> &genParticles, const reco::GenParticle* &tau, const reco::GenParticle* &dau)
{
  for(const reco::GenParticle &p : genParticles) {
    const reco::Candidate* mom = p.mother(0);
    if(mom==nullptr) continue;             // not filled
    if(!isAncestor(tau, mom)) continue;    // not related
    if(p.status()!=2 && p.status()!=1) continue;
    if(isA(ParticleInfo::p_tauminus,&p) || isA(ParticleInfo::p_nu_tau,&p) || isA(ParticleInfo::p_Wplus, &p) || isA(ParticleInfo::p_gamma,&p)) continue;    // get the 1st lepton/hadron from the tau decay, status 2 should imply a decayed hadron in this case
    if(!dau) {
      dau = &p;                            // fill daughter
      break;
    }
  }
}
//_____________________________________________________________________________
std::ostream& ParticleUtilities::printGenHistory(const std::vector<reco::GenParticle>& genParticles, const unsigned int particleIndex)
{
  const reco::GenParticle&      particle        = genParticles[particleIndex];
  if      (particle.numberOfMothers() == 1)
    printGenHistory(genParticles, particle.motherRef().key());
  else if (particle.numberOfMothers()  > 1)
    std::cout  << "...";
  std::cout    << " -> " << particleIndex << ":" << ParticleInfo::titleFor(particle.pdgId(),particle.charge());
  return std::cout;
}

//_____________________________________________________________________________
void ParticleUtilities::printGenInfo(const std::vector<reco::GenParticle>& genParticles, int genBound)
{
  using namespace std;

  cout    << right << setw(6) << "#" << " " << setw(10) << "pdgId" << "  " << left << setw(20) << "Name"
          << "  " << "Chg" << "  " << setw(10) << "Mass" << "  " << setw(48) << " Momentum"
          << left << "  " << setw(10) << "Mothers" << " " << setw(30) << "Daughters" << endl;
  if (genBound < 0)             genBound        =            static_cast<int>(genParticles.size());
  else                          genBound        = TMath::Min(static_cast<int>(genParticles.size()), genBound);
  for (int iGen = 0; iGen < genBound; ++iGen) {
    const reco::GenParticle&    genParticle     = genParticles[iGen];

    cout  << right << setw(3) << genParticle.status();
    cout  << right << setw(3) << iGen << " " << setw(10) << genParticle.pdgId() << "  ";
    cout  << left << setw(20) << ParticleInfo::titleFor(genParticle.pdgId(),genParticle.charge());
    cout  << right << "  " << setw(3) << genParticle.charge() << "  " << TString::Format("%10.3g", genParticle.mass() < 1e-5 ? 0 : genParticle.mass());
    cout  << left << setw(50) << TString::Format("  (E=%6.4g pT=%6.4g eta=%7.3g phi=%7.3g)", genParticle.energy(), genParticle.pt(), genParticle.eta(), genParticle.phi());

    TString                     mothers;
    for (unsigned int iMom = 0; iMom < genParticle.numberOfMothers(); ++iMom) {
      if (mothers.Length())     mothers        += ",";
      mothers   += genParticle.motherRef(iMom).key();
    }
    cout << "  " << setw(10) << mothers;
    TString                     daughters;
    for (unsigned int iDau = 0; iDau < genParticle.numberOfDaughters(); ++iDau) {
      if (daughters.Length())   daughters      += ",";
      daughters += genParticle.daughterRef(iDau).key();
    }
    cout << " " << setw(30) << daughters << endl;
  }
  cout << setfill('-') << setw(150) << "" << setfill(' ') << endl;
}
