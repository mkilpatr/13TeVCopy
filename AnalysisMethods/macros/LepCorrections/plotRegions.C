#if !defined(__CINT__) || defined(__MAKECINT__)
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <THStack.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TString.h>
#include <TStyle.h>

using namespace std;

//root -l "plotRegions.C( )"
//root -l -b -q "plotRegions.C( )"
//root -b -q ../CMSSW_7_4_11/src/AnalysisMethods/macros/LepCorrections/plotRegions.C

TFile* f_zl = TFile::Open("trees/151008_lepactivity/sm_tree.root"                ,"READONLY");
TFile* f_mu = TFile::Open("trees/151006_newNtuples2015d/dyjetstoll-mu_tree.root" ,"READONLY");
TFile* f_el = TFile::Open("trees/151006_newNtuples2015d/dyjetstoll-el_tree.root" ,"READONLY");


TTree* t_zl = (TTree*)f_zl ->Get("Events");
TTree* t_mu = (TTree*)f_mu ->Get("Probes");
TTree* t_el = (TTree*)f_el ->Get("Probes");

TString lumim = "0.2092*";
TString srwt  = lumim+"weight*puWeight";
TString zwt   = lumim+"weight";
TString trig  = "passdijetmet && passjson && passcscflt && passeebadscflt && "; // passhbhefixflt   passhbheflt
TString lepcr = trig + "met>200 && njets60>=2 && j1pt>75 && j2pt>75 && njets>=5 && nlbjets>=2 && nbjets>=1 && dphij12met>0.4 && dphij3met>0.4";
TString sr    = trig + "met>200 && njets60>=2 && j1pt>75 && j2pt>75 && njets>=5 && nlbjets>=2 && nbjets>=1 && dphij12met>1   && dphij3met>0.5 && mtcsv12met<175";
TString zcr   = "pass";
TString selmu = " && nvetomu>=1";
TString selel = " && nvetolele>=1 && nvetomu==0";

TString savedir = "plots/regions/";


double getError(double a, double b, double c=0) { return sqrt(a*a+b*b+c*c); }
void normalize(TH1F* &h) { h->Scale(1.0/h->Integral(1,h->GetNbinsX())); }

double getMax(double a, double b, double c=0, double d=0, double e=0) {
  double    max = a;
  if(b>max) max = b;
  if(c>max) max = c;
  if(d>max) max = d;
  if(e>max) max = e;
  return max;
} // getMax()

void plotmet(TString srvar, TString lepvar, int nbins, double xmin, double xmax, TString xlabel) {

  TH1F* h_200_mu  = new TH1F("h_200_mu", "h_200_mu", nbins, xmin, xmax); h_200_mu ->SetLineWidth(4); h_200_mu ->SetLineColor(1); // black
  TH1F* h_300_mu  = new TH1F("h_300_mu", "h_300_mu", nbins, xmin, xmax); h_300_mu ->SetLineWidth(4); h_300_mu ->SetLineColor(4); // blue
  TH1F* h_400_mu  = new TH1F("h_400_mu", "h_400_mu", nbins, xmin, xmax); h_400_mu ->SetLineWidth(4); h_400_mu ->SetLineColor(7); // cyan
  TH1F* h_500_mu  = new TH1F("h_500_mu", "h_500_mu", nbins, xmin, xmax); h_500_mu ->SetLineWidth(4); h_500_mu ->SetLineColor(2); // red
  TH1F* h_600_mu  = new TH1F("h_600_mu", "h_600_mu", nbins, xmin, xmax); h_600_mu ->SetLineWidth(4); h_600_mu ->SetLineColor(3); // green
  TH1F* h_200_el  = new TH1F("h_200_el", "h_200_el", nbins, xmin, xmax); h_200_el ->SetLineWidth(4); h_200_el ->SetLineColor(1); // black
  TH1F* h_300_el  = new TH1F("h_300_el", "h_300_el", nbins, xmin, xmax); h_300_el ->SetLineWidth(4); h_300_el ->SetLineColor(4); // blue
  TH1F* h_400_el  = new TH1F("h_400_el", "h_400_el", nbins, xmin, xmax); h_400_el ->SetLineWidth(4); h_400_el ->SetLineColor(7); // cyan
  TH1F* h_500_el  = new TH1F("h_500_el", "h_500_el", nbins, xmin, xmax); h_500_el ->SetLineWidth(4); h_500_el ->SetLineColor(2); // red
  TH1F* h_600_el  = new TH1F("h_600_el", "h_600_el", nbins, xmin, xmax); h_600_el ->SetLineWidth(4); h_600_el ->SetLineColor(3); // green

  t_zl->Project("h_200_mu", srvar, srwt+"*("+sr+selmu+" && met>200 && met<300)" );
  t_zl->Project("h_300_mu", srvar, srwt+"*("+sr+selmu+" && met>300 && met<400)" );
  t_zl->Project("h_400_mu", srvar, srwt+"*("+sr+selmu+" && met>400 && met<500)" );
  t_zl->Project("h_500_mu", srvar, srwt+"*("+sr+selmu+" && met>500 && met<600)" );
  t_zl->Project("h_600_mu", srvar, srwt+"*("+sr+selmu+" && met>600           )" );

  t_zl->Project("h_200_el", srvar, srwt+"*("+sr+selel+" && met>200 && met<300)" );
  t_zl->Project("h_300_el", srvar, srwt+"*("+sr+selel+" && met>300 && met<400)" );
  t_zl->Project("h_400_el", srvar, srwt+"*("+sr+selel+" && met>400 && met<500)" );
  t_zl->Project("h_500_el", srvar, srwt+"*("+sr+selel+" && met>500 && met<600)" );
  t_zl->Project("h_600_el", srvar, srwt+"*("+sr+selel+" && met>600           )" );

  h_200_mu->SetBinContent(nbins, h_200_mu->GetBinContent(nbins)+h_200_mu->GetBinContent(nbins+1) );
  h_300_mu->SetBinContent(nbins, h_300_mu->GetBinContent(nbins)+h_300_mu->GetBinContent(nbins+1) );
  h_400_mu->SetBinContent(nbins, h_400_mu->GetBinContent(nbins)+h_400_mu->GetBinContent(nbins+1) );
  h_500_mu->SetBinContent(nbins, h_500_mu->GetBinContent(nbins)+h_500_mu->GetBinContent(nbins+1) );
  h_600_mu->SetBinContent(nbins, h_600_mu->GetBinContent(nbins)+h_600_mu->GetBinContent(nbins+1) );
  h_200_el->SetBinContent(nbins, h_200_el->GetBinContent(nbins)+h_200_el->GetBinContent(nbins+1) );
  h_300_el->SetBinContent(nbins, h_300_el->GetBinContent(nbins)+h_300_el->GetBinContent(nbins+1) );
  h_400_el->SetBinContent(nbins, h_400_el->GetBinContent(nbins)+h_400_el->GetBinContent(nbins+1) );
  h_500_el->SetBinContent(nbins, h_500_el->GetBinContent(nbins)+h_500_el->GetBinContent(nbins+1) );
  h_600_el->SetBinContent(nbins, h_600_el->GetBinContent(nbins)+h_600_el->GetBinContent(nbins+1) );
  h_200_mu->SetBinError(nbins, getError(h_200_mu->GetBinError(nbins),h_200_mu->GetBinError(nbins+1)) );
  h_300_mu->SetBinError(nbins, getError(h_300_mu->GetBinError(nbins),h_300_mu->GetBinError(nbins+1)) );
  h_400_mu->SetBinError(nbins, getError(h_400_mu->GetBinError(nbins),h_400_mu->GetBinError(nbins+1)) );
  h_500_mu->SetBinError(nbins, getError(h_500_mu->GetBinError(nbins),h_500_mu->GetBinError(nbins+1)) );
  h_600_mu->SetBinError(nbins, getError(h_600_mu->GetBinError(nbins),h_600_mu->GetBinError(nbins+1)) );
  h_200_el->SetBinError(nbins, getError(h_200_el->GetBinError(nbins),h_200_el->GetBinError(nbins+1)) );
  h_300_el->SetBinError(nbins, getError(h_300_el->GetBinError(nbins),h_300_el->GetBinError(nbins+1)) );
  h_400_el->SetBinError(nbins, getError(h_400_el->GetBinError(nbins),h_400_el->GetBinError(nbins+1)) );
  h_500_el->SetBinError(nbins, getError(h_500_el->GetBinError(nbins),h_500_el->GetBinError(nbins+1)) );
  h_600_el->SetBinError(nbins, getError(h_600_el->GetBinError(nbins),h_600_el->GetBinError(nbins+1)) );
  normalize(h_200_mu); double max2mu = h_200_mu->GetMaximum();
  normalize(h_300_mu); double max3mu = h_300_mu->GetMaximum();
  normalize(h_400_mu); double max4mu = h_400_mu->GetMaximum();
  normalize(h_500_mu); double max5mu = h_500_mu->GetMaximum();
  normalize(h_600_mu); double max6mu = h_600_mu->GetMaximum();
  normalize(h_200_el); double max2el = h_200_el->GetMaximum();
  normalize(h_300_el); double max3el = h_300_el->GetMaximum();
  normalize(h_400_el); double max4el = h_400_el->GetMaximum();
  normalize(h_500_el); double max5el = h_500_el->GetMaximum();
  normalize(h_600_el); double max6el = h_600_el->GetMaximum();

  THStack * stack_mu  = new THStack("stack_mu","muons");
  stack_mu->Add(h_200_mu);
  stack_mu->Add(h_300_mu);
  stack_mu->Add(h_400_mu);
  stack_mu->Add(h_500_mu);
  stack_mu->Add(h_600_mu);
  THStack * stack_el  = new THStack("stack_el","electrons");
  stack_el->Add(h_200_el);
  stack_el->Add(h_300_el);
  stack_el->Add(h_400_el);
  stack_el->Add(h_500_el);
  stack_el->Add(h_600_el);

  double max_mu = 1.2*getMax(max2mu,max3mu,max4mu,max5mu,max6mu);
  double max_el = 1.2*getMax(max2el,max3el,max4el,max5el,max6el);
  stack_mu->SetMaximum(max_mu);
  stack_el->SetMaximum(max_el);

  TLegend * legend = new TLegend(.75,.7,.9,.9);
  legend->AddEntry(h_200_mu ,"met 200-300");
  legend->AddEntry(h_300_mu ,"met 300-400");
  legend->AddEntry(h_400_mu ,"met 400-500");
  legend->AddEntry(h_500_mu ,"met 500-600");
  legend->AddEntry(h_600_mu ,"met 600+"   );

  TCanvas * c_mu = new TCanvas( "c_mu","c_mu", 1000, 1000 ); c_mu->cd();
  stack_mu->Draw("nostackhist"); stack_mu->GetXaxis()->SetTitle(xlabel);
  legend->Draw();
  c_mu->SaveAs(savedir+"met_mu_"+lepvar+".png");
  TCanvas * c_el = new TCanvas( "c_el","c_el", 1000, 1000 ); c_el->cd();
  stack_el->Draw("nostackhist"); stack_el->GetXaxis()->SetTitle(xlabel);
  legend->Draw();
  c_el->SaveAs(savedir+"met_el_"+lepvar+".png");

  delete h_200_mu  ;
  delete h_300_mu  ;
  delete h_400_mu  ;
  delete h_500_mu  ;
  delete h_600_mu  ;
  delete h_200_el  ;
  delete h_300_el  ;
  delete h_400_el  ;
  delete h_500_el  ;
  delete h_600_el  ;
  delete stack_mu  ;
  delete stack_el  ;
  delete c_mu      ;
  delete c_el      ;

  return;
} // plot3()

void plot3(TString srvar, TString lepvar, int nbins, double xmin, double xmax, TString xlabel, bool logy=false) {
  TString stag = "";
  if(logy) stag = "_logy";

  TH1F* h_sr_mu  = new TH1F("h_sr_mu", "h_sr_mu", nbins, xmin, xmax); h_sr_mu ->SetLineWidth(4); h_sr_mu ->SetLineColor(4); // blue
  TH1F* h_cr_mu  = new TH1F("h_cr_mu", "h_cr_mu", nbins, xmin, xmax); h_cr_mu ->SetLineWidth(4); h_cr_mu ->SetLineColor(2); // red
  TH1F* h_zr_mu  = new TH1F("h_zr_mu", "h_zr_mu", nbins, xmin, xmax); h_zr_mu ->SetLineWidth(4); h_zr_mu ->SetLineColor(1); // black
  TH1F* h_sr_el  = new TH1F("h_sr_el", "h_sr_el", nbins, xmin, xmax); h_sr_el ->SetLineWidth(4); h_sr_el ->SetLineColor(4); // blue
  TH1F* h_cr_el  = new TH1F("h_cr_el", "h_cr_el", nbins, xmin, xmax); h_cr_el ->SetLineWidth(4); h_cr_el ->SetLineColor(2); // red
  TH1F* h_zr_el  = new TH1F("h_zr_el", "h_zr_el", nbins, xmin, xmax); h_zr_el ->SetLineWidth(4); h_zr_el ->SetLineColor(1); // black

  t_zl->Project("h_sr_mu", srvar , srwt+"*("+sr   +selmu+")" );
  t_zl->Project("h_cr_mu", srvar , srwt+"*("+lepcr+selmu+")" );
  t_mu->Project("h_zr_mu", lepvar,  zwt+"*("+zcr        +")" );
  t_zl->Project("h_sr_el", srvar , srwt+"*("+sr   +selel+")" );
  t_zl->Project("h_cr_el", srvar , srwt+"*("+lepcr+selel+")" );
  t_el->Project("h_zr_el", lepvar,  zwt+"*("+zcr        +")" );

  h_sr_mu->SetBinContent(nbins, h_sr_mu->GetBinContent(nbins)+h_sr_mu->GetBinContent(nbins+1) );
  h_cr_mu->SetBinContent(nbins, h_cr_mu->GetBinContent(nbins)+h_cr_mu->GetBinContent(nbins+1) );
  h_zr_mu->SetBinContent(nbins, h_zr_mu->GetBinContent(nbins)+h_zr_mu->GetBinContent(nbins+1) );
  h_sr_el->SetBinContent(nbins, h_sr_el->GetBinContent(nbins)+h_sr_el->GetBinContent(nbins+1) );
  h_cr_el->SetBinContent(nbins, h_cr_el->GetBinContent(nbins)+h_cr_el->GetBinContent(nbins+1) );
  h_zr_el->SetBinContent(nbins, h_zr_el->GetBinContent(nbins)+h_zr_el->GetBinContent(nbins+1) );
  h_sr_mu->SetBinError(nbins, getError(h_sr_mu->GetBinError(nbins),h_sr_mu->GetBinError(nbins+1)) );
  h_cr_mu->SetBinError(nbins, getError(h_cr_mu->GetBinError(nbins),h_cr_mu->GetBinError(nbins+1)) );
  h_zr_mu->SetBinError(nbins, getError(h_zr_mu->GetBinError(nbins),h_zr_mu->GetBinError(nbins+1)) );
  h_sr_el->SetBinError(nbins, getError(h_sr_el->GetBinError(nbins),h_sr_el->GetBinError(nbins+1)) );
  h_cr_el->SetBinError(nbins, getError(h_cr_el->GetBinError(nbins),h_cr_el->GetBinError(nbins+1)) );
  h_zr_el->SetBinError(nbins, getError(h_zr_el->GetBinError(nbins),h_zr_el->GetBinError(nbins+1)) );
  normalize(h_sr_mu); double max1mu = h_sr_mu->GetMaximum();
  normalize(h_cr_mu); double max2mu = h_cr_mu->GetMaximum();
  normalize(h_zr_mu); double max3mu = h_zr_mu->GetMaximum();
  normalize(h_sr_el); double max1el = h_sr_el->GetMaximum();
  normalize(h_cr_el); double max2el = h_cr_el->GetMaximum();
  normalize(h_zr_el); double max3el = h_zr_el->GetMaximum();

  THStack * stack_mu  = new THStack("stack_mu","muons");
  stack_mu->Add(h_sr_mu);
  stack_mu->Add(h_cr_mu);
  stack_mu->Add(h_zr_mu);
  THStack * stack_el  = new THStack("stack_el","electrons");
  stack_el->Add(h_sr_el);
  stack_el->Add(h_cr_el);
  stack_el->Add(h_zr_el);

  double max_mu = 1.2*getMax(max1mu,max2mu,max3mu);
  double max_el = 1.2*getMax(max1el,max2el,max3el);
  stack_mu->SetMaximum(max_mu);
  stack_el->SetMaximum(max_el);

  TLegend * legend = new TLegend(.75,.7,.9,.9);
  legend->AddEntry(h_sr_mu ,"search region");
  legend->AddEntry(h_cr_mu ,"contorl region");
  legend->AddEntry(h_zr_mu ,"tag and probe");

  TCanvas * c_mu = new TCanvas( "c_mu","c_mu", 1000, 1000 ); c_mu->cd();
  stack_mu->Draw("nostackhist"); stack_mu->GetXaxis()->SetTitle(xlabel);
  if(logy) c_mu->SetLogy(true);
  legend->Draw();
  c_mu->SaveAs(savedir+"mu_"+lepvar+stag+".png");
  TCanvas * c_el = new TCanvas( "c_el","c_el", 1000, 1000 ); c_el->cd();
  stack_el->Draw("nostackhist"); stack_el->GetXaxis()->SetTitle(xlabel);
  if(logy) c_el->SetLogy(true);
  legend->Draw();
  c_el->SaveAs(savedir+"el_"+lepvar+stag+".png");

  delete h_sr_mu   ;
  delete h_cr_mu   ;
  delete h_zr_mu   ;
  delete h_sr_el   ;
  delete h_cr_el   ;
  delete h_zr_el   ;
  delete stack_mu  ;
  delete stack_el  ;
  delete c_mu      ;
  delete c_el      ;

  return;
} // plot3()

#endif

void plotRegions() {

  gStyle->SetOptStat(0);
  TH1::SetDefaultSumw2();

  //plotmet("leptonpt"      ,"pt"     , 10, 0,  200   , "lepton pt");
  //plotmet("abs(leptoneta)","eta"    , 15, 0,    3.14, "lepton eta");
  //plotmet("htalonglep"    ,"htalong", 10, 0, 1000   , "lepton htalong");
  plotmet("annulus"       ,"annulus", 10, 0,    10 , "lepton annulus");
  return;

  plot3("leptonpt"      ,"pt"     , 20, 0,  200   , "lepton pt" );
  plot3("abs(leptoneta)","eta"    , 31, 0,    3.14, "lepton eta");
  plot3("htalonglep"    ,"htalong", 20, 0, 1000   , "lepton htalong");
  plot3("htalonglep"    ,"htalong", 20, 0, 1000   , "lepton htalong", true);
  plot3("annulus"       ,"annulus", 20, 0,    10 , "lepton annulus");

} // plotRegions()



