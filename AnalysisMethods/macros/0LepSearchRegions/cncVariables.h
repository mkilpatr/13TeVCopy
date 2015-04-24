#ifndef cncVariables_H
#define cncVariables_H

class Panvariate;
template<typename Object> class Paramatrix;
typedef   Paramatrix<Panvariate>  ParamatrixMVA;


#include "AnalysisBase/TreeAnalyzer/interface/BaseTreeAnalyzer.h"
#include "AnalysisTools/KinematicVariables/interface/JetKinematics.h"
#include "math.h"

using namespace std;

namespace ucsbsusy {

struct a {
  static const BaseTreeAnalyzer * analyzer;
  static bool isMediumBJet(const RecoJetF& jet){
    return analyzer->isMediumBJet(jet);
  };
};
const BaseTreeAnalyzer * a::analyzer = 0;


class VariableCalculator{
public:
  VariableCalculator() :
    //passPresel        ( 0)
      ptMet             (-1)
    , npv               (-1)
    , nj60              (-1)
    , nJ20              (-1)
    , ntBtag            (-1)
    , nmBtag            (-1)
    , dPhiMET12         (-1)
    , dPhiMET3          (-1)
    , dPhiB0MET         (-1)
    , dPhiB1MET         (-1)
    , dPhiB2MET         (-1)
    , dPhiB01MET        (-1)
    , dPhiCVSnearMET    (-1)
    , Mb1b2n            (-1)
    , mtB0MET           (-1)
    , mtB1MET           (-1)
    , mtB2MET           (-1)
    , mtB01MET          (-1)
    , sSumB01oMET       (-1)
    , vSumB01oMET       (-1)
    , qgl0              (-1)
    , qgl1              (-1)
    , qglprod           (-1)
    , ht                (-1)
    , htAlongAway20     (-1)
    , htAlongAway40     (-1)
    , maxMj12           (-1)
    , rmsJetPT          (-1)
    , rmsJetDphiMET     (-1)
    , bInvMass          (-1)
    , bTransMass        (-1)
    , rmsBEta           (-1)
    , wInvMass          (-1)
    , Bpt0              (-1)
    , Bpt1              (-1)
    , htnoB             (-1)
    , htJ12             (-1)
    , htJ12noB          (-1)
    , metOsqrtHt        (-1)
    , metOsqrtHtnoB     (-1)
    , metOsqrtHtJ12     (-1)
    , metOsqrtHtJ12noB  (-1)
    , dPhivHtMET        (-1)
    , dPhivHtMETnoB     (-1)
    , dotHtAlongAway    (-1)
    , dotHtAlongAwayNoB (-1)
  {}

  void rankedByCSV(vector<RecoJetF*> inJets,vector<RecoJetF*>& outJets);

  // assumes that jets are inclusive in eta and pt!!!
  // also assumes that the jets are pt sorted!
  void processVariables( const BaseTreeAnalyzer * analyzer
  		             , const JetReader*         ak4JetReader
  		             , const vector<RecoJetF*>& inAK4Jets
  		             , const vector<RecoJetF*>& inJets
  		             , const MomentumF*         inMet
  		             ) {
    a::analyzer = analyzer;

    // preselection so that variable initializations that follow make sense
    if (inJets.size()<5) return;

    //passPresel      =  0;
    ptMet             =  0;
    npv               =  0;
    nj60              =  0;
    nJ20              =  0;
    ntBtag            =  0;
    nmBtag            =  0;
    dPhiMET12         = -1;
    dPhiMET3          = -1;
    dPhiB0MET         = -1;
    dPhiB1MET         = -1;
    dPhiB2MET         = -1;
    dPhiB01MET        = -1;
    dPhiCVSnearMET    = -1;
    Mb1b2n            = -1;
    mtB0MET           = -1;
    mtB1MET           = -1;
    mtB2MET           = -1;
    mtB01MET          = -1;
    sSumB01oMET       = -1;
    vSumB01oMET       = -1;
    qgl0              =  0;
    qgl1              =  0;
    qglprod           =  1;
    ht                =  0;
    htAlongAway20     =  0;
    htAlongAway40     =  0;
    maxMj12           =  0;
    rmsJetPT          =  0;
    rmsJetDphiMET     =  0;
    bInvMass          = -1;
    bTransMass        = -1;
    rmsBEta           =  0;
    wInvMass          =  0;
    Bpt0              = -1;
    Bpt1              = -1;
    htnoB             =  0;
    htJ12             =  0;
    htJ12noB          =  0;
    metOsqrtHt        = -1;
    metOsqrtHtnoB     = -1;
    metOsqrtHtJ12     = -1;
    metOsqrtHtJ12noB  = -1;
    dPhivHtMET        = -1;
    dPhivHtMETnoB     = -1;
    dotHtAlongAway    = -1;
    dotHtAlongAwayNoB = -1;

    ptMet = inMet->pt();
    npv   = analyzer->nPV;

    // AK4 jets are always used for met/jet correlations, trigger requirements, and QGTagging
    for(unsigned int iJ = 0; iJ < inAK4Jets.size(); ++iJ){
      const auto& j = *inAK4Jets[iJ];
      if(j.pt() >= 60 ) nj60++;
      // qgl stuff
      if(analyzer->isLooseBJet(j)) continue;
      double tempqgl = 1; //(1.+ak4JetReader->jetqgl_->at(j.index()))/2.; // transform [-1,1] -> [0,1]
      double qgl = std::max( .01, double(tempqgl) ); // make sure qglprod isn't zeroed out
      qglprod *= qgl;
      if      (qgl > qgl0) { qgl1 = qgl0; qgl0 = qgl; }
      else if (qgl > qgl1) { qgl1 = qgl;              }
    } // iJ in ak4jets
    qglprod   = TMath::Log(qglprod);
    dPhiMET12 = JetKinematics::absDPhiMETJ12(*inMet,inAK4Jets);
    dPhiMET3  = JetKinematics::absDPhiMETJ3 (*inMet,inAK4Jets);

    // can be ak4, picky
    for(unsigned int iJ = 0; iJ < inJets.size(); ++iJ){
      auto& j = *inJets[iJ];
      ++nJ20;
      if(analyzer->isTightBJet(j)) ++ntBtag;
      if(analyzer->isMediumBJet(j)){
        ++nmBtag;
        if      (Bpt0 == 0) Bpt0 = j.pt();
        else if (Bpt1 == 0) Bpt1 = j.pt();
      }
    } // iJ in Jets

    //passPresel = ptMet >= 200 && nj60 >= 2 && nJ20 >= 6 && ntBtag >= 1  && dPhiMET12 >= .5 && dPhiMET3 >= .3;

    htAlongAway20 = JetKinematics::htAlongHtAway(*inMet,inJets,20,2.4);
    htAlongAway40 = JetKinematics::htAlongHtAway(*inMet,inJets,40,2.4);
    maxMj12       = JetKinematics::highestPTJetPair(inJets);
    rmsJetPT      = JetKinematics::ptRMS(inJets);
    rmsJetDphiMET = JetKinematics::deltaPhiMETRMS(*inMet,inJets);

    vector<RecoJetF*> jetsCSV;
    rankedByCSV(inJets,jetsCSV);
    double nearestDphi = -9.;
    int bjetNearMETindx = PhysicsUtilities::findNearestDPhiDeref(*inMet,analyzer->bJets,nearestDphi);
    if(jetsCSV.size()>0)   dPhiB0MET      = PhysicsUtilities::absDeltaPhi(jetsCSV.at(0)->p4(),*inMet);
    if(jetsCSV.size()>1)   dPhiB1MET      = PhysicsUtilities::absDeltaPhi(jetsCSV.at(1)->p4(),*inMet);
    if(jetsCSV.size()>2)   dPhiB2MET      = PhysicsUtilities::absDeltaPhi(jetsCSV.at(2)->p4(),*inMet);
    if(bjetNearMETindx>=0) dPhiCVSnearMET = PhysicsUtilities::absDeltaPhi(analyzer->bJets.at(bjetNearMETindx)->p4(),*inMet);
    dPhiB01MET = (dPhiB0MET<dPhiB1MET) ? dPhiB0MET : dPhiB1MET;

    if(jetsCSV.size()>1) Mb1b2n  = (jetsCSV.at(0)->p4()+jetsCSV.at(1)->p4()).mass();
    if(jetsCSV.size()>0) mtB0MET = JetKinematics::transverseMass(jetsCSV.at(0)->p4(),*inMet);
    if(jetsCSV.size()>1) mtB1MET = JetKinematics::transverseMass(jetsCSV.at(1)->p4(),*inMet);
    if(jetsCSV.size()>2) mtB2MET = JetKinematics::transverseMass(jetsCSV.at(2)->p4(),*inMet);
    mtB01MET = (mtB0MET<mtB1MET) ? mtB0MET : mtB1MET;

    if (ptMet>0) {
      if(jetsCSV.size()>1) sSumB01oMET = ( jetsCSV.at(0)->pt() + jetsCSV.at(1)->pt() )      / ptMet;
      if(jetsCSV.size()>1) vSumB01oMET = ( jetsCSV.at(0)->p4() + jetsCSV.at(1)->p4() ).pt() / ptMet;
    } // ptMet>0

    bInvMass   = JetKinematics::bJetInvMass(inJets,&a::isMediumBJet);
    bTransMass = JetKinematics::bJetTranverseMass(*inMet,inJets,&a::isMediumBJet);
    rmsBEta    = JetKinematics::deltaEtaBJetRMS(inJets,&a::isMediumBJet);
    wInvMass   = JetKinematics::highestPTJetPair(inJets);
    ht         = JetKinematics::ht(inJets);

    htJ12 = inJets[0]->pt() + inJets[1]->pt();
    MomentumF vHt;
    MomentumF vHtnoB;
    double htAlong    = 0;
    double htAway     = 0;
    double htAlongNoB = 0;
    double htAwayNoB  = 0;
    int nNonB = 0;
    for(unsigned int nJ = 0; nJ < inJets.size(); ++nJ){
      auto& j = *inJets[nJ];
      if (j.pt()<20)   continue;
      if (j.eta()>2.4) continue;
      vHt = vHt.p4() + j.p4();

      // dotted HTalongOaway
      double dPhi = PhysicsUtilities::absDeltaPhi(j,*inMet);
      if (dPhi < TMath::PiOver2()) htAlong += abs(j.pt() * cos(dPhi));
      else                         htAway  += abs(j.pt() * cos(dPhi));

      // past this, only want to use non-B jets
      if(analyzer->isMediumBJet(j)) continue;
      htnoB += j.pt();
      vHtnoB = vHt.p4() + j.p4();
      if(nNonB<2) htJ12noB += j.pt();

      // dotted HTalongOaway
      if (dPhi < TMath::PiOver2()) htAlongNoB += abs(j.pt() * cos(dPhi));
      else                         htAwayNoB  += abs(j.pt() * cos(dPhi));

      ++nNonB;
    } // iJ in Jets

    if(ht      >0) metOsqrtHt       = ptMet / sqrt(ht      ) ;
    if(htnoB   >0) metOsqrtHtnoB    = ptMet / sqrt(htnoB   ) ;
    if(htJ12   >0) metOsqrtHtJ12    = ptMet / sqrt(htJ12   ) ;
    if(htJ12noB>0) metOsqrtHtJ12noB = ptMet / sqrt(htJ12noB) ;
    dPhivHtMET    = PhysicsUtilities::absDeltaPhi(vHt   ,*inMet);
    dPhivHtMETnoB = PhysicsUtilities::absDeltaPhi(vHtnoB,*inMet);
    if(htAway   >0) dotHtAlongAway    = htAlong    / htAway   ;
    if(htAwayNoB>0) dotHtAlongAwayNoB = htAlongNoB / htAwayNoB;

  } // processVariables()

  //bool   passPresel;
  float ptMet;
  int   npv;
  int   nj60;
  int   nJ20;
  int   ntBtag;
  int   nmBtag;
  float dPhiMET12;
  float dPhiMET3;
  float dPhiB0MET;     // new
  float dPhiB1MET;     // new
  float dPhiB2MET;     // new
  float dPhiB01MET;    // new
  float dPhiCVSnearMET;// new
  float Mb1b2n;        // new
  float mtB0MET;       // new
  float mtB1MET;       // new
  float mtB2MET;       // new
  float mtB01MET;      // new
  float sSumB01oMET;   // new
  float vSumB01oMET;   // new
  float qgl0;
  float qgl1;
  float qglprod;
  float ht;
  float htAlongAway20;
  float htAlongAway40;  // new
  float maxMj12;        // new
  float rmsJetPT;
  float rmsJetDphiMET;
  float bInvMass;
  float bTransMass;
  float rmsBEta;
  float wInvMass;
  float Bpt0;
  float Bpt1;
  float htnoB;             // new2
  float htJ12;             // new2
  float htJ12noB;          // new2
  float metOsqrtHt;        // new2
  float metOsqrtHtnoB;     // new2
  float metOsqrtHtJ12;     // new2
  float metOsqrtHtJ12noB;  // new2
  float dPhivHtMET;        // new2
  float dPhivHtMETnoB;     // new2
  float dotHtAlongAway;    // new2
  float dotHtAlongAwayNoB; // new2
}; // VariableCalculator


void VariableCalculator::rankedByCSV(vector<RecoJetF*> inJets,vector<RecoJetF*>& outJets) {
  outJets.clear();
  outJets.resize(inJets.size());
  vector<pair<double,int> > rankedJets(inJets.size());

  for(unsigned int iJ =0; iJ < inJets.size(); ++iJ){
    rankedJets[iJ].first = inJets[iJ]->csv();
    rankedJets[iJ].second = iJ;
  } // inJets.size()

  std::sort(rankedJets.begin(),rankedJets.end(),PhysicsUtilities::greaterFirst<double,int>());
  for(unsigned int iJ =0; iJ < inJets.size(); ++iJ){ outJets[iJ] = inJets[rankedJets[iJ].second]; }
} // rankedByCSV()

} // namespace ucsbsusy





#endif