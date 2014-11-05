#ifndef TOPNESS_H
#define TOPNESS_H
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

//class genVariables;

class Topness {


 public:

  static float mW    ;
  static float mT    ;
  static float aW    ;
  static float aT    ;
  static float aCM   ;

  MomentumF *topSeen1_;
  MomentumF *topMiss1_;
  MomentumF *topSeen2_;
  MomentumF *topMiss2_;

  // Constructor
  Topness();

  // Destructor
  virtual ~Topness() {};


  // operations
  static float topnessFunction(double pwx_, double pwy_, double pwz_, double pnz_,
			       double plx_, double ply_, double plz_, double ple_,
			       double pb1x_, double pb1y_, double pb1z_, double pb1e_,
			       double pb2x_, double pb2y_, double pb2z_, double pb2e_,
			       double pmx_, double pmy_, double pmz_, double pme_
			       );
  static void minuitFunctionWrapper(int& nDim, double* gout, double& result, double *par, int flg);
  //  float topnessMinimization(MomentumF lep_, MomentumF bjet1_, MomentumF bjet2_, MomentumF met_);
  float topnessMinimization(LeptonF *lep_, RecoJetF *bjet1_, RecoJetF *bjet2_,MomentumF *met_);
  float getMinTopness(LeptonF *lep_, RecoJetF *bjet1_, RecoJetF *bjet2_, MomentumF *met_);
  float getMaxTopness(LeptonF *lep_, RecoJetF *bjet1_, RecoJetF *bjet2_, MomentumF *met_);
  //  bool sortInDecreasingCSV(RecoJetF *jet1, RecoJetF *jet2);
  float findMinTopnessConfiguration(LeptonFCollection *leptons, RecoJetFCollection *jets, MomentumF *met);

  std::vector<float> topnessArray(LeptonF *lep_, RecoJetF *bjet1_, RecoJetF *bjet2_, MomentumF *met_);
  
  MomentumF *getRecoTopSeen1();
  MomentumF *getRecoTopMiss1();
  MomentumF *getRecoTopSeen2();
  MomentumF *getRecoTopMiss2();


  
}; // end of class Topness

#endif
