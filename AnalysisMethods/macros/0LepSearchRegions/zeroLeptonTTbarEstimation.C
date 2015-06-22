#include "setTDRStyle.h"
#include "TTree.h"
#include "TFile.h"
#include "TH1D.h"
#include <vector>
//#include "Math/VectorUtil.h"
#include "TString.h"
#include <sstream>
#include <string>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <stdio.h>
#include <locale>
#include "TCanvas.h"


void setTDRStyle();
void pretifyLabels(TH1F* hTemp);
TCanvas *createCanvas(TString cName);
TH1F *create1Dhisto(TTree *tree,TString intLumi,TString cuts,TString branch,int bins,float xmin,float xmax,bool useLog,int color, int style,bool norm);
TH2D *create2Dhisto(TTree *tree,TString intLumi,TString cuts,
                    TString branchX,int binsX,float xmin,float xmax
                    TString branchY,int binsY,float ymin,float ymax);
void getYandE(TTree *tree,TString intLumi,std::vector<TString> cuts,std::vector<float> &yields,std::vector<float> &errors);
void getYandE(TTree *tree,TString intLumi,TString cuts,float &yields,float &errors);
void calcTandE(std::vector<float> y_0l, std::vector<float> ye_0l, std::vector<float> y_1l, std::vector<float> ye_1l,
	       std::vector<float> &tcs, std::vector<float> &tcse);
TCanvas *decompBKGin1l(std::vector<float> y_1l_tt, std::vector<float> y_1l_w, std::vector<float> y_1l_ttW, std::vector<float> y_1l_ttZ, std::vector<float> y_1l_sm,
		       std::vector<float> ye_1l_tt, std::vector<float> ye_1l_w, std::vector<float> ye_1l_ttW, std::vector<float> ye_1l_ttZ);
TCanvas *expYin0and1l(std::vector<float> y_0l,std::vector<float> ye_0l,std::vector<float> y_1l,std::vector<float> ye_1l);
TCanvas *relEffwrtPreSel(std::vector<float> y_0l,std::vector<float> ye_0l,std::vector<float> y_1l,std::vector<float> ye_1l,float y_presel_0l,float y_presel_1l);
TCanvas *drawTcs(std::vector<float> y,std::vector<float> ye);

TCanvas *createCanvas(TTree *t_0l,TTree *t_1l,TString intLumi,TString cut_0l,TString cut_1l,TString branch,int bins,float xmin,float xmax,bool norm,bool savePlots,bool useLog,TString stdLog,TString name);
TCanvas *createCanvas(TTree *t_0l,TTree *t_0l_data,TH1F *h_0l_mc,TTree *t_1l,TString intLumi,TString cut_0l,TString cut_1l,TString branch,int bins,float xmin,float xmax,
		      bool norm,bool savePlots,bool useLog,TString stdLog,TString name);
TH1F *createSRhisto(TTree *tree,TString intLumi,std::vector<TString> cuts,int color, int style,bool norm,TString name);
TH1F *calculateTcsInMC(TH1F* h_0l_mc,TH1F* h_1l_mc);
TH1F *calcRelUnc(TH1F* hIn);
void prediction(TH1F *h_1lCS_data,TH1F *hTcs_mc,TH1F *hTcs_mcstats,TH1F *hPred_Stat,TH1F *hPred_Syst);

// paths 2 files
//TString fname_1l_mc_tt     = "/afs/cern.ch/work/g/gouskos/private/UCSB_2015/CMSSW_7_3_1/src/AnalysisMethods/macros/run/trees_0l_20150610_1lCS_pTl5_ak4jt/ttbar_tree.root";
//TString fname_1l_mc_tt     = "/afs/cern.ch/work/g/gouskos/private/UCSB_2015/CMSSW_7_3_1/src/AnalysisMethods/macros/run/trees_0l_20150615_1lCS_pTl5MultiIsoL_WithLepJetClean_ak4jt/ttbar_tree.root";
//TString fname_1l_mc_tt     = "/afs/cern.ch/work/g/gouskos/private/UCSB_2015/CMSSW_7_3_1/src/AnalysisMethods/macros/run/trees_0l_20150618_1lCS_pTl5MultiIsoL_NoLepJetClean_JetReWgt_ak4jt/ttbar_tree.root";
TString fname_1l_mc_tt     = "/afs/cern.ch/work/g/gouskos/private/UCSB_2015/CMSSW_7_3_1/src/AnalysisMethods/macros/run/trees_0l_20150622_1lCS_pTl5MultiIsoL_NoLepJetClean_RndLep_ak4jt/ttbar_tree.root";
TString fname_1l_mc_w      = "/afs/cern.ch/work/g/gouskos/private/UCSB_2015/CMSSW_7_3_1/src/AnalysisMethods/macros/run/trees_0l_20150622_1lCS_pTl5MultiIsoL_NoLepJetClean_RndLep_ak4jt/wjets_tree.root";
TString fname_1l_mc_ttW    = "/afs/cern.ch/work/g/gouskos/private/UCSB_2015/CMSSW_7_3_1/src/AnalysisMethods/macros/run/trees_0l_20150622_1lCS_pTl5MultiIsoL_NoLepJetClean_RndLep_ak4jt/ttW_tree.root";
//TString fname_1l_mc_ttall  = "/afs/cern.ch/work/g/gouskos/private/UCSB_2015/CMSSW_7_3_1/src/AnalysisMethods/macros/run/trees_0l_20150610_1lCS_pTl5_ak4jt/ttbar_tree.root";
TString fname_1l_mc_ttall  = "/afs/cern.ch/work/g/gouskos/private/UCSB_2015/CMSSW_7_3_1/src/AnalysisMethods/macros/run/trees_0l_20150622_1lCS_pTl5MultiIsoL_NoLepJetClean_RndLep_ak4jt/ttbar_tree.root";
TString fname_1l_mc_znunu  = "/afs/cern.ch/work/g/gouskos/private/UCSB_2015/CMSSW_7_3_1/src/AnalysisMethods/macros/run/trees_0l_20150622_1lCS_pTl5MultiIsoL_NoLepJetClean_RndLep_ak4jt/wjets_tree.root";
TString fname_1l_mc_ttZ    = "/afs/cern.ch/work/g/gouskos/private/UCSB_2015/CMSSW_7_3_1/src/AnalysisMethods/macros/run/trees_0l_20150622_1lCS_pTl5MultiIsoL_NoLepJetClean_RndLep_ak4jt/ttW_tree.root";
TString fname_1l_mc_sm     = "/afs/cern.ch/work/g/gouskos/private/UCSB_2015/CMSSW_7_3_1/src/AnalysisMethods/macros/run/trees_0l_20150622_1lCS_pTl5MultiIsoL_NoLepJetClean_RndLep_ak4jt/sm_tree.root";
TString fname_1l_mc_T2ttMM = "/afs/cern.ch/work/g/gouskos/private/UCSB_2015/CMSSW_7_3_1/src/AnalysisMethods/macros/run/trees_0l_20150622_1lCS_pTl5MultiIsoL_NoLepJetClean_RndLep_ak4jt/T2tt_650_325_tree.root";
TString fname_1l_mc_T2ttHM = "/afs/cern.ch/work/g/gouskos/private/UCSB_2015/CMSSW_7_3_1/src/AnalysisMethods/macros/run/trees_0l_20150622_1lCS_pTl5MultiIsoL_NoLepJetClean_RndLep_ak4jt/T2tt_850_100_tree.root";
TString fname_1l_mc_T14tMM = "/afs/cern.ch/work/g/gouskos/private/UCSB_2015/CMSSW_7_3_1/src/AnalysisMethods/macros/run/trees_0l_20150622_1lCS_pTl5MultiIsoL_NoLepJetClean_RndLep_ak4jt/T1tttt_1200_800_tree.root";
TString fname_1l_mc_T14tHM = "/afs/cern.ch/work/g/gouskos/private/UCSB_2015/CMSSW_7_3_1/src/AnalysisMethods/macros/run/trees_0l_20150622_1lCS_pTl5MultiIsoL_NoLepJetClean_RndLep_ak4jt/T1tttt_1500_100_tree.root";

//TString fname_0l_mc_tt     = "/afs/cern.ch/work/g/gouskos/private/UCSB_2015/CMSSW_7_3_1/src/AnalysisMethods/macros/run/trees_0l_20150615_TopDecayGenStudies/ttbar_tree.root";
//TString fname_0l_mc_tt     = "/afs/cern.ch/work/g/gouskos/private/UCSB_2015/CMSSW_7_3_1/src/AnalysisMethods/macros/run/trees_0l_20150610_multiisol_ak4jt/ttbar_tree.root";
TString fname_0l_mc_tt     = "/afs/cern.ch/work/g/gouskos/private/UCSB_2015/CMSSW_7_3_1/src/AnalysisMethods/macros/run/trees_0l_20150622_MultiIsoL_ak4jt/ttbar_tree.root";
TString fname_0l_mc_w      = "/afs/cern.ch/work/g/gouskos/private/UCSB_2015/CMSSW_7_3_1/src/AnalysisMethods/macros/run/trees_0l_20150622_MultiIsoL_ak4jt/wjets_tree.root";
TString fname_0l_mc_ttW    = "/afs/cern.ch/work/g/gouskos/private/UCSB_2015/CMSSW_7_3_1/src/AnalysisMethods/macros/run/trees_0l_20150622_MultiIsoL_ak4jt/ttW_tree.root";
TString fname_0l_mc_ttall  = "/afs/cern.ch/work/g/gouskos/private/UCSB_2015/CMSSW_7_3_1/src/AnalysisMethods/macros/run/trees_0l_20150622_MultiIsoL_ak4jt/ttbar_tree.root";
TString fname_0l_mc_znunu  = "/afs/cern.ch/work/g/gouskos/private/UCSB_2015/CMSSW_7_3_1/src/AnalysisMethods/macros/run/trees_0l_20150622_MultiIsoL_ak4jt/znunu_tree.root";
TString fname_0l_mc_ttZ    = "/afs/cern.ch/work/g/gouskos/private/UCSB_2015/CMSSW_7_3_1/src/AnalysisMethods/macros/run/trees_0l_20150622_MultiIsoL_ak4jt/ttW_tree.root";
TString fname_0l_mc_sm     = "/afs/cern.ch/work/g/gouskos/private/UCSB_2015/CMSSW_7_3_1/src/AnalysisMethods/macros/run/trees_0l_20150622_MultiIsoL_ak4jt/sm_tree.root";
TString fname_0l_mc_T2ttMM = "/afs/cern.ch/work/g/gouskos/private/UCSB_2015/CMSSW_7_3_1/src/AnalysisMethods/macros/run/trees_0l_20150622_MultiIsoL_ak4jt/T2tt_650_325_tree.root";
TString fname_0l_mc_T2ttHM = "/afs/cern.ch/work/g/gouskos/private/UCSB_2015/CMSSW_7_3_1/src/AnalysisMethods/macros/run/trees_0l_20150622_MultiIsoL_ak4jt/T2tt_850_100_tree.root";
TString fname_0l_mc_T14tMM = "/afs/cern.ch/work/g/gouskos/private/UCSB_2015/CMSSW_7_3_1/src/AnalysisMethods/macros/run/trees_0l_20150622_MultiIsoL_ak4jt/T1tttt_1200_800_tree.root";
TString fname_0l_mc_T14tHM = "/afs/cern.ch/work/g/gouskos/private/UCSB_2015/CMSSW_7_3_1/src/AnalysisMethods/macros/run/trees_0l_20150622_MultiIsoL_ak4jt/T1tttt_1500_100_tree.root";

TString fname_1l_data = "/afs/cern.ch/work/g/gouskos/private/UCSB_2015/CMSSW_7_3_1/src/AnalysisMethods/macros/run/trees_0l_20150622_1lCS_pTl5MultiIsoL_NoLepJetClean_RndLep_ak4jt/ttbar_pythia_tree.root";
TString fname_0l_data = "/afs/cern.ch/work/g/gouskos/private/UCSB_2015/CMSSW_7_3_1/src/AnalysisMethods/macros/run/trees_0l_20150622_MultiIsoL_ak4jt/ttbar_pythia_tree.root";



// open files
TFile *f_1l_mc_tt     = TFile::Open(fname_1l_mc_tt,"READONLY");
TFile *f_1l_mc_w      = TFile::Open(fname_1l_mc_w,"READONLY");
TFile *f_1l_mc_ttW    = TFile::Open(fname_1l_mc_ttW,"READONLY");
TFile *f_1l_mc_ttall  = TFile::Open(fname_1l_mc_ttall,"READONLY");
TFile *f_1l_mc_znunu  = TFile::Open(fname_1l_mc_znunu,"READONLY");
TFile *f_1l_mc_ttZ    = TFile::Open(fname_1l_mc_ttZ,"READONLY");
TFile *f_1l_mc_sm     = TFile::Open(fname_1l_mc_sm,"READONLY");
TFile *f_1l_mc_T2ttMM = TFile::Open(fname_1l_mc_T2ttMM,"READONLY");
TFile *f_1l_mc_T2ttHM = TFile::Open(fname_1l_mc_T2ttHM,"READONLY");
TFile *f_1l_mc_T14tMM = TFile::Open(fname_1l_mc_T14tMM,"READONLY");
TFile *f_1l_mc_T14tHM = TFile::Open(fname_1l_mc_T14tHM,"READONLY");

TFile *f_0l_mc_tt     = TFile::Open(fname_0l_mc_tt,"READONLY");
TFile *f_0l_mc_w      = TFile::Open(fname_0l_mc_w,"READONLY");
TFile *f_0l_mc_ttW    = TFile::Open(fname_0l_mc_ttW,"READONLY");
TFile *f_0l_mc_ttall  = TFile::Open(fname_0l_mc_ttall,"READONLY");
TFile *f_0l_mc_znunu  = TFile::Open(fname_0l_mc_znunu,"READONLY");
TFile *f_0l_mc_ttZ    = TFile::Open(fname_0l_mc_ttZ,"READONLY");
TFile *f_0l_mc_sm     = TFile::Open(fname_0l_mc_sm,"READONLY");
TFile *f_0l_mc_T2ttMM = TFile::Open(fname_0l_mc_T2ttMM,"READONLY");
TFile *f_0l_mc_T2ttHM = TFile::Open(fname_0l_mc_T2ttHM,"READONLY");
TFile *f_0l_mc_T14tMM = TFile::Open(fname_0l_mc_T14tMM,"READONLY");
TFile *f_0l_mc_T14tHM = TFile::Open(fname_0l_mc_T14tHM,"READONLY");

TFile *f_1l_data   = TFile::Open(fname_1l_data,"READONLY");
TFile *f_0l_data   = TFile::Open(fname_0l_data,"READONLY");


void mainfunction() {

  setTDRStyle();
  gROOT->SetBatch(false);
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(1);
  gStyle->SetPalette(1);


  // options
  bool    savePlots = false;
  bool    useLog    = false;
  TString strLog;   if (useLog) { strLog = "log"; } else { strLog = "lin"; }
  // ====


  // Define Integrated Lumi [pb^{-1}]
  float intLumi     = 4.;
  ostringstream tmpLumi;
  tmpLumi << intLumi;
  TString lumi = tmpLumi.str();
  // ====
  
  // get trees
  TTree *t_1l_mc_tt     = (TTree*)f_1l_mc_tt->Get("events");
  TTree *t_1l_mc_w      = (TTree*)f_1l_mc_w->Get("events");
  TTree *t_1l_mc_ttW    = (TTree*)f_1l_mc_ttW->Get("events");
  TTree *t_1l_mc_ttall  = (TTree*)f_1l_mc_ttall->Get("events");
  TTree *t_1l_mc_znunu  = (TTree*)f_1l_mc_znunu->Get("events");
  TTree *t_1l_mc_ttZ    = (TTree*)f_1l_mc_ttZ->Get("events");
  TTree *t_1l_mc_sm     = (TTree*)f_1l_mc_sm->Get("events");
  TTree *t_1l_mc_T2ttMM = (TTree*)f_1l_mc_T2ttMM->Get("events");
  TTree *t_1l_mc_T2ttHM = (TTree*)f_1l_mc_T2ttHM->Get("events");
  TTree *t_1l_mc_T14tMM = (TTree*)f_1l_mc_T14tMM->Get("events");
  TTree *t_1l_mc_T14tHM = (TTree*)f_1l_mc_T14tHM->Get("events");

  TTree *t_0l_mc_tt     = (TTree*)f_0l_mc_tt->Get("events");
  TTree *t_0l_mc_w      = (TTree*)f_0l_mc_w->Get("events");
  TTree *t_0l_mc_ttW    = (TTree*)f_0l_mc_ttW->Get("events");
  TTree *t_0l_mc_ttall  = (TTree*)f_0l_mc_ttall->Get("events");
  TTree *t_0l_mc_znunu  = (TTree*)f_0l_mc_znunu->Get("events");
  TTree *t_0l_mc_ttZ    = (TTree*)f_0l_mc_ttZ->Get("events");
  TTree *t_0l_mc_sm     = (TTree*)f_0l_mc_sm->Get("events");
  TTree *t_0l_mc_T2ttMM = (TTree*)f_0l_mc_T2ttMM->Get("events");
  TTree *t_0l_mc_T2ttHM = (TTree*)f_0l_mc_T2ttHM->Get("events");
  TTree *t_0l_mc_T14tMM = (TTree*)f_0l_mc_T14tMM->Get("events");
  TTree *t_0l_mc_T14tHM = (TTree*)f_0l_mc_T14tHM->Get("events");

  TTree *t_1l_data      = (TTree*)f_1l_data->Get("events");
  TTree *t_0l_data      = (TTree*)f_0l_data->Get("events");
  // ====


  // definition of search regions
  TString c_presel_0l = "((NJets>=5) && (NBJets>=1) && (NVetoedTau==0) && (DphiJ12MET>1) && (DphiJ3MET>0.5) && (MET>200) && (MinMTB1B2MET>175))";
  TString c_presel_1l = "((NJets>=5) && (NBJets>=1) && (NVetoedTau>=0) && (DphiJ12MET>1) && (DphiJ3MET>0.5) && (MET>200) && (MinMTB1B2MET>175) && (abs(DphiLepW)<1))";

  TString c_Nb1NCTT0_0l = c_presel_0l+" && (NBJets==1) && (NCTTstd==0)";
  TString c_Nb1NCTT1_0l = c_presel_0l+" && (NBJets==1) && (NCTTstd>=1)";
  TString c_Nb2NCTT0_0l = c_presel_0l+" && (NBJets>=2) && (NCTTstd==0)";
  TString c_Nb2NCTT1_0l = c_presel_0l+" && (NBJets>=2) && (NCTTstd>=1)";

  TString c_Nb1NCTT0_1l = c_presel_1l+" && (NBJets==1) && (NCTTstd==0)";
  TString c_Nb1NCTT1_1l = c_presel_1l+" && (NBJets==1) && (NCTTstd>=1)";
  TString c_Nb2NCTT0_1l = c_presel_1l+" && (NBJets>=2) && (NCTTstd==0)";
  TString c_Nb2NCTT1_1l = c_presel_1l+" && (NBJets>=2) && (NCTTstd>=1)";

  std::vector<TString> c_MET; 
  c_MET.push_back("(MET>=200. && MET<300.)" );  
  c_MET.push_back("(MET>=300. && MET<400.)" );
  c_MET.push_back("(MET>=400. && MET<500.)" );
  c_MET.push_back("(MET>=500. && MET<600.)" );
  c_MET.push_back("(MET>=600. && MET<7000.)");

  std::vector<TString> c_CTTstd; 
  c_CTTstd.push_back("(NCTTstd==0)"); 
  c_CTTstd.push_back("(NCTTstd>=1)");

  std::vector<TString> c_Nb; 
  c_Nb.push_back("(NBJets==1)");
  c_Nb.push_back("(NBJets>=2)");

  std::vector<TString> c_SBins_0l;
  std::vector<TString> c_SBins_1l;

  for (unsigned int i0=0; i0<c_Nb.size(); ++i0) {
    for (unsigned int i1=0; i1<c_CTTstd.size(); ++i1) {
      for (unsigned int i2=0; i2<c_MET.size(); ++i2) {
	TString tmpStr_0l = "("+c_presel_0l+" && "+c_Nb[i0]+" && "+c_CTTstd[i1]+" && "+c_MET[i2]+")";
	TString tmpStr_1l = "("+c_presel_1l+" && "+c_Nb[i0]+" && "+c_CTTstd[i1]+" && "+c_MET[i2]+")";
	//cout << " tot_0l " << tmpStr_0l << "\n";
	//cout << " tot_1l " << tmpStr_1l << "\n";
	c_SBins_0l.push_back(tmpStr_0l);
	c_SBins_1l.push_back(tmpStr_1l);
      } // end of looping over i2
    } // end of looping over i1
  } // end of looping over i0
  // ====

    
  // expected yields after preselection
  const int totSR = c_SBins_0l.size();
  float totSR_    = totSR;
  TH1F *hPresel_0l_mc_tt_abs = create1Dhisto(t_0l_mc_tt,lumi,c_presel_0l,"presel",totSR,0.,totSR_,false,1,1,false); hPresel_0l_mc_tt_abs->SetName("hPresel_0l_mc_tt_abs");
  TH1F *hPresel_1l_mc_tt_abs = create1Dhisto(t_1l_mc_tt,lumi,c_presel_1l,"presel",totSR,0.,totSR_,false,2,2,false); hPresel_1l_mc_tt_abs->SetName("hPresel_1l_mc_tt_abs");

  // expected yields in 0l and in 1l control sample in MC and data
  TH1F *hSR_0l_mc_tt_abs = createSRhisto(t_0l_mc_tt,lumi,c_SBins_0l,1,1,false,"0l_mc_tt_abs"); hSR_0l_mc_tt_abs->SetName("hSR_0l_mc_tt_abs");
  TH1F *hSR_1l_mc_tt_abs = createSRhisto(t_1l_mc_tt,lumi,c_SBins_1l,2,2,false,"1l_mc_tt_abs"); hSR_1l_mc_tt_abs->SetName("hSR_1l_mc_tt_abs");
  TH1F *hSR_1l_data_abs  = createSRhisto(t_1l_data,lumi,c_SBins_1l,1,1,false,"1l_data_abs");   hSR_1l_data_abs->SetName("hSR_1l_data_abs"); 
  TH1F *hSR_0l_data_abs  = createSRhisto(t_0l_data,lumi,c_SBins_0l,1,1,false,"0l_data_abs");   hSR_0l_data_abs->SetName("hSR_0l_data_abs"); 

  // decomposition of backgrounds in 1-lep
  TH1F *hSR_1l_mc_sm_abs_  = createSRhisto(t_1l_mc_sm,lumi,c_SBins_1l,1,2,false,"1l_mc_sm_abs");            hSR_1l_mc_sm_abs_->SetName("hSR_1l_mc_sm_abs_");
  TH1F *hSR_1l_mc_tt_abs_  = (TH1F*)hSR_1l_mc_tt_abs->Clone("hSR_1l_mc_tt_abs_");                           hSR_1l_mc_tt_abs_->SetLineStyle(1); hSR_1l_mc_tt_abs_->SetLineColor(2);
  TH1F *hSR_1l_mc_w_abs_   = createSRhisto(t_1l_mc_w,lumi,c_SBins_1l,4,2,false,"1l_mc_w_abs");              hSR_1l_mc_w_abs_->SetName("hSR_1l_mc_w_abs_");
  TH1F *hSR_1l_mc_ttW_abs_ = createSRhisto(t_1l_mc_ttW,lumi,c_SBins_1l,kMagenta+2,2,false,"1l_mc_ttW_abs"); hSR_1l_mc_ttW_abs->SetName("hSR_1l_mc_ttW_abs");
  TH1F *hSR_1l_mc_ttZ_abs_ = createSRhisto(t_1l_mc_ttZ,lumi,c_SBins_1l,kGreen+2,2,false,"1l_mc_ttZ_abs");   hSR_1l_mc_ttZ_abs->SetName("hSR_1l_mc_ttZ_abs");

  // estimate transfer factor
  TH1F *hTcs         = calculateTcsInMC(hSR_0l_mc_tt_abs,hSR_1l_mc_tt_abs); hTcs->SetName("hTcs");
  TH1F *hTcs_mcstats = calcRelUnc(hTcs);                                    hTcs_mcstats->SetName("hTcs_mcstats");

  // make prediction
  TH1F *hPred_Stat = (TH1F*)hTcs->Clone("hPred_Stat");
  TH1F *hPred_Syst = (TH1F*)hTcs->Clone("hPred_Syst");
  prediction(hSR_1l_data_abs,hSR_0l_data_abs,hSR_0l_mc_tt_abs,hTcs,hTcs_mcstats,hPred_Stat,hPred_Syst);
  

  
  // make plots
  TCanvas *cPresel_abs = new TCanvas("cPresel_abs","cPresel_abs",500,500);
  hPresel_1l_mc_tt_abs->Draw("HIST");
  hPresel_0l_mc_tt_abs->Draw("HIST sames");
  hPresel_1l_mc_tt_abs->Draw("HIST sames");

  TCanvas *cSR_abs = new TCanvas("cSR_abs","cSR_abs",500,500);
  hSR_1l_mc_tt_abs->Draw("HIST E0");
  hSR_0l_mc_tt_abs->Draw("HIST E0 sames");
  hSR_1l_mc_tt_abs->Draw("HIST E0 sames");

  TCanvas *cTcs = new TCanvas("cTcs","cTcs",500,500);
  hTcs->Draw("HIST E0"); 

  TCanvas *cTcs_mcstats = new TCanvas("cTcs_mcstats","cTcs_mcstats",500,500);
  hTcs_mcstats->Draw("P");

  TCanvas *cPrediction = new TCanvas("cPrediction","cPrediction",1500,500);
  hPred_Syst->Draw("E2"); 
  hPred_Stat->Draw("E1 sames"); 
  hSR_0l_data_abs->SetLineColor(4); hSR_0l_data_abs->SetMarkerColor(4);
  hSR_0l_data_abs->Draw("P sames");
  



  // comparison plots after preselection
  //  TCanvas *cMET_presel       = createCanvas(t_0l_mc_ttall,t_1l_mc_ttall,lumi,c_presel_0l,c_presel_1l,"MET",20,100.,900.,true,savePlots,useLog,strLog,"5GeVMultiIsoL-WithLepJetClean-Presel-MET-tt");
  //  TCanvas *cNJ_presel        = createCanvas(t_0l_mc_ttall,t_1l_mc_ttall,lumi,c_presel_0l,c_presel_1l,"NJets",8,3.5,11.5,true,savePlots,useLog,strLog,"5GeVMultiIsoL-WithLepJetClean-Presel-NJ-tt");
  //  TCanvas *cNB_presel        = createCanvas(t_0l_mc_ttall,t_1l_mc_ttall,lumi,c_presel_0l,c_presel_1l,"NBJets",6,-0.5,5.5,true,savePlots,useLog,strLog,"5GeVMultiIsoL-WithLepJetClean-Presel-NB-tt");
  //  TCanvas *cNCTT_presel      = createCanvas(t_0l_mc_ttall,t_1l_mc_ttall,lumi,c_presel_0l,c_presel_1l,"NCTT",6,-0.5,5.5,true,savePlots,useLog,strLog,"5GeVMultiIsoL-WithLepJetClean-Presel-NCTT-tt");
  //  TCanvas *cNCTTstd_presel   = createCanvas(t_0l_mc_ttall,t_1l_mc_ttall,lumi,c_presel_0l,c_presel_1l,"NCTTstd",6,-0.5,5.5,true,savePlots,useLog,strLog,"5GeVMultiIsoL-WithLepJetClean-Presel-NCTTstd-tt");
  //  TCanvas *cNCTTstdb_presel  = createCanvas(t_0l_ttall,t_1l_mc_ttall,lumi,c_presel_0l,c_presel_1l,"NCTTstdb",6,-0.5,5.5,true,savePlots,useLog,strLog,"5GeVMultiIsoL-WithLepJetClean-Presel-NCTTstdb-tt");
  //  TCanvas *cMinMTBMET_presel = createCanvas(t_0l_mc_ttall,t_1l_mc_ttall,lumi,c_presel_0l,c_presel_1l,"MinMTB1B2MET",10,150,550,true,savePlots,useLog,strLog,"5GeVMultiIsoL-WithLepJetClean-Presel-MinMTBMET-tt");
  //  TCanvas *cDRTop1Lep_presel = createCanvas(t_0l_ttall,t_1l_mc_ttall,lumi,c_presel_0l,c_presel_1l,"DRTop1Lep",60,0.,6.,true,savePlots,useLog,strLog,"5GeVMultiIsoL-WithLepJetClean-Presel-DRTop1Lep-tt");
  //  TCanvas *cDRTop2Lep_presel = createCanvas(t_0l_ttall,t_1l_mc_ttall,lumi,c_presel_0l,c_presel_1l,"DRTop2Lep",60,0.,6.,true,savePlots,useLog,strLog,"5GeVMultiIsoL-WithLepJetClean-Presel-DRTop2Lep-tt");
  //  TCanvas *cDRB1Lep_presel   = createCanvas(t_0l_ttall,t_1l_mc_ttall,lumi,c_presel_0l,c_presel_1l,"DRB1Lep",60,0.,6.,true,savePlots,useLog,strLog,"5GeVMultiIsoL-WithLepJetClean-Presel-DRB1Lep-tt");
  //  TCanvas *cDRB2Lep_presel   = createCanvas(t_0l_ttall,t_1l_mc_ttall,lumi,c_presel_0l,c_presel_1l,"DRB2Lep",60,0.,6.,true,savePlots,useLog,strLog,"5GeVMultiIsoL-WithLepJetClean-Presel-DRB2Lep-tt");
  // ====


  // prediction in search bins
  TCanvas *cMET_Nb1Ntop0 = createCanvas(t_0l_mc_ttall,t_0l_data,t_1l_mc_ttall,lumi,c_Nb1NCTT0_0l,c_Nb1NCTT0_1l,"MET",5,200.,700.,true,savePlots,useLog,strLog,"5GeVMultiIsoL-WithLepJetClean-Nb1Ntop0-MET-tt");
  TCanvas *cMET_Nb1Ntop1 = createCanvas(t_0l_mc_ttall,t_0l_data,t_1l_mc_ttall,lumi,c_Nb1NCTT1_0l,c_Nb1NCTT1_1l,"MET",5,200.,700.,true,savePlots,useLog,strLog,"5GeVMultiIsoL-WithLepJetClean-Nb1Ntop1-MET-tt");
  TCanvas *cMET_Nb2Ntop0 = createCanvas(t_0l_mc_ttall,t_0l_data,t_1l_mc_ttall,lumi,c_Nb2NCTT0_0l,c_Nb2NCTT0_1l,"MET",5,200.,700.,true,savePlots,useLog,strLog,"5GeVMultiIsoL-WithLepJetClean-Nb2Ntop0-MET-tt");
  TCanvas *cMET_Nb2Ntop1 = createCanvas(t_0l_mc_ttall,t_0l_data,t_1l_mc_ttall,lumi,c_Nb2NCTT1_0l,c_Nb2NCTT1_1l,"MET",5,200.,700.,true,savePlots,useLog,strLog,"5GeVMultiIsoL-WithLepJetClean-Nb2Ntop1-MET-tt");
  // ====  

} // end of void mainfunction()


void prediction(TH1F *h_1lCS_data,TH1F *h_0l_data,TH1F *h_0l_mc,TH1F *hTcs_mc,TH1F *hTcs_mcstats,TH1F *hPred_Stat,TH1F *hPred_Syst) {
  TH1::SetDefaultSumw2(kTRUE);

  hPred_Stat->Multiply(h_1lCS_data,hTcs_mc);
  hPred_Syst->Multiply(h_1lCS_data,hTcs_mc);

  hPred_Stat->SetLineWidth(3); hPred_Stat->SetLineColor(1); 
  hPred_Syst->SetLineWidth(3); hPred_Syst->SetLineColor(2); hPred_Syst->SetFillColor(2); hPred_Syst->SetFillStyle(3001);

  for (unsigned int i0=0; i0<hPred_Stat->GetNbinsX(); ++i0) {

    float tcs_     = hTcs_mc->GetBinContent(i0+1);
    float pred_    = hPred_Stat->GetBinContent(i0+1);
    float statUnc_ = sqrt(h_1lCS_data->GetBinContent(i0+1));
    float systUnc_ = hTcs_mcstats->GetBinContent(i0+1);
    hPred_Stat->SetBinError(i0+1,(statUnc_*tcs_));
    hPred_Syst->SetBinError(i0+1,pred_*systUnc_);
    
    cout << " data(CR) tcs data(SR) mc(SR) pred +/- stat +/- syst : " 
	 << h_1lCS_data->GetBinContent(i0+1) << " " 
	 << hTcs_mc->GetBinContent(i0+1)     << " "
	 << h_0l_data->GetBinContent(i0+1)   << " " 
	 << h_0l_mc->GetBinContent(i0+1)     << " "
	 << hPred_Stat->GetBinContent(i0+1)  << " +/- " << hPred_Stat->GetBinError(i0+1) << " +/- " << hPred_Syst->GetBinError(i0+1) << "\n";
  }

 }


TH1F *calcRelUnc(TH1F* hIn) {
  TH1::SetDefaultSumw2(kTRUE);

  TH1F* hTemp = (TH1F*)hIn->Clone("hTemp"); hTemp->SetName("hIn");

  for (unsigned int i0=0; i0<hIn->GetNbinsX(); ++i0) {
    
    float y  = hIn->GetBinContent(i0+1); 
    float ye = hIn->GetBinError(i0+1);

    if (y==0) { y=1.; ye=0.; }

    hTemp->SetBinContent(i0+1,(ye/y));
    hTemp->SetBinError(i0+1,0.);
    //cout << "Bin " << i0+1 << ": " << ye/y << "\n";   

  }

  return hTemp;
}


TH1F *calculateTcsInMC(TH1F* h_0l_mc,TH1F* h_1l_mc) {
  TH1::SetDefaultSumw2(kTRUE);

  TH1F* hTemp = (TH1F*)h_0l_mc->Clone("hTemp");
  hTemp->Divide(h_0l_mc,h_1l_mc,1.,1.);
  
  for (unsigned int i0=0; i0<h_0l_mc->GetNbinsX(); ++i0) {
    
    float y_0l   = h_0l_mc->GetBinContent(i0+1);
    float ye_0l  = h_0l_mc->GetBinError(i0+1);
    float y_1l   = h_1l_mc->GetBinContent(i0+1);
    float ye_1l  = h_1l_mc->GetBinError(i0+1);
    
    float tmpTcs  = hTemp->GetBinContent(i0+1);
    float tmpTcse = sqrt( pow((ye_0l/y_1l),2) + pow(((y_0l*ye_0l)/(y_1l*y_1l)),2) );

    hTemp->SetBinError(i0+1,tmpTcse);
    //cout << "Error on TCS: " << hTemp->GetBinContent(i0+1) << " +/- " << hTemp->GetBinError(i0+1) << " " << tmpTcs << " +/- " << tmpTcse << "\n";
    
  }
  
  return hTemp;
}





TH1F *createSRhisto(TTree *tree,TString intLumi,std::vector<TString> cuts,int color, int style,bool norm,TString name) {
  TH1::SetDefaultSumw2(kTRUE);
  
  const int bins = cuts.size();
  TH1F *hSR = new TH1F("hSR","hSR",bins,0.,cuts.size()); hSR->SetName("hSR_"+name);

  for (unsigned int i0=0; i0<cuts.size(); ++i0) {
    
    TString cut ="(ScaleFactor*"+intLumi+")*("+cuts[i0]+")";
    TH1F *hTemp = new TH1F("hTemp","hTemp",2,0.,1000.);
    tree->Project("hTemp","MET",cut);
    double e = 0.; double y = hTemp->IntegralAndError(0,2+1,e);
    
    hSR->SetBinContent(i0+1,y);
    hSR->SetBinError(i0+1,e);
    hTemp->Delete();
  }
  
  hSR->SetLineWidth(7);
  //  hSR->GetXaxis()->SetTitle("SR-");
  hSR->SetMarkerSize(0);
  hSR->SetLineColor(color);
  hSR->SetLineStyle(style);
  
  if (norm) { hSR->Scale(1./(hSR->Integral())); }
  
  return hSR;
} //~ end of createSRhisto




TCanvas *createCanvas(TTree *t_0l,TTree *t_0l_data,TTree *t_1l,TString intLumi,TString cut_0l,TString cut_1l,TString branch,int bins,float xmin,float xmax,
		      bool norm,bool savePlots,bool useLog,TString strLog,TString name) {

  TString b_0l; TString b_1l; 
  if (branch=="NCTTstdb") { b_0l = "NCTTstd"; b_1l = branch; }
  else                    { b_0l = branch;    b_1l = branch; }
  TH1F *hTemp_0l      = create1Dhisto(t_0l,intLumi,cut_0l,b_0l,bins,xmin,xmax,useLog,1,1,norm); hTemp_0l->SetName("h"+name+"_0l"); 
  TH1F *hTemp_0l_data = create1Dhisto(t_0l_data,intLumi,cut_0l,b_0l,bins,xmin,xmax,useLog,4,1,norm); hTemp_0l->SetName("h"+name+"_0l_data"); 
  TH1F *hTemp_1l      = create1Dhisto(t_1l,intLumi,cut_1l,b_1l,bins,xmin,xmax,useLog,2,2,norm); hTemp_1l->SetName("h"+name+"_1l"); 
  TH1F *hTemp_r       = (TH1F*)hTemp_0l->Clone("h"+name+"_r"); hTemp_r->Divide(hTemp_1l,hTemp_0l,1.,1.);
  TH1F *hTemp_r_data  = (TH1F*)hTemp_0l_data->Clone("h"+name+"_r_data"); hTemp_r_data->Divide(hTemp_0l,hTemp_0l_data,1.,1.);
  
  TCanvas *cTemp = createCanvas("c"+name);
  pMain->cd(); if (useLog) { gPad->SetLogy(); }
  hTemp_0l->Draw("HIST E0");
  hTemp_1l->Draw("HIST E0 sames");
  hTemp_0l_data->SetMarkerColor(4); hTemp_0l_data->SetMarkerSize(1.2);
  hTemp_0l_data->Draw("data E0 sames");
  pRatio->cd();
  hTemp_r->GetYaxis()->SetRangeUser(0.,2.);
  hTemp_r->Draw("P E0");
  hTemp_r_data->Draw("P E0 sames");

  if (savePlots) { cTemp->Print("../plots/20150615-0l-1lCS/TTBKGEst-"+name+"-"+strLog+".png"); }

  return cTemp;
}



TCanvas *createCanvas(TTree *t_0l,TTree *t_1l,TString intLumi,TString cut_0l,TString cut_1l,TString branch,int bins,float xmin,float xmax,bool norm,bool savePlots,bool useLog,TString strLog,TString name) {

  TString b_0l; TString b_1l; 
  if (branch=="NCTTstdb") { b_0l = "NCTTstd"; b_1l = branch; }
  else                    { b_0l = branch; b_1l = branch; }
  TH1F *hTemp_0l = create1Dhisto(t_0l,intLumi,cut_0l,b_0l,bins,xmin,xmax,useLog,1,1,norm); hTemp_0l->SetName("h"+name+"_0l"); 
  TH1F *hTemp_1l = create1Dhisto(t_1l,intLumi,cut_1l,b_1l,bins,xmin,xmax,useLog,2,2,norm); hTemp_1l->SetName("h"+name+"_1l"); 
  TH1F *hTemp_r  = (TH1F*)hTemp_0l->Clone("h"+name+"_r"); hTemp_r->Divide(hTemp_1l,hTemp_0l,1.,1.);
  
  TCanvas *cTemp = createCanvas("c"+name);
  pMain->cd(); if (useLog) { gPad->SetLogy(); }
  hTemp_0l->Draw("HIST E0");
  hTemp_1l->Draw("HIST E0 sames");
  pRatio->cd();
  hTemp_r->GetYaxis()->SetRangeUser(0.,2.);
  hTemp_r->Draw("P E0");

  if (savePlots) { cTemp->Print("../plots/20150615-0l-1lCS/TTBKGEst-"+name+"-"+strLog+".png"); }

  return cTemp;
}


TCanvas *relEffwrtPreSel(std::vector<float> y_0l,std::vector<float> ye_0l,std::vector<float> y_1l,std::vector<float> ye_1l,float y_presel_0l,float y_presel_1l) {

  const int bins = y_0l.size()+1;

  TH1F *h_0l_presel = new TH1F("h_0l_presel","h_0l_presel",bins,-0.5,y_0l.size()+0.5);
  TH1F *h_1l_presel = new TH1F("h_1l_presel","h_1l_presel",bins,-0.5,y_1l.size()+0.5);

  TH1F *h_0l_eff = new TH1F("h_0l_eff","h_0l_eff",bins,-0.5,y_0l.size()+0.5); h_0l_eff->SetLineWidth(3); h_0l_eff->SetLineColor(1); h_0l_eff->SetLineStyle(1); h_0l_eff->SetMarkerSize(0); 
  TH1F *h_1l_eff = new TH1F("h_1l_eff","h_1l_eff",bins,-0.5,y_1l.size()+0.5); h_1l_eff->SetLineWidth(3); h_1l_eff->SetLineColor(2); h_1l_eff->SetLineStyle(1); h_1l_eff->SetMarkerSize(0); 

  for (unsigned int i0=0; i0<y_0l.size(); ++i0) {

    h_0l_eff->SetBinContent(i0+1,y_0l[i0]);
    h_0l_eff->SetBinError(i0+1,ye_0l[i0]);
    h_0l_presel->SetBinContent(i0+1,y_presel_0l);

    h_1l_eff->SetBinContent(i0+1,y_1l[i0]);
    h_1l_eff->SetBinError(i0+1,ye_1l[i0]);
    h_1l_presel->SetBinContent(i0+1,y_presel_1l);

  }

  h_0l_eff->Divide(h_0l_eff,h_0l_presel,1.,1.,"B");
  h_1l_eff->Divide(h_1l_eff,h_1l_presel,1.,1.,"B");

  TCanvas *cRelEffwrtPreSel_ = new TCanvas("cRelEffwrtPreSel_","cRelEffwrtPreSel_",500,500);

  h_1l_eff->Draw("HIST E0");
  h_0l_eff->Draw("HIST E0 sames");


  return cRelEffwrtPreSel_;
} //~ end of create1Dhisto


TCanvas *expYin0and1l(std::vector<float> y_0l,std::vector<float> ye_0l,std::vector<float> y_1l,std::vector<float> ye_1l) {

  const int bins = y_0l.size()+1;

  TH1F *h_0l = new TH1F("h_0l","h_0l",bins,-0.5,y_0l.size()+0.5); h_0l->SetLineWidth(3); h_0l->SetLineColor(1); h_0l->SetLineStyle(1); h_0l->SetMarkerSize(0);
  TH1F *h_1l = new TH1F("h_1l","h_1l",bins,-0.5,y_1l.size()+0.5); h_1l->SetLineWidth(3); h_1l->SetLineColor(2); h_1l->SetLineStyle(1); h_1l->SetMarkerSize(0);

  for (unsigned int i0=0; i0<y_0l.size(); ++i0) {

    h_0l->SetBinContent(i0+1,y_0l[i0]);
    h_0l->SetBinError(i0+1,ye_0l[i0]);

    h_1l->SetBinContent(i0+1,y_1l[i0]);
    h_1l->SetBinError(i0+1,ye_1l[i0]);

  }

  TCanvas *cExpYin0and1l_ = new TCanvas("cExpYin0and1l_","cExpYin0and1l_",500,500);
  h_1l->Draw("HIST E0");
  h_0l->Draw("HIST E0 sames");

  return cExpYin0and1l_;
} //~ end of create1Dhisto


TCanvas *drawTcs(std::vector<float> y,std::vector<float> ye) {

  const int bins = y.size()+1;

  TH1F *hTcs = new TH1F("hTcs","hTcs",bins,-0.5,y.size()+0.5); hTcs->SetLineWidth(3); hTcs->SetLineColor(1); hTcs->SetLineStyle(1); hTcs->SetMarkerSize(0);

  for (unsigned int i0=0; i0<y.size(); ++i0) {

    hTcs->SetBinContent(i0+1,y[i0]);
    hTcs->SetBinError(i0+1,ye[i0]);
    cout << "bin " << i0 << " = " << y[i0] << " +/- " << ye[i0] << " ; " << 1.+ye[i0]/y[i0] << "\n";
  }

  TCanvas *cTcs_ = new TCanvas("cTcs_","cTcs_",500,500);
  hTcs->Draw("P E0");

  return cTcs_;
} //~ end of create1Dhisto


TCanvas *decompBKGin1l(std::vector<float> y_1l_tt, std::vector<float> y_1l_w, std::vector<float> y_1l_ttW, std::vector<float> y_1l_ttZ, std::vector<float> y_1l_sm,
		       std::vector<float> ye_1l_tt, std::vector<float> ye_1l_w, std::vector<float> ye_1l_ttW, std::vector<float> ye_1l_ttZ) {

  const int bins = y_1l_tt.size()+1;

  TH1F *h_tt  = new TH1F("h_tt","h_tt",bins,-0.5,y_1l_tt.size()+0.5);    h_tt->SetLineWidth(3);  h_tt->SetLineColor(1);       h_tt->SetMarkerSize(0);
  TH1F *h_w   = new TH1F("h_w","h_w",bins,-0.5,y_1l_w.size()+0.5);       h_w->SetLineWidth(3);   h_w->SetLineColor(kGreen+2); h_w->SetMarkerSize(0);
  TH1F *h_ttW = new TH1F("h_ttW","h_ttW",bins,-0.5,y_1l_ttW.size()+0.5); h_ttW->SetLineWidth(3); h_ttW->SetLineColor(4);      h_ttW->SetMarkerSize(0);
  TH1F *h_ttZ = new TH1F("h_ttZ","h_ttZ",bins,-0.5,y_1l_ttZ.size()+0.5); h_ttZ->SetLineWidth(3); h_ttZ->SetLineColor(2);      h_ttZ->SetMarkerSize(0);

  for (unsigned int i0=0; i0<y_1l_tt.size(); ++i0) {

    h_tt->SetBinContent(i0+1,y_1l_tt[i0]/y_1l_sm[i0]);
    h_tt->SetBinError(i0+1,ye_1l_tt[i0]/y_1l_sm[i0]);
    h_w->SetBinContent(i0+1,y_1l_w[i0]/y_1l_sm[i0]);
    h_w->SetBinError(i0+1,ye_1l_w[i0]/y_1l_sm[i0]);
    h_ttW->SetBinContent(i0+1,y_1l_ttW[i0]/y_1l_sm[i0]);
    h_ttW->SetBinError(i0+1,ye_1l_ttW[i0]/y_1l_sm[i0]);
    h_ttZ->SetBinContent(i0+1,y_1l_ttZ[i0]/y_1l_sm[i0]);
    h_ttZ->SetBinError(i0+1,ye_1l_ttZ[i0]/y_1l_sm[i0]);

  }

  TCanvas *cTemp = new TCanvas("cTemp","cTemp",500,500);
  h_tt->Draw("HIST E0");
  h_w->Draw("HIST E0 sames");
  h_ttW->Draw("HIST E0 sames");
  h_ttZ->Draw("HIST E0 sames");

  return cTemp;
} //~ end of create1Dhisto


void getYandE(TTree *tree,TString intLumi,std::vector<TString> cuts,std::vector<float> &yields,std::vector<float> &errors) {
  TH1::SetDefaultSumw2(kTRUE);

  for (unsigned int i0=0; i0<cuts.size(); ++i0) {

    TString cut ="(ScaleFactor*"+intLumi+")*("+cuts[i0]+")";
    TH1F *hTemp = new TH1F("hTemp","hTemp",2,0.,1000.);
    tree->Project("hTemp","MET",cut);
    double e = 0.; double y = hTemp->IntegralAndError(0,2+1,e);

    yields.push_back(y);
    errors.push_back(e);
    hTemp->Delete();
  }

}


void getYandE(TTree *tree,TString intLumi,TString cuts,float &yields,float &errors) {
  TH1::SetDefaultSumw2(kTRUE);

  TString cut ="(ScaleFactor*"+intLumi+")*("+cuts+")";
  TH1F *hTemp = new TH1F("hTemp","hTemp",2,0.,1000.);
  tree->Project("hTemp","MET",cut);
  float e =0.; float y = hTemp->IntegralAndError(0,2+1,e);
  hTemp->Delete();
  yields = y;
  errors = e;

}


void calcTandE(std::vector<float> y_0l, std::vector<float> ye_0l, std::vector<float> y_1l, std::vector<float> ye_1l,
	       std::vector<float> &tcs, std::vector<float> &tcse) {
  TH1::SetDefaultSumw2(kTRUE);

  for (unsigned int i0=0; i0<y_0l.size(); ++i0) {

    float tmpTcs  = y_0l[i0]/y_1l[i0];
    float tmpTcse = sqrt( pow((ye_0l[i0]/y_1l[i0]),2) + pow(((y_0l[i0]*ye_0l[i0])/(y_1l[i0]*y_1l[i0])),2) );

    tcs.push_back(tmpTcs);
    tcse.push_back(tmpTcse);
  }

}



TH1F *create1Dhisto(TTree *tree,TString intLumi,TString cuts,TString branch,int bins,float xmin,float xmax,bool useLog,int color, int style,bool norm) {
  TH1::SetDefaultSumw2(kTRUE);

  TString cut ="(ScaleFactor*"+intLumi+")*("+cuts+")";


  TH1F *hTemp = new TH1F("hTemp","hTemp",bins,xmin,xmax);

  if (branch!="presel") { 

    tree->Project("hTemp",branch,cut);

    if (useLog) {
      hTemp->GetYaxis()->SetRangeUser(0.1,50.*(hTemp->GetBinContent(hTemp->GetMaximumBin())));
    }
    else if (!useLog) {
      hTemp->GetYaxis()->SetRangeUser(0.,2.*(hTemp->GetBinContent(hTemp->GetMaximumBin())));
    }

    hTemp->GetXaxis()->SetTitle(branch);

  }

  else {

    for (unsigned int i0=0; i0<bins; ++i0) {
    
      TString cut ="(ScaleFactor*"+intLumi+")*("+cuts+")";
      TH1F *h_ = new TH1F("h_","h_",2,0.,1000.);
      tree->Project("h_","MET",cut);
      double e = 0.; double y = h_->IntegralAndError(0,2+1,e);
      
      hTemp->SetBinContent(i0+1,y);
      hTemp->SetBinError(i0+1,e);
      h_->Delete();
    }
    
  }
  
  hTemp->SetLineWidth(7);
  hTemp->SetMarkerSize(0);
  hTemp->SetLineWidth(7);
  hTemp->SetLineColor(color);
  hTemp->SetLineStyle(style);
  
  // ad overflow bin
  double error =0.; double integral = hTemp->IntegralAndError(bins,bins+1,error);
  hTemp->SetBinContent(bins,integral);
  hTemp->SetBinError(bins,error);

  if (norm) { hTemp->Scale(1./(hTemp->Integral())); }

  return hTemp;
} //~ end of create1Dhisto



TH2D *create2Dhisto(TTree *tree,TString intLumi,TString cuts,
                    TString branchX,int binsX,float xmin,float xmax,
                    TString branchY,int binsY,float ymin,float ymax) {

  TH1::SetDefaultSumw2(kTRUE);

  TString cut ="(ScaleFactor)*("+cuts+")";
  
  TH2D *hTemp = new TH2D("hTemp","hTemp",binsX,xmin,xmax,binsY,ymin,ymax);
  tree->Project("hTemp",branchY+":"+branchX,cut);

  hTemp->GetXaxis()->SetTitle(branchX);
  hTemp->GetYaxis()->SetTitle(branchY);

  return hTemp;
} //~ end of create2Dhisto 


TCanvas *createCanvas(TString cName) {
  
  TCanvas *cTemp = new TCanvas(cName,cName,500,700); cTemp->SetName(cName);

  //  TPad *pMain  = new TPad("pMain_"+cName,"pMain_"+cName,0.0,0.35,1.0,1.0);
  TPad *pMain  = new TPad("pMain","pMain",0.0,0.35,1.0,1.0);
  pMain->SetRightMargin(0.05);
  pMain->SetLeftMargin(0.1);
  pMain->SetBottomMargin(0.);

  //  TPad *pRatio = new TPad("pRatio_"+cName,"pRatio_"+cName,0.0,0.0,1.0,0.35);
  TPad *pRatio = new TPad("pRatio","pRatio",0.0,0.0,1.0,0.35);
  pRatio->SetRightMargin(0.05);
  pRatio->SetLeftMargin(0.1);
  pRatio->SetTopMargin(0.);
  pRatio->SetBottomMargin(0.2);

  pRatio->SetGrid(1);

  pMain->Draw();
  pRatio->Draw();
  
  return cTemp;
}


void pretifyLabels(TH1F* hTemp) {

  hTemp->GetYaxis()->SetRangeUser(0.,2.);
  hTemp->GetYaxis()->SetLabelSize(0.09);
  hTemp->GetYaxis()->SetNdivisions(508);

  hTemp->GetXaxis()->SetLabelSize(0.09);
  hTemp->GetXaxis()->SetTitleSize(0.1);

} 
