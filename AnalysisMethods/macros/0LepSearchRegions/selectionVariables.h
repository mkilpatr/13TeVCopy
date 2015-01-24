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
      , nJ30            (0)
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
    {}



    //assumes that jets are inclusive in eta and pt!!!
    //also assumes that the jets are pt sorted!
    void processVariables(const BaseTreeAnalyzer * analyzer, const JetReader* inJetReader,  vector<RecoJetF*>& inJets, const MomentumF* inMet){
      a::analyzer = analyzer;

      met_pt          = 0;
      nJ90            = 0;
      nJ30            = 0;
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

      static const double minJetPT  = 30;
      static const double maxJetETA = 2.4;

      met_pt = inMet->pt();
      vector<RecoJetF*> jets;

      for(unsigned int iJ = 0; iJ < inJets.size(); ++iJ){
        auto& j = *inJets[iJ];
        if(TMath::Abs(j.eta()) >= maxJetETA ) continue;
        if(j.pt() < minJetPT ) break;

        jets.push_back(&j);
        nJ30++;
        if(j.pt() >= 90 ) nJ90++;
        if(analyzer->isTightBJet(j)) nTightBTags++;
        if(analyzer->isMediumBJet(j)) nMedBTags++;

        //qgl
        if(analyzer->isLooseBJet(j)) continue;
        double qgl = std::max( .01, double((1 + inJetReader->jetqgl_->at(j.index()))/2)); //trasnform from
        prodQL *= qgl;
        if(qgl > leadLeadQL){
          secLeadQL = leadLeadQL;
          leadLeadQL = qgl;
        } else if(qgl > secLeadQL)
          secLeadQL = qgl;
      }

      prodQL = TMath::Log(prodQL);
      dPhiMET12 = JetKinematics::absDPhiMETJ12(*inMet,jets);
      dPhiMET3 = JetKinematics::absDPhiMETJ3(*inMet,jets);
      passPreselction = met_pt >= 200 && nJ90 >= 2 && nJ30 >= 5 && nTightBTags >= 1  && dPhiMET12 >= .5 && dPhiMET3 >= .3;

      htAlongAway = JetKinematics::htAlongHtAway(*inMet,jets);
      rmsJetPT = JetKinematics::ptRMS(jets);
      rmsJetDphiMET = JetKinematics::deltaPhiMETRMS(*inMet,jets);


      bInvMass = JetKinematics::bJetInvMass(jets,&a::isMediumBJet);
      bTransverseMass = JetKinematics::bJetTranverseMass(*inMet,jets,&a::isMediumBJet);
      rmsBEta = JetKinematics::deltaEtaBJetRMS(jets,&a::isMediumBJet);
      wInvMass = JetKinematics::highestPTJetPair(jets);
    }

    double met_pt;
    int    nJ90;
    int    nJ30;
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
  };
}





#endif
