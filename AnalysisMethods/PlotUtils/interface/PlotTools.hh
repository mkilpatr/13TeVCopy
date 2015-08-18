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
#include "AnalysisMethods/PlotUtils/interface/Sample.hh"

using namespace std;

namespace PlotTools {

  void initSamples(TString conf, vector<Sample*> &samplev);

}

#endif
