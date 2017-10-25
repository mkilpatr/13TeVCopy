#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisTools/Utilities/interface/JetFlavorInfo.h"
#include "AnalysisTools/TreeReader/interface/Defaults.h"
#include "AnalysisMethods/PlotUtils/interface/Plot.hh"
#include "AnalysisMethods/EstTools/utils/Estimator.hh"
#include "AnalysisMethods/EstTools/utils/HistGetter.hh"
#include "AnalysisMethods/EstTools/Moriond2017/SRParameters.hh"
//#include "AnalysisMethods/EstMethods/LLBEstimator.hh"
//#include "AnalysisMethods/macros/JetMETStudies/GetQCDPrediction.C"
//#include "LLPredictionHelper.h"

using namespace std;
using namespace EstTools;

const TString originalTreeDir = "../../run/plots_17_10_18_MC";
TString outputDir = "";
vector<int> colors;

TString local_stdwgt = "1.0";
TString local_wgtvar = wgtvar;
//TString local_wgtvar_2016 = wgtvar2016;
TString local_qcdwgt = qcdwgt;

TTree* getTree(TString filename);
float getMaximum(TH1F* p_Hist);
void drawPlot(Plot* p_Plot, float yMin = 0., float yMax = 1., bool p_drawRatio = false, bool p_drawStack = false);
void printComparison(vector <pair <TTree*, TString> >& p_SampleTrees, HistogramGetter* p_HistG, TString p_Suffix, TString p_Sel, TString p_Desc, TString p_yTitle, bool p_drawRatio = false, bool p_logY = false, bool p_normalize = false);
void printComparison(vector <pair <TTree*, TString> >& p_SampleTrees, HistogramGetter* p_HistG, TString p_Suffix, TString p_Sel, vector <pair <TString, pair <TString, TString> > >& p_BinInfo, TString p_yTitle, bool p_drawRatio = false, bool p_logY = false, bool p_normalize = false);
void printComparison(vector <pair <TTree*, TString> >& p_SampleTrees, HistogramGetter* p_HistG, TString p_Suffix, TString p_Sel, vector <pair <TString, pair <TString, TString> > >& p_BinInfo_1, vector <pair <TString, pair <TString, TString> > >& p_BinInfo_2, TString p_yTitle, bool p_drawRatio = false, bool p_logY = false, bool p_normalize = false);
void printStack(vector <pair <TTree*, TString> >& p_StackTrees, HistogramGetter* p_HistG, TString p_Suffix, TString p_Sel, TString p_Desc, TString p_yTitle, bool p_drawRatio, bool p_logY);
void printStack(vector <pair <TTree*, TString> >& p_StackTrees, HistogramGetter* p_HistG, TString p_Suffix, vector <pair <TString, TString> >& p_Sel, TString p_Desc, TString p_yTitle, bool p_drawRatio, bool p_logY);
void printStack(vector <pair <TTree*, TString> >& p_StackTrees, HistogramGetter* p_HistG, TString p_Suffix, TString p_Sel, vector <pair <TString, pair <TString, TString> > >& p_BinInfo, TString p_yTitle, bool p_drawRatio = false, bool p_logY = false);
void printStack(vector <pair <TTree*, TString> >& p_StackTrees, HistogramGetter* p_HistG, TString p_Suffix, vector <pair <TString, TString> >& p_Sel, vector <pair <TString, pair <TString, TString> > >& p_BinInfo, TString p_yTitle, bool p_drawRatio = false, bool p_logY = false);
void printStack(vector <pair <TTree*, TString> >& p_StackTrees, HistogramGetter* p_HistG, TString p_Suffix, TString p_Sel, vector <pair <TString, pair <TString, TString> > >& p_BinInfo_1, vector <pair <TString, pair <TString, TString> > >& p_BinInfo_2, TString p_yTitle, bool p_drawRatio = false, bool p_logY = false);
void printStack(vector <pair <TTree*, TString> >& p_StackTrees, HistogramGetter* p_HistG, TString p_Suffix, TString p_Sel, vector <pair <TString, pair <TString, TString> > >& p_BinInfo_1, vector <pair <TString, pair <TString, TString> > >& p_BinInfo_2, vector <pair <TString, pair <TString, TString> > >& p_BinInfo_3, TString p_yTitle, bool p_drawRatio = false, bool p_logY = false);
void printLocalPlots(TTree* p_qcdTree, vector <HistogramGetter*>& p_HistGs, TString p_Prefix, TString p_Sel, vector <pair <pair <TString, TString>, pair <TString, TString> > >& p_LocalSels, vector <pair <pair <TString, TString>, pair<TString, TString> > >& p_SelsAndNames, bool p_logY = false, bool p_normalize = false, bool p_drawRatio = false);
void printLepPlots(TTree* p_qcdTree, vector <HistogramGetter*>& p_HistGs, TString p_Prefix, TString p_Sel, vector <pair <pair <TString, TString>, pair <TString, TString> > >& p_LocalSels, vector <pair <pair <TString, TString>, pair<TString, TString> > >& p_SelsAndNames, bool p_logY = false, bool p_normalize = false, bool p_drawRatio = false);
TH1F* modifyFlavorHist(TH1F* p_FlavorHist);
void printTFPlots(TTree* p_qcdTree, TString p_Prefix, TString p_Sel, vector <pair <pair <TString, TString>, pair <TString, TString> > >& p_LocalSels_CR, vector <pair <pair <TString, TString>, pair <TString, TString> > >& p_LocalSels_SR, vector <vector <double> >& p_METbins, vector <pair <pair <TString, TString>, pair<TString, TString> > >& p_SelsAndNames, bool p_logY = false);

void ValidateLepton();
void makeLepShape_Plots();
void makeJetLepStuct_Plots();
void LLTransferFactor();

#endif

void OneLepDistributions(){
  SetTDRStyle();

//  ValidateLepton();
  LLTransferFactor();
//  makeLLBPrediction();
//  makeLepShape_Plots();
//  makeJetLepStuct_Plots();
}

void ValidateLepton(){
  outputDir = "validate_CR_fix";
  gSystem->mkdir(outputDir, true);
//  TString local_baseline     = "met>250 && njets>=2 && (passmetfilters || process==10) && j1chEnFrac>0.1 && j1chEnFrac<0.99 && ak8isrpt>300 && dphiisrmet>2 && nsdtop==0 && nsdw==0 && nrestop==0 && metovsqrtht>10";
//  TString local_baseline_200 = "met>200 && njets>=2 && (passmetfilters || process==10) && j1chEnFrac>0.1 && j1chEnFrac<0.99 && ak8isrpt>300 && dphiisrmet>2 && nsdtop==0 && nsdw==0 && nrestop==0 && metovsqrtht>10";
//  TString local_baseline_CR  = local_baseline + dphi_invert;
//  TString local_baseline_SR  = local_baseline + "&& dphij1met>0.5 && dphij2met>0.15";
  TString local_baseline     = "met>250 && mht > 120 && njets>=2  && (passmetfilters || process==10) && j1chEnFrac>0.1 && j1chEnFrac<0.99";
  TString local_baseline2017 = "met>250 && mht > 120 && njets>=2  && (passmetfilters2017 || process==10) && j1chEnFrac>0.1 && j1chEnFrac<0.99";
  TString local_baseline_200 = "met>200 && ht>250 && njets>=2  && (passmetfilters || process==10) && j1chEnFrac>0.1 && j1chEnFrac<0.99";
  TString local_baseline_CR2017 = local_baseline2017 + lepcrsel + " && dphij1met>0.5 && dphij2met>0.15";
  TString local_baseline_CR     = local_baseline + lepcrsel + " && dphij1met>0.5 && dphij2met>0.15";
  TString local_baseline_SR  = local_baseline + "&& dphij1met>0.5 && dphij2met>0.15";
  TString ivfDeltaR = "ivf_DRak4j1 > 0.4";
//  TString ivfDeltaR = "max(max(ivf_DRak4j1, ivf_DRak4j2),max(ivf_DRak4j3, ivf_DRak4j4)) > 0.4";
//  TString local_baseline_ht  = "met >= 200 && met < 250 && ht > 1000 && njets>=5 && (passtright || ismc) && j1chEnFrac>0.1 && j1chEnFrac<0.99";
//  TString old_sel            = "passjson && (passmetfilters || process == 10) && met >= 250 && (passmetmht || ismc) && njl>=1 && njets>=5 && dphij1lmet>2. && j1lpt>=250 && met/sqrt(ht)>=10 && j1chEnFrac>0.1 && j1chEnFrac<0.99";
  
  vector <pair <TTree*, TString> > sampleTrees;
  sampleTrees.emplace_back(getTree(originalTreeDir + "/met_tree.root"),      "Data");                 colors.push_back(1);
  //sampleTrees.emplace_back(getTree(originalTreeDir + "/qcd_tree.root"),      "Smeared (puWeight*weight)");
  sampleTrees.emplace_back(getTree(originalTreeDir + "/qcd_orig_tree.root"), "With orig. QCD MC");    colors.push_back(color_qcd);
  sampleTrees.emplace_back(getTree(originalTreeDir + "/ttbar_tree.root"),    "ttbar");                colors.push_back(color_ttbar);
  sampleTrees.emplace_back(getTree(originalTreeDir + "/wjets_tree.root"),    "wjets");                colors.push_back(color_wjets);
  sampleTrees.emplace_back(getTree(originalTreeDir + "/znunu_tree.root"),    "znunu");                colors.push_back(kMagenta);
  sampleTrees.emplace_back(getTree(originalTreeDir + "/ttOther_tree.root"),  "other");                colors.push_back(color_tW);
  //sampleTrees.emplace_back(getTree(originalTreeDir + "/ttbarplusw_tree.root"), "Lepton Elec");        colors.push_back(kRed);
  //sampleTrees.emplace_back(getTree(originalTreeDir + "/ttbarplusw_tree.root"), "Lepton Muon");        colors.push_back(kBlue);
  vector <pair <TTree*, TString> > stackTrees;
  stackTrees.emplace_back(getTree(originalTreeDir + "/../plots_17_10_18_data/met_tree.root"),                        "Data 2017");            colors.push_back(1);
  stackTrees.emplace_back(getTree(originalTreeDir + "/ttbar_tree.root"),    					     "ttbar");                colors.push_back(color_ttbar);
  stackTrees.emplace_back(getTree(originalTreeDir + "/wjets_tree.root"),    					     "wjets");                colors.push_back(color_wjets);
  stackTrees.emplace_back(getTree(originalTreeDir + "/znunu_tree.root"),    					     "znunu");                colors.push_back(kMagenta);
  stackTrees.emplace_back(getTree(originalTreeDir + "/ttOther_tree.root"),  					     "other");                colors.push_back(color_tW);
  stackTrees.emplace_back(getTree(originalTreeDir + "/qcd_orig_tree.root"), 					     "With orig. QCD MC");    colors.push_back(color_qcd);
  //stackTrees.emplace_back(getTree(originalTreeDir + "/ttbarplusw_tree.root"),   "Lepton Elec");        colors.push_back(color_qcd);
  //stackTrees.emplace_back(getTree(originalTreeDir + "/ttbarplusw_tree.root"),   "Lepton Muon");        colors.push_back(color_tW);
  //stackTrees.emplace_back(getTree(originalTreeDir + "/ttbarplusw_tree.root"),   "Lepton Tau Total");    colors.push_back(kAzure + 10);
  //stackTrees.emplace_back(getTree(originalTreeDir + "/ttbarplusw_tree.root"),   "Lepton Tau Elec");     colors.push_back(kBlue);
  //stackTrees.emplace_back(getTree(originalTreeDir + "/ttbarplusw_tree.root"),   "Lepton Tau Muon");     colors.push_back(kMagenta);
  //stackTrees.emplace_back(getTree(originalTreeDir + "/ttbarplusw_tree.root"),   "Lepton Tau Hadr");     colors.push_back(kRed);
  //stackTrees.emplace_back(getTree(originalTreeDir + "/qcd_tree.root"),      "Smeared QCD MC");    colors.push_back(color_qcd);
  vector <pair <TTree*, TString> > stackTrees_data;
  stackTrees_data.emplace_back(getTree(originalTreeDir + "/../plots_17_10_18_data/met_tree.root"),                        "Data 2017");              colors.push_back(1);
  stackTrees_data.emplace_back(getTree(originalTreeDir + "/met_tree.root"),       "Data 2016");              colors.push_back(kRed);
  //vector <pair <TTree*, TString> > stackTrees_jetht;
  ////stackTrees_jetht.emplace_back(getTree(originalTreeDir + "/jetht_tree.root"),      "Data");            colors.push_back(1);
  //stackTrees_jetht.emplace_back(getTree(originalTreeDir + "/nonQCD_tree.root"),   "Non-QCD bkg");       colors.push_back(color_ttbar);
  //stackTrees_jetht.emplace_back(getTree(originalTreeDir + "/qcd_tree.root"),      "Smeared QCD MC");    colors.push_back(color_qcd);
  ////stackTrees_jetht.emplace_back(getTree(originalTreeDir + "/qcd_orig_tree.root"), "With orig. QCD MC"); colors.push_back(color_tW);
  
  vector <pair <TString, TString> > dataMC_selsAndNames = {
         								   make_pair(local_baseline_CR2017,                local_wgtvar),
									   make_pair(local_baseline_CR    ,                local_wgtvar),
         								   make_pair(local_baseline_CR    ,                local_wgtvar),
         								   make_pair(local_baseline_CR    ,                local_wgtvar),
         								   make_pair(local_baseline_CR    ,                local_wgtvar),
         								   make_pair(local_baseline_CR    ,                local_wgtvar),
							}; 
  vector <pair <TString, TString> > dataVData_selsAndNames = {
         								   make_pair(local_baseline_CR2017,                local_wgtvar),
         								   make_pair(local_baseline_CR    ,                local_wgtvar),
							}; 
  vector <pair <TString, pair <TString, TString> > >   passmetmht_bins = { 
									   make_pair("fullmet_",           make_pair("passmetmht",                         "fullpassmetmht")), 
                                                                           make_pair("120met_",            make_pair("passmetmht_120trig",                 "120passmetmht"))
									 };
  
  vector <pair <TString, pair <TString, TString> > >   ivfDeltaR_bins = { 
									   make_pair("noCut_",           make_pair("1",                         "noCut")), 
                                                                           make_pair("DR4_",             make_pair(ivfDeltaR,                   "DR4_"))
									 };
  
  vector <pair <TString, pair <TString, TString> > > multiplicity_bins = { make_pair("nj_2_to_5",          make_pair("njets >= 2 && njets <= 5", "2 #leq N_{jets} #leq 5")), 
                                                                           make_pair("nj_geq_6",           make_pair("njets >= 6",               "N_{jets} #geq 6")), 
                                                                           make_pair("nj_geq_7",           make_pair("njets >= 7",               "N_{jets} #geq 7")), 
                                                                           make_pair("nb_0",               make_pair("nbjets == 0",              "N_{b} = 0")), 
                                                                           make_pair("nb_1",               make_pair("nbjets == 1",              "N_{b} = 1")), 
                                                                           make_pair("nb_geq_2",           make_pair("nbjets >= 2",              "N_{b} #geq 2")), 
                                                                           make_pair("nsv_0",              make_pair("nivf == 0",                "N_{SV} = 0")), 
                                                                           make_pair("nsv_geq_1",          make_pair("nivf >= 1",                "N_{SV} #geq 1")), 
                                                                           make_pair("nt_0",               make_pair("nsdtop == 0",              "N_{t} = 0")), 
                                                                           make_pair("nt_1",               make_pair("nsdtop == 1",              "N_{t} = 1")), 
                                                                           make_pair("nt_geq_2",           make_pair("nsdtop >= 2",              "N_{t} #geq 2")), 
                                                                           make_pair("nw_0",               make_pair("nsdw == 0",                "N_{W} = 0")), 
                                                                           make_pair("nw_1",               make_pair("nsdw == 1",                "N_{W} = 1")), 
                                                                           make_pair("nw_geq_2",           make_pair("nsdw >= 2",                "N_{W} #geq 2")), 
                                                                           make_pair("nrt_0",              make_pair("nrestop == 0",             "N_{res} = 0")), 
                                                                           make_pair("nrt_1",              make_pair("nrestop == 1",             "N_{res} = 1")), 
                                                                           make_pair("nrt_geq_2",          make_pair("nrestop >= 2",             "N_{res} #geq 2")),
                                                                         };
  vector <pair <TString, pair <TString, TString> > >          met_bins = { make_pair("met_200_to_300",     make_pair("met >= 200 && met < 300",  "200 #leq #slash{#it{E}}_{T} #leq 300")), 
                                                                           make_pair("met_300_to_400",     make_pair("met >= 300 && met < 400",  "300 #leq #slash{#it{E}}_{T} #leq 400")), 
                                                                           make_pair("met_400_to_500",     make_pair("met >= 400 && met < 500",  "400 #leq #slash{#it{E}}_{T} #leq 500")), 
                                                                           make_pair("met_500_to_600",     make_pair("met >= 500 && met < 600",  "500 #leq #slash{#it{E}}_{T} #leq 600")), 
                                                                           make_pair("met_600_to_700",     make_pair("met >= 600 && met < 700",  "600 #leq #slash{#it{E}}_{T} #leq 700")), 
                                                                           make_pair("met_700_to_800",     make_pair("met >= 700 && met < 800",  "700 #leq #slash{#it{E}}_{T} #leq 800")), 
                                                                           make_pair("met_800_to_900",     make_pair("met >= 800 && met < 900",  "800 #leq #slash{#it{E}}_{T} #leq 900")), 
                                                                           make_pair("met_900_to_1000",    make_pair("met >= 900 && met < 1000", "900 #leq #slash{#it{E}}_{T} #leq 1000")), 
                                                                           make_pair("met_ge_1000",        make_pair("met >= 1000",              "#slash{#it{E}}_{T} #geq 1000")) };
  vector <pair <TString, pair <TString, TString> > >        nb_binning = { make_pair("nb_eq_0",            make_pair("nbjets == 0",              "n_{b} = 0")),
                                                                           make_pair("nb_eq_1",            make_pair("nbjets == 1",              "n_{b} = 1")),
                                                                           make_pair("nb_ge_2",            make_pair("nbjets >= 2",              "n_{b} #geq 2")) };
  vector <pair <TString, pair <TString, TString> > >      dphi_binning = { make_pair("CR_123",             make_pair(dphi_invert,                                                        "CR")),
                                                                           make_pair("SR_12",              make_pair("dphij1met >= 0.5 && dphij2met>=0.15",                              "SR (low mass)")),
                                                                           make_pair("SR_1234",            make_pair("dphij1met>0.5 && dphij2met>0.5 && dphij3met>0.5 && dphij4met>0.5", "SR (high mass)"))};
  vector <pair <TString, pair <TString, TString> > >       met_binning = { make_pair("met_250_to_400",     make_pair("met >= 250 && met < 400", "250 #leq #slash{#it{E}}_{T} < 400")),
                                                                           make_pair("met_ge_400",         make_pair("met >= 400",              "#slash{#it{E}}_{T} #geq 400")) };
  vector <pair <TString, pair <TString, TString> > >        ht_binning = { make_pair("ht_200_to_300",      make_pair("ht >= 200 && ht < 300",   "200 #leq h_{T} < 300")),
                                                                           make_pair("ht_300_to_500",      make_pair("ht >= 300 && ht < 500",   "300 #leq h_{T} < 500")),
                                                                           make_pair("ht_500_to_700",      make_pair("ht >= 500 && ht < 700",   "500 #leq h_{T} < 700")),
                                                                           make_pair("ht_700_to_1000",     make_pair("ht >= 700 && ht < 1000",  "700 #leq h_{T} < 1000")),
                                                                           make_pair("ht_1000_to_1500",    make_pair("ht >= 1000 && ht < 1500", "1000 #leq h_{T} < 1500")),
                                                                           make_pair("ht_1500_to_2000",    make_pair("ht >= 1500 && ht < 2000", "1500 #leq h_{T} < 2000")),
                                                                           make_pair("ht_gt_2000",         make_pair("ht >= 2000",              "h_{T} #geq 2000")) };
  vector <pair <TString, pair <TString, TString> > >     genht_binning = { make_pair("genht_200_to_300",   make_pair("whichQCD_HT_Sample == 1", "200 #leq genh_{T} < 300")),
                                                                           make_pair("genht_300_to_500",   make_pair("whichQCD_HT_Sample == 2", "300 #leq genh_{T} < 500")),
                                                                           make_pair("genht_500_to_700",   make_pair("whichQCD_HT_Sample == 3", "500 #leq genh_{T} < 700")),
                                                                           make_pair("genht_700_to_1000",  make_pair("whichQCD_HT_Sample == 4", "700 #leq genh_{T} < 1000")),
                                                                           make_pair("genht_1000_to_1500", make_pair("whichQCD_HT_Sample == 5", "1000 #leq genh_{T} < 1500")),
                                                                           make_pair("genht_1500_to_2000", make_pair("whichQCD_HT_Sample == 6", "1500 #leq genh_{T} < 2000")),
                                                                           make_pair("genht_gt_2000",      make_pair("whichQCD_HT_Sample == 7", "genh_{T} #geq 2000")) };
  vector <pair <TString, pair <TString, TString> > >    genlep_binning = { make_pair("gen_cut",      make_pair("met >= 100",  "")) };

//  vector <pair <TString, pair <TString, TString> > > bins = { make_pair("common",      make_pair("met >= 250 && (passmetmht || ismc) && passjson && (passmetfilters || process==10) && j1chEnFrac>0.1 && j1chEnFrac<0.99", "Common")),
//                                                              make_pair("njge7",       make_pair("njets>=7",       "N_{j} #geq 7")),
//                                                              make_pair("nb1",         make_pair("nbjets==1",      "N_{b} = 1")),
//                                                              make_pair("highmtb",     make_pair("mtcsv12met>175", "m_{T}(b) > 175")),
//                                                              make_pair("nttau0",      make_pair("nvetotau==0",    "N_{vtau} = 0")),
//                                                              make_pair("CR",          make_pair("(dphij1met<0.1 || dphij2met<0.1 || dphij3met<0.1)", "CR")),
//                                                              make_pair("mtlepmet100", make_pair("mtlepmet < 100", "m_{T}(lep) < 100")),
//                                                              make_pair("ntlepge1",    make_pair("nvetolep>=1",    "N_{vlep} #geq 1")),
//                                                              make_pair("nvlep0",      make_pair("nvetolep==0",    "N_{vlep} = 0")) };
//
//  vector <pair <TString, pair <TString, TString> > > binning;
//  for(unsigned int i = 0; i < bins.size(); ++i){
//    binning.push_back(bins[0]);
//    for(unsigned int j = 1; j <= i; ++j){
//      binning.back().first         += "_"    + bins[j].first;
//      binning.back().second.first  += " && " + bins[j].second.first;
//      binning.back().second.second += ", "   + bins[j].second.second;
//    }
//  }
//  for(unsigned int i = 0; i < binning.size(); ++i){
//    cout << binning[i].first         << endl;
//    cout << binning[i].second.first  << endl;
//    cout << binning[i].second.second << endl;
//    cout << "**********************************************************************************************************" << endl;
//  }

  vector<double> dphi_bins = { 0., 0.5, 3.14 };

  HistogramGetter* met_shape     = new HistogramGetter("met",             "met",                                       "#slash{#it{E}}_{T} [GeV]",                       20, 200., 1000.);
  //HistogramGetter* dphi12_shape  = new HistogramGetter("dphij12met",      "min(dphij1met, dphij2met)",                 "min(#Delta#phi(j_{1,2}, #slash{#it{E}}_{T}))",   32,   0., 3.2);
  //HistogramGetter* dphi123_shape = new HistogramGetter("dphij123met",     "min(dphij1met, min(dphij2met, dphij3met))", "min(#Delta#phi(j_{1,2,3}, #slash{#it{E}}_{T}))", 32,   0., 3.2);
  //HistogramGetter* dphi12_var    = new HistogramGetter("dphij12met_var",  "min(dphij1met, dphij2met)",                 "min(#Delta#phi(j_{1,2}, #slash{#it{E}}_{T}))",   dphi_bins.size() - 1, dphi_bins.data());
  //HistogramGetter* dphi123_var   = new HistogramGetter("dphij123met_var", "min(dphij1met, min(dphij2met, dphij3met))", "min(#Delta#phi(j_{1,2,3}, #slash{#it{E}}_{T}))", dphi_bins.size() - 1, dphi_bins.data());
  HistogramGetter* njet_shape    = new HistogramGetter("njets",           "njets",                                     "N_{jets}",                                       11,   -0.5,  10.5);
  HistogramGetter* j1pt_shape    = new HistogramGetter("j1pt",            "j1pt",                                      "p_{T}(j1)",                                      50,     0., 1500.);
  HistogramGetter* j2pt_shape    = new HistogramGetter("j2pt",            "j2pt",                                      "p_{T}(j2)",                                      50,     0., 1500.);
  HistogramGetter* leppt_shape   = new HistogramGetter("leptonpt",        "leptonpt",                                  "p_{T}(l)",                                       50,     0.,  200.);
  HistogramGetter* nlbjet_shape  = new HistogramGetter("nlbjets",         "nlbjets",                                   "N_{b}^{l}",                                      11,   -0.5,  10.5);
  HistogramGetter* nb_shape      = new HistogramGetter("nb",              "nbjets",                                    "N_{b}^{m}",                                       6,   -0.5,   5.5);
  HistogramGetter* nsdt_shape    = new HistogramGetter("nt",              "nsdtop",                                    "N_{t}",                                           6,   -0.5,   5.5);
  HistogramGetter* nrest_shape   = new HistogramGetter("nrt",             "nrestop",                                   "N_{restop}",                                      6,   -0.5,   5.5);
  HistogramGetter* nsdw_shape    = new HistogramGetter("nw",              "nsdw",                                      "N_{W}",                                           6,   -0.5,   5.5);
  HistogramGetter* nsv_shape     = new HistogramGetter("nsv",             "nivf",                                      "N_{SV}",                                          6,   -0.5,   5.5);
  HistogramGetter* npv_shape     = new HistogramGetter("npv",             "npv",                                       "N_{PV}",                                        121,   -0.5, 120.5);
  HistogramGetter* ptisr_shape   = new HistogramGetter("ptisr",           "ak8isrpt",                                  "p_{T}(ISR) [GeV]",                               70,   150., 1550.);
  HistogramGetter* mtb_shape     = new HistogramGetter("mtb",             "mtcsv12met",                                "M_{T}(b_{1,2},#slash{E}_{T}) [GeV]",             50,     0., 1000.);
  HistogramGetter* mtlepmet_shape= new HistogramGetter("mtlepmet",        "mtlepmet",                                  "M_{T}(l,#slash{E}_{T}) [GeV]",                   50,     0., 1000.);
  HistogramGetter* ptb_shape     = new HistogramGetter("ptb",             "csvj1pt",                                   "p_{T}(b) [GeV]",                                 50,     0., 1000.);
  HistogramGetter* ptb12_shape   = new HistogramGetter("ptb12",           "csvj1pt + csvj2pt",                         "p_{T}(b_{1}) + p_{T}(b_{2}) [GeV]",              50,     0., 1000.);
  HistogramGetter* ht_shape      = new HistogramGetter("ht",              "ht",                                        "h_{T} [GeV]",                                    60,     0., 3000.);
  HistogramGetter* mht_shape     = new HistogramGetter("mht",             "mht",                                       "mh_{T}",                                         50,     0., 1000.);
  HistogramGetter* ivf_IP2D_shape= new HistogramGetter("ivf_IP2D",        "ivf_IP2D",				       "IP(2D)",					 40,     0.,   4.0);
  HistogramGetter* ivf_svCosSVPV_shape= new HistogramGetter("ivf_svCosSVPV",        "ivf_svCosSVPV",		       "cos(SV,PV)",					 50,     0.,   1.0);
  HistogramGetter* ivf_pt_shape  = new HistogramGetter("ivf_pt",          "ivf_pt",		   		       "p_{T}^{ivf}",					 50,     0., 500.0);
  HistogramGetter* ivf_svNtrack_shape  = new HistogramGetter("ivf_svNtrack",          "ivf_svNtrack",		       "N_{SV}^{ivf}",					 11,   -0.5,  10.5);
  HistogramGetter* ivf_SIP3D_shape  = new HistogramGetter("ivf_SIP3D",          "ivf_SIP3D",		   	       "#sigma_{IP3D}",					 50,     0., 100.0);
  HistogramGetter* ivf_DRak4j1_shape  = new HistogramGetter("ivf_DRak4j1",          "ivf_DRak4j1",		       "#DeltaR(E_{ivf}, j_{1})",			 60,     0.,   6.0);
  HistogramGetter* ivf_DRak4j2_shape  = new HistogramGetter("ivf_DRak4j2",          "ivf_DRak4j2",		       "#DeltaR(E_{ivf}, j_{2})",			 60,     0.,   6.0);
  HistogramGetter* ivf_DRak4j3_shape  = new HistogramGetter("ivf_DRak4j3",          "ivf_DRak4j3",		       "#DeltaR(E_{ivf}, j_{3})",			 60,     0.,   6.0);
  HistogramGetter* ivf_DRak4j4_shape  = new HistogramGetter("ivf_DRak4j4",          "ivf_DRak4j4",		       "#DeltaR(E_{ivf}, j_{4})",			 60,     0.,   6.0);
  HistogramGetter* fj_mass_shape = new HistogramGetter("fatjetmass",      "fatjetmass",				       "m_jet",						 30,	 0.,  300.);
  HistogramGetter* fj_pt_shape   = new HistogramGetter("fatjetpt",        "fatjetpt",		 		       "p_{T}",						 50,	 0., 1000.);
  HistogramGetter* fj_tau12_shape= new HistogramGetter("fatjettau12",     "fatjettau2/fatjettau1",		       "#tau_{2}/#tau_{1}",				 20,	 0.,   2.0);
  HistogramGetter* fj_tau23_shape= new HistogramGetter("fatjettau23",     "fatjettau3/fatjettau2",		       "#tau_{3}/#tau_{2}",				 20,	 0.,   2.0);
//  HistogramGetter* drlepw_shape  = new HistogramGetter("drlepw",          "drlepw",				       "#DeltaR(l, W)",					 16,     0., 1.6);
//  HistogramGetter* drleptop_shape= new HistogramGetter("drleptop",        "drleptop",				       "#DeltaR(l, t)",					 16,     0., 1.6);
  //HistogramGetter* metht_shape   = new HistogramGetter("met_ht",          "(met + ht)",                                "#slash{#it{E}}_{T} + h_{T} [GeV]",               nHT_bins - 1, ht_bins);
  
  printStack(stackTrees, met_shape    , "baseline_CR",   dataMC_selsAndNames, "baseline_CR", "Events", true, true);
//  printStack(stackTrees, met_shape    , "baseline_CR",   dataMC_selsAndNames, passmetmht_bins, "Events", true, true);
////  printComparison(sampleTrees, met_shape    , "_baseline",   local_baseline_CR,    "LepShape", "Events", true, true);
////  printComparison(sampleTrees, dphi12_shape , "_baseline",   local_baseline,    met_bins,          "Events", true, true);
////  printComparison(sampleTrees, dphi123_shape, "_baseline",   local_baseline,    met_bins,          "Events", true, true);
////  printComparison(sampleTrees, dphi12_shape,  "_old_sel",    old_sel,           nb_binning,  met_binning, "Events", true, true);
////  printComparison(sampleTrees, dphi123_shape, "_old_sel",    old_sel,           nb_binning,  met_binning, "Events", true, true);
////  printComparison(sampleTrees, dphi12_shape , "baseline",    local_baseline,    "baseline",        "Events", true, true);
////  printComparison(sampleTrees, dphi123_shape, "baseline",    local_baseline,    "baseline",        "Events", true, true);
//  printStack(stackTrees, njet_shape     , "baseline_CR",    dataMC_selsAndNames, 		   "baseline_CR",        "Events", true, true);
//  printStack(stackTrees, j1pt_shape     , "baseline_CR",    dataMC_selsAndNames, 		   "baseline_CR",        "Events", true, true);
//  printStack(stackTrees, j2pt_shape     , "baseline_CR",    dataMC_selsAndNames, 		   "baseline_CR",        "Events", true, true);
//  printStack(stackTrees, leppt_shape    , "baseline_CR",    dataMC_selsAndNames, 		   "baseline_CR",        "Events", true, true);
//  printStack(stackTrees, nlbjet_shape   , "baseline_CR",    dataMC_selsAndNames, 		   "baseline_CR",        "Events", true, true);
//  printStack(stackTrees, nb_shape       , "baseline_CR",    dataMC_selsAndNames, 		   "baseline_CR",        "Events", true, true);
//  printStack(stackTrees, nsdt_shape     , "baseline_CR",    dataMC_selsAndNames, 		   "baseline_CR",        "Events", true, true);
//  printStack(stackTrees, nrest_shape    , "baseline_CR",    dataMC_selsAndNames, 		   "baseline_CR",        "Events", true, true);
//  printStack(stackTrees, nsdw_shape     , "baseline_CR",    dataMC_selsAndNames, 		   "baseline_CR",        "Events", true, true);
//  printStack(stackTrees, nsv_shape      , "baseline_CR",    dataMC_selsAndNames,             "baseline_CR",        "Events", true, true);
//  printStack(stackTrees, npv_shape      , "baseline_CR",    dataMC_selsAndNames,             "baseline_CR",        "Events", true, true);
////  printStack(stackTrees, drlepw_shape   , "baseline_CR",    local_baseline_CR + " && " + nt1, "baseline_CR",        "Events", true, true);
////  printStack(stackTrees, drleptop_shape , "baseline_CR",    local_baseline_CR + " && " + nt1, "baseline_CR",        "Events", true, true);
//  printStack(stackTrees, ptisr_shape    , "baseline_CR",    dataMC_selsAndNames, 		   "baseline_CR",        "Events", true, true);
////  printComparison(sampleTrees, ptisr_shape  , "baseline_SR", local_baseline_SR, "baseline_SR",     "Events", true, true);
//  printStack(stackTrees, mtb_shape      , "baseline_CR",    dataMC_selsAndNames, 		   "baseline_CR",        "Events", true, true);
////  printStack(stackTrees, ptb_shape    , "baseline_CR",    local_baseline_CR, 		   "baseline_CR",        "Events", true, true);
////  printStack(stackTrees, ptb12_shape  , "baseline_CR",    local_baseline_CR, 		   "baseline_CR",        "Events", true, true);
////  printStack(stackTrees, ht_shape       , "baseline_CR",    dataMC_selsAndNames, 		   "baseline_CR",        "Events", true, true);
////  printStack(stackTrees, mht_shape      , "baseline_CR",    dataMC_selsAndNames, 		   "baseline_CR",        "Events", true, true);
//  printStack(stackTrees, ht_shape            , "baseline_CR",    dataMC_selsAndNames, passmetmht_bins,        "Events", true, true);
//  printStack(stackTrees, mht_shape           , "baseline_CR",    dataMC_selsAndNames, passmetmht_bins,        "Events", true, true);
//  printStack(stackTrees, ivf_IP2D_shape      , "baseline_CR",    dataMC_selsAndNames, ivfDeltaR_bins,        "Events", true, true);
//  printStack(stackTrees, ivf_svCosSVPV_shape , "baseline_CR",    dataMC_selsAndNames, ivfDeltaR_bins,        "Events", true, true);
//  printStack(stackTrees, ivf_pt_shape        , "baseline_CR",    dataMC_selsAndNames, ivfDeltaR_bins,        "Events", true, true);
//  printStack(stackTrees, ivf_svNtrack_shape  , "baseline_CR",    dataMC_selsAndNames, ivfDeltaR_bins,        "Events", true, true);
//  printStack(stackTrees, ivf_SIP3D_shape     , "baseline_CR",    dataMC_selsAndNames, ivfDeltaR_bins,        "Events", true, true);
//  printStack(stackTrees, ivf_DRak4j1_shape   , "baseline_CR",    dataMC_selsAndNames, ivfDeltaR_bins,        "Events", true, true);
//  printStack(stackTrees, ivf_DRak4j2_shape   , "baseline_CR",    dataMC_selsAndNames, ivfDeltaR_bins,        "Events", true, true);
//  printStack(stackTrees, ivf_DRak4j3_shape   , "baseline_CR",    dataMC_selsAndNames, ivfDeltaR_bins,        "Events", true, true);
//  printStack(stackTrees, ivf_DRak4j4_shape   , "baseline_CR",    dataMC_selsAndNames, ivfDeltaR_bins,        "Events", true, true);
//  printStack(stackTrees, fj_mass_shape       , "baseline_CR",    dataMC_selsAndNames, 		  "baseline_CR",        "Events", true, true);
//  printStack(stackTrees, fj_pt_shape         , "baseline_CR",    dataMC_selsAndNames, 		  "baseline_CR",        "Events", true, true);
//  printStack(stackTrees, fj_tau12_shape      , "baseline_CR",    dataMC_selsAndNames, 		  "baseline_CR",        "Events", true, true);
//  printStack(stackTrees, fj_tau23_shape      , "baseline_CR",    dataMC_selsAndNames, 		  "baseline_CR",        "Events", true, true);

  colors.clear();
  colors.push_back(1);
  colors.push_back(kRed);
////  printStack(stackTrees_data, met_shape      , "baseline_CR_data_comp",    dataVData_selsAndNames,                "baseline_CR_data_comp",        "Events", true, true);
//  printStack(stackTrees_data, met_shape      , "baseline_CR_data_comp",    dataVData_selsAndNames, passmetmht_bins,     "Events", true, true);
//  printStack(stackTrees_data, njet_shape     , "baseline_CR_data_comp",    dataVData_selsAndNames, 		  "baseline_CR_data_comp",        "Events", true, true);
//  printStack(stackTrees_data, j1pt_shape     , "baseline_CR_data_comp",    dataVData_selsAndNames, 		  "baseline_CR_data_comp",        "Events", true, true);
//  printStack(stackTrees_data, j2pt_shape     , "baseline_CR_data_comp",    dataVData_selsAndNames, 		  "baseline_CR_data_comp",        "Events", true, true);
//  printStack(stackTrees_data, leppt_shape    , "baseline_CR_data_comp",    dataVData_selsAndNames, 		  "baseline_CR_data_comp",        "Events", true, true);
//  printStack(stackTrees_data, nlbjet_shape   , "baseline_CR_data_comp",    dataVData_selsAndNames, 		  "baseline_CR_data_comp",        "Events", true, true);
//  printStack(stackTrees_data, nb_shape       , "baseline_CR_data_comp",    dataVData_selsAndNames, 		  "baseline_CR_data_comp",        "Events", true, true);
//  printStack(stackTrees_data, nsdt_shape     , "baseline_CR_data_comp",    dataVData_selsAndNames, 		  "baseline_CR_data_comp",        "Events", true, true);
//  printStack(stackTrees_data, nrest_shape    , "baseline_CR_data_comp",    dataVData_selsAndNames, 		  "baseline_CR_data_comp",        "Events", true, true);
//  printStack(stackTrees_data, nsdw_shape     , "baseline_CR_data_comp",    dataVData_selsAndNames, 		  "baseline_CR_data_comp",        "Events", true, true);
//  printStack(stackTrees_data, nsv_shape      , "baseline_CR_data_comp",    dataVData_selsAndNames,                "baseline_CR_data_comp",        "Events", true, true);
//  printStack(stackTrees_data, npv_shape      , "baseline_CR_data_comp",    dataVData_selsAndNames,                "baseline_CR_data_comp",        "Events", true, true);
//  printStack(stackTrees_data, ptisr_shape    , "baseline_CR_data_comp",    dataVData_selsAndNames, 		  "baseline_CR_data_comp",        "Events", true, true);
//  printStack(stackTrees_data, mtb_shape      , "baseline_CR_data_comp",    dataVData_selsAndNames, 		  "baseline_CR_data_comp",        "Events", true, true);
////  printStack(stackTrees_data, ht_shape       , "baseline_CR_data_comp",    dataVData_selsAndNames, 		  "baseline_CR_data_comp",        "Events", true, true);
////  printStack(stackTrees_data, mht_shape      , "baseline_CR_data_comp",    dataVData_selsAndNames, 		  "baseline_CR_data_comp",        "Events", true, true);
//  printStack(stackTrees_data, ht_shape       , "baseline_CR_data_comp",    dataVData_selsAndNames, passmetmht_bins,	        "Events", true, true);
//  printStack(stackTrees_data, mht_shape      , "baseline_CR_data_comp",    dataVData_selsAndNames, passmetmht_bins,	        "Events", true, true);
//
//  printStack(stackTrees_data, ivf_IP2D_shape      , "baseline_CR_data_comp",    dataVData_selsAndNames, ivfDeltaR_bins,        "Events", true, true);
//  printStack(stackTrees_data, ivf_svCosSVPV_shape , "baseline_CR_data_comp",    dataVData_selsAndNames, ivfDeltaR_bins,        "Events", true, true);
//  printStack(stackTrees_data, ivf_pt_shape        , "baseline_CR_data_comp",    dataVData_selsAndNames, ivfDeltaR_bins,        "Events", true, true);
//  printStack(stackTrees_data, ivf_svNtrack_shape  , "baseline_CR_data_comp",    dataVData_selsAndNames, ivfDeltaR_bins,        "Events", true, true);
//  printStack(stackTrees_data, ivf_SIP3D_shape     , "baseline_CR_data_comp",    dataVData_selsAndNames, ivfDeltaR_bins,        "Events", true, true);
//  printStack(stackTrees_data, ivf_DRak4j1_shape   , "baseline_CR_data_comp",    dataVData_selsAndNames, ivfDeltaR_bins,        "Events", true, true);
//  printStack(stackTrees_data, ivf_DRak4j2_shape   , "baseline_CR_data_comp",    dataVData_selsAndNames, ivfDeltaR_bins,        "Events", true, true);
//  printStack(stackTrees_data, ivf_DRak4j3_shape   , "baseline_CR_data_comp",    dataVData_selsAndNames, ivfDeltaR_bins,        "Events", true, true);
//  printStack(stackTrees_data, ivf_DRak4j4_shape   , "baseline_CR_data_comp",    dataVData_selsAndNames, ivfDeltaR_bins,        "Events", true, true);
//  printStack(stackTrees_data, fj_mass_shape       , "baseline_CR_data_comp",    dataVData_selsAndNames, 		  "baseline_CR_data_comp",        "Events", true, true);
//  printStack(stackTrees_data, fj_pt_shape         , "baseline_CR_data_comp",    dataVData_selsAndNames, 		  "baseline_CR_data_comp",        "Events", true, true);
//  printStack(stackTrees_data, fj_tau12_shape      , "baseline_CR_data_comp",    dataVData_selsAndNames, 		  "baseline_CR_data_comp",        "Events", true, true);
//  printStack(stackTrees_data, fj_tau23_shape      , "baseline_CR_data_comp",    dataVData_selsAndNames, 		  "baseline_CR_data_comp",        "Events", true, true);


//  printComparison(sampleTrees, dphi12_shape,  "baseline_ht",       local_baseline_ht, "baseline",    "Events", true, true);
//  printComparison(sampleTrees, dphi123_shape, "baseline_ht",       local_baseline_ht, "baseline",    "Events", true, true);
//  printStack(stackTrees_jetht, dphi12_shape,  "baseline_ht_stack", local_baseline_ht, "baseline",    "Events", true, true);
//  printStack(stackTrees_jetht, dphi123_shape, "baseline_ht_stack", local_baseline_ht, "baseline",    "Events", true, true);

//  printComparison(sampleTrees, dphi12_shape,  "_baseline",       local_baseline, genht_binning, "Events", true, true);
//  printComparison(sampleTrees, dphi123_shape, "_baseline",       local_baseline, genht_binning, "Events", true, true);
//  printStack(stackTrees,       dphi12_shape,  "_baseline_stack", local_baseline, genht_binning, "Events", true, false);
//  printStack(stackTrees,       dphi123_shape, "_baseline_stack", local_baseline, genht_binning, "Events", true, false);

//  printComparison(sampleTrees, dphi12_var,  "baseline",        local_baseline_200 + " && met < 250", "baseline", "Events", true);
//  printComparison(sampleTrees, dphi123_var, "baseline",        local_baseline_200 + " && met < 250", "baseline", "Events", true);
//  printStack(stackTrees,       dphi12_var,  "baseline_stack",  local_baseline_200 + " && met < 250", "baseline", "Events", true, false);
//  printStack(stackTrees,       dphi123_var, "baseline_stack",  local_baseline_200 + " && met < 250", "baseline", "Events", true, false);
//  printComparison(sampleTrees, dphi12_var,  "_baseline",       local_baseline_200 + " && met < 250", ht_binning, "Events", true);
//  printComparison(sampleTrees, dphi123_var, "_baseline",       local_baseline_200 + " && met < 250", ht_binning, "Events", true);
//  printStack(stackTrees,       dphi12_var,  "_baseline_stack", local_baseline_200 + " && met < 250", ht_binning, "Events", true);
//  printStack(stackTrees,       dphi123_var, "_baseline_stack", local_baseline_200 + " && met < 250", ht_binning, "Events", true);

//  printComparison(sampleTrees, ht_shape, "baseline_met_lt_250", local_baseline_200 + " && met < 250", "baseline", "Events", true, true);
//  printStack(stackTrees,       ht_shape, "baseline_met_lt_250", local_baseline_200 + " && met < 250", "baseline", "Events", true, true);
}

void LLTransferFactor(){
  outputDir = "TFactor";
  gSystem->mkdir(outputDir, true);
//  TString local_baseline     = "met>250 && njets>=2 && (passmetfilters || process==10) && j1chEnFrac>0.1 && j1chEnFrac<0.99 && ak8isrpt>300 && dphiisrmet>2 && nsdtop==0 && nsdw==0 && nrestop==0 && metovsqrtht>10";
//  TString local_baseline_200 = "met>200 && njets>=2 && (passmetfilters || process==10) && j1chEnFrac>0.1 && j1chEnFrac<0.99 && ak8isrpt>300 && dphiisrmet>2 && nsdtop==0 && nsdw==0 && nrestop==0 && metovsqrtht>10";
//  TString local_baseline_CR  = local_baseline + dphi_invert;
//  TString local_baseline_SR  = local_baseline + "&& dphij1met>0.5 && dphij2met>0.15";
  TString local_baseline     = "met>250 && mht > 120 && njets>=2  && (passmetfilters || process==10) && j1chEnFrac>0.1 && j1chEnFrac<0.99";
  TString local_baseline2017 = "met>250 && mht > 120 && njets>=2  && (passmetfilters2017 || process==10) && j1chEnFrac>0.1 && j1chEnFrac<0.99";
  TString local_baseline_200 = "met>200 && ht>250 && njets>=2  && (passmetfilters || process==10) && j1chEnFrac>0.1 && j1chEnFrac<0.99";
  TString local_baseline_CR2017 = local_baseline2017 + lepcrsel + " && dphij1met>0.5 && dphij2met>0.15";
  TString local_baseline_CR     = local_baseline + lepcrsel + " && dphij1met>0.5 && dphij2met>0.15";
  TString local_baseline_SR  = local_baseline + "&& dphij1met>0.5 && dphij2met>0.15";
  TString ivfDeltaR = "ivf_DRak4j1 > 0.4";
//  TString ivfDeltaR = "max(max(ivf_DRak4j1, ivf_DRak4j2),max(ivf_DRak4j3, ivf_DRak4j4)) > 0.4";
//  TString local_baseline_ht  = "met >= 200 && met < 250 && ht > 1000 && njets>=5 && (passtright || ismc) && j1chEnFrac>0.1 && j1chEnFrac<0.99";
//  TString old_sel            = "passjson && (passmetfilters || process == 10) && met >= 250 && (passmetmht || ismc) && njl>=1 && njets>=5 && dphij1lmet>2. && j1lpt>=250 && met/sqrt(ht)>=10 && j1chEnFrac>0.1 && j1chEnFrac<0.99";
  
  vector <pair <TTree*, TString> > stackTrees;
  stackTrees.emplace_back(getTree(originalTreeDir + "/../plots_17_10_18_data/met_tree.root"),                        "Data 2017");            colors.push_back(1);
  stackTrees.emplace_back(getTree(originalTreeDir + "/ttbar_tree.root"),    					     "ttbar");                colors.push_back(color_ttbar);
  stackTrees.emplace_back(getTree(originalTreeDir + "/wjets_tree.root"),    					     "wjets");                colors.push_back(color_wjets);
  stackTrees.emplace_back(getTree(originalTreeDir + "/znunu_tree.root"),    					     "znunu");                colors.push_back(kMagenta);
  stackTrees.emplace_back(getTree(originalTreeDir + "/ttOther_tree.root"),  					     "other");                colors.push_back(color_tW);
  stackTrees.emplace_back(getTree(originalTreeDir + "/qcd_orig_tree.root"), 					     "With orig. QCD MC");    colors.push_back(color_qcd);
  
  vector <pair <TString, TString> > dataMC_selsAndNames = {
         								   make_pair(local_baseline_CR2017,                local_wgtvar),
									   make_pair(local_baseline_CR    ,                local_wgtvar),
         								   make_pair(local_baseline_CR    ,                local_wgtvar),
         								   make_pair(local_baseline_CR    ,                local_wgtvar),
         								   make_pair(local_baseline_CR    ,                local_wgtvar),
         								   make_pair(local_baseline_CR    ,                local_wgtvar),
							}; 
  vector <pair <TString, pair <TString, TString> > > multiplicity_bins = { make_pair("nj_2_to_5",          make_pair("njets >= 2 && njets <= 5", "2 #leq N_{jets} #leq 5")), 
                                                                           make_pair("nj_geq_6",           make_pair("njets >= 6",               "N_{jets} #geq 6")), 
                                                                           make_pair("nj_geq_7",           make_pair("njets >= 7",               "N_{jets} #geq 7")), 
                                                                           make_pair("nb_0",               make_pair("nbjets == 0",              "N_{b} = 0")), 
                                                                           make_pair("nb_1",               make_pair("nbjets == 1",              "N_{b} = 1")), 
                                                                           make_pair("nb_geq_2",           make_pair("nbjets >= 2",              "N_{b} #geq 2")), 
                                                                           make_pair("nsv_0",              make_pair("nivf == 0",                "N_{SV} = 0")), 
                                                                           make_pair("nsv_geq_1",          make_pair("nivf >= 1",                "N_{SV} #geq 1")), 
                                                                           make_pair("nt_0",               make_pair("nsdtop == 0",              "N_{t} = 0")), 
                                                                           make_pair("nt_1",               make_pair("nsdtop == 1",              "N_{t} = 1")), 
                                                                           make_pair("nt_geq_2",           make_pair("nsdtop >= 2",              "N_{t} #geq 2")), 
                                                                           make_pair("nw_0",               make_pair("nsdw == 0",                "N_{W} = 0")), 
                                                                           make_pair("nw_1",               make_pair("nsdw == 1",                "N_{W} = 1")), 
                                                                           make_pair("nw_geq_2",           make_pair("nsdw >= 2",                "N_{W} #geq 2")), 
                                                                           make_pair("nrt_0",              make_pair("nrestop == 0",             "N_{res} = 0")), 
                                                                           make_pair("nrt_1",              make_pair("nrestop == 1",             "N_{res} = 1")), 
                                                                           make_pair("nrt_geq_2",          make_pair("nrestop >= 2",             "N_{res} #geq 2")),
                                                                         };
  vector <pair <TString, pair <TString, TString> > >           SV_bins = { make_pair("nsv_0",              make_pair("nivf == 0",                "N_{SV} = 0")),
									   make_pair("nsv_geq_1",          make_pair("nivf >= 1",                "N_{SV} #geq 1"))
									 };
  vector <pair <TString, pair <TString, TString> > >          jet_bins = { make_pair("nj_2_to_5",          make_pair("njets >= 2 && njets <= 5", "2 #leq N_{jets} #leq 5")),
                                                                           make_pair("nj_geq_6",           make_pair("njets >= 6",               "N_{jets} #geq 6")) 
									 };
  vector <pair <TString, pair <TString, TString> > >          met_bins = { make_pair("met_450_to_550",     make_pair("met >= 450 && met < 550",  "450 #leq #slash{#it{E}}_{T} #leq 550")), 
                                                                           make_pair("met_550_to_650",     make_pair("met >= 550 && met < 650",  "550 #leq #slash{#it{E}}_{T} #leq 650")), 
                                                                           make_pair("met_650_to_750",     make_pair("met >= 650 && met < 750",  "650 #leq #slash{#it{E}}_{T} #leq 750")), 
                                                                           make_pair("met_ge_750",         make_pair("met >= 750",               "#slash{#it{E}}_{T} #geq 750")) };
  vector <pair <TString, pair <TString, TString> > >        nb_binning = { make_pair("nb_eq_0",            make_pair("nbjets == 0",              "n_{b} = 0")),
                                                                           make_pair("nb_eq_1",            make_pair("nbjets == 1",              "n_{b} = 1")),
                                                                           make_pair("nb_ge_2",            make_pair("nbjets >= 2",              "n_{b} #geq 2")) };
  vector <pair <TString, pair <TString, TString> > >      dphi_binning = { make_pair("CR_123",             make_pair(dphi_invert,                                                        "CR")),
                                                                           make_pair("SR_12",              make_pair("dphij1met >= 0.5 && dphij2met>=0.15",                              "SR (low mass)")),
                                                                           make_pair("SR_1234",            make_pair("dphij1met>0.5 && dphij2met>0.5 && dphij3met>0.5 && dphij4met>0.5", "SR (high mass)"))};
  vector <pair <TString, pair <TString, TString> > >       met_binning = { make_pair("met_250_to_400",     make_pair("met >= 250 && met < 400", "250 #leq #slash{#it{E}}_{T} < 400")),
                                                                           make_pair("met_ge_400",         make_pair("met >= 400",              "#slash{#it{E}}_{T} #geq 400")) };
  
  HistogramGetter* met_shape     = new HistogramGetter("met",             "met",                                       "#slash{#it{E}}_{T} [GeV]",                       20, 200., 1000.);
 
  vector <pair <TString, pair <TString, TString> > >        lepCR_bins;
  for(auto it = lepcrCuts.cbegin(); it != lepcrCuts.cend(); ++it)
  {
     lepCR_bins.push_back(make_pair(it->first,     make_pair(it->second, it->first)));
     std::cout << it->first << " " << it->second << "\n";
  }
 
  printStack(stackTrees, met_shape    , "lep_CR",   dataMC_selsAndNames, lepCR_bins, "Events", true, true);
}

void makeLepShape_Plots(){
  outputDir = "lepShape_plots";
  gSystem->mkdir(outputDir, true);
  TTree* lepShape_wjetsTree = getTree(originalTreeDir + "/ttbarplusw_tree.root");
//  TTree* lepShape_wjetsTree = getTree(originalTreeDir + "/met_tree.root");
  TString lepShape_wjetsLabel = "Lepton Shape";

  TString METPresel         = "passjson && passmetmht && (passmetfilters || process==10) && (met >= 250) && (j1chEnFrac > 0.1) && (j1chEnFrac < 0.99)";
  TString BaselineExtraCuts = "((njets >= 5) && (nlbjets>= 2) && (nbjets>=1))";
  TString oneLep            = "(nvetolep > 0 && mtlepmet < 100)";
  TString elecSel           = "(ngoodgenele == 1) && (leptonpdgid == 11) && (nvetotau == 0)";
  TString muSel             = "(ngoodgenmu  == 1) && (leptonpdgid == 13) && (nvetotau == 0)";
  TString tauElecSel        = "(ngoodgenele == 1) && (leptonpdgid == 11)";
  TString tauMuSel          = "(ngoodgenmu  == 1) && (leptonpdgid == 13)";
  TString negelecSel        = "(ngoodgenele == 0)";
  TString negmuSel          = "(ngoodgenmu  == 0)";
  TString tauSel            = "(npromptgentau == 1)";
  
  TString lepShape_baseline = METPresel + " && " + BaselineExtraCuts + " && " + oneLep;
  //TString lepShape_baseline = TString::Format(METPresel + " && " + BaselineExtraCuts + " && " + oneLep);
  //TString lepShape_baseline = TString::Format("%s && %s && %s", METPresel, BaselineExtraCuts, oneLep);

  vector<HistogramGetter *> met_HG; 
  met_HG.push_back(new HistogramGetter("met",             "met",                                       "#slash{#it{E}}_{T} [GeV]",                       20, 200., 1000.));
  
  vector <pair <pair <TString, TString>, pair <TString, TString> > > lepShape_selsAndNames = {
         make_pair(make_pair(elecSel,                                          local_wgtvar),    make_pair("Lep Electron",  "elec")),
         make_pair(make_pair(muSel,                                            local_wgtvar),    make_pair("Lep Muon",      "mu")),
         make_pair(make_pair(tauSel,                                           local_wgtvar),    make_pair("Lep Tau Total", "tau")),
												}; 
  
  vector <pair <pair <TString, TString>, pair <TString, TString> > > tauShape_selsAndNames = {
         make_pair(make_pair(tauSel + " && " + negelecSel + " && " + negmuSel, local_wgtvar),    make_pair("Lep Tau Hadr",  "tau")),
         make_pair(make_pair(tauSel + " && " + tauMuSel,                       local_wgtvar),    make_pair("Lep Tau Muon",  "tau")),
         make_pair(make_pair(tauSel + " && " + tauElecSel,                     local_wgtvar),    make_pair("Lep Tau Elec",  "tau")),
												}; 
  colors.push_back(kGreen);
  colors.push_back(kRed);
  colors.push_back(kAzure + 10);

  vector <pair <pair <TString, TString>, pair <TString, TString> > > lepShape_localSels = {
         make_pair(make_pair("(leptonpt >= 5 && (leptoneta <= 2.4 && leptoneta >= -2.4))", "1.0"),               make_pair("in_acceptance",       "in_acceptance")),
         make_pair(make_pair("(leptonpt <= 5 || (leptoneta > 2.4  || leptoneta < -2.4))",  "1.0"),              make_pair("out_acceptance",      "out_acceptance")),
                                                                                               };

  printLepPlots(lepShape_wjetsTree, met_HG, "LepShape_test", lepShape_baseline, lepShape_localSels, lepShape_selsAndNames, true, true, true);
  colors.clear();
  colors.push_back(color_qcd);
  colors.push_back(color_tW);
  colors.push_back(kBlue);
  printLepPlots(lepShape_wjetsTree, met_HG, "TauShape_test", lepShape_baseline, lepShape_localSels, tauShape_selsAndNames, true, true, true);
}

void makeJetLepStuct_Plots(){
  outputDir = "lepShape_plots";
  gSystem->mkdir(outputDir, true);
  TTree* lepShape_wjetsTree = getTree(originalTreeDir + "/ttbarplusw_tree.root");
  TString lepShape_wjetsLabel = "Lepton Shape";

  TString METPresel         = "passjson && passmetmht && (passmetfilters || process==10) && (met >= 250) && (j1chEnFrac > 0.1) && (j1chEnFrac < 0.99)";
  TString BaselineExtraCuts = "((njets >= 5) && (nlbjets>= 2) && (nbjets>=1))";
  TString oneLep            = "(nvetolep > 0 && mtlepmet < 100)";
  TString zeroLep           = "(nvetolep==0 && nvetotau==0)";
  TString inAcceptance      = "(leptonpt >= 5 && (leptoneta <= 2.4 && leptoneta >= -2.4))";
  TString outAcceptance     = "(leptonpt <= 5 || (leptoneta > 2.4  || leptoneta < -2.4))";
  TString elecSel           = "(ngoodgenele == 1) && (leptonpdgid == 11) && (nvetotau == 0)";
  TString muSel             = "(ngoodgenmu  == 1) && (leptonpdgid == 13) && (nvetotau == 0)";
  TString tauElecSel        = "(ngoodgenele == 1) && (leptonpdgid == 11)";
  TString tauMuSel          = "(ngoodgenmu  == 1) && (leptonpdgid == 13)";
  TString negelecSel        = "(ngoodgenele == 0)";
  TString negmuSel          = "(ngoodgenmu  == 0)";
  TString tauSel            = "(npromptgentau == 1)";
  
  //TString lepShape_baseline = TString::Format("passjson && passmetmht && (passmetfilters || process==10) && (met >= 250) && (j1chEnFrac > 0.1) && (j1chEnFrac < 0.99) && ((njets >= 5) && (nlbjets>= 2) && (nbjets>=1)) && (nvetolep > 0) && (mtlepmet < 100)");
  TString lepShape_baseline = METPresel + " && " + BaselineExtraCuts + " && " + oneLep;

  vector<HistogramGetter *> met_HG; 
  met_HG.push_back(new HistogramGetter("met",             "met",                                       "#slash{#it{E}}_{T} [GeV]",                       20, 200., 1000.));
  met_HG.push_back(new HistogramGetter("njets",           "njets",                                     "N_{jets}",                                       10,   .5, 10.5));
  met_HG.push_back(new HistogramGetter("nb",              "nbjets",                                    "N_{b}",                                          5,    .5, 5.5));
  met_HG.push_back(new HistogramGetter("mtb",             "mtcsv12met",                                "M_{T}(b_{1,2},#slash{E}_{T}) [GeV]",             50,   0., 1000.)); 
  
  vector <pair <pair <TString, TString>, pair <TString, TString> > > lepShape_selsAndNames = {
         make_pair(make_pair(elecSel,                                          local_wgtvar),    make_pair("Lep Electron",  "elec")),
         make_pair(make_pair(muSel,                                            local_wgtvar),    make_pair("Lep Muon",      "mu")),
         make_pair(make_pair(tauSel,                                           local_wgtvar),    make_pair("Lep Tau Total", "tau")),
												}; 
  
  vector <pair <pair <TString, TString>, pair <TString, TString> > > tauShape_selsAndNames = {
         make_pair(make_pair(tauSel + " && " + negelecSel + " && " + negmuSel, local_wgtvar),    make_pair("Lep Tau Hadr",  "tau")),
         make_pair(make_pair(tauSel + " && " + tauMuSel,                       local_wgtvar),    make_pair("Lep Tau Muon",  "tau")),
         make_pair(make_pair(tauSel + " && " + tauElecSel,                     local_wgtvar),    make_pair("Lep Tau Elec",  "tau")),
												}; 
  colors.push_back(kGreen);
  colors.push_back(kRed);
  colors.push_back(kAzure + 10);

  vector <pair <pair <TString, TString>, pair <TString, TString> > > lepShape_localSels = {
         make_pair(make_pair(oneLep + " && " + inAcceptance, "1.0"),                make_pair("in_acceptance",       "in_acceptance")),
         make_pair(make_pair(oneLep + " && " + outAcceptance,  "1.0"),              make_pair("out_acceptance",      "out_acceptance")),
                                                                                               };

  printLepPlots(lepShape_wjetsTree, met_HG, "LepShape", lepShape_baseline, lepShape_localSels, lepShape_selsAndNames, true, true, true);
  colors.clear();
  colors.push_back(color_qcd);
  colors.push_back(color_tW);
  colors.push_back(kBlue);
  printLepPlots(lepShape_wjetsTree, met_HG, "TauShape", lepShape_baseline, lepShape_localSels, tauShape_selsAndNames, true, true, true);
}

void makeLLBPrediction(){
  outputDir = "LLBPrediction";
  gSystem->mkdir(outputDir, true);
  vector <pair <TTree*, TString> > stackTrees;
  stackTrees.emplace_back(getTree(originalTreeDir + "/../plots_17_10_18_data/met_tree.root"),                        "Data 2017");            colors.push_back(1);
  stackTrees.emplace_back(getTree(originalTreeDir + "/ttbar_tree.root"),    					     "ttbar");                colors.push_back(color_ttbar);
  stackTrees.emplace_back(getTree(originalTreeDir + "/wjets_tree.root"),    					     "wjets");                colors.push_back(color_wjets);
  stackTrees.emplace_back(getTree(originalTreeDir + "/tw_tree.root"),    					     "tW");                   colors.push_back(color_tW);
  stackTrees.emplace_back(getTree(originalTreeDir + "/ttW_tree.root"),  					     "ttW");                  colors.push_back(kMagenta);
  TString lepShape_wjetsLabel = "Lepton Shape";

  TString LLBcr = datasel + trigSR + revert_vetoes;

  vector<HistogramGetter *> met_HG; 
  met_HG.push_back(new HistogramGetter("met",             "met",                                       "#slash{#it{E}}_{T} [GeV]",                       20, 200., 1000.));
  
  //printLepPlots(lepShape_wjetsTree, met_HG, "LepShape", lepShape_baseline, lepShape_localSels, lepShape_selsAndNames, true, true, true);
}

TTree* getTree(TString filename){
  TFile* sf = new TFile(filename,"read");
  TTree* st = 0;
  sf->GetObject("Events",st);
  return st;
}

float getMinimum(TH1F* p_Hist){
  float min = p_Hist->GetMinimum(0);
  float temp_min = 99999.;
//  for(int iB = 0; iB <= p_Hist->GetNbinsX(); iB++) if((temp_min > p_Hist->GetBinContent(iB)) && (0 < p_Hist->GetBinContent(iB)) ) temp_min = p_Hist->GetBinContent(iB);
//  cout << "min = " << min << ", temp_min = " << temp_min << endl;
  return min;
}

float getMaximum(TH1F* p_Hist){
  float max = p_Hist->GetMaximum();
  max = 0.;
  for(int iB = 2; iB < p_Hist->GetNbinsX(); iB++) if(max <= p_Hist->GetBinContent(iB)) max = p_Hist->GetBinContent(iB);
  return max;
}

void setTitleOffset(TCanvas *c, double xOff = .950, double yOff = 1.400){
  TList * list = c->GetListOfPrimitives();
  for(unsigned int iP = 0; iP < list->GetSize(); ++iP){
    TH1 * h = dynamic_cast<TH1*>(list->At(iP));
    if(h == 0) continue;
   h->GetXaxis()->SetTitleOffset(xOff);
    h->GetYaxis()->SetTitleOffset(yOff);
  }
}

void drawPlot(Plot* p_Plot, float yMin, float yMax, bool p_drawRatio, bool p_drawStack){
  TCanvas* canvas = new TCanvas;
  p_Plot->setDrawCMSLumi(getLumi() + " fb^{-1}");
  p_Plot->setYRange(yMin, yMax);
  p_Plot->setLegend(.45, .65, .92, .9);
  canvas->cd();
  if(p_drawRatio){
    if(!p_drawStack){
      p_Plot->drawRatios(canvas);
    } else {
//      p_Plot->setUsePoisson();
      p_Plot->drawRatioStack(canvas);
    }
  } else {
    if(p_drawStack){
      p_Plot->setUsePoisson();
    }
    p_Plot->draw(canvas);
  }
  setTitleOffset(canvas);
  canvas->SaveAs(outputDir + "/" + p_Plot->getName() + TString(".pdf"));
}

void printComparison(vector <pair <TTree*, TString> >& p_SampleTrees, HistogramGetter* p_HistG, TString p_Suffix, TString p_Sel, TString p_Desc, TString p_yTitle, bool p_drawRatio, bool p_logY, bool p_normalize){
  float yMin = 99999., yMax = 0.;
  if(p_normalize) p_yTitle = "Arbitrary units";
  Plot* plot = new Plot(p_HistG->plotInfo->name + (p_HistG->plotInfo->name.EndsWith("_") ? "" : "_") + p_Suffix + (p_normalize ? "_norm" : "_noNorm"), p_Desc, p_HistG->plotInfo->xTitle, p_yTitle);
cout << p_Sel << endl;
  for(int iS = 0; iS < p_SampleTrees.size(); iS++){
    TString temp_PlotName = p_HistG->plotInfo->name;
    if(p_SampleTrees[iS].second.Contains("Smeared")) p_HistG->setNBS(50);
cout << p_HistG->plotInfo->name << endl;
    TH1F* hist = p_HistG->getHistogram(p_SampleTrees[iS].first, p_Sel, local_wgtvar, TString::Format("QCD_%s_%u", p_Suffix.Data(), iS));
    if(p_normalize) normalize(hist);
//    if( (yMin > getMinimum(hist)) && (0 > getMinimum(hist)) ) yMin = getMinimum(hist);
    if(yMin > getMinimum(hist)) yMin = getMinimum(hist);
    if(yMax < getMaximum(hist)) yMax = getMaximum(hist);
    plot->addHist(hist, p_SampleTrees[iS].second, "E1", StyleTools::colorGetter(iS), 0, StyleTools::colorGetter(iS));
    p_HistG->setNBS(0);
    p_HistG->plotInfo->name = temp_PlotName;
  }
  if(p_logY){
    plot->setLogy();
    drawPlot(plot, 0.1 * yMin, 100 * yMax, p_drawRatio);
  } else {
    drawPlot(plot, 0.,           3 * yMax, p_drawRatio);
  }
}

void printComparison(vector <pair <TTree*, TString> >& p_SampleTrees, HistogramGetter* p_HistG, TString p_Suffix, TString p_Sel, vector <pair <TString, pair <TString, TString> > >& p_BinInfo, TString p_yTitle, bool p_drawRatio, bool p_logY, bool p_normalize){
  for(int iB = 0; iB < p_BinInfo.size(); iB++){
    TString sel = "";
    if(p_Sel.Sizeof() > 1){
      sel = TString::Format("%s && %s", p_Sel.Data(), p_BinInfo[iB].second.first.Data());
    } else {
      sel = p_BinInfo[iB].second.first;
    }
cout << p_BinInfo[iB].first + p_Suffix << endl;
    printComparison(p_SampleTrees, p_HistG, p_BinInfo[iB].first + p_Suffix, sel, p_BinInfo[iB].second.second, p_yTitle, p_drawRatio, p_logY, p_normalize);
  }
}

void printComparison(vector <pair <TTree*, TString> >& p_SampleTrees, HistogramGetter* p_HistG, TString p_Suffix, TString p_Sel, vector <pair <TString, pair <TString, TString> > >& p_BinInfo_1, vector <pair <TString, pair <TString, TString> > >& p_BinInfo_2, TString p_yTitle, bool p_drawRatio, bool p_logY, bool p_normalize){
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
      printComparison(p_SampleTrees, p_HistG, suf + p_Suffix, sel, dsc, p_yTitle, p_drawRatio, p_logY, p_normalize);
    }
  }
}

void printStack(vector <pair <TTree*, TString> >& p_StackTrees, HistogramGetter* p_HistG, TString p_Suffix, TString p_Sel, TString p_Desc, TString p_yTitle, bool p_drawRatio, bool p_logY){
  Plot* plot = new Plot(p_HistG->plotInfo->name + (p_HistG->plotInfo->name.EndsWith("_") ? "" : "_") + p_Suffix, p_Desc, p_HistG->plotInfo->xTitle, p_yTitle);
  float yMin = 99999., yMax = 0.;
  float totMCYield = 0;
  TH1F* nonQCD_hist;
  TH1F* nonSame_hist;
  TH1F* data_hist;
  bool isDataComp = false;
  float dataInt = 0.;
  for(unsigned int iS = 0; iS < p_StackTrees.size(); ++iS){
    if(p_StackTrees[iS].second == "Data 2017"){
cout << local_stdwgt << "*(" << p_Sel << ")" << endl;
      TH1F* hist = p_HistG->getHistogram(p_StackTrees[iS].first, p_Sel, local_stdwgt, TString::Format("data_%s_%u", p_Suffix.Data(), iS));
      plot->addHist(hist, p_StackTrees[iS].second, "E0", colors[iS], 0, colors[iS], 1, 0, 1);
//      if( (yMin > getMinimum(hist)) && (0 > getMinimum(hist)) ) yMin = getMinimum(hist);
      if(yMin > getMinimum(hist))   yMin = getMinimum(hist);
      if(yMax < hist->GetMaximum()) yMax = hist->GetMaximum();
      TH1F* data_hist = (TH1F*) hist->Clone();
      dataInt = data_hist->Integral(0, data_hist->GetNbinsX() + 1);
    }
    else if(p_StackTrees[iS].second == "Data 2016"){
cout << local_stdwgt << "*(" << p_Sel << ")" << endl;
      isDataComp = true;
      TH1F* hist = p_HistG->getHistogram(p_StackTrees[iS].first, p_Sel, local_stdwgt, TString::Format("met_%s_%u", p_Suffix.Data(), iS));
      hist->Scale(dataInt / hist->Integral(0, hist->GetNbinsX() + 1));
      plot->addHist(hist, p_StackTrees[iS].second, "hist", colors[iS], 0, colors[iS]);
      //plot->addToStack(hist, p_StackTrees[iS].second, colors[iS], 1001, 1, 1, 3);
    }
    else if (p_StackTrees[iS].second == "Non-QCD bkg" || p_StackTrees[iS].second == "ttbar" || p_StackTrees[iS].second == "wjets" || p_StackTrees[iS].second == "znunu" || p_StackTrees[iS].second == "other"){
cout << local_wgtvar << "*(" << p_Sel << ")" << endl;
      nonQCD_hist = p_HistG->getHistogram(p_StackTrees[iS].first, p_Sel, local_wgtvar, TString::Format("nonQCD_%s_%u", p_Suffix.Data(), iS));
      plot->addToStack(nonQCD_hist, p_StackTrees[iS].second, colors[iS], 1001, 1, 1, 3);
      if(p_StackTrees[iS].second == "ttbar") nonSame_hist = (TH1F*) nonQCD_hist->Clone();
      else nonSame_hist->Add(nonQCD_hist);
    }
    else if(p_StackTrees[iS].second == "With orig. QCD MC"){
cout << local_wgtvar << "*(" << p_Sel << ")" << endl;
      TH1F* hist = p_HistG->getHistogram(p_StackTrees[iS].first, p_Sel, local_wgtvar, TString::Format("origQCD_%s_%u", p_Suffix.Data(), iS));
      //hist->Add(nonQCD_hist);
      hist->Add(nonSame_hist);
      //plot->addHistForRatio(hist, p_StackTrees[iS].second, "hist", colors[iS], 0, colors[iS]);
      plot->addToStack(hist, p_StackTrees[iS].second, colors[iS], 1001, 1, 1, 3);
    }
    else if (p_StackTrees[iS].second.Contains("Delta")){
cout << local_wgtvar << "*(" << p_Sel << ")" << endl;
      TH1F* hist = p_HistG->getHistogram(p_StackTrees[iS].first, p_Sel, local_wgtvar, TString::Format("signal_%s_%u", p_Suffix.Data(), iS));
      hist->Scale(totMCYield / hist->Integral(0, hist->GetNbinsX() + 1));
      plot->addHist(hist, p_StackTrees[iS].second, "hist", colors[iS], 0, colors[iS]);
    }
    else {
cout << local_wgtvar << "*(" << p_Sel << ")" << endl;
      TH1F* hist = p_HistG->getHistogram(p_StackTrees[iS].first, p_Sel, local_wgtvar, TString::Format("QCD_%s_%u", p_Suffix.Data(), iS));
      plot->addToStack(hist, p_StackTrees[iS].second, colors[iS], 1001, 1, 1, 3);
    }
  }
  if(p_logY){
    plot->setLogy();
    drawPlot(plot, 0.1 * yMin, 100 * yMax, p_drawRatio, !isDataComp);
  } else {
    drawPlot(plot, 0.,           3 * yMax, p_drawRatio, !isDataComp);
  }
}

void printStack(vector <pair <TTree*, TString> >& p_StackTrees, HistogramGetter* p_HistG, TString p_Suffix, vector <pair <TString, TString> >& p_Sel, TString p_Desc, TString p_yTitle, bool p_drawRatio, bool p_logY){
  Plot* plot = new Plot(p_HistG->plotInfo->name + (p_HistG->plotInfo->name.EndsWith("_") ? "" : "_") + p_Suffix, p_Desc, p_HistG->plotInfo->xTitle, p_yTitle);
  float yMin = 99999., yMax = 0.;
  float totMCYield = 0;
//  TH1F* nonQCD_hist;
  TH1F* nonSame_hist;
  TH1F* data_hist;
  float dataInt = 0.;
  for(unsigned int iS = 0; iS < p_StackTrees.size(); ++iS){
    if(p_StackTrees[iS].second == "Data 2017"){
cout << local_stdwgt << "*(" << p_Sel[iS].first << ")" << endl;
      TH1F* hist = p_HistG->getHistogram(p_StackTrees[iS].first, p_Sel[iS].first, local_stdwgt, TString::Format("data_%s_%u", p_Suffix.Data(), iS));
      plot->addHist(hist, p_StackTrees[iS].second, "E0", colors[iS], 0, colors[iS], 1, 0, 1);
//      if( (yMin > getMinimum(hist)) && (0 > getMinimum(hist)) ) yMin = getMinimum(hist);
      if(yMin > getMinimum(hist))   yMin = getMinimum(hist);
      if(yMax < hist->GetMaximum()) yMax = hist->GetMaximum();
      TH1F* data_hist = (TH1F*) hist->Clone();
      dataInt = data_hist->Integral(0, data_hist->GetNbinsX() + 1);
    }
    else if(p_StackTrees[iS].second == "Data 2016"){
cout << local_stdwgt << "*(" << p_Sel[iS].first << ")" << endl;
      TH1F* hist = p_HistG->getHistogram(p_StackTrees[iS].first, p_Sel[iS].first, local_stdwgt, TString::Format("met_%s_%u", p_Suffix.Data(), iS));
      hist->Scale(dataInt / hist->Integral(0, hist->GetNbinsX() + 1));
      plot->addToStack(hist, p_StackTrees[iS].second, colors[iS], 1001, 1, 1, 3);
    }
    else if (p_StackTrees[iS].second == "Non-QCD bkg" || p_StackTrees[iS].second == "ttbar" || p_StackTrees[iS].second == "wjets" || p_StackTrees[iS].second == "znunu" || p_StackTrees[iS].second == "other"){
cout << local_wgtvar << "*(" << p_Sel[iS].first << ")" << endl;
      TH1F* nonQCD_hist = p_HistG->getHistogram(p_StackTrees[iS].first, p_Sel[iS].first, local_wgtvar, TString::Format("nonQCD_%s_%u", p_Suffix.Data(), iS));
      plot->addToStack(nonQCD_hist, p_StackTrees[iS].second, colors[iS], 1001, 1, 1, 3);
      if(p_StackTrees[iS].second == "ttbar") nonSame_hist = (TH1F*) nonQCD_hist->Clone();
      else nonSame_hist->Add(nonQCD_hist);
    }
    else if(p_StackTrees[iS].second == "With orig. QCD MC"){
cout << local_wgtvar << "*(" << p_Sel[iS].first << ")" << endl;
      TH1F* hist = p_HistG->getHistogram(p_StackTrees[iS].first, p_Sel[iS].first, local_wgtvar, TString::Format("origQCD_%s_%u", p_Suffix.Data(), iS));
      hist->Add(nonSame_hist);
      plot->addToStack(hist, p_StackTrees[iS].second, colors[iS], 1001, 1, 1, 3);
    }
    else if (p_StackTrees[iS].second.Contains("Delta")){
cout << local_wgtvar << "*(" << p_Sel[iS].first << ")" << endl;
      TH1F* hist = p_HistG->getHistogram(p_StackTrees[iS].first, p_Sel[iS].first, local_wgtvar, TString::Format("signal_%s_%u", p_Suffix.Data(), iS));
      hist->Scale(totMCYield / hist->Integral(0, hist->GetNbinsX() + 1));
      plot->addHist(hist, p_StackTrees[iS].second, "hist", colors[iS], 0, colors[iS]);
    }
    else {
cout << local_wgtvar << "*(" << p_Sel[iS].first << ")" << endl;
      TH1F* hist = p_HistG->getHistogram(p_StackTrees[iS].first, p_Sel[iS].first, local_wgtvar, TString::Format("QCD_%s_%u", p_Suffix.Data(), iS));
      plot->addToStack(hist, p_StackTrees[iS].second, colors[iS], 1001, 1, 1, 3);
    }
  }
  if(p_logY){
    plot->setLogy();
    drawPlot(plot, 0.1 * yMin, 100 * yMax, p_drawRatio, true);
  } else {
    drawPlot(plot, 0.,           3 * yMax, p_drawRatio, true);
  }
}

void printStack(vector <pair <TTree*, TString> >& p_StackTrees, HistogramGetter* p_HistG, TString p_Suffix, TString p_Sel, vector <pair <TString, pair <TString, TString> > >& p_BinInfo, TString p_yTitle, bool p_drawRatio, bool p_logY){
  for(int iB = 0; iB < p_BinInfo.size(); iB++){
    TString sel = "";
    if(p_Sel.Sizeof() > 1){
      sel = TString::Format("%s && %s", p_Sel.Data(), p_BinInfo[iB].second.first.Data());
    } else {
      sel = p_BinInfo[iB].second.first;
    }
    printStack(p_StackTrees, p_HistG, p_BinInfo[iB].first + p_Suffix, sel, p_BinInfo[iB].second.second, p_yTitle, p_drawRatio, p_logY);
  }
}

void printStack(vector <pair <TTree*, TString> >& p_StackTrees, HistogramGetter* p_HistG, TString p_Suffix, vector <pair <TString, TString> >& p_Sel, vector <pair <TString, pair <TString, TString> > >& p_BinInfo, TString p_yTitle, bool p_drawRatio, bool p_logY){
  for(int iB = 0; iB < p_BinInfo.size(); iB++){
    vector <pair <TString, TString> > sel;
    TString sel_ = "";
    for(unsigned int iS = 0; iS < p_StackTrees.size(); ++iS){
      	sel_ = TString::Format("%s && %s", p_Sel[iS].first.Data(), p_BinInfo[iB].second.first.Data());
    	sel.push_back(make_pair(sel_, p_Sel[iS].second.Data()));
    }
    printStack(p_StackTrees, p_HistG, p_BinInfo[iB].first + p_Suffix, sel, p_BinInfo[iB].second.second, p_yTitle, p_drawRatio, p_logY);
  }
}

void printStack(vector <pair <TTree*, TString> >& p_StackTrees, HistogramGetter* p_HistG, TString p_Suffix, TString p_Sel, vector <pair <TString, pair <TString, TString> > >& p_BinInfo_1, vector <pair <TString, pair <TString, TString> > >& p_BinInfo_2, TString p_yTitle, bool p_drawRatio, bool p_logY){
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
      printStack(p_StackTrees, p_HistG, suf + p_Suffix, sel, dsc, p_yTitle, p_drawRatio, p_logY);
    }
  }
}

void printStack(vector <pair <TTree*, TString> >& p_StackTrees, HistogramGetter* p_HistG, TString p_Suffix, TString p_Sel, vector <pair <TString, pair <TString, TString> > >& p_BinInfo_1, vector <pair <TString, pair <TString, TString> > >& p_BinInfo_2, vector <pair <TString, pair <TString, TString> > >& p_BinInfo_3, TString p_yTitle, bool p_drawRatio, bool p_logY){
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
        printStack(p_StackTrees, p_HistG, suf + p_Suffix, sel, dsc, p_yTitle, p_drawRatio, p_logY);
      }
    }
  }
}

void printLocalPlots(TTree* p_qcdTree, vector <HistogramGetter*>& p_HistGs, TString p_Prefix, TString p_Sel, vector <pair <pair <TString, TString>, pair <TString, TString> > >& p_LocalSels, vector <pair <pair <TString, TString>, pair<TString, TString> > >& p_SelsAndNames, bool p_logY, bool p_normalize, bool p_drawRatio){
  TString yTitle = "Events";
  if(p_normalize) yTitle = "Arbitrary units";
  for(unsigned int iSR = 0; iSR < p_LocalSels.size(); ++iSR){
    for(unsigned int iH = 0; iH < p_HistGs.size(); ++iH){
      TString name = TString::Format("%s_%s_%s", p_Prefix.Data(), p_HistGs[iH]->plotInfo->name.Data(), p_LocalSels[iSR].second.second.Data());
      p_normalize ? name += "_norm" : name += "_noNorm";
cout << name << endl;
      Plot* plot = new Plot(name, p_LocalSels[iSR].second.first, p_HistGs[iH]->plotInfo->xTitle, yTitle);
      float yMin = 99999., yMax = 0.;
      for(unsigned int iP = 0; iP < p_SelsAndNames.size(); ++iP){
        TString sel    = TString::Format("%s && %s && %s", p_Sel.Data(), p_LocalSels[iSR].first.first.Data(), p_SelsAndNames[iP].first.first.Data());
        TString weight = TString::Format("%s*%s", p_LocalSels[iSR].first.second.Data(), p_SelsAndNames[iP].first.second.Data());
        TH1F* hist;
cout << weight << "*(" << sel << ")" << endl;
        if(p_HistGs[iH]->plotInfo->name.Contains("_flavor")){
          hist = modifyFlavorHist(p_HistGs[iH]->getHistogram(p_qcdTree, sel, weight, TString::Format("QCD_%s_%s_%s", p_Prefix.Data(), p_LocalSels[iSR].second.second.Data(), p_SelsAndNames[iP].second.second.Data())));
          if(p_normalize) normalize(hist);
          plot->addHist(hist, p_SelsAndNames[iP].second.first, "hist", StyleTools::colorGetter(iP), 0, StyleTools::colorGetter(iP));
        } else {
          hist = p_HistGs[iH]->getHistogram(p_qcdTree, sel, weight, TString::Format("QCD_%s_%s_%s", p_Prefix.Data(), p_LocalSels[iSR].second.second.Data(), p_SelsAndNames[iP].second.second.Data()));
          if(p_normalize) normalize(hist);
          plot->addHist(hist, p_SelsAndNames[iP].second.first, "hist", StyleTools::colorGetter(iP), 0, StyleTools::colorGetter(iP), 1, 1, 3);
        }
        if(yMin > getMinimum(hist))   yMin = getMinimum(hist);
        if(yMax < hist->GetMaximum()) yMax = hist->GetMaximum();
      }
      plot->setRatioRange(0.8, 2.);
      if(p_logY){
        plot->setLogy();
        drawPlot(plot, 0.1 * yMin, 100 * yMax, p_drawRatio);
      } else {
        drawPlot(plot, 0.,         1.5 * yMax, p_drawRatio);
      }
    }
  }
}

void printLepPlots(TTree* p_qcdTree, vector <HistogramGetter*>& p_HistGs, TString p_Prefix, TString p_Sel, vector <pair <pair <TString, TString>, pair <TString, TString> > >& p_LocalSels, vector <pair <pair <TString, TString>, pair<TString, TString> > >& p_SelsAndNames, bool p_logY, bool p_normalize, bool p_drawRatio){
  TString yTitle = "Events";
  if(p_normalize) yTitle = "Arbitrary units";
  for(unsigned int iSR = 0; iSR < p_LocalSels.size(); ++iSR){
    for(unsigned int iH = 0; iH < p_HistGs.size(); ++iH){
      TString name = TString::Format("%s_%s_%s", p_Prefix.Data(), p_HistGs[iH]->plotInfo->name.Data(), p_LocalSels[iSR].second.second.Data());
      p_normalize ? name += "_norm" : name += "_noNorm";
cout << name << endl;
      Plot* plot = new Plot(name, p_LocalSels[iSR].second.first, p_HistGs[iH]->plotInfo->xTitle, yTitle);
      float yMin = 99999., yMax = 0.;
      TH1F* hist = 0;
      for(unsigned int iP = 0; iP < p_SelsAndNames.size(); ++iP){
        TString sel    = TString::Format("%s && %s && %s", p_Sel.Data(), p_LocalSels[iSR].first.first.Data(), p_SelsAndNames[iP].first.first.Data());
        TString weight = TString::Format("%s*%s", p_LocalSels[iSR].first.second.Data(), p_SelsAndNames[iP].first.second.Data());
cout << weight << "*(" << sel << ")" << endl;
        if(p_SelsAndNames[iP].second.first == "Lep Electron" || p_SelsAndNames[iP].second.first == "Lep Muon"){
          hist = p_HistGs[iH]->getHistogram(p_qcdTree, sel, weight, TString::Format("signal_%s_%s_%s", p_Prefix.Data(), p_LocalSels[iSR].second.second.Data(), p_SelsAndNames[iP].second.second.Data()));
	  if(p_normalize) normalize(hist);
          plot->addHist(hist, p_SelsAndNames[iP].second.first, "hist", colors[iP], 0, colors[iP]);
        } else if(p_SelsAndNames[iP].second.first.Contains("Lep Tau")){
          hist = p_HistGs[iH]->getHistogram(p_qcdTree, sel, weight, TString::Format("signal_%s_%s_%s", p_Prefix.Data(), p_LocalSels[iSR].second.second.Data(), p_SelsAndNames[iP].second.second.Data()));
          if(p_normalize) normalize(hist);
          plot->addHist(hist, p_SelsAndNames[iP].second.first, "hist", colors[iP], 0, colors[iP]);
        }
        if(yMin > getMinimum(hist))   yMin = getMinimum(hist);
        if(yMax < hist->GetMaximum()) yMax = hist->GetMaximum();
      }
      plot->setRatioRange(0.5, 2.);
      if(p_logY){
        plot->setLogy();
        drawPlot(plot, 0.0001, 100 * yMax, p_drawRatio);
      } else {
        drawPlot(plot, 0.,     1.5 * yMax, p_drawRatio);
      }
    }
  }
}

TH1F* modifyFlavorHist(TH1F* p_FlavorHist){
  TH1F* flavorHist = new TH1F((TString(p_FlavorHist->GetName()) + "_rebinned").Data(), p_FlavorHist->GetTitle(), 3, 0.5, 3.5);
  double num_other = 0;
  for(int i_bin = 1; i_bin <= p_FlavorHist->GetNbinsX() + 1; i_bin++){
    switch(int(p_FlavorHist->GetBinCenter(i_bin))){
      case JetFlavorInfo::b_jet: flavorHist->SetBinContent(3, p_FlavorHist->GetBinContent(i_bin)); flavorHist->GetXaxis()->SetBinLabel(3, "b_{jets}"); break;
      case JetFlavorInfo::c_jet: flavorHist->SetBinContent(2, p_FlavorHist->GetBinContent(i_bin)); flavorHist->GetXaxis()->SetBinLabel(2, "c_{jets}"); break;
      default: num_other += p_FlavorHist->GetBinContent(i_bin); break;
    }
  }
  flavorHist->SetBinContent(1, num_other);
  flavorHist->GetXaxis()->SetBinLabel(1, "other");
  return flavorHist;
}

void printTFPlots(TTree* p_qcdTree, TString p_Prefix, TString p_Sel, vector <pair <pair <TString, TString>, pair <TString, TString> > >& p_LocalSels_CR, vector <pair <pair <TString, TString>, pair <TString, TString> > >& p_LocalSels_SR, vector <vector <double> >& p_METbins, vector <pair <pair <TString, TString>, pair<TString, TString> > >& p_SelsAndNames, bool p_logY){
  TString yTitle = "SR/CR";
  for(unsigned int iSR = 0; iSR < p_LocalSels_CR.size(); ++iSR){
    HistogramGetter* met_HG = new HistogramGetter("met", "met", "#slash{#it{E}}_{T} [GeV]",  p_METbins[iSR].size() - 1, p_METbins[iSR].data());
    met_HG->setNBS(50);
    TString name = TString::Format("%s_%s", p_Prefix.Data(), p_LocalSels_SR[iSR].second.second.Data());
cout << name << endl;
    Plot* plot = new Plot(name, p_LocalSels_SR[iSR].second.first, met_HG->plotInfo->xTitle, yTitle);
    float yMin = 99999., yMax = 0.;
    for(unsigned int iP = 0; iP < p_SelsAndNames.size(); ++iP){
      TString sel_CR = TString::Format("%s && %s && %s", p_Sel.Data(), p_LocalSels_CR[iSR].first.first.Data(), p_SelsAndNames[iP].first.first.Data());
      TString sel_SR = TString::Format("%s && %s && %s", p_Sel.Data(), p_LocalSels_SR[iSR].first.first.Data(), p_SelsAndNames[iP].first.first.Data());
      TString weight = TString::Format("%s*%s", p_LocalSels_CR[iSR].first.second.Data(), p_SelsAndNames[iP].first.second.Data());
cout << weight << "*(" << sel_CR << ")" << endl;
cout << weight << "*(" << sel_SR << ")" << endl;
      TH1F* hist_CR = met_HG->getHistogram(p_qcdTree, sel_CR, weight, TString::Format("QCD_%s_%s_%s", p_Prefix.Data(), p_LocalSels_CR[iSR].second.second.Data(), p_SelsAndNames[iP].second.second.Data()));
      TH1F* hist_SR = met_HG->getHistogram(p_qcdTree, sel_SR, weight, TString::Format("QCD_%s_%s_%s", p_Prefix.Data(), p_LocalSels_SR[iSR].second.second.Data(), p_SelsAndNames[iP].second.second.Data()));
      hist_SR->Divide(hist_CR);
      plot->addHist(hist_SR, p_SelsAndNames[iP].second.first, "", StyleTools::colorGetter(iP), 0, StyleTools::colorGetter(iP), 1, 1, 3);
      if(yMin > getMinimum(hist_SR))   yMin = getMinimum(hist_SR);
      if(yMax < hist_SR->GetMaximum()) yMax = hist_SR->GetMaximum();
      if(p_logY){
        plot->setLogy();
        drawPlot(plot, 0.1 * yMin, 100 * yMax, false);
      } else {
        drawPlot(plot, 0.,         1.5 * yMax, false);
      }
    }
  }
}
