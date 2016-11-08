#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisTools/Utilities/interface/JetFlavorInfo.h"
#include "AnalysisMethods/PlotUtils/interface/HistogramGetter.hh"
#include "AnalysisTools/TreeReader/interface/Defaults.h"
#include "../QCDPredictionHelper.h"
#include "AnalysisMethods/PlotUtils/interface/PlotROCCurves.h"
#include "TDirectory.h"
#include "TFile.h"

using namespace std;

const TString originalTreeDir = "../../../run/plots_16_09_28_Smear";
const TString outputDir = "validate_smearing_log_data_signal_LM";
QCDSupport::SRegInfo srinfo;
vector<int> colors;

const TString weights = QCDSupport::stdMCWeight;

float getMaximum(TH1F* p_Hist);
void drawPlot(Plot* p_Plot, float yMin, float yMax, bool p_drawRatio = false, bool p_drawStack = false);
void printSmearComparison(vector <pair <TTree*, TString> >& p_SampleTrees, TString p_Suffix, TString p_Sel, TString p_Desc, HistogramGetter* p_HistG, TString p_yTitle, bool p_drawRatio = false, bool p_logY = false, bool p_normalize = false);
void printSmearComparisonsByBin(vector <pair <TTree*, TString> >& p_SampleTrees, HistogramGetter* p_HistG, TString p_suffix, vector <pair <TString, pair <TString, TString> > >& p_BinInfo, TString p_Sel, TString p_yTitle, bool p_drawRatio = false, bool p_logY = false, bool p_normalize = false);
void printSmearComparisonsByBin(vector <pair <TTree*, TString> >& p_SampleTrees, HistogramGetter* p_HistG, TString p_suffix, vector <pair <TString, pair <TString, TString> > >& p_BinInfo_1, vector <pair <TString, pair <TString, TString> > >& p_BinInfo_2, TString p_Sel, TString p_yTitle, bool p_drawRatio = false, bool p_logY = false, bool p_normalize = false);
void printStack(vector <pair <TTree*, TString> >& p_StackTrees, TString p_Suffix, TString p_Sel, TString p_Desc, HistogramGetter* p_HistG, TString p_yTitle, bool p_drawRatio, bool p_logY, bool p_ROCCurve);
void printStack(vector <pair <TTree*, TString> >& p_StackTrees, HistogramGetter* p_HistG, TString p_suffix, vector <pair <TString, pair <TString, TString> > >& p_BinInfo_1, TString p_Sel, TString p_yTitle, bool p_drawRatio = false, bool p_logY = false, bool p_ROCCurve = false);
void printStack(vector <pair <TTree*, TString> >& p_StackTrees, HistogramGetter* p_HistG, TString p_suffix, vector <pair <TString, pair <TString, TString> > >& p_BinInfo_1, vector <pair <TString, pair <TString, TString> > >& p_BinInfo_2, TString p_Sel, TString p_yTitle, bool p_drawRatio = false, bool p_logY = false, bool p_ROCCurve = false);
void printStack(vector <pair <TTree*, TString> >& p_StackTrees, HistogramGetter* p_HistG, TString p_suffix, vector <pair <TString, pair <TString, TString> > >& p_BinInfo_1, vector <pair <TString, pair <TString, TString> > >& p_BinInfo_2, vector <pair <TString, pair <TString, TString> > >& p_BinInfo_3, TString p_Sel, TString p_yTitle, bool p_drawRatio = false, bool p_logY = false, bool p_ROCCurve = false);

#endif

void ValidateQCDSmearing(){
  StyleTools::SetStyle();
  gStyle->SetTitleOffset(1.400, "Y");
  gStyle->SetTitleOffset(0.950, "X");

  //vector <pair <TTree*, TString> > sampleTrees;
  //sampleTrees.emplace_back(QCDSupport::getTree(QCDSupport::inputDir + "/QCD_smeared_tree_skimmed.root"), "Smeared (puWeight*weight)");
  //sampleTrees.emplace_back(QCDSupport::getTree(QCDSupport::inputDir + "/TT.root"),    "TT Events");
  vector <pair <TTree*, TString> > stackTrees;
  stackTrees.emplace_back(QCDSupport::getTree(QCDSupport::inputDir + "/MET.root"),                                        "Data");              colors.push_back(1);
  stackTrees.emplace_back(QCDSupport::getTree(QCDSupport::inputDir + "/TT_WJets.root"),                                   "Non-QCD bkg");       colors.push_back(color_ttbar);
  stackTrees.emplace_back(QCDSupport::getTree(QCDSupport::inputDir + "/QCD_smeared_tree_skimmed.root"),                   "Smeared");           colors.push_back(color_qcd);
  stackTrees.emplace_back(QCDSupport::getTree(QCDSupport::inputDir + "/../plots_16_09_28_NoSmear/qcd_orig_tree.root"),    "With orig. QCD MC"); colors.push_back(color_tW);
  stackTrees.emplace_back(QCDSupport::getTree(QCDSupport::inputDir + "/T2tt_400_313_tree.root"),                          "T2tt{400,313}");     colors.push_back(kGreen + 2);
  //stackTrees.emplace_back(QCDSupport::getTree(QCDSupport::inputDir + "/T2tt_600_300_tree.root"),                          "T2tt{600,300}");     colors.push_back(kRed);
  //stackTrees.emplace_back(QCDSupport::getTree(QCDSupport::inputDir + "/T2tt_800_100_tree.root"),                          "T2tt{800,100}");     colors.push_back(kBlue);
  //stackTrees.emplace_back(QCDSupport::getTree(QCDSupport::inputDir + "/T2fbd_400_350_tree.root"),                         "T2fbd{400,350}");    colors.push_back(kMagenta);
  gSystem->mkdir(outputDir, true);

  QCDSupport::CRegInfo crinfo;

  vector <pair <TString, pair <TString, TString> > >   nb_binning = { make_pair("nb_eq_0", make_pair("nbjets == 0", "n_{b} = 0")), 
                                                                      make_pair("nb_eq_1", make_pair("nbjets == 1", "n_{b} = 1")), 
                                                                      make_pair("nb_ge_2", make_pair("nbjets >= 2", "n_{b} #geq 2")) };
  vector <pair <TString, pair <TString, TString> > > dphi_binning_test = {          make_pair("METPresel_&&_BaselineExtraCuts_signal_test",  make_pair("(dphij1met >= 0)", ""))};
  vector <pair <TString, pair <TString, TString> > > dphi_binning = {               make_pair("METPresel_&&_BaselineExtraCuts_signal",  make_pair("((dphij1met >= 0) && (dphij2met >= 0))", ""))};
  vector <pair <TString, pair <TString, TString> > > dphimhtj_binning = {           make_pair("METPresel_&&_BaselineExtraCuts_signal",  make_pair("((dphimhtj1 >= 0) && (dphimhtj2 >= 0))", ""))};
  vector <pair <TString, pair <TString, TString> > > dphiSmhtj_binning = {          make_pair("METPresel_&&_BaselineExtraCuts_signal",  make_pair("((dphistarmhtj1 >= 0) && (dphistarmhtj2 >= 0))", ""))};
  vector <pair <TString, pair <TString, TString> > > dphiSmhtj_T_binning = {        make_pair("METPresel_&&_BaselineExtraCuts_signal",  make_pair("((dphistarmhtj1_tilde >= 0) && (dphistarmhtj2_tilde >= 0))",""))};  
  vector <pair <TString, pair <TString, TString> > > omegamhtj_binning = {          make_pair("METPresel_&&_BaselineExtraCuts_signal",  make_pair("((omegamhtj1 >= 0) && (omegamhtj2 >= 0))", ""))};
    vector <pair <TString, pair <TString, TString> > > omegamhtj_T_binning = {      make_pair("METPresel_&&_BaselineExtraCuts_signal",  make_pair("((omegamhtj1_tilde >= 0) && (omegamhtj2_tilde >= 0))", ""))};
  vector <pair <TString, pair <TString, TString> > > chimhtj_binning = {            make_pair("METPresel_&&_BaselineExtraCuts_signal",  make_pair("((chimhtj1 >= 0) && (chimhtj2 >= 0))", ""))};
  vector <pair <TString, pair <TString, TString> > >  met_binning = { make_pair("met_250_to_400", make_pair("met >= 250 && met < 400", "250 #leq #slash{#it{E}}_{T} < 400")), 
                                                                      make_pair("met_ge_400",     make_pair("met >= 400",              "#slash{#it{E}}_{T} #geq 400")) };
  vector <pair <TString, pair <TString, TString> > >  mtb_binning = { make_pair("mtb_lt_175", make_pair("mtcsv12met < 175",  "m_{T}(b_{1,2},#slash{#it{E}}_{T} < 175")), 
                                                                      make_pair("mtb_ge_175", make_pair("mtcsv12met >= 175", "m_{T}(b_{1,2},#slash{#it{E}}_{T} #geq 175")) };
  vector <pair <TString, pair <TString, TString> > >  jet_binning = { make_pair("jets_5_to_6", make_pair("njets >= 5 && njets <= 6", "5 #leq N_{j} #leq 6")), 
                                                                      make_pair("jets_ge_7",   make_pair("njets >= 7",               "N_{j} #geq 7")) };
  vector <pair <TString, pair <TString, TString> > >   ht_binning = { make_pair("ht200to300",   make_pair("whichQCD_HT_Sample == 1", "QCD Sample 200 #leq h_{T} < 300")),
                                                                      make_pair("ht300to500",   make_pair("whichQCD_HT_Sample == 2", "QCD Sample 300 #leq h_{T} < 500")),
                                                                      make_pair("ht500to700",   make_pair("whichQCD_HT_Sample == 3", "QCD Sample 500 #leq h_{T} < 700")),
                                                                      make_pair("ht700to1000",  make_pair("whichQCD_HT_Sample == 4", "QCD Sample 700 #leq h_{T} < 1000")),
                                                                      make_pair("ht1000to1500", make_pair("whichQCD_HT_Sample == 5", "QCD Sample 1000 #leq h_{T} < 1500")),
                                                                      make_pair("ht1500to2000", make_pair("whichQCD_HT_Sample == 6", "QCD Sample 1500 #leq h_{T} < 2000")),
                                                                      make_pair("ht2000toinf",  make_pair("whichQCD_HT_Sample == 7", "QCD Sample 2000 #leq h_{T}")) };
  vector <pair <TString, pair <TString, TString> > > ht_v_binning = { make_pair("std", make_pair("whichQCD_Sample == 0", "Standard")),
                                                                      make_pair("ext", make_pair("whichQCD_Sample == 1", "Extended")),
                                                                      make_pair("gj5", make_pair("whichQCD_Sample == 2", "GenJets5")) };
  vector <pair <TString, pair <TString, TString> > > met2_binning = { make_pair("met_250_to_350", make_pair("met >= 250 && met < 350", "250 #leq #slash{#it{E}}_{T} < 350")), 
                                                                      make_pair("met_ge_350",     make_pair("met >= 350",              "#slash{#it{E}}_{T} #geq 350")) };
//  vector <float> met_Bins = {   0, 250, 350, 450,  550,  650,  750, 1000 };
//  pair <TString, TString> met_Info = make_pair("met", "#slash{#it{E}}_{T}");

  const int nHT_bins = 8;
  double ht_bins[nHT_bins] = { 0., 250, 500, 750, 1000, 1500., 2000, 3000 };
  HistogramGetter* met_shape     = new HistogramGetter("met",         "met",                                       "#slash{#it{E}}_{T} [GeV]",                       20, 200., 1000.);
  HistogramGetter* dphi12_shape_test  = new HistogramGetter(  "dphij12met",  "dphij1met",                            "min(#Delta#phi(j_{1}, #slash{#it{E}}_{T}))",   32,   0., 3.2);
  HistogramGetter* dphi12_shape  = new HistogramGetter(       "dphij12met",  "min(dphij1met, dphij2met)",                 "min(#Delta#phi(j_{1,2}, #slash{#it{E}}_{T}))",   32,   0., 3.2);
  HistogramGetter* dphi123_shape = new HistogramGetter(       "dphij123met", "min(dphij1met, min(dphij2met, dphij3met))", "min(#Delta#phi(j_{1,2,3}, #slash{#it{E}}_{T}))", 32,   0., 3.2);
  HistogramGetter* dphimht12_shape = new HistogramGetter(     "dphimhtj12","min(dphimhtj1, dphimhtj2)",		   "min(#Delta#phi(j_{1,2}, #slash(#it{E}}_{T}))",   32,   0., 3.2);
  HistogramGetter* dphimht123_shape = new HistogramGetter(    "dphimhtj123","min(dphimhtj1, min(dphimhtj2, dphimhtj3))","min(#Delta#phi(j_{1,2,3}, #slash{#it{E}}_{T}))", 32,   0., 3.2);
  HistogramGetter* dphimht1234_shape = new HistogramGetter(   "dphimhtj1234","min(min(dphimhtj1, dphimhtj2), min(dphimhtj3,dphimhtj4))", "min(#Delta#phi(j_{1,2,3,4}, #slash(#it{E}}_{T}))",   32,   0., 3.2);
  HistogramGetter* dphiSmht12_shape = new HistogramGetter(    "dphistarmhtj12","min(dphistarmhtj1, dphistarmhtj2)","min(#Delta#phi*(j_{1,2}, #slash(#it{E}}_{T}))",   32,   0., 3.2);
  HistogramGetter* dphiSmht123_shape = new HistogramGetter(   "dphistarmhtj123","min(dphistarmhtj1, min(dphistarmhtj2, dphistarmhtj3))","min(#Delta#phi*(j_{1,2,3}, #slash{#it{E}}_{T}))", 32,   0., 3.2);
  HistogramGetter* dphiSmht1234_shape = new HistogramGetter(  "dphistarmhtj1234","min(min(dphistarmhtj1, dphistarmhtj2), min(dphistarmhtj3,dphistarmhtj4))", "min(#Delta#phi*(j_{1,2,3,4}, #slash(#it{E}}_{T}))",   32,   0., 3.2);
  HistogramGetter* dphiSmht12_T_shape = new HistogramGetter(  "dphistarmhtj12_tilde","min(dphistarmhtj1_tilde, dphistarmhtj2_tilde)","min(#Delta#phi~*(j_{1,2}, #slash(#it{E}}_{T}))",   32,   0., 3.2);
  HistogramGetter* dphiSmht123_T_shape = new HistogramGetter( "dphistarmhtj123_tilde","min(dphistarmhtj1_tilde, min(dphistarmhtj2_tilde, dphistarmhtj3_tilde))","min(#Delta#phi~*(j_{1,2,3}, #slash{#it{E}}_{T}))", 32,   0., 3.2);
  HistogramGetter* dphiSmht1234_T_shape = new HistogramGetter("dphistarmhtj1234_tilde","min(min(dphistarmhtj1_tilde, dphistarmhtj2_tilde), min(dphistarmhtj3_tilde,dphistarmhtj4_tilde))", "min(#Delta#phi~*(j_{1,2,3,4}, #slash(#it{E}}_{T}))",   32,   0., 3.2);
  HistogramGetter* omegamht12_shape = new HistogramGetter(    "omegamhtj12","min(omegamhtj1, omegamhtj2)", "min(#omega(j_{1,2}, #slash(#it{E}}_{T}))", 16, 0., 1.6);
  HistogramGetter* omegamht123_shape = new HistogramGetter(   "omegamhtj123","min(omegamhtj1, min(omegamhtj2, omegamhtj3))","min(#omega(j_{1,2,3}, #slash{#it{E}}_{T}))", 16, 0., 1.6);
  HistogramGetter* omegamht1234_shape = new HistogramGetter(  "omegamhtj1234","min(min(omegamhtj1, omegamhtj2), min(omegamhtj3,omegamhtj4))", "min(#omega(j_{1,2,3,4}, #slash(#it{E}}_{T}))",   16,   0., 1.6);
  HistogramGetter* omegamht12_T_shape = new HistogramGetter(  "omegamhtj12_tilde","min(omegamhtj1_tilde, omegamhtj2_tilde)", "min(#omega~(j_{1,2}, #slash(#it{E}}_{T}))", 16, 0., 1.6);
  HistogramGetter* omegamht123_T_shape = new HistogramGetter( "omegamhtj123_tilde","min(omegamhtj1_tilde, min(omegamhtj2_tilde, omegamhtj3_tilde))","min(#omega~(j_{1,2,3}, #slash{#it{E}}_{T}))", 16,0., 1.6);
  HistogramGetter* omegamht1234_T_shape = new HistogramGetter("omegamhtj1234_tilde","min(min(omegamhtj1_tilde, omegamhtj2_tilde), min(omegamhtj3_tilde,omegamhtj4_tilde))", "min(#omega~(j_{1,2,3,4}, #slash(#it{E}}_{T}))",   16,   0., 1.6);
  HistogramGetter* chimht12_shape = new HistogramGetter(      "chimhtj12",  "min(chimhtj1, chimhtj2)",                 "min(#chi(j_{1,2}, #slash(#it{E}}_{T}))",   16,   0., 1.6);
  HistogramGetter* chimht123_shape = new HistogramGetter(     "chimhtj123","min(chimhtj1, min(chimhtj2, chimhtj3))",   "min(#chi(j_{1,2,3}, #slash{#it{E}}_{T}))", 16,   0., 1.6);
  HistogramGetter* chimht1234_shape = new HistogramGetter(    "chimhtj1234","min(min(chimhtj1, chimhtj2), min(chimhtj3, chimhtj4))", "min(#chi(j_{1,2,3,4}, #slash(#it{E}}_{T}))", 16, 0., 1.6);
  HistogramGetter* dphi12Z_shape  = new HistogramGetter(      "dphij12met",  "min(dphij1met, dphij2met)",                 "min(#Delta#phi(j_{1,2}, #slash{#it{E}}_{T}))",   20,   0., 1);
  HistogramGetter* dphi123Z_shape = new HistogramGetter(      "dphij123met", "min(dphij1met, min(dphij2met, dphij3met))", "min(#Delta#phi(j_{1,2,3}, #slash{#it{E}}_{T}))", 20,   0., 1);
  HistogramGetter* dphimht12Z_shape = new HistogramGetter(    "dphimhtj12","min(dphimhtj1, dphimhtj2)",		   "min(#Delta#phi(j_{1,2}, #slash(#it{E}}_{T}))",   20,   0., 1);
  HistogramGetter* dphimht123Z_shape = new HistogramGetter(   "dphimhtj123","min(dphimhtj1, min(dphimhtj2, dphimhtj3))","min(#Delta#phi(j_{1,2,3}, #slash{#it{E}}_{T}))", 20,   0., 1);
  HistogramGetter* dphimht1234Z_shape = new HistogramGetter(  "dphimhtj1234","min(min(dphimhtj1, dphimhtj2), min(dphimhtj3,dphimhtj4))", "min(#Delta#phi(j_{1,2,3,4}, #slash(#it{E}}_{T}))",   20,   0., 1);
  HistogramGetter* dphiSmht12Z_shape = new HistogramGetter(   "dphistarmhtj12","min(dphistarmhtj1, dphistarmhtj2)","min(#Delta#phi*(j_{1,2}, #slash(#it{E}}_{T}))",   20,   0., 1);
  HistogramGetter* dphiSmht123Z_shape = new HistogramGetter(  "dphistarmhtj123","min(dphistarmhtj1, min(dphistarmhtj2, dphistarmhtj3))","min(#Delta#phi*(j_{1,2,3}, #slash{#it{E}}_{T}))", 20,   0., 1);
  HistogramGetter* dphiSmht1234Z_shape = new HistogramGetter( "dphistarmhtj1234","min(min(dphistarmhtj1, dphistarmhtj2), min(dphistarmhtj3,dphistarmhtj4))", "min(#Delta#phi*(j_{1,2,3,4}, #slash(#it{E}}_{T}))",   20,   0., 1);
  HistogramGetter* dphiSmht12Z_T_shape = new HistogramGetter( "dphistarmhtj12_tilde","min(dphistarmhtj1_tilde, dphistarmhtj2_tilde)","min(#Delta#phi~*(j_{1,2}, #slash(#it{E}}_{T}))",   20,   0., 1);
  HistogramGetter* dphiSmht123Z_T_shape = new HistogramGetter("dphistarmhtj123_tilde","min(dphistarmhtj1_tilde, min(dphistarmhtj2_tilde, dphistarmhtj3_tilde))","min(#Delta#phi~*(j_{1,2,3}, #slash{#it{E}}_{T}))", 20,   0., 1);
  HistogramGetter* dphiSmht1234Z_T_shape = new HistogramGetter("dphistarmhtj1234_tilde","min(min(dphistarmhtj1_tilde, dphistarmhtj2_tilde), min(dphistarmhtj3_tilde,dphistarmhtj4_tilde))", "min(#Delta#phi~*(j_{1,2,3,4}, #slash(#it{E}}_{T}))",   20,   0., 1);
  HistogramGetter* omegamht12Z_shape = new HistogramGetter(   "omegamhtj12","min(omegamhtj1, omegamhtj2)", "min(#omega(j_{1,2}, #slash(#it{E}}_{T}))", 20, 0., 1);
  HistogramGetter* omegamht123Z_shape = new HistogramGetter(  "omegamhtj123","min(omegamhtj1, min(omegamhtj2, omegamhtj3))","min(#omega(j_{1,2,3}, #slash{#it{E}}_{T}))", 20,0., 1);
  HistogramGetter* omegamht1234Z_shape = new HistogramGetter( "omegamhtj1234","min(min(omegamhtj1, omegamhtj2), min(omegamhtj3,omegamhtj4))", "min(#omega(j_{1,2,3,4}, #slash(#it{E}}_{T}))",   20,   0., 1);
  HistogramGetter* omegamht12Z_T_shape = new HistogramGetter( "omegamhtj12_tilde","min(omegamhtj1_tilde, omegamhtj2_tilde)", "min(#omega~(j_{1,2}, #slash(#it{E}}_{T}))", 20, 0., 1);
  HistogramGetter* omegamht123Z_T_shape = new HistogramGetter("omegamhtj123_tilde","min(omegamhtj1_tilde, min(omegamhtj2_tilde, omegamhtj3_tilde))","min(#omega~(j_{1,2,3}, #slash{#it{E}}_{T}))", 20,0., 1);
  HistogramGetter* omegamht1234Z_T_shape = new HistogramGetter("omegamhtj1234_tilde","min(min(omegamhtj1_tilde, omegamhtj2_tilde), min(omegamhtj3_tilde,omegamhtj4_tilde))", "min(#omega~(j_{1,2,3,4}, #slash(#it{E}}_{T}))",   20,   0., 1);
  HistogramGetter* chimht12Z_shape = new HistogramGetter(     "chimhtj12",  "min(chimhtj1, chimhtj2)",                 "min(#chi(j_{1,2}, #slash(#it{E}}_{T}))",   20,   0., 1);
  HistogramGetter* chimht123Z_shape = new HistogramGetter(    "chimhtj123","min(chimhtj1, min(chimhtj2, chimhtj3))",   "min(#chi(j_{1,2,3}, #slash{#it{E}}_{T}))", 20,   0., 1);
  HistogramGetter* chimht1234Z_shape = new HistogramGetter(   "chimhtj1234","min(min(chimhtj1, chimhtj2), min(chimhtj3, chimhtj4))", "min(#chi(j_{1,2,3,4}, #slash(#it{E}}_{T}))", 20, 0., 1);
  HistogramGetter* njet_shape    = new HistogramGetter("njets",       "njets",                                     "N_{jets}",                                       10,   .5, 10.5);
  HistogramGetter* mht_shape     = new HistogramGetter("mht",         "mht",                                       "mh_{T}",                                         20,   0., 1000.);
  HistogramGetter* metht_shape   = new HistogramGetter("met_ht",      "(met + ht)",                                "#slash{#it{E}}_{T} + h_{T} [GeV]",               nHT_bins - 1, ht_bins);
  HistogramGetter* ht_shape      = new HistogramGetter("ht",          "ht",                                        "h_{T} [GeV]",                                    60, 0., 3000.);
  HistogramGetter* ptb_shape     = new HistogramGetter("ptb",         "csvj1pt",                                   "p_{T}(b) [GeV]",                                 60, 0., 1500.);

//  printSmearComparisonsByBin(sampleTrees, met_shape,     "", ht_binning,               QCDSupport::CR_def + " && nbjets == 0 && j2pt > 70 && njets >= 5", "Events", true, true);
//  printSmearComparisonsByBin(sampleTrees, met_shape,     "", ht_binning, ht_v_binning, QCDSupport::CR_def + " && nbjets == 0 && j2pt > 70 && njets >= 5", "Events", true, true);
//  printSmearComparisonsByBin(sampleTrees, met_shape,     "", nb_binning,                                        "j2pt > 70 && njets >= 5", "Events", true);
//  printSmearComparisonsByBin(sampleTrees, met_shape,     "", nb_binning, dphi_binning,                          "j2pt > 70 && njets >= 5", "Events", true);
//  printSmearComparisonsByBin(sampleTrees, met_shape,     "", nb_binning,  mtb_binning, QCDSupport::CR_def + " && j2pt > 70 && njets >= 5", "Events", true);
//  printSmearComparisonsByBin(sampleTrees, dphi12_shape,  "", nb_binning,  met_binning,                          "j2pt > 70 && njets >= 5", "Events", true, true);
//  printSmearComparisonsByBin(sampleTrees, dphi123_shape, "", nb_binning,  met_binning,                          "j2pt > 70 && njets >= 5", "Events", true, true);

//  printStack(stackTrees,  met_shape,  "stack", nb_binning,  mtb_binning, jet_binning, QCDSupport::CR_def, "Events", true);
//  printStack(stackTrees,  njet_shape, "stack", nb_binning, met2_binning, QCDSupport::CR_def, "Events", true);
//  printStack(stackTrees,   mht_shape, "stack", nb_binning, met2_binning, QCDSupport::CR_def, "Events", true);
//  printStack(stackTrees, metht_shape, "stack", nb_binning, met2_binning, QCDSupport::CR_def, "Events", true);
//  printStack(stackTrees, ht_shape,    "stack", nb_binning, met2_binning, QCDSupport::CR_def, "Events", true);

  printStack(stackTrees, dphi12_shape,           "stack", dphi_binning_test,       QCDSupport::METPresel + " && " + QCDSupport::BaselineExtraCuts, "Events",false,false,true);
  //printStack(stackTrees, dphi12_shape,           "stack", dphi_binning,            QCDSupport::METPresel + " && " + QCDSupport::BaselineExtraCuts, "Events",true,true);
  //printStack(stackTrees, dphi123_shape,          "stack", dphi_binning,            QCDSupport::METPresel + " && " + QCDSupport::BaselineExtraCuts, "Events",true,true);
  //printStack(stackTrees, dphimht12_shape,        "stack", dphimhtj_binning,        QCDSupport::METPresel + " && " + QCDSupport::BaselineExtraCuts, "Events",true,true);
  //printStack(stackTrees, dphimht123_shape,       "stack", dphimhtj_binning,        QCDSupport::METPresel + " && " + QCDSupport::BaselineExtraCuts, "Events",true,true);
  //printStack(stackTrees, dphimht1234_shape,      "stack", dphimhtj_binning,        QCDSupport::METPresel + " && " + QCDSupport::BaselineExtraCuts, "Events",true,true);
  //printStack(stackTrees, dphiSmht12_shape,       "stack", dphiSmhtj_binning,       QCDSupport::METPresel + " && " + QCDSupport::BaselineExtraCuts, "Events",true,true);
  //printStack(stackTrees, dphiSmht123_shape,      "stack", dphiSmhtj_binning,       QCDSupport::METPresel + " && " + QCDSupport::BaselineExtraCuts, "Events",true,true);
  //printStack(stackTrees, dphiSmht1234_shape,     "stack", dphiSmhtj_binning,       QCDSupport::METPresel + " && " + QCDSupport::BaselineExtraCuts, "Events",true,true);
  //printStack(stackTrees, dphi12Z_shape,          "Z_stack", dphi_binning,          QCDSupport::METPresel + " && " + QCDSupport::BaselineExtraCuts, "Events",true,true);
  //printStack(stackTrees, dphi123Z_shape,         "Z_stack", dphi_binning,          QCDSupport::METPresel + " && " + QCDSupport::BaselineExtraCuts, "Events",true,true);
  //printStack(stackTrees, dphimht12Z_shape,       "Z_stack", dphimhtj_binning,      QCDSupport::METPresel + " && " + QCDSupport::BaselineExtraCuts, "Events",true,true);
  //printStack(stackTrees, dphimht123Z_shape,      "Z_stack", dphimhtj_binning,      QCDSupport::METPresel + " && " + QCDSupport::BaselineExtraCuts, "Events",true,true);
  //printStack(stackTrees, dphimht1234Z_shape,     "Z_stack", dphimhtj_binning,      QCDSupport::METPresel + " && " + QCDSupport::BaselineExtraCuts, "Events",true,true);
  //printStack(stackTrees, dphiSmht12Z_shape,      "Z_stack", dphiSmhtj_binning,     QCDSupport::METPresel + " && " + QCDSupport::BaselineExtraCuts, "Events",true,true);
  //printStack(stackTrees, dphiSmht123Z_shape,     "Z_stack", dphiSmhtj_binning,     QCDSupport::METPresel + " && " + QCDSupport::BaselineExtraCuts, "Events",true,true);
  //printStack(stackTrees, dphiSmht1234Z_shape,    "Z_stack", dphiSmhtj_binning,     QCDSupport::METPresel + " && " + QCDSupport::BaselineExtraCuts, "Events",true,true);
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

void drawPlot(Plot* p_Plot, float yMin, float yMax, bool p_drawRatio, bool p_drawStack){
  TCanvas* canvas = new TCanvas;
  p_Plot->setLegend(.45, .65, .92, .9);
  p_Plot->setYRange(yMin, yMax);
  p_Plot->setHeader(QCDSupport::header, "");
  if(p_drawRatio){
    if(!p_drawStack){
      p_Plot->drawRatios(canvas);
    } else {
      p_Plot->drawRatioStack(canvas);
    }
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
    TH1F* hist = p_HistG->getHistogram(p_SampleTrees[iS].first, p_Sel, QCDSupport::stdMCWeight, TString::Format("QCD_%s_%u", p_Suffix.Data(), iS));
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

void printSmearComparisonsByBin(vector <pair <TTree*, TString> >& p_SampleTrees, HistogramGetter* p_HistG, TString p_suffix, vector <pair <TString, pair <TString, TString> > >& p_BinInfo, TString p_Sel, TString p_yTitle, bool p_drawRatio, bool p_logY, bool p_normalize){
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

void printSmearComparisonsByBin(vector <pair <TTree*, TString> >& p_SampleTrees, HistogramGetter* p_HistG, TString p_suffix, vector <pair <TString, pair <TString, TString> > >& p_BinInfo_1, vector <pair <TString, pair <TString, TString> > >& p_BinInfo_2, TString p_Sel, TString p_yTitle, bool p_drawRatio, bool p_logY, bool p_normalize){
  for(int iB_1 = 0; iB_1 < p_BinInfo_1.size(); iB_1++){
    for(int iB_2 = 0; iB_2 < p_BinInfo_2.size(); iB_2++){
      TString sel = "";
      TString suf = TString::Format("%s_%s",  p_BinInfo_1[iB_1].first.Data(), p_BinInfo_2[iB_2].first.Data());
      TString dsc = TString::Format("%s, %s", p_BinInfo_2[iB_2].second.second.Data(), p_BinInfo_1[iB_1].second.second.Data());
      if(p_Sel.Sizeof() > 1){
        sel = TString::Format("%s && %s && %s", p_Sel.Data(), p_BinInfo_1[iB_1].second.first.Data(), p_BinInfo_2[iB_2].second.first.Data());
      } else {
        sel = TString::Format("%s && %s", p_BinInfo_1[iB_1].second.first.Data(), p_BinInfo_2[iB_2].second.first.Data());
      }
      printSmearComparison(p_SampleTrees, suf + p_suffix, sel, dsc, p_HistG, p_yTitle, p_drawRatio, p_logY, p_normalize);
    }
  }
}

void printStack(vector <pair <TTree*, TString> >& p_StackTrees, TString p_Suffix, TString p_Sel, TString p_Desc, HistogramGetter* p_HistG, TString p_yTitle, bool p_drawRatio, bool p_logY, bool p_ROCCurve){
  Plot* plot = new Plot(p_HistG->plotInfo->name + "_" + p_Suffix, p_Desc, p_HistG->plotInfo->xTitle, p_yTitle);
  PlotROCCurves* plots = new PlotROCCurves();
  float yMin = 99999., yMax = 0., yMax_qcd = 0., yMax_data = 0., yMax_sig = 0., yMin_sig = 0.;
  float totMCYield = 0;
  TH1F* nonQCD_hist; 
  for(unsigned int iS = 0; iS < p_StackTrees.size(); ++iS){
    if(p_StackTrees[iS].second == "Data"){
      TH1F* hist = p_HistG->getHistogram(p_StackTrees[iS].first, p_Sel, QCDSupport::stdWeight, TString::Format("data_%s_%u", p_Suffix.Data(), iS));
      plot->addHist(hist, p_StackTrees[iS].second, "E0", colors[iS], 0, colors[iS]);
      if(yMax_data < hist->GetMaximum()) yMax_data = hist->GetMaximum();
    }
    else if (p_StackTrees[iS].second == "Non-QCD bkg"){
      nonQCD_hist = p_HistG->getHistogram(p_StackTrees[iS].first, p_Sel, QCDSupport::stdMCWeight, TString::Format("nonQCD_%s_%u", p_Suffix.Data(), iS));
      plot->addToStack(nonQCD_hist, p_StackTrees[iS].second, colors[iS], 1001, 1, 1, 3);
      totMCYield += nonQCD_hist->Integral(0, nonQCD_hist->GetNbinsX() + 1);
      if(yMin > getMinimum(nonQCD_hist)) yMin = getMinimum(nonQCD_hist);
      if(yMax < nonQCD_hist->GetMaximum()) yMax = nonQCD_hist->GetMaximum();
    }
    else if(p_StackTrees[iS].second == "Smeared") {
      TH1F* hist = p_HistG->getHistogram(p_StackTrees[iS].first, p_Sel, QCDSupport::stdMCWeight, TString::Format("QCD_%s_%u", p_Suffix.Data(), iS));
      plot->addToStack(hist, p_StackTrees[iS].second, colors[iS], 1001, 1, 1, 3);
      totMCYield += hist->Integral(0, hist->GetNbinsX() + 1);
      if(yMax_qcd < hist->GetMaximum()) yMax_qcd = hist->GetMaximum();
    }
    else if(p_StackTrees[iS].second == "With orig. QCD MC"){
      TH1F* hist = p_HistG->getHistogram(p_StackTrees[iS].first, p_Sel, QCDSupport::stdMCWeight, TString::Format("origQCD_%s_%u", p_Suffix.Data(), iS));
      hist->Add(nonQCD_hist);
      plot->addHistForRatio(hist, p_StackTrees[iS].second, "hist", colors[iS], 0, colors[iS]);
    }
    else if (p_StackTrees[iS].second.Contains("T2tt")){
      TH1F* hist = p_HistG->getHistogram(p_StackTrees[iS].first, p_Sel, QCDSupport::stdMCWeight, TString::Format("signal_%s_%u", p_Suffix.Data(), iS));
      hist->Scale(totMCYield / hist->Integral(0, hist->GetNbinsX() + 1));
      plot->addHist(hist, p_StackTrees[iS].second, "hist", colors[iS], 0, colors[iS]);
    }
    else if (p_StackTrees[iS].second.Contains("T2fbd")){
      TH1F* hist = p_HistG->getHistogram(p_StackTrees[iS].first, p_Sel, QCDSupport::stdMCWeight, TString::Format("signal_fbd_%s_%u", p_Suffix.Data(), iS));
      hist->Scale(totMCYield / hist->Integral(0, hist->GetNbinsX() + 1));
      plot->addHist(hist, p_StackTrees[iS].second, "hist", colors[iS], 0, colors[iS]);
    }
  }

  yMax = max(yMax + yMax_qcd, yMax_data);
  if(p_logY){
    plot->setLogy();
    drawPlot(plot,  0.9 * yMin, 2 * yMax, p_drawRatio, true);
  } else {
    drawPlot(plot,  .1, 1.3 * yMax , p_drawRatio, true);
  }
}

void printStack(vector <pair <TTree*, TString> >& p_StackTrees, HistogramGetter* p_HistG, TString p_suffix, vector <pair <TString, pair <TString, TString> > >& p_BinInfo_1, TString p_Sel, TString p_yTitle, bool p_drawRatio, bool p_logY, bool p_ROCCurve){
  for(int iB_1 = 0; iB_1 < p_BinInfo_1.size(); iB_1++){
      TString sel = "";
      TString suf = TString::Format("%s",  p_BinInfo_1[iB_1].first.Data());
      TString dsc = TString::Format("%s", p_BinInfo_1[iB_1].second.second.Data());
      if(p_Sel.Sizeof() > 1){
        sel = TString::Format("%s && %s", p_Sel.Data(), p_BinInfo_1[iB_1].second.first.Data());
      } else {
        sel = TString::Format("%s", p_BinInfo_1[iB_1].second.first.Data());
      }
      printStack(p_StackTrees, suf + p_suffix, sel, dsc, p_HistG, p_yTitle, p_drawRatio, p_logY, p_ROCCurve);
  }
}

void printStack(vector <pair <TTree*, TString> >& p_StackTrees, HistogramGetter* p_HistG, TString p_suffix, vector <pair <TString, pair <TString, TString> > >& p_BinInfo_1, vector <pair <TString, pair <TString, TString> > >& p_BinInfo_2, TString p_Sel, TString p_yTitle, bool p_drawRatio, bool p_logY, bool p_ROCCurve){
  for(int iB_1 = 0; iB_1 < p_BinInfo_1.size(); iB_1++){
    for(int iB_2 = 0; iB_2 < p_BinInfo_2.size(); iB_2++){
      TString sel = "";
      TString suf = TString::Format("%s_%s",  p_BinInfo_1[iB_1].first.Data(),         p_BinInfo_2[iB_2].first.Data());
      TString dsc = TString::Format("%s, %s", p_BinInfo_1[iB_1].second.second.Data(), p_BinInfo_2[iB_2].second.second.Data());
      if(p_Sel.Sizeof() > 1){
        sel = TString::Format("%s && %s && %s", p_Sel.Data(), p_BinInfo_1[iB_1].second.first.Data(), p_BinInfo_2[iB_2].second.first.Data());
      } else {
        sel = TString::Format("%s && %s", p_BinInfo_1[iB_1].second.first.Data(), p_BinInfo_2[iB_2].second.first.Data());
      }
      printStack(p_StackTrees, suf + p_suffix, sel, dsc, p_HistG, p_yTitle, p_drawRatio, p_logY, p_ROCCurve);
    }
  }
}

void printStack(vector <pair <TTree*, TString> >& p_StackTrees, HistogramGetter* p_HistG, TString p_suffix, vector <pair <TString, pair <TString, TString> > >& p_BinInfo_1, vector <pair <TString, pair <TString, TString> > >& p_BinInfo_2, vector <pair <TString, pair <TString, TString> > >& p_BinInfo_3, TString p_Sel, TString p_yTitle, bool p_drawRatio, bool p_logY, bool p_ROCCurve){
  for(int iB_1 = 0; iB_1 < p_BinInfo_1.size(); iB_1++){
    for(int iB_2 = 0; iB_2 < p_BinInfo_2.size(); iB_2++){
      for(int iB_3 = 0; iB_3 < p_BinInfo_3.size(); iB_3++){
        TString sel = "";
        TString suf = TString::Format("%s_%s_%s",   p_BinInfo_1[iB_1].first.Data(),         p_BinInfo_2[iB_2].first.Data(),         p_BinInfo_3[iB_3].first.Data());
        TString dsc = TString::Format("%s, %s, %s", p_BinInfo_1[iB_1].second.second.Data(), p_BinInfo_2[iB_2].second.second.Data(), p_BinInfo_3[iB_3].second.second.Data());
        if(p_Sel.Sizeof() > 1){
          sel = TString::Format("%s && %s && %s && %s", p_Sel.Data(), p_BinInfo_1[iB_1].second.first.Data(), p_BinInfo_2[iB_2].second.first.Data(), p_BinInfo_3[iB_3].second.first.Data());
        } else {
          sel = TString::Format("%s && %s && %s", p_BinInfo_1[iB_1].second.first.Data(), p_BinInfo_2[iB_2].second.first.Data(), p_BinInfo_3[iB_3].second.first.Data());
        }
        printStack(p_StackTrees, suf + p_suffix, sel, dsc, p_HistG, p_yTitle, p_drawRatio, p_logY, p_ROCCurve);
      }
    }
  }
}
