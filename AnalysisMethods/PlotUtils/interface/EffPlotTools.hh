#ifndef EFFPLOTTOOLS_HH
#define EFFPLOTTOOLS_HH

#include "TH1F.h"
#include "TGraph.h"
#include "TGraphAsymmErrors.h"
#include "TEfficiency.h"
#include "TString.h"

namespace EffPlotTools {

  TGraphAsymmErrors* computeEffGraph(TH1F* pass, TH1F* total, bool debug=false);
  TGraph* computeROCCurve(TH1F* signal, TH1F* background, TString title, bool reversecutdir = false, bool plotbkgrej = false, bool reverseaxes=false);
  float * getCutValueForEfficiency(TH1F* hist, float targeteff, bool reversecutdir=false);
  float * getEfficiencyForCutValue(TH1F* hist, float cut, bool reversecutdir=false);
  float * getCutValueForBestEffSOverSqrtEffB(TH1F* sighist, TH1F* bkghist, bool reversecutdir=false);

}

#endif
