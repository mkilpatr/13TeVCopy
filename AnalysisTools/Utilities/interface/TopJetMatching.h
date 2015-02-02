#ifndef TOPJETMATCHING_H_
#define TOPJETMATCHING_H_

#include "AnalysisTools/Utilities/interface/Types.h"
#include "AnalysisTools/DataFormats/interface/Jet.h"
#include "AnalysisTools/DataFormats/interface/GenParticle.h"

namespace TopJetMatching {

//types used for containment
//kept as typedef for easier changing if the need arises
typedef ucsbsusy::int8 conType;  //containment (in percent...so times 100)...if negative means we have a new parton

//because we want to store the contianments as chars, let's have these conversions handy
conType toContainmentType(const float inCon );
float fromContainmentType(const conType inCon);


//typedefs for portability
typedef ucsbsusy::GenParticleF Particle;
typedef ucsbsusy::GenJetF Jet;

enum  PartonDiagnosis {
                        SOFT_PARTON        //low pT...no use matching
                      , HIGH_ETA_PARTON    //high eta parton...no use matching
                      , DISPERSED_PARTON   //Hadronized energy is much less than parton energy
                      , NO_JET             //Not associated to any jet
                      , SPLIT_JETS         //Parton deposits sign. energy in more than one jet
                      , DIRTY_JET          //Jet has significant contribution from non-hard sources
                      , RESOLVED_PARTON    //Fully resolved parton
                      , numPartonDiagnoses
                      };
//diagnosis constants
const float minPartonPT    = 20;
const float maxPartonETA   = 2.4;
const float minHadronRelE  = .50;
const float minPartontRelE = .50;
const float extraJetsPartonRelE = .15;
const float minJetRelE     = .50;


class Parton {
public:
  const Particle * parton;
  const unsigned int genIdx;
  const float      hadE;

  PartonDiagnosis diag;
  std::vector<std::pair<float,int> > containment; //[containment] [jet]
  std::vector<std::pair<float,int> > filteredContaiment; //[containment] [jet] ... from whatever we get in the genjet colleciton

  const Jet * matchedJet;

  Parton(const Particle * p, const unsigned int idx, const float inHadE ) :
    parton(p),genIdx(idx),hadE(inHadE), diag(numPartonDiagnoses), matchedJet(0) {}

  void addContainment(const unsigned int jetIDx, const float con) {containment.emplace_back(con,jetIDx);}
  void finalize(const std::vector<Jet*>&   jets);

  static PartonDiagnosis getDiagnosis(const Parton& parton,const std::vector<Jet*>& jets);
};


enum TopDiagnosis {
  BAD_PARTON   ,      //At least one parton is soft or dispersed
  LOST_JET     , //At least one parton has no matched jet
  SPLIT_PARTONS,  //At least one parton is split into two jets
  CONTAMINATION,  //At least one parton is highly contaminated from outside
  RESOLVED_TOP ,  //ALL three partons are resolved well
  numTopDiagnoses,
  };



class TopDecay {
public:
  std::vector<Parton> leptonPartons;
  std::vector<const Parton *> hadronicPartons;

  const Particle * top;
  const Particle * W;

  const Parton * b;
  const Parton * W_dau1;
  const Parton * W_dau2;

  bool isLeptonic;
  TopDiagnosis diag;

  TopDecay(const Particle * inTop, const std::vector<Parton>& allPartons);
  static TopDiagnosis getDiagnosis(const TopDecay& parton);

};

template<typename TopDecay>
struct GreaterTopDecayPT : public std::binary_function<const TopDecay&, const TopDecay&, bool> {
  bool operator()(const TopDecay& h1, const TopDecay& h2) const
  { return h1.top->pt() > h2.top->pt(); }
};

class TopDecayEvent {
public:
  const std::vector<Jet*> jets;
  std::vector<Parton>     partons;
  std::vector<TopDecay>   topDecays;

  TopDecayEvent(
      const std::vector<ucsbsusy::size16 >* genAssocPrtIndex, const std::vector<ucsbsusy::size16 >* genAssocJetIndex, const std::vector<conType>* genAssocCon,
      const std::vector<Particle>* genParticles,const std::vector<float   >* hadronE, const std::vector<Jet*>& inJets);
};

};

//#include "AnalysisTools/Utilities/src/TopJetMatching.icc"


#endif
