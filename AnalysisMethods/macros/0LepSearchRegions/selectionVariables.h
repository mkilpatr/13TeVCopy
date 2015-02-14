#ifndef selectionVariables_H
#define selectionVariables_H

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
        met_pt          (0)
      , nJ90            (0)
      , nJ20            (0)
      , nTightBTags     (0)
      , dPhiMET12       (10.7)
      , dPhiMET3        (10.7)
      , passPreselction (0)
      , nMedBTags       (0)
      , secLeadQL       (0)
      , leadLeadQL      (0)
      , prodQL          (1)
      , htAlongAway     (8000)
      , rmsJetPT        (0)
      , rmsJetDphiMET   (0)
      , bInvMass        (0)
      , bTransverseMass (0)
      , rmsBEta         (0)
      , wInvMass        (0)
      , leadBPT         (0)
      , secLeadBPT      (0)
      , ht              (0)
    {}



    //assumes that jets are inclusive in eta and pt!!!
    //also assumes that the jets are pt sorted!
    void processVariables(const BaseTreeAnalyzer * analyzer, const JetReader*ak4JetReader, const vector<RecoJetF*>&inAK4Jets, const vector<RecoJetF*>& inJets, const MomentumF* inMet){
      a::analyzer = analyzer;

      met_pt          = 0;
      nJ90            = 0;
      nJ20            = 0;
      nTightBTags     = 0;
      dPhiMET12       = 10.7;
      dPhiMET3        = 10.7;
      passPreselction = 0;
      nMedBTags       = 0;
      secLeadQL       = 0;
      leadLeadQL      = 0;
      prodQL          = 1;
      htAlongAway     = 8000;
      rmsJetPT        = 0;
      rmsJetDphiMET   = 0;
      bInvMass        = 0;
      bTransverseMass = 0;
      rmsBEta         = 0;
      wInvMass        = 0;
      leadBPT         = 0;
      secLeadBPT      = 0;
      ht              = 0;

      met_pt = inMet->pt();

      //AK4 jets are always used for met/jet correlations, trigger requirements, and QGTagging
      for(unsigned int iJ = 0; iJ < inAK4Jets.size(); ++iJ){
        const auto& j = *inAK4Jets[iJ];
        if(j.pt() >= 90 ) nJ90++;
        //qgl
        if(analyzer->isLooseBJet(j)) continue;
        double qgl = std::max( .01, double((1 + ak4JetReader->jetqgl_->at(j.index()))/2)); //trasnform from
        prodQL *= qgl;
        if(qgl > leadLeadQL){
          secLeadQL = leadLeadQL;
          leadLeadQL = qgl;
        } else if(qgl > secLeadQL)
          secLeadQL = qgl;
      }
      prodQL = TMath::Log(prodQL);
      dPhiMET12 = JetKinematics::absDPhiMETJ12(*inMet,inAK4Jets);
      dPhiMET3  = JetKinematics::absDPhiMETJ3(*inMet,inAK4Jets);

      for(unsigned int iJ = 0; iJ < inJets.size(); ++iJ){
        auto& j = *inJets[iJ];
        nJ20++;
        if(analyzer->isTightBJet(j)) nTightBTags++;
        if(analyzer->isMediumBJet(j)){
          if(leadBPT == 0) leadBPT = j.pt();
          else if(secLeadBPT == 0) secLeadBPT = j.pt();
          nMedBTags++;
        }
      }


      passPreselction = met_pt >= 200 && nJ90 >= 2 && nJ20 >= 6 && nTightBTags >= 1  && dPhiMET12 >= .5 && dPhiMET3 >= .3;

      htAlongAway = JetKinematics::htAlongHtAway(*inMet,inJets);
      rmsJetPT = JetKinematics::ptRMS(inJets);
      rmsJetDphiMET = JetKinematics::deltaPhiMETRMS(*inMet,inJets);


      bInvMass = JetKinematics::bJetInvMass(inJets,&a::isMediumBJet);
      bTransverseMass = JetKinematics::bJetTranverseMass(*inMet,inJets,&a::isMediumBJet);
      rmsBEta = JetKinematics::deltaEtaBJetRMS(inJets,&a::isMediumBJet);
      wInvMass = JetKinematics::highestPTJetPair(inJets);
      ht = JetKinematics::ht(inJets);
    }

    double met_pt;
    int    nJ90;
    int    nJ20;
    int    nTightBTags;
    double dPhiMET12;
    double dPhiMET3;
    bool   passPreselction;
    int    nMedBTags;
    double secLeadQL;
    double leadLeadQL;
    double prodQL;
    double htAlongAway;
    double rmsJetPT;
    double rmsJetDphiMET;
    double bInvMass;
    double bTransverseMass;
    double rmsBEta;
    double wInvMass;
    double leadBPT;
    double secLeadBPT;
    double ht;
  };
}





#endif
