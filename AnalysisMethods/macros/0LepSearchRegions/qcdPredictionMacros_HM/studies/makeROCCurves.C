#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/PlotUtils/interface/PlotROCCurves.h"
#include "AnalysisMethods/PlotUtils/interface/Plot.hh"
#include "AnalysisTools/Utilities/interface/JetFlavorInfo.h"
#include "AnalysisMethods/PlotUtils/interface/HistogramGetter.hh"
#include "AnalysisTools/TreeReader/interface/Defaults.h"
#include "../QCDPredictionHelper.h"
#include "TDirectory.h"
#include "TFile.h"
#endif

const TString originalTreeDir = "../../../run/plots_16_09_28_Smear";
const TString outputDir = "qcdROCCurves";
QCDSupport::SRegInfo srinfo;
vector<int> colors;

const TString weights = QCDSupport::stdMCWeight;

void makeROCCurves(const TString sigtree_600   = QCDSupport::inputDir + "/T2tt_600_300_tree.root",
		   const TString sigtree_400   = QCDSupport::inputDir + "/T2tt_400_313_tree.root",
		   const TString sigtree_800   = QCDSupport::inputDir + "/T2tt_800_100_tree.root",
		   const TString sigtree_fbd   = QCDSupport::inputDir + "/T2fbd_400_350_tree.root",
                   const TString bkgtree_tot   = QCDSupport::inputDir + "/Total_Bkg_tree.root",
                   const TString bkgtree_qcd   = QCDSupport::inputDir + "/QCD_smeared_tree_skimmed.root",
                   const TString siglabel_600  = "T2tt(600, 300)",
		   const TString siglabel_400  = "T2tt(400, 313)",
		   const TString siglabel_800  = "T2tt(800, 100)",
		   const TString siglabel_fbd  = "T2fbd(400, 350)",
                   const TString bkglabel_tot  = "Total Bkg HM",
		   const TString bkglabel_qcd  = "QCD Bkg HM",
		   const TString treename      = "Events",
                   const TString outputname    = "comp")
{


  TString selbase           = "njets>3 && misset>0.5 && pt>10";
  TString selqcd            = QCDSupport::METPresel + "&&" + QCDSupport::BaselineExtraCuts;
  TString selmet0to500      = "met <= 500";
  TString selmet500to1000   = "met > 500";
  TString selmet1000to1500  = "met > 1000";
  TString selmet1500to2000  = "met > 1500";
  //TString selmet2000to2500  = "(met > 2000)";
  //TString selmet2500toInf   = "met > 2500";
  //vector<TString> compmvas_training   = {"dphimhtj12", "dphistarmhtj12", "omegamhtj12", "chimhtj12", "dphimhtj12_qcd", "dphistarmhtj12_qcd", "omegamhtj12_qcd", "chimhtj12_qcd"};
  vector<TString> compmvas_training   = {"dphimhtj12", "dphistarmhtj12", "dphimhtj123", "dphistarmhtj123", "dphimhtj12_500_Inf", "dphimhtj123_500_Inf", "dphistarmhtj123_0_500","dphistarmhtj12_0_500", "dphistarmhtj12_500_1000", "dphistarmhtj123_500_1000"};
  vector<TString> compmvas_training_2   = {"dphimhtj12_qcd", "dphistarmhtj12_qcd", "dphimhtj123_qcd", "dphistarmhtj123_qcd",  "dphimhtj12_qcd_500_Inf", "dphimhtj123_qcd_500_Inf", "dphistarmhtj12_qcd_0_500","dphistarmhtj123_qcd_0_500", "dphistarmhtj12_qcd_500_1000", "dphistarmhtj123_qcd_500_1000"};
  vector<std::string> plotNames = {"t2tt_400_313", "t2tt_600_300", "t2tt_800_100", "t2fbd_400_350"};

  PlotROCCurves* plots = new PlotROCCurves();
  PlotROCCurves* plots_qcd = new PlotROCCurves();

  //for(int i = 0; i < 1; i++){
  //plots->addSignalTree    (sigtree_400, treename, siglabel_400);
  //plots->addSignalTree    (sigtree_600, treename, siglabel_600);
  //plots->addSignalTree    (sigtree_800, treename, siglabel_800);
  plots->addSignalTree    (sigtree_fbd, treename, siglabel_fbd);
  plots->addBackgroundTree(bkgtree_tot, treename, bkglabel_tot);

  plots->addROCVariable("dphimhtj12",                  "dphimhtj12_tot_0_500",              selqcd + "&&" + selmet0to500,     	  selqcd + "&&" + selmet0to500,     kRed,      1,        200, 0., 3.2);
  plots->addROCVariable("dphimhtj123",                 "dphimhtj123_tot_0_500",             selqcd + "&&" + selmet0to500,     	  selqcd + "&&" + selmet0to500,     kBlue,     1,        200, 0., 3.2);
  plots->addROCVariable("dphimhtj12_500_Inf",          "dphimhtj12_tot_500_Inf",            selqcd + "&&" + selmet500to1000,      selqcd + "&&" + selmet500to1000,  kGreen,    1,        200, 0., 3.2);
  plots->addROCVariable("dphimhtj123_500_Inf",         "dphimhtj123_tot_500_Inf",           selqcd + "&&" + selmet500to1000,      selqcd + "&&" + selmet500to1000,  kMagenta,  1,        200, 0., 3.2);
  plots->addROCVariable("dphistarmhtj12_0_500",        "dphistarmhtj12_tot_0_500",          selqcd + "&&" + selmet0to500,         selqcd + "&&" + selmet0to500,     kRed,      2,        200, 0., 3.2);
  plots->addROCVariable("dphistarmhtj123_0_500",       "dphistarmhtj123_tot_0_500",         selqcd + "&&" + selmet0to500,         selqcd + "&&" + selmet0to500,     kBlue,     2,        200, 0., 3.2);
  plots->addROCVariable("dphistarmhtj12_500_1000",     "dphistarmhtj12_tot_500_Inf",        selqcd + "&&" + selmet500to1000,      selqcd + "&&" + selmet500to1000,  kGreen,    2,        200, 0., 3.2);
  plots->addROCVariable("dphistarmhtj123_500_1000",    "dphistarmhtj123_tot_500_Inf",       selqcd + "&&" + selmet500to1000,      selqcd + "&&" + selmet500to1000,  kMagenta,  2,        200, 0., 3.2);
 
  //plots_qcd->addSignalTree    (sigtree_400, treename, siglabel_400);
  //plots_qcd->addSignalTree    (sigtree_600, treename, siglabel_600);
  //plots_qcd->addSignalTree    (sigtree_800, treename, siglabel_800);
  plots_qcd->addSignalTree    (sigtree_fbd, treename, siglabel_fbd); 
  plots_qcd->addBackgroundTree(bkgtree_qcd, treename, bkglabel_qcd);

  plots_qcd->addROCVariable("dphimhtj12_qcd",              "dphimhtj12_qcd_tot_0_500",              selqcd + "&&" + selmet0to500,         selqcd + "&&" + selmet0to500,     kRed,      1,        200, 0., 3.2);
  plots_qcd->addROCVariable("dphimhtj123_qcd",             "dphimhtj123_qcd_tot_0_500",             selqcd + "&&" + selmet0to500,         selqcd + "&&" + selmet0to500,     kBlue,     1,        200, 0., 3.2);
  plots_qcd->addROCVariable("dphimhtj12_qcd_500_Inf",      "dphimhtj12_qcd_tot_500_Inf",            selqcd + "&&" + selmet500to1000,      selqcd + "&&" + selmet500to1000,  kGreen,    1,        200, 0., 3.2);
  plots_qcd->addROCVariable("dphimhtj123_qcd_500_Inf",     "dphimhtj123_qcd_tot_500_Inf",           selqcd + "&&" + selmet500to1000,      selqcd + "&&" + selmet500to1000,  kMagenta,  1,        200, 0., 3.2);
  plots_qcd->addROCVariable("dphistarmhtj12_qcd_0_500",    "dphistarmhtj12_qcd_tot_0_500",          selqcd + "&&" + selmet0to500,         selqcd + "&&" + selmet0to500,     kRed,      2,        200, 0., 3.2);
  plots_qcd->addROCVariable("dphistarmhtj123_qcd_0_500",   "dphistarmhtj123_qcd_tot_0_500",         selqcd + "&&" + selmet0to500,         selqcd + "&&" + selmet0to500,     kBlue,     2,        200, 0., 3.2);
  plots_qcd->addROCVariable("dphistarmhtj12_qcd_500_1000", "dphistarmhtj12_qcd_tot_500_Inf",        selqcd + "&&" + selmet500to1000,      selqcd + "&&" + selmet500to1000,  kGreen,    2,        200, 0., 3.2);
  plots_qcd->addROCVariable("dphistarmhtj123_qcd_500_1000","dphistarmhtj123_qcd_tot_500_Inf",       selqcd + "&&" + selmet500to1000,      selqcd + "&&" + selmet500to1000,  kMagenta,  2,        200, 0., 3.2);

  plots->    addCompPlot(outputname+"_qcdVariables_1_t2fbd_400_350_tot",      compmvas_training);
  plots_qcd->addCompPlot(outputname+"_qcdVariables_2_t2fbd_400_350_qcd",      compmvas_training_2);
  
  plots->    getCompPlot(outputname+"_qcdVariables_1_t2fbd_400_350_tot")->setLegend(0.45, 0.4, 0.9, 0.55);
  plots_qcd->getCompPlot(outputname+"_qcdVariables_2_t2fbd_400_350_qcd")->setLegend(0.45, 0.4, 0.9, 0.55);

  float sigeff_dphi12 =     EffPlotTools::getEfficiencyForCutValue(plots->getSigHist("dphimhtj12"),     0.5, 0)[0];
  float bkgeff_dphi12 =     EffPlotTools::getEfficiencyForCutValue(plots->getBkgHist("dphimhtj12"),     0.5, 0)[0];
  float sigeff_dphi123 =     EffPlotTools::getEfficiencyForCutValue(plots->getSigHist("dphimhtj123"),     0.5, 0)[0];
  float bkgeff_dphi123 =     EffPlotTools::getEfficiencyForCutValue(plots->getBkgHist("dphimhtj123"),     0.5, 0)[0];
  plots->    getCompPlot(outputname+"_qcdVariables_1_t2fbd_400_350_tot")->addLine(0.0, sigeff_dphi12, bkgeff_dphi12, sigeff_dphi12, 1, 2, "#Delta#phij12 < 0.5");
  plots->    getCompPlot(outputname+"_qcdVariables_1_t2fbd_400_350_tot")->addLine(bkgeff_dphi12, 0.0, bkgeff_dphi12, sigeff_dphi12, 1, 2);
  plots->    getCompPlot(outputname+"_qcdVariables_1_t2fbd_400_350_tot")->addLine(0.0, sigeff_dphi123, bkgeff_dphi123, sigeff_dphi123, 1, 6, "#Delta#phij123 < 0.5");
  plots->    getCompPlot(outputname+"_qcdVariables_1_t2fbd_400_350_tot")->addLine(bkgeff_dphi123, 0.0, bkgeff_dphi123, sigeff_dphi123, 1, 6);
  float sigeff_500_dphi12 =     EffPlotTools::getEfficiencyForCutValue(plots->getSigHist("dphimhtj12_500_Inf"),     0.5, 0)[0];
  float bkgeff_500_dphi12 =     EffPlotTools::getEfficiencyForCutValue(plots->getBkgHist("dphimhtj12_500_Inf"),     0.5, 0)[0];
  float sigeff_500_dphi123 =     EffPlotTools::getEfficiencyForCutValue(plots->getSigHist("dphimhtj123_500_Inf"),     0.5, 0)[0];
  float bkgeff_500_dphi123 =     EffPlotTools::getEfficiencyForCutValue(plots->getBkgHist("dphimhtj123_500_Inf"),     0.5, 0)[0];
  plots->    getCompPlot(outputname+"_qcdVariables_1_t2fbd_400_350_tot")->addLine(0.0, sigeff_500_dphi12, bkgeff_500_dphi12, sigeff_500_dphi12, 1, 4, "#Delta#phij12 < 0.5");
  plots->    getCompPlot(outputname+"_qcdVariables_1_t2fbd_400_350_tot")->addLine(bkgeff_500_dphi12, 0.0, bkgeff_500_dphi12, sigeff_500_dphi12, 1, 4);
  plots->    getCompPlot(outputname+"_qcdVariables_1_t2fbd_400_350_tot")->addLine(0.0, sigeff_500_dphi123, bkgeff_500_dphi123, sigeff_500_dphi123, 1, 8, "#Delta#phij123 < 0.5");
  plots->    getCompPlot(outputname+"_qcdVariables_1_t2fbd_400_350_tot")->addLine(bkgeff_500_dphi123, 0.0, bkgeff_500_dphi123, sigeff_500_dphi123, 1, 8);
  plots->plotAll();

  float sigeff_dphi12_qcd =     EffPlotTools::getEfficiencyForCutValue(plots_qcd->getSigHist("dphimhtj12_qcd"),     0.5, 0)[0];
  float bkgeff_dphi12_qcd =     EffPlotTools::getEfficiencyForCutValue(plots_qcd->getBkgHist("dphimhtj12_qcd"),     0.5, 0)[0];
  float sigeff_dphi123_qcd =     EffPlotTools::getEfficiencyForCutValue(plots_qcd->getSigHist("dphimhtj123_qcd"),     0.5, 0)[0];
  float bkgeff_dphi123_qcd =     EffPlotTools::getEfficiencyForCutValue(plots_qcd->getBkgHist("dphimhtj123_qcd"),     0.5, 0)[0];
  plots_qcd->getCompPlot(outputname+"_qcdVariables_2_t2fbd_400_350_qcd")->addLine(0.0, sigeff_dphi12_qcd, bkgeff_dphi12_qcd, sigeff_dphi12_qcd, 1, 2, "#Delta#phij12 < 0.5");
  plots_qcd->getCompPlot(outputname+"_qcdVariables_2_t2fbd_400_350_qcd")->addLine(bkgeff_dphi12_qcd, 0.0, bkgeff_dphi12_qcd, sigeff_dphi12_qcd, 1, 2);
  plots_qcd->getCompPlot(outputname+"_qcdVariables_2_t2fbd_400_350_qcd")->addLine(0.0, sigeff_dphi123_qcd, bkgeff_dphi123_qcd, sigeff_dphi123_qcd, 1, 6, "#Delta#phij123 < 0.5");
  plots_qcd->getCompPlot(outputname+"_qcdVariables_2_t2fbd_400_350_qcd")->addLine(bkgeff_dphi123_qcd, 0.0, bkgeff_dphi123_qcd, sigeff_dphi123_qcd, 1, 6);
  float sigeff_500_dphi12_qcd =     EffPlotTools::getEfficiencyForCutValue(plots_qcd->getSigHist("dphimhtj12_qcd_500_Inf"),     0.5, 0)[0];
  float bkgeff_500_dphi12_qcd =     EffPlotTools::getEfficiencyForCutValue(plots_qcd->getBkgHist("dphimhtj12_qcd_500_Inf"),     0.5, 0)[0];
  float sigeff_500_dphi123_qcd =     EffPlotTools::getEfficiencyForCutValue(plots_qcd->getSigHist("dphimhtj123_qcd_500_Inf"),     0.5, 0)[0];
  float bkgeff_500_dphi123_qcd =     EffPlotTools::getEfficiencyForCutValue(plots_qcd->getBkgHist("dphimhtj123_qcd_500_Inf"),     0.5, 0)[0];
  plots_qcd->getCompPlot(outputname+"_qcdVariables_2_t2fbd_400_350_qcd")->addLine(0.0, sigeff_500_dphi12_qcd, bkgeff_500_dphi12_qcd, sigeff_500_dphi12_qcd, 1, 4, "#Delta#phij12 < 0.5");
  plots_qcd->getCompPlot(outputname+"_qcdVariables_2_t2fbd_400_350_qcd")->addLine(bkgeff_500_dphi12_qcd, 0.0, bkgeff_500_dphi12_qcd, sigeff_500_dphi12_qcd, 1, 4);
  plots_qcd->getCompPlot(outputname+"_qcdVariables_2_t2fbd_400_350_qcd")->addLine(0.0, sigeff_500_dphi123_qcd, bkgeff_500_dphi123_qcd, sigeff_500_dphi123_qcd, 1, 8, "#Delta#phij123 < 0.5");
  plots_qcd->getCompPlot(outputname+"_qcdVariables_2_t2fbd_400_350_qcd")->addLine(bkgeff_500_dphi123_qcd, 0.0, bkgeff_500_dphi123_qcd, sigeff_500_dphi123_qcd, 1, 8);
  plots_qcd->plotAll();

  plots_qcd->getInfoForSignalEff("dphistarmhtj12_qcd_0_500",        0.45);
  plots_qcd->getInfoForSignalEff("dphistarmhtj123_qcd_0_500",       0.36);
  plots_qcd->getInfoForSignalEff("dphistarmhtj12_qcd_500_1000",     0.45);
  plots_qcd->getInfoForSignalEff("dphistarmhtj123_qcd_500_1000",    0.36);
  
  plots_qcd->getInfoForSignalEff("dphistarmhtj12_qcd_0_500",        0.67);
  plots_qcd->getInfoForSignalEff("dphistarmhtj123_qcd_0_500",       0.54);
  plots_qcd->getInfoForSignalEff("dphistarmhtj12_qcd_500_1000",     0.67);
  plots_qcd->getInfoForSignalEff("dphistarmhtj123_qcd_500_1000",    0.54);
 
  //plots->getInfoForCut("dphimhtj12", 0.5);
  //plots->getInfoForCut("dphimhtj123", 0.5);
  //plots_qcd->getInfoForCut("dphimhtj12_qcd", 0.5);
  //plots_qcd->getInfoForCut("dphimhtj123_qcd", 0.5);
//}
  //plots->getInfoForSignalEff("dphimhtj12", 0.7);
  //plots->getInfoForSignalEff("dphistarmhtj12", 0.7);
  //plots->getInfoForSignalEff("dphimhtj123", 0.7);
  //plots->getInfoForSignalEff("dphistarmhtj123", 0.7);
}
