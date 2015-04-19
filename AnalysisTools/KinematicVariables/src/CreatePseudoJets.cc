#include "../interface/CreatePseudoJets.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
#include "Math/VectorUtil.h"

using namespace std;
using namespace ucsbsusy;


CreatePseudoJets::CreatePseudoJets() {}
CreatePseudoJets::~CreatePseudoJets(){}

void CreatePseudoJets::getInitAxis(const std::vector<RecoJetF*>& jets,RecoJetF &initJet1,RecoJetF &initJet2,unsigned int &indx1,unsigned int &indx2) {

  if (jets.size()<2) { 
    initJet1.p4().SetPxPyPzE(0.0001,0.0001,0.0001,0.0001);
    initJet2.p4().SetPxPyPzE(0.0001,0.0001,0.0001,0.0001); }
  

  float invMassMax = -1.;
  for (UInt_t i1=0; i1<jets.size(); ++i1) {
    for (UInt_t i2=0; i2<jets.size(); ++i2) {

      if (i1>i2) {
	float tmpMass = (jets[i1]->p4()+jets[i2]->p4()).M();
	if (tmpMass>invMassMax) { 
	  invMassMax = tmpMass; 
	  indx1 = i1; indx2 = i2; 
	  initJet1.p4().SetPxPyPzE(jets[i1]->p4().Px(),jets[i1]->p4().Py(),jets[i1]->p4().Pz(),jets[i1]->p4().E());
	  initJet2.p4().SetPxPyPzE(jets[i2]->p4().Px(),jets[i2]->p4().Py(),jets[i2]->p4().Pz(),jets[i2]->p4().E()); }
      } else { continue; }


    } // end of looping over jets-i2
  } // end of looping over jets-i1
  
} //end of getInitAxis



float CreatePseudoJets::getLundDistance(RecoJetF JetAxis,RecoJetF *tmpJet) {


  float cosTheta = ROOT::Math::VectorUtil::CosTheta(JetAxis.p4(),tmpJet->p4());
  float tmpDistance = (JetAxis.p4().E()-(tmpJet->p4().P())*(cosTheta))*((JetAxis.p4().E())/(pow((JetAxis.p4().E()+tmpJet->p4().E()),2)));

  return tmpDistance;
} //end of getLundDistance()


void CreatePseudoJets::makePseudoJets(const std::vector<RecoJetF*>& jets, MomentumF &pseudoJet1, MomentumF &pseudoJet2) {

  RecoJetF initJet1; RecoJetF initJet2;
  unsigned int indx1 = 0; unsigned int indx2 = 0;
  getInitAxis(jets,initJet1,initJet2,indx1,indx2);

  pseudoJet1 = initJet1;
  pseudoJet2 = initJet2;

  for  (UInt_t i1=0; i1<jets.size(); ++i1) {

    if ((i1==indx1) || (i1==indx2)) { continue; }
    RecoJetF *tmpJet = jets[i1]; 
    float tmp1 = getLundDistance(initJet1,tmpJet);
    float tmp2 = getLundDistance(initJet2,tmpJet);

    if (tmp1<=tmp2) { pseudoJet1.p4() += tmpJet->p4(); }
    else            { pseudoJet2.p4() += tmpJet->p4(); }

  }

} // end of makePseudoJets
