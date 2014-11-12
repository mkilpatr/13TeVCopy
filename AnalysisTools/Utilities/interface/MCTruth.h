#ifndef ANALYSISTOOLS_UTILITIES_PARTICLEINFO_H
#define ANALYSISTOOLS_UTILITIES_PARTICLEINFO_H

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"

namespace ucsbsusy {

  class MCTruth
  {

    public :
      MCTruth() {}

      enum ParticleType {kNone=0, kUp=1, kDown=2, kCharm=3, kStrange=4, kBottom=5, kTop=6, kElectron=11, kElNu=12, kMuon=13, kMuNu=14, kTau=15, kTauNu=16, kGluon=21, kPhoton=22, kZ=23, kW=24, kH=25, kPi0=111, kRho0=113, kKLong=130, kPiPlus=211, kRhoPlus=213, kEta=221, kKShort=310, kK0=311, kKPlus=321, kD0=421, kJPsi=443, kB0=511, kBPlus=521, kBs0=531, kUpsilon=553, kNeutron=2122, kProton=2212, kA10=20113, kA1Plus=20213, kStop1=1000006, kGluino=1000021, kChi10=1000022};

      static bool isAncestor(const reco::Candidate* ancestor, const reco::Candidate* particle)
      {
	//particle is already the ancestor
	if(ancestor == particle) return true;
	//otherwise loop on mothers, if any and return true if the ancestor is found
	for(size_t i=0; i<particle->numberOfMothers(); i++) {
	  if(isAncestor(ancestor,particle->mother(i))) return true;
	}
	//if we did not return yet, then particle and ancestor are not relatives
	return false;
      }

      static bool isA(int id, const reco::GenParticle* particle, bool checkCharge=false)
      {
        if(checkCharge) return (particle->pdgId() == id);
        else            return (fabs(particle->pdgId()) == id);
      }

      static bool isAnAnti(int id, const reco::GenParticle* particle)
      {
        return (particle->pdgId() == -id);
      }

      static bool isAQuark(const reco::GenParticle* particle)
      {
        return (isA(kUp, particle) || isA(kDown, particle) || isA(kCharm, particle) || isA(kStrange, particle) || isA(kBottom, particle) || isA(kTop, particle));
      }

      static bool isAParton(const reco::GenParticle* particle)
      {
        return (isA(kGluon, particle) || isAQuark(particle));
      }

      static bool isALepton(const reco::GenParticle* particle)
      {
        return (isA(kElectron, particle) || isA(kMuon, particle) || isA(kTau, particle));
      }

      static bool isANeutrino(const reco::GenParticle* particle)
      {
        return (isA(kElNu, particle) || isA(kMuNu, particle) || isA(kTauNu, particle));
      }

      static bool isAGaugeBoson(const reco::GenParticle* particle)
      {
        return (isA(kGluon, particle) || isA(kPhoton, particle) || isA(kW, particle) || isA(kZ, particle));
      }

      static bool isAWOrZ(const reco::GenParticle* particle)
      {
        return(isA(kW, particle) || isA(kZ, particle));
      }

      static bool findBosonDaughters(const std::vector<reco::GenParticle> &genParticles, const reco::GenParticle* &boson, const reco::GenParticle* &dau1, const reco::GenParticle* &dau2)
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
          printf("looking for daughter for boson with status %d\n", outbosons[i]->status());
          if(findBosonDaughters(genParticles, outbosons[i], dau1, dau2)) {
            boson = outbosons[i];
            return true;
          }
        }

        // no luck!
        return false;
      }

      static void findTauDaughter(const std::vector<reco::GenParticle> &genParticles, const reco::GenParticle* &tau, const reco::GenParticle* &dau)
      {
        for(const reco::GenParticle &p : genParticles) {
          const reco::Candidate* mom = p.mother(0);
          if(mom==nullptr) continue;             // not filled
          if(!isAncestor(tau, mom)) continue;    // not related
          if(p.status()!=2 && p.status()!=1) continue;
          if(isA(kTau,&p) || isA(kTauNu,&p) || isA(kW, &p) || isA(kPhoton,&p)) continue;    // get the 1st lepton/hadron from the tau decay, status 2 should imply a decayed hadron in this case
          if(!dau) {
            dau = &p;                            // fill daughter
            break;
          }
        }
      }

  };

}

#endif
