#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/PlotUtils/interface/PlotROCCurves.h"
#endif

void makeROCCurves(const TString sigtree    = "tauMVATrees/realtaus_tt1tau_addtaumva.root",
                   const TString bkgtree    = "tauMVATrees/faketaus_T2tt_500_325_addtaumva.root",
                   const TString siglabel   = "#varepsilon(t#bar{t} gen-matched h^{#pm})",
                   const TString bkglabel   = "#varepsilon(T2tt(500,325) non-matched h^{#pm})",
                   const TString treename   = "Candidates",
                   const TString outputname = "comp_meddm")
{

  TString selbase   = "njets>3 && misset>150 && pt>10";
  TString selmt     = selbase + " && absdz<0.2 && mt<100";
//  TString seldphi   = selbase + " && absdz<0.2 && dphimet<1.34";

  vector<TString> compmvas_training = {"taumva", "taumva_new"};

  vector<TString> compvars = {"mt"};

  PlotROCCurves* plots = new PlotROCCurves();

  plots->addSignalTree    (sigtree, treename, siglabel);
  plots->addBackgroundTree(bkgtree, treename, bkglabel);

  plots->addROCVariable("taumva", "Old training", selmt, selmt, kRed+2,    200, -1.0, 1.0);
  plots->addROCVariable("taumva_new", "New training", selmt, selmt, kBlue+2,    200, -1.0, 1.0);
//  plots->addROCVariable("dphimet", "|#Delta#phi(h^{#pm}, #slash{E}_{T})|", selbase, selbase, kRed+2, 210, 0, 3.15, 1);
//  plots->addROCVariable("dphiw", "|#Delta#phi(h^{#pm}, W)|", selbase, selbase, kBlue+2, 210, 0, 3.15, 1);
  plots->addROCVariable("mt", "m_{T} (with #gamma)", selbase, selbase, kOrange+2, 250, 0, 500, 1);
//  plots->addROCVariable("mtnophoton", "m_{T} (without #gamma)", selbase, selbase, kAzure+2, 250, 0, 500, 1);

  plots->addCompPlot(outputname+"_mvatraining",      compmvas_training);
  plots->addCompPlot(outputname+"_mvatraining_zoom", compmvas_training, 0.0, 0.1, 0.4, 0.9);
  plots->addCompPlot(outputname+"_preselvars",       compvars);
  plots->addCompPlot(outputname+"_preselvars_zoom",  compvars, 0.0, 0.4, 0.8, 1.0);

  plots->getCompPlot(outputname+"_mvatraining")->setLegend(0.23, 0.25, 0.9, 0.35);
  plots->getCompPlot(outputname+"_mvatraining_zoom")->setLegend(0.3, 0.25, 0.93, 0.35);

  plots->getCompPlot(outputname+"_preselvars_zoom")->setLegend(0.55, 0.3, 0.93, 0.5);

  float sigeff_mt100 = EffPlotTools::getEfficiencyForCutValue(plots->getSigHist("mt"), 100.0, 1)[0];
  float bkgeff_mt100 = EffPlotTools::getEfficiencyForCutValue(plots->getBkgHist("mt"), 100.0, 1)[0];
  plots->getCompPlot(outputname+"_preselvars")->addLine(0.0, sigeff_mt100, bkgeff_mt100, sigeff_mt100, 1, kDashed, "m_{T} < 100 GeV");
  plots->getCompPlot(outputname+"_preselvars")->addLine(bkgeff_mt100, 0.0, bkgeff_mt100, sigeff_mt100, 1, kDashed);
  plots->getCompPlot(outputname+"_preselvars_zoom")->addLine(0.0, sigeff_mt100, bkgeff_mt100, sigeff_mt100, 1, kDashed, "m_{T} < 100 GeV");
  plots->getCompPlot(outputname+"_preselvars_zoom")->addLine(bkgeff_mt100, 0.8, bkgeff_mt100, sigeff_mt100, 1, kDashed);

  plots->plotAll();

  plots->getInfoForCut("mt", 100.0);
//  plots->getInfoForBackgroundEff("dphimet", bkgeff_mt100);

  plots->getInfoForSignalEff("taumva_new", 0.7);
  plots->getInfoForSignalEff("taumva", 0.7);

}
