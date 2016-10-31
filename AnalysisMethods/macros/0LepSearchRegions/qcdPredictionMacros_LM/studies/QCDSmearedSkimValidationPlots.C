#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisTools/Utilities/interface/JetFlavorInfo.h"
#include "AnalysisMethods/PlotUtils/interface/HistogramGetter.hh"
#include "AnalysisTools/TreeReader/interface/Defaults.h"
#include "QCDPredictionHelper.h"
#include "TDirectory.h"
#include "TFile.h"

using namespace std;

const TString originalTreeDir = "/eos/uscms//store/user/jzabel/13TeV/T2FBD_Trees";
const TString outputDir = "qcdSampleTestPlots";
QCDSupport::SRegInfo srinfo;

const TString weights = QCDSupport::stdMCWeight;

float getMaximum(TH1F* p_Hist);
void drawPlot(Plot* p_Plot, float yMin = 0., float yMax = 1., bool p_drawRatio = false);
void printSmearComparison(vector <pair <TTree*, TString> >& p_SampleTrees, TString p_Suffix, TString p_Sel, TString p_Desc, HistogramGetter* p_HistG, TString p_yTitle, bool p_drawRatio = false, bool p_logY = false, bool p_normalize = false);
void printSmearComparisonsByBin(vector <pair <TTree*, TString> >& p_SampleTrees, vector <pair <TString, pair <TString, TString> > >& p_BinInfo, vector<HistogramGetter*> p_HistGs, TString p_Sel, TString p_yTitle, bool p_drawRatio = false, bool p_logY = false, bool p_normalize = false);
void printSmearComparisonsBySignalRegion(vector <pair <TTree*, TString> >& p_SampleTrees, vector<HistogramGetter*> p_HistGs, TString p_yTitle, bool p_drawRatio = false, bool p_logY = false, bool p_normalize = false);
void printSmearComparisonsByBinVar(vector <pair <TTree*, TString> >& p_SampleTrees, TString p_Var, pair <TString, TString> p_HistGInfo, float p_BinSize, pair <TString, TString> p_BinInfo, vector <float>& p_Bins, TString p_Sel, TString p_yTitle, bool p_drawRatio = false, bool p_logY = false, bool p_normalize = false);
void printSmearComparisonsByBinVar(vector <pair <TTree*, TString> >& p_SampleTrees, HistogramGetter* p_HistG, TString p_suffix, pair <TString, TString> p_BinInfo, vector <float>& p_Bins, TString p_Sel, TString p_yTitle, bool p_drawRatio = false, bool p_logY = false, bool p_normalize = false);
void printSmearComparisonsByBinVar(vector <pair <TTree*, TString> >& p_SampleTrees, HistogramGetter* p_HistG, TString p_suffix, vector <pair <TString, pair <TString, TString> > >& p_BinInfo, TString p_Sel, TString p_yTitle, bool p_drawRatio = false, bool p_logY = false, bool p_normalize = false);

#endif

void QCDSmearedSkimValidationPlots(){
  StyleTools::SetStyle();
  gStyle->SetTitleOffset(1.400, "Y");
  gStyle->SetTitleOffset(0.950, "X");

  vector <pair <TTree*, TString> > sampleTrees;
  sampleTrees.emplace_back(QCDSupport::getTree(QCDSupport::inputDir + "/qcd_puWeight_combined_tree_skimmed_baseline.root"),        "Smeared (puWeight)");
  sampleTrees.emplace_back(QCDSupport::getTree(QCDSupport::inputDir + "/qcd_puWeight_weight_combined_tree_skimmed_baseline.root"), "Smeared (puWeight*weight)");
  sampleTrees.emplace_back(QCDSupport::getTree(QCDSupport::inputDir + "/qcd_orig_tree_skimmed_baseline.root"),                     "unSmeared (original)");
  sampleTrees.emplace_back(QCDSupport::getTree(QCDSupport::inputDir + "/qcd_orig_plus_tree_skimmed_baseline.root"),                "unSmeared (new)");
  vector <pair <TTree*, TString> > sampleTrees_noSkim;
  sampleTrees_noSkim.emplace_back(QCDSupport::getTree(originalTreeDir + "/qcd_orig_tree.root"),          "original");
  sampleTrees_noSkim.emplace_back(QCDSupport::getTree(originalTreeDir + "/qcd_orig_plus_tree.root"),     "new");
  sampleTrees_noSkim.emplace_back(QCDSupport::getTree(originalTreeDir + "/qcd_orig_combined_tree.root"), "combined");
  gSystem->mkdir(outputDir, true);

  vector <pair <TString, pair <TString, TString> > > null_binning = { make_pair("INC",          make_pair("met",                     "")) };
  vector <pair <TString, pair <TString, TString> > > dphi_binning = { make_pair("CR",           make_pair(QCDSupport::CR_def,        "Control Region")), 
                                                                      make_pair("SR",           make_pair(QCDSupport::SR_def,        "Signal Region")) };
  vector <pair <TString, pair <TString, TString> > >   ht_binning = { make_pair("ht200to300",   make_pair("whichQCD_HT_Sample == 1", "QCD Sample 200 #leq h_{T} < 300")), 
                                                                      make_pair("ht300to500",   make_pair("whichQCD_HT_Sample == 2", "QCD Sample 300 #leq h_{T} < 500")), 
                                                                      make_pair("ht500to700",   make_pair("whichQCD_HT_Sample == 3", "QCD Sample 500 #leq h_{T} < 700")), 
                                                                      make_pair("ht700to1000",  make_pair("whichQCD_HT_Sample == 4", "QCD Sample 700 #leq h_{T} < 1000")), 
                                                                      make_pair("ht1000to1500", make_pair("whichQCD_HT_Sample == 5", "QCD Sample 1000 #leq h_{T} < 1500")), 
                                                                      make_pair("ht1500to2000", make_pair("whichQCD_HT_Sample == 6", "QCD Sample 1500 #leq h_{T} < 2000")), 
                                                                      make_pair("ht2000toinf",  make_pair("whichQCD_HT_Sample == 7", "QCD Sample 2000 #leq h_{T}")) };
  vector <float> ht_Bins  = { 200, 300, 500, 700, 1000, 1500, 2000, 3000 };
  vector <float> met_Bins = {   0, 250, 350, 450,  550,  650,  750, 1000 };
  pair <TString, TString> ht_Info  = make_pair("ht", "h_{T}");
  pair <TString, TString> met_Info = make_pair("met", "#slash{#it{E}}_{T}");

  const int nHT_bins = 8;
  double ht_bins[nHT_bins] = { 0., 250, 500, 750, 1000, 1500., 2000, 3000 };
  vector<HistogramGetter*> histGetters_noSkim;
  histGetters_noSkim.push_back(new HistogramGetter("ht",  "ht",  "h_{T}",                    nHT_bins - 1, ht_bins));
//  histGetters_noSkim.push_back(new HistogramGetter("ht",  "ht",  "h_{T}",                    60, 0., 3000.));
  histGetters_noSkim.push_back(new HistogramGetter("met", "met", "#slash{#it{E}}_{T} [GeV]", 50, 0., 1000.));
  vector<HistogramGetter*> histGetters;
  histGetters.push_back(new HistogramGetter("ht",  "ht",  "h_{T}",                    nHT_bins - 1, ht_bins));
//  histGetters.push_back(new HistogramGetter("ht",  "ht",  "h_{T}",                    60, 0.,   3000.));
  histGetters.push_back(new HistogramGetter("met", "met", "#slash{#it{E}}_{T} [GeV]", 40, 200., 1000.));

//  printSmearComparisonsByBin(sampleTrees_noSkim, null_binning, histGetters_noSkim, "", "Events", true, true);
//  printSmearComparisonsByBin(sampleTrees_noSkim, dphi_binning, histGetters_noSkim, "", "Events", true, true);
//  printSmearComparisonsBySignalRegion(sampleTrees, histGetters, "Events", true);
//  printSmearComparisonsByBinVar(sampleTrees_noSkim, "ht",  ht_Info,  5., ht_Info,  ht_Bins,  "", "Events", true);
//  printSmearComparisonsByBinVar(sampleTrees_noSkim, "met", met_Info, 5., met_Info, met_Bins, "", "Events", true);

//  printSmearComparisonsByBinVar(sampleTrees_noSkim, histGetters[1], "", ht_Info, ht_Bins, "", "Events", true);
//  printSmearComparisonsByBinVar(sampleTrees,        srinfo.SRmetGetters[srinfo.SRReg::SR_nb1_med_boost_low_pTb], "_SR_5_",  ht_Info, ht_Bins, TString::Format("%s && %s", srinfo.srPreQCD.Data(), srinfo.srSel[srinfo.SRReg::SR_nb1_med_boost_low_pTb].Data()), "Events", true);
  printSmearComparisonsByBinVar(sampleTrees_noSkim, histGetters[1],                                              "",       ht_binning, "",                                                                                                               "Events", true);
//  printSmearComparisonsByBinVar(sampleTrees,        srinfo.SRmetGetters[srinfo.SRReg::SR_nb1_med_boost_low_pTb], "_SR_5", ht_binning, TString::Format("%s && %s", srinfo.srPreQCD.Data(), srinfo.srSel[srinfo.SRReg::SR_nb1_med_boost_low_pTb].Data()), "Events", true);
}

float getMinimum(TH1F* p_Hist){
  float min = p_Hist->GetMinimum(0);
  return min;
}

float getMaximum(TH1F* p_Hist){
  float max = p_Hist->GetMaximum();
  max = 0.;
  for(int iB = 2; iB < p_Hist->GetNbinsX(); iB++) if(max <= p_Hist->GetBinContent(iB)) max = p_Hist->GetBinContent(iB);
  return max;
}

void drawPlot(Plot* p_Plot, float yMin, float yMax, bool p_drawRatio){
  TCanvas* canvas = new TCanvas;
  p_Plot->setLegend(.45, .65, .92, .9);
  p_Plot->setYRange(yMin, yMax);
  p_Plot->setHeader(QCDSupport::header, "");
  if(p_drawRatio){
    p_Plot->drawRatios(canvas);
  } else {
    p_Plot->draw(canvas);
  }
  QCDSupport::setTitleOffset(canvas);
  canvas->SaveAs(outputDir + "/" + p_Plot->getName() + TString(".") + QCDSupport::format);
}

void printSmearComparison(vector <pair <TTree*, TString> >& p_SampleTrees, TString p_Suffix, TString p_Sel, TString p_Desc, HistogramGetter* p_HistG, TString p_yTitle, bool p_drawRatio, bool p_logY, bool p_normalize){
  float yMin = 99999., yMax = 0.;
  if(p_normalize) p_yTitle = "Arbitrary units";
  Plot* plot = new Plot(p_HistG->plotInfo->name + "_" + p_Suffix + (p_normalize ? "_norm" : "_noNorm"), p_Desc, p_HistG->plotInfo->xTitle, p_yTitle);
cout << p_Sel << endl;
  for(int iS = 0; iS < p_SampleTrees.size(); iS++){
    TH1F* hist = p_HistG->getHistogram(p_SampleTrees[iS].first, p_Sel, QCDSupport::stdQCDWeight, TString::Format("QCD_%s_%u", p_Suffix.Data(), iS));
    if(p_normalize) PlotTools::normalize(hist);
    if(yMin > getMinimum(hist)) yMin = getMinimum(hist);
    if(yMax < getMaximum(hist)) yMax = getMaximum(hist);
    plot->addHist(hist, p_SampleTrees[iS].second, "E1", StyleTools::colorGetter(iS), 0, StyleTools::colorGetter(iS));
  }
  if(p_logY){
    plot->setLogy();
    drawPlot(plot, 0.1 * yMin, 100 * yMax, p_drawRatio);
  } else {
    drawPlot(plot, 0.,         1.3 * yMax, p_drawRatio);
  }
}

void printSmearComparisonsByBin(vector <pair <TTree*, TString> >& p_SampleTrees, vector <pair <TString, pair <TString, TString> > >& p_BinInfo, vector<HistogramGetter*> p_HistGs, TString p_Sel, TString p_yTitle, bool p_drawRatio, bool p_logY, bool p_normalize){
  for(int iHG = 0; iHG < p_HistGs.size(); iHG++){
    for(int iB = 0; iB < p_BinInfo.size(); iB++){
      TString sel = "";
      if(p_Sel.Sizeof() > 1){
        sel = TString::Format("%s && %s", p_Sel.Data(), p_BinInfo[iB].second.first.Data());
      } else {
        sel = p_BinInfo[iB].second.first;
      }
      printSmearComparison(p_SampleTrees, p_BinInfo[iB].first, sel, p_BinInfo[iB].second.second, p_HistGs[iHG], p_yTitle, p_drawRatio, p_logY, p_normalize);
    }
  }
}

void printSmearComparisonsBySignalRegion(vector <pair <TTree*, TString> >& p_SampleTrees, vector<HistogramGetter*> p_HistGs, TString p_yTitle, bool p_drawRatio, bool p_logY, bool p_normalize){
  for(int iHG = 0; iHG < p_HistGs.size(); iHG++){
    for(unsigned int iSR = 0; iSR < srinfo.nSR; ++iSR){
      TString sel_SR = TString::Format("%s && %s", srinfo.srPreQCD.Data(), srinfo.srSel[iSR].Data());
      if(p_HistGs[iHG]->plotInfo->name == "met"){
        printSmearComparison(p_SampleTrees, srinfo.srRegBinNames[iSR], sel_SR, srinfo.srSelNames[iSR], srinfo.SRmetGetters[iSR], p_yTitle, p_drawRatio, p_logY, p_normalize);
      } else {
        printSmearComparison(p_SampleTrees, srinfo.srRegBinNames[iSR], sel_SR, srinfo.srSelNames[iSR], p_HistGs[iHG],            p_yTitle, p_drawRatio, p_logY, p_normalize);
      }
    }
  }
}

void printSmearComparisonsByBinVar(vector <pair <TTree*, TString> >& p_SampleTrees, TString p_Var, pair <TString, TString> p_HistGInfo, float p_BinSize, pair <TString, TString> p_BinInfo, vector <float>& p_Bins, TString p_Sel, TString p_yTitle, bool p_drawRatio, bool p_logY, bool p_normalize){
  for(int iB = 0; iB < p_Bins.size() - 1; iB++){
    int nBins = (p_Bins[iB + 1] - p_Bins[iB]) / p_BinSize;
    TString sel = "", name = "", desc = "";
    HistogramGetter* local_HG = new HistogramGetter(p_Var, p_HistGInfo.first, p_HistGInfo.second, nBins, p_Bins[iB], p_Bins[iB + 1]);
    if(iB == p_Bins.size() - 2){
      sel  = TString::Format("%.f <= %s", p_Bins[iB], p_BinInfo.first.Data());
      desc = TString::Format("%.f #leq %s", p_Bins[iB], p_BinInfo.second.Data());
      name = TString::Format("%s_gt_%.f", p_BinInfo.first.Data(), p_Bins[iB]);
    } else {
      sel  = TString::Format("%.f <= %s && %s < %.f", p_Bins[iB], p_BinInfo.first.Data(), p_BinInfo.first.Data(),  p_Bins[iB + 1]);
      desc = TString::Format("%.f #leq %s < %.f", p_Bins[iB], p_BinInfo.second.Data(), p_Bins[iB + 1]);
      name = TString::Format("%s_%.f_to_%.f", p_BinInfo.first.Data(), p_Bins[iB], p_Bins[iB + 1]);
    }
    if(p_Sel.Sizeof() > 1){
      sel += " && " + p_Sel;
    }
    printSmearComparison(p_SampleTrees, name, sel, desc, local_HG, p_yTitle, p_drawRatio, p_logY, p_normalize);
  }
}

void printSmearComparisonsByBinVar(vector <pair <TTree*, TString> >& p_SampleTrees, HistogramGetter* p_HistG, TString p_suffix, pair <TString, TString> p_BinInfo, vector <float>& p_Bins, TString p_Sel, TString p_yTitle, bool p_drawRatio, bool p_logY, bool p_normalize){
  for(int iB = 0; iB < p_Bins.size() - 1; iB++){
    TString sel = "", name = "", desc = "";
    if(iB == p_Bins.size() - 2){
      sel  = TString::Format("%.f <= %s", p_Bins[iB], p_BinInfo.first.Data());
      desc = TString::Format("%.f #leq %s", p_Bins[iB], p_BinInfo.second.Data());
      name = TString::Format("%s%s_gt_%.f", p_suffix.Data(), p_BinInfo.first.Data(), p_Bins[iB]);
    } else {
      sel  = TString::Format("%.f <= %s && %s < %.f", p_Bins[iB], p_BinInfo.first.Data(), p_BinInfo.first.Data(),  p_Bins[iB + 1]);
      desc = TString::Format("%.f #leq %s < %.f", p_Bins[iB], p_BinInfo.second.Data(), p_Bins[iB + 1]);
      name = TString::Format("%s%s_%.f_to_%.f", p_suffix.Data(), p_BinInfo.first.Data(), p_Bins[iB], p_Bins[iB + 1]);
    }
    if(p_Sel.Sizeof() > 1){
      sel += " && " + p_Sel;
    }
    printSmearComparison(p_SampleTrees, name, sel, desc, p_HistG, p_yTitle, p_drawRatio, p_logY, p_normalize);
  }
}

void printSmearComparisonsByBinVar(vector <pair <TTree*, TString> >& p_SampleTrees, HistogramGetter* p_HistG, TString p_suffix, vector <pair <TString, pair <TString, TString> > >& p_BinInfo, TString p_Sel, TString p_yTitle, bool p_drawRatio, bool p_logY, bool p_normalize){
  for(int iB = 0; iB < p_BinInfo.size(); iB++){
    TString sel = "";
    if(p_Sel.Sizeof() > 1){
      sel = TString::Format("%s && %s", p_Sel.Data(), p_BinInfo[iB].second.first.Data());
    } else {
      sel = p_BinInfo[iB].second.first;
    }
    printSmearComparison(p_SampleTrees, p_BinInfo[iB].first + p_suffix, sel, p_BinInfo[iB].second.second, p_HistG, p_yTitle, p_drawRatio, p_logY, p_normalize);
  }
}
