//--------------------------------------------------------------------------------------------------
//
// Class to implement a few simple style functions to make slightly better looking ROOT plots.
//
//--------------------------------------------------------------------------------------------------
#ifndef STYLETOOLS_HH
#define STYLETOOLS_HH

#include <TCanvas.h>
#include <TH1.h>
#include <TPad.h>
#include <TStyle.h>
#include <TLegend.h>
#include <TPaveText.h>
#include "map"

namespace StyleTools {

  enum Colors {color_data=1,                 // kBlack
               color_qcd=414,                // kGreen-2
               color_ttbar=866,              // kAzure+6
               color_ttbar1l=866,            // kAzure+6
               color_ttbar2l=625,            // kRed-7
               color_ttW=606,                // kBlue+6
               color_ttZ=797,                // kOrange-3
               color_tW=391,                 // kYellow-9
               color_wjets=606,              // kBlue+6
               color_znunu=632,              // kRed
               color_other=391,              // kYellow-9
               color_T2tt=634,               // kRed+2
               color_T2tt_850_100=634,       // kRed+2
               color_T2tt_650_325=633,       // kRed+3
               color_T2tt_500_325=635,       // kRed+4
               color_T1tttt=602,             // kBlue+2
               color_T1tttt_1500_100=602,    // kBlue+2
               color_T1tttt_1200_800=601,    // kBlue+1
               color_comp1=634,              // kRed+2
               color_comp2=603,              // kBlue+3
               color_comp3=419,              // kGreen+3
               color_comp4=882,              // kViolet+2
               color_comp5=802,              // kOrange+2
               color_comp6=862,              // kAzure+2
               color_comp7=902};             // kPink+2

  typedef std::map<TString, unsigned int> ColorMap;

  TCanvas*	MakeCanvas	(const char* name, const char *title, int dX = 500, int dY = 500);
  void		InitSubPad	(TPad* pad, int i);
  void		InitHist	(TH1 *hist, const char *xtit, const char *ytit  = "Number of Entries", int color = kBlack, int style = 0);
  void		SetLegendStyle	(TLegend* leg);
  void		SetStyle	();
  ColorMap      DefaultColors   ();
  
};

#endif
