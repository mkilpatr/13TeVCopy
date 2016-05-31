#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisTools/Utilities/interface/JetFlavorInfo.h"
#include "AnalysisMethods/PlotUtils/interface/HistogramGetter.hh"
#include "AnalysisTools/TreeReader/interface/Defaults.h"
#include "QCDPredictionHelper.h"
#include "TDirectory.h"
#include "TFile.h"

using namespace std;

TString outputDir = "qcdSampleTestPlots";
QCDSupport::SRegInfo srinfo;

const TString weights = QCDSupport::stdMCWeight;

void drawPlot(Plot* p_Plot, float yMin = 0., float yMax = 1.);
void drawRatiosPlot(Plot* p_Plot, float yMin = 0, float yMax = 1.);
void printSmearComparison(vector <pair <TTree*, TString> >& p_SampleTrees, TString p_Suffix, TString p_Sel, TString p_Desc, HistogramGetter* p_HistG, TString p_yTitle, bool p_flagForBrokenQCDTree, bool p_normalize);
void printSmearComparisonsByBin(vector <pair <TTree*, TString> >& p_SampleTrees, vector <pair <TString, pair <TString, TString> > >& p_BinInfo, vector<HistogramGetter*> p_HistGs, TString p_yTitle, bool p_flagForBrokenQCDTree = false, bool p_normalize = false);
void printSmearComparisonsBySignalRegion(vector <pair <TTree*, TString> >& p_SampleTrees, vector<HistogramGetter*> p_HistGs, TString p_yTitle, bool p_flagForBrokenQCDTree = false, bool p_normalize = false);

#endif

void QCDSmearedSkimValidationPlots(){
  StyleTools::SetStyle();
  gStyle->SetTitleOffset(1.400, "Y");
  gStyle->SetTitleOffset(0.950, "X");

  vector <pair <TTree*, TString> > sampleTrees;
  sampleTrees.emplace_back(QCDSupport::getTree(QCDSupport::inputDir + "/qcd_puWeight_tree_skimmed.root"),        "Smeared (puWeight)");
  sampleTrees.emplace_back(QCDSupport::getTree(QCDSupport::inputDir + "/qcd_puWeight_weight_tree_skimmed.root"), "Smeared (puWeight*weight)");
  sampleTrees.emplace_back(QCDSupport::getTree(QCDSupport::inputDir + "/qcd_orig_tree_skimmed.root"),            "unSmeared (original)");
  sampleTrees.emplace_back(QCDSupport::getTree(QCDSupport::inputDir + "/qcd_orig_combined_tree_skimmed.root"),   "unSmeared (combined)");
  vector <pair <TTree*, TString> > sampleTrees_noSkim;
  sampleTrees_noSkim.emplace_back(QCDSupport::getTree(QCDSupport::inputDir + "/qcd_orig_tree.root"),            "unSmeared (original)");
  sampleTrees_noSkim.emplace_back(QCDSupport::getTree(QCDSupport::inputDir + "/qcd_orig_plus_tree.root"),       "unSmeared (new)");
  sampleTrees_noSkim.emplace_back(QCDSupport::getTree(QCDSupport::inputDir + "/qcd_orig_combined_tree_1.root"), "unSmeared (combined)");
  gSystem->mkdir(outputDir, true);

  vector <pair <TString, pair <TString, TString> > > null_binning = { make_pair("",   make_pair("event",            "")) };

  vector <pair <TString, pair <TString, TString> > > dphi_binning = { make_pair("CR", make_pair(QCDSupport::CR_def, "Control Region")), 
                                                                      make_pair("SR", make_pair(QCDSupport::SR_def, "Signal Region")) };

  vector<HistogramGetter*> histGetters_noSkim;
  histGetters_noSkim.push_back(new HistogramGetter("ht",  "ht",  "h_{T}",                    60, 0., 3000.));
  histGetters_noSkim.push_back(new HistogramGetter("met", "met", "#slash{#it{E}}_{T} [GeV]", 50, 0., 1000.));
  vector<HistogramGetter*> histGetters;
  histGetters.push_back(new HistogramGetter("ht",  "ht",  "h_{T}",                    60, 0.,   3000.));
  histGetters.push_back(new HistogramGetter("met", "met", "#slash{#it{E}}_{T} [GeV]", 40, 200., 1000.));

  printSmearComparisonsByBin(sampleTrees_noSkim, null_binning, histGetters_noSkim, "Events", true);
  printSmearComparisonsByBin(sampleTrees_noSkim, dphi_binning, histGetters_noSkim, "Events", true);
  printSmearComparisonsBySignalRegion(sampleTrees, histGetters, "Events");
}

void drawPlot(Plot* p_Plot, float yMin, float yMax){
  TCanvas* canvas = new TCanvas;
  p_Plot->setLegend(.45, .65, .92, .9);
  p_Plot->setYRange(yMin, 1.3 * yMax);
  p_Plot->setHeader(QCDSupport::header, "");
  p_Plot->draw(canvas);
  QCDSupport::setTitleOffset(canvas);
  canvas->SaveAs(outputDir + "/" + p_Plot->getName() + TString(".") + QCDSupport::format);
}

void drawRatiosPlot(Plot* p_Plot, float yMin, float yMax){
  TCanvas* canvas = new TCanvas;
  p_Plot->setLegend(.45, .65, .92, .9);
  p_Plot->setYRange(yMin, 1.3 * yMax);
  p_Plot->setHeader(QCDSupport::header, "");
  p_Plot->drawRatios(canvas);
  QCDSupport::setTitleOffset(canvas);
  canvas->SaveAs(outputDir + "/" + p_Plot->getName() + TString(".") + QCDSupport::format);
}

void printSmearComparison(vector <pair <TTree*, TString> >& p_SampleTrees, TString p_Suffix, TString p_Sel, TString p_Desc, HistogramGetter* p_HistG, TString p_yTitle, bool p_flagForBrokenQCDTree, bool p_normalize){
  float yMax = 0.;
  if(p_normalize) p_yTitle = "Arbitrary units";
  Plot* plot = new Plot(p_HistG->plotInfo->name + "_" + p_Suffix + (p_normalize ? "_norm" : "_noNorm"), p_Desc, p_HistG->plotInfo->xTitle, p_yTitle);
cout << p_Sel << endl;
  for(int iS = 0; iS < p_SampleTrees.size(); iS++){
    TH1F* hist = p_HistG->getHistogram(p_SampleTrees[iS].first, p_Sel, QCDSupport::stdQCDWeight, "QCD");
    if(p_flagForBrokenQCDTree){
      TTree* temp_QCDTree = QCDSupport::getTree(QCDSupport::inputDir + "/qcd_orig_combined_tree_2.root");
      TH1F* temp_hist = p_HistG->getHistogram(temp_QCDTree, p_Sel, QCDSupport::stdQCDWeight, "QCD_2");
      hist->Add(temp_hist);
    }
    if(p_normalize) PlotTools::normalize(hist);
    if(yMax < hist->GetMaximum()) yMax = hist->GetMaximum();
    plot->addHist(hist, p_SampleTrees[iS].second, "E1", StyleTools::colorGetter(iS), 0, StyleTools::colorGetter(iS));
  }
//  drawPlot(plot, 0., yMax);
  drawRatiosPlot(plot, 0., yMax);
}

void printSmearComparisonsByBin(vector <pair <TTree*, TString> >& p_SampleTrees, vector <pair <TString, pair <TString, TString> > >& p_BinInfo, vector<HistogramGetter*> p_HistGs, TString p_yTitle, bool p_flagForBrokenQCDTree, bool p_normalize){
  for(int iHG = 0; iHG < p_HistGs.size(); iHG++){
    for(int iB = 0; iB < p_BinInfo.size(); iB++){
      printSmearComparison(p_SampleTrees, p_BinInfo[iB].first, p_BinInfo[iB].second.first, p_BinInfo[iB].second.second, p_HistGs[iHG], p_yTitle, p_flagForBrokenQCDTree, p_normalize);
    }
  }
}

void printSmearComparisonsBySignalRegion(vector <pair <TTree*, TString> >& p_SampleTrees, vector<HistogramGetter*> p_HistGs, TString p_yTitle, bool p_flagForBrokenQCDTree, bool p_normalize){
  for(int iHG = 0; iHG < p_HistGs.size(); iHG++){
    for(unsigned int iSR = 0; iSR < srinfo.nSR; ++iSR){
      TString sel_SR = TString::Format("%s && %s", srinfo.srPreQCD.Data(), srinfo.srSel[iSR].Data());
      printSmearComparison(p_SampleTrees, srinfo.srRegBinNames[iSR], sel_SR, srinfo.srSelNames[iSR], p_HistGs[iHG], p_yTitle, p_flagForBrokenQCDTree, p_normalize);
    }
  }
}
