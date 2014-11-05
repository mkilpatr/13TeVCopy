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

class TopnessInformation {
public:
  TopnessInformation() :
    topness(0),
    tW (0),
    tTL(0),
    tTM(0),
    tCM(0)
{};
  float     topness;
  MomentumF top1_l;
  MomentumF top1_n;
  MomentumF top1_b;
  MomentumF top2_w;
  MomentumF top2_b;
  float tW ;
  float tTL;
  float tTM;
  float tCM;
};


class Topness {
 public:
  static const float mW    ;
  static const float mT    ;
  static const float aW    ;
  static const float aT    ;
  static const float aCM   ;

  TFitter *minimizer;

  Topness();
  ~Topness();


  // operations
  static float topnessFunction(double pwx_, double pwy_, double pwz_, double pnz_,
			       double plx_, double ply_, double plz_, double ple_,
			       double pb1x_, double pb1y_, double pb1z_, double pb1e_,
			       double pb2x_, double pb2y_, double pb2z_, double pb2e_,
			       double pmx_, double pmy_, double pmz_, double pme_, TopnessInformation * info = 0
			       );

  static void minuitFunctionWrapper(int& nDim, double* gout, double& result, double *par, int flg);
  //  float topnessMinimization(MomentumF lep_, MomentumF bjet1_, MomentumF bjet2_, MomentumF met_);
  float topnessMinimization(const MomentumF *lep_, const MomentumF *bjet1_, const MomentumF *bjet2_, const MomentumF *met_,TopnessInformation * info = 0);
  float getMinTopness      (const MomentumF *lep_, const MomentumF *bjet1_, const MomentumF *bjet2_, const MomentumF *met_,TopnessInformation * info = 0);
  float getMaxTopness      (const MomentumF *lep_, const MomentumF *bjet1_, const MomentumF *bjet2_, const MomentumF *met_,TopnessInformation * info = 0);
  //  bool sortInDecreasingCSV(RecoJetF *jet1, RecoJetF *jet2);
  float findMinTopnessConfiguration(const std::vector<LeptonF*>& leptons,const std::vector<RecoJetF*>& jets,const MomentumF *met,TopnessInformation * info = 0);


  
}; // end of class Topness

#endif
