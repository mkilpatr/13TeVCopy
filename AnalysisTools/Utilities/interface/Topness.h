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

using namespace ucsbsusy;

//class genVariables;

class Topness {


 public:

  static double mW    ;
  static double mT    ;
  static double aW    ;
  static double aT    ;
  static double aCM   ;

  MomentumF topSeen1_;
  MomentumF topMiss1_;
  MomentumF topSeen2_;
  MomentumF topMiss2_;

  // Constructor
  Topness();

  // Destructor
  virtual ~Topness() {};


  // operations
  static double topnessFunction(double pwx_, double pwy_, double pwz_, double pnz_,
				double plx_, double ply_, double plz_, double ple_,
				double pb1x_, double pb1y_, double pb1z_, double pb1e_,
				double pb2x_, double pb2y_, double pb2z_, double pb2e_,
				double pmx_, double pmy_, double pmz_, double pme_
				);
  static void minuitFunctionWrapper(int& nDim, double* gout, double& result, double *par, int flg);
  //  double topnessMinimization(MomentumF lep_, MomentumF bjet1_, MomentumF bjet2_, MomentumF met_);
  double topnessMinimization(MomentumF lep_, MomentumF bjet1_, MomentumF bjet2_,MomentumF met_);
  double getMinTopness(MomentumF lep_, MomentumF bjet1_, MomentumF bjet2_, MomentumF met_);
  double getMaxTopness(MomentumF lep_, MomentumF bjet1_, MomentumF bjet2_, MomentumF met_);
  
  std::vector<double> topnessArray(MomentumF lep_, MomentumF bjet1_, MomentumF bjet2_, MomentumF met_);
  
  MomentumF getRecoTopSeen1();
  MomentumF getRecoTopMiss1();
  MomentumF getRecoTopSeen2();
  MomentumF getRecoTopMiss2();



 private:
  TString name;


  
}; // end of class Topness

#endif
