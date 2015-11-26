#ifndef TOOLS_HH
#define TOOLS_HH

#include <vector>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <string>
#include <assert.h>
#include <TString.h>
#include <TH1.h>
#include "AnalysisMethods/PlotUtils/interface/Sample.hh"

using namespace std;

namespace PlotTools {

  void initSamples(TString conf, vector<Sample*> &samplev);

  void toUnderflow(TH1 * h);
  void toOverflow (TH1 * h);
  void normalize  (TH1 * h);
  TH1* rebin      (TH1* h, int n);
  TH1* rebin      (TH1* h, int n, double * bins);
}

#endif
