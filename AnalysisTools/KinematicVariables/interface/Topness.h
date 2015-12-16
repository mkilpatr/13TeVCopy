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
#include "AnalysisTools/TreeReader/interface/Defaults.h"

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
  double     topness;
  MomentumD top1_l;
  MomentumD top1_n;
  MomentumD top1_b;
  MomentumD top1_w;
  MomentumD top2_w;
  MomentumD top2_b;
  double tW ;
  double tTL;
  double tTM;
  double tCM;
};


class Topness {
 public:
  static const double mW    ;
  static const double mT    ;
  static const double aW    ;
  static const double aT    ;
  static const double aCM   ;

  TFitter *minimizer;

  Topness();
  ~Topness();


  // operations
  static double topnessFunction(double pwx_, double pwy_, double pwz_, double pnz_,
			       double plx_, double ply_, double plz_, double ple_,
			       double pb1x_, double pb1y_, double pb1z_, double pb1e_,
			       double pb2x_, double pb2y_, double pb2z_, double pb2e_,
			       double pmx_, double pmy_, double pmz_, double pme_, TopnessInformation * info = 0
			       );

  static void minuitFunctionWrapper(int& nDim, double* gout, double& result, double *par, int flg);
  double topnessMinimization(const MomentumF *lep_, const MomentumF *bjet1_, const MomentumF *bjet2_, const MomentumF *met_,TopnessInformation * info = 0);
  double getMinTopness      (const MomentumF *lep_, const MomentumF *bjet1_, const MomentumF *bjet2_, const MomentumF *met_,TopnessInformation * info = 0);
  double getMaxTopness      (const MomentumF *lep_, const MomentumF *bjet1_, const MomentumF *bjet2_, const MomentumF *met_,TopnessInformation * info = 0);
  double findMinTopnessConfiguration(const std::vector<LeptonF*>& leptons,const std::vector<RecoJetF*>& jets,const MomentumF *met,TopnessInformation * info = 0);
  double findMinTopnessConfiguration(const LeptonF* lepton,const std::vector<RecoJetF*>& jets,const MomentumF *met,TopnessInformation * info = 0);
  double find2BtagWeightedTopness(const std::vector<LeptonF*>& leptons,const std::vector<RecoJetF*>& jets,const MomentumF *met,int option);

  double round(double num, int x){
    return ceil( ( num * pow( 10,x ) ) - 0.5 ) / pow( 10,x );
  }
  
}; // end of class Topness

#endif
