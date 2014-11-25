/**
  @file         Splittiness.cc
  @author       Sue Ann Koay (sakoay@cern.ch)
*/


#ifndef __SPLITTINESS_H__
#define __SPLITTINESS_H__

#include <TString.h>

#include <fastjet/PseudoJet.hh>

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Provenance/interface/ProductID.h"

#include "../../PickyJetUtilities/interface/FastFunctions.h"
#include "../../PickyJetUtilities/interface/LumpTools.h"
#include "../../PickyJetUtilities/interface/NjettinessPlugin.hh"
#include "../../PickyJetUtilities/interface/Nsubjettiness.hh"
#include "AnalysisTools/Parang/interface/Panvariate.h"
#include "AnalysisTools/Utilities/interface/Types.h"

namespace ucsbsusy{
//=============================================================================
class Splittiness
{
public:
  class JetDeposition {
  public:
    BimodalShape1D        lobes         ;
    double                centerLocation;
    double                centerValue   ;
    double                pileUpEnergy  ;
    std::vector<double>   subJetPUEnergy;

    JetDeposition ( const std::vector<fastjet::PseudoJet*>& subJets);
  };
  
  enum  PileUpRhoType { RHO_PT, RHO_MASS, numPileUpRhoTypes };


protected:
  double                                    currentRhoPT  ;
  double                                    currentRhoMass;

  mutable const fastjet::PseudoJet*         currentJet;
  mutable std::vector<fastjet::PseudoJet>   currentConstituents;
  mutable JetDeposition*                    jetStuff;

public:
  static const TString                      NSUBJETTINESS_AXES[];
  static const TString                      STANDARD_JET_ALGOS[];

  const ParamatrixMVA*                      declusterVsDirty;
  const ParamatrixMVA*                      declusterVsMixed;
  const ParamatrixMVA*                      nsubjettiVsDirty;
  const ParamatrixMVA*                      nsubjettiVsMixed;
  const ParamatrixMVA*                      goodVsJunkSubjet;

//  SampleEnergyTest*                         partonSpread  ;
  PopulationD*                              meanRhoVsEta  ;
#if 0
  ParamatrixF*                              l1FastPFchs;
  ParamatrixF*                              l1FastPFall;
#endif
  const NsubParameters                      nSubParams    ;
  mutable Njettiness                        nSubjettiness ;

  Splittiness ( const edm::ParameterSet&    iConfig
              , bool      loadPartonSpread  = true
              , double    nSubBeta          = 1
              , double    nSubR0            = 1
              , double    nSubRcutoff       = 1e4
              );
  virtual ~Splittiness();

  
/**********************/
/*     Event info     */
/**********************/
Njettiness::AxesMode  getMode(std::string setting);
void  setProductIDs(const edm::ProductID& recoParticlesID, const edm::ProductID& genParticlesID);

template<typename Particles>
void  setPileUpInfo( const Particles& particles, double maxParticleEta, double maxPatchEta, double rParameter, double ghostArea, size ignoreNHardest ) {
  Splittiness::computeMedianPU( particles, currentRhoPT, currentRhoMass, maxParticleEta, maxPatchEta, rParameter, ghostArea, ignoreNHardest );
}

const JetDeposition* getJetStuff() {return jetStuff;}
  
/**********************/
/*    Computations    */
/**********************/

static double sqrtPotential   (double r2) { return -fastpow(r2, 0.25);                  }
static double sqrtGradient    (double r2) { return  fastpow(r2, -1.5);                  }

static double logPotential    (double r2) { return -fastlog(r2)/2;                      }
static double logGradient     (double r2) { return 1/r2;                                }

static double oneOverPotential(double r2) { return fastpow(r2, -0.5);                   }
static double oneOverGradient (double r2) { return fastpow(r2, -1.5);                   }

static double linearPotential (double r2) { return -fastpow(r2, 0.5);                   }
static double linearGradient  (double r2) { return -1;                                  }

static double normalPotential (double r2) { return fastexp(-r2/2);                      }
static double normalGradient  (double r2) { return -fastpow(r2, 0.5) * fastexp(-r2/2);  }


void    cacheJetInfo(const fastjet::PseudoJet& superJet, const std::vector<fastjet::PseudoJet*>* subJets, bool forceReset = false) const;


/**********************/
/*      Services      */
/**********************/

#if 0
template<typename Particle>
CartLorentzVector computePileUp(const std::vector<Particle>& ghosts, double rho) const;
CartLorentzVector computePileUp(const fastjet::PseudoJet& location , double rho) const;

template<typename Particle>
CartLorentzVector expectedPFchsPileUp(const std::vector<Particle>& junks, double rho, double* sumPFchsPT = 0) const;
#endif

struct PUPatchInfo { double eta; double ptDensity; double dMassDensity; };

template<typename Particles>
static void computeMedianPU ( const Particles& particles, double& rhoPT, double& rhoMass, double maxParticleEta, double maxPatchEta
                            , double rParameter, double ghostArea, size ignoreNHardest = 0, std::vector<PUPatchInfo>* patchInfos = 0
                            , const std::vector<bool>* selected = 0
                            );

CartLorentzVector computePUSubtractedP4(const fastjet::PseudoJet& jet, double rhoPT, double rhoMass, double nominalArea = -9, const PopulationD* etaScaling = 0) const;



/**********************/
/*    Declustering    */
/**********************/

double  getDeclusterSubjets     (const fastjet::PseudoJet& superJet,       std::vector<fastjet::PseudoJet*>& subJets        ) const;


/**********************/
/*   N-subjettiness   */
/**********************/

double  getNSubjettinessSubjets (const fastjet::PseudoJet& superJet,       std::vector<fastjet::PseudoJet*>& subJets                                       ) const;
bool    shouldCleanNSubjettiness(const fastjet::PseudoJet& superJet,       std::vector<fastjet::PseudoJet*>& subJets, double tau2, double * discResult = 0) const;
bool    shouldSplitNSubjettiness(const fastjet::PseudoJet& superJet, const std::vector<fastjet::PseudoJet*>& subJets, double tau2, double * discResult = 0) const;


/**********************/
/*    Jet Trimming    */
/**********************/

void    eraseTrailingJunkSubjets(const fastjet::PseudoJet& superJet, size superIndex, std::vector<fastjet::PseudoJet>& subJets) const;
void    erasePULikeSubjets      (const fastjet::PseudoJet& superJet, size superIndex, std::vector<fastjet::PseudoJet>& subJets) const;


};  // end class Splittiness

}
#include "ObjectProducers/JetProducers/src/Splittiness.icc"


#endif //__SPLITTINESS_H__
