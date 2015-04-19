#ifndef CREATEPSEUDOJETS_H
#define CREATEPSEUDOJETS_H
#include <vector>
#include <sstream>
#include <string>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <stdio.h>
#include "TLorentzVector.h"
#include "TFitter.h"
#include "TString.h"
#include "AnalysisTools/DataFormats/interface/Momentum.h"
#include "AnalysisTools/DataFormats/interface/Lepton.h"
#include "AnalysisTools/DataFormats/interface/Jet.h"

using namespace ucsbsusy;

class CreatePseudoJets {
 public:

  CreatePseudoJets();
  ~CreatePseudoJets();


  // operations
  void getInitAxis(const std::vector<RecoJetF*>& jets,RecoJetF &initJet1,RecoJetF &initJet2,unsigned int &indx1,unsigned int &indx2);
  float getLundDistance(RecoJetF JetAxis,RecoJetF *tmpJet);
  void makePseudoJets(const std::vector<RecoJetF*>& jets,MomentumF &pseudoJet1, MomentumF &pseudoJet2);

}; // end of class CreatePseudoJets

#endif
