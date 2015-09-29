#ifndef PARTONMATCHING_H_
#define PARTONMATCHING_H_

#include "AnalysisTools/Utilities/interface/Types.h"
#include "AnalysisTools/DataFormats/interface/Jet.h"
#include "AnalysisTools/DataFormats/interface/GenParticle.h"

namespace PartonMatching {

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
                      , MERGED_JET        //Parton's jet is also an important parton's jet (if this parton is not 'important' the parton will not be labeled as merged)
                      , DIRTY_JET          //Jet has significant contribution from other sources
                      , RESOLVED_PARTON    //Fully resolved
                      , numPartonDiagnoses
                      };

//diagnosis constants
extern float minPartonPT        ;
extern float maxPartonETA       ;
extern float minHadronRelE      ;
extern float minPartontRelE     ;
extern float extraJetsPartonRelE;
extern float minJetRelE         ;

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

  friend std::ostream& operator<<(std::ostream& os, const Parton& m);
};


void setPartonPurity(bool pure = true);
PartonDiagnosis getPartonDiagnosis(const Parton& parton, const std::vector<Jet*>& jets,const std::vector<const Parton *>* impPartons = 0);


enum DecayDiagnosis {
  BAD_PARTON   ,      //At least one parton is soft or dispersed
  LOST_JET     , //At least one parton has no matched jet
  SPLIT_PARTONS,  //At least one parton is split into two jets
  MERGED_PARTONS, // depending on your accepted contamination, you can have siginficant deposits from other top quarks
  CONTAMINATION,  //At least one parton is highly contaminated from outside
  RESOLVED ,  //ALL three partons are resolved well
  numDecayDiagnoses,
  };
DecayDiagnosis getDecayDiagnosis(const std::vector<const Parton *>& decayPartons);

class BosonDecay {
public:
  std::vector<Parton> nonHadronicPartons; //holder for leptons and photons
  std::vector<const Parton *> hadronicPartons;

  const Particle * boson;

  const Parton * boson_dau1;
  const Parton * boson_dau2;

  bool isHadronic;
  DecayDiagnosis diag;

  BosonDecay(const Particle * inBoson, int indx,  const std::vector<Parton>& allPartons);

  friend std::ostream& operator<<(std::ostream& os, const BosonDecay& m);

};


class TopDecay {
public:
  std::vector<const Parton *> hadronicPartons;

  const Particle * top;
  const Particle * W;

  const BosonDecay * W_decay;

  const Parton * b;
  const Parton * W_dau1;
  const Parton * W_dau2;

  bool isLeptonic;
  DecayDiagnosis diag;

  TopDecay(const Particle * inTop, const std::vector<BosonDecay>& bosonDecays, const std::vector<Parton>& allPartons);

  friend std::ostream& operator<<(std::ostream& os, const TopDecay& m);
};


class DecayID {
public:
  enum Type {NONE,RADIATED,TOP_B,TOP_W};
  DecayID() : type(NONE), topInd(-1) {};

  const Parton* mainParton() const {return conPartons.size() == 0 ? 0 : conPartons.front().second;}

  Type type;
  int topInd;
  std::vector<std::pair<float,const Parton*> > conPartons; //[Contained jet E][parton ptr]

};


class PartonEvent {
public:
  const std::vector<Jet*> jets;
  std::vector<Parton>     partons;
  std::vector<const Parton*>    importantPartons;

  std::vector<TopDecay> topDecays;
  std::vector<BosonDecay> bosonDecays;

  std::vector<float> subtractedJetPTs;


  PartonEvent(
      const std::vector<ucsbsusy::size16 >* genAssocPrtIndex, const std::vector<ucsbsusy::size16 >* genAssocJetIndex, const std::vector<conType>* genAssocCon,
      const std::vector<Particle>* genParticles,const std::vector<float   >* hadronE, const std::vector<Jet*>& inJets) : jets(inJets) {
    getPartonsAndContaiment(genAssocPrtIndex,genAssocJetIndex,genAssocCon,genParticles,hadronE);
    finalizePartons(&importantPartons);
  }

  template<typename GenPrtRead,typename JetRead>
  PartonEvent(const GenPrtRead& genParticleReader, JetRead& jetReader, const std::vector<Jet*>& inJets): jets(inJets){
    //Get pointers to necessary variables
    const std::vector<ucsbsusy::size16 >* genAssocPrtIndex = jetReader.genAssocPrtIndex_;
    const std::vector<ucsbsusy::size16 >* genAssocJetIndex = jetReader.genAssocJetIndex_;
    const std::vector<ucsbsusy::int8>*    genAssocCon      = jetReader.genAssocCont_;
    const std::vector<Particle>*          genParticles     = &genParticleReader.genParticles;
    const std::vector<float   >*          hadronE          = genParticleReader.hade_;

    getPartonsAndContaiment(genAssocPrtIndex,genAssocJetIndex,genAssocCon,genParticles,hadronE);
    finalizePartons(&importantPartons);
  }

  void processSubtractedJetPTs(float maxNonHadDR = 0.4);
  void getTopJetDecayMatches(const std::vector<ucsbsusy::RecoJetF*> recoJets, std::vector<DecayID>& decayIDs) const;
private:
  void getPartonsAndContaiment(const std::vector<ucsbsusy::size16 >* genAssocPrtIndex, const std::vector<ucsbsusy::size16 >* genAssocJetIndex, const std::vector<conType>* genAssocCon,
      const std::vector<Particle>* genParticles,const std::vector<float   >* hadronE);

  void finalizePartons(const std::vector<const Parton *>* impPartons = 0);


};


}

#endif
