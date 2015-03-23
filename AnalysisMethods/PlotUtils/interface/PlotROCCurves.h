#ifndef PLOTROCCURVES_H
#define PLOTROCCURVES_H

#include <TTree.h>
#include "AnalysisMethods/PlotUtils/interface/Plot.hh"
#include "AnalysisMethods/PlotUtils/interface/StyleTools.hh"
#include "AnalysisMethods/PlotUtils/interface/EffPlotTools.hh"

class PlotROCCurves {

  public :

    struct ROCPlot {

      public :
        TH1F*        sighist;
        TH1F*        bkghist;
        TString      varname;
        TString      label;
        unsigned int color;
        bool         reversecut;

        ROCPlot(TH1F* insighist, TH1F* inbkghist, TString invarname, TString inlabel, unsigned int incolor, bool inreversecut) :
          sighist    (insighist),
          bkghist    (inbkghist),
          varname    (invarname),
          label      (inlabel),
          color      (incolor),
          reversecut (inreversecut)
        {}

    };


  public :

    PlotROCCurves();
    virtual ~PlotROCCurves() {}

    void    addSignalTree(const TString fname, const TString tname, const TString label);
    void    addBackgroundTree(const TString fname, const TString tname, const TString label);

    void    addROCVariable(const TString varname, const TString label, const TString sigsel, const TString bkgsel, const unsigned int color, const int nbins, const float xmin, const float xmax, const bool cutlessthan = false);

    void    addCompPlot(const TString compplotname, vector<TString> compvarnames, const double xmin = 0.0, const double xmax = 1.0, const double ymin = 0.0, const double ymax = 0.0, const bool plotbkgrej = false, const bool plotsigvsbkg = true);

    void    plotAll(TString format="png");

    void    getInfoForSignalEff(const TString varname, const float eff);
    void    getInfoForBackgroundEff(const TString varname, const float eff);
    void    getInfoForCut(const TString varname, const float cut);

    Plot*   getCompPlot(const TString compplotname);

    TH1F*   getSigHist(const TString varname);
    TH1F*   getBkgHist(const TString varname);

  private :
    TFile*  sigfile_;
    TFile*  bkgfile_;

    TTree*  sigtree_;
    TTree*  bkgtree_;

    TString siglabel_;
    TString bkglabel_;

    vector<ROCPlot> rocplots_;
    vector<Plot*>   compplots_;

};

#endif
