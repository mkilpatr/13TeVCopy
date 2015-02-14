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

  enum Colors {color_data=1,
               color_qcd=414,
               color_ttbar=866,
               color_ttbar1l=866,
               color_ttbar2l=625,
               color_ttW=606,
               color_ttZ=797,
               color_tW=391,
               color_wjets=606,
               color_znunu=632,
               color_other=391,
               color_T2tt=634,
               color_T2tt_850_100=634,
               color_T2tt_650_325=633,
               color_T2tt_500_325=635,
               color_T1tttt=602,
               color_T1tttt_1500_100=602,
               color_T1tttt_1200_800=601,
               color_comp1=634,
               color_comp2=603,
               color_comp3=419,
               color_comp4=882,
               color_comp5=802,
               color_comp6=862,
               color_comp7=902};

  typedef std::map<TString, unsigned int> ColorMap;

  TCanvas*	MakeCanvas	(const char* name, const char *title, int dX = 500, int dY = 500);
  void		InitSubPad	(TPad* pad, int i);
  void		InitHist	(TH1 *hist, const char *xtit, const char *ytit  = "Number of Entries", int color = kBlack, int style = 0);
  void		SetLegendStyle	(TLegend* leg);
  void		SetStyle	();
  ColorMap      DefaultColors   ();
  
};

#endif
