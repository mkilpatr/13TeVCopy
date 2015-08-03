//#include "setTDRStyle.h"
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
#include "TH1"
#include "TH1F"
#include "TPad"

// IMPORTANT NOTE: 1l and LowMET used interchangeably
//                 0l and 2J used interchangeably
//                 3l and 3J used interchangeably
//                 4l and 4J used interchangeably
//                 5l and 5J used interchangeably


//void setTDRStyle();
void pretifyLabels(TH1F* hTemp);
TCanvas *createCanvas(TString cName);
TH1F *create1Dhisto(TTree *tree,TString intLumi,TString cuts,TString branch,int bins,float xmin,float xmax,bool useLog,int color, int style,bool norm);
TH2D *create2Dhisto(TTree *tree,TString intLumi,TString cuts,
                    TString branchX,int binsX,float xmin,float xmax
                    TString branchY,int binsY,float ymin,float ymax);
TCanvas *createCanvas(TTree *t_0l,TTree *t_1l,TString intLumi,TString cut_0l,TString cut_1l,TString branch,int bins,float xmin,float xmax,bool norm,bool savePlots,bool useLog,TString stdLog,TString name);
TCanvas *createCanvas(TTree *t_0l,TTree *t_0l_data,TH1F *h_0l_mc,TTree *t_1l,TString intLumi,TString cut_0l,TString cut_1l,TString branch,int bins,float xmin,float xmax,
		      bool norm,bool savePlots,bool useLog,TString stdLog,TString name);
TH1F *createSRhisto(TTree *tree,TString intLumi,std::vector<TString> cuts,int color, int style,bool norm,TString name);
TH1F *calculateTcsInMC(TH1F* h_0l_mc,TH1F* h_1l_mc);
TH1F *calcRelUnc(TH1F* hIn);
void prediction(TH1F *h_1lCS_data,TH1F *hTcs_mc,TH1F *hTcs_mcstats,TH1F *hPred_Stat,TH1F *hPred_Syst,TH1F *hPred_r);


// path 2 file
TString fname_LowMET_mc_tt     = "/afs/cern.ch/user/r/rdorst/CMSSW_7_3_1/src/AnalysisMethods/macros/run/makeZeroLeptonLowMETCRTrees/ttbar_tree.root";
TString fname_LowMET_mc_w      = "/afs/cern.ch/user/r/rdorst/CMSSW_7_3_1/src/AnalysisMethods/macros/run/makeZeroLeptonLowMETCRTrees/wjets_tree.root";
TString fname_LowMET_mc_ttW    = "/afs/cern.ch/user/r/rdorst/CMSSW_7_3_1/src/AnalysisMethods/macros/run/makeZeroLeptonLowMETCRTrees/ttW_tree.root";
TString fname_LowMET_mc_ttall  = "/afs/cern.ch/user/r/rdorst/CMSSW_7_3_1/src/AnalysisMethods/macros/run/makeZeroLeptonLowMETCRTrees/ttall_tree.root";
TString fname_LowMET_mc_znunu  = "/afs/cern.ch/user/r/rdorst/CMSSW_7_3_1/src/AnalysisMethods/macros/run/makeZeroLeptonLowMETCRTrees/znunu_tree.root";
TString fname_LowMET_mc_ttZ    = "/afs/cern.ch/user/r/rdorst/CMSSW_7_3_1/src/AnalysisMethods/macros/run/makeZeroLeptonLowMETCRTrees/ttZ_tree.root";
TString fname_LowMET_mc_sm     = "/afs/cern.ch/user/r/rdorst/CMSSW_7_3_1/src/AnalysisMethods/macros/run/makeZeroLeptonLowMETCRTrees/sm_tree.root";
TString fname_LowMET_mc_T2ttMM = "/afs/cern.ch/user/r/rdorst/CMSSW_7_3_1/src/AnalysisMethods/macros/run/makeZeroLeptonLowMETCRTrees/T2tt_650_325_tree.root";
TString fname_LowMET_mc_T2ttHM = "/afs/cern.ch/user/r/rdorst/CMSSW_7_3_1/src/AnalysisMethods/macros/run/makeZeroLeptonLowMETCRTrees/T2tt_850_100_tree.root";
TString fname_LowMET_mc_T14tMM = "/afs/cern.ch/user/r/rdorst/CMSSW_7_3_1/src/AnalysisMethods/macros/run/makeZeroLeptonLowMETCRTrees/T1tttt_1200_800_tree.root";
TString fname_LowMET_mc_T14tHM = "/afs/cern.ch/user/r/rdorst/CMSSW_7_3_1/src/AnalysisMethods/macros/run/makeZeroLeptonLowMETCRTrees/T1tttt_1500_100_tree.root";

TString fname_0l_mc_tt     = "/afs/cern.ch/user/r/rdorst/CMSSW_7_3_1/src/AnalysisMethods/macros/run/makeZeroLeptonLowMETCRTrees/ttbar_tree.root";
TString fname_0l_mc_w      = "/afs/cern.ch/user/r/rdorst/CMSSW_7_3_1/src/AnalysisMethods/macros/run/makeZeroLeptonLowMETCRTrees/wjets_tree.root";
TString fname_0l_mc_ttW    = "/afs/cern.ch/user/r/rdorst/CMSSW_7_3_1/src/AnalysisMethods/macros/run/makeZeroLeptonLowMETCRTrees/ttW_tree.root";
TString fname_0l_mc_ttall  = "/afs/cern.ch/user/r/rdorst/CMSSW_7_3_1/src/AnalysisMethods/macros/run/makeZeroLeptonLowMETCRTrees/ttall_tree.root";
TString fname_0l_mc_znunu  = "/afs/cern.ch/user/r/rdorst/CMSSW_7_3_1/src/AnalysisMethods/macros/run/makeZeroLeptonLowMETCRTrees/znunu_tree.root";
TString fname_0l_mc_ttZ    = "/afs/cern.ch/user/r/rdorst/CMSSW_7_3_1/src/AnalysisMethods/macros/run/makeZeroLeptonLowMETCRTrees/ttZ_tree.root";
TString fname_0l_mc_sm     = "/afs/cern.ch/user/r/rdorst/CMSSW_7_3_1/src/AnalysisMethods/macros/run/makeZeroLeptonLowMETCRTrees/sm_tree.root";
TString fname_0l_mc_T2ttMM = "/afs/cern.ch/user/r/rdorst/CMSSW_7_3_1/src/AnalysisMethods/macros/run/makeZeroLeptonLowMETCRTrees/T2tt_650_325_tree.root";
TString fname_0l_mc_T2ttHM = "/afs/cern.ch/user/r/rdorst/CMSSW_7_3_1/src/AnalysisMethods/macros/run/makeZeroLeptonLowMETCRTrees/T2tt_850_100_tree.root";
TString fname_0l_mc_T14tMM = "/afs/cern.ch/user/r/rdorst/CMSSW_7_3_1/src/AnalysisMethods/macros/run/makeZeroLeptonLowMETCRTrees/T1tttt_1200_800_tree.root";
TString fname_0l_mc_T14tHM = "/afs/cern.ch/user/r/rdorst/CMSSW_7_3_1/src/AnalysisMethods/macros/run/makeZeroLeptonLowMETCRTrees/T1tttt_1500_100_tree.root";

TString fname_LowMET_data = "/afs/cern.ch/user/r/rdorst/CMSSW_7_3_1/src/AnalysisMethods/macros/run/makeZeroLeptonLowMETCRTrees/ttbar_pythia_tree.root";
TString fname_0l_data = "/afs/cern.ch/user/r/rdorst/CMSSW_7_3_1/src/AnalysisMethods/macros/run/makeZeroLeptonLowMETCRTrees/ttbar_pythia_tree.root";
// =====


// open files
TFile *f_LowMET_mc_tt     = TFile::Open(fname_LowMET_mc_tt,"READONLY");
TFile *f_LowMET_mc_w      = TFile::Open(fname_LowMET_mc_w,"READONLY");
TFile *f_LowMET_mc_ttW    = TFile::Open(fname_LowMET_mc_ttW,"READONLY");
TFile *f_LowMET_mc_ttall  = TFile::Open(fname_LowMET_mc_ttall,"READONLY");
TFile *f_LowMET_mc_znunu  = TFile::Open(fname_LowMET_mc_znunu,"READONLY");
TFile *f_LowMET_mc_ttZ    = TFile::Open(fname_LowMET_mc_ttZ,"READONLY");
TFile *f_LowMET_mc_sm     = TFile::Open(fname_LowMET_mc_sm,"READONLY");
TFile *f_LowMET_mc_T2ttMM = TFile::Open(fname_LowMET_mc_T2ttMM,"READONLY");
TFile *f_LowMET_mc_T2ttHM = TFile::Open(fname_LowMET_mc_T2ttHM,"READONLY");
TFile *f_LowMET_mc_T14tMM = TFile::Open(fname_LowMET_mc_T14tMM,"READONLY");
TFile *f_LowMET_mc_T14tHM = TFile::Open(fname_LowMET_mc_T14tHM,"READONLY");

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

TFile *f_LowMET_data   = TFile::Open(fname_LowMET_data,"READONLY");
TFile *f_0l_data   = TFile::Open(fname_0l_data,"READONLY");
// ======


void mainfunction() {

  //setTDRStyle();
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
  TTree *t_1l_mc_tt     = (TTree*)f_LowMET_mc_tt->Get("Events");
  TTree *t_1l_mc_w      = (TTree*)f_LowMET_mc_w->Get("Events");
  TTree *t_1l_mc_ttW    = (TTree*)f_LowMET_mc_ttW->Get("Events");
  TTree *t_1l_mc_ttall  = (TTree*)f_LowMET_mc_ttall->Get("Events");
  TTree *t_1l_mc_znunu  = (TTree*)f_LowMET_mc_znunu->Get("Events");
  TTree *t_1l_mc_ttZ    = (TTree*)f_LowMET_mc_ttZ->Get("Events");
  TTree *t_1l_mc_sm     = (TTree*)f_LowMET_mc_sm->Get("Events");
  TTree *t_1l_mc_T2ttMM = (TTree*)f_LowMET_mc_T2ttMM->Get("Events");
  TTree *t_1l_mc_T2ttHM = (TTree*)f_LowMET_mc_T2ttHM->Get("Events");
  TTree *t_1l_mc_T14tMM = (TTree*)f_LowMET_mc_T14tMM->Get("Events");
  TTree *t_1l_mc_T14tHM = (TTree*)f_LowMET_mc_T14tHM->Get("Events");

  TTree *t_0l_mc_tt     = (TTree*)f_0l_mc_tt->Get("Events");
  TTree *t_0l_mc_w      = (TTree*)f_0l_mc_w->Get("Events");
  TTree *t_0l_mc_ttW    = (TTree*)f_0l_mc_ttW->Get("Events");
  TTree *t_0l_mc_ttall  = (TTree*)f_0l_mc_ttall->Get("Events");
  TTree *t_0l_mc_znunu  = (TTree*)f_0l_mc_znunu->Get("Events");
  TTree *t_0l_mc_ttZ    = (TTree*)f_0l_mc_ttZ->Get("Events");
  TTree *t_0l_mc_sm     = (TTree*)f_0l_mc_sm->Get("Events");
  TTree *t_0l_mc_T2ttMM = (TTree*)f_0l_mc_T2ttMM->Get("Events");
  TTree *t_0l_mc_T2ttHM = (TTree*)f_0l_mc_T2ttHM->Get("Events");
  TTree *t_0l_mc_T14tMM = (TTree*)f_0l_mc_T14tMM->Get("Events");
  TTree *t_0l_mc_T14tHM = (TTree*)f_0l_mc_T14tHM->Get("Events");

  TTree *t_1l_data      = (TTree*)f_LowMET_data->Get("Events");
  TTree *t_0l_data      = (TTree*)f_0l_data->Get("Events");
  // ====


  // definition of search regions
  TString c_presel_1l = "((njets>=5) && (nbjets>=1) && (nvetotau==0) && (dphij12met>1) && (dphij3met>0.5) && (met>175) && (met<225) && (mtb12met>175))";
  TString c_presel_2J = "((njets==2) && (nbjets>=1) && (nvetotau==0) && (dphij12met>1) && (dphij3met>0.5) && (met>200) && (mtb12met>175))";
  TString c_presel_3J = "((njets==3) && (nbjets>=1) && (nvetotau==0) && (dphij12met>1) && (dphij3met>0.5) && (met>200) && (mtb12met>175))";
  TString c_presel_4J = "((njets==4) && (nbjets>=1) && (nvetotau==0) && (dphij12met>1) && (dphij3met>0.5) && (met>200) && (mtb12met>175))";
  TString c_presel_5J = "((njets>=5) && (nbjets>=1) && (nvetotau==0) && (dphij12met>1) && (dphij3met>0.5) && (met>200) && (mtb12met>175))";  

  TString c_Nb1NCTT0_2J = c_presel_2J+" && (nbjets==1) && (ncttstd==0)";
  TString c_Nb1NCTT1_2J = c_presel_2J+" && (nbjets==1) && (ncttstd>=1)";
  TString c_Nb2NCTT0_2J = c_presel_2J+" && (nbjets>=2) && (ncttstd==0)";
  TString c_Nb2NCTT1_2J = c_presel_2J+" && (nbjets>=2) && (ncttstd>=1)";

  TString c_Nb1NCTT0_3J = c_presel_3J+" && (nbjets==1) && (ncttstd==0)";
  TString c_Nb1NCTT1_3J = c_presel_3J+" && (nbjets==1) && (ncttstd>=1)";
  TString c_Nb2NCTT0_3J = c_presel_3J+" && (nbjets>=2) && (ncttstd==0)";
  TString c_Nb2NCTT1_3J = c_presel_3J+" && (nbjets>=2) && (ncttstd>=1)";

  TString c_Nb1NCTT0_4J = c_presel_4J+" && (nbjets==1) && (ncttstd==0)";
  TString c_Nb1NCTT1_4J = c_presel_4J+" && (nbjets==1) && (ncttstd>=1)";
  TString c_Nb2NCTT0_4J = c_presel_4J+" && (nbjets>=2) && (ncttstd==0)";
  TString c_Nb2NCTT1_4J = c_presel_4J+" && (nbjets>=2) && (ncttstd>=1)";

  TString c_Nb1NCTT0_5J = c_presel_4J+" && (nbjets==1) && (ncttstd==0)";
  TString c_Nb1NCTT1_5J = c_presel_4J+" && (nbjets==1) && (ncttstd>=1)";
  TString c_Nb2NCTT0_5J = c_presel_4J+" && (nbjets>=2) && (ncttstd==0)";
  TString c_Nb2NCTT1_5J = c_presel_4J+" && (nbjets>=2) && (ncttstd>=1)";

  TString c_Nb1NCTT0_LowMET = c_presel_1l+" && (nbjets==1) && (ncttstd==0)";
  TString c_Nb1NCTT1_LowMET = c_presel_1l+" && (nbjets==1) && (ncttstd>=1)";
  TString c_Nb2NCTT0_LowMET = c_presel_1l+" && (nbjets>=2) && (ncttstd==0)";
  TString c_Nb2NCTT1_LowMET = c_presel_1l+" && (nbjets>=2) && (ncttstd>=1)";

  std::vector<TString> c_MET; 
  c_MET.push_back("(met>=200. && met<300.)" );  
  c_MET.push_back("(met>=300. && met<400.)" );
  c_MET.push_back("(met>=400. && met<500.)" );
  c_MET.push_back("(met>=500. && met<600.)" );
  c_MET.push_back("(met>=600. && met<7000.)");

  std::vector<TString> c_CTTstd; 
  c_CTTstd.push_back("(ncttstd==0)"); 
  c_CTTstd.push_back("(ncttstd>=1)");

  std::vector<TString> c_Nb; 
  c_Nb.push_back("(nbjets==1)");
  c_Nb.push_back("(nbjets>=2)");

  std::vector<TString> c_SBins_0l;
  std::vector<TString> c_SBins_3l;
  std::vector<TString> c_SBins_4l;
  std::vector<TString> c_SBins_5l;
  std::vector<TString> c_SBins_1l;

  for (unsigned int i0=0; i0<c_Nb.size(); ++i0) {
    for (unsigned int i1=0; i1<c_CTTstd.size(); ++i1) {
      for (unsigned int i2=0; i2<c_MET.size(); ++i2) {
	TString tmpStr_0l = "("+c_presel_2J+" && "+c_Nb[i0]+" && "+c_CTTstd[i1]+" && "+c_MET[i2]+")";
	TString tmpStr_3l = "("+c_presel_3J+" && "+c_Nb[i0]+" && "+c_CTTstd[i1]+" && "+c_MET[i2]+")";
	TString tmpStr_4l = "("+c_presel_4J+" && "+c_Nb[i0]+" && "+c_CTTstd[i1]+" && "+c_MET[i2]+")";
	TString tmpStr_5l = "("+c_presel_5J+" && "+c_Nb[i0]+" && "+c_CTTstd[i1]+" && "+c_MET[i2]+")";
	TString tmpStr_1l = "("+c_presel_1l+" && "+c_Nb[i0]+" && "+c_CTTstd[i1]+" && "+c_MET[i2]+")";
	//cout << " tot_0l " << tmpStr_0l << "\n";
	//cout << " tot_1l " << tmpStr_1l << "\n";
	c_SBins_0l.push_back(tmpStr_0l);
	c_SBins_3l.push_back(tmpStr_3l);
	c_SBins_4l.push_back(tmpStr_4l);
	c_SBins_5l.push_back(tmpStr_5l);
	c_SBins_1l.push_back(tmpStr_1l);
      } // end of looping over i2
    } // end of looping over i1
  } // end of looping over i0
  // ====

      
  // expected yields after preselection
  const int totSR = c_SBins_0l.size();
  float totSR_    = totSR;
  TH1F *hPresel_0l_mc_ttall_abs = create1Dhisto(t_0l_mc_ttall,lumi,c_presel_2J,"presel",totSR,0.,totSR_,false,1,1,false); hPresel_0l_mc_ttall_abs->SetName("hPresel_0l_mc_ttall_abs");
  TH1F *hPresel_3l_mc_ttall_abs = create1Dhisto(t_0l_mc_ttall,lumi,c_presel_3J,"presel",totSR,0.,totSR_,false,1,1,false); hPresel_3l_mc_ttall_abs->SetName("hPresel_3l_mc_ttall_abs");
  TH1F *hPresel_4l_mc_ttall_abs = create1Dhisto(t_0l_mc_ttall,lumi,c_presel_4J,"presel",totSR,0.,totSR_,false,1,1,false); hPresel_4l_mc_ttall_abs->SetName("hPresel_4l_mc_ttall_abs");
  TH1F *hPresel_5l_mc_ttall_abs = create1Dhisto(t_0l_mc_ttall,lumi,c_presel_5J,"presel",totSR,0.,totSR_,false,1,1,false); hPresel_5l_mc_ttall_abs->SetName("hPresel_5l_mc_ttall_abs");
  TH1F *hPresel_1l_mc_ttall_abs = create1Dhisto(t_1l_mc_ttall,lumi,c_presel_1l,"presel",totSR,0.,totSR_,false,2,2,false); hPresel_1l_mc_ttall_abs->SetName("hPresel_1l_mc_ttall_abs");

  // expected yields in LowMET J=2,3,4,>=5 control sample in MC and data
  TH1F *hSR_0l_mc_ttall_abs = createSRhisto(t_0l_mc_ttall,lumi,c_SBins_0l,1,1,false,"0l_mc_ttall_abs"); hSR_0l_mc_ttall_abs->SetName("hSR_0l_mc_ttall_abs");
  TH1F *hSR_3l_mc_ttall_abs = createSRhisto(t_0l_mc_ttall,lumi,c_SBins_3l,1,1,false,"0l_mc_ttall_abs"); hSR_3l_mc_ttall_abs->SetName("hSR_0l_mc_ttall_abs");
  TH1F *hSR_4l_mc_ttall_abs = createSRhisto(t_0l_mc_ttall,lumi,c_SBins_4l,1,1,false,"0l_mc_ttall_abs"); hSR_4l_mc_ttall_abs->SetName("hSR_0l_mc_ttall_abs");
  TH1F *hSR_5l_mc_ttall_abs = createSRhisto(t_0l_mc_ttall,lumi,c_SBins_5l,1,1,false,"0l_mc_ttall_abs"); hSR_5l_mc_ttall_abs->SetName("hSR_0l_mc_ttall_abs");
  TH1F *hSR_1l_mc_ttall_abs = createSRhisto(t_1l_mc_ttall,lumi,c_SBins_1l,2,2,false,"1l_mc_ttall_abs"); hSR_1l_mc_ttall_abs->SetName("hSR_1l_mc_ttall_abs");
  TH1F *hSR_1l_data_abs     = createSRhisto(t_1l_data,lumi,c_SBins_1l,1,1,false,"1l_data_abs");         hSR_1l_data_abs->SetName("hSR_1l_data_abs"); 
  TH1F *hSR_0l_data_abs     = createSRhisto(t_0l_data,lumi,c_SBins_0l,1,1,false,"0l_data_abs");         hSR_0l_data_abs->SetName("hSR_0l_data_abs"); 
  TH1F *hSR_3l_data_abs     = createSRhisto(t_0l_data,lumi,c_SBins_3l,1,1,false,"0l_data_abs");         hSR_3l_data_abs->SetName("hSR_0l_data_abs"); 
  TH1F *hSR_4l_data_abs     = createSRhisto(t_0l_data,lumi,c_SBins_4l,1,1,false,"0l_data_abs");         hSR_4l_data_abs->SetName("hSR_0l_data_abs"); 
  TH1F *hSR_5l_data_abs     = createSRhisto(t_0l_data,lumi,c_SBins_5l,1,1,false,"0l_data_abs");         hSR_5l_data_abs->SetName("hSR_0l_data_abs"); 

  // decomposition of backgrounds in LowMET
  TH1F *hSR_1l_mc_sm_abs_  = createSRhisto(t_1l_mc_sm,lumi,c_SBins_1l,1,2,false,"1l_mc_sm_abs");             hSR_1l_mc_sm_abs_->SetName("hSR_1l_mc_sm_abs_");
  TH1F *hSR_1l_mc_znunu_abs_  = createSRhisto(t_1l_mc_znunu,lumi,c_SBins_1l,2,2,false,"1l_mc_znunu_abs");    hSR_1l_mc_znunu_abs_->SetName("hSR_1l_mc_znunu_abs_");
  TH1F *hSR_1l_mc_tt_abs_  = createSRhisto(t_1l_mc_tt,lumi,c_SBins_1l,2,2,false,"1l_mc_tt_abs");             hSR_1l_mc_tt_abs_->SetName("hSR_1l_mc_tt_abs_");
  TH1F *hSR_1l_mc_w_abs_   = createSRhisto(t_1l_mc_w,lumi,c_SBins_1l,4,2,false,"1l_mc_w_abs");               hSR_1l_mc_w_abs_->SetName("hSR_1l_mc_w_abs_");
  TH1F *hSR_1l_mc_ttW_abs_ = createSRhisto(t_1l_mc_ttW,lumi,c_SBins_1l,kMagenta+2,2,false,"1l_mc_ttW_abs");  hSR_1l_mc_ttW_abs->SetName("hSR_1l_mc_ttW_abs");
  TH1F *hSR_1l_mc_ttZ_abs_ = createSRhisto(t_1l_mc_ttZ,lumi,c_SBins_1l,kGreen+2,2,false,"1l_mc_ttZ_abs");    hSR_1l_mc_ttZ_abs->SetName("hSR_1l_mc_ttZ_abs");
  TH1F *hSR_1l_mc_T2ttMM_abs_ = createSRhisto(t_1l_mc_T2ttMM,lumi,c_SBins_1l,3,2,false,"1l_mc_T2ttMM_abs_"); hSR_1l_mc_T2ttMM_abs_->SetName("hSR_1l_mc_T2ttMM_abs_");
  TH1F *hSR_1l_mc_T2ttHM_abs_ = createSRhisto(t_1l_mc_T2ttHM,lumi,c_SBins_1l,5,2,false,"1l_mc_T2ttHM_abs_"); hSR_1l_mc_T2ttHM_abs_->SetName("hSR_1l_mc_T2ttHM_abs_");
  TH1F *hSR_1l_mc_T14tMM_abs_ = createSRhisto(t_1l_mc_T14tMM,lumi,c_SBins_1l,6,2,false,"1l_mc_T14tMM_abs_"); hSR_1l_mc_T14tMM_abs_->SetName("hSR_1l_mc_T14tMM_abs_");
  TH1F *hSR_1l_mc_T14tHM_abs_ = createSRhisto(t_1l_mc_T14tHM,lumi,c_SBins_1l,7,2,false,"1l_mc_T14tHM_abs_"); hSR_1l_mc_T14tHM_abs_->SetName("hSR_1l_mc_T14tHM_abs_");

  // decomposition of backgrounds in J=2
  TH1F *hSR_0l_mc_sm_abs_  = createSRhisto(t_0l_mc_sm,lumi,c_SBins_0l,1,1,false,"0l_mc_sm_abs");             hSR_0l_mc_sm_abs_->SetName("hSR_0l_mc_sm_abs_");
  TH1F *hSR_0l_mc_znunu_abs_  = createSRhisto(t_0l_mc_znunu,lumi,c_SBins_0l,2,2,false,"0l_mc_znunu_abs");    hSR_0l_mc_znunu_abs_->SetName("hSR_0l_mc_znunu_abs_");
  TH1F *hSR_0l_mc_tt_abs_  = createSRhisto(t_0l_mc_tt,lumi,c_SBins_0l,2,1,false,"0l_mc_tt_abs");             hSR_0l_mc_tt_abs_->SetName("hSR_0l_mc_tt_abs_");
  TH1F *hSR_0l_mc_w_abs_   = createSRhisto(t_0l_mc_w,lumi,c_SBins_0l,4,1,false,"0l_mc_w_abs");               hSR_0l_mc_w_abs_->SetName("hSR_0l_mc_w_abs_");
  TH1F *hSR_0l_mc_ttW_abs_ = createSRhisto(t_0l_mc_ttW,lumi,c_SBins_0l,kMagenta+2,1,false,"0l_mc_ttW_abs");  hSR_0l_mc_ttW_abs_->SetName("hSR_0l_mc_ttW_abs");
  TH1F *hSR_0l_mc_ttZ_abs_ = createSRhisto(t_0l_mc_ttZ,lumi,c_SBins_0l,kGreen+2,1,false,"0l_mc_ttZ_abs");    hSR_0l_mc_ttZ_abs_->SetName("hSR_0l_mc_ttZ_abs");
  TH1F *hSR_0l_mc_T2ttMM_abs_ = createSRhisto(t_0l_mc_T2ttMM,lumi,c_SBins_0l,3,2,false,"0l_mc_T2ttMM_abs_"); hSR_0l_mc_T2ttMM_abs_->SetName("hSR_0l_mc_T2ttMM_abs_");
  TH1F *hSR_0l_mc_T2ttHM_abs_ = createSRhisto(t_0l_mc_T2ttHM,lumi,c_SBins_0l,5,2,false,"0l_mc_T2ttHM_abs_"); hSR_0l_mc_T2ttHM_abs_->SetName("hSR_0l_mc_T2ttHM_abs_");
  TH1F *hSR_0l_mc_T14tMM_abs_ = createSRhisto(t_0l_mc_T14tMM,lumi,c_SBins_0l,6,2,false,"0l_mc_T14tMM_abs_"); hSR_0l_mc_T14tMM_abs_->SetName("hSR_0l_mc_T14tMM_abs_");
  TH1F *hSR_0l_mc_T14tHM_abs_ = createSRhisto(t_0l_mc_T14tHM,lumi,c_SBins_0l,7,2,false,"0l_mc_T14tHM_abs_"); hSR_0l_mc_T14tHM_abs_->SetName("hSR_0l_mc_T14tHM_abs_");

  // decomposition of backgrounds in J=3
  TH1F *hSR_3l_mc_sm_abs_  = createSRhisto(t_0l_mc_sm,lumi,c_SBins_3l,1,1,false,"0l_mc_sm_abs");             hSR_3l_mc_sm_abs_->SetName("hSR_3l_mc_sm_abs_");
  TH1F *hSR_3l_mc_znunu_abs_  = createSRhisto(t_0l_mc_znunu,lumi,c_SBins_3l,2,2,false,"0l_mc_znunu_abs");    hSR_3l_mc_znunu_abs_->SetName("hSR_3l_mc_znunu_abs_");
  TH1F *hSR_3l_mc_tt_abs_  = createSRhisto(t_0l_mc_tt,lumi,c_SBins_3l,2,1,false,"0l_mc_tt_abs");             hSR_3l_mc_tt_abs_->SetName("hSR_3l_mc_tt_abs_");
  TH1F *hSR_3l_mc_w_abs_   = createSRhisto(t_0l_mc_w,lumi,c_SBins_3l,4,1,false,"0l_mc_w_abs");               hSR_3l_mc_w_abs_->SetName("hSR_3l_mc_w_abs_");
  TH1F *hSR_3l_mc_ttW_abs_ = createSRhisto(t_0l_mc_ttW,lumi,c_SBins_3l,kMagenta+2,1,false,"0l_mc_ttW_abs");  hSR_3l_mc_ttW_abs_->SetName("hSR_3l_mc_ttW_abs");
  TH1F *hSR_3l_mc_ttZ_abs_ = createSRhisto(t_0l_mc_ttZ,lumi,c_SBins_3l,kGreen+2,1,false,"0l_mc_ttZ_abs");    hSR_3l_mc_ttZ_abs_->SetName("hSR_3l_mc_ttZ_abs");
  TH1F *hSR_3l_mc_T2ttMM_abs_ = createSRhisto(t_0l_mc_T2ttMM,lumi,c_SBins_3l,3,2,false,"0l_mc_T2ttMM_abs_"); hSR_3l_mc_T2ttMM_abs_->SetName("hSR_3l_mc_T2ttMM_abs_");
  TH1F *hSR_3l_mc_T2ttHM_abs_ = createSRhisto(t_0l_mc_T2ttHM,lumi,c_SBins_3l,5,2,false,"0l_mc_T2ttHM_abs_"); hSR_3l_mc_T2ttHM_abs_->SetName("hSR_3l_mc_T2ttHM_abs_");
  TH1F *hSR_3l_mc_T14tMM_abs_ = createSRhisto(t_0l_mc_T14tMM,lumi,c_SBins_3l,6,2,false,"0l_mc_T14tMM_abs_"); hSR_3l_mc_T14tMM_abs_->SetName("hSR_3l_mc_T14tMM_abs_");
  TH1F *hSR_3l_mc_T14tHM_abs_ = createSRhisto(t_0l_mc_T14tHM,lumi,c_SBins_3l,7,2,false,"0l_mc_T14tHM_abs_"); hSR_3l_mc_T14tHM_abs_->SetName("hSR_3l_mc_T14tHM_abs_");

  // decomposition of backgrounds in J=4
  TH1F *hSR_4l_mc_sm_abs_  = createSRhisto(t_0l_mc_sm,lumi,c_SBins_4l,1,1,false,"0l_mc_sm_abs");             hSR_4l_mc_sm_abs_->SetName("hSR_4l_mc_sm_abs_");
  TH1F *hSR_4l_mc_znunu_abs_  = createSRhisto(t_0l_mc_znunu,lumi,c_SBins_4l,2,2,false,"0l_mc_znunu_abs");    hSR_4l_mc_znunu_abs_->SetName("hSR_4l_mc_znunu_abs_");
  TH1F *hSR_4l_mc_tt_abs_  = createSRhisto(t_0l_mc_tt,lumi,c_SBins_4l,2,1,false,"0l_mc_tt_abs");             hSR_4l_mc_tt_abs_->SetName("hSR_4l_mc_tt_abs_");
  TH1F *hSR_4l_mc_w_abs_   = createSRhisto(t_0l_mc_w,lumi,c_SBins_4l,4,1,false,"0l_mc_w_abs");               hSR_4l_mc_w_abs_->SetName("hSR_4l_mc_w_abs_");
  TH1F *hSR_4l_mc_ttW_abs_ = createSRhisto(t_0l_mc_ttW,lumi,c_SBins_4l,kMagenta+2,1,false,"0l_mc_ttW_abs");  hSR_4l_mc_ttW_abs_->SetName("hSR_4l_mc_ttW_abs");
  TH1F *hSR_4l_mc_ttZ_abs_ = createSRhisto(t_0l_mc_ttZ,lumi,c_SBins_4l,kGreen+2,1,false,"0l_mc_ttZ_abs");    hSR_4l_mc_ttZ_abs_->SetName("hSR_4l_mc_ttZ_abs");
  TH1F *hSR_4l_mc_T2ttMM_abs_ = createSRhisto(t_0l_mc_T2ttMM,lumi,c_SBins_4l,3,2,false,"0l_mc_T2ttMM_abs_"); hSR_4l_mc_T2ttMM_abs_->SetName("hSR_4l_mc_T2ttMM_abs_");
  TH1F *hSR_4l_mc_T2ttHM_abs_ = createSRhisto(t_0l_mc_T2ttHM,lumi,c_SBins_4l,5,2,false,"0l_mc_T2ttHM_abs_"); hSR_4l_mc_T2ttHM_abs_->SetName("hSR_4l_mc_T2ttHM_abs_");
  TH1F *hSR_4l_mc_T14tMM_abs_ = createSRhisto(t_0l_mc_T14tMM,lumi,c_SBins_4l,6,2,false,"0l_mc_T14tMM_abs_"); hSR_4l_mc_T14tMM_abs_->SetName("hSR_4l_mc_T14tMM_abs_");
  TH1F *hSR_4l_mc_T14tHM_abs_ = createSRhisto(t_0l_mc_T14tHM,lumi,c_SBins_4l,7,2,false,"0l_mc_T14tHM_abs_"); hSR_4l_mc_T14tHM_abs_->SetName("hSR_4l_mc_T14tHM_abs_");

  // decomposition of backgrounds in J>=5
  TH1F *hSR_5l_mc_sm_abs_  = createSRhisto(t_0l_mc_sm,lumi,c_SBins_5l,1,1,false,"0l_mc_sm_abs");             hSR_5l_mc_sm_abs_->SetName("hSR_5l_mc_sm_abs_");
  TH1F *hSR_5l_mc_znunu_abs_  = createSRhisto(t_0l_mc_znunu,lumi,c_SBins_5l,2,2,false,"0l_mc_znunu_abs");    hSR_5l_mc_znunu_abs_->SetName("hSR_5l_mc_znunu_abs_");
  TH1F *hSR_5l_mc_tt_abs_  = createSRhisto(t_0l_mc_tt,lumi,c_SBins_5l,2,1,false,"0l_mc_tt_abs");             hSR_5l_mc_tt_abs_->SetName("hSR_5l_mc_tt_abs_");
  TH1F *hSR_5l_mc_w_abs_   = createSRhisto(t_0l_mc_w,lumi,c_SBins_5l,4,1,false,"0l_mc_w_abs");               hSR_5l_mc_w_abs_->SetName("hSR_5l_mc_w_abs_");
  TH1F *hSR_5l_mc_ttW_abs_ = createSRhisto(t_0l_mc_ttW,lumi,c_SBins_5l,kMagenta+2,1,false,"0l_mc_ttW_abs");  hSR_5l_mc_ttW_abs_->SetName("hSR_5l_mc_ttW_abs");
  TH1F *hSR_5l_mc_ttZ_abs_ = createSRhisto(t_0l_mc_ttZ,lumi,c_SBins_5l,kGreen+2,1,false,"0l_mc_ttZ_abs");    hSR_5l_mc_ttZ_abs_->SetName("hSR_5l_mc_ttZ_abs");
  TH1F *hSR_5l_mc_T2ttMM_abs_ = createSRhisto(t_0l_mc_T2ttMM,lumi,c_SBins_5l,3,2,false,"0l_mc_T2ttMM_abs_"); hSR_5l_mc_T2ttMM_abs_->SetName("hSR_5l_mc_T2ttMM_abs_");
  TH1F *hSR_5l_mc_T2ttHM_abs_ = createSRhisto(t_0l_mc_T2ttHM,lumi,c_SBins_5l,5,2,false,"0l_mc_T2ttHM_abs_"); hSR_5l_mc_T2ttHM_abs_->SetName("hSR_5l_mc_T2ttHM_abs_");
  TH1F *hSR_5l_mc_T14tMM_abs_ = createSRhisto(t_0l_mc_T14tMM,lumi,c_SBins_5l,6,2,false,"0l_mc_T14tMM_abs_"); hSR_5l_mc_T14tMM_abs_->SetName("hSR_5l_mc_T14tMM_abs_");
  TH1F *hSR_5l_mc_T14tHM_abs_ = createSRhisto(t_0l_mc_T14tHM,lumi,c_SBins_5l,7,2,false,"0l_mc_T14tHM_abs_"); hSR_5l_mc_T14tHM_abs_->SetName("hSR_5l_mc_T14tHM_abs_");


  // estimate transfer factor
  TH1F *hTcs         = calculateTcsInMC(hSR_0l_mc_ttall_abs,hSR_1l_mc_ttall_abs); hTcs->SetName("hTcs");
  TH1F *hTcs_mcstats = calcRelUnc(hTcs);                                          hTcs_mcstats->SetName("hTcs_mcstats");

  // make prediction
  TH1F *hPred_Stat = (TH1F*)hTcs->Clone("hPred_Stat");
  TH1F *hPred_Syst = (TH1F*)hTcs->Clone("hPred_Syst");
  TH1F *hPred_r    = (TH1F*)hTcs->Clone("hPred_r");
  prediction(hSR_1l_data_abs,hSR_0l_data_abs,hSR_0l_mc_ttall_abs,hTcs,hTcs_mcstats,hPred_Stat,hPred_Syst,hPred_r);
  

  
  // make plots
  //TCanvas *cPresel_abs = new TCanvas("cPresel_abs","cPresel_abs",500,500);
  //hPresel_1l_mc_ttall_abs->Draw("HIST");
  //hPresel_0l_mc_ttall_abs->Draw("HIST sames");
  //hPresel_1l_mc_ttall_abs->Draw("HIST sames");

  //TCanvas *cSR_abs = new TCanvas("cSR_abs","cSR_abs",500,500);
  //hSR_1l_mc_ttall_abs->Draw("HIST E0");
  //hSR_0l_mc_ttall_abs->Draw("HIST E0 sames");

  //Signal Contamination for 175<MET<225
  TCanvas *cSR_1l_CS_Decomp_abs = new TCanvas("cSR_1l_CS_Decomp_abs","cSR_1l_CS_Decomp_abs",500,500);
  cSR_1l_CS_Decomp_abs->SetLogy();
  //hSR_1l_mc_tt_abs_->Draw("HIST E0");
  //hSR_1l_mc_w_abs_->Draw("HIST E0 sames");
  //hSR_1l_mc_ttW_abs_->Draw("HIST E0 sames");
  //hSR_1l_mc_ttZ_abs_->Draw("HIST E0 sames");
  hSR_1l_mc_sm_abs_->GetYaxis()->SetRangeUser(.01,1000);
  hSR_1l_mc_sm_abs_->SetTitle("Signal Contamination 175<MET<225");
  hSR_1l_mc_sm_abs_->SetLineStyle(1);
  hSR_1l_mc_T2ttMM_abs_->SetLineStyle(1);
  hSR_1l_mc_T2ttHM_abs_->SetLineStyle(1);
  hSR_1l_mc_T14tMM_abs_->SetLineStyle(1);
  hSR_1l_mc_T14tHM_abs_->SetLineStyle(1);
  hSR_1l_mc_sm_abs_->Draw("HIST E0");
  hSR_1l_mc_T2ttMM_abs_->Draw("HIST E0 sames");
  hSR_1l_mc_T2ttHM_abs_->Draw("HIST E0 sames");
  hSR_1l_mc_T14tMM_abs_->Draw("HIST E0 sames");
  hSR_1l_mc_T14tHM_abs_->Draw("HIST E0 sames");
  leg = new TLegend(0.65,0.7,0.9,0.9);
  leg->SetHeader("The Legend");
  leg->AddEntry(hSR_1l_mc_sm_abs_,"SM","f");
  leg->AddEntry(hSR_1l_mc_T2ttMM_abs_,"T2tt 650","f");
  leg->AddEntry(hSR_1l_mc_T2ttHM_abs_,"T2tt 850","f");
  leg->AddEntry(hSR_1l_mc_T14tMM_abs_,"T14t 1200","f");
  leg->AddEntry(hSR_1l_mc_T14tHM_abs_,"T14t 1500","f");
  leg->Draw();

  //Ratio stuff for 175<MET<225
  TCanvas *cSR_LowMET_SigCon = new TCanvas("cSR_LowMET_SigCon","cSR_LowMET_SigCon",500,500);
  cSR_LowMET_SigCon->SetLogy();
  TH1F *hSR_1l_T2ttMM_abs_ = (TH1F*)hSR_1l_mc_T2ttMM_abs_->Clone();
  TH1F *hSR_1l_T2ttHM_abs_ = (TH1F*)hSR_1l_mc_T2ttHM_abs_->Clone();
  TH1F *hSR_1l_T14tMM_abs_ = (TH1F*)hSR_1l_mc_T14tMM_abs_->Clone();
  TH1F *hSR_1l_T14tHM_abs_ = (TH1F*)hSR_1l_mc_T14tHM_abs_->Clone();
  hSR_1l_T2ttMM_abs_->Sumw2();
  hSR_1l_T2ttMM_abs_->SetTitle("Signal Contamination Ratio 175<MET<225");
  hSR_1l_T2ttMM_abs_->Divide(hSR_1l_mc_sm_abs_);
  hSR_1l_T2ttHM_abs_->Divide(hSR_1l_mc_sm_abs_);
  hSR_1l_T14tMM_abs_->Divide(hSR_1l_mc_sm_abs_);
  hSR_1l_T14tHM_abs_->Divide(hSR_1l_mc_sm_abs_);
  TH1F *hSR_1l_SigTot_abs_ = (TH1F*)hSR_1l_T2ttMM_abs_->Clone();
  hSR_1l_SigTot_abs_->Add(hSR_1l_T2ttHM_abs_);
  hSR_1l_SigTot_abs_->Add(hSR_1l_T14tMM_abs_);
  hSR_1l_SigTot_abs_->Add(hSR_1l_T14tHM_abs_);
  hSR_1l_SigTot_abs_->SetLineColor(1);
  hSR_1l_T2ttMM_abs_->SetLineColor(2);
  hSR_1l_T2ttHM_abs_->SetLineColor(3);
  hSR_1l_T14tMM_abs_->SetLineColor(4);
  hSR_1l_T14tHM_abs_->SetLineColor(5);
  hSR_1l_T2ttMM_abs_->SetLineStyle(1);
  hSR_1l_T2ttHM_abs_->SetLineStyle(1);
  hSR_1l_T14tMM_abs_->SetLineStyle(1);
  hSR_1l_T14tHM_abs_->SetLineStyle(1);
  hSR_1l_SigTot_abs_->SetLineStyle(1);
  hSR_1l_T2ttMM_abs_->Draw("HIST E0");
  hSR_1l_T2ttHM_abs_->Draw("HIST E0 sames");
  hSR_1l_T14tMM_abs_->Draw("HIST E0 sames");
  hSR_1l_T14tHM_abs_->Draw("HIST E0 sames");
  hSR_1l_SigTot_abs_->Draw("HIST E0 sames");
  hSR_1l_T2ttMM_abs_->GetYaxis()->SetRangeUser(.0000001,10);
  leg = new TLegend(0.65,0.7,0.9,0.9);
  leg->SetHeader("The Legend");
  leg->AddEntry(hSR_1l_T2ttMM_abs_,"T2tt 650","f");
  leg->AddEntry(hSR_1l_T2ttHM_abs_,"T2tt 850","f");
  leg->AddEntry(hSR_1l_T14tMM_abs_,"T14t 1200","f");
  leg->AddEntry(hSR_1l_T14tHM_abs_,"T14t 1500","f");
  leg->AddEntry(hSR_1l_SigTot_abs_,"Total Signal","f");
  leg->Draw();

  //Signal Contamination for NJets=2
  TCanvas *cSR_0l_Decomp_abs = new TCanvas("cSR_0l_Decomp_abs","cSR_0l_Decomp_abs",500,500);
  cSR_0l_Decomp_abs->SetLogy();
  //hSR_0l_mc_tt_abs_->Draw("HIST E0");
  //hSR_0l_mc_w_abs_->Draw("HIST E0 sames");
  //hSR_0l_mc_ttW_abs_->Draw("HIST E0 sames");
  //hSR_0l_mc_ttZ_abs_->Draw("HIST E0 sames");
  hSR_0l_mc_sm_abs_->GetYaxis()->SetRangeUser(.01,1000);
  hSR_0l_mc_sm_abs_->SetTitle("Signal Contamination NJets=2");
  hSR_0l_mc_sm_abs_->Draw("HIST E0");
  hSR_0l_mc_T2ttMM_abs_->Draw("HIST E0 sames");
  hSR_0l_mc_T2ttHM_abs_->Draw("HIST E0 sames");
  hSR_0l_mc_T14tMM_abs_->Draw("HIST E0 sames");
  hSR_0l_mc_T14tHM_abs_->Draw("HIST E0 sames");
  leg = new TLegend(0.65,0.7,0.9,0.9);
  leg->SetHeader("The Legend");
  leg->AddEntry(hSR_0l_mc_sm_abs_,"SM","f");
  leg->AddEntry(hSR_0l_mc_T2ttMM_abs_,"T2tt 650","f");
  leg->AddEntry(hSR_0l_mc_T2ttHM_abs_,"T2tt 850","f");
  leg->AddEntry(hSR_0l_mc_T14tMM_abs_,"T14t 1200","f");
  leg->AddEntry(hSR_0l_mc_T14tHM_abs_,"T14t 1500","f");
  leg->Draw();

  //Ratio stuff for NJets=2
  TCanvas *cSR_2J_SigCon = new TCanvas("cSR_2J_SigCon","cSR_2J_SigCon",500,500);
  cSR_2J_SigCon->SetLogy();
  TH1F *hSR_2J_T2ttMM_abs_ = (TH1F*)hSR_0l_mc_T2ttMM_abs_->Clone();
  TH1F *hSR_2J_T2ttHM_abs_ = (TH1F*)hSR_0l_mc_T2ttHM_abs_->Clone();
  TH1F *hSR_2J_T14tMM_abs_ = (TH1F*)hSR_0l_mc_T14tMM_abs_->Clone();
  TH1F *hSR_2J_T14tHM_abs_ = (TH1F*)hSR_0l_mc_T14tHM_abs_->Clone();
  hSR_2J_T2ttMM_abs_->Sumw2();
  hSR_2J_T2ttMM_abs_->SetTitle("Signal Contamination Ratio NJets=2");
  hSR_2J_T2ttMM_abs_->Divide(hSR_0l_mc_sm_abs_);
  hSR_2J_T2ttHM_abs_->Divide(hSR_0l_mc_sm_abs_);
  hSR_2J_T14tMM_abs_->Divide(hSR_0l_mc_sm_abs_);
  hSR_2J_T14tHM_abs_->Divide(hSR_0l_mc_sm_abs_);
  TH1F *hSR_2J_SigTot_abs_ = (TH1F*)hSR_2J_T2ttMM_abs_->Clone();
  hSR_2J_SigTot_abs_->Add(hSR_2J_T2ttHM_abs_);
  hSR_2J_SigTot_abs_->Add(hSR_2J_T14tMM_abs_);
  hSR_2J_SigTot_abs_->Add(hSR_2J_T14tHM_abs_);
  hSR_2J_SigTot_abs_->SetLineColor(1);
  hSR_2J_T2ttMM_abs_->SetLineColor(2);
  hSR_2J_T2ttHM_abs_->SetLineColor(3);
  hSR_2J_T14tMM_abs_->SetLineColor(4);
  hSR_2J_T14tHM_abs_->SetLineColor(5);
  hSR_2J_T2ttMM_abs_->SetLineStyle(1);
  hSR_2J_T2ttHM_abs_->SetLineStyle(1);
  hSR_2J_T14tMM_abs_->SetLineStyle(1);
  hSR_2J_T14tHM_abs_->SetLineStyle(1);
  hSR_2J_SigTot_abs_->SetLineStyle(1);
  hSR_2J_T2ttMM_abs_->Draw("HIST E0");
  hSR_2J_T2ttHM_abs_->Draw("HIST E0 sames");
  hSR_2J_T14tMM_abs_->Draw("HIST E0 sames");
  hSR_2J_T14tHM_abs_->Draw("HIST E0 sames");
  hSR_2J_SigTot_abs_->Draw("HIST E0 sames");
  hSR_2J_T2ttMM_abs_->GetYaxis()->SetRangeUser(.0000001,10);
  leg = new TLegend(0.65,0.7,0.9,0.9);
  leg->SetHeader("The Legend");
  leg->AddEntry(hSR_2J_T2ttMM_abs_,"T2tt 650","f");
  leg->AddEntry(hSR_2J_T2ttHM_abs_,"T2tt 850","f");
  leg->AddEntry(hSR_2J_T14tMM_abs_,"T14t 1200","f");
  leg->AddEntry(hSR_2J_T14tHM_abs_,"T14t 1500","f");
  leg->AddEntry(hSR_2J_SigTot_abs_,"Total Signal","f");
  leg->Draw();

  //Signal Contamination for NJets=3
  TCanvas *cSR_3l_Decomp_abs = new TCanvas("cSR_3l_Decomp_abs","cSR_3l_Decomp_abs",500,500);
  cSR_3l_Decomp_abs->SetLogy();
  //hSR_0l_mc_tt_abs_->Draw("HIST E0");
  //hSR_0l_mc_w_abs_->Draw("HIST E0 sames");
  //hSR_0l_mc_ttW_abs_->Draw("HIST E0 sames");
  //hSR_0l_mc_ttZ_abs_->Draw("HIST E0 sames");
  hSR_3l_mc_sm_abs_->GetYaxis()->SetRangeUser(.01,1000);
  hSR_3l_mc_sm_abs_->SetTitle("Signal Contamination NJets=3");
  hSR_3l_mc_sm_abs_->Draw("HIST E0");
  hSR_3l_mc_T2ttMM_abs_->Draw("HIST E0 sames");
  hSR_3l_mc_T2ttHM_abs_->Draw("HIST E0 sames");
  hSR_3l_mc_T14tMM_abs_->Draw("HIST E0 sames");
  hSR_3l_mc_T14tHM_abs_->Draw("HIST E0 sames");
  leg = new TLegend(0.65,0.7,0.9,0.9);
  leg->SetHeader("The Legend");
  leg->AddEntry(hSR_3l_mc_sm_abs_,"SM","f");
  leg->AddEntry(hSR_3l_mc_T2ttMM_abs_,"T2tt 650","f");
  leg->AddEntry(hSR_3l_mc_T2ttHM_abs_,"T2tt 850","f");
  leg->AddEntry(hSR_3l_mc_T14tMM_abs_,"T14t 1200","f");
  leg->AddEntry(hSR_3l_mc_T14tHM_abs_,"T14t 1500","f");
  leg->Draw();

  //Ratio stuff for NJets=3
  TCanvas *cSR_3J_SigCon = new TCanvas("cSR_3J_SigCon","cSR_3J_SigCon",500,500);
  cSR_3J_SigCon->SetLogy();
  TH1F *hSR_3J_T2ttMM_abs_ = (TH1F*)hSR_3l_mc_T2ttMM_abs_->Clone();
  TH1F *hSR_3J_T2ttHM_abs_ = (TH1F*)hSR_3l_mc_T2ttHM_abs_->Clone();
  TH1F *hSR_3J_T14tMM_abs_ = (TH1F*)hSR_3l_mc_T14tMM_abs_->Clone();
  TH1F *hSR_3J_T14tHM_abs_ = (TH1F*)hSR_3l_mc_T14tHM_abs_->Clone();
  hSR_3J_T2ttMM_abs_->Sumw2();
  hSR_3J_T2ttMM_abs_->SetTitle("Signal Contamination Ratio NJets=3");
  hSR_3J_T2ttMM_abs_->Divide(hSR_3l_mc_sm_abs_);
  hSR_3J_T2ttHM_abs_->Divide(hSR_3l_mc_sm_abs_);
  hSR_3J_T14tMM_abs_->Divide(hSR_3l_mc_sm_abs_);
  hSR_3J_T14tHM_abs_->Divide(hSR_3l_mc_sm_abs_);
  TH1F *hSR_3J_SigTot_abs_ = (TH1F*)hSR_3J_T2ttMM_abs_->Clone();
  hSR_3J_SigTot_abs_->Add(hSR_3J_T2ttHM_abs_);
  hSR_3J_SigTot_abs_->Add(hSR_3J_T14tMM_abs_);
  hSR_3J_SigTot_abs_->Add(hSR_3J_T14tHM_abs_);
  hSR_3J_SigTot_abs_->SetLineColor(1);
  hSR_3J_T2ttMM_abs_->SetLineColor(2);
  hSR_3J_T2ttHM_abs_->SetLineColor(3);
  hSR_3J_T14tMM_abs_->SetLineColor(4);
  hSR_3J_T14tHM_abs_->SetLineColor(5);
  hSR_3J_T2ttMM_abs_->SetLineStyle(1);
  hSR_3J_T2ttHM_abs_->SetLineStyle(1);
  hSR_3J_T14tMM_abs_->SetLineStyle(1);
  hSR_3J_T14tHM_abs_->SetLineStyle(1);
  hSR_3J_SigTot_abs_->SetLineStyle(1);
  hSR_3J_T2ttMM_abs_->Draw("HIST E0");
  hSR_3J_T2ttHM_abs_->Draw("HIST E0 sames");
  hSR_3J_T14tMM_abs_->Draw("HIST E0 sames");
  hSR_3J_T14tHM_abs_->Draw("HIST E0 sames");
  hSR_3J_SigTot_abs_->Draw("HIST E0 sames");
  hSR_3J_T2ttMM_abs_->GetYaxis()->SetRangeUser(.0000001,10);
  leg = new TLegend(0.65,0.7,0.9,0.9);
  leg->SetHeader("The Legend");
  leg->AddEntry(hSR_3J_T2ttMM_abs_,"T2tt 650","f");
  leg->AddEntry(hSR_3J_T2ttHM_abs_,"T2tt 850","f");
  leg->AddEntry(hSR_3J_T14tMM_abs_,"T14t 1200","f");
  leg->AddEntry(hSR_3J_T14tHM_abs_,"T14t 1500","f");
  leg->AddEntry(hSR_3J_SigTot_abs_,"Total Signal","f");
  leg->Draw();

  //Signal Contamination for NJets=4
  TCanvas *cSR_4l_Decomp_abs = new TCanvas("cSR_4l_Decomp_abs","cSR_4l_Decomp_abs",500,500);
  cSR_4l_Decomp_abs->SetLogy();
  //hSR_0l_mc_tt_abs_->Draw("HIST E0");
  //hSR_0l_mc_w_abs_->Draw("HIST E0 sames");
  //hSR_0l_mc_ttW_abs_->Draw("HIST E0 sames");
  //hSR_0l_mc_ttZ_abs_->Draw("HIST E0 sames");
  hSR_4l_mc_sm_abs_->GetYaxis()->SetRangeUser(.01,1000);
  hSR_4l_mc_sm_abs_->SetTitle("Signal Contamination NJets=4");
  hSR_4l_mc_sm_abs_->Draw("HIST E0");
  hSR_4l_mc_T2ttMM_abs_->Draw("HIST E0 sames");
  hSR_4l_mc_T2ttHM_abs_->Draw("HIST E0 sames");
  hSR_4l_mc_T14tMM_abs_->Draw("HIST E0 sames");
  hSR_4l_mc_T14tHM_abs_->Draw("HIST E0 sames");
  leg = new TLegend(0.65,0.7,0.9,0.9);
  leg->SetHeader("The Legend");
  leg->AddEntry(hSR_4l_mc_sm_abs_,"SM","f");
  leg->AddEntry(hSR_4l_mc_T2ttMM_abs_,"T2tt 650","f");
  leg->AddEntry(hSR_4l_mc_T2ttHM_abs_,"T2tt 850","f");
  leg->AddEntry(hSR_4l_mc_T14tMM_abs_,"T14t 1200","f");
  leg->AddEntry(hSR_4l_mc_T14tHM_abs_,"T14t 1500","f");
  leg->Draw();

  //Ratio stuff for NJets=4
  TCanvas *cSR_4J_SigCon = new TCanvas("cSR_4J_SigCon","cSR_4J_SigCon",500,500);
  cSR_4J_SigCon->SetLogy();
  TH1F *hSR_4J_T2ttMM_abs_ = (TH1F*)hSR_4l_mc_T2ttMM_abs_->Clone();
  TH1F *hSR_4J_T2ttHM_abs_ = (TH1F*)hSR_4l_mc_T2ttHM_abs_->Clone();
  TH1F *hSR_4J_T14tMM_abs_ = (TH1F*)hSR_4l_mc_T14tMM_abs_->Clone();
  TH1F *hSR_4J_T14tHM_abs_ = (TH1F*)hSR_4l_mc_T14tHM_abs_->Clone();
  hSR_4J_T2ttMM_abs_->Sumw2();
  hSR_4J_T2ttMM_abs_->SetTitle("Signal Contamination Ratio NJets=4");
  hSR_4J_T2ttMM_abs_->Divide(hSR_4l_mc_sm_abs_);
  hSR_4J_T2ttHM_abs_->Divide(hSR_4l_mc_sm_abs_);
  hSR_4J_T14tMM_abs_->Divide(hSR_4l_mc_sm_abs_);
  hSR_4J_T14tHM_abs_->Divide(hSR_4l_mc_sm_abs_);
  TH1F *hSR_4J_SigTot_abs_ = (TH1F*)hSR_4J_T2ttMM_abs_->Clone();
  hSR_4J_SigTot_abs_->Add(hSR_4J_T2ttHM_abs_);
  hSR_4J_SigTot_abs_->Add(hSR_4J_T14tMM_abs_);
  hSR_4J_SigTot_abs_->Add(hSR_4J_T14tHM_abs_);
  hSR_4J_SigTot_abs_->SetLineColor(1);
  hSR_4J_T2ttMM_abs_->SetLineColor(2);
  hSR_4J_T2ttHM_abs_->SetLineColor(3);
  hSR_4J_T14tMM_abs_->SetLineColor(4);
  hSR_4J_T14tHM_abs_->SetLineColor(5);
  hSR_4J_T2ttMM_abs_->SetLineStyle(1);
  hSR_4J_T2ttHM_abs_->SetLineStyle(1);
  hSR_4J_T14tMM_abs_->SetLineStyle(1);
  hSR_4J_T14tHM_abs_->SetLineStyle(1);
  hSR_4J_SigTot_abs_->SetLineStyle(1);
  hSR_4J_T2ttMM_abs_->Draw("HIST E0");
  hSR_4J_T2ttHM_abs_->Draw("HIST E0 sames");
  hSR_4J_T14tMM_abs_->Draw("HIST E0 sames");
  hSR_4J_T14tHM_abs_->Draw("HIST E0 sames");
  hSR_4J_SigTot_abs_->Draw("HIST E0 sames");
  hSR_4J_T2ttMM_abs_->GetYaxis()->SetRangeUser(.0000001,10);
  leg = new TLegend(0.65,0.7,0.9,0.9);
  leg->SetHeader("The Legend");
  leg->AddEntry(hSR_4J_T2ttMM_abs_,"T2tt 650","f");
  leg->AddEntry(hSR_4J_T2ttHM_abs_,"T2tt 850","f");
  leg->AddEntry(hSR_4J_T14tMM_abs_,"T14t 1200","f");
  leg->AddEntry(hSR_4J_T14tHM_abs_,"T14t 1500","f");
  leg->AddEntry(hSR_4J_SigTot_abs_,"Total Signal","f");
  leg->Draw();

  //Signal Contamination for NJets>=5
  TCanvas *cSR_5l_Decomp_abs = new TCanvas("cSR_5l_Decomp_abs","cSR_5l_Decomp_abs",500,500);
  cSR_5l_Decomp_abs->SetLogy();
  //hSR_0l_mc_tt_abs_->Draw("HIST E0");
  //hSR_0l_mc_w_abs_->Draw("HIST E0 sames");
  //hSR_0l_mc_ttW_abs_->Draw("HIST E0 sames");
  //hSR_0l_mc_ttZ_abs_->Draw("HIST E0 sames");
  hSR_5l_mc_sm_abs_->GetYaxis()->SetRangeUser(.01,1000);
  hSR_5l_mc_sm_abs_->SetTitle("Signal Contamination NJets>=5");
  hSR_5l_mc_sm_abs_->Draw("HIST E0");
  hSR_5l_mc_T2ttMM_abs_->Draw("HIST E0 sames");
  hSR_5l_mc_T2ttHM_abs_->Draw("HIST E0 sames");
  hSR_5l_mc_T14tMM_abs_->Draw("HIST E0 sames");
  hSR_5l_mc_T14tHM_abs_->Draw("HIST E0 sames");
  leg = new TLegend(0.65,0.7,0.9,0.9);
  leg->SetHeader("The Legend");
  leg->AddEntry(hSR_5l_mc_sm_abs_,"SM","f");
  leg->AddEntry(hSR_5l_mc_T2ttMM_abs_,"T2tt 650","f");
  leg->AddEntry(hSR_5l_mc_T2ttHM_abs_,"T2tt 850","f");
  leg->AddEntry(hSR_5l_mc_T14tMM_abs_,"T14t 1200","f");
  leg->AddEntry(hSR_5l_mc_T14tHM_abs_,"T14t 1500","f");
  leg->Draw();

  //Ratio stuff for NJets>=5
  TCanvas *cSR_5J_SigCon = new TCanvas("cSR_5J_SigCon","cSR_5J_SigCon",500,500);
  cSR_5J_SigCon->SetLogy();
  TH1F *hSR_5J_T2ttMM_abs_ = (TH1F*)hSR_5l_mc_T2ttMM_abs_->Clone();
  TH1F *hSR_5J_T2ttHM_abs_ = (TH1F*)hSR_5l_mc_T2ttHM_abs_->Clone();
  TH1F *hSR_5J_T14tMM_abs_ = (TH1F*)hSR_5l_mc_T14tMM_abs_->Clone();
  TH1F *hSR_5J_T14tHM_abs_ = (TH1F*)hSR_5l_mc_T14tHM_abs_->Clone();
  hSR_5J_T2ttMM_abs_->Sumw2();
  hSR_5J_T2ttMM_abs_->SetTitle("Signal Contamination Ratio NJets>=5");
  hSR_5J_T2ttMM_abs_->Divide(hSR_5l_mc_sm_abs_);
  hSR_5J_T2ttHM_abs_->Divide(hSR_5l_mc_sm_abs_);
  hSR_5J_T14tMM_abs_->Divide(hSR_5l_mc_sm_abs_);
  hSR_5J_T14tHM_abs_->Divide(hSR_5l_mc_sm_abs_);
  TH1F *hSR_5J_SigTot_abs_ = (TH1F*)hSR_5J_T2ttMM_abs_->Clone();
  hSR_5J_SigTot_abs_->Add(hSR_5J_T2ttHM_abs_);
  hSR_5J_SigTot_abs_->Add(hSR_5J_T14tMM_abs_);
  hSR_5J_SigTot_abs_->Add(hSR_5J_T14tHM_abs_);
  hSR_5J_SigTot_abs_->SetLineColor(1);
  hSR_5J_T2ttMM_abs_->SetLineColor(2);
  hSR_5J_T2ttHM_abs_->SetLineColor(3);
  hSR_5J_T14tMM_abs_->SetLineColor(4);
  hSR_5J_T14tHM_abs_->SetLineColor(5);
  hSR_5J_T2ttMM_abs_->SetLineStyle(1);
  hSR_5J_T2ttHM_abs_->SetLineStyle(1);
  hSR_5J_T14tMM_abs_->SetLineStyle(1);
  hSR_5J_T14tHM_abs_->SetLineStyle(1);
  hSR_5J_SigTot_abs_->SetLineStyle(1);
  hSR_5J_T2ttMM_abs_->Draw("HIST E0");
  hSR_5J_T2ttHM_abs_->Draw("HIST E0 sames");
  hSR_5J_T14tMM_abs_->Draw("HIST E0 sames");
  hSR_5J_T14tHM_abs_->Draw("HIST E0 sames");
  hSR_5J_SigTot_abs_->Draw("HIST E0 sames");
  hSR_5J_T2ttMM_abs_->GetYaxis()->SetRangeUser(.0000001,10);
  leg = new TLegend(0.65,0.7,0.9,0.9);
  leg->SetHeader("The Legend");
  leg->AddEntry(hSR_5J_T2ttMM_abs_,"T2tt 650","f");
  leg->AddEntry(hSR_5J_T2ttHM_abs_,"T2tt 850","f");
  leg->AddEntry(hSR_5J_T14tMM_abs_,"T14t 1200","f");
  leg->AddEntry(hSR_5J_T14tHM_abs_,"T14t 1500","f");
  leg->AddEntry(hSR_5J_SigTot_abs_,"Total Signal","f");
  leg->Draw();

  //SM Processes for 175<MET<225
  TCanvas *cSR_1l_Decomp_sm = new TCanvas("cSR_1l_Decomp_sm","cSR_1l_Decomp_sm",500,500);
  cSR_1l_Decomp_sm->SetLogy();
  hSR_1l_mc_tt_abs_->Sumw2();
  hSR_1l_mc_tt_abs_->SetTitle("SM Processes 175<MET<225");
  hSR_1l_mc_tt_abs_->Divide(hSR_1l_mc_sm_abs_);
  hSR_1l_mc_ttZ_abs_->Divide(hSR_1l_mc_sm_abs_);
  hSR_1l_mc_ttW_abs_->Divide(hSR_1l_mc_sm_abs_);
  hSR_1l_mc_znunu_abs_->Divide(hSR_1l_mc_sm_abs_);
  hSR_1l_mc_w_abs_->Divide(hSR_1l_mc_sm_abs_);
  TH1F *hSR_1l_mc_smCheck_abs_ = (TH1F*)hSR_1l_mc_tt_abs_->Clone();
  hSR_1l_mc_smCheck_abs_->Add(hSR_1l_mc_ttZ_abs_);
  hSR_1l_mc_smCheck_abs_->Add(hSR_1l_mc_ttW_abs_);
  hSR_1l_mc_smCheck_abs_->Add(hSR_1l_mc_w_abs_);
  hSR_1l_mc_smCheck_abs_->Add(hSR_1l_mc_znunu_abs_);
  double maxSMCheck1l = hSR_1l_mc_smCheck_abs_->GetMaximum(2);
  double minSMCheck1l = hSR_1l_mc_smCheck_abs_->GetMinimum(0.5);
  cout << maxSMCheck1l << endl;
  cout << minSMCheck1l << endl;
  hSR_1l_mc_tt_abs_->SetLineColor(1);
  hSR_1l_mc_tt_abs_->SetLineStyle(1);
  hSR_1l_mc_ttW_abs_->SetLineStyle(1);
  hSR_1l_mc_ttZ_abs_->SetLineStyle(1);
  hSR_1l_mc_w_abs_->SetLineStyle(1);
  hSR_1l_mc_znunu_abs_->SetLineStyle(1);
  hSR_1l_mc_tt_abs_->Draw("HIST E0");
  hSR_1l_mc_w_abs_->Draw("HIST E0 sames");
  hSR_1l_mc_ttW_abs_->Draw("HIST E0 sames");
  hSR_1l_mc_ttZ_abs_->Draw("HIST E0 sames");
  hSR_1l_mc_znunu_abs_->Draw("HIST E0 sames");
//  if(hSR_1l_mc_smCheck_abs_ == (hSR_1l_mc_sm_abs_->Divide(hSR_1l_mc_sm_abs_))) {
//     hSR_1l_mc_smCheck_abs_->Draw("HIST E0 sames"); }
  leg = new TLegend(0.75,0.7,0.9,0.9);
  hSR_1l_mc_tt_abs_->GetYaxis()->SetRangeUser(.000001,2);
  leg->AddEntry(hSR_1l_mc_tt_abs_,"tt","f");  
  leg->AddEntry(hSR_1l_mc_w_abs_,"W","f");
  leg->AddEntry(hSR_1l_mc_ttW_abs_,"ttW","f");
  leg->AddEntry(hSR_1l_mc_ttZ_abs_,"ttZ","f");
  leg->AddEntry(hSR_1l_mc_znunu_abs_,"Znunu","f");
//  leg->AddEntry(hSR_1l_mc_smCheck_abs_,"SMCheck","f");
  leg->Draw();

  //SM Processes for NJets=2
  TCanvas *cSR_0l_Decomp_sm = new TCanvas("cSR_0l_Decomp_sm","cSR_0l_Decomp_sm",500,500);
  cSR_0l_Decomp_sm->SetLogy();
  hSR_0l_mc_tt_abs_->Sumw2();
  hSR_0l_mc_tt_abs_->SetTitle("SM Processes NJets=2");
  hSR_0l_mc_tt_abs_->Divide(hSR_0l_mc_sm_abs_);
  hSR_0l_mc_ttZ_abs_->Divide(hSR_0l_mc_sm_abs_);
  hSR_0l_mc_ttW_abs_->Divide(hSR_0l_mc_sm_abs_);
  hSR_0l_mc_znunu_abs_->Divide(hSR_0l_mc_sm_abs_);
  hSR_0l_mc_w_abs_->Divide(hSR_0l_mc_sm_abs_);
  TH1F *hSR_0l_mc_smCheck_abs_ = (TH1F*)hSR_0l_mc_tt_abs_->Clone();
  hSR_0l_mc_smCheck_abs_->Add(hSR_0l_mc_ttZ_abs_);
  hSR_0l_mc_smCheck_abs_->Add(hSR_0l_mc_ttW_abs_);
  hSR_0l_mc_smCheck_abs_->Add(hSR_0l_mc_w_abs_);
  hSR_0l_mc_smCheck_abs_->Add(hSR_0l_mc_znunu_abs_);
  double maxSMCheck0l = hSR_0l_mc_smCheck_abs_->GetMaximum(2);
  double minSMCheck0l = hSR_0l_mc_smCheck_abs_->GetMinimum(0.5);
  cout << maxSMCheck0l << endl;
  cout << minSMCheck0l << endl;
//  hSR_0l_mc_smCheck_abs_->SetColor(1);
  hSR_0l_mc_tt_abs_->Draw("HIST E0");
  hSR_0l_mc_w_abs_->Draw("HIST E0 sames");
  hSR_0l_mc_ttW_abs_->Draw("HIST E0 sames");
  hSR_0l_mc_ttZ_abs_->Draw("HIST E0 sames");
  hSR_0l_mc_znunu_abs_->Draw("HIST E0 sames");
//  if(hSR_0l_mc_smCheck_abs_ == (hSR_0l_mc_sm_abs_->Divide(hSR_0l_mc_sm_abs_))) {
//     hSR_0l_mc_smCheck_abs_->Draw("HIST E0 sames"); }
  leg = new TLegend(0.75,0.7,0.9,0.9);
  hSR_0l_mc_tt_abs_->GetYaxis()->SetRangeUser(.000001,2);
  leg->AddEntry(hSR_0l_mc_tt_abs_,"tt","f");  
  leg->AddEntry(hSR_0l_mc_w_abs_,"W","f");
  leg->AddEntry(hSR_0l_mc_ttW_abs_,"ttW","f");
  leg->AddEntry(hSR_0l_mc_ttZ_abs_,"ttZ","f");
  leg->AddEntry(hSR_0l_mc_znunu_abs_,"Znunu","f");
//  leg->AddEntry(hSR_1l_mc_smCheck_abs_,"SMCheck","f");
  leg->Draw();

  //SM Processes for NJets=3
  TCanvas *cSR_3l_Decomp_sm = new TCanvas("cSR_3l_Decomp_sm","cSR_3l_Decomp_sm",500,500);
  cSR_3l_Decomp_sm->SetLogy();
  hSR_3l_mc_tt_abs_->Sumw2();
  hSR_3l_mc_tt_abs_->SetTitle("SM Processes NJets=3");
  hSR_3l_mc_tt_abs_->Divide(hSR_3l_mc_sm_abs_);
  hSR_3l_mc_ttZ_abs_->Divide(hSR_3l_mc_sm_abs_);
  hSR_3l_mc_ttW_abs_->Divide(hSR_3l_mc_sm_abs_);
  hSR_3l_mc_znunu_abs_->Divide(hSR_3l_mc_sm_abs_);
  hSR_3l_mc_w_abs_->Divide(hSR_3l_mc_sm_abs_);
  TH1F *hSR_3l_mc_smCheck_abs_ = (TH1F*)hSR_3l_mc_tt_abs_->Clone();
  hSR_3l_mc_smCheck_abs_->Add(hSR_3l_mc_ttZ_abs_);
  hSR_3l_mc_smCheck_abs_->Add(hSR_3l_mc_ttW_abs_);
  hSR_3l_mc_smCheck_abs_->Add(hSR_3l_mc_w_abs_);
  hSR_3l_mc_smCheck_abs_->Add(hSR_3l_mc_znunu_abs_);
  double maxSMCheck3l = hSR_3l_mc_smCheck_abs_->GetMaximum(2);
  double minSMCheck3l = hSR_3l_mc_smCheck_abs_->GetMinimum(0.5);
  cout << maxSMCheck3l << endl;
  cout << minSMCheck3l << endl;
//  hSR_0l_mc_smCheck_abs_->SetColor(1);
  hSR_3l_mc_tt_abs_->Draw("HIST E0");
  hSR_3l_mc_w_abs_->Draw("HIST E0 sames");
  hSR_3l_mc_ttW_abs_->Draw("HIST E0 sames");
  hSR_3l_mc_ttZ_abs_->Draw("HIST E0 sames");
  hSR_3l_mc_znunu_abs_->Draw("HIST E0 sames");
//  if(hSR_0l_mc_smCheck_abs_ == (hSR_0l_mc_sm_abs_->Divide(hSR_0l_mc_sm_abs_))) {
//     hSR_0l_mc_smCheck_abs_->Draw("HIST E0 sames"); }
  leg = new TLegend(0.75,0.7,0.9,0.9);
  hSR_3l_mc_tt_abs_->GetYaxis()->SetRangeUser(.000001,2);
  leg->AddEntry(hSR_3l_mc_tt_abs_,"tt","f");  
  leg->AddEntry(hSR_3l_mc_w_abs_,"W","f");
  leg->AddEntry(hSR_3l_mc_ttW_abs_,"ttW","f");
  leg->AddEntry(hSR_3l_mc_ttZ_abs_,"ttZ","f");
  leg->AddEntry(hSR_3l_mc_znunu_abs_,"Znunu","f");
//  leg->AddEntry(hSR_1l_mc_smCheck_abs_,"SMCheck","f");
  leg->Draw();

  //SM Processes for NJets=4
  TCanvas *cSR_4l_Decomp_sm = new TCanvas("cSR_4l_Decomp_sm","cSR_4l_Decomp_sm",500,500);
  cSR_4l_Decomp_sm->SetLogy();
  hSR_4l_mc_tt_abs_->Sumw2();
  hSR_4l_mc_tt_abs_->SetTitle("SM Processes NJets=4");
  hSR_4l_mc_tt_abs_->Divide(hSR_4l_mc_sm_abs_);
  hSR_4l_mc_ttZ_abs_->Divide(hSR_4l_mc_sm_abs_);
  hSR_4l_mc_ttW_abs_->Divide(hSR_4l_mc_sm_abs_);
  hSR_4l_mc_znunu_abs_->Divide(hSR_4l_mc_sm_abs_);
  hSR_4l_mc_w_abs_->Divide(hSR_4l_mc_sm_abs_);
  TH1F *hSR_4l_mc_smCheck_abs_ = (TH1F*)hSR_4l_mc_tt_abs_->Clone();
  hSR_4l_mc_smCheck_abs_->Add(hSR_4l_mc_ttZ_abs_);
  hSR_4l_mc_smCheck_abs_->Add(hSR_4l_mc_ttW_abs_);
  hSR_4l_mc_smCheck_abs_->Add(hSR_4l_mc_w_abs_);
  hSR_4l_mc_smCheck_abs_->Add(hSR_4l_mc_znunu_abs_);
  double maxSMCheck4l = hSR_4l_mc_smCheck_abs_->GetMaximum(2);
  double minSMCheck4l = hSR_4l_mc_smCheck_abs_->GetMinimum(0.5);
  cout << maxSMCheck4l << endl;
  cout << minSMCheck4l << endl;
//  hSR_0l_mc_smCheck_abs_->SetColor(1);
  hSR_4l_mc_tt_abs_->Draw("HIST E0");
  hSR_4l_mc_w_abs_->Draw("HIST E0 sames");
  hSR_4l_mc_ttW_abs_->Draw("HIST E0 sames");
  hSR_4l_mc_ttZ_abs_->Draw("HIST E0 sames");
  hSR_4l_mc_znunu_abs_->Draw("HIST E0 sames");
//  if(hSR_0l_mc_smCheck_abs_ == (hSR_0l_mc_sm_abs_->Divide(hSR_0l_mc_sm_abs_))) {
//     hSR_0l_mc_smCheck_abs_->Draw("HIST E0 sames"); }
  leg = new TLegend(0.75,0.7,0.9,0.9);
  hSR_4l_mc_tt_abs_->GetYaxis()->SetRangeUser(.000001,2);
  leg->AddEntry(hSR_4l_mc_tt_abs_,"tt","f");  
  leg->AddEntry(hSR_4l_mc_w_abs_,"W","f");
  leg->AddEntry(hSR_4l_mc_ttW_abs_,"ttW","f");
  leg->AddEntry(hSR_4l_mc_ttZ_abs_,"ttZ","f");
  leg->AddEntry(hSR_4l_mc_znunu_abs_,"Znunu","f");
//  leg->AddEntry(hSR_1l_mc_smCheck_abs_,"SMCheck","f");
  leg->Draw();

  //SM Processes for NJets>=5
  TCanvas *cSR_5l_Decomp_sm = new TCanvas("cSR_5l_Decomp_sm","cSR_5l_Decomp_sm",500,500);
  cSR_5l_Decomp_sm->SetLogy();
  hSR_5l_mc_tt_abs_->Sumw2();
  hSR_5l_mc_tt_abs_->SetTitle("SM Processes NJets>=5");
  hSR_5l_mc_tt_abs_->Divide(hSR_5l_mc_sm_abs_);
  hSR_5l_mc_ttZ_abs_->Divide(hSR_5l_mc_sm_abs_);
  hSR_5l_mc_ttW_abs_->Divide(hSR_5l_mc_sm_abs_);
  hSR_5l_mc_znunu_abs_->Divide(hSR_5l_mc_sm_abs_);
  hSR_5l_mc_w_abs_->Divide(hSR_5l_mc_sm_abs_);
  TH1F *hSR_5l_mc_smCheck_abs_ = (TH1F*)hSR_5l_mc_tt_abs_->Clone();
  hSR_5l_mc_smCheck_abs_->Add(hSR_5l_mc_ttZ_abs_);
  hSR_5l_mc_smCheck_abs_->Add(hSR_5l_mc_ttW_abs_);
  hSR_5l_mc_smCheck_abs_->Add(hSR_5l_mc_w_abs_);
  hSR_5l_mc_smCheck_abs_->Add(hSR_5l_mc_znunu_abs_);
  double maxSMCheck5l = hSR_5l_mc_smCheck_abs_->GetMaximum(2);
  double minSMCheck5l = hSR_5l_mc_smCheck_abs_->GetMinimum(0.5);
  cout << maxSMCheck5l << endl;
  cout << minSMCheck5l << endl;
//  hSR_0l_mc_smCheck_abs_->SetColor(1);
  hSR_5l_mc_tt_abs_->Draw("HIST E0");
  hSR_5l_mc_w_abs_->Draw("HIST E0 sames");
  hSR_5l_mc_ttW_abs_->Draw("HIST E0 sames");
  hSR_5l_mc_ttZ_abs_->Draw("HIST E0 sames");
  hSR_5l_mc_znunu_abs_->Draw("HIST E0 sames");
//  if(hSR_0l_mc_smCheck_abs_ == (hSR_0l_mc_sm_abs_->Divide(hSR_0l_mc_sm_abs_))) {
//     hSR_0l_mc_smCheck_abs_->Draw("HIST E0 sames"); }
  leg = new TLegend(0.75,0.7,0.9,0.9);
  hSR_5l_mc_tt_abs_->GetYaxis()->SetRangeUser(.000001,2);
  leg->AddEntry(hSR_5l_mc_tt_abs_,"tt","f");  
  leg->AddEntry(hSR_5l_mc_w_abs_,"W","f");
  leg->AddEntry(hSR_5l_mc_ttW_abs_,"ttW","f");
  leg->AddEntry(hSR_5l_mc_ttZ_abs_,"ttZ","f");
  leg->AddEntry(hSR_5l_mc_znunu_abs_,"Znunu","f");
//  leg->AddEntry(hSR_1l_mc_smCheck_abs_,"SMCheck","f");
  leg->Draw();


  //TCanvas *cTcs = new TCanvas("cTcs","cTcs",500,500);
  //hTcs->Draw("HIST E0"); 

  //TCanvas *cTcs_mcstats = new TCanvas("cTcs_mcstats","cTcs_mcstats",500,500);
  //hTcs_mcstats->Draw("P");

  //  TCanvas *cPrediction = new TCanvas("cPrediction","cPrediction",1500,500);
  //TCanvas *cPrediction = createCanvas("cPrediction");
  //pMain->cd();
  //hPred_Syst->Draw("E2"); 
  //hPred_Stat->Draw("E1 sames"); 
  //hSR_0l_data_abs->SetLineColor(4); hSR_0l_data_abs->SetMarkerColor(4); hSR_0l_data_abs->SetMarkerColor(4); hSR_0l_data_abs->SetMarkerStyle(20); hSR_0l_data_abs->SetMarkerSize(1.5);
  //hSR_0l_data_abs->Draw("P sames");
  //pRatio->cd();
  //hPred_r->Draw("P E0");
  



  // comparison plots after preselection
  //  TCanvas *cMET_presel       = createCanvas(t_0l_mc_ttall,t_0l_data,lumi,c_presel_0l,c_presel_0l,"MET",20,100.,900.,true,savePlots,useLog,strLog,"5GeVMultiIsoL-WithLepJetClean-Presel-MET-tt");
  //  TCanvas *cNJ_presel        = createCanvas(t_0l_mc_ttall,t_0l_data,lumi,c_presel_0l,c_presel_0l,"NJets",8,3.5,11.5,true,savePlots,useLog,strLog,"5GeVMultiIsoL-WithLepJetClean-Presel-NJ-tt");
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
/*  TCanvas *cMET_Nb1Ntop0 = createCanvas(t_0l_mc_ttall,t_1l_mc_ttall,lumi,c_Nb1NCTT0_2J,c_Nb1NCTT0_LowMET,"met",5,200.,700.,true,savePlots,useLog,strLog,"5GeVMultiIsoL-WithLepJetClean-Nb1Ntop0-MET-tt");
  TCanvas *cMET_Nb1Ntop1 = createCanvas(t_0l_mc_ttall,t_1l_mc_ttall,lumi,c_Nb1NCTT1_2J,c_Nb1NCTT1_LowMET,"met",5,200.,700.,true,savePlots,useLog,strLog,"5GeVMultiIsoL-WithLepJetClean-Nb1Ntop1-MET-tt");
  TCanvas *cMET_Nb2Ntop0 = createCanvas(t_0l_mc_ttall,t_1l_mc_ttall,lumi,c_Nb2NCTT0_2J,c_Nb2NCTT0_LowMET,"met",5,200.,700.,true,savePlots,useLog,strLog,"5GeVMultiIsoL-WithLepJetClean-Nb2Ntop0-MET-tt");
  TCanvas *cMET_Nb2Ntop1 = createCanvas(t_0l_mc_ttall,t_1l_mc_ttall,lumi,c_Nb2NCTT1_2J,c_Nb2NCTT1_LowMET,"met",5,200.,700.,true,savePlots,useLog,strLog,"5GeVMultiIsoL-WithLepJetClean-Nb2Ntop1-MET-tt");*/
  // ====  
  
} // end of void mainfunction()


void prediction(TH1F *h_1lCS_data,TH1F *h_0l_data,TH1F *h_0l_mc,TH1F *hTcs_mc,TH1F *hTcs_mcstats,TH1F *hPred_Stat,TH1F *hPred_Syst,TH1F *hPred_r) {
  TH1::SetDefaultSumw2(kTRUE);

  hPred_Stat->Multiply(h_1lCS_data,hTcs_mc);
  hPred_Syst->Multiply(h_1lCS_data,hTcs_mc);

  hPred_Stat->SetLineWidth(3); hPred_Stat->SetLineColor(1); 
  hPred_Syst->SetLineWidth(3); hPred_Syst->SetLineColor(2); hPred_Syst->SetFillColor(2); hPred_Syst->SetFillStyle(3001);

  for (unsigned int i0=0; i0<hPred_Stat->GetNbinsX(); ++i0) {

    float tcs_     = hTcs_mc->GetBinContent(i0+1);
    float pred_    = hPred_Stat->GetBinContent(i0+1);
    float obs_     = h_0l_data->GetBinContent(i0+1); 
    float statUnc_ = sqrt(h_1lCS_data->GetBinContent(i0+1));
    float systUnc_ = hTcs_mcstats->GetBinContent(i0+1);
    float totUnc_  = sqrt(pow(statUnc_,2)+pow(systUnc_,2));

    hPred_Stat->SetBinError(i0+1,(statUnc_*tcs_));
    hPred_Syst->SetBinError(i0+1,pred_*systUnc_);
    hPred_r->SetBinContent(i0+1,pred_/obs_); 
    hPred_r->SetBinError(i0+1,totUnc_/obs_); if (obs_==0) { hPred_r->SetBinContent(i0+1,0.); hPred_r->SetBinError(i0+1,0.); }
    h_0l_data->SetBinError(i0+1,0.); // make it optional

 
    cout << " data(CR) tcs data(SR) mc(SR) pred +/- stat +/- syst : " 
	 << h_1lCS_data->GetBinContent(i0+1) << " " 
	 << hTcs_mc->GetBinContent(i0+1)     << " "
	 << h_0l_data->GetBinContent(i0+1)   << " " 
	 << h_0l_mc->GetBinContent(i0+1)     << " "
	 << hPred_Stat->GetBinContent(i0+1)  << " +/- " << hPred_Stat->GetBinError(i0+1) << " +/- " << hPred_Syst->GetBinError(i0+1) << "\n";
  }

} // end of prediction(..)


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
    
    TString cut ="(weight*"+intLumi+")*("+cuts[i0]+")";
    TH1F *hTemp = new TH1F("hTemp","hTemp",2,0.,1000.);
    if (tree) {
    tree->Project("hTemp","met",cut);}
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


TH1F *create1Dhisto(TTree *tree,TString intLumi,TString cuts,TString branch,int bins,float xmin,float xmax,bool useLog,int color, int style,bool norm) {
  TH1::SetDefaultSumw2(kTRUE);

  TString cut ="(weight*"+intLumi+")*("+cuts+")";


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
    
      TString cut ="(weight*"+intLumi+")*("+cuts+")";
      TH1F *h_ = new TH1F("h_","h_",2,0.,1000.);
      if (tree) {
      tree->Project("h_","met",cut);}
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

  TString cut ="(weight)*("+cuts+")";
  
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
