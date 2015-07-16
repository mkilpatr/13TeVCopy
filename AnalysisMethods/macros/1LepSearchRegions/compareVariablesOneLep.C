// plots ROC curves, S/B, S/sqrt(B), etc to compare variables' performances
// long length is since the MET bin loop is now unrolled

#include "AnalysisMethods/PlotUtils/interface/setTDRStyle.h"
#include "TTree.h"
#include "TLegend.h"
#include "TFile.h"
#include "TH1.h"
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
#include "TGraph.h"

#ifdef __MAKECINT__
#pragma link C++ class vector<TH1F*>+;
#endif

using namespace std;

void setTDRStyle();
TH1F *create1Dhisto(TTree *tree,TString intLumi,TString cuts,TString branch,int bins,float xmin,float xmax,bool useLog,int color, int style,bool norm, bool ovBin);
TGraph *drawROCcurve(TTree *tBKG,TTree *tSIG,TString branch,TString cuts,int bins,double xmin,double xmax,bool ovFlow,int color);
TH1F *getSvsB(TTree *tBKG,TTree *tSIG,TString branch,TString cuts,int bins,double xmin,double xmax,bool ovFlow,int color,int SovB);
TGraph *getVsEff(TTree *tBKG,TTree *tSIG,TString branch,TString cuts,int bins,double xmin,double xmax,bool ovFlow,int color);

// filenames
TString fprefix = "/afs/cern.ch/user/a/apatters/scratch2/CMSSW_7_3_1/src/AnalysisMethods/macros/run/plots/";
TString fname_1 = fprefix+"bkg.root";
TString fname_2 = fprefix+"T1tttt_1200_800_tree.root";
TString fname_3 = fprefix+"T1tttt_1500_100_tree.root";
TString fname_4 = fprefix+"T2tt_650_325_tree.root";
TString fname_5 = fprefix+"T2tt_850_100_tree.root";
//TString fname_6 = fprefix+"t2tb_850_400_100_0_tree.root";
//TString fname_7 = fprefix+"t2tb_850_105_100_0_tree.root";
//TString fname_8 = fprefix+"t2tb_850_100_5_0_tree.root";

// open files
TFile *f1 = TFile::Open(fname_1,"READONLY");
TFile *f2 = TFile::Open(fname_2,"READONLY");
TFile *f3 = TFile::Open(fname_3,"READONLY");
TFile *f4 = TFile::Open(fname_4,"READONLY");
TFile *f5 = TFile::Open(fname_5,"READONLY");
//TFile *f6 = TFile::Open(fname_6,"READONLY");
//TFile *f7 = TFile::Open(fname_7,"READONLY");
//TFile *f8 = TFile::Open(fname_8,"READONLY");

void compareVariablesOneLep() {

  setTDRStyle();
  //  gROOT->SetBatch(false);
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(1);
  gStyle->SetPalette(1);

  // Define Integrated Lumi [pb^{-1}]
  // float intLumi     = 1.;
  float intLumi = 1.; // our current lumi
  ostringstream tmpLumi;
  tmpLumi << intLumi;
  TString lumi = tmpLumi.str();

  TTree *t1 = (TTree*)f1->Get("events");
  TTree *t2 = (TTree*)f2->Get("events");
  TTree *t3 = (TTree*)f3->Get("events");
  TTree *t4 = (TTree*)f4->Get("events");
  TTree *t5 = (TTree*)f5->Get("events");
  //TTree *t6 = (TTree*)f6->Get("events");
  //TTree *t7 = (TTree*)f7->Get("events");
  //TTree *t8 = (TTree*)f8->Get("events");

  // -- Selection
  // TString cut_base = "((NJets>=5) && (NBJets==1) && HT>0 && MET>200 && MT2_0>0)";
  // will be scaled by ScaleFactor in S/B functions
  // do need the outer parentheses
  TString cut_base = "(NJets>=4) && (NBJets>=1) && (200<MET)"; // updated preselection
  TString cut_MET1 = "(200<MET) && (MET<300) && "+cut_base; 
  TString cut_MET2 = "(300<MET) && (MET<400) && "+cut_base; 
  TString cut_MET3 = "(400<MET) && (MET<500) && "+cut_base; 
  TString cut_MET4 = "(500<MET) && (MET<600) && "+cut_base;
  TString cut_MET5 = "(600<MET) && "+cut_base;

  // signal name for plots & filenames
  TString bkgname = "bkg";
  TString signame = "T2tt_850_100_tree.root";
  TTree *sigtree = t5;

  // variable branch names
  TString var1name = "MT";
  TString var2name = "DphiLepW";

  // variable properties
  float var1_xmin = 0., var1_xmax = 1000.;
  float var2_xmin = 0., var2_xmax = 3.15;

  int var1_bins = 100, var1_color = 2;
  int var2_bins = 100, var2_color = 3;
  int bkg_color = 1;

  bool var1_norm = true;
  bool var2_norm = true;

  bool ovflow = true;

  //TH1F *create1Dhisto(TTree *tree,TString intLumi,TString cuts,TString branch,int bins,float xmin,float xmax,bool useLog,int color, int style,bool norm, bool ovBin) {
  //TGraph *getVsEff(TTree *tBKG,TTree *tSIG,TString branch,TString cuts,int bins,double xmin,double xmax,bool ovFlow,int color) {

// MET 0
  // bkg
  TH1F    *hNorm_bkg_Var1_MET0 = create1Dhisto(t1, lumi, cut_base, var1name, var1_bins,var1_xmin,var1_xmax,false, bkg_color, 1, var1_norm, ovflow);
  TH1F    *hNorm_bkg_Var2_MET0 = create1Dhisto(t1, lumi, cut_base, var2name, var2_bins,var2_xmin,var2_xmax,false, bkg_color, 1, var2_norm, ovflow);

  // var1
  TGraph  *gROC_Var1_MET0   = drawROCcurve(t1,sigtree,var1name,cut_base,1000,var1_xmin,var1_xmax,ovflow,var1_color);
  TH1F    *hSovB_Var1_MET0  =         getSvsB(t1,sigtree,var1name,cut_base,var1_bins,var1_xmin,var1_xmax,ovflow,var1_color,0);
  TH1F    *hSovSqrtB_Var1_MET0 =      getSvsB(t1,sigtree,var1name,cut_base,var1_bins,var1_xmin,var1_xmax,ovflow,var1_color,1);
  TH1F    *hSovSqrtSplusB_Var1_MET0 = getSvsB(t1,sigtree,var1name,cut_base,var1_bins,var1_xmin,var1_xmax,ovflow,var1_color,2);
  TH1F    *hEffSvsVar_Var1_MET0 =     getSvsB(t1,sigtree,var1name,cut_base,var1_bins,var1_xmin,var1_xmax,ovflow,var1_color,3);
  TH1F    *hEffBvsVar_Var1_MET0 =     getSvsB(t1,sigtree,var1name,cut_base,var1_bins,var1_xmin,var1_xmax,ovflow,var1_color,4);
  TGraph  *gEff_Var1_MET0 = getVsEff(t1,sigtree,var1name,cut_base,var1_bins,var1_xmin,var1_xmax,ovflow,var1_color);
  TH1F    *hNorm_Var1_MET0     = create1Dhisto(sigtree, lumi, cut_base, var1name, var1_bins,var1_xmin,var1_xmax,false, var1_color, 1, var1_norm, ovflow);

  // var2
  TGraph  *gROC_Var2_MET0   = drawROCcurve(t1,sigtree,var2name,cut_base,1000,var2_xmin,var2_xmax,ovflow,var2_color);
  TH1F    *hSovB_Var2_MET0  = getSvsB(t1,sigtree,var2name,cut_base,var2_bins,var2_xmin,var2_xmax,ovflow,var2_color,0);
  TH1F    *hSovSqrtB_Var2_MET0 = getSvsB(t1,sigtree,var2name,cut_base,var2_bins,var2_xmin,var2_xmax,ovflow,var2_color,1);
  TH1F    *hSovSqrtSplusB_Var2_MET0 = getSvsB(t1,sigtree,var2name,cut_base,var2_bins,var2_xmin,var2_xmax,ovflow,var2_color,2);
  TH1F    *hEffSvsVar_Var2_MET0 =     getSvsB(t1,sigtree,var2name,cut_base,var2_bins,var2_xmin,var2_xmax,ovflow,var2_color,3);
  TH1F    *hEffBvsVar_Var2_MET0 =     getSvsB(t1,sigtree,var2name,cut_base,var2_bins,var2_xmin,var2_xmax,ovflow,var2_color,4);
  TGraph    *gEff_Var2_MET0 = getVsEff(t1,sigtree,var2name,cut_base,var2_bins,var2_xmin,var2_xmax,ovflow,var2_color);
  TH1F    *hNorm_Var2_MET0     = create1Dhisto(sigtree, lumi, cut_base, var2name, var2_bins,var2_xmin,var2_xmax,false, var2_color, 1, var2_norm, ovflow);

// norm plots of vars
  TCanvas *cNorm_Var1_MET0   = new TCanvas("cNorm_Var1_MET0","cNorm_Var1_MET0",500,500);
  //gStyle->SetOptStat(1100000); // stat box: name, # entries
  TLegend* leg0 = new TLegend(0.65,0.7,.95,0.8);
  leg0->AddEntry(hNorm_Var1_MET0,signame,"l");
  leg0->AddEntry(hNorm_bkg_Var1_MET0,bkgname,"l");
  hNorm_Var1_MET0->SetTitle(signame+"_"+var1name);
  hNorm_bkg_Var1_MET0->Draw("H");
  hNorm_Var1_MET0->Draw("SAME");
  leg0->Draw();
  cNorm_Var1_MET0->Print(signame+"_"+"norm_"+var1name+"_MET0.png");

  TCanvas *cNorm_Var2_MET0   = new TCanvas("cNorm_Var2_MET0","cNorm_Var2_MET0",500,500);
//  gStyle->SetOptStat(1100010); // stat box: name, # entries, ovflow count
  hNorm_Var2_MET0->SetTitle(signame+"_"+var2name);
  TLegend* leg5 = new TLegend(0.65,0.7,0.95,0.8);
  leg5->AddEntry(hNorm_Var2_MET0,signame,"l");
  leg5->AddEntry(hNorm_bkg_Var2_MET0,bkgname,"l");
  hNorm_bkg_Var2_MET0->Draw("H");
  hNorm_Var2_MET0->Draw("SAME");
  leg5->Draw();
  cNorm_Var2_MET0->Print(signame+"_"+"norm_"+var2name+"_MET0.png");

  
// ROC curves
  gStyle->SetOptStat(0);
  TCanvas *cROC_MET0 = new TCanvas("cROC_MET0","cROC_MET0",500,500);
  cROC_MET0->SetGrid(1);
  gROC_Var2_MET0->Draw("AL");
  gROC_Var2_MET0->GetYaxis()->SetRangeUser(0.,1.);
  gROC_Var2_MET0->GetXaxis()->SetRangeUser(0.,.25);
  gROC_Var2_MET0->SetTitle(signame);
  gROC_Var1_MET0->Draw("SAME");
  TLegend* leg2 = new TLegend(0.7,0.2,.95,0.35);
  leg2->AddEntry(gROC_Var1_MET0,var1name,"l");
  leg2->AddEntry(gROC_Var2_MET0,var2name,"l");
  leg2->Draw();
  cROC_MET0->Print(signame+"_"+var1name+"_"+var2name+"_"+"ROC_MET0.png");

// eff_S/sqrt(eff_B) curves
  TCanvas *cROC_Eff_MET0 = new TCanvas("cROC_Eff_MET0","cROC_Eff_MET0",500,500);
  cROC_Eff_MET0->SetGrid(1);
  TLegend* leg3 = new TLegend(0.7,0.8,.95,0.95);
  leg3->AddEntry(gROC_Var1_MET0,var1name,"l");
  leg3->AddEntry(gROC_Var2_MET0,var2name,"l");
  gEff_Var1_MET0->Draw("AL");
//  gEff_Var1->GetYaxis()->SetRangeUser(0.,2.5);
  gEff_Var1_MET0->SetTitle(signame);
  gEff_Var2_MET0->Draw("SAME");
  leg3->Draw();
  cROC_Eff_MET0->Print(signame+"_"+var1name+"_"+var2name+"_"+"ESovSqrtEB_MET0.png");

// S/B stuff, var 1
  TCanvas *cSovB_MET0 = new TCanvas("cSovB_MET0","cSovB_MET0",500,500);
  cSovB_MET0->cd(1);
  gPad->SetGrid(1);
  hSovB_Var1_MET0->Draw("L");
  hSovB_Var1_MET0->SetTitle(signame);
  hSovB_Var1_MET0->GetXaxis()->SetTitle(var1name);
  TLegend* leg1 = new TLegend(0.7,0.8,.95,0.95);
  leg1->AddEntry(hSovB_Var1_MET0,signame,"l");
  leg1->Draw();
  cSovB_MET0->Print(signame+"_"+var1name+"_"+"SovB_MET0.png");

  TCanvas *cSovSqrtB_MET0 = new TCanvas("cSovSqrtB_MET0","cSovSqrtB_MET0",500,500);
  cSovSqrtB_MET0->cd(1);
  gPad->SetGrid(1);
  hSovSqrtB_Var1_MET0->Draw("L");
  hSovSqrtB_Var1_MET0->SetTitle(signame);
  hSovSqrtB_Var1_MET0->GetXaxis()->SetTitle(var1name);
  //hSovSqrtB_Var4->Draw("SAME");
  //leg1->Draw();
  TLegend* leg7 = new TLegend(0.7,0.8,.95,0.95);
  leg7->AddEntry(hSovB_Var1_MET0,signame,"l");
  leg7->Draw();
  cSovSqrtB_MET0->Print(signame+"_"+var1name+"_"+"SovSqrtB_MET0.png");

  TCanvas *cSovSqrtSplusB_MET0 = new TCanvas("cSovSqrtSplusB_MET0","cSovSqrtSplusB_MET0",500,500);
  cSovSqrtSplusB_MET0->cd(1);
  gPad->SetGrid(1);
  hSovSqrtSplusB_Var1_MET0->Draw("L");
  hSovSqrtSplusB_Var1_MET0->SetTitle(signame);
  hSovSqrtSplusB_Var1_MET0->GetXaxis()->SetTitle(var1name);
  //hSovSqrtSplusB_Var4->Draw("SAME");
  leg7->Draw();
  cSovSqrtSplusB_MET0->Print(signame+"_"+var1name+"_"+"SovSqrtSplusB_MET0.png");

  TCanvas *cEffSvsVar1_MET0 = new TCanvas("cEffSvsVar1_MET0", "cEffSvsVar1_MET0",500,500);
  gPad->SetGrid(1);
  hEffSvsVar_Var1_MET0->Draw("L");
  hEffSvsVar_Var1_MET0->SetTitle(signame);
  hEffSvsVar_Var1_MET0->GetXaxis()->SetTitle(var1name);
  leg7->Draw();
  cEffSvsVar1_MET0->Print(signame+"_"+var1name+"_"+"EffSvsVar_MET0.png");

  TCanvas *cEffBvsVar1_MET0 = new TCanvas("cEffBvsVar1_MET0", "cEffBvsVar1_MET0",500,500);
  gPad->SetGrid(1);
  hEffBvsVar_Var1_MET0->Draw("L");
  hEffBvsVar_Var1_MET0->SetTitle(signame);
  hEffBvsVar_Var1_MET0->GetXaxis()->SetTitle(var1name);
  leg7->Draw();
  cEffBvsVar1_MET0->Print(signame+"_"+var1name+"_"+"EffBvsVar_MET0.png");

// S?B stuff, var 2 
  TCanvas *cSovB2_MET0 = new TCanvas("cSovB2_MET0","cSovB2_MET0",500,500);
  cSovB2_MET0->cd(1);
  gPad->SetGrid(1);
  //hSovB_Var2->GetYaxis()->SetRangeUser(0.,0.1);
  hSovB_Var2_MET0->Draw("L");
  hSovB_Var2_MET0->GetXaxis()->SetTitle(var2name);
  hSovB_Var2_MET0->SetTitle(signame);
  TLegend* leg4 = new TLegend(0.7,0.8,.95,0.95);
  leg4->AddEntry(hSovB_Var2_MET0,signame,"l");
  leg4->Draw();
  cSovB2_MET0->Print(signame+"_"+var2name+"_"+"SovB_MET0.png");

  TCanvas *cSovSqrtB2_MET0 = new TCanvas("cSovSqrtB2_MET0","cSovSqrtB2_MET0",500,500);
  cSovSqrtB2_MET0->cd(1);
  gPad->SetGrid(1);
  hSovSqrtB_Var2_MET0->Draw("L");
//  hSovSqrtB_Var2->GetYaxis()->SetRangeUser(0.,.55);
  hSovSqrtB_Var2_MET0->GetXaxis()->SetTitle(var2name);
  hSovSqrtB_Var2_MET0->SetTitle(signame);
  TLegend* leg6 = new TLegend(0.7,0.8,.95,0.95);
  leg6->AddEntry(hSovSqrtB_Var2_MET0,signame,"l");
  leg6->Draw();
  cSovSqrtB2_MET0->Print(signame+"_"+var2name+"_"+"SovSqrtB_MET0.png");

  TCanvas *cSovSqrtSplusB2_MET0 = new TCanvas("cSovSqrtSplusB2_MET0","cSovSqrtSplusB2_MET0",500,500);
  cSovSqrtSplusB2_MET0->cd(1);
  gPad->SetGrid(1);
  hSovSqrtSplusB_Var2_MET0->GetXaxis()->SetTitle(var2name);
  hSovSqrtSplusB_Var2_MET0->SetTitle(signame);
  hSovSqrtSplusB_Var2_MET0->Draw("L");
//  hSovSqrtSplusB_Var2->GetYaxis()->SetRangeUser(0.,.55);
  leg6->Draw();
  cSovSqrtSplusB2_MET0->Print(signame+"_"+var2name+"_"+"SovSqrtSplusB_MET0.png");

  TCanvas *cEffSvsVar2_MET0 = new TCanvas("cEffSvsVar2_MET0", "cEffSvsVar2_MET0",500,500);
  gPad->SetGrid(1);
  hEffSvsVar_Var2_MET0->Draw("L");
  hEffSvsVar_Var2_MET0->SetTitle(signame);
  hEffSvsVar_Var2_MET0->GetXaxis()->SetTitle(var2name);
  leg6->Draw();
  cEffSvsVar2_MET0->Print(signame+"_"+var2name+"_"+"EffSvsVar_MET0.png");

  TCanvas *cEffBvsVar2_MET0 = new TCanvas("cEffBvsVar2_MET0", "cEffBvsVar2_MET0",500,500);
  gPad->SetGrid(1);
  hEffBvsVar_Var2_MET0->Draw("L");
  hEffBvsVar_Var2_MET0->SetTitle(signame);
  hEffBvsVar_Var2_MET0->GetXaxis()->SetTitle(var2name);
  leg6->Draw();
  cEffBvsVar2_MET0->Print(signame+"_"+var2name+"_"+"EffBvsVar_MET0.png");


// MET 1
  // bkg
  TH1F    *hNorm_bkg_Var1_MET1 = create1Dhisto(t1, lumi, cut_MET1, var1name, var1_bins,var1_xmin,var1_xmax,false, bkg_color, 1, var1_norm, ovflow);
  TH1F    *hNorm_bkg_Var2_MET1 = create1Dhisto(t1, lumi, cut_MET1, var2name, var2_bins,var2_xmin,var2_xmax,false, bkg_color, 1, var2_norm, ovflow);

  // var1
  TGraph  *gROC_Var1_MET1   = drawROCcurve(t1,sigtree,var1name,cut_MET1,1000,var1_xmin,var1_xmax,ovflow,var1_color);
  TH1F    *hSovB_Var1_MET1  =         getSvsB(t1,sigtree,var1name,cut_MET1,var1_bins,var1_xmin,var1_xmax,ovflow,var1_color,0);
  TH1F    *hSovSqrtB_Var1_MET1 =      getSvsB(t1,sigtree,var1name,cut_MET1,var1_bins,var1_xmin,var1_xmax,ovflow,var1_color,1);
  TH1F    *hSovSqrtSplusB_Var1_MET1 = getSvsB(t1,sigtree,var1name,cut_MET1,var1_bins,var1_xmin,var1_xmax,ovflow,var1_color,2);
  TH1F    *hEffSvsVar_Var1_MET1 =     getSvsB(t1,sigtree,var1name,cut_MET1,var1_bins,var1_xmin,var1_xmax,ovflow,var1_color,3);
  TH1F    *hEffBvsVar_Var1_MET1 =     getSvsB(t1,sigtree,var1name,cut_MET1,var1_bins,var1_xmin,var1_xmax,ovflow,var1_color,4);
  TGraph  *gEff_Var1_MET1 = getVsEff(t1,sigtree,var1name,cut_MET1,var1_bins,var1_xmin,var1_xmax,ovflow,var1_color);
  TH1F    *hNorm_Var1_MET1     = create1Dhisto(sigtree, lumi, cut_MET1, var1name, var1_bins,var1_xmin,var1_xmax,false, var1_color, 1, var1_norm, ovflow);

  // var2
  TGraph  *gROC_Var2_MET1   = drawROCcurve(t1,sigtree,var2name,cut_MET1,1000,var2_xmin,var2_xmax,ovflow,var2_color);
  TH1F    *hSovB_Var2_MET1  = getSvsB(t1,sigtree,var2name,cut_MET1,var2_bins,var2_xmin,var2_xmax,ovflow,var2_color,0);
  TH1F    *hSovSqrtB_Var2_MET1 = getSvsB(t1,sigtree,var2name,cut_MET1,var2_bins,var2_xmin,var2_xmax,ovflow,var2_color,1);
  TH1F    *hSovSqrtSplusB_Var2_MET1 = getSvsB(t1,sigtree,var2name,cut_MET1,var2_bins,var2_xmin,var2_xmax,ovflow,var2_color,2);
  TH1F    *hEffSvsVar_Var2_MET1 =     getSvsB(t1,sigtree,var2name,cut_MET1,var2_bins,var2_xmin,var2_xmax,ovflow,var2_color,3);
  TH1F    *hEffBvsVar_Var2_MET1 =     getSvsB(t1,sigtree,var2name,cut_MET1,var2_bins,var2_xmin,var2_xmax,ovflow,var2_color,4);
  TGraph    *gEff_Var2_MET1 = getVsEff(t1,sigtree,var2name,cut_MET1,var2_bins,var2_xmin,var2_xmax,ovflow,var2_color);
  TH1F    *hNorm_Var2_MET1     = create1Dhisto(sigtree, lumi, cut_MET1, var2name, var2_bins,var2_xmin,var2_xmax,false, var2_color, 1, var2_norm, ovflow);

// norm plots of vars
  TCanvas *cNorm_Var1_MET1   = new TCanvas("cNorm_Var1_MET1","cNorm_Var1_MET1",500,500);
  //gStyle->SetOptStat(1100000); // stat box: name, # entries
  hNorm_Var1_MET1->SetTitle(signame+"_"+var1name);
  hNorm_bkg_Var1_MET1->Draw("H");
  hNorm_Var1_MET1->Draw("SAME");
  leg0->Draw();
  cNorm_Var1_MET1->Print(signame+"_"+"norm_"+var1name+"_MET1.png");

  TCanvas *cNorm_Var2_MET1   = new TCanvas("cNorm_Var2_MET1","cNorm_Var2_MET1",500,500);
//  gStyle->SetOptStat(1100010); // stat box: name, # entries, ovflow count
  hNorm_Var2_MET1->SetTitle(signame+"_"+var2name);
  hNorm_bkg_Var2_MET1->Draw("H");
  hNorm_Var2_MET1->Draw("SAME");
  leg5->Draw();
  cNorm_Var2_MET1->Print(signame+"_"+"norm_"+var2name+"_MET1.png");

  
// ROC curves
  gStyle->SetOptStat(0);
  TCanvas *cROC_MET1 = new TCanvas("cROC_MET1","cROC_MET1",500,500);
  cROC_MET1->SetGrid(1);
  gROC_Var2_MET1->Draw("AL");
  gROC_Var2_MET1->GetYaxis()->SetRangeUser(0.,1.);
  gROC_Var2_MET1->GetXaxis()->SetRangeUser(0.,.25);
  gROC_Var2_MET1->SetTitle(signame);
  gROC_Var1_MET1->Draw("SAME");
  leg2->Draw();
  cROC_MET1->Print(signame+"_"+var1name+"_"+var2name+"_"+"ROC_MET1.png");

// eff_S/sqrt(eff_B) curves
  TCanvas *cROC_Eff_MET1 = new TCanvas("cROC_Eff_MET1","cROC_Eff_MET1",500,500);
  cROC_Eff_MET1->SetGrid(1);
  gEff_Var1_MET1->Draw("AL");
//  gEff_Var1->GetYaxis()->SetRangeUser(0.,2.5);
  gEff_Var1_MET1->SetTitle(signame);
  gEff_Var2_MET1->Draw("SAME");
  leg3->Draw();
  cROC_Eff_MET1->Print(signame+"_"+var1name+"_"+var2name+"_"+"ESovSqrtEB_MET1.png");

// S/B stuff, var 1
  TCanvas *cSovB_MET1 = new TCanvas("cSovB_MET1","cSovB_MET1",500,500);
  cSovB_MET1->cd(1);
  gPad->SetGrid(1);
  hSovB_Var1_MET1->Draw("L");
  hSovB_Var1_MET1->SetTitle(signame);
  hSovB_Var1_MET1->GetXaxis()->SetTitle(var1name);
  leg1->Draw();
  cSovB_MET1->Print(signame+"_"+var1name+"_"+"SovB_MET1.png");

  TCanvas *cSovSqrtB_MET1 = new TCanvas("cSovSqrtB_MET1","cSovSqrtB_MET1",500,500);
  cSovSqrtB_MET1->cd(1);
  gPad->SetGrid(1);
  hSovSqrtB_Var1_MET1->Draw("L");
  hSovSqrtB_Var1_MET1->SetTitle(signame);
  hSovSqrtB_Var1_MET1->GetXaxis()->SetTitle(var1name);
  //hSovSqrtB_Var4->Draw("SAME");
  //leg1->Draw();
  leg7->Draw();
  cSovSqrtB_MET1->Print(signame+"_"+var1name+"_"+"SovSqrtB_MET1.png");

  TCanvas *cSovSqrtSplusB_MET1 = new TCanvas("cSovSqrtSplusB_MET1","cSovSqrtSplusB_MET1",500,500);
  cSovSqrtSplusB_MET1->cd(1);
  gPad->SetGrid(1);
  hSovSqrtSplusB_Var1_MET1->Draw("L");
  hSovSqrtSplusB_Var1_MET1->SetTitle(signame);
  hSovSqrtSplusB_Var1_MET1->GetXaxis()->SetTitle(var1name);
  //hSovSqrtSplusB_Var4->Draw("SAME");
  leg7->Draw();
  cSovSqrtSplusB_MET1->Print(signame+"_"+var1name+"_"+"SovSqrtSplusB_MET1.png");

  TCanvas *cEffSvsVar1_MET1 = new TCanvas("cEffSvsVar1_MET1", "cEffSvsVar1_MET1",500,500);
  gPad->SetGrid(1);
  hEffSvsVar_Var1_MET1->Draw("L");
  hEffSvsVar_Var1_MET1->SetTitle(signame);
  hEffSvsVar_Var1_MET1->GetXaxis()->SetTitle(var1name);
  leg7->Draw();
  cEffSvsVar1_MET1->Print(signame+"_"+var1name+"_"+"EffSvsVar_MET1.png");

  TCanvas *cEffBvsVar1_MET1 = new TCanvas("cEffBvsVar1_MET1", "cEffBvsVar1_MET1",500,500);
  gPad->SetGrid(1);
  hEffBvsVar_Var1_MET1->Draw("L");
  hEffBvsVar_Var1_MET1->SetTitle(signame);
  hEffBvsVar_Var1_MET1->GetXaxis()->SetTitle(var1name);
  leg7->Draw();
  cEffBvsVar1_MET1->Print(signame+"_"+var1name+"_"+"EffBvsVar_MET1.png");

// S?B stuff, var 2 
  TCanvas *cSovB2_MET1 = new TCanvas("cSovB2_MET1","cSovB2_MET1",500,500);
  cSovB2_MET1->cd(1);
  gPad->SetGrid(1);
  //hSovB_Var2->GetYaxis()->SetRangeUser(0.,0.1);
  hSovB_Var2_MET1->Draw("L");
  hSovB_Var2_MET1->GetXaxis()->SetTitle(var2name);
  hSovB_Var2_MET1->SetTitle(signame);
  leg4->Draw();
  cSovB2_MET1->Print(signame+"_"+var2name+"_"+"SovB_MET1.png");

  TCanvas *cSovSqrtB2_MET1 = new TCanvas("cSovSqrtB2_MET1","cSovSqrtB2_MET1",500,500);
  cSovSqrtB2_MET1->cd(1);
  gPad->SetGrid(1);
  hSovSqrtB_Var2_MET1->Draw("L");
//  hSovSqrtB_Var2->GetYaxis()->SetRangeUser(0.,.55);
  hSovSqrtB_Var2_MET1->GetXaxis()->SetTitle(var2name);
  hSovSqrtB_Var2_MET1->SetTitle(signame);
  leg6->Draw();
  cSovSqrtB2_MET1->Print(signame+"_"+var2name+"_"+"SovSqrtB_MET1.png");

  TCanvas *cSovSqrtSplusB2_MET1 = new TCanvas("cSovSqrtSplusB2_MET1","cSovSqrtSplusB2_MET1",500,500);
  cSovSqrtSplusB2_MET1->cd(1);
  gPad->SetGrid(1);
  hSovSqrtSplusB_Var2_MET1->GetXaxis()->SetTitle(var2name);
  hSovSqrtSplusB_Var2_MET1->SetTitle(signame);
  hSovSqrtSplusB_Var2_MET1->Draw("L");
//  hSovSqrtSplusB_Var2->GetYaxis()->SetRangeUser(0.,.55);
  leg6->Draw();
  cSovSqrtSplusB2_MET1->Print(signame+"_"+var2name+"_"+"SovSqrtSplusB_MET1.png");

  TCanvas *cEffSvsVar2_MET1 = new TCanvas("cEffSvsVar2_MET1", "cEffSvsVar2_MET1",500,500);
  gPad->SetGrid(1);
  hEffSvsVar_Var2_MET1->Draw("L");
  hEffSvsVar_Var2_MET1->SetTitle(signame);
  hEffSvsVar_Var2_MET1->GetXaxis()->SetTitle(var2name);
  leg6->Draw();
  cEffSvsVar2_MET1->Print(signame+"_"+var2name+"_"+"EffSvsVar_MET1.png");

  TCanvas *cEffBvsVar2_MET1 = new TCanvas("cEffBvsVar2_MET1", "cEffBvsVar2_MET1",500,500);
  gPad->SetGrid(1);
  hEffBvsVar_Var2_MET1->Draw("L");
  hEffBvsVar_Var2_MET1->SetTitle(signame);
  hEffBvsVar_Var2_MET1->GetXaxis()->SetTitle(var2name);
  leg6->Draw();
  cEffBvsVar2_MET1->Print(signame+"_"+var2name+"_"+"EffBvsVar_MET1.png");





// MET 2 
  // bkg
  TH1F    *hNorm_bkg_Var1_MET2 = create1Dhisto(t1, lumi, cut_MET2, var1name, var1_bins,var1_xmin,var1_xmax,false, bkg_color, 1, var1_norm, ovflow);
  TH1F    *hNorm_bkg_Var2_MET2 = create1Dhisto(t1, lumi, cut_MET2, var2name, var2_bins,var2_xmin,var2_xmax,false, bkg_color, 1, var2_norm, ovflow);

  // var1
  TGraph  *gROC_Var1_MET2   = drawROCcurve(t1,sigtree,var1name,cut_MET2,1000,var1_xmin,var1_xmax,ovflow,var1_color);
  TH1F    *hSovB_Var1_MET2  =         getSvsB(t1,sigtree,var1name,cut_MET2,var1_bins,var1_xmin,var1_xmax,ovflow,var1_color,0);
  TH1F    *hSovSqrtB_Var1_MET2 =      getSvsB(t1,sigtree,var1name,cut_MET2,var1_bins,var1_xmin,var1_xmax,ovflow,var1_color,1);
  TH1F    *hSovSqrtSplusB_Var1_MET2 = getSvsB(t1,sigtree,var1name,cut_MET2,var1_bins,var1_xmin,var1_xmax,ovflow,var1_color,2);
  TH1F    *hEffSvsVar_Var1_MET2 =     getSvsB(t1,sigtree,var1name,cut_MET2,var1_bins,var1_xmin,var1_xmax,ovflow,var1_color,3);
  TH1F    *hEffBvsVar_Var1_MET2 =     getSvsB(t1,sigtree,var1name,cut_MET2,var1_bins,var1_xmin,var1_xmax,ovflow,var1_color,4);
  TGraph  *gEff_Var1_MET2 = getVsEff(t1,sigtree,var1name,cut_MET2,var1_bins,var1_xmin,var1_xmax,ovflow,var1_color);
  TH1F    *hNorm_Var1_MET2     = create1Dhisto(sigtree, lumi, cut_MET2, var1name, var1_bins,var1_xmin,var1_xmax,false, var1_color, 1, var1_norm, ovflow);

  // var2
  TGraph  *gROC_Var2_MET2   = drawROCcurve(t1,sigtree,var2name,cut_MET2,1000,var2_xmin,var2_xmax,ovflow,var2_color);
  TH1F    *hSovB_Var2_MET2  = getSvsB(t1,sigtree,var2name,cut_MET2,var2_bins,var2_xmin,var2_xmax,ovflow,var2_color,0);
  TH1F    *hSovSqrtB_Var2_MET2 = getSvsB(t1,sigtree,var2name,cut_MET2,var2_bins,var2_xmin,var2_xmax,ovflow,var2_color,1);
  TH1F    *hSovSqrtSplusB_Var2_MET2 = getSvsB(t1,sigtree,var2name,cut_MET2,var2_bins,var2_xmin,var2_xmax,ovflow,var2_color,2);
  TH1F    *hEffSvsVar_Var2_MET2 =     getSvsB(t1,sigtree,var2name,cut_MET2,var2_bins,var2_xmin,var2_xmax,ovflow,var2_color,3);
  TH1F    *hEffBvsVar_Var2_MET2 =     getSvsB(t1,sigtree,var2name,cut_MET2,var2_bins,var2_xmin,var2_xmax,ovflow,var2_color,4);
  TGraph    *gEff_Var2_MET2 = getVsEff(t1,sigtree,var2name,cut_MET2,var2_bins,var2_xmin,var2_xmax,ovflow,var2_color);
  TH1F    *hNorm_Var2_MET2     = create1Dhisto(sigtree, lumi, cut_MET2, var2name, var2_bins,var2_xmin,var2_xmax,false, var2_color, 1, var2_norm, ovflow);

// norm plots of vars
  TCanvas *cNorm_Var1_MET2   = new TCanvas("cNorm_Var1_MET2","cNorm_Var1_MET2",500,500);
  //gStyle->SetOptStat(1100000); // stat box: name, # entries
  hNorm_Var1_MET2->SetTitle(signame+"_"+var1name);
  hNorm_bkg_Var1_MET2->Draw("H");
  hNorm_Var1_MET2->Draw("SAME");
  leg0->Draw();
  cNorm_Var1_MET2->Print(signame+"_"+"norm_"+var1name+"_MET2.png");

  TCanvas *cNorm_Var2_MET2   = new TCanvas("cNorm_Var2_MET2","cNorm_Var2_MET2",500,500);
//  gStyle->SetOptStat(1100010); // stat box: name, # entries, ovflow count
  hNorm_Var2_MET2->SetTitle(signame+"_"+var2name);
  hNorm_bkg_Var2_MET2->Draw("H");
  hNorm_Var2_MET2->Draw("SAME");
  leg5->Draw();
  cNorm_Var2_MET2->Print(signame+"_"+"norm_"+var2name+"_MET2.png");

  
// ROC curves
  gStyle->SetOptStat(0);
  TCanvas *cROC_MET2 = new TCanvas("cROC_MET2","cROC_MET2",500,500);
  cROC_MET2->SetGrid(1);
  gROC_Var2_MET2->Draw("AL");
  gROC_Var2_MET2->GetYaxis()->SetRangeUser(0.,1.);
  gROC_Var2_MET2->GetXaxis()->SetRangeUser(0.,.25);
  gROC_Var2_MET2->SetTitle(signame);
  gROC_Var1_MET2->Draw("SAME");
  leg2->Draw();
  cROC_MET2->Print(signame+"_"+var1name+"_"+var2name+"_"+"ROC_MET2.png");

// eff_S/sqrt(eff_B) curves
  TCanvas *cROC_Eff_MET2 = new TCanvas("cROC_Eff_MET2","cROC_Eff_MET2",500,500);
  cROC_Eff_MET2->SetGrid(1);
  gEff_Var1_MET2->Draw("AL");
//  gEff_Var1->GetYaxis()->SetRangeUser(0.,2.5);
  gEff_Var1_MET2->SetTitle(signame);
  gEff_Var2_MET2->Draw("SAME");
  leg3->Draw();
  cROC_Eff_MET2->Print(signame+"_"+var1name+"_"+var2name+"_"+"ESovSqrtEB_MET2.png");

// S/B stuff, var 1
  TCanvas *cSovB_MET2 = new TCanvas("cSovB_MET2","cSovB_MET2",500,500);
  cSovB_MET2->cd(1);
  gPad->SetGrid(1);
  hSovB_Var1_MET2->Draw("L");
  hSovB_Var1_MET2->SetTitle(signame);
  hSovB_Var1_MET2->GetXaxis()->SetTitle(var1name);
  leg1->Draw();
  cSovB_MET2->Print(signame+"_"+var1name+"_"+"SovB_MET2.png");

  TCanvas *cSovSqrtB_MET2 = new TCanvas("cSovSqrtB_MET2","cSovSqrtB_MET2",500,500);
  cSovSqrtB_MET2->cd(1);
  gPad->SetGrid(1);
  hSovSqrtB_Var1_MET2->Draw("L");
  hSovSqrtB_Var1_MET2->SetTitle(signame);
  hSovSqrtB_Var1_MET2->GetXaxis()->SetTitle(var1name);
  //hSovSqrtB_Var4->Draw("SAME");
  //leg1->Draw();
  leg7->Draw();
  cSovSqrtB_MET2->Print(signame+"_"+var1name+"_"+"SovSqrtB_MET2.png");

  TCanvas *cSovSqrtSplusB_MET2 = new TCanvas("cSovSqrtSplusB_MET2","cSovSqrtSplusB_MET2",500,500);
  cSovSqrtSplusB_MET2->cd(1);
  gPad->SetGrid(1);
  hSovSqrtSplusB_Var1_MET2->Draw("L");
  hSovSqrtSplusB_Var1_MET2->SetTitle(signame);
  hSovSqrtSplusB_Var1_MET2->GetXaxis()->SetTitle(var1name);
  //hSovSqrtSplusB_Var4->Draw("SAME");
  leg7->Draw();
  cSovSqrtSplusB_MET2->Print(signame+"_"+var1name+"_"+"SovSqrtSplusB_MET2.png");

  TCanvas *cEffSvsVar1_MET2 = new TCanvas("cEffSvsVar1_MET2", "cEffSvsVar1_MET2",500,500);
  gPad->SetGrid(1);
  hEffSvsVar_Var1_MET2->Draw("L");
  hEffSvsVar_Var1_MET2->SetTitle(signame);
  hEffSvsVar_Var1_MET2->GetXaxis()->SetTitle(var1name);
  leg7->Draw();
  cEffSvsVar1_MET2->Print(signame+"_"+var1name+"_"+"EffSvsVar_MET2.png");

  TCanvas *cEffBvsVar1_MET2 = new TCanvas("cEffBvsVar1_MET2", "cEffBvsVar1_MET2",500,500);
  gPad->SetGrid(1);
  hEffBvsVar_Var1_MET2->Draw("L");
  hEffBvsVar_Var1_MET2->SetTitle(signame);
  hEffBvsVar_Var1_MET2->GetXaxis()->SetTitle(var1name);
  leg7->Draw();
  cEffBvsVar1_MET2->Print(signame+"_"+var1name+"_"+"EffBvsVar_MET2.png");

// S?B stuff, var 2 
  TCanvas *cSovB2_MET2 = new TCanvas("cSovB2_MET2","cSovB2_MET2",500,500);
  cSovB2_MET2->cd(1);
  gPad->SetGrid(1);
  //hSovB_Var2->GetYaxis()->SetRangeUser(0.,0.1);
  hSovB_Var2_MET2->Draw("L");
  hSovB_Var2_MET2->GetXaxis()->SetTitle(var2name);
  hSovB_Var2_MET2->SetTitle(signame);
  leg4->Draw();
  cSovB2_MET2->Print(signame+"_"+var2name+"_"+"SovB_MET2.png");

  TCanvas *cSovSqrtB2_MET2 = new TCanvas("cSovSqrtB2_MET2","cSovSqrtB2_MET2",500,500);
  cSovSqrtB2_MET2->cd(1);
  gPad->SetGrid(1);
  hSovSqrtB_Var2_MET2->Draw("L");
//  hSovSqrtB_Var2->GetYaxis()->SetRangeUser(0.,.55);
  hSovSqrtB_Var2_MET2->GetXaxis()->SetTitle(var2name);
  hSovSqrtB_Var2_MET2->SetTitle(signame);
  leg6->Draw();
  cSovSqrtB2_MET2->Print(signame+"_"+var2name+"_"+"SovSqrtB_MET2.png");

  TCanvas *cSovSqrtSplusB2_MET2 = new TCanvas("cSovSqrtSplusB2_MET2","cSovSqrtSplusB2_MET2",500,500);
  cSovSqrtSplusB2_MET2->cd(1);
  gPad->SetGrid(1);
  hSovSqrtSplusB_Var2_MET2->GetXaxis()->SetTitle(var2name);
  hSovSqrtSplusB_Var2_MET2->SetTitle(signame);
  hSovSqrtSplusB_Var2_MET2->Draw("L");
//  hSovSqrtSplusB_Var2->GetYaxis()->SetRangeUser(0.,.55);
  leg6->Draw();
  cSovSqrtSplusB2_MET2->Print(signame+"_"+var2name+"_"+"SovSqrtSplusB_MET2.png");

  TCanvas *cEffSvsVar2_MET2 = new TCanvas("cEffSvsVar2_MET2", "cEffSvsVar2_MET2",500,500);
  gPad->SetGrid(1);
  hEffSvsVar_Var2_MET2->Draw("L");
  hEffSvsVar_Var2_MET2->SetTitle(signame);
  hEffSvsVar_Var2_MET2->GetXaxis()->SetTitle(var2name);
  leg6->Draw();
  cEffSvsVar2_MET2->Print(signame+"_"+var2name+"_"+"EffSvsVar_MET2.png");

  TCanvas *cEffBvsVar2_MET2 = new TCanvas("cEffBvsVar2_MET2", "cEffBvsVar2_MET2",500,500);
  gPad->SetGrid(1);
  hEffBvsVar_Var2_MET2->Draw("L");
  hEffBvsVar_Var2_MET2->SetTitle(signame);
  hEffBvsVar_Var2_MET2->GetXaxis()->SetTitle(var2name);
  leg6->Draw();
  cEffBvsVar2_MET2->Print(signame+"_"+var2name+"_"+"EffBvsVar_MET2.png");




// MET 3
  // bkg
  TH1F    *hNorm_bkg_Var1_MET3 = create1Dhisto(t1, lumi, cut_MET3, var1name, var1_bins,var1_xmin,var1_xmax,false, bkg_color, 1, var1_norm, ovflow);
  TH1F    *hNorm_bkg_Var2_MET3 = create1Dhisto(t1, lumi, cut_MET3, var2name, var2_bins,var2_xmin,var2_xmax,false, bkg_color, 1, var2_norm, ovflow);

  // var1
  TGraph  *gROC_Var1_MET3   = drawROCcurve(t1,sigtree,var1name,cut_MET3,1000,var1_xmin,var1_xmax,ovflow,var1_color);
  TH1F    *hSovB_Var1_MET3  =         getSvsB(t1,sigtree,var1name,cut_MET3,var1_bins,var1_xmin,var1_xmax,ovflow,var1_color,0);
  TH1F    *hSovSqrtB_Var1_MET3 =      getSvsB(t1,sigtree,var1name,cut_MET3,var1_bins,var1_xmin,var1_xmax,ovflow,var1_color,1);
  TH1F    *hSovSqrtSplusB_Var1_MET3 = getSvsB(t1,sigtree,var1name,cut_MET3,var1_bins,var1_xmin,var1_xmax,ovflow,var1_color,2);
  TH1F    *hEffSvsVar_Var1_MET3 =     getSvsB(t1,sigtree,var1name,cut_MET3,var1_bins,var1_xmin,var1_xmax,ovflow,var1_color,3);
  TH1F    *hEffBvsVar_Var1_MET3 =     getSvsB(t1,sigtree,var1name,cut_MET3,var1_bins,var1_xmin,var1_xmax,ovflow,var1_color,4);
  TGraph  *gEff_Var1_MET3 = getVsEff(t1,sigtree,var1name,cut_MET3,var1_bins,var1_xmin,var1_xmax,ovflow,var1_color);
  TH1F    *hNorm_Var1_MET3     = create1Dhisto(sigtree, lumi, cut_MET3, var1name, var1_bins,var1_xmin,var1_xmax,false, var1_color, 1, var1_norm, ovflow);

  // var2
  TGraph  *gROC_Var2_MET3   = drawROCcurve(t1,sigtree,var2name,cut_MET3,1000,var2_xmin,var2_xmax,ovflow,var2_color);
  TH1F    *hSovB_Var2_MET3  = getSvsB(t1,sigtree,var2name,cut_MET3,var2_bins,var2_xmin,var2_xmax,ovflow,var2_color,0);
  TH1F    *hSovSqrtB_Var2_MET3 = getSvsB(t1,sigtree,var2name,cut_MET3,var2_bins,var2_xmin,var2_xmax,ovflow,var2_color,1);
  TH1F    *hSovSqrtSplusB_Var2_MET3 = getSvsB(t1,sigtree,var2name,cut_MET3,var2_bins,var2_xmin,var2_xmax,ovflow,var2_color,2);
  TH1F    *hEffSvsVar_Var2_MET3 =     getSvsB(t1,sigtree,var2name,cut_MET3,var2_bins,var2_xmin,var2_xmax,ovflow,var2_color,3);
  TH1F    *hEffBvsVar_Var2_MET3 =     getSvsB(t1,sigtree,var2name,cut_MET3,var2_bins,var2_xmin,var2_xmax,ovflow,var2_color,4);
  TGraph    *gEff_Var2_MET3 = getVsEff(t1,sigtree,var2name,cut_MET3,var2_bins,var2_xmin,var2_xmax,ovflow,var2_color);
  TH1F    *hNorm_Var2_MET3     = create1Dhisto(sigtree, lumi, cut_MET3, var2name, var2_bins,var2_xmin,var2_xmax,false, var2_color, 1, var2_norm, ovflow);

// norm plots of vars
  TCanvas *cNorm_Var1_MET3   = new TCanvas("cNorm_Var1_MET3","cNorm_Var1_MET3",500,500);
  //gStyle->SetOptStat(1100000); // stat box: name, # entries
  hNorm_Var1_MET3->SetTitle(signame+"_"+var1name);
  hNorm_bkg_Var1_MET3->Draw("H");
  hNorm_Var1_MET3->Draw("SAME");
  leg0->Draw();
  cNorm_Var1_MET3->Print(signame+"_"+"norm_"+var1name+"_MET3.png");

  TCanvas *cNorm_Var2_MET3   = new TCanvas("cNorm_Var2_MET3","cNorm_Var2_MET3",500,500);
//  gStyle->SetOptStat(1100010); // stat box: name, # entries, ovflow count
  hNorm_Var2_MET3->SetTitle(signame+"_"+var2name);
  hNorm_bkg_Var2_MET3->Draw("H");
  hNorm_Var2_MET3->Draw("SAME");
  leg5->Draw();
  cNorm_Var2_MET3->Print(signame+"_"+"norm_"+var2name+"_MET3.png");

  
// ROC curves
  gStyle->SetOptStat(0);
  TCanvas *cROC_MET3 = new TCanvas("cROC_MET3","cROC_MET3",500,500);
  cROC_MET3->SetGrid(1);
  gROC_Var2_MET3->Draw("AL");
  gROC_Var2_MET3->GetYaxis()->SetRangeUser(0.,1.);
  gROC_Var2_MET3->GetXaxis()->SetRangeUser(0.,.25);
  gROC_Var2_MET3->SetTitle(signame);
  gROC_Var1_MET3->Draw("SAME");
  leg2->Draw();
  cROC_MET3->Print(signame+"_"+var1name+"_"+var2name+"_"+"ROC_MET3.png");

// eff_S/sqrt(eff_B) curves
  TCanvas *cROC_Eff_MET3 = new TCanvas("cROC_Eff_MET3","cROC_Eff_MET3",500,500);
  cROC_Eff_MET3->SetGrid(1);
  gEff_Var1_MET3->Draw("AL");
//  gEff_Var1->GetYaxis()->SetRangeUser(0.,2.5);
  gEff_Var1_MET3->SetTitle(signame);
  gEff_Var2_MET3->Draw("SAME");
  leg3->Draw();
  cROC_Eff_MET3->Print(signame+"_"+var1name+"_"+var2name+"_"+"ESovSqrtEB_MET3.png");

// S/B stuff, var 1
  TCanvas *cSovB_MET3 = new TCanvas("cSovB_MET3","cSovB_MET3",500,500);
  cSovB_MET3->cd(1);
  gPad->SetGrid(1);
  hSovB_Var1_MET3->Draw("L");
  hSovB_Var1_MET3->SetTitle(signame);
  hSovB_Var1_MET3->GetXaxis()->SetTitle(var1name);
  leg1->Draw();
  cSovB_MET3->Print(signame+"_"+var1name+"_"+"SovB_MET3.png");

  TCanvas *cSovSqrtB_MET3 = new TCanvas("cSovSqrtB_MET3","cSovSqrtB_MET3",500,500);
  cSovSqrtB_MET3->cd(1);
  gPad->SetGrid(1);
  hSovSqrtB_Var1_MET3->Draw("L");
  hSovSqrtB_Var1_MET3->SetTitle(signame);
  hSovSqrtB_Var1_MET3->GetXaxis()->SetTitle(var1name);
  //hSovSqrtB_Var4->Draw("SAME");
  //leg1->Draw();
  leg7->Draw();
  cSovSqrtB_MET3->Print(signame+"_"+var1name+"_"+"SovSqrtB_MET3.png");

  TCanvas *cSovSqrtSplusB_MET3 = new TCanvas("cSovSqrtSplusB_MET3","cSovSqrtSplusB_MET3",500,500);
  cSovSqrtSplusB_MET3->cd(1);
  gPad->SetGrid(1);
  hSovSqrtSplusB_Var1_MET3->Draw("L");
  hSovSqrtSplusB_Var1_MET3->SetTitle(signame);
  hSovSqrtSplusB_Var1_MET3->GetXaxis()->SetTitle(var1name);
  //hSovSqrtSplusB_Var4->Draw("SAME");
  leg7->Draw();
  cSovSqrtSplusB_MET3->Print(signame+"_"+var1name+"_"+"SovSqrtSplusB_MET3.png");

  TCanvas *cEffSvsVar1_MET3 = new TCanvas("cEffSvsVar1_MET3", "cEffSvsVar1_MET3",500,500);
  gPad->SetGrid(1);
  hEffSvsVar_Var1_MET3->Draw("L");
  hEffSvsVar_Var1_MET3->SetTitle(signame);
  hEffSvsVar_Var1_MET3->GetXaxis()->SetTitle(var1name);
  leg7->Draw();
  cEffSvsVar1_MET3->Print(signame+"_"+var1name+"_"+"EffSvsVar_MET3.png");

  TCanvas *cEffBvsVar1_MET3 = new TCanvas("cEffBvsVar1_MET3", "cEffBvsVar1_MET3",500,500);
  gPad->SetGrid(1);
  hEffBvsVar_Var1_MET3->Draw("L");
  hEffBvsVar_Var1_MET3->SetTitle(signame);
  hEffBvsVar_Var1_MET3->GetXaxis()->SetTitle(var1name);
  leg7->Draw();
  cEffBvsVar1_MET3->Print(signame+"_"+var1name+"_"+"EffBvsVar_MET3.png");

// S?B stuff, var 2 
  TCanvas *cSovB2_MET3 = new TCanvas("cSovB2_MET3","cSovB2_MET3",500,500);
  cSovB2_MET3->cd(1);
  gPad->SetGrid(1);
  //hSovB_Var2->GetYaxis()->SetRangeUser(0.,0.1);
  hSovB_Var2_MET3->Draw("L");
  hSovB_Var2_MET3->GetXaxis()->SetTitle(var2name);
  hSovB_Var2_MET3->SetTitle(signame);
  leg4->Draw();
  cSovB2_MET3->Print(signame+"_"+var2name+"_"+"SovB_MET3.png");

  TCanvas *cSovSqrtB2_MET3 = new TCanvas("cSovSqrtB2_MET3","cSovSqrtB2_MET3",500,500);
  cSovSqrtB2_MET3->cd(1);
  gPad->SetGrid(1);
  hSovSqrtB_Var2_MET3->Draw("L");
//  hSovSqrtB_Var2->GetYaxis()->SetRangeUser(0.,.55);
  hSovSqrtB_Var2_MET3->GetXaxis()->SetTitle(var2name);
  hSovSqrtB_Var2_MET3->SetTitle(signame);
  leg6->Draw();
  cSovSqrtB2_MET3->Print(signame+"_"+var2name+"_"+"SovSqrtB_MET3.png");

  TCanvas *cSovSqrtSplusB2_MET3 = new TCanvas("cSovSqrtSplusB2_MET3","cSovSqrtSplusB2_MET3",500,500);
  cSovSqrtSplusB2_MET3->cd(1);
  gPad->SetGrid(1);
  hSovSqrtSplusB_Var2_MET3->GetXaxis()->SetTitle(var2name);
  hSovSqrtSplusB_Var2_MET3->SetTitle(signame);
  hSovSqrtSplusB_Var2_MET3->Draw("L");
//  hSovSqrtSplusB_Var2->GetYaxis()->SetRangeUser(0.,.55);
  leg6->Draw();
  cSovSqrtSplusB2_MET3->Print(signame+"_"+var2name+"_"+"SovSqrtSplusB_MET3.png");

  TCanvas *cEffSvsVar2_MET3 = new TCanvas("cEffSvsVar2_MET3", "cEffSvsVar2_MET3",500,500);
  gPad->SetGrid(1);
  hEffSvsVar_Var2_MET3->Draw("L");
  hEffSvsVar_Var2_MET3->SetTitle(signame);
  hEffSvsVar_Var2_MET3->GetXaxis()->SetTitle(var2name);
  leg6->Draw();
  cEffSvsVar2_MET3->Print(signame+"_"+var2name+"_"+"EffSvsVar_MET3.png");

  TCanvas *cEffBvsVar2_MET3 = new TCanvas("cEffBvsVar2_MET3", "cEffBvsVar2_MET3",500,500);
  gPad->SetGrid(1);
  hEffBvsVar_Var2_MET3->Draw("L");
  hEffBvsVar_Var2_MET3->SetTitle(signame);
  hEffBvsVar_Var2_MET3->GetXaxis()->SetTitle(var2name);
  leg6->Draw();
  cEffBvsVar2_MET3->Print(signame+"_"+var2name+"_"+"EffBvsVar_MET3.png");



// MET 4
  // bkg
  TH1F    *hNorm_bkg_Var1_MET4 = create1Dhisto(t1, lumi, cut_MET4, var1name, var1_bins,var1_xmin,var1_xmax,false, bkg_color, 1, var1_norm, ovflow);
  TH1F    *hNorm_bkg_Var2_MET4 = create1Dhisto(t1, lumi, cut_MET4, var2name, var2_bins,var2_xmin,var2_xmax,false, bkg_color, 1, var2_norm, ovflow);

  // var1
  TGraph  *gROC_Var1_MET4   = drawROCcurve(t1,sigtree,var1name,cut_MET4,1000,var1_xmin,var1_xmax,ovflow,var1_color);
  TH1F    *hSovB_Var1_MET4  =         getSvsB(t1,sigtree,var1name,cut_MET4,var1_bins,var1_xmin,var1_xmax,ovflow,var1_color,0);
  TH1F    *hSovSqrtB_Var1_MET4 =      getSvsB(t1,sigtree,var1name,cut_MET4,var1_bins,var1_xmin,var1_xmax,ovflow,var1_color,1);
  TH1F    *hSovSqrtSplusB_Var1_MET4 = getSvsB(t1,sigtree,var1name,cut_MET4,var1_bins,var1_xmin,var1_xmax,ovflow,var1_color,2);
  TH1F    *hEffSvsVar_Var1_MET4 =     getSvsB(t1,sigtree,var1name,cut_MET4,var1_bins,var1_xmin,var1_xmax,ovflow,var1_color,3);
  TH1F    *hEffBvsVar_Var1_MET4 =     getSvsB(t1,sigtree,var1name,cut_MET4,var1_bins,var1_xmin,var1_xmax,ovflow,var1_color,4);
  TGraph  *gEff_Var1_MET4 = getVsEff(t1,sigtree,var1name,cut_MET4,var1_bins,var1_xmin,var1_xmax,ovflow,var1_color);
  TH1F    *hNorm_Var1_MET4     = create1Dhisto(sigtree, lumi, cut_MET4, var1name, var1_bins,var1_xmin,var1_xmax,false, var1_color, 1, var1_norm, ovflow);

  // var2
  TGraph  *gROC_Var2_MET4   = drawROCcurve(t1,sigtree,var2name,cut_MET4,1000,var2_xmin,var2_xmax,ovflow,var2_color);
  TH1F    *hSovB_Var2_MET4  = getSvsB(t1,sigtree,var2name,cut_MET4,var2_bins,var2_xmin,var2_xmax,ovflow,var2_color,0);
  TH1F    *hSovSqrtB_Var2_MET4 = getSvsB(t1,sigtree,var2name,cut_MET4,var2_bins,var2_xmin,var2_xmax,ovflow,var2_color,1);
  TH1F    *hSovSqrtSplusB_Var2_MET4 = getSvsB(t1,sigtree,var2name,cut_MET4,var2_bins,var2_xmin,var2_xmax,ovflow,var2_color,2);
  TH1F    *hEffSvsVar_Var2_MET4 =     getSvsB(t1,sigtree,var2name,cut_MET4,var2_bins,var2_xmin,var2_xmax,ovflow,var2_color,3);
  TH1F    *hEffBvsVar_Var2_MET4 =     getSvsB(t1,sigtree,var2name,cut_MET4,var2_bins,var2_xmin,var2_xmax,ovflow,var2_color,4);
  TGraph    *gEff_Var2_MET4 = getVsEff(t1,sigtree,var2name,cut_MET4,var2_bins,var2_xmin,var2_xmax,ovflow,var2_color);
  TH1F    *hNorm_Var2_MET4     = create1Dhisto(sigtree, lumi, cut_MET4, var2name, var2_bins,var2_xmin,var2_xmax,false, var2_color, 1, var2_norm, ovflow);

// norm plots of vars
  TCanvas *cNorm_Var1_MET4   = new TCanvas("cNorm_Var1_MET4","cNorm_Var1_MET4",500,500);
  //gStyle->SetOptStat(1100000); // stat box: name, # entries
  hNorm_Var1_MET4->SetTitle(signame+"_"+var1name);
  hNorm_bkg_Var1_MET4->Draw("H");
  hNorm_Var1_MET4->Draw("SAME");
  leg0->Draw();
  cNorm_Var1_MET4->Print(signame+"_"+"norm_"+var1name+"_MET4.png");

  TCanvas *cNorm_Var2_MET4   = new TCanvas("cNorm_Var2_MET4","cNorm_Var2_MET4",500,500);
//  gStyle->SetOptStat(1100010); // stat box: name, # entries, ovflow count
  hNorm_Var2_MET4->SetTitle(signame+"_"+var2name);
  hNorm_bkg_Var2_MET4->Draw("H");
  hNorm_Var2_MET4->Draw("SAME");
  leg5->Draw();
  cNorm_Var2_MET4->Print(signame+"_"+"norm_"+var2name+"_MET4.png");

  
// ROC curves
  gStyle->SetOptStat(0);
  TCanvas *cROC_MET4 = new TCanvas("cROC_MET4","cROC_MET4",500,500);
  cROC_MET4->SetGrid(1);
  gROC_Var2_MET4->Draw("AL");
  gROC_Var2_MET4->GetYaxis()->SetRangeUser(0.,1.);
  gROC_Var2_MET4->GetXaxis()->SetRangeUser(0.,.25);
  gROC_Var2_MET4->SetTitle(signame);
  gROC_Var1_MET4->Draw("SAME");
  leg2->Draw();
  cROC_MET4->Print(signame+"_"+var1name+"_"+var2name+"_"+"ROC_MET4.png");

// eff_S/sqrt(eff_B) curves
  TCanvas *cROC_Eff_MET4 = new TCanvas("cROC_Eff_MET4","cROC_Eff_MET4",500,500);
  cROC_Eff_MET4->SetGrid(1);
  gEff_Var1_MET4->Draw("AL");
//  gEff_Var1->GetYaxis()->SetRangeUser(0.,2.5);
  gEff_Var1_MET4->SetTitle(signame);
  gEff_Var2_MET4->Draw("SAME");
  leg3->Draw();
  cROC_Eff_MET4->Print(signame+"_"+var1name+"_"+var2name+"_"+"ESovSqrtEB_MET4.png");

// S/B stuff, var 1
  TCanvas *cSovB_MET4 = new TCanvas("cSovB_MET4","cSovB_MET4",500,500);
  cSovB_MET4->cd(1);
  gPad->SetGrid(1);
  hSovB_Var1_MET4->Draw("L");
  hSovB_Var1_MET4->SetTitle(signame);
  hSovB_Var1_MET4->GetXaxis()->SetTitle(var1name);
  leg1->Draw();
  cSovB_MET4->Print(signame+"_"+var1name+"_"+"SovB_MET4.png");

  TCanvas *cSovSqrtB_MET4 = new TCanvas("cSovSqrtB_MET4","cSovSqrtB_MET4",500,500);
  cSovSqrtB_MET4->cd(1);
  gPad->SetGrid(1);
  hSovSqrtB_Var1_MET4->Draw("L");
  hSovSqrtB_Var1_MET4->SetTitle(signame);
  hSovSqrtB_Var1_MET4->GetXaxis()->SetTitle(var1name);
  //hSovSqrtB_Var4->Draw("SAME");
  //leg1->Draw();
  leg7->Draw();
  cSovSqrtB_MET4->Print(signame+"_"+var1name+"_"+"SovSqrtB_MET4.png");

  TCanvas *cSovSqrtSplusB_MET4 = new TCanvas("cSovSqrtSplusB_MET4","cSovSqrtSplusB_MET4",500,500);
  cSovSqrtSplusB_MET4->cd(1);
  gPad->SetGrid(1);
  hSovSqrtSplusB_Var1_MET4->Draw("L");
  hSovSqrtSplusB_Var1_MET4->SetTitle(signame);
  hSovSqrtSplusB_Var1_MET4->GetXaxis()->SetTitle(var1name);
  //hSovSqrtSplusB_Var4->Draw("SAME");
  leg7->Draw();
  cSovSqrtSplusB_MET4->Print(signame+"_"+var1name+"_"+"SovSqrtSplusB_MET4.png");

  TCanvas *cEffSvsVar1_MET4 = new TCanvas("cEffSvsVar1_MET4", "cEffSvsVar1_MET4",500,500);
  gPad->SetGrid(1);
  hEffSvsVar_Var1_MET4->Draw("L");
  hEffSvsVar_Var1_MET4->SetTitle(signame);
  hEffSvsVar_Var1_MET4->GetXaxis()->SetTitle(var1name);
  leg7->Draw();
  cEffSvsVar1_MET4->Print(signame+"_"+var1name+"_"+"EffSvsVar_MET4.png");

  TCanvas *cEffBvsVar1_MET4 = new TCanvas("cEffBvsVar1_MET4", "cEffBvsVar1_MET4",500,500);
  gPad->SetGrid(1);
  hEffBvsVar_Var1_MET4->Draw("L");
  hEffBvsVar_Var1_MET4->SetTitle(signame);
  hEffBvsVar_Var1_MET4->GetXaxis()->SetTitle(var1name);
  leg7->Draw();
  cEffBvsVar1_MET4->Print(signame+"_"+var1name+"_"+"EffBvsVar_MET4.png");

// S?B stuff, var 2 
  TCanvas *cSovB2_MET4 = new TCanvas("cSovB2_MET4","cSovB2_MET4",500,500);
  cSovB2_MET4->cd(1);
  gPad->SetGrid(1);
  //hSovB_Var2->GetYaxis()->SetRangeUser(0.,0.1);
  hSovB_Var2_MET4->Draw("L");
  hSovB_Var2_MET4->GetXaxis()->SetTitle(var2name);
  hSovB_Var2_MET4->SetTitle(signame);
  leg4->Draw();
  cSovB2_MET4->Print(signame+"_"+var2name+"_"+"SovB_MET4.png");

  TCanvas *cSovSqrtB2_MET4 = new TCanvas("cSovSqrtB2_MET4","cSovSqrtB2_MET4",500,500);
  cSovSqrtB2_MET4->cd(1);
  gPad->SetGrid(1);
  hSovSqrtB_Var2_MET4->Draw("L");
//  hSovSqrtB_Var2->GetYaxis()->SetRangeUser(0.,.55);
  hSovSqrtB_Var2_MET4->GetXaxis()->SetTitle(var2name);
  hSovSqrtB_Var2_MET4->SetTitle(signame);
  leg6->Draw();
  cSovSqrtB2_MET4->Print(signame+"_"+var2name+"_"+"SovSqrtB_MET4.png");

  TCanvas *cSovSqrtSplusB2_MET4 = new TCanvas("cSovSqrtSplusB2_MET4","cSovSqrtSplusB2_MET4",500,500);
  cSovSqrtSplusB2_MET4->cd(1);
  gPad->SetGrid(1);
  hSovSqrtSplusB_Var2_MET4->GetXaxis()->SetTitle(var2name);
  hSovSqrtSplusB_Var2_MET4->SetTitle(signame);
  hSovSqrtSplusB_Var2_MET4->Draw("L");
//  hSovSqrtSplusB_Var2->GetYaxis()->SetRangeUser(0.,.55);
  leg6->Draw();
  cSovSqrtSplusB2_MET4->Print(signame+"_"+var2name+"_"+"SovSqrtSplusB_MET4.png");

  TCanvas *cEffSvsVar2_MET4 = new TCanvas("cEffSvsVar2_MET4", "cEffSvsVar2_MET4",500,500);
  gPad->SetGrid(1);
  hEffSvsVar_Var2_MET4->Draw("L");
  hEffSvsVar_Var2_MET4->SetTitle(signame);
  hEffSvsVar_Var2_MET4->GetXaxis()->SetTitle(var2name);
  leg6->Draw();
  cEffSvsVar2_MET4->Print(signame+"_"+var2name+"_"+"EffSvsVar_MET4.png");

  TCanvas *cEffBvsVar2_MET4 = new TCanvas("cEffBvsVar2_MET4", "cEffBvsVar2_MET4",500,500);
  gPad->SetGrid(1);
  hEffBvsVar_Var2_MET4->Draw("L");
  hEffBvsVar_Var2_MET4->SetTitle(signame);
  hEffBvsVar_Var2_MET4->GetXaxis()->SetTitle(var2name);
  leg6->Draw();
  cEffBvsVar2_MET4->Print(signame+"_"+var2name+"_"+"EffBvsVar_MET4.png");



// MET 5
  // bkg
  TH1F    *hNorm_bkg_Var1_MET5 = create1Dhisto(t1, lumi, cut_MET5, var1name, var1_bins,var1_xmin,var1_xmax,false, bkg_color, 1, var1_norm, ovflow);
  TH1F    *hNorm_bkg_Var2_MET5 = create1Dhisto(t1, lumi, cut_MET5, var2name, var2_bins,var2_xmin,var2_xmax,false, bkg_color, 1, var2_norm, ovflow);

  // var1
  TGraph  *gROC_Var1_MET5   = drawROCcurve(t1,sigtree,var1name,cut_MET5,1000,var1_xmin,var1_xmax,ovflow,var1_color);
  TH1F    *hSovB_Var1_MET5  =         getSvsB(t1,sigtree,var1name,cut_MET5,var1_bins,var1_xmin,var1_xmax,ovflow,var1_color,0);
  TH1F    *hSovSqrtB_Var1_MET5 =      getSvsB(t1,sigtree,var1name,cut_MET5,var1_bins,var1_xmin,var1_xmax,ovflow,var1_color,1);
  TH1F    *hSovSqrtSplusB_Var1_MET5 = getSvsB(t1,sigtree,var1name,cut_MET5,var1_bins,var1_xmin,var1_xmax,ovflow,var1_color,2);
  TH1F    *hEffSvsVar_Var1_MET5 =     getSvsB(t1,sigtree,var1name,cut_MET5,var1_bins,var1_xmin,var1_xmax,ovflow,var1_color,3);
  TH1F    *hEffBvsVar_Var1_MET5 =     getSvsB(t1,sigtree,var1name,cut_MET5,var1_bins,var1_xmin,var1_xmax,ovflow,var1_color,4);
  TGraph  *gEff_Var1_MET5 = getVsEff(t1,sigtree,var1name,cut_MET5,var1_bins,var1_xmin,var1_xmax,ovflow,var1_color);
  TH1F    *hNorm_Var1_MET5     = create1Dhisto(sigtree, lumi, cut_MET5, var1name, var1_bins,var1_xmin,var1_xmax,false, var1_color, 1, var1_norm, ovflow);

  // var2
  TGraph  *gROC_Var2_MET5   = drawROCcurve(t1,sigtree,var2name,cut_MET5,1000,var2_xmin,var2_xmax,ovflow,var2_color);
  TH1F    *hSovB_Var2_MET5  = getSvsB(t1,sigtree,var2name,cut_MET5,var2_bins,var2_xmin,var2_xmax,ovflow,var2_color,0);
  TH1F    *hSovSqrtB_Var2_MET5 = getSvsB(t1,sigtree,var2name,cut_MET5,var2_bins,var2_xmin,var2_xmax,ovflow,var2_color,1);
  TH1F    *hSovSqrtSplusB_Var2_MET5 = getSvsB(t1,sigtree,var2name,cut_MET5,var2_bins,var2_xmin,var2_xmax,ovflow,var2_color,2);
  TH1F    *hEffSvsVar_Var2_MET5 =     getSvsB(t1,sigtree,var2name,cut_MET5,var2_bins,var2_xmin,var2_xmax,ovflow,var2_color,3);
  TH1F    *hEffBvsVar_Var2_MET5 =     getSvsB(t1,sigtree,var2name,cut_MET5,var2_bins,var2_xmin,var2_xmax,ovflow,var2_color,4);
  TGraph    *gEff_Var2_MET5 = getVsEff(t1,sigtree,var2name,cut_MET5,var2_bins,var2_xmin,var2_xmax,ovflow,var2_color);
  TH1F    *hNorm_Var2_MET5     = create1Dhisto(sigtree, lumi, cut_MET5, var2name, var2_bins,var2_xmin,var2_xmax,false, var2_color, 1, var2_norm, ovflow);

// norm plots of vars
  TCanvas *cNorm_Var1_MET5   = new TCanvas("cNorm_Var1_MET5","cNorm_Var1_MET5",500,500);
  //gStyle->SetOptStat(1100000); // stat box: name, # entries
  hNorm_Var1_MET5->SetTitle(signame+"_"+var1name);
  hNorm_bkg_Var1_MET5->Draw("H");
  hNorm_Var1_MET5->Draw("SAME");
  leg0->Draw();
  cNorm_Var1_MET5->Print(signame+"_"+"norm_"+var1name+"_MET5.png");

  TCanvas *cNorm_Var2_MET5   = new TCanvas("cNorm_Var2_MET5","cNorm_Var2_MET5",500,500);
//  gStyle->SetOptStat(1100010); // stat box: name, # entries, ovflow count
  hNorm_Var2_MET5->SetTitle(signame+"_"+var2name);
  hNorm_bkg_Var2_MET5->Draw("H");
  hNorm_Var2_MET5->Draw("SAME");
  leg5->Draw();
  cNorm_Var2_MET5->Print(signame+"_"+"norm_"+var2name+"_MET5.png");

  
// ROC curves
  gStyle->SetOptStat(0);
  TCanvas *cROC_MET5 = new TCanvas("cROC_MET5","cROC_MET5",500,500);
  cROC_MET5->SetGrid(1);
  gROC_Var2_MET5->Draw("AL");
  gROC_Var2_MET5->GetYaxis()->SetRangeUser(0.,1.);
  gROC_Var2_MET5->GetXaxis()->SetRangeUser(0.,.25);
  gROC_Var2_MET5->SetTitle(signame);
  gROC_Var1_MET5->Draw("SAME");
  leg2->Draw();
  cROC_MET5->Print(signame+"_"+var1name+"_"+var2name+"_"+"ROC_MET5.png");

// eff_S/sqrt(eff_B) curves
  TCanvas *cROC_Eff_MET5 = new TCanvas("cROC_Eff_MET5","cROC_Eff_MET5",500,500);
  cROC_Eff_MET5->SetGrid(1);
  gEff_Var1_MET5->Draw("AL");
//  gEff_Var1->GetYaxis()->SetRangeUser(0.,2.5);
  gEff_Var1_MET5->SetTitle(signame);
  gEff_Var2_MET5->Draw("SAME");
  leg3->Draw();
  cROC_Eff_MET5->Print(signame+"_"+var1name+"_"+var2name+"_"+"ESovSqrtEB_MET5.png");

// S/B stuff, var 1
  TCanvas *cSovB_MET5 = new TCanvas("cSovB_MET5","cSovB_MET5",500,500);
  cSovB_MET5->cd(1);
  gPad->SetGrid(1);
  hSovB_Var1_MET5->Draw("L");
  hSovB_Var1_MET5->SetTitle(signame);
  hSovB_Var1_MET5->GetXaxis()->SetTitle(var1name);
  leg1->Draw();
  cSovB_MET5->Print(signame+"_"+var1name+"_"+"SovB_MET5.png");

  TCanvas *cSovSqrtB_MET5 = new TCanvas("cSovSqrtB_MET5","cSovSqrtB_MET5",500,500);
  cSovSqrtB_MET5->cd(1);
  gPad->SetGrid(1);
  hSovSqrtB_Var1_MET5->Draw("L");
  hSovSqrtB_Var1_MET5->SetTitle(signame);
  hSovSqrtB_Var1_MET5->GetXaxis()->SetTitle(var1name);
  //hSovSqrtB_Var4->Draw("SAME");
  //leg1->Draw();
  leg7->Draw();
  cSovSqrtB_MET5->Print(signame+"_"+var1name+"_"+"SovSqrtB_MET5.png");

  TCanvas *cSovSqrtSplusB_MET5 = new TCanvas("cSovSqrtSplusB_MET5","cSovSqrtSplusB_MET5",500,500);
  cSovSqrtSplusB_MET5->cd(1);
  gPad->SetGrid(1);
  hSovSqrtSplusB_Var1_MET5->Draw("L");
  hSovSqrtSplusB_Var1_MET5->SetTitle(signame);
  hSovSqrtSplusB_Var1_MET5->GetXaxis()->SetTitle(var1name);
  //hSovSqrtSplusB_Var4->Draw("SAME");
  leg7->Draw();
  cSovSqrtSplusB_MET5->Print(signame+"_"+var1name+"_"+"SovSqrtSplusB_MET5.png");

  TCanvas *cEffSvsVar1_MET5 = new TCanvas("cEffSvsVar1_MET5", "cEffSvsVar1_MET5",500,500);
  gPad->SetGrid(1);
  hEffSvsVar_Var1_MET5->Draw("L");
  hEffSvsVar_Var1_MET5->SetTitle(signame);
  hEffSvsVar_Var1_MET5->GetXaxis()->SetTitle(var1name);
  leg7->Draw();
  cEffSvsVar1_MET5->Print(signame+"_"+var1name+"_"+"EffSvsVar_MET5.png");

  TCanvas *cEffBvsVar1_MET5 = new TCanvas("cEffBvsVar1_MET5", "cEffBvsVar1_MET5",500,500);
  gPad->SetGrid(1);
  hEffBvsVar_Var1_MET5->Draw("L");
  hEffBvsVar_Var1_MET5->SetTitle(signame);
  hEffBvsVar_Var1_MET5->GetXaxis()->SetTitle(var1name);
  leg7->Draw();
  cEffBvsVar1_MET5->Print(signame+"_"+var1name+"_"+"EffBvsVar_MET5.png");

// S?B stuff, var 2 
  TCanvas *cSovB2_MET5 = new TCanvas("cSovB2_MET5","cSovB2_MET5",500,500);
  cSovB2_MET5->cd(1);
  gPad->SetGrid(1);
  //hSovB_Var2->GetYaxis()->SetRangeUser(0.,0.1);
  hSovB_Var2_MET5->Draw("L");
  hSovB_Var2_MET5->GetXaxis()->SetTitle(var2name);
  hSovB_Var2_MET5->SetTitle(signame);
  leg4->Draw();
  cSovB2_MET5->Print(signame+"_"+var2name+"_"+"SovB_MET5.png");

  TCanvas *cSovSqrtB2_MET5 = new TCanvas("cSovSqrtB2_MET5","cSovSqrtB2_MET5",500,500);
  cSovSqrtB2_MET5->cd(1);
  gPad->SetGrid(1);
  hSovSqrtB_Var2_MET5->Draw("L");
//  hSovSqrtB_Var2->GetYaxis()->SetRangeUser(0.,.55);
  hSovSqrtB_Var2_MET5->GetXaxis()->SetTitle(var2name);
  hSovSqrtB_Var2_MET5->SetTitle(signame);
  leg6->Draw();
  cSovSqrtB2_MET5->Print(signame+"_"+var2name+"_"+"SovSqrtB_MET5.png");

  TCanvas *cSovSqrtSplusB2_MET5 = new TCanvas("cSovSqrtSplusB2_MET5","cSovSqrtSplusB2_MET5",500,500);
  cSovSqrtSplusB2_MET5->cd(1);
  gPad->SetGrid(1);
  hSovSqrtSplusB_Var2_MET5->GetXaxis()->SetTitle(var2name);
  hSovSqrtSplusB_Var2_MET5->SetTitle(signame);
  hSovSqrtSplusB_Var2_MET5->Draw("L");
//  hSovSqrtSplusB_Var2->GetYaxis()->SetRangeUser(0.,.55);
  leg6->Draw();
  cSovSqrtSplusB2_MET5->Print(signame+"_"+var2name+"_"+"SovSqrtSplusB_MET5.png");

  TCanvas *cEffSvsVar2_MET5 = new TCanvas("cEffSvsVar2_MET5", "cEffSvsVar2_MET5",500,500);
  gPad->SetGrid(1);
  hEffSvsVar_Var2_MET5->Draw("L");
  hEffSvsVar_Var2_MET5->SetTitle(signame);
  hEffSvsVar_Var2_MET5->GetXaxis()->SetTitle(var2name);
  leg6->Draw();
  cEffSvsVar2_MET5->Print(signame+"_"+var2name+"_"+"EffSvsVar_MET5.png");

  TCanvas *cEffBvsVar2_MET5 = new TCanvas("cEffBvsVar2_MET5", "cEffBvsVar2_MET5",500,500);
  gPad->SetGrid(1);
  hEffBvsVar_Var2_MET5->Draw("L");
  hEffBvsVar_Var2_MET5->SetTitle(signame);
  hEffBvsVar_Var2_MET5->GetXaxis()->SetTitle(var2name);
  leg6->Draw();
  cEffBvsVar2_MET5->Print(signame+"_"+var2name+"_"+"EffBvsVar_MET5.png");


} // end of void mainfunction()

TGraph *drawROCcurve(TTree *tBKG,TTree *tSIG,TString branch,TString cuts,int bins,double xmin,double xmax,bool ovFlow,int color) {
  TH1::SetDefaultSumw2(kTRUE);

  TString cut ="(ScaleFactor)*("+cuts+")";


  TH1F *hBKG = new TH1F("hBKG","hBKG",bins,xmin,xmax);
  tBKG->Project("hBKG",branch,cut);

  TH1F *hSIG = new TH1F("hSIG","hSIG",bins,xmin,xmax);
  tSIG->Project("hSIG",branch,cut);


  double initBKG = 0.;
  double initSIG = 0.;


  if (ovFlow) {
    double errInitBKG = 0.; initBKG = hBKG->IntegralAndError(0,bins+1,errInitBKG);
    double errInitSIG = 0.; initSIG = hSIG->IntegralAndError(0,bins+1,errInitSIG);
  }
  else {
    double errInitBKG = 0.; initBKG = hBKG->IntegralAndError(0,bins,errInitBKG);
    double errInitSIG = 0.; initSIG = hSIG->IntegralAndError(0,bins,errInitSIG);
  }


  const int nBins = bins;
  double eBKG[nBins];
  double eSIG[nBins];

  for (UInt_t i=0; i<(UInt_t)bins; i++) {

    double intBKG = 0.;
    double intSIG = 0.;

    if (ovFlow) {
      double errBKG = 0.; intBKG = hBKG->IntegralAndError(i,bins+1,errBKG);
      double errSIG = 0.; intSIG = hSIG->IntegralAndError(i,bins+1,errSIG);
    }
    else {
      double errBKG = 0.; intBKG = hBKG->IntegralAndError(i,bins,errBKG);
      double errSIG = 0.; intSIG = hSIG->IntegralAndError(i,bins,errSIG);
    }

    eBKG[i] = intBKG/initBKG;
    eSIG[i] = intSIG/initSIG;
  }

  hBKG->Delete();
  hSIG->Delete();

  TGraph *gROC = new TGraph(nBins,eBKG,eSIG);
  gROC->SetLineWidth(3);
  gROC->SetLineColor(color);
  gROC->GetXaxis()->SetRangeUser(0.,1.);
  gROC->GetYaxis()->SetRangeUser(0.,1.);
  gROC->GetXaxis()->SetTitle("#epsilon(BKG)");
  gROC->GetXaxis()->CenterTitle();
  gROC->GetYaxis()->SetTitle("#epsilon(SIG)");
  gROC->GetYaxis()->CenterTitle();
  gROC->SetTitle(branch);

  return gROC;
}

// returns a histo of S/B, S/sqrt(B), S/sqrt(S+B) vs cut on variable; or Eff(S) or Eff(B) vs cut on variable. Selection option through SovB parameter.
TH1F *getSvsB(TTree *tBKG,TTree *tSIG,TString branch,TString cuts,int bins,double xmin,double xmax,bool ovFlow,int color,int SovB) {
//void getSovB(std::vector<TH1F*> &hTemp,TTree *tBKG,TTree *tSIG,TString branch,TString cuts,int bins,double xmin,double xmax,bool ovFlow,int color) {
  //std::vector<TH1F*> getSovB(TTree *tBKG,TTree *tSIG,TString branch,TString cuts,int bins,double xmin,double xmax,bool ovFlow,int color) {
  TH1::SetDefaultSumw2(kTRUE);

  TString cut ="(ScaleFactor)*("+cuts+")";

  TH1F *hBKG = new TH1F("hBKG","hBKG",bins,xmin,xmax);
  tBKG->Project("hBKG",branch,cut);

  TH1F *hSIG = new TH1F("hSIG","hSIG",bins,xmin,xmax);
  tSIG->Project("hSIG",branch,cut);

  TH1F *hSovB     = (TH1F*)hSIG->Clone("hSovB");
  hSovB->SetMarkerColor(color);
  hSovB->SetLineColor(color);
  hSovB->SetLineWidth(3);
  hSovB->GetXaxis()->SetTitle(branch);
  hSovB->GetXaxis()->CenterTitle();
  hSovB->GetYaxis()->SetTitle("S/B");
  hSovB->GetYaxis()->CenterTitle();
  hSovB->SetTitle(branch);

  TH1F *hSovSqrtB = (TH1F*)hSIG->Clone("hSovSqrtB");
  hSovSqrtB->SetMarkerColor(color);
  hSovSqrtB->SetLineColor(color);
  hSovSqrtB->SetLineWidth(3);
  hSovSqrtB->GetXaxis()->SetTitle(branch);
  hSovSqrtB->GetXaxis()->CenterTitle();
  hSovSqrtB->GetYaxis()->SetTitle("S/sqrt(B)");
  hSovSqrtB->GetYaxis()->CenterTitle();
  hSovSqrtB->SetTitle(branch);

  TH1F *hSovSqrtSplusB = (TH1F*)hSIG->Clone("hSovSqrtSplusB");
  hSovSqrtSplusB->SetMarkerColor(color);
  hSovSqrtSplusB->SetLineColor(color);
  hSovSqrtSplusB->SetLineWidth(3);
  hSovSqrtSplusB->GetXaxis()->SetTitle(branch);
  hSovSqrtSplusB->GetXaxis()->CenterTitle();
  hSovSqrtSplusB->GetYaxis()->SetTitle("S/sqrt(S+B)");
  hSovSqrtSplusB->GetYaxis()->CenterTitle();
  hSovSqrtSplusB->SetTitle(branch);

  TH1F *hEffSvsVar = (TH1F*)hSIG->Clone("hEffSvsVar");
  hEffSvsVar->SetMarkerColor(color);
  hEffSvsVar->SetLineColor(color);
  hEffSvsVar->SetLineWidth(3);
  hEffSvsVar->GetXaxis()->SetTitle(branch);
  hEffSvsVar->GetXaxis()->CenterTitle();
  hEffSvsVar->GetYaxis()->SetTitle("#epsilon(SIG)");
  hEffSvsVar->GetYaxis()->CenterTitle();
  hEffSvsVar->SetTitle(branch);

  TH1F *hEffBvsVar = (TH1F*)hSIG->Clone("hEffBvsVar");
  hEffBvsVar->SetMarkerColor(color);
  hEffBvsVar->SetLineColor(color);
  hEffBvsVar->SetLineWidth(3);
  hEffBvsVar->GetXaxis()->SetTitle(branch);
  hEffBvsVar->GetXaxis()->CenterTitle();
  hEffBvsVar->GetYaxis()->SetTitle("#epsilon(BKG)");
  hEffBvsVar->GetYaxis()->CenterTitle();
  hEffBvsVar->SetTitle(branch);

  double initBKG = 0.;
  double initSIG = 0.;

  // total integrals
  if (ovFlow) {
    double errInitBKG = 0.; initBKG = hBKG->IntegralAndError(0,bins+1,errInitBKG);
    double errInitSIG = 0.; initSIG = hSIG->IntegralAndError(0,bins+1,errInitSIG);
  }
  else {
    double errInitBKG = 0.; initBKG = hBKG->IntegralAndError(0,bins,errInitBKG);
    double errInitSIG = 0.; initSIG = hSIG->IntegralAndError(0,bins,errInitSIG);
  }

  // note loops 1 to bins here, as these are the main bins of the histo
  for (UInt_t i=1; i<=(UInt_t)bins; i++) {

    double yBKG = 0.;
    double ySIG = 0.;


    if (ovFlow) {
      double errBKG = 0.; yBKG = hBKG->IntegralAndError(i,bins+1,errBKG);
      double errSIG = 0.; ySIG = hSIG->IntegralAndError(i,bins+1,errSIG);
    }
    else {

      double errBKG = 0.; yBKG = hBKG->IntegralAndError(i,bins,errBKG);
      double errSIG = 0.; ySIG = hSIG->IntegralAndError(i,bins,errSIG);
    }

    // zero checks
    if (yBKG != 0.) {
      hSovB->SetBinContent(i,ySIG/yBKG);
      hSovSqrtB->SetBinContent(i,ySIG/sqrt(yBKG));
    }
    else {
      hSovB->SetBinContent(i,0);
      hSovSqrtB->SetBinContent(i,0);
    }
    if (ySIG+yBKG != 0.) {
      hSovSqrtSplusB->SetBinContent(i,ySIG/sqrt(ySIG+yBKG));
    }
    else {
      hSovSqrtSplusB->SetBinContent(i,0);
    }

    hEffSvsVar->SetBinContent(i,ySIG/initSIG);
    hEffBvsVar->SetBinContent(i,yBKG/initBKG);

    hSovB->SetBinError(i,0.);
    hSovSqrtB->SetBinError(i,0.);
    hSovSqrtSplusB->SetBinError(i,0.);
    hEffSvsVar->SetBinError(i,0.);
    hEffBvsVar->SetBinError(i,0.);


    //hSovB->SetBinContent(i,ySIG/yBKG);
    //hSovSqrtB->SetBinContent(i,ySIG/sqrt(yBKG));
    //hSovSqrtSplusB->SetBinContent(i,ySIG/sqrt(ySIG+yBKG));
  }

/*
hSovB->SetBinContent(bins,0.);
hSovB->SetBinError(bins,0.);
hSovSqrtB->SetBinContent(bins,0.);
hSovSqrtB->SetBinError(bins,0.);
*/
  //  std::vector<TH1F*> hTemp;
  //  hTemp.push_back(hSovB);
  //  hTemp.push_back(hSovSqrtB);
  //  std::vector<double> hTemp;
  //  hTemp.push_back(4.); hTemp.push_back(5.);
  hBKG->Delete();
  hSIG->Delete();

  if (SovB == 0) {
      /* hSovB->Delete(); */ hSovSqrtB->Delete(); hSovSqrtSplusB->Delete(); hEffSvsVar->Delete(); hEffBvsVar->Delete();
      return hSovB;
  } else if (SovB == 1) {
      hSovB->Delete(); /* hSovSqrtB->Delete(); */ hSovSqrtSplusB->Delete(); hEffSvsVar->Delete(); hEffBvsVar->Delete();
      return hSovSqrtB;
  } else if (SovB == 2) {
      hSovB->Delete(); hSovSqrtB->Delete();/*  hSovSqrtSplusB->Delete(); */ hEffSvsVar->Delete(); hEffBvsVar->Delete();
      return hSovSqrtSplusB;
  } else if (SovB == 3) {
      hSovB->Delete(); hSovSqrtB->Delete(); hSovSqrtSplusB->Delete(); /* hEffSvsVar->Delete();*/  hEffBvsVar->Delete();
      return hEffSvsVar;
  } else if (SovB == 4) {
      hSovB->Delete(); hSovSqrtB->Delete(); hSovSqrtSplusB->Delete(); hEffSvsVar->Delete(); /* hEffBvsVar->Delete(); */
      return hEffBvsVar;
  } else {
      cout << "Fatal: Please SovB of 0 thru 4" << endl;
    return hSovB;
  }
  /*
  if (SovB) { hSovSqrtB->Delete(); return hSovB;     }
  else      { hSovB->Delete();     return hSovSqrtB; }
  */
}

// returns a TGraph of variable vs efficiency
TGraph *getVsEff(TTree *tBKG,TTree *tSIG,TString branch,TString cuts,int bins,double xmin,double xmax,bool ovFlow,int color) {
//void getSovB(std::vector<TH1F*> &hTemp,TTree *tBKG,TTree *tSIG,TString branch,TString cuts,int bins,double xmin,double xmax,bool ovFlow,int color) {
  //std::vector<TH1F*> getSovB(TTree *tBKG,TTree *tSIG,TString branch,TString cuts,int bins,double xmin,double xmax,bool ovFlow,int color) {
  TH1::SetDefaultSumw2(kTRUE);

  TString cut ="(ScaleFactor)*("+cuts+")";

    TH1F *hBKG = new TH1F("hBKG","hBKG",bins,xmin,xmax);
    tBKG->Project("hBKG",branch,cut);

    TH1F *hSIG = new TH1F("hSIG","hSIG",bins,xmin,xmax);
    tSIG->Project("hSIG",branch,cut);


    double initBKG = 0.;
    double initSIG = 0.;


    if (ovFlow) {
      double errInitBKG = 0.; initBKG = hBKG->IntegralAndError(0,bins+1,errInitBKG);
      double errInitSIG = 0.; initSIG = hSIG->IntegralAndError(0,bins+1,errInitSIG);
    }
    else {
      double errInitBKG = 0.; initBKG = hBKG->IntegralAndError(0,bins,errInitBKG);
      double errInitSIG = 0.; initSIG = hSIG->IntegralAndError(0,bins,errInitSIG);
    }


    const int nBins = bins;
    double eBKG[nBins];
    double eSIG[nBins];
    double yRatio[nBins];

    for (UInt_t i=0; i<(UInt_t)bins; i++) {

      double intBKG = 0.;
      double intSIG = 0.;

      if (ovFlow) {
        double errBKG = 0.; intBKG = hBKG->IntegralAndError(i,bins+1,errBKG);
        double errSIG = 0.; intSIG = hSIG->IntegralAndError(i,bins+1,errSIG);
      }
      else {
        double errBKG = 0.; intBKG = hBKG->IntegralAndError(i,bins,errBKG);
        double errSIG = 0.; intSIG = hSIG->IntegralAndError(i,bins,errSIG);
      }


      // convert to efficiencies
      if (initBKG*initSIG != 0.) {
        eBKG[i] = intBKG/initBKG;
        eSIG[i] = intSIG/initSIG;
        // and ES/sqrt(EB)
        yRatio[i] = eSIG[i]/sqrt(eBKG[i]);
      }
      else {
        eBKG[i] = 0.;
        eSIG[i] = 0.;
        // and ES/sqrt(EB)
        yRatio[i] = 0.;
      }
    }

    hBKG->Delete();
    hSIG->Delete();

    //TGraph *gROC = new TGraph(nBins,eBKG,eSIG);
    TGraph *gROC = new TGraph(nBins,eSIG,yRatio);
    gROC->SetLineWidth(3);
    gROC->SetLineColor(color);
    gROC->GetXaxis()->SetRangeUser(0.,1.);
    gROC->SetTitle(branch);
      gROC->GetXaxis()->SetTitle("#epsilon(SIG)");
      gROC->GetXaxis()->CenterTitle();
      gROC->GetYaxis()->SetTitle("#epsilon(SIG)/sqrt(#epsilon(BKG))");
      gROC->GetYaxis()->CenterTitle();

    return gROC;
}

TH1F *create1Dhisto(TTree *tree,TString intLumi,TString cuts,TString branch,int bins,float xmin,float xmax,bool useLog,int color, int style,bool norm, bool ovBin) {
  TH1::SetDefaultSumw2(kTRUE);

  TString cut ="(ScaleFactor)*("+cuts+")";
  //  TString cut ="("+cuts+")";

  TH1F *hTemp = new TH1F("hTemp","hTemp",bins,xmin,xmax);
  tree->Project("hTemp",branch,cut);

  hTemp->GetXaxis()->SetTitle(branch);

  hTemp->SetMarkerSize(0);
  hTemp->SetLineWidth(3);
  hTemp->SetLineColor(color);
  hTemp->SetLineStyle(style);

  // ad overflow bin
  if (ovBin) {
    double error = 0.; double integral = hTemp->IntegralAndError(bins,bins+1,error);
    hTemp->SetBinContent(bins,integral);
    hTemp->SetBinError(bins,error);
  }

  if (norm) { hTemp->Scale(1./(hTemp->Integral())); }

  // set error
  for (UInt_t i=0; i<(UInt_t)(bins+1); i++) {
    hTemp->SetBinError(i,0);
  }

  return hTemp;
} //~ end of create1Dhisto
