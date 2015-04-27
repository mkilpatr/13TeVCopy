#ifndef OBJECTPRODUCERS_TOPTAGGING_CORRAL_H
#define OBJECTPRODUCERS_TOPTAGGING_CORRAL_H

#include <vector>
#include "AnalysisTools/Utilities/interface/TopJetMatching.h"

namespace ucsbsusy{
class GenParticleReader;
class JetReader;
}

template<typename Object>
class Paramatrix;
class Panvariate;
typedef   Paramatrix<Panvariate>  ParamatrixMVA;

namespace CORRAL {

// ---------------------------------------------------------------------
//
//     GENERAL
//
// ---------------------------------------------------------------------

//CORRAL jet parameters
bool isCORRALJet(const float jPT, const float jETA){ return (TMath::Abs(jETA) < 2.4) && (jPT >= 20);}

//Figure out what the decay structure of the event looks like
//Filter out jets
TopJetMatching::TopDecayEvent* associateDecays(const ucsbsusy::GenParticleReader* genParticleReader, ucsbsusy::JetReader * jetReader,
    std::vector<ucsbsusy::RecoJetF*>& recoJets,  std::vector<TopJetMatching::TopDecayEvent::DecayID>& decays);

//Wrapper for above, returns a bool if you should filter the event or not
bool setup(const ucsbsusy::GenParticleReader* genParticleReader, ucsbsusy::JetReader * jetReader,
    std::vector<ucsbsusy::RecoJetF*>& recoJets,  std::vector<TopJetMatching::TopDecayEvent::DecayID>& decays);

// ---------------------------------------------------------------------
//
//     W Jet Likilihood
//
// ---------------------------------------------------------------------

// Return truth level info on the jets
// signal (W jet = 1)
// bkg (Non W or B jet = 2)

void getJetCandidates(const std::vector<ucsbsusy::RecoJetF*>& recoJets, const std::vector<TopJetMatching::TopDecayEvent::DecayID>& decays,
    std::vector<int>& types );

// Container for all jet candidate variables
struct jetCandVars {
  float jetPT   ;
  float axis1   ;
  float axis2   ;
  float ptD     ;
  float jetMult ;
  float betaStar;
  float area    ;
  float mass    ;
  float mva     ;
};

jetCandVars calculateJetCandVars(const int nPV, const ucsbsusy::JetReader * jetReader, const ucsbsusy::RecoJetF * jet);

//Holder of the WJet MVA
struct WJetLikliMVA{
  ParamatrixMVA* param;
  int i_jetPT      ;
  int i_axis1      ;
  int i_axis2      ;
  int i_ptD        ;
  int i_jetMult    ;
  int i_betaStar   ;
  int i_area       ;
  int i_mass       ;

  WJetLikliMVA(TString filename,TString bdtName );
  float mvaVal(jetCandVars& v) const;
};

// ---------------------------------------------------------------------
//
//     W Candidates
//
// ---------------------------------------------------------------------

// Two jet candidate that is tested to see if it is a W
struct WCand {
  WCand(const ucsbsusy::RecoJetF * jet1_,const ucsbsusy::RecoJetF * jet2_, int ind1_, int ind2_, bool isW_, int topIndex_, int fakeCategory_);
  WCand(const WCand& other);

  const ucsbsusy::RecoJetF * jet1;
  const ucsbsusy::RecoJetF * jet2;
  const int ind1;
  const int ind2;
  const ucsbsusy::MomentumF mom;
  const bool isW;
  const int topIndex;
  const int fakeCategory;
};

//Takes two jets and adds the candidate to the wCand vector, with truth info
void addWCandidate(const unsigned int iJ, const unsigned int iJ2,
    const std::vector<ucsbsusy::RecoJetF*>& recoJets, const std::vector<TopJetMatching::TopDecayEvent::DecayID>& decays,
    std::vector<WCand>& wCands);

//Iterates over the list of jets and builds all candidates
void getWCandidates(const std::vector<ucsbsusy::RecoJetF*>& recoJets, const std::vector<TopJetMatching::TopDecayEvent::DecayID>& decays,
    std::vector<WCand>& wCands  );

struct WCandVars {
  float wPT       ;
  float wMass     ;
  float pt2opt1   ;
  float wJetLikli1;
  float wJetLikli2;
  float maxCSV    ;
  float dr        ;
  float deta      ;
  float dphi      ;
  float charge    ;
  float pullAng1  ;
  float pullAng2  ;
  float nWCon     ;
  float mva       ;
};

WCandVars calculateWCandVars(const ucsbsusy::JetReader * jetReader, const std::vector<ucsbsusy::RecoJetF*>& recoJets,
    const std::vector<jetCandVars>& jetCandVars, const WCand& cand);

struct WMVA{
  ParamatrixMVA* param;
  ucsbsusy::size i_wPT       ;
  ucsbsusy::size i_wMass     ;
  ucsbsusy::size i_pt2opt1   ;
  ucsbsusy::size i_wJetLikli1;
  ucsbsusy::size i_wJetLikli2;
  ucsbsusy::size i_maxCSV    ;
  ucsbsusy::size i_dr        ;
  ucsbsusy::size i_deta      ;
  ucsbsusy::size i_dphi      ;
  ucsbsusy::size i_nWCon     ;

  WMVA(TString filename,TString bdtName );

  float mvaVal(WCandVars& vars) const;
  bool passMVA(const double pt, const double mvaV) const;
};

// ---------------------------------------------------------------------
//
//     T Candidates
//
// ---------------------------------------------------------------------
// Single top candidate...composed of a W candiate and one other jet
// signal good combo (W->W, b->b) (type = 1)
// bad combo (correct three jets) (type = 0)
// bkg (eveything else (type = 2)
struct TCand {
  TCand(const WCand * wCand_,const ucsbsusy::RecoJetF * bJet_, int wInd_, int bInd_, int type_, int topIndex_, int fakeCategory_);
  const WCand * wCand;
  const ucsbsusy::RecoJetF * bJet;
  const int wInd;
  const int bInd;
  const ucsbsusy::MomentumF mom;
  const int type;
  const int topIndex;
  const int fakeCategory;

  bool equivJets(const TCand& o) const {
    if(o.bInd        != bInd && o.bInd        != wCand->ind1 && o.bInd        != wCand->ind2) return false;
    if(o.wCand->ind1 != bInd && o.wCand->ind1 != wCand->ind1 && o.wCand->ind1 != wCand->ind2) return false;
    if(o.wCand->ind2 != bInd && o.wCand->ind2 != wCand->ind1 && o.wCand->ind2 != wCand->ind2) return false;
    return true;
  }
  bool exclJets(const TCand& o) const {
    if(o.bInd        == bInd || o.bInd        == wCand->ind1 || o.bInd        == wCand->ind2) return false;
    if(o.wCand->ind1 == bInd || o.wCand->ind1 == wCand->ind1 || o.wCand->ind1 == wCand->ind2) return false;
    if(o.wCand->ind2 == bInd || o.wCand->ind2 == wCand->ind1 || o.wCand->ind2 == wCand->ind2) return false;
    return true;
  }
};

//Put together a TCandidate with truth info and place it in the TCand vector
void addTCandidate(const unsigned int iW, const WCand& wCand, const unsigned int iJ,
    const std::vector<ucsbsusy::RecoJetF*>& recoJets, const std::vector<TopJetMatching::TopDecayEvent::DecayID>& decays,
    std::vector<TCand>& tCands);

void getTCandidates(const WMVA& wMVA, const std::vector<ucsbsusy::RecoJetF*>& recoJets, const std::vector<TopJetMatching::TopDecayEvent::DecayID>& decays,
    const std::vector<WCand>& wCands,const std::vector<WCandVars>& wCandVars, std::vector<TCand>& tCands   );

struct TCandVars {
  float tPT          ;
  float wPT          ;
  float tMass        ;
  float wMass        ;
  float bPTotPT      ;
  float bCSV         ;
  float maxWCSV      ;
  float bWLikli      ;
  float wDisc        ;
  float maxOWDisc    ;
  float m23om123     ;
  float m13om12      ;
  float atan_m13om12 ;
  float maxjjdr      ;
  float wbDR         ;
  float wbDEta       ;
  float wbDPhi       ;
  float nTCon        ;
  float mva          ;
};

TCandVars calculateTCandVars(const ucsbsusy::JetReader * jetReader, const std::vector<ucsbsusy::RecoJetF*>& recoJets,
    const std::vector<WCand>& wCands, const std::vector<jetCandVars>& jetCandVars, const std::vector<WCandVars>& wCandVars,
    const TCand& cand);

struct T_MVA{
  ParamatrixMVA* param;
  ucsbsusy::size i_tPT           ;
  ucsbsusy::size i_wPT           ;
  ucsbsusy::size i_tMass         ;
  ucsbsusy::size i_wMass         ;
  ucsbsusy::size i_bPTotPT       ;
  ucsbsusy::size i_bCSV          ;
  ucsbsusy::size i_maxWCSV       ;
  ucsbsusy::size i_bWLikli       ;
  ucsbsusy::size i_wDisc         ;
  ucsbsusy::size i_maxOWDisc     ;
  ucsbsusy::size i_m23om123      ;
  ucsbsusy::size i_m13om12       ;
  ucsbsusy::size i_atan_m13om12  ;
  ucsbsusy::size i_maxjjdr       ;
  ucsbsusy::size i_wbDR          ;
  ucsbsusy::size i_wbDEta        ;
  ucsbsusy::size i_wbDPhi        ;
  ucsbsusy::size i_nTCon         ;

  T_MVA(TString filename,TString bdtName );

  float mvaVal(TCandVars& vars) const;
  bool passMVA(const double pt, const double mvaV) const;
};

// ---------------------------------------------------------------------
//
//     T Selection
//
// ---------------------------------------------------------------------
//Remove redundant tops:
//Those composed of the same three jets...keep the guy with the maximum discriminator
//Also..remove guys that fall below your good top cut (or dont if you dont fil the T_MVA var)
void pruneTopCandidates(const std::vector<TCand>& tCands,const std::vector<TCandVars>& tCandVars,
    std::vector<ucsbsusy::RankedIndex>& prunedTops, T_MVA * tMVA = 0);

//ranking metric of pairs
float pairMetric(const float mva1, const float mva2){ return mva1 + mva2; }

//Find top pairs
//Make combinations of all non-overlapping top pairs
//rank them by some metric
std::vector<std::pair<int,int>> getRankedTopPairs(const std::vector<TCand>& tCands,const std::vector<TCandVars>& tCandVars,
    const std::vector<ucsbsusy::RankedIndex>& rankedTops);

// ---------------------------------------------------------------------
//
//     DEFAULT RUNNING -> Produce TOPS and Ws
//
// ---------------------------------------------------------------------

//Collection of all parts of the algorithm
struct CORRALData {
  //storage of all pieces of the algorithm
  std::vector<ucsbsusy::RecoJetF*>  recoJets ;
  std::vector<TopJetMatching::TopDecayEvent::DecayID> decays;
  std::vector<jetCandVars>     jetVars  ;
  std::vector<WCand>     wCands   ;
  std::vector<WCandVars> wCandVars;
  std::vector<TCand>     tCands   ;
  std::vector<TCandVars> tCandVars;
  std::vector<std::pair<int,int>> rankedTPairs;

  //user level information
  bool reconstructedTop;
  TCand * top1;
  TCand * top2;
  float top1_disc;
  float top2_disc;

  //clear out old info
  void reset();
};

//Class that will handle all reconstruction, from jets to top pairs
class CORRALReconstructor {
public:
  WJetLikliMVA  wJetLikliMVA;
  WMVA  wMVA;
  T_MVA  tMVA;
  CORRALData data;
  CORRALReconstructor (TString MVAPrefix = "$CMSSW_BASE/src/data/CORRAL/") :
     wJetLikliMVA( MVAPrefix + "T2tt_merged_wJetLikli_disc.root","mva_0")
    , wMVA(MVAPrefix + "T2tt_merged_wCand_disc.root","mva_0")
    , tMVA(MVAPrefix +"T2tt_merged_tCand_disc.root","mva_0")
  {}

  bool getTopPairs(const ucsbsusy::GenParticleReader * genParticleReader, ucsbsusy::JetReader * jetReader, const int nPV,
      std::vector<ucsbsusy::RankedIndex> * prunedTops = 0 );

};

}

#endif
