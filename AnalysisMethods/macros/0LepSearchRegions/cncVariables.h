#ifndef cncVariables_H
#define cncVariables_H

class Panvariate;
template<typename Object> class Paramatrix;
typedef   Paramatrix<Panvariate>  ParamatrixMVA;


#include "AnalysisBase/TreeAnalyzer/interface/BaseTreeAnalyzer.h"
#include "AnalysisTools/KinematicVariables/interface/JetKinematics.h"

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
      //passPresel    (0)
        ptMet         (0)
      , nJ90          (0)
      , nJ20          (0)
      , ntBtag        (0)
      , nmBtag        (0)
      , dPhiMET12     (10.7)
      , dPhiMET3      (10.7)
      , qgl0          (0)
      , qgl1          (0)
      , qglprod       (1)
      , ht            (0)
      , htAlongAway   (8000)
      , rmsJetPT      (0)
      , rmsJetDphiMET (0)
      , bInvMass      (0)
      , bTransMass    (0)
      , rmsBEta       (0)
      , wInvMass      (0)
      , Bpt0          (0)
      , Bpt1          (0)
    {}



    // assumes that jets are inclusive in eta and pt!!!
    // also assumes that the jets are pt sorted!
    void processVariables( const BaseTreeAnalyzer * analyzer
    		             , const JetReader*         ak4JetReader
    		             , const vector<RecoJetF*>& inAK4Jets
    		             , const vector<RecoJetF*>& inJets
    		             , const MomentumF*         inMet
    		             ) {
      a::analyzer = analyzer;

      //passPresel    = 0;
      ptMet         = 0;
      nJ90          = 0;
      nJ20          = 0;
      ntBtag        = 0;
      nmBtag        = 0;
      dPhiMET12     = 10.7;
      dPhiMET3      = 10.7;
      qgl0          = 0;
      qgl1          = 0;
      qglprod       = 1;
      ht            = 0;
      htAlongAway   = 8000;
      rmsJetPT      = 0;
      rmsJetDphiMET = 0;
      bInvMass      = 0;
      bTransMass    = 0;
      rmsBEta       = 0;
      wInvMass      = 0;
      Bpt0          = 0;
      Bpt1          = 0;

      ptMet = inMet->pt();

      // AK4 jets are always used for met/jet correlations, trigger requirements, and QGTagging
      for(unsigned int iJ = 0; iJ < inAK4Jets.size(); ++iJ){
        const auto& j = *inAK4Jets[iJ];
        if(j.pt() >= 90 ) nJ90++;
        // qgl stuff
        if(analyzer->isLooseBJet(j)) continue;
        double tempqgl = (1.+ak4JetReader->jetqgl_->at(j.index()))/2.; // transform [-1,1] -> [0,1]
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

      //passPresel = ptMet >= 200 && nJ90 >= 2 && nJ20 >= 6 && ntBtag >= 1  && dPhiMET12 >= .5 && dPhiMET3 >= .3;

      htAlongAway   = JetKinematics::htAlongHtAway(*inMet,inJets);
      rmsJetPT      = JetKinematics::ptRMS(inJets);
      rmsJetDphiMET = JetKinematics::deltaPhiMETRMS(*inMet,inJets);

      bInvMass   = JetKinematics::bJetInvMass(inJets,&a::isMediumBJet);
      bTransMass = JetKinematics::bJetTranverseMass(*inMet,inJets,&a::isMediumBJet);
      rmsBEta    = JetKinematics::deltaEtaBJetRMS(inJets,&a::isMediumBJet);
      wInvMass   = JetKinematics::highestPTJetPair(inJets);
      ht         = JetKinematics::ht(inJets);

    } // processVariables()

    //bool   passPresel;
    double ptMet;
    int    nJ90;
    int    nJ20;
    int    ntBtag;
    int    nmBtag;
    double dPhiMET12;
    double dPhiMET3;
    double qgl0;
    double qgl1;
    double qglprod;
    double ht;
    double htAlongAway;
    double rmsJetPT;
    double rmsJetDphiMET;
    double bInvMass;
    double bTransMass;
    double rmsBEta;
    double wInvMass;
    double Bpt0;
    double Bpt1;
  };
}





#endif
