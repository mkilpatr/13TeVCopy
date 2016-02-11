//--------------------------------------------------------------------------------------------------
//
// Class to implement a few simple style functions to make slightly better looking ROOT plots.
//
//--------------------------------------------------------------------------------------------------
#ifndef STYLETOOLS_HH
#define STYLETOOLS_HH

#include <TCanvas.h>
#include <TH1.h>
#include <TGraph.h>
#include <TPad.h>
#include <TStyle.h>
#include <TLatex.h>
#include <TLegend.h>
#include <TPaveText.h>
#include <TObjArray.h>
#include <TASImage.h>
#include "map"


namespace StyleTools {

  enum Colors {color_data=1,                 // kBlack
               color_qcd=811,                // kSpring-9
               color_ttbar=866,              // kAzure+6
               color_ttbar1l=866,            // kAzure+6
               color_ttbar2l=625,            // kRed-7
               color_ttW=606,                // kMagenta-10
               color_ttZ=797,                // kOrange-3
               color_tW=876,                 // kViolet-4
               color_wjets=391,              // kYellow-9
               color_znunu=623,              // kRed-9
               color_other=391,              // kYellow-9
               color_T2tt=634,               // kRed+2
               color_T2tt_850_100=634,       // kRed+2
               color_T2tt_650_325=633,       // kRed+1
               color_T2tt_500_325=635,       // kRed+3
               color_T2tt_425_325=622,       // kRed-10
               color_T1tttt=602,             // kBlue+2
               color_T1tttt_1500_100=602,    // kBlue+2
               color_T1tttt_1200_800=601,    // kBlue+1
               color_comp1=634,              // kRed+2
               color_comp2=862,              // kAzure+2
               color_comp3=797,              // kOrange-3
               color_comp4=882,              // kViolet+2
               color_comp5=419,              // kGreen+3
               color_comp6=603,              // kBlue+3
               color_comp7=802,              // kOrange+2
               color_comp8=616,              // kMagenta
               color_comp9=600,              // kBlue
               color_comp10=434,             // kCyan+2
               color_comp11=800,             // kOrange
               color_comp12=417,             // kGreen+1
               color_comp13=632              // kRed
  };

  typedef std::map<TString, unsigned int> ColorMap;

  TCanvas*  MakeCanvas     (const char* name, const char *title, int dX = 500, int dY = 500);
  void      InitSubPad     (TPad* pad, int i);
  void      InitHist       (TH1 *hist, const char *xtit, const char *ytit  = "Number of Entries", int color = kBlack, int style = 0);
  void      InitGraph      (TGraph *gr, const char *title, const char *xtit, const char *ytit, double xmin, double xmax, double ymin, double ymax, int color = kBlack);
  void      SetLegendStyle (TLegend* leg);
  void      SetStyle       ();
  void      SetTDRStyle    ();
  void      CMS_lumi       (TPad* pad, int iPeriod=3, int iPosX=10);
  ColorMap  DefaultColors  ();
  
  TCanvas* newAlbum(Int_t numItems, const Char_t* name, const Char_t* title, Int_t width, Int_t height, Int_t rows, Int_t columns);
  TCanvas* drawAll(TObjArray* plots, TString name);
  int colorGetter(int counter); //For when you need quick new colors!

};

#endif
