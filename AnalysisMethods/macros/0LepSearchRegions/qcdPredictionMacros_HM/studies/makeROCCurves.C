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

const TString originalTreeDir = "../../../run/plots_16_11_15_Smear";
const TString outputDir = "qcdROCCurves_new";
QCDSupport::SRegInfo srinfo;
vector<int> colors;

const TString weights = QCDSupport::stdMCWeight;

void makeROCCurves(const TString sigtree_400   = QCDSupport::inputDir + "/T2tt_400_313_tree.root",
                   const TString sigtree_600   = QCDSupport::inputDir + "/T2tt_600_300_tree.root",
		   const TString sigtree_800   = QCDSupport::inputDir + "/T2tt_800_100_tree.root",
		   const TString bkgtree_tot   = QCDSupport::inputDir + "/Total_Bkg_tree.root",
                   const TString bkgtree_qcd   = QCDSupport::inputDir + "/QCD_smeared_tree.root",
		   const TString siglabel_400  = "T2tt(400, 313)",
		   const TString siglabel_600  = "T2tt(600, 300)",
		   const TString siglabel_800  = "T2tt(800, 100)",
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
  //vector<TString> compmvas_training   = {"dphimetj12", "dphistarmetj12", "omegametj12", "chimetj12", "dphimetj12", "dphistarmetj12", "omegametj12", "chimetj12"};
  vector<TString> compmvas_training   = {"dphimetj12", "dphistarmetj12", "dphimetj123", "dphistarmetj123", "dphimetj12_500_Inf", "dphimetj123_500_Inf", "dphistarmetj123_0_500","dphistarmetj12_0_500", "dphistarmetj12_500_1000", "dphistarmetj123_500_1000", "dphimetj1234", "dphistarmetj1234_0_500", "dphistarmetj1234_500_Inf", "dphimetj1234_500_Inf"};
  vector<TString> compmvas_training_2   = {"dphimetj12", "dphistarmetj12", "dphimetj123", "dphistarmetj123",  "dphimetj1234", "dphistarmetj1234"};
  vector<std::string> plotNames = {"t2tt_800_100", "t2tt_800_100", "t2tt_800_100", "t2tt_800_100"};

  PlotROCCurves* plots = new PlotROCCurves();
  PlotROCCurves* plots_qcd = new PlotROCCurves();

  //for(int i = 0; i < 1; i++){
  //plots->addSignalTree    (sigtree_fbd, treename, siglabel_fbd);
  //plots->addBackgroundTree(bkgtree_tot, treename, bkglabel_tot);

  //plots->addROCVariable("dphimetj12",                  "dphimetj12_tot_0_500",              selqcd + "&&" + selmet0to500,     	  selqcd + "&&" + selmet0to500,     kRed,      1,        200, 0., 3.2);
  //plots->addROCVariable("dphimetj123",                 "dphimetj123_tot_0_500",             selqcd + "&&" + selmet0to500,     	  selqcd + "&&" + selmet0to500,     kBlue,     1,        200, 0., 3.2);
  //plots->addROCVariable("dphimetj1234",                "dphimetj1234_tot_0_500",            selqcd + "&&" + selmet0to500,     	  selqcd + "&&" + selmet0to500,     kCyan,     1,        200, 0., 3.2);
  //plots->addROCVariable("dphimetj12_500_Inf",          "dphimetj12_tot_500_Inf",            selqcd + "&&" + selmet500to1000,      selqcd + "&&" + selmet500to1000,  kGreen,    1,        200, 0., 3.2);
  //plots->addROCVariable("dphimetj123_500_Inf",         "dphimetj123_tot_500_Inf",           selqcd + "&&" + selmet500to1000,      selqcd + "&&" + selmet500to1000,  kMagenta,  1,        200, 0., 3.2);
  //plots->addROCVariable("dphimetj1234_500_Inf",        "dphimetj1234_tot_500_Inf",          selqcd + "&&" + selmet500to1000,      selqcd + "&&" + selmet500to1000,  kViolet,  1,        200, 0., 3.2);
  //plots->addROCVariable("dphistarmetj12_0_500",        "dphistarmetj12_tot_0_500",          selqcd + "&&" + selmet0to500,         selqcd + "&&" + selmet0to500,     kRed,      2,        200, 0., 3.2);
  //plots->addROCVariable("dphistarmetj123_0_500",       "dphistarmetj123_tot_0_500",         selqcd + "&&" + selmet0to500,         selqcd + "&&" + selmet0to500,     kBlue,     2,        200, 0., 3.2);
  //plots->addROCVariable("dphistarmetj1234_0_500",      "dphistarmetj1234_tot_0_500",        selqcd + "&&" + selmet0to500,         selqcd + "&&" + selmet0to500,     kCyan,     2,        200, 0., 3.2);
  //plots->addROCVariable("dphistarmetj12_500_1000",     "dphistarmetj12_tot_500_Inf",        selqcd + "&&" + selmet500to1000,      selqcd + "&&" + selmet500to1000,  kGreen,    2,        200, 0., 3.2);
  //plots->addROCVariable("dphistarmetj123_500_1000",    "dphistarmetj123_tot_500_Inf",       selqcd + "&&" + selmet500to1000,      selqcd + "&&" + selmet500to1000,  kMagenta,  2,        200, 0., 3.2);
  //plots->addROCVariable("dphistarmetj1234_500_Inf",    "dphistarmetj1234_tot_500_Inf",      selqcd + "&&" + selmet500to1000,      selqcd + "&&" + selmet500to1000,  kViolet,  2,        200, 0., 3.2);

  plots_qcd->addSignalTree    (sigtree_400, treename, siglabel_400); 
  //plots_qcd->addSignalTree    (sigtree_600, treename, siglabel_600); 
  //plots_qcd->addSignalTree    (sigtree_800, treename, siglabel_800); 
  plots_qcd->addBackgroundTree(bkgtree_qcd, treename, bkglabel_qcd);

  plots_qcd->addROCVariable("dphimetj12",               "dphimetj12",              selqcd,         selqcd,     kRed,      1,        200, 0., 3.2);
  plots_qcd->addROCVariable("dphimetj123",              "dphimetj123",             selqcd,         selqcd,     kBlue,     1,        200, 0., 3.2);
  plots_qcd->addROCVariable("dphimetj1234",             "dphimetj1234",            selqcd,         selqcd,     kCyan,    1,        200, 0., 3.2);
  plots_qcd->addROCVariable("dphistarmetj12",           "dphistarmetj12",          selqcd,         selqcd,     kRed,      2,        200, 0., 3.2);
  plots_qcd->addROCVariable("dphistarmetj123",          "dphistarmetj123",         selqcd,         selqcd,     kBlue,     2,        200, 0., 3.2);
  plots_qcd->addROCVariable("dphistarmetj1234",         "dphistarmetj1234",        selqcd,         selqcd,     kCyan,    2,        200, 0., 3.2);

  //plots->    addCompPlot(outputname+"_qcdVariables_1_t2tt_400_313_tot",      compmvas_training);
  plots_qcd->addCompPlot(outputname+"_qcdVariables_2_t2tt_400_313",      compmvas_training_2);
  
  //plots->    getCompPlot(outputname+"_qcdVariables_1_t2tt_400_313_tot")->setLegend(0.45, 0.4, 0.9, 0.55);
  plots_qcd->getCompPlot(outputname+"_qcdVariables_2_t2tt_400_313")->setLegend(0.45, 0.4, 0.9, 0.55);

  //float sigeff_dphi12 =     EffPlotTools::getEfficiencyForCutValue(plots->getSigHist("dphimetj12"),     0.5, 0)[0];
  //float bkgeff_dphi12 =     EffPlotTools::getEfficiencyForCutValue(plots->getBkgHist("dphimetj12"),     0.5, 0)[0];
  //float sigeff_dphi123 =     EffPlotTools::getEfficiencyForCutValue(plots->getSigHist("dphimetj123"),     0.5, 0)[0];
  //float bkgeff_dphi123 =     EffPlotTools::getEfficiencyForCutValue(plots->getBkgHist("dphimetj123"),     0.5, 0)[0];
  //plots->    getCompPlot(outputname+"_qcdVariables_1_t2tt_400_313_tot")->addLine(0.0, sigeff_dphi12, bkgeff_dphi12, sigeff_dphi12, 1, 2, "#Delta#phij12 < 0.5");
  //plots->    getCompPlot(outputname+"_qcdVariables_1_t2tt_400_313_tot")->addLine(bkgeff_dphi12, 0.0, bkgeff_dphi12, sigeff_dphi12, 1, 2);
  //plots->    getCompPlot(outputname+"_qcdVariables_1_t2tt_400_313_tot")->addLine(0.0, sigeff_dphi123, bkgeff_dphi123, sigeff_dphi123, 1, 6, "#Delta#phij123 < 0.5");
  //plots->    getCompPlot(outputname+"_qcdVariables_1_t2tt_400_313_tot")->addLine(bkgeff_dphi123, 0.0, bkgeff_dphi123, sigeff_dphi123, 1, 6);
  //float sigeff_500_dphi12 =     EffPlotTools::getEfficiencyForCutValue(plots->getSigHist("dphimetj12_500_Inf"),     0.5, 0)[0];
  //float bkgeff_500_dphi12 =     EffPlotTools::getEfficiencyForCutValue(plots->getBkgHist("dphimetj12_500_Inf"),     0.5, 0)[0];
  //float sigeff_500_dphi123 =     EffPlotTools::getEfficiencyForCutValue(plots->getSigHist("dphimetj123_500_Inf"),     0.5, 0)[0];
  //float bkgeff_500_dphi123 =     EffPlotTools::getEfficiencyForCutValue(plots->getBkgHist("dphimetj123_500_Inf"),     0.5, 0)[0];
  //plots->    getCompPlot(outputname+"_qcdVariables_1_t2tt_400_313_tot")->addLine(0.0, sigeff_500_dphi12, bkgeff_500_dphi12, sigeff_500_dphi12, 1, 4, "#Delta#phij12 < 0.5");
  //plots->    getCompPlot(outputname+"_qcdVariables_1_t2tt_400_313_tot")->addLine(bkgeff_500_dphi12, 0.0, bkgeff_500_dphi12, sigeff_500_dphi12, 1, 4);
  //plots->    getCompPlot(outputname+"_qcdVariables_1_t2tt_400_313_tot")->addLine(0.0, sigeff_500_dphi123, bkgeff_500_dphi123, sigeff_500_dphi123, 1, 8, "#Delta#phij123 < 0.5");
  //plots->    getCompPlot(outputname+"_qcdVariables_1_t2tt_400_313_tot")->addLine(bkgeff_500_dphi123, 0.0, bkgeff_500_dphi123, sigeff_500_dphi123, 1, 8);
  //plots->plotAll();

  float sigeff_dphi12_qcd =     EffPlotTools::getEfficiencyForCutValue(plots_qcd->getSigHist("dphimetj12"),     0.5, 0)[0];
  float bkgeff_dphi12_qcd =     EffPlotTools::getEfficiencyForCutValue(plots_qcd->getBkgHist("dphimetj12"),     0.5, 0)[0];
  float sigeff_dphi123_qcd =    EffPlotTools::getEfficiencyForCutValue(plots_qcd->getSigHist("dphimetj123"),     0.5, 0)[0];
  float bkgeff_dphi123_qcd =    EffPlotTools::getEfficiencyForCutValue(plots_qcd->getBkgHist("dphimetj123"),     0.5, 0)[0];
  float sigeff_dphi1234_qcd =   EffPlotTools::getEfficiencyForCutValue(plots_qcd->getSigHist("dphimetj1234"),     0.5, 0)[0];
  float bkgeff_dphi1234_qcd =   EffPlotTools::getEfficiencyForCutValue(plots_qcd->getBkgHist("dphimetj1234"),     0.5, 0)[0];
  plots_qcd->getCompPlot(outputname+"_qcdVariables_2_t2tt_400_313")->addLine(0.0, sigeff_dphi12_qcd, bkgeff_dphi12_qcd, sigeff_dphi12_qcd, 1, 2, "#Delta#phij12 < 0.5");
  plots_qcd->getCompPlot(outputname+"_qcdVariables_2_t2tt_400_313")->addLine(bkgeff_dphi12_qcd, 0.0, bkgeff_dphi12_qcd, sigeff_dphi12_qcd, 1, 2);
  plots_qcd->getCompPlot(outputname+"_qcdVariables_2_t2tt_400_313")->addLine(0.0, sigeff_dphi123_qcd, bkgeff_dphi123_qcd, sigeff_dphi123_qcd, 1, 6, "#Delta#phij123 < 0.5");
  plots_qcd->getCompPlot(outputname+"_qcdVariables_2_t2tt_400_313")->addLine(bkgeff_dphi123_qcd, 0.0, bkgeff_dphi123_qcd, sigeff_dphi123_qcd, 1, 6);
  plots_qcd->getCompPlot(outputname+"_qcdVariables_2_t2tt_400_313")->addLine(0.0, sigeff_dphi1234_qcd, bkgeff_dphi1234_qcd, sigeff_dphi1234_qcd, 1, 6, "#Delta#phij1234 < 0.5");
  plots_qcd->getCompPlot(outputname+"_qcdVariables_2_t2tt_400_313")->addLine(bkgeff_dphi1234_qcd, 0.0, bkgeff_dphi1234_qcd, sigeff_dphi1234_qcd, 1, 6);
  //float sigeff_500_dphi12_qcd =     EffPlotTools::getEfficiencyForCutValue(plots_qcd->getSigHist("dphimetj12_qcd_500_Inf"),     0.5, 0)[0];
  //float bkgeff_500_dphi12_qcd =     EffPlotTools::getEfficiencyForCutValue(plots_qcd->getBkgHist("dphimetj12_qcd_500_Inf"),     0.5, 0)[0];
  //float sigeff_500_dphi123_qcd =    EffPlotTools::getEfficiencyForCutValue(plots_qcd->getSigHist("dphimetj123_qcd_500_Inf"),     0.5, 0)[0];
  //float bkgeff_500_dphi123_qcd =    EffPlotTools::getEfficiencyForCutValue(plots_qcd->getBkgHist("dphimetj123_qcd_500_Inf"),     0.5, 0)[0];
  //float sigeff_500_dphi1234_qcd =   EffPlotTools::getEfficiencyForCutValue(plots_qcd->getSigHist("dphimetj1234_qcd_500_Inf"),     0.5, 0)[0];
  //float bkgeff_500_dphi1234_qcd =   EffPlotTools::getEfficiencyForCutValue(plots_qcd->getBkgHist("dphimetj1234_qcd_500_Inf"),     0.5, 0)[0];
  //plots_qcd->getCompPlot(outputname+"_qcdVariables_2_t2tt_800_100")->addLine(0.0, sigeff_500_dphi12_qcd, bkgeff_500_dphi12_qcd, sigeff_500_dphi12_qcd, 1, 4, "#Delta#phij12 < 0.5");
  //plots_qcd->getCompPlot(outputname+"_qcdVariables_2_t2tt_800_100")->addLine(bkgeff_500_dphi12_qcd, 0.0, bkgeff_500_dphi12_qcd, sigeff_500_dphi12_qcd, 1, 4);
  //plots_qcd->getCompPlot(outputname+"_qcdVariables_2_t2tt_800_100")->addLine(0.0, sigeff_500_dphi123_qcd, bkgeff_500_dphi123_qcd, sigeff_500_dphi123_qcd, 1, 8, "#Delta#phij123 < 0.5");
  //plots_qcd->getCompPlot(outputname+"_qcdVariables_2_t2tt_800_100")->addLine(bkgeff_500_dphi123_qcd, 0.0, bkgeff_500_dphi123_qcd, sigeff_500_dphi123_qcd, 1, 8);
  //plots_qcd->getCompPlot(outputname+"_qcdVariables_2_t2tt_800_100")->addLine(0.0, sigeff_500_dphi1234_qcd, bkgeff_500_dphi1234_qcd, sigeff_500_dphi1234_qcd, 1, 8, "#Delta#phij1234 < 0.5");
  //plots_qcd->getCompPlot(outputname+"_qcdVariables_2_t2tt_800_100")->addLine(bkgeff_500_dphi1234_qcd, 0.0, bkgeff_500_dphi1234_qcd, sigeff_500_dphi1234_qcd, 1, 8);
  plots_qcd->plotAll();

  plots_qcd->getInfoForBackgroundEff("dphistarmetj12",        bkgeff_dphi12_qcd);
  plots_qcd->getInfoForBackgroundEff("dphistarmetj123",       bkgeff_dphi123_qcd);
  plots_qcd->getInfoForBackgroundEff("dphistarmetj1234",      bkgeff_dphi1234_qcd);
  plots_qcd->getInfoForBackgroundEff("dphimetj12", 	      bkgeff_dphi12_qcd);
  plots_qcd->getInfoForBackgroundEff("dphimetj123",           bkgeff_dphi123_qcd);
  plots_qcd->getInfoForBackgroundEff("dphimetj1234",          bkgeff_dphi1234_qcd);  

  plots_qcd->getInfoForBackgroundEff("dphistarmetj12",        0.5 + bkgeff_dphi12_qcd);
  plots_qcd->getInfoForBackgroundEff("dphistarmetj123",       0.5 + bkgeff_dphi123_qcd);
  plots_qcd->getInfoForBackgroundEff("dphistarmetj1234",      0.5 + bkgeff_dphi1234_qcd);
  
 
  //plots->getInfoForCut("dphimetj12", 0.5);
  //plots->getInfoForCut("dphimetj123", 0.5);
  //plots_qcd->getInfoForCut("dphimetj12", 0.5);
  //plots_qcd->getInfoForCut("dphimetj123", 0.5);
//}
  //plots->getInfoForSignalEff("dphimetj12", 0.7);
  //plots->getInfoForSignalEff("dphistarmetj12", 0.7);
  //plots->getInfoForSignalEff("dphimetj123", 0.7);
  //plots->getInfoForSignalEff("dphistarmetj123", 0.7);
}
