#ifndef MT2HELPER_H
#define MT2HELPER_H
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
#include "AnalysisTools/KinematicVariables/interface/Davismt2.h"

using namespace ucsbsusy;




class Mt2Helper {
 public:

  Mt2Helper();
   ~Mt2Helper();

  // operations
	double CalcMT2( const MomentumF *visibleA, const MomentumF *visibleB, const MomentumF *ptmiss, double mEachInvisible = 0);
}; // end of class Mt2Helper

#endif
