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

//root -l "run8TeVbkgestPythia.C( )"
//root -l -b -q "run8TeVbkgestPythia.C( )"
//root -b -q ../CMSSW_7_4_7/src/AnalysisMethods/macros/1LepSearchRegions/run8TeVbkgestPythia.C

TFile* f_ttbar  = TFile::Open("trees/150813_pdgid/ttbar_tree.root"    ,"READONLY");
TFile* f_wjets  = TFile::Open("trees/150813_pdgid/wjets_tree.root"    ,"READONLY");
//TFile* f_qcd    = TFile::Open("trees/150806_v0/qcd_tree.root"      ,"READONLY");
// data=data
//TFile* f_eldata = TFile::Open("trees/150813_pdgid/singlee_tree.root"  ,"READONLY");
//TFile* f_mudata = TFile::Open("trees/150813_pdgid/singlemu_tree.root" ,"READONLY");
//TFile* f_lpdata = TFile::Open("trees/150813_pdgid/singllep_tree.root" ,"READONLY");
// data=ttbar_pythia
TFile* f_eldata = TFile::Open("trees/150813_pdgid/pythiaPlusWjet_tree.root" ,"READONLY");
TFile* f_mudata = TFile::Open("trees/150813_pdgid/pythiaPlusWjet_tree.root" ,"READONLY");
TFile* f_lpdata = TFile::Open("trees/150813_pdgid/pythiaPlusWjet_tree.root" ,"READONLY");


TTree* t_eldata = (TTree*)f_eldata ->Get("Events");
TTree* t_mudata = (TTree*)f_mudata ->Get("Events");
TTree* t_lpdata = (TTree*)f_lpdata ->Get("Events");
TTree* t_ttbar  = (TTree*)f_ttbar  ->Get("Events");
TTree* t_wjets  = (TTree*)f_wjets  ->Get("Events");

const int nBins = 4;
//double edges[nBins+1] = {50, 75, 100, 150, 250}; // data
double edges[nBins+1] = {250, 300, 350, 400, 500}; // pythia
TH1F* SF_unit = new TH1F("SF_unit", "SF_unit", nBins, edges );

TString isDilep   = " && ngenlep>=2";
TString ntDilep   = " && ngenlep<=1";
TString baseCuts1 = "( nbjets==0 && njets>=4 && nvetotau==0 && nvetolep==1 && nselelep==1 && dphij12met>0.4 && mt2w<200"; // && mt2w<200
TString baseCuts4 = "( nbjets>=1 && njets>=4 && nvetotau==0 && nvetolep>=2 && nselelep>=1 && dphij12met>0.4 && mt2w<200 && lep2pt>20 && abs(lep2eta)<2.1 ";
TString preVeto5  = "( nbjets>=1 && njets>=4                               && nselelep==1 && dphij12met>0.4 && mt2w<200";
TString pstVeto5  = "( nbjets>=1 && njets>=4 && (nvetotau+nvetolep)>=2     && nselelep>=1 && dphij12met>0.4 && mt2w<200";
TString incBCuts  = "(              njets>=4 && nvetotau==0 && nvetolep==1 && nselelep==1 && dphij12met>0.4 && mt2w<200";
TString srCuts    = "( nbjets>=1 && njets>=4 && nvetotau==0 && nvetolep==1 && nselelep==1 && dphij12met>0.4 && mt2w<200";

TString dilepCut  = " && abs(dilepinvmass-91)>15 && opsignlep"; // && dilepinvmass>15
TString lowMT     = " && mt>=50 && mt<80";
TString hghMT     = " && mt>=150";
// data=data
//TString mcweight  = "(.040*weight)*";
//TString dtweight  = "";
//TString elTrg     = " && passtrige && !passtrigmu )";
//TString muTrg     = " && passtrigmu )";
//TString lpTrg     = " && ((sampType==0 && (passtrige || passtrigmu)) || (sampType==2 && passtrigmu) || (sampType==1 && passtrige && !passtrigmu)) )";
// data=ttbar_pythia
TString mcweight  = "(4*weight)*";
TString dtweight  = mcweight;
TString elTrg     = " && lep1pdgid==11)";
TString muTrg     = " && lep1pdgid==13)";
TString lpTrg     = " )";

double getError(double a, double b, double c=0) {
  return sqrt(a*a+b*b+c*c);
} // getError()

void plotMT(TString cuts, TString saveTag, TString saveSubdir,
            TH1F* &SF_el_wjets, TH1F* &SF_mu_wjets, TH1F* &SF_lp_wjets,
            TH1F* &SF_el_ttbar, TH1F* &SF_mu_ttbar, TH1F* &SF_lp_ttbar,
            TH1F* &SF_el_dilep, TH1F* &SF_mu_dilep, TH1F* &SF_lp_dilep
            ) {

  vector<TString> metBins(nBins+1,"");
  vector<TString> metName(nBins+1,"");
  vector<TString> metLabel(nBins+1,"");

  // data
  //metBins[1] = " && met>= 50 && met< 75"; metName[1] = "met050"; metLabel[1] = ", MET (50-75)";
  //metBins[2] = " && met>= 75 && met<100"; metName[2] = "met075"; metLabel[2] = ", MET (75-100)";
  //metBins[3] = " && met>=100 && met<150"; metName[3] = "met100"; metLabel[3] = ", MET (100-150)";
  //metBins[4] = " && met>=150";            metName[4] = "met150"; metLabel[4] = ", MET 150+";
  // pythia
  metBins[1] = " && met>=250 && met<300"; metName[1] = "met050"; metLabel[1] = ", MT2W<200, MET (250-300)";
  metBins[2] = " && met>=300 && met<350"; metName[2] = "met075"; metLabel[2] = ", MT2W<200, MET (300-350)";
  metBins[3] = " && met>=350 && met<400"; metName[3] = "met100"; metLabel[3] = ", MT2W<200, MET (350-400)";
  metBins[4] = " && met>=400"           ; metName[4] = "met150"; metLabel[4] = ", MT2W<200, MET 400+"     ;
  //metBins[4] = " && met>=400 && met<500"; metName[4] = "met150"; metLabel[4] = ", MT2W>200, MET (400-500)";
  //metBins[5] = " && met>=500";            metName[5] = "met500"; metLabel[5] = ", MT2W>200, MET 500+";

  //metBins[1] = " && met>= 50"; metName[1] = "met050inc"; metLabel[1] = ", MET 50+";
  TString savePrefix = "plots/"+saveSubdir+"/inclusiveMT_"+saveTag;

  for(int i=1; i<=nBins; ++i) {
    // TODO: add ratio?

    const int mtBins = 8; // 13;
    double mtEdges[mtBins+1] = { 0, 20, 35, 50, 80, 100, 150, 300, 400};

    TH1F* el_data  = new TH1F("el_data",  "el_data",  mtBins, mtEdges); el_data ->SetLineWidth(3); el_data ->SetLineColor(1); // black
    TH1F* mu_data  = new TH1F("mu_data",  "mu_data",  mtBins, mtEdges); mu_data ->SetLineWidth(3); mu_data ->SetLineColor(1); // black
    TH1F* lp_data  = new TH1F("lp_data",  "lp_data",  mtBins, mtEdges); lp_data ->SetLineWidth(3); lp_data ->SetLineColor(1); // black
    TH1F* el_wjets = new TH1F("el_wjets", "el_wjets", mtBins, mtEdges); el_wjets->SetLineWidth(1); el_wjets->SetLineColor(1);
    TH1F* mu_wjets = new TH1F("mu_wjets", "mu_wjets", mtBins, mtEdges); mu_wjets->SetLineWidth(1); mu_wjets->SetLineColor(1);
    TH1F* lp_wjets = new TH1F("lp_wjets", "lp_wjets", mtBins, mtEdges); lp_wjets->SetLineWidth(1); lp_wjets->SetLineColor(1);
    TH1F* el_ttbar = new TH1F("el_ttbar", "el_ttbar", mtBins, mtEdges); el_ttbar->SetLineWidth(1); el_ttbar->SetLineColor(1);
    TH1F* mu_ttbar = new TH1F("mu_ttbar", "mu_ttbar", mtBins, mtEdges); mu_ttbar->SetLineWidth(1); mu_ttbar->SetLineColor(1);
    TH1F* lp_ttbar = new TH1F("lp_ttbar", "lp_ttbar", mtBins, mtEdges); lp_ttbar->SetLineWidth(1); lp_ttbar->SetLineColor(1);
    TH1F* el_dilep = new TH1F("el_dilep", "el_dilep", mtBins, mtEdges); el_ttbar->SetLineWidth(1); el_ttbar->SetLineColor(1);
    TH1F* mu_dilep = new TH1F("mu_dilep", "mu_dilep", mtBins, mtEdges); mu_ttbar->SetLineWidth(1); mu_ttbar->SetLineColor(1);
    TH1F* lp_dilep = new TH1F("lp_dilep", "lp_dilep", mtBins, mtEdges); lp_ttbar->SetLineWidth(1); lp_ttbar->SetLineColor(1);

    el_wjets->SetFillColor(4); // blue
    mu_wjets->SetFillColor(4); // blue
    lp_wjets->SetFillColor(4); // blue
    el_ttbar->SetFillColor(2); // red
    mu_ttbar->SetFillColor(2); // red
    lp_ttbar->SetFillColor(2); // red
    el_dilep->SetFillColor(5); // yellow
    mu_dilep->SetFillColor(5); // yellow
    lp_dilep->SetFillColor(5); // yellow

    t_eldata->Project("el_data" ,"mt",dtweight+cuts+metBins[i]        +elTrg);
    t_mudata->Project("mu_data" ,"mt",dtweight+cuts+metBins[i]        +muTrg);
    t_lpdata->Project("lp_data" ,"mt",dtweight+cuts+metBins[i]        +lpTrg);
    t_wjets ->Project("el_wjets","mt",mcweight+cuts+metBins[i]        +elTrg);
    t_wjets ->Project("mu_wjets","mt",mcweight+cuts+metBins[i]        +muTrg);
    t_wjets ->Project("lp_wjets","mt",mcweight+cuts+metBins[i]        +lpTrg);
    t_ttbar ->Project("el_ttbar","mt",mcweight+cuts+metBins[i]+ntDilep+elTrg);
    t_ttbar ->Project("mu_ttbar","mt",mcweight+cuts+metBins[i]+ntDilep+muTrg);
    t_ttbar ->Project("lp_ttbar","mt",mcweight+cuts+metBins[i]+ntDilep+lpTrg);
    t_ttbar ->Project("el_dilep","mt",mcweight+cuts+metBins[i]+isDilep+elTrg);
    t_ttbar ->Project("mu_dilep","mt",mcweight+cuts+metBins[i]+isDilep+muTrg);
    t_ttbar ->Project("lp_dilep","mt",mcweight+cuts+metBins[i]+isDilep+lpTrg);

    el_data ->SetBinContent(mtBins, el_data ->GetBinContent(mtBins)+el_data ->GetBinContent(mtBins+1) );
    mu_data ->SetBinContent(mtBins, mu_data ->GetBinContent(mtBins)+mu_data ->GetBinContent(mtBins+1) );
    lp_data ->SetBinContent(mtBins, lp_data ->GetBinContent(mtBins)+lp_data ->GetBinContent(mtBins+1) );
    el_wjets->SetBinContent(mtBins, el_wjets->GetBinContent(mtBins)+el_wjets->GetBinContent(mtBins+1) );
    mu_wjets->SetBinContent(mtBins, mu_wjets->GetBinContent(mtBins)+mu_wjets->GetBinContent(mtBins+1) );
    lp_wjets->SetBinContent(mtBins, lp_wjets->GetBinContent(mtBins)+lp_wjets->GetBinContent(mtBins+1) );
    el_ttbar->SetBinContent(mtBins, el_ttbar->GetBinContent(mtBins)+el_ttbar->GetBinContent(mtBins+1) );
    mu_ttbar->SetBinContent(mtBins, mu_ttbar->GetBinContent(mtBins)+mu_ttbar->GetBinContent(mtBins+1) );
    lp_ttbar->SetBinContent(mtBins, lp_ttbar->GetBinContent(mtBins)+lp_ttbar->GetBinContent(mtBins+1) );
    el_dilep->SetBinContent(mtBins, el_dilep->GetBinContent(mtBins)+el_dilep->GetBinContent(mtBins+1) );
    mu_dilep->SetBinContent(mtBins, mu_dilep->GetBinContent(mtBins)+mu_dilep->GetBinContent(mtBins+1) );
    lp_dilep->SetBinContent(mtBins, lp_dilep->GetBinContent(mtBins)+lp_dilep->GetBinContent(mtBins+1) );
    el_data ->SetBinError(mtBins, getError(el_data ->GetBinError(mtBins),el_data ->GetBinError(mtBins+1)) );
    mu_data ->SetBinError(mtBins, getError(mu_data ->GetBinError(mtBins),mu_data ->GetBinError(mtBins+1)) );
    lp_data ->SetBinError(mtBins, getError(lp_data ->GetBinError(mtBins),lp_data ->GetBinError(mtBins+1)) );
    el_wjets->SetBinError(mtBins, getError(el_wjets->GetBinError(mtBins),el_wjets->GetBinError(mtBins+1)) );
    mu_wjets->SetBinError(mtBins, getError(mu_wjets->GetBinError(mtBins),mu_wjets->GetBinError(mtBins+1)) );
    lp_wjets->SetBinError(mtBins, getError(lp_wjets->GetBinError(mtBins),lp_wjets->GetBinError(mtBins+1)) );
    el_ttbar->SetBinError(mtBins, getError(el_ttbar->GetBinError(mtBins),el_ttbar->GetBinError(mtBins+1)) );
    mu_ttbar->SetBinError(mtBins, getError(mu_ttbar->GetBinError(mtBins),mu_ttbar->GetBinError(mtBins+1)) );
    lp_ttbar->SetBinError(mtBins, getError(lp_ttbar->GetBinError(mtBins),lp_ttbar->GetBinError(mtBins+1)) );
    el_dilep->SetBinError(mtBins, getError(el_dilep->GetBinError(mtBins),el_dilep->GetBinError(mtBins+1)) );
    mu_dilep->SetBinError(mtBins, getError(mu_dilep->GetBinError(mtBins),mu_dilep->GetBinError(mtBins+1)) );
    lp_dilep->SetBinError(mtBins, getError(lp_dilep->GetBinError(mtBins),lp_dilep->GetBinError(mtBins+1)) );

    el_wjets->Scale(SF_el_wjets->GetBinContent(i));
    mu_wjets->Scale(SF_mu_wjets->GetBinContent(i));
    lp_wjets->Scale(SF_lp_wjets->GetBinContent(i));
    el_ttbar->Scale(SF_el_ttbar->GetBinContent(i));
    mu_ttbar->Scale(SF_mu_ttbar->GetBinContent(i));
    lp_ttbar->Scale(SF_lp_ttbar->GetBinContent(i));
    el_dilep->Scale(SF_el_dilep->GetBinContent(i));
    mu_dilep->Scale(SF_mu_dilep->GetBinContent(i));
    lp_dilep->Scale(SF_lp_dilep->GetBinContent(i));

    THStack * stack_el  = new THStack("stack_el","electrons"+metLabel[i]);
    stack_el->Add(el_wjets);
    stack_el->Add(el_ttbar);
    stack_el->Add(el_dilep);
    THStack * stack_mu  = new THStack("stack_mu","muons"+metLabel[i]);
    stack_mu->Add(mu_wjets);
    stack_mu->Add(mu_ttbar);
    stack_mu->Add(mu_dilep);
    THStack * stack_lp  = new THStack("stack_lp","both"+metLabel[i]);
    stack_lp->Add(lp_wjets);
    stack_lp->Add(lp_ttbar);
    stack_lp->Add(lp_dilep);

    double max_el = 1.2*( stack_el->GetMaximum() > el_data->GetMaximum() ? stack_el->GetMaximum() : el_data->GetMaximum() );
    double max_mu = 1.2*( stack_mu->GetMaximum() > mu_data->GetMaximum() ? stack_mu->GetMaximum() : mu_data->GetMaximum() );
    double max_lp = 1.2*( stack_lp->GetMaximum() > lp_data->GetMaximum() ? stack_lp->GetMaximum() : lp_data->GetMaximum() );
    stack_el->SetMaximum(max_el);
    stack_mu->SetMaximum(max_mu);
    stack_lp->SetMaximum(max_lp);

    TLegend * legend = new TLegend(.75,.7,.9,.9);
    legend->AddEntry(el_wjets ,"wjets");
    legend->AddEntry(el_ttbar ,"ttbar 1L");
    legend->AddEntry(el_dilep ,"ttbar dilep");
    legend->AddEntry(el_data  ,"data" );

    TCanvas * c_el = new TCanvas( "c_el","c_el", 1000, 1000 ); c_el->cd();
    stack_el->Draw("hist"); stack_el->GetXaxis()->SetTitle("MT");
    el_data->Draw("samee");
    legend->Draw();
    c_el->SaveAs(savePrefix+"_el_"+metName[i]+".png");
    TCanvas * c_mu = new TCanvas( "c_mu","c_mu", 1000, 1000 ); c_mu->cd();
    stack_mu->Draw("hist"); stack_mu->GetXaxis()->SetTitle("MT");
    mu_data->Draw("samee");
    legend->Draw();
    c_mu->SaveAs(savePrefix+"_mu_"+metName[i]+".png");
    TCanvas * c_lp = new TCanvas( "c_lp","c_lp", 1000, 1000 ); c_lp->cd();
    stack_lp->Draw("hist"); stack_lp->GetXaxis()->SetTitle("MT");
    lp_data->Draw("samee");
    legend->Draw();
    c_lp->SaveAs(savePrefix+"_lp_"+metName[i]+".png");

    delete el_data;
    delete mu_data;
    delete lp_data;
    delete el_wjets;
    delete mu_wjets;
    delete lp_wjets;
    delete el_ttbar;
    delete mu_ttbar;
    delete lp_ttbar;
    delete el_dilep;
    delete mu_dilep;
    delete lp_dilep;
    delete c_el;
    delete c_mu;
    delete c_lp;
  } // nBins
  return;
} // plotMT()

void getSRnumbers(TString cuts, TString saveTag,
                  TH1F* &SF_el_wjets, TH1F* &SF_mu_wjets, TH1F* &SF_lp_wjets,
                  TH1F* &SF_el_ttbar, TH1F* &SF_mu_ttbar, TH1F* &SF_lp_ttbar,
                  TH1F* &SF_el_dilep, TH1F* &SF_mu_dilep, TH1F* &SF_lp_dilep,
                  bool getOOtB=false
                  ) {

  vector<TString> metBins(nBins+1,"");
  vector<TString> metName(nBins+1,"");
  vector<TString> metLabel(nBins+1,"");

  // data
  //metBins[1] = " && met>= 50 && met< 75"; metName[1] = "met050"; metLabel[1] = ", MET (50-75)";
  //metBins[2] = " && met>= 75 && met<100"; metName[2] = "met075"; metLabel[2] = ", MET (75-100)";
  //metBins[3] = " && met>=100 && met<150"; metName[3] = "met100"; metLabel[3] = ", MET (100-150)";
  //metBins[4] = " && met>=150";            metName[4] = "met150"; metLabel[4] = ", MET 150+";
  // pythia
  metBins[1] = " && met>=250 && met<300"; metName[1] = "met050"; metLabel[1] = "MT2W$<$200, MET (250-300)";
  metBins[2] = " && met>=300 && met<350"; metName[2] = "met075"; metLabel[2] = "MT2W$<$200, MET (300-350)";
  metBins[3] = " && met>=350 && met<400"; metName[3] = "met100"; metLabel[3] = "MT2W$<$200, MET (350-400)";
  metBins[4] = " && met>=400"           ; metName[4] = "met150"; metLabel[4] = "MT2W$<$200, MET 400+"     ;
  //metBins[4] = " && met>=400 && met<500"; metName[4] = "met150"; metLabel[4] = "MT2W$>$200, MET (400-500)";
  //metBins[5] = " && met>=500";            metName[5] = "met500"; metLabel[5] = "MT2W$>$200, MET 500+";

  //metBins[1] = " && met>= 50"; metName[1] = "met050inc"; metLabel[1] = ", MET 50+";

  for(int i=1; i<=nBins; ++i) {
    // TODO: add ratio?

    const int mtBins = 8;
    double mtEdges[mtBins+1] = { 0, 20, 35, 50, 80, 100, 150, 300, 400};

    TH1F* el_data  = new TH1F("el_data",  "el_data",  1, 0, 10000000000 );
    TH1F* mu_data  = new TH1F("mu_data",  "mu_data",  1, 0, 10000000000 );
    TH1F* lp_data  = new TH1F("lp_data",  "lp_data",  1, 0, 10000000000 );
    TH1F* el_wjets = new TH1F("el_wjets", "el_wjets", 1, 0, 10000000000 );
    TH1F* mu_wjets = new TH1F("mu_wjets", "mu_wjets", 1, 0, 10000000000 );
    TH1F* lp_wjets = new TH1F("lp_wjets", "lp_wjets", 1, 0, 10000000000 );
    TH1F* el_ttbar = new TH1F("el_ttbar", "el_ttbar", 1, 0, 10000000000 );
    TH1F* mu_ttbar = new TH1F("mu_ttbar", "mu_ttbar", 1, 0, 10000000000 );
    TH1F* lp_ttbar = new TH1F("lp_ttbar", "lp_ttbar", 1, 0, 10000000000 );
    TH1F* el_dilep = new TH1F("el_dilep", "el_dilep", 1, 0, 10000000000 );
    TH1F* mu_dilep = new TH1F("mu_dilep", "mu_dilep", 1, 0, 10000000000 );
    TH1F* lp_dilep = new TH1F("lp_dilep", "lp_dilep", 1, 0, 10000000000 );

    if (getOOtB) {
      t_eldata->Project("el_data" ,"met",dtweight+cuts+metBins[i]        +hghMT+elTrg);
      t_mudata->Project("mu_data" ,"met",dtweight+cuts+metBins[i]        +hghMT+muTrg);
      t_lpdata->Project("lp_data" ,"met",dtweight+cuts+metBins[i]        +hghMT+lpTrg);
      t_wjets ->Project("el_wjets","met",mcweight+cuts+metBins[i]        +hghMT+elTrg);
      t_wjets ->Project("mu_wjets","met",mcweight+cuts+metBins[i]        +hghMT+muTrg);
      t_wjets ->Project("lp_wjets","met",mcweight+cuts+metBins[i]        +hghMT+lpTrg);
      t_ttbar ->Project("el_ttbar","met",mcweight+cuts+metBins[i]+ntDilep+hghMT+elTrg);
      t_ttbar ->Project("mu_ttbar","met",mcweight+cuts+metBins[i]+ntDilep+hghMT+muTrg);
      t_ttbar ->Project("lp_ttbar","met",mcweight+cuts+metBins[i]+ntDilep+hghMT+lpTrg);
      t_ttbar ->Project("el_dilep","met",mcweight+cuts+metBins[i]+isDilep+hghMT+elTrg);
      t_ttbar ->Project("mu_dilep","met",mcweight+cuts+metBins[i]+isDilep+hghMT+muTrg);
      t_ttbar ->Project("lp_dilep","met",mcweight+cuts+metBins[i]+isDilep+hghMT+lpTrg);
    }else {
      t_eldata->Project("el_data" ,"met",dtweight+cuts+metBins[i]        +hghMT+elTrg);
      t_mudata->Project("mu_data" ,"met",dtweight+cuts+metBins[i]        +hghMT+muTrg);
      t_lpdata->Project("lp_data" ,"met",dtweight+cuts+metBins[i]        +hghMT+lpTrg);
      t_wjets ->Project("el_wjets","met",mcweight+cuts+metBins[i]        +lowMT+elTrg);
      t_wjets ->Project("mu_wjets","met",mcweight+cuts+metBins[i]        +lowMT+muTrg);
      t_wjets ->Project("lp_wjets","met",mcweight+cuts+metBins[i]        +lowMT+lpTrg);
      t_ttbar ->Project("el_ttbar","met",mcweight+cuts+metBins[i]+ntDilep+lowMT+elTrg);
      t_ttbar ->Project("mu_ttbar","met",mcweight+cuts+metBins[i]+ntDilep+lowMT+muTrg);
      t_ttbar ->Project("lp_ttbar","met",mcweight+cuts+metBins[i]+ntDilep+lowMT+lpTrg);
      t_ttbar ->Project("el_dilep","met",mcweight+cuts+metBins[i]+isDilep+hghMT+elTrg);
      t_ttbar ->Project("mu_dilep","met",mcweight+cuts+metBins[i]+isDilep+hghMT+muTrg);
      t_ttbar ->Project("lp_dilep","met",mcweight+cuts+metBins[i]+isDilep+hghMT+lpTrg);
    } // getOOtB

    double n_el_ttbar = el_ttbar->GetBinContent(1) * SF_el_ttbar->GetBinContent(i);
    double n_el_wjets = el_wjets->GetBinContent(1) * SF_el_wjets->GetBinContent(i);
    double n_el_dilep = el_dilep->GetBinContent(1) * SF_el_dilep->GetBinContent(i);
    double n_mu_wjets = mu_wjets->GetBinContent(1) * SF_mu_wjets->GetBinContent(i);
    double n_mu_ttbar = mu_ttbar->GetBinContent(1) * SF_mu_ttbar->GetBinContent(i);
    double n_mu_dilep = mu_dilep->GetBinContent(1) * SF_mu_dilep->GetBinContent(i);
    double n_lp_wjets = lp_wjets->GetBinContent(1) * SF_lp_wjets->GetBinContent(i);
    double n_lp_ttbar = lp_ttbar->GetBinContent(1) * SF_lp_ttbar->GetBinContent(i);
    double n_lp_dilep = lp_dilep->GetBinContent(1) * SF_lp_dilep->GetBinContent(i);
    double n_el_all   = n_el_wjets + n_el_ttbar + n_el_dilep;
    double n_mu_all   = n_mu_wjets + n_mu_ttbar + n_mu_dilep;
    double n_lp_all   = n_lp_wjets + n_lp_ttbar + n_lp_dilep;
    double n_el_data  = el_data ->GetBinContent(1);
    double n_mu_data  = mu_data ->GetBinContent(1);
    double n_lp_data  = lp_data ->GetBinContent(1);

    double n_error_el_wjets = 0;
    double n_error_el_ttbar = 0;
    double n_error_el_dilep = 0;
    double n_error_mu_wjets = 0;
    double n_error_mu_ttbar = 0;
    double n_error_mu_dilep = 0;
    double n_error_lp_wjets = 0;
    double n_error_lp_ttbar = 0;
    double n_error_lp_dilep = 0;
    double n_error_el_all   = 0;
    double n_error_mu_all   = 0;
    double n_error_lp_all   = 0;
    double n_error_el_data  = 0;
    double n_error_mu_data  = 0;
    double n_error_lp_data  = 0;

    if(getOOtB) {
      n_error_el_wjets = el_wjets->GetBinError(1);
      n_error_el_ttbar = el_ttbar->GetBinError(1);
      n_error_el_dilep = el_dilep->GetBinError(1);
      n_error_mu_wjets = mu_wjets->GetBinError(1);
      n_error_mu_ttbar = mu_ttbar->GetBinError(1);
      n_error_mu_dilep = mu_dilep->GetBinError(1);
      n_error_lp_wjets = lp_wjets->GetBinError(1);
      n_error_lp_ttbar = lp_ttbar->GetBinError(1);
      n_error_lp_dilep = lp_dilep->GetBinError(1);
    }else{
      n_error_el_wjets = n_el_wjets * getError(el_wjets->GetBinError(1)/el_wjets->GetBinContent(1),SF_el_wjets->GetBinError(i)/SF_el_wjets->GetBinContent(i));
      n_error_el_ttbar = n_el_ttbar * getError(el_ttbar->GetBinError(1)/el_ttbar->GetBinContent(1),SF_el_ttbar->GetBinError(i)/SF_el_ttbar->GetBinContent(i));
      n_error_el_dilep = n_el_dilep * getError(el_dilep->GetBinError(1)/el_dilep->GetBinContent(1),SF_el_dilep->GetBinError(i)/SF_el_dilep->GetBinContent(i));
      n_error_mu_wjets = n_mu_wjets * getError(mu_wjets->GetBinError(1)/mu_wjets->GetBinContent(1),SF_mu_wjets->GetBinError(i)/SF_mu_wjets->GetBinContent(i));
      n_error_mu_ttbar = n_mu_ttbar * getError(mu_ttbar->GetBinError(1)/mu_ttbar->GetBinContent(1),SF_mu_ttbar->GetBinError(i)/SF_mu_ttbar->GetBinContent(i));
      n_error_mu_dilep = n_mu_dilep * getError(mu_dilep->GetBinError(1)/mu_dilep->GetBinContent(1),SF_mu_dilep->GetBinError(i)/SF_mu_dilep->GetBinContent(i));
      n_error_lp_wjets = n_lp_wjets * getError(lp_wjets->GetBinError(1)/lp_wjets->GetBinContent(1),SF_lp_wjets->GetBinError(i)/SF_lp_wjets->GetBinContent(i));
      n_error_lp_ttbar = n_lp_ttbar * getError(lp_ttbar->GetBinError(1)/lp_ttbar->GetBinContent(1),SF_lp_ttbar->GetBinError(i)/SF_lp_ttbar->GetBinContent(i));
      n_error_lp_dilep = n_lp_dilep * getError(lp_dilep->GetBinError(1)/lp_dilep->GetBinContent(1),SF_lp_dilep->GetBinError(i)/SF_lp_dilep->GetBinContent(i));
    } // getOOtB
    if(abs(n_el_wjets)<0.0001||abs(n_el_wjets)>100000000.0) n_error_el_wjets=0.0;
    if(abs(n_el_ttbar)<0.0001||abs(n_el_ttbar)>100000000.0) n_error_el_ttbar=0.0;
    if(abs(n_el_dilep)<0.0001||abs(n_el_dilep)>100000000.0) n_error_el_dilep=0.0;
    if(abs(n_mu_wjets)<0.0001||abs(n_mu_wjets)>100000000.0) n_error_mu_wjets=0.0;
    if(abs(n_mu_ttbar)<0.0001||abs(n_mu_ttbar)>100000000.0) n_error_mu_ttbar=0.0;
    if(abs(n_mu_dilep)<0.0001||abs(n_mu_dilep)>100000000.0) n_error_mu_dilep=0.0;
    if(abs(n_lp_wjets)<0.0001||abs(n_lp_wjets)>100000000.0) n_error_lp_wjets=0.0;
    if(abs(n_lp_ttbar)<0.0001||abs(n_lp_ttbar)>100000000.0) n_error_lp_ttbar=0.0;
    if(abs(n_lp_dilep)<0.0001||abs(n_lp_dilep)>100000000.0) n_error_lp_dilep=0.0;
    n_error_el_all   = getError(n_error_el_wjets, n_error_el_ttbar, n_error_el_dilep);
    n_error_mu_all   = getError(n_error_mu_wjets, n_error_mu_ttbar, n_error_mu_dilep);
    n_error_lp_all   = getError(n_error_lp_wjets, n_error_lp_ttbar, n_error_lp_dilep);
    n_error_el_data  = el_data ->GetBinError(1);
    n_error_mu_data  = mu_data ->GetBinError(1);
    n_error_lp_data  = lp_data ->GetBinError(1);

    cout << metLabel[i] << " & & & & & \\\\"<< endl
         << "electrons"
         << " & " << setprecision(3) <<  n_el_wjets << " $\\pm$ " << setprecision(2) << n_error_el_wjets
         << " & " << setprecision(3) <<  n_el_ttbar << " $\\pm$ " << setprecision(2) << n_error_el_ttbar
         << " & " << setprecision(3) <<  n_el_dilep << " $\\pm$ " << setprecision(2) << n_error_el_dilep
         << " & " << setprecision(3) <<  n_el_all   << " $\\pm$ " << setprecision(2) << n_error_el_all
         << " & " << setprecision(3) <<  n_el_data  << " $\\pm$ " << setprecision(2) << n_error_el_data
         << " \\\\ "<< endl
         << "muons"
         << " & " << setprecision(3) <<  n_mu_wjets << " $\\pm$ " << setprecision(2) << n_error_mu_wjets
         << " & " << setprecision(3) <<  n_mu_ttbar << " $\\pm$ " << setprecision(2) << n_error_mu_ttbar
         << " & " << setprecision(3) <<  n_mu_dilep << " $\\pm$ " << setprecision(2) << n_error_mu_dilep
         << " & " << setprecision(3) <<  n_mu_all   << " $\\pm$ " << setprecision(2) << n_error_mu_all
         << " & " << setprecision(3) <<  n_mu_data  << " $\\pm$ " << setprecision(2) << n_error_mu_data
         << " \\\\ "<< endl
         << "both"
         << " & " << setprecision(3) <<  n_lp_wjets << " $\\pm$ " << setprecision(2) << n_error_lp_wjets
         << " & " << setprecision(3) <<  n_lp_ttbar << " $\\pm$ " << setprecision(2) << n_error_lp_ttbar
         << " & " << setprecision(3) <<  n_lp_dilep << " $\\pm$ " << setprecision(2) << n_error_lp_dilep
         << " & " << setprecision(3) <<  n_lp_all   << " $\\pm$ " << setprecision(2) << n_error_lp_all
         << " & " << setprecision(3) <<  n_lp_data  << " $\\pm$ " << setprecision(2) << n_error_lp_data
         << " \\\\ "<< endl
         << "\\hline" << endl;
    delete el_data;
    delete mu_data;
    delete lp_data;
    delete el_wjets;
    delete mu_wjets;
    delete lp_wjets;
    delete el_ttbar;
    delete mu_ttbar;
    delete lp_ttbar;
    delete el_dilep;
    delete mu_dilep;
    delete lp_dilep;

  } // nBins
  return;
} // getSRnumbers()

void runWJets(TH1F* &SFW_el, TH1F* &SFW_mu, TH1F* &SFW_lp){

  // inclusive MT plots that SFs will come from
  plotMT(baseCuts1,"zerB_OOtB","cr1",SF_unit,SF_unit,SF_unit,SF_unit,SF_unit,SF_unit,SF_unit,SF_unit,SF_unit);

  // ===== "R" to scale W MC so W+ttbar MC agrees with data in low MT region =====

  // fill low MT regions
  TH1F* h_el_data_lowmt  = new TH1F("h_el_data_lowmt",  "h_el_data_lowmt",  nBins, edges );
  TH1F* h_mu_data_lowmt  = new TH1F("h_mu_data_lowmt",  "h_mu_data_lowmt",  nBins, edges );
  TH1F* h_lp_data_lowmt  = new TH1F("h_lp_data_lowmt",  "h_lp_data_lowmt",  nBins, edges );
  TH1F* h_el_wjets_lowmt = new TH1F("h_el_wjets_lowmt", "h_el_wjets_lowmt", nBins, edges );
  TH1F* h_mu_wjets_lowmt = new TH1F("h_mu_wjets_lowmt", "h_mu_wjets_lowmt", nBins, edges );
  TH1F* h_lp_wjets_lowmt = new TH1F("h_lp_wjets_lowmt", "h_lp_wjets_lowmt", nBins, edges );
  TH1F* h_el_ttbar_lowmt = new TH1F("h_el_ttbar_lowmt", "h_el_ttbar_lowmt", nBins, edges );
  TH1F* h_mu_ttbar_lowmt = new TH1F("h_mu_ttbar_lowmt", "h_mu_ttbar_lowmt", nBins, edges );
  TH1F* h_lp_ttbar_lowmt = new TH1F("h_lp_ttbar_lowmt", "h_lp_ttbar_lowmt", nBins, edges );
  t_eldata->Project("h_el_data_lowmt" ,"met",dtweight+baseCuts1+lowMT+elTrg);
  t_mudata->Project("h_mu_data_lowmt" ,"met",dtweight+baseCuts1+lowMT+muTrg);
  t_lpdata->Project("h_lp_data_lowmt" ,"met",dtweight+baseCuts1+lowMT+lpTrg);
  t_wjets ->Project("h_el_wjets_lowmt","met",mcweight+baseCuts1+lowMT+elTrg);
  t_wjets ->Project("h_mu_wjets_lowmt","met",mcweight+baseCuts1+lowMT+muTrg);
  t_wjets ->Project("h_lp_wjets_lowmt","met",mcweight+baseCuts1+lowMT+lpTrg);
  t_ttbar ->Project("h_el_ttbar_lowmt","met",mcweight+baseCuts1+lowMT+elTrg);
  t_ttbar ->Project("h_mu_ttbar_lowmt","met",mcweight+baseCuts1+lowMT+muTrg);
  t_ttbar ->Project("h_lp_ttbar_lowmt","met",mcweight+baseCuts1+lowMT+lpTrg);
  h_el_data_lowmt ->SetBinContent( nBins, h_el_data_lowmt ->GetBinContent(nBins) + h_el_data_lowmt ->GetBinContent(nBins+1) );
  h_mu_data_lowmt ->SetBinContent( nBins, h_mu_data_lowmt ->GetBinContent(nBins) + h_mu_data_lowmt ->GetBinContent(nBins+1) );
  h_lp_data_lowmt ->SetBinContent( nBins, h_lp_data_lowmt ->GetBinContent(nBins) + h_lp_data_lowmt ->GetBinContent(nBins+1) );
  h_el_wjets_lowmt->SetBinContent( nBins, h_el_wjets_lowmt->GetBinContent(nBins) + h_el_wjets_lowmt->GetBinContent(nBins+1) );
  h_mu_wjets_lowmt->SetBinContent( nBins, h_mu_wjets_lowmt->GetBinContent(nBins) + h_mu_wjets_lowmt->GetBinContent(nBins+1) );
  h_lp_wjets_lowmt->SetBinContent( nBins, h_lp_wjets_lowmt->GetBinContent(nBins) + h_lp_wjets_lowmt->GetBinContent(nBins+1) );
  h_el_ttbar_lowmt->SetBinContent( nBins, h_el_ttbar_lowmt->GetBinContent(nBins) + h_el_ttbar_lowmt->GetBinContent(nBins+1) );
  h_mu_ttbar_lowmt->SetBinContent( nBins, h_mu_ttbar_lowmt->GetBinContent(nBins) + h_mu_ttbar_lowmt->GetBinContent(nBins+1) );
  h_lp_ttbar_lowmt->SetBinContent( nBins, h_lp_ttbar_lowmt->GetBinContent(nBins) + h_lp_ttbar_lowmt->GetBinContent(nBins+1) );
  h_el_data_lowmt ->SetBinError( nBins, getError(h_el_data_lowmt ->GetBinError(nBins),h_el_data_lowmt ->GetBinError(nBins+1)) );
  h_mu_data_lowmt ->SetBinError( nBins, getError(h_mu_data_lowmt ->GetBinError(nBins),h_mu_data_lowmt ->GetBinError(nBins+1)) );
  h_lp_data_lowmt ->SetBinError( nBins, getError(h_lp_data_lowmt ->GetBinError(nBins),h_lp_data_lowmt ->GetBinError(nBins+1)) );
  h_el_wjets_lowmt->SetBinError( nBins, getError(h_el_wjets_lowmt->GetBinError(nBins),h_el_wjets_lowmt->GetBinError(nBins+1)) );
  h_mu_wjets_lowmt->SetBinError( nBins, getError(h_mu_wjets_lowmt->GetBinError(nBins),h_mu_wjets_lowmt->GetBinError(nBins+1)) );
  h_lp_wjets_lowmt->SetBinError( nBins, getError(h_lp_wjets_lowmt->GetBinError(nBins),h_lp_wjets_lowmt->GetBinError(nBins+1)) );
  h_el_ttbar_lowmt->SetBinError( nBins, getError(h_el_ttbar_lowmt->GetBinError(nBins),h_el_ttbar_lowmt->GetBinError(nBins+1)) );
  h_mu_ttbar_lowmt->SetBinError( nBins, getError(h_mu_ttbar_lowmt->GetBinError(nBins),h_mu_ttbar_lowmt->GetBinError(nBins+1)) );
  h_lp_ttbar_lowmt->SetBinError( nBins, getError(h_lp_ttbar_lowmt->GetBinError(nBins),h_lp_ttbar_lowmt->GetBinError(nBins+1)) );

  // ttbar-subtracted data
  h_el_data_lowmt->Add(h_el_ttbar_lowmt,-1);
  h_mu_data_lowmt->Add(h_mu_ttbar_lowmt,-1);
  h_lp_data_lowmt->Add(h_lp_ttbar_lowmt,-1);

  TH1F* R_el = (TH1F*)h_el_data_lowmt->Clone("R_el"); R_el->Divide(h_el_wjets_lowmt);
  TH1F* R_mu = (TH1F*)h_mu_data_lowmt->Clone("R_mu"); R_mu->Divide(h_mu_wjets_lowmt);
  TH1F* R_lp = (TH1F*)h_lp_data_lowmt->Clone("R_lp"); R_lp->Divide(h_lp_wjets_lowmt);
  R_el->SetLineColor(1); R_el->SetLineWidth(3); R_el->SetTitle("electrons");
  R_mu->SetLineColor(1); R_mu->SetLineWidth(3); R_mu->SetTitle("muons");
  R_lp->SetLineColor(1); R_lp->SetLineWidth(3); R_lp->SetTitle("muons");

  TCanvas * c_R = new TCanvas( "c_R","c_R", 1500, 500 ); c_R->Divide(3,1);
  c_R->cd(1); R_el->Draw(); R_el->GetXaxis()->SetTitle("MET");
  c_R->cd(2); R_mu->Draw(); R_mu->GetXaxis()->SetTitle("MET");
  c_R->cd(3); R_lp->Draw(); R_lp->GetXaxis()->SetTitle("MET");
  c_R->Update();
  c_R->SaveAs("plots/cr1/R.png");

  plotMT(baseCuts1,"zerB_midMTWscaled"  ,"cr1",R_el,R_mu,R_lp,SF_unit,SF_unit,SF_unit,SF_unit,SF_unit,SF_unit);


  // ===== extract SFWs from MT>150 tail =====

  TH1F* h_el_data_hghmt  = new TH1F("h_el_data_hghmt" , "h_el_data_hghmt" , nBins, edges );
  TH1F* h_mu_data_hghmt  = new TH1F("h_mu_data_hghmt" , "h_mu_data_hghmt" , nBins, edges );
  TH1F* h_lp_data_hghmt  = new TH1F("h_lp_data_hghmt" , "h_lp_data_hghmt" , nBins, edges );
  TH1F* h_el_wjets_hghmt = new TH1F("h_el_wjets_hghmt", "h_el_wjets_hghmt", nBins, edges );
  TH1F* h_mu_wjets_hghmt = new TH1F("h_mu_wjets_hghmt", "h_mu_wjets_hghmt", nBins, edges );
  TH1F* h_lp_wjets_hghmt = new TH1F("h_lp_wjets_hghmt", "h_lp_wjets_hghmt", nBins, edges );
  TH1F* h_el_ttbar_hghmt = new TH1F("h_el_ttbar_hghmt", "h_el_ttbar_hghmt", nBins, edges );
  TH1F* h_mu_ttbar_hghmt = new TH1F("h_mu_ttbar_hghmt", "h_mu_ttbar_hghmt", nBins, edges );
  TH1F* h_lp_ttbar_hghmt = new TH1F("h_lp_ttbar_hghmt", "h_lp_ttbar_hghmt", nBins, edges );
  t_eldata->Project("h_el_data_hghmt" ,"met",dtweight+baseCuts1+hghMT+elTrg);
  t_mudata->Project("h_mu_data_hghmt" ,"met",dtweight+baseCuts1+hghMT+muTrg);
  t_lpdata->Project("h_lp_data_hghmt" ,"met",dtweight+baseCuts1+hghMT+lpTrg);
  t_wjets ->Project("h_el_wjets_hghmt","met",mcweight+baseCuts1+hghMT+elTrg);
  t_wjets ->Project("h_mu_wjets_hghmt","met",mcweight+baseCuts1+hghMT+muTrg);
  t_wjets ->Project("h_lp_wjets_hghmt","met",mcweight+baseCuts1+hghMT+lpTrg);
  t_ttbar ->Project("h_el_ttbar_hghmt","met",mcweight+baseCuts1+hghMT+elTrg);
  t_ttbar ->Project("h_mu_ttbar_hghmt","met",mcweight+baseCuts1+hghMT+muTrg);
  t_ttbar ->Project("h_lp_ttbar_hghmt","met",mcweight+baseCuts1+hghMT+lpTrg);
  h_el_data_hghmt ->SetBinContent( nBins, h_el_data_hghmt ->GetBinContent(nBins) + h_el_data_hghmt ->GetBinContent(nBins+1) );
  h_mu_data_hghmt ->SetBinContent( nBins, h_mu_data_hghmt ->GetBinContent(nBins) + h_mu_data_hghmt ->GetBinContent(nBins+1) );
  h_lp_data_hghmt ->SetBinContent( nBins, h_lp_data_hghmt ->GetBinContent(nBins) + h_lp_data_hghmt ->GetBinContent(nBins+1) );
  h_el_wjets_hghmt->SetBinContent( nBins, h_el_wjets_hghmt->GetBinContent(nBins) + h_el_wjets_hghmt->GetBinContent(nBins+1) );
  h_mu_wjets_hghmt->SetBinContent( nBins, h_mu_wjets_hghmt->GetBinContent(nBins) + h_mu_wjets_hghmt->GetBinContent(nBins+1) );
  h_lp_wjets_hghmt->SetBinContent( nBins, h_lp_wjets_hghmt->GetBinContent(nBins) + h_lp_wjets_hghmt->GetBinContent(nBins+1) );
  h_el_ttbar_hghmt->SetBinContent( nBins, h_el_ttbar_hghmt->GetBinContent(nBins) + h_el_ttbar_hghmt->GetBinContent(nBins+1) );
  h_mu_ttbar_hghmt->SetBinContent( nBins, h_mu_ttbar_hghmt->GetBinContent(nBins) + h_mu_ttbar_hghmt->GetBinContent(nBins+1) );
  h_lp_ttbar_hghmt->SetBinContent( nBins, h_lp_ttbar_hghmt->GetBinContent(nBins) + h_lp_ttbar_hghmt->GetBinContent(nBins+1) );
  h_el_data_hghmt ->SetBinError( nBins, getError(h_el_data_hghmt ->GetBinError(nBins),h_el_data_hghmt ->GetBinError(nBins+1)) );
  h_mu_data_hghmt ->SetBinError( nBins, getError(h_mu_data_hghmt ->GetBinError(nBins),h_mu_data_hghmt ->GetBinError(nBins+1)) );
  h_lp_data_hghmt ->SetBinError( nBins, getError(h_lp_data_hghmt ->GetBinError(nBins),h_lp_data_hghmt ->GetBinError(nBins+1)) );
  h_el_wjets_hghmt->SetBinError( nBins, getError(h_el_wjets_hghmt->GetBinError(nBins),h_el_wjets_hghmt->GetBinError(nBins+1)) );
  h_mu_wjets_hghmt->SetBinError( nBins, getError(h_mu_wjets_hghmt->GetBinError(nBins),h_mu_wjets_hghmt->GetBinError(nBins+1)) );
  h_lp_wjets_hghmt->SetBinError( nBins, getError(h_lp_wjets_hghmt->GetBinError(nBins),h_lp_wjets_hghmt->GetBinError(nBins+1)) );
  h_el_ttbar_hghmt->SetBinError( nBins, getError(h_el_ttbar_hghmt->GetBinError(nBins),h_el_ttbar_hghmt->GetBinError(nBins+1)) );
  h_mu_ttbar_hghmt->SetBinError( nBins, getError(h_mu_ttbar_hghmt->GetBinError(nBins),h_mu_ttbar_hghmt->GetBinError(nBins+1)) );
  h_lp_ttbar_hghmt->SetBinError( nBins, getError(h_lp_ttbar_hghmt->GetBinError(nBins),h_lp_ttbar_hghmt->GetBinError(nBins+1)) );

  // scale W-component by R
  h_el_wjets_hghmt->Multiply(R_el);
  h_mu_wjets_hghmt->Multiply(R_mu);
  h_lp_wjets_hghmt->Multiply(R_lp);

  TH1F* h_el_wjall_hghmt = (TH1F*)h_el_wjets_hghmt->Clone("h_el_wjall_hghmt"); h_el_wjall_hghmt->Add(h_el_ttbar_hghmt);
  TH1F* h_mu_wjall_hghmt = (TH1F*)h_mu_wjets_hghmt->Clone("h_mu_wjall_hghmt"); h_mu_wjall_hghmt->Add(h_mu_ttbar_hghmt);
  TH1F* h_lp_wjall_hghmt = (TH1F*)h_lp_wjets_hghmt->Clone("h_lp_wjall_hghmt"); h_lp_wjall_hghmt->Add(h_lp_ttbar_hghmt);

  // find SFW_all by scaling all MC in the tail region
  TH1F* SFW_all_el = (TH1F*)h_el_data_hghmt->Clone("SFW_all_el"); SFW_all_el->Divide(h_el_wjall_hghmt);
  TH1F* SFW_all_mu = (TH1F*)h_mu_data_hghmt->Clone("SFW_all_mu"); SFW_all_mu->Divide(h_mu_wjall_hghmt);
  TH1F* SFW_all_lp = (TH1F*)h_lp_data_hghmt->Clone("SFW_all_lp"); SFW_all_lp->Divide(h_lp_wjall_hghmt);
  SFW_all_el->SetLineColor(1); SFW_all_el->SetLineWidth(3); SFW_all_el->SetTitle("electrons");
  SFW_all_mu->SetLineColor(1); SFW_all_mu->SetLineWidth(3); SFW_all_mu->SetTitle("muons");
  SFW_all_lp->SetLineColor(1); SFW_all_lp->SetLineWidth(3); SFW_all_lp->SetTitle("both");

  TCanvas * c_SFW_all = new TCanvas( "c_SFW_all","c_SFW_all", 1500, 500 ); c_SFW_all->Divide(3,1);
  c_SFW_all->cd(1); SFW_all_el->Draw(); SFW_all_el->GetXaxis()->SetTitle("MET");
  c_SFW_all->cd(2); SFW_all_mu->Draw(); SFW_all_mu->GetXaxis()->SetTitle("MET");
  c_SFW_all->cd(3); SFW_all_lp->Draw(); SFW_all_lp->GetXaxis()->SetTitle("MET");
  c_SFW_all->Update();
  c_SFW_all->SaveAs("plots/cr1/SFW_all.png");

  // MT tail scaled by SFW_all
  TH1F* SFWxR_all_el = (TH1F*)SFW_all_el->Clone("SFWxR_all_el"); SFWxR_all_el->Multiply(R_el);
  TH1F* SFWxR_all_mu = (TH1F*)SFW_all_mu->Clone("SFWxR_all_mu"); SFWxR_all_mu->Multiply(R_mu);
  TH1F* SFWxR_all_lp = (TH1F*)SFW_all_lp->Clone("SFWxR_all_lp"); SFWxR_all_lp->Multiply(R_lp);

  // === find SFW_wjt by scaling only W MC in the tail region
  // ttbar subtracted data
  h_el_data_hghmt->Add(h_el_ttbar_hghmt,-1);
  h_mu_data_hghmt->Add(h_mu_ttbar_hghmt,-1);
  h_lp_data_hghmt->Add(h_lp_ttbar_hghmt,-1);

  TH1F* SFW_wjt_el = (TH1F*)h_el_data_hghmt->Clone("SFW_wjt_el"); SFW_wjt_el->Divide(h_el_wjets_hghmt);
  TH1F* SFW_wjt_mu = (TH1F*)h_mu_data_hghmt->Clone("SFW_wjt_mu"); SFW_wjt_mu->Divide(h_mu_wjets_hghmt);
  TH1F* SFW_wjt_lp = (TH1F*)h_lp_data_hghmt->Clone("SFW_wjt_lp"); SFW_wjt_lp->Divide(h_lp_wjets_hghmt);
  SFW_wjt_el->SetLineColor(1); SFW_wjt_el->SetLineWidth(3); SFW_wjt_el->SetTitle("electrons");
  SFW_wjt_mu->SetLineColor(1); SFW_wjt_mu->SetLineWidth(3); SFW_wjt_mu->SetTitle("muons");
  SFW_wjt_lp->SetLineColor(1); SFW_wjt_lp->SetLineWidth(3); SFW_wjt_lp->SetTitle("both");

  TCanvas * c_SFW_wjt = new TCanvas( "c_SFW_wjt","c_SFW_wjt", 1500, 500 ); c_SFW_wjt->Divide(3,1);
  c_SFW_wjt->cd(1); SFW_wjt_el->Draw(); SFW_wjt_el->GetXaxis()->SetTitle("MET");
  c_SFW_wjt->cd(2); SFW_wjt_mu->Draw(); SFW_wjt_mu->GetXaxis()->SetTitle("MET");
  c_SFW_wjt->cd(3); SFW_wjt_lp->Draw(); SFW_wjt_lp->GetXaxis()->SetTitle("MET");
  c_SFW_wjt->Update();
  c_SFW_wjt->SaveAs("plots/cr1/SFW_wjt.png");

  // MT tail scaled by SFW_wjt
  TH1F* SFWxR_wjt_el = (TH1F*)SFW_wjt_el->Clone("SFWxR_wjt_el"); SFWxR_wjt_el->Multiply(R_el);
  TH1F* SFWxR_wjt_mu = (TH1F*)SFW_wjt_mu->Clone("SFWxR_wjt_mu"); SFWxR_wjt_mu->Multiply(R_mu);
  TH1F* SFWxR_wjt_lp = (TH1F*)SFW_wjt_lp->Clone("SFWxR_wjt_lp"); SFWxR_wjt_lp->Multiply(R_lp);

  // average SFW_all and SFW_wjt to get SFW
  for(int i=1; i<=nBins; ++i){
    SFW_el->SetBinContent( i, (SFW_all_el->GetBinContent(i)+SFW_wjt_el->GetBinContent(i))/2.0 );
    SFW_mu->SetBinContent( i, (SFW_all_mu->GetBinContent(i)+SFW_wjt_mu->GetBinContent(i))/2.0 );
    SFW_lp->SetBinContent( i, (SFW_all_lp->GetBinContent(i)+SFW_wjt_lp->GetBinContent(i))/2.0 );
    double el_err_all = SFW_all_el->GetBinError(i);
    double el_err_wjt = SFW_wjt_el->GetBinError(i);
    double el_err = sqrt( el_err_all*el_err_all + el_err_wjt*el_err_wjt);
    SFW_el->SetBinError(i,el_err);
    double mu_err_all = SFW_all_mu->GetBinError(i);
    double mu_err_wjt = SFW_wjt_mu->GetBinError(i);
    double mu_err = sqrt( mu_err_all*mu_err_all + mu_err_wjt*mu_err_wjt);
    SFW_mu->SetBinError(i,mu_err);
    double lp_err_all = SFW_all_lp->GetBinError(i);
    double lp_err_wjt = SFW_wjt_lp->GetBinError(i);
    double lp_err = sqrt( lp_err_all*lp_err_all + lp_err_wjt*lp_err_wjt);
    SFW_lp->SetBinError(i,lp_err);
  } // nBins
  SFW_el->SetLineColor(1); SFW_el->SetLineWidth(3); SFW_el->SetTitle("electrons");
  SFW_mu->SetLineColor(1); SFW_mu->SetLineWidth(3); SFW_mu->SetTitle("muons");
  SFW_lp->SetLineColor(1); SFW_lp->SetLineWidth(3); SFW_lp->SetTitle("both");

  TCanvas * c_SFW = new TCanvas( "c_SFW","c_SFW", 1500, 500 ); c_SFW->Divide(3,1);
  c_SFW->cd(1); SFW_el->Draw(); SFW_el->GetXaxis()->SetTitle("MET");
  c_SFW->cd(2); SFW_mu->Draw(); SFW_mu->GetXaxis()->SetTitle("MET");
  c_SFW->cd(3); SFW_lp->Draw(); SFW_lp->GetXaxis()->SetTitle("MET");
  c_SFW->Update();
  c_SFW->SaveAs("plots/cr1/SFW.png");

  delete h_el_data_hghmt  ;
  delete h_mu_data_hghmt  ;
  delete h_lp_data_hghmt  ;
  delete h_el_wjets_hghmt ;
  delete h_mu_wjets_hghmt ;
  delete h_lp_wjets_hghmt ;
  delete h_el_ttbar_hghmt ;
  delete h_mu_ttbar_hghmt ;
  delete h_lp_ttbar_hghmt ;
  delete h_el_data_lowmt  ;
  delete h_mu_data_lowmt  ;
  delete h_lp_data_lowmt  ;
  delete h_el_wjets_lowmt ;
  delete h_mu_wjets_lowmt ;
  delete h_lp_wjets_lowmt ;
  delete h_el_ttbar_lowmt ;
  delete h_mu_ttbar_lowmt ;
  delete h_lp_ttbar_lowmt ;
  delete c_R              ;
  delete c_SFW_all        ;
  delete c_SFW_wjt        ;
  delete c_SFW            ;

  return;
} // runWJets()

void runDiLep(TH1F* &SF2L_el, TH1F* &SF2L_mu, TH1F* &SF2L_lp){

  // todo: K factors from p34-35

  plotMT(baseCuts4+dilepCut,"OOtB","cr4",SF_unit,SF_unit,SF_unit,SF_unit,SF_unit,SF_unit,SF_unit,SF_unit,SF_unit);

  // ===== "R" to scale ttbar MC so W+ttbar MC agrees with data in each MET bin =====

  // fill low MT regions
  TH1F* h_el_data_inc  = new TH1F("h_el_data_inc",  "h_el_data_inc",  nBins, edges );
  TH1F* h_mu_data_inc  = new TH1F("h_mu_data_inc",  "h_mu_data_inc",  nBins, edges );
  TH1F* h_lp_data_inc  = new TH1F("h_lp_data_inc",  "h_lp_data_inc",  nBins, edges );
  TH1F* h_el_wjets_inc = new TH1F("h_el_wjets_inc", "h_el_wjets_inc", nBins, edges );
  TH1F* h_mu_wjets_inc = new TH1F("h_mu_wjets_inc", "h_mu_wjets_inc", nBins, edges );
  TH1F* h_lp_wjets_inc = new TH1F("h_lp_wjets_inc", "h_lp_wjets_inc", nBins, edges );
  TH1F* h_el_ttbar_inc = new TH1F("h_el_ttbar_inc", "h_el_ttbar_inc", nBins, edges );
  TH1F* h_mu_ttbar_inc = new TH1F("h_mu_ttbar_inc", "h_mu_ttbar_inc", nBins, edges );
  TH1F* h_lp_ttbar_inc = new TH1F("h_lp_ttbar_inc", "h_lp_ttbar_inc", nBins, edges );
  t_eldata->Project("h_el_data_inc" ,"met",dtweight+baseCuts4+dilepCut+elTrg);
  t_mudata->Project("h_mu_data_inc" ,"met",dtweight+baseCuts4+dilepCut+muTrg);
  t_lpdata->Project("h_lp_data_inc" ,"met",dtweight+baseCuts4+dilepCut+lpTrg);
  t_wjets ->Project("h_el_wjets_inc","met",mcweight+baseCuts4+dilepCut+elTrg);
  t_wjets ->Project("h_mu_wjets_inc","met",mcweight+baseCuts4+dilepCut+muTrg);
  t_wjets ->Project("h_lp_wjets_inc","met",mcweight+baseCuts4+dilepCut+lpTrg);
  t_ttbar ->Project("h_el_ttbar_inc","met",mcweight+baseCuts4+dilepCut+elTrg);
  t_ttbar ->Project("h_mu_ttbar_inc","met",mcweight+baseCuts4+dilepCut+muTrg);
  t_ttbar ->Project("h_lp_ttbar_inc","met",mcweight+baseCuts4+dilepCut+lpTrg);
  h_el_data_inc ->SetBinContent( nBins, h_el_data_inc ->GetBinContent(nBins) + h_el_data_inc ->GetBinContent(nBins+1) );
  h_mu_data_inc ->SetBinContent( nBins, h_mu_data_inc ->GetBinContent(nBins) + h_mu_data_inc ->GetBinContent(nBins+1) );
  h_lp_data_inc ->SetBinContent( nBins, h_lp_data_inc ->GetBinContent(nBins) + h_lp_data_inc ->GetBinContent(nBins+1) );
  h_el_wjets_inc->SetBinContent( nBins, h_el_wjets_inc->GetBinContent(nBins) + h_el_wjets_inc->GetBinContent(nBins+1) );
  h_mu_wjets_inc->SetBinContent( nBins, h_mu_wjets_inc->GetBinContent(nBins) + h_mu_wjets_inc->GetBinContent(nBins+1) );
  h_lp_wjets_inc->SetBinContent( nBins, h_lp_wjets_inc->GetBinContent(nBins) + h_lp_wjets_inc->GetBinContent(nBins+1) );
  h_el_ttbar_inc->SetBinContent( nBins, h_el_ttbar_inc->GetBinContent(nBins) + h_el_ttbar_inc->GetBinContent(nBins+1) );
  h_mu_ttbar_inc->SetBinContent( nBins, h_mu_ttbar_inc->GetBinContent(nBins) + h_mu_ttbar_inc->GetBinContent(nBins+1) );
  h_lp_ttbar_inc->SetBinContent( nBins, h_lp_ttbar_inc->GetBinContent(nBins) + h_lp_ttbar_inc->GetBinContent(nBins+1) );
  h_el_data_inc ->SetBinError( nBins, getError(h_el_data_inc ->GetBinError(nBins),h_el_data_inc ->GetBinError(nBins+1)) );
  h_mu_data_inc ->SetBinError( nBins, getError(h_mu_data_inc ->GetBinError(nBins),h_mu_data_inc ->GetBinError(nBins+1)) );
  h_lp_data_inc ->SetBinError( nBins, getError(h_lp_data_inc ->GetBinError(nBins),h_lp_data_inc ->GetBinError(nBins+1)) );
  h_el_wjets_inc->SetBinError( nBins, getError(h_el_wjets_inc->GetBinError(nBins),h_el_wjets_inc->GetBinError(nBins+1)) );
  h_mu_wjets_inc->SetBinError( nBins, getError(h_mu_wjets_inc->GetBinError(nBins),h_mu_wjets_inc->GetBinError(nBins+1)) );
  h_lp_wjets_inc->SetBinError( nBins, getError(h_lp_wjets_inc->GetBinError(nBins),h_lp_wjets_inc->GetBinError(nBins+1)) );
  h_el_ttbar_inc->SetBinError( nBins, getError(h_el_ttbar_inc->GetBinError(nBins),h_el_ttbar_inc->GetBinError(nBins+1)) );
  h_mu_ttbar_inc->SetBinError( nBins, getError(h_mu_ttbar_inc->GetBinError(nBins),h_mu_ttbar_inc->GetBinError(nBins+1)) );
  h_lp_ttbar_inc->SetBinError( nBins, getError(h_lp_ttbar_inc->GetBinError(nBins),h_lp_ttbar_inc->GetBinError(nBins+1)) );

  // W-subtracted data
  h_el_data_inc->Add(h_el_wjets_inc,-1);
  h_mu_data_inc->Add(h_mu_wjets_inc,-1);
  h_lp_data_inc->Add(h_lp_wjets_inc,-1);

  TH1F* R_el = (TH1F*)h_el_data_inc->Clone("R_el"); R_el->Divide(h_el_ttbar_inc);
  TH1F* R_mu = (TH1F*)h_mu_data_inc->Clone("R_mu"); R_mu->Divide(h_mu_ttbar_inc);
  TH1F* R_lp = (TH1F*)h_lp_data_inc->Clone("R_lp"); R_lp->Divide(h_lp_ttbar_inc);
  for(int i=1; i<=nBins; ++i) {
    if(h_el_data_inc->GetBinContent(i)<0.00001) { R_el->SetBinContent(i,1); R_el->SetBinError(i,1); }
    if(h_mu_data_inc->GetBinContent(i)<0.00001) { R_mu->SetBinContent(i,1); R_mu->SetBinError(i,1); }
    if(h_lp_data_inc->GetBinContent(i)<0.00001) { R_lp->SetBinContent(i,1); R_lp->SetBinError(i,1); }
  }
  R_el->SetLineColor(1); R_el->SetLineWidth(3); R_el->SetTitle("electrons");
  R_mu->SetLineColor(1); R_mu->SetLineWidth(3); R_mu->SetTitle("muons");
  R_lp->SetLineColor(1); R_lp->SetLineWidth(3); R_lp->SetTitle("both");

  TCanvas * c_R = new TCanvas( "c_R","c_R", 1500, 500 ); c_R->Divide(3,1);
  c_R->cd(1); R_el->Draw(); R_el->GetXaxis()->SetTitle("MET");
  c_R->cd(2); R_mu->Draw(); R_mu->GetXaxis()->SetTitle("MET");
  c_R->cd(3); R_lp->Draw(); R_lp->GetXaxis()->SetTitle("MET");
  c_R->Update();
  c_R->SaveAs("plots/cr4/R.png");

  plotMT(baseCuts4+dilepCut,"ttbarScaled","cr4"  ,SF_unit,SF_unit,SF_unit,R_el,R_mu,R_lp,R_el,R_mu,R_lp);


  // ===== extract SF2L from MT>150 tail =====

  TH1F* h_el_data_hghmt  = new TH1F("h_el_data_hghmt" , "h_el_data_hghmt" , nBins, edges );
  TH1F* h_mu_data_hghmt  = new TH1F("h_mu_data_hghmt" , "h_mu_data_hghmt" , nBins, edges );
  TH1F* h_lp_data_hghmt  = new TH1F("h_lp_data_hghmt" , "h_lp_data_hghmt" , nBins, edges );
  TH1F* h_el_wjets_hghmt = new TH1F("h_el_wjets_hghmt", "h_el_wjets_hghmt", nBins, edges );
  TH1F* h_mu_wjets_hghmt = new TH1F("h_mu_wjets_hghmt", "h_mu_wjets_hghmt", nBins, edges );
  TH1F* h_lp_wjets_hghmt = new TH1F("h_lp_wjets_hghmt", "h_lp_wjets_hghmt", nBins, edges );
  TH1F* h_el_ttbar_hghmt = new TH1F("h_el_ttbar_hghmt", "h_el_ttbar_hghmt", nBins, edges );
  TH1F* h_mu_ttbar_hghmt = new TH1F("h_mu_ttbar_hghmt", "h_mu_ttbar_hghmt", nBins, edges );
  TH1F* h_lp_ttbar_hghmt = new TH1F("h_lp_ttbar_hghmt", "h_lp_ttbar_hghmt", nBins, edges );
  t_eldata->Project("h_el_data_hghmt" ,"met",dtweight+baseCuts4+dilepCut+hghMT+elTrg);
  t_mudata->Project("h_mu_data_hghmt" ,"met",dtweight+baseCuts4+dilepCut+hghMT+muTrg);
  t_lpdata->Project("h_lp_data_hghmt" ,"met",dtweight+baseCuts4+dilepCut+hghMT+lpTrg);
  t_wjets ->Project("h_el_wjets_hghmt","met",mcweight+baseCuts4+dilepCut+hghMT+elTrg);
  t_wjets ->Project("h_mu_wjets_hghmt","met",mcweight+baseCuts4+dilepCut+hghMT+muTrg);
  t_wjets ->Project("h_lp_wjets_hghmt","met",mcweight+baseCuts4+dilepCut+hghMT+lpTrg);
  t_ttbar ->Project("h_el_ttbar_hghmt","met",mcweight+baseCuts4+dilepCut+hghMT+elTrg);
  t_ttbar ->Project("h_mu_ttbar_hghmt","met",mcweight+baseCuts4+dilepCut+hghMT+muTrg);
  t_ttbar ->Project("h_lp_ttbar_hghmt","met",mcweight+baseCuts4+dilepCut+hghMT+lpTrg);
  h_el_data_hghmt ->SetBinContent( nBins, h_el_data_hghmt ->GetBinContent(nBins) + h_el_data_hghmt ->GetBinContent(nBins+1) );
  h_mu_data_hghmt ->SetBinContent( nBins, h_mu_data_hghmt ->GetBinContent(nBins) + h_mu_data_hghmt ->GetBinContent(nBins+1) );
  h_lp_data_hghmt ->SetBinContent( nBins, h_lp_data_hghmt ->GetBinContent(nBins) + h_lp_data_hghmt ->GetBinContent(nBins+1) );
  h_el_wjets_hghmt->SetBinContent( nBins, h_el_wjets_hghmt->GetBinContent(nBins) + h_el_wjets_hghmt->GetBinContent(nBins+1) );
  h_mu_wjets_hghmt->SetBinContent( nBins, h_mu_wjets_hghmt->GetBinContent(nBins) + h_mu_wjets_hghmt->GetBinContent(nBins+1) );
  h_lp_wjets_hghmt->SetBinContent( nBins, h_lp_wjets_hghmt->GetBinContent(nBins) + h_lp_wjets_hghmt->GetBinContent(nBins+1) );
  h_el_ttbar_hghmt->SetBinContent( nBins, h_el_ttbar_hghmt->GetBinContent(nBins) + h_el_ttbar_hghmt->GetBinContent(nBins+1) );
  h_mu_ttbar_hghmt->SetBinContent( nBins, h_mu_ttbar_hghmt->GetBinContent(nBins) + h_mu_ttbar_hghmt->GetBinContent(nBins+1) );
  h_lp_ttbar_hghmt->SetBinContent( nBins, h_lp_ttbar_hghmt->GetBinContent(nBins) + h_lp_ttbar_hghmt->GetBinContent(nBins+1) );
  h_el_data_hghmt ->SetBinError( nBins, getError(h_el_data_hghmt ->GetBinError(nBins),h_el_data_hghmt ->GetBinError(nBins+1)) );
  h_mu_data_hghmt ->SetBinError( nBins, getError(h_mu_data_hghmt ->GetBinError(nBins),h_mu_data_hghmt ->GetBinError(nBins+1)) );
  h_lp_data_hghmt ->SetBinError( nBins, getError(h_lp_data_hghmt ->GetBinError(nBins),h_lp_data_hghmt ->GetBinError(nBins+1)) );
  h_el_wjets_hghmt->SetBinError( nBins, getError(h_el_wjets_hghmt->GetBinError(nBins),h_el_wjets_hghmt->GetBinError(nBins+1)) );
  h_mu_wjets_hghmt->SetBinError( nBins, getError(h_mu_wjets_hghmt->GetBinError(nBins),h_mu_wjets_hghmt->GetBinError(nBins+1)) );
  h_lp_wjets_hghmt->SetBinError( nBins, getError(h_lp_wjets_hghmt->GetBinError(nBins),h_lp_wjets_hghmt->GetBinError(nBins+1)) );
  h_el_ttbar_hghmt->SetBinError( nBins, getError(h_el_ttbar_hghmt->GetBinError(nBins),h_el_ttbar_hghmt->GetBinError(nBins+1)) );
  h_mu_ttbar_hghmt->SetBinError( nBins, getError(h_mu_ttbar_hghmt->GetBinError(nBins),h_mu_ttbar_hghmt->GetBinError(nBins+1)) );
  h_lp_ttbar_hghmt->SetBinError( nBins, getError(h_lp_ttbar_hghmt->GetBinError(nBins),h_lp_ttbar_hghmt->GetBinError(nBins+1)) );

  // subtract off Wjet MC from data
  h_el_data_hghmt->Add(h_el_wjets_hghmt,-1);
  h_mu_data_hghmt->Add(h_mu_wjets_hghmt,-1);
  h_lp_data_hghmt->Add(h_lp_wjets_hghmt,-1);

  // scale ttbar-component by R
  h_el_ttbar_hghmt->Multiply(R_el);
  h_mu_ttbar_hghmt->Multiply(R_mu);
  h_lp_ttbar_hghmt->Multiply(R_lp);

  // find SF2L by scaling all ttbar MC to data in the tail region
  SF2L_el = (TH1F*)h_el_data_hghmt->Clone("SF2L_el"); SF2L_el->Divide(h_el_ttbar_hghmt);
  SF2L_mu = (TH1F*)h_mu_data_hghmt->Clone("SF2L_mu"); SF2L_mu->Divide(h_mu_ttbar_hghmt);
  SF2L_lp = (TH1F*)h_lp_data_hghmt->Clone("SF2L_lp"); SF2L_lp->Divide(h_lp_ttbar_hghmt);
  SF2L_el->SetLineColor(1); SF2L_el->SetLineWidth(3); SF2L_el->SetTitle("electrons");
  SF2L_mu->SetLineColor(1); SF2L_mu->SetLineWidth(3); SF2L_mu->SetTitle("muons");
  SF2L_lp->SetLineColor(1); SF2L_lp->SetLineWidth(3); SF2L_lp->SetTitle("both");

  TCanvas * c_SF2L = new TCanvas( "c_SF2L","c_SF2L", 1500, 500 ); c_SF2L->Divide(3,1);
  c_SF2L->cd(1); SF2L_el->Draw(); SF2L_el->GetXaxis()->SetTitle("MET");
  c_SF2L->cd(2); SF2L_mu->Draw(); SF2L_mu->GetXaxis()->SetTitle("MET");
  c_SF2L->cd(3); SF2L_lp->Draw(); SF2L_lp->GetXaxis()->SetTitle("MET");
  c_SF2L->Update();
  c_SF2L->SaveAs("plots/cr4/SF2L.png");

  delete h_el_data_inc  ;
  delete h_mu_data_inc  ;
  delete h_lp_data_inc  ;
  delete h_el_wjets_inc ;
  delete h_mu_wjets_inc ;
  delete h_lp_wjets_inc ;
  delete h_el_ttbar_inc ;
  delete h_mu_ttbar_inc ;
  delete h_lp_ttbar_inc ;
  delete h_el_data_hghmt  ;
  delete h_mu_data_hghmt  ;
  delete h_lp_data_hghmt  ;
  delete h_el_wjets_hghmt ;
  delete h_mu_wjets_hghmt ;
  delete h_lp_wjets_hghmt ;
  delete h_el_ttbar_hghmt ;
  delete h_mu_ttbar_hghmt ;
  delete h_lp_ttbar_hghmt ;
  delete c_R    ;
  delete c_SF2L ;

  return;
} // runDiLep()

void runIsoTrk(){

  // ===== pre-veto region =====

  // inclusive MT plots
  plotMT(preVeto5,"preVeto_OOtB","cr5",SF_unit,SF_unit,SF_unit,SF_unit,SF_unit,SF_unit,SF_unit,SF_unit,SF_unit);

  // === "R" to scale ttbar MC in the MT peak so W+ttbar MC agrees with data in each MET bin

  // fill low MT regions
  TH1F* h_el_data_lowMT_preVeto  = new TH1F("h_el_data_lowMT_preVeto",  "h_el_data_lowMT_preVeto",  nBins, edges );
  TH1F* h_mu_data_lowMT_preVeto  = new TH1F("h_mu_data_lowMT_preVeto",  "h_mu_data_lowMT_preVeto",  nBins, edges );
  TH1F* h_lp_data_lowMT_preVeto  = new TH1F("h_lp_data_lowMT_preVeto",  "h_lp_data_lowMT_preVeto",  nBins, edges );
  TH1F* h_el_wjets_lowMT_preVeto = new TH1F("h_el_wjets_lowMT_preVeto", "h_el_wjets_lowMT_preVeto", nBins, edges );
  TH1F* h_mu_wjets_lowMT_preVeto = new TH1F("h_mu_wjets_lowMT_preVeto", "h_mu_wjets_lowMT_preVeto", nBins, edges );
  TH1F* h_lp_wjets_lowMT_preVeto = new TH1F("h_lp_wjets_lowMT_preVeto", "h_lp_wjets_lowMT_preVeto", nBins, edges );
  TH1F* h_el_ttbar_lowMT_preVeto = new TH1F("h_el_ttbar_lowMT_preVeto", "h_el_ttbar_lowMT_preVeto", nBins, edges );
  TH1F* h_mu_ttbar_lowMT_preVeto = new TH1F("h_mu_ttbar_lowMT_preVeto", "h_mu_ttbar_lowMT_preVeto", nBins, edges );
  TH1F* h_lp_ttbar_lowMT_preVeto = new TH1F("h_lp_ttbar_lowMT_preVeto", "h_lp_ttbar_lowMT_preVeto", nBins, edges );
  t_eldata->Project("h_el_data_lowMT_preVeto" ,"met",dtweight+preVeto5+lowMT+elTrg);
  t_mudata->Project("h_mu_data_lowMT_preVeto" ,"met",dtweight+preVeto5+lowMT+muTrg);
  t_lpdata->Project("h_lp_data_lowMT_preVeto" ,"met",dtweight+preVeto5+lowMT+lpTrg);
  t_wjets ->Project("h_el_wjets_lowMT_preVeto","met",mcweight+preVeto5+lowMT+elTrg);
  t_wjets ->Project("h_mu_wjets_lowMT_preVeto","met",mcweight+preVeto5+lowMT+muTrg);
  t_wjets ->Project("h_lp_wjets_lowMT_preVeto","met",mcweight+preVeto5+lowMT+lpTrg);
  t_ttbar ->Project("h_el_ttbar_lowMT_preVeto","met",mcweight+preVeto5+lowMT+elTrg);
  t_ttbar ->Project("h_mu_ttbar_lowMT_preVeto","met",mcweight+preVeto5+lowMT+muTrg);
  t_ttbar ->Project("h_lp_ttbar_lowMT_preVeto","met",mcweight+preVeto5+lowMT+lpTrg);
  h_el_data_lowMT_preVeto ->SetBinContent( nBins, h_el_data_lowMT_preVeto ->GetBinContent(nBins) + h_el_data_lowMT_preVeto ->GetBinContent(nBins+1) );
  h_mu_data_lowMT_preVeto ->SetBinContent( nBins, h_mu_data_lowMT_preVeto ->GetBinContent(nBins) + h_mu_data_lowMT_preVeto ->GetBinContent(nBins+1) );
  h_lp_data_lowMT_preVeto ->SetBinContent( nBins, h_lp_data_lowMT_preVeto ->GetBinContent(nBins) + h_lp_data_lowMT_preVeto ->GetBinContent(nBins+1) );
  h_el_wjets_lowMT_preVeto->SetBinContent( nBins, h_el_wjets_lowMT_preVeto->GetBinContent(nBins) + h_el_wjets_lowMT_preVeto->GetBinContent(nBins+1) );
  h_mu_wjets_lowMT_preVeto->SetBinContent( nBins, h_mu_wjets_lowMT_preVeto->GetBinContent(nBins) + h_mu_wjets_lowMT_preVeto->GetBinContent(nBins+1) );
  h_lp_wjets_lowMT_preVeto->SetBinContent( nBins, h_lp_wjets_lowMT_preVeto->GetBinContent(nBins) + h_lp_wjets_lowMT_preVeto->GetBinContent(nBins+1) );
  h_el_ttbar_lowMT_preVeto->SetBinContent( nBins, h_el_ttbar_lowMT_preVeto->GetBinContent(nBins) + h_el_ttbar_lowMT_preVeto->GetBinContent(nBins+1) );
  h_mu_ttbar_lowMT_preVeto->SetBinContent( nBins, h_mu_ttbar_lowMT_preVeto->GetBinContent(nBins) + h_mu_ttbar_lowMT_preVeto->GetBinContent(nBins+1) );
  h_lp_ttbar_lowMT_preVeto->SetBinContent( nBins, h_lp_ttbar_lowMT_preVeto->GetBinContent(nBins) + h_lp_ttbar_lowMT_preVeto->GetBinContent(nBins+1) );
  h_el_data_lowMT_preVeto ->SetBinError( nBins, getError(h_el_data_lowMT_preVeto ->GetBinError(nBins),h_el_data_lowMT_preVeto ->GetBinError(nBins+1)) );
  h_mu_data_lowMT_preVeto ->SetBinError( nBins, getError(h_mu_data_lowMT_preVeto ->GetBinError(nBins),h_mu_data_lowMT_preVeto ->GetBinError(nBins+1)) );
  h_lp_data_lowMT_preVeto ->SetBinError( nBins, getError(h_lp_data_lowMT_preVeto ->GetBinError(nBins),h_lp_data_lowMT_preVeto ->GetBinError(nBins+1)) );
  h_el_wjets_lowMT_preVeto->SetBinError( nBins, getError(h_el_wjets_lowMT_preVeto->GetBinError(nBins),h_el_wjets_lowMT_preVeto->GetBinError(nBins+1)) );
  h_mu_wjets_lowMT_preVeto->SetBinError( nBins, getError(h_mu_wjets_lowMT_preVeto->GetBinError(nBins),h_mu_wjets_lowMT_preVeto->GetBinError(nBins+1)) );
  h_lp_wjets_lowMT_preVeto->SetBinError( nBins, getError(h_lp_wjets_lowMT_preVeto->GetBinError(nBins),h_lp_wjets_lowMT_preVeto->GetBinError(nBins+1)) );
  h_el_ttbar_lowMT_preVeto->SetBinError( nBins, getError(h_el_ttbar_lowMT_preVeto->GetBinError(nBins),h_el_ttbar_lowMT_preVeto->GetBinError(nBins+1)) );
  h_mu_ttbar_lowMT_preVeto->SetBinError( nBins, getError(h_mu_ttbar_lowMT_preVeto->GetBinError(nBins),h_mu_ttbar_lowMT_preVeto->GetBinError(nBins+1)) );
  h_lp_ttbar_lowMT_preVeto->SetBinError( nBins, getError(h_lp_ttbar_lowMT_preVeto->GetBinError(nBins),h_lp_ttbar_lowMT_preVeto->GetBinError(nBins+1)) );

  // W-subtracted data
  h_el_data_lowMT_preVeto->Add(h_el_wjets_lowMT_preVeto,-1);
  h_mu_data_lowMT_preVeto->Add(h_mu_wjets_lowMT_preVeto,-1);
  h_lp_data_lowMT_preVeto->Add(h_lp_wjets_lowMT_preVeto,-1);

  TH1F* R_el_preVeto = (TH1F*)h_el_data_lowMT_preVeto->Clone("R_el_preVeto"); R_el_preVeto->Divide(h_el_ttbar_lowMT_preVeto);
  TH1F* R_mu_preVeto = (TH1F*)h_mu_data_lowMT_preVeto->Clone("R_mu_preVeto"); R_mu_preVeto->Divide(h_mu_ttbar_lowMT_preVeto);
  TH1F* R_lp_preVeto = (TH1F*)h_lp_data_lowMT_preVeto->Clone("R_lp_preVeto"); R_lp_preVeto->Divide(h_lp_ttbar_lowMT_preVeto);
  R_el_preVeto->SetLineColor(1); R_el_preVeto->SetLineWidth(3); R_el_preVeto->SetTitle("electrons");
  R_mu_preVeto->SetLineColor(1); R_mu_preVeto->SetLineWidth(3); R_mu_preVeto->SetTitle("muons");
  R_lp_preVeto->SetLineColor(1); R_lp_preVeto->SetLineWidth(3); R_lp_preVeto->SetTitle("both");

  TCanvas * c_R_preVeto = new TCanvas( "c_R_preVeto","c_R_preVeto", 1500, 500 ); c_R_preVeto->Divide(3,1);
  c_R_preVeto->cd(1); R_el_preVeto->Draw(); R_el_preVeto->GetXaxis()->SetTitle("MET");
  c_R_preVeto->cd(2); R_mu_preVeto->Draw(); R_mu_preVeto->GetXaxis()->SetTitle("MET");
  c_R_preVeto->cd(3); R_mu_preVeto->Draw(); R_lp_preVeto->GetXaxis()->SetTitle("MET");
  c_R_preVeto->Update();
  c_R_preVeto->SaveAs("plots/cr5/R_preVeto.png");

  plotMT(preVeto5,"preVeto_ttbarScaled","cr5",SF_unit,SF_unit,SF_unit,R_el_preVeto,R_mu_preVeto,R_lp_preVeto,R_el_preVeto,R_mu_preVeto,R_lp_preVeto);

  // ===== post-veto region =====

  // inclusive MT plots
  plotMT(pstVeto5,"pstVeto_OOtB","cr5",SF_unit,SF_unit,SF_unit,SF_unit,SF_unit,SF_unit,SF_unit,SF_unit,SF_unit);

  // === "R" to scale ttbar MC in the MT peak so W+ttbar MC agrees with data in each MET bin

  // fill low MT regions
  TH1F* h_el_data_lowMT_pstVeto  = new TH1F("h_el_data_lowMT_pstVeto" , "h_el_data_lowMT_pstVeto" , nBins, edges );
  TH1F* h_mu_data_lowMT_pstVeto  = new TH1F("h_mu_data_lowMT_pstVeto" , "h_mu_data_lowMT_pstVeto" , nBins, edges );
  TH1F* h_lp_data_lowMT_pstVeto  = new TH1F("h_lp_data_lowMT_pstVeto" , "h_lp_data_lowMT_pstVeto" , nBins, edges );
  TH1F* h_el_wjets_lowMT_pstVeto = new TH1F("h_el_wjets_lowMT_pstVeto", "h_el_wjets_lowMT_pstVeto", nBins, edges );
  TH1F* h_mu_wjets_lowMT_pstVeto = new TH1F("h_mu_wjets_lowMT_pstVeto", "h_mu_wjets_lowMT_pstVeto", nBins, edges );
  TH1F* h_lp_wjets_lowMT_pstVeto = new TH1F("h_lp_wjets_lowMT_pstVeto", "h_lp_wjets_lowMT_pstVeto", nBins, edges );
  TH1F* h_el_ttbar_lowMT_pstVeto = new TH1F("h_el_ttbar_lowMT_pstVeto", "h_el_ttbar_lowMT_pstVeto", nBins, edges );
  TH1F* h_mu_ttbar_lowMT_pstVeto = new TH1F("h_mu_ttbar_lowMT_pstVeto", "h_mu_ttbar_lowMT_pstVeto", nBins, edges );
  TH1F* h_lp_ttbar_lowMT_pstVeto = new TH1F("h_lp_ttbar_lowMT_pstVeto", "h_lp_ttbar_lowMT_pstVeto", nBins, edges );
  TH1F* h_el_dilep_lowMT_pstVeto = new TH1F("h_el_dilep_lowMT_pstVeto", "h_el_ttbar_dilep_pstVeto", nBins, edges );
  TH1F* h_mu_dilep_lowMT_pstVeto = new TH1F("h_mu_dilep_lowMT_pstVeto", "h_mu_ttbar_dilep_pstVeto", nBins, edges );
  TH1F* h_lp_dilep_lowMT_pstVeto = new TH1F("h_lp_dilep_lowMT_pstVeto", "h_lp_ttbar_dilep_pstVeto", nBins, edges );
  t_eldata->Project("h_el_data_lowMT_pstVeto" ,"met",dtweight+pstVeto5+lowMT        +elTrg);
  t_mudata->Project("h_mu_data_lowMT_pstVeto" ,"met",dtweight+pstVeto5+lowMT        +muTrg);
  t_lpdata->Project("h_lp_data_lowMT_pstVeto" ,"met",dtweight+pstVeto5+lowMT        +lpTrg);
  t_wjets ->Project("h_el_wjets_lowMT_pstVeto","met",mcweight+pstVeto5+lowMT        +elTrg);
  t_wjets ->Project("h_mu_wjets_lowMT_pstVeto","met",mcweight+pstVeto5+lowMT        +muTrg);
  t_wjets ->Project("h_lp_wjets_lowMT_pstVeto","met",mcweight+pstVeto5+lowMT        +lpTrg);
  t_ttbar ->Project("h_el_ttbar_lowMT_pstVeto","met",mcweight+pstVeto5+lowMT+ntDilep+elTrg);
  t_ttbar ->Project("h_mu_ttbar_lowMT_pstVeto","met",mcweight+pstVeto5+lowMT+ntDilep+muTrg);
  t_ttbar ->Project("h_lp_ttbar_lowMT_pstVeto","met",mcweight+pstVeto5+lowMT+ntDilep+lpTrg);
  t_ttbar ->Project("h_el_dilep_lowMT_pstVeto","met",mcweight+pstVeto5+lowMT+isDilep+elTrg);
  t_ttbar ->Project("h_mu_dilep_lowMT_pstVeto","met",mcweight+pstVeto5+lowMT+isDilep+muTrg);
  t_ttbar ->Project("h_lp_dilep_lowMT_pstVeto","met",mcweight+pstVeto5+lowMT+isDilep+lpTrg);
  h_el_data_lowMT_pstVeto ->SetBinContent( nBins, h_el_data_lowMT_pstVeto ->GetBinContent(nBins) + h_el_data_lowMT_pstVeto ->GetBinContent(nBins+1) );
  h_mu_data_lowMT_pstVeto ->SetBinContent( nBins, h_mu_data_lowMT_pstVeto ->GetBinContent(nBins) + h_mu_data_lowMT_pstVeto ->GetBinContent(nBins+1) );
  h_lp_data_lowMT_pstVeto ->SetBinContent( nBins, h_lp_data_lowMT_pstVeto ->GetBinContent(nBins) + h_lp_data_lowMT_pstVeto ->GetBinContent(nBins+1) );
  h_el_wjets_lowMT_pstVeto->SetBinContent( nBins, h_el_wjets_lowMT_pstVeto->GetBinContent(nBins) + h_el_wjets_lowMT_pstVeto->GetBinContent(nBins+1) );
  h_mu_wjets_lowMT_pstVeto->SetBinContent( nBins, h_mu_wjets_lowMT_pstVeto->GetBinContent(nBins) + h_mu_wjets_lowMT_pstVeto->GetBinContent(nBins+1) );
  h_lp_wjets_lowMT_pstVeto->SetBinContent( nBins, h_lp_wjets_lowMT_pstVeto->GetBinContent(nBins) + h_lp_wjets_lowMT_pstVeto->GetBinContent(nBins+1) );
  h_el_ttbar_lowMT_pstVeto->SetBinContent( nBins, h_el_ttbar_lowMT_pstVeto->GetBinContent(nBins) + h_el_ttbar_lowMT_pstVeto->GetBinContent(nBins+1) );
  h_mu_ttbar_lowMT_pstVeto->SetBinContent( nBins, h_mu_ttbar_lowMT_pstVeto->GetBinContent(nBins) + h_mu_ttbar_lowMT_pstVeto->GetBinContent(nBins+1) );
  h_lp_ttbar_lowMT_pstVeto->SetBinContent( nBins, h_lp_ttbar_lowMT_pstVeto->GetBinContent(nBins) + h_lp_ttbar_lowMT_pstVeto->GetBinContent(nBins+1) );
  h_el_dilep_lowMT_pstVeto->SetBinContent( nBins, h_el_dilep_lowMT_pstVeto->GetBinContent(nBins) + h_el_dilep_lowMT_pstVeto->GetBinContent(nBins+1) );
  h_mu_dilep_lowMT_pstVeto->SetBinContent( nBins, h_mu_dilep_lowMT_pstVeto->GetBinContent(nBins) + h_mu_dilep_lowMT_pstVeto->GetBinContent(nBins+1) );
  h_lp_dilep_lowMT_pstVeto->SetBinContent( nBins, h_lp_dilep_lowMT_pstVeto->GetBinContent(nBins) + h_lp_dilep_lowMT_pstVeto->GetBinContent(nBins+1) );
  h_el_data_lowMT_pstVeto ->SetBinError( nBins, getError(h_el_data_lowMT_pstVeto ->GetBinError(nBins),h_el_data_lowMT_pstVeto ->GetBinError(nBins+1)) );
  h_mu_data_lowMT_pstVeto ->SetBinError( nBins, getError(h_mu_data_lowMT_pstVeto ->GetBinError(nBins),h_mu_data_lowMT_pstVeto ->GetBinError(nBins+1)) );
  h_lp_data_lowMT_pstVeto ->SetBinError( nBins, getError(h_lp_data_lowMT_pstVeto ->GetBinError(nBins),h_lp_data_lowMT_pstVeto ->GetBinError(nBins+1)) );
  h_el_wjets_lowMT_pstVeto->SetBinError( nBins, getError(h_el_wjets_lowMT_pstVeto->GetBinError(nBins),h_el_wjets_lowMT_pstVeto->GetBinError(nBins+1)) );
  h_mu_wjets_lowMT_pstVeto->SetBinError( nBins, getError(h_mu_wjets_lowMT_pstVeto->GetBinError(nBins),h_mu_wjets_lowMT_pstVeto->GetBinError(nBins+1)) );
  h_lp_wjets_lowMT_pstVeto->SetBinError( nBins, getError(h_lp_wjets_lowMT_pstVeto->GetBinError(nBins),h_lp_wjets_lowMT_pstVeto->GetBinError(nBins+1)) );
  h_el_ttbar_lowMT_pstVeto->SetBinError( nBins, getError(h_el_ttbar_lowMT_pstVeto->GetBinError(nBins),h_el_ttbar_lowMT_pstVeto->GetBinError(nBins+1)) );
  h_mu_ttbar_lowMT_pstVeto->SetBinError( nBins, getError(h_mu_ttbar_lowMT_pstVeto->GetBinError(nBins),h_mu_ttbar_lowMT_pstVeto->GetBinError(nBins+1)) );
  h_lp_ttbar_lowMT_pstVeto->SetBinError( nBins, getError(h_lp_ttbar_lowMT_pstVeto->GetBinError(nBins),h_lp_ttbar_lowMT_pstVeto->GetBinError(nBins+1)) );
  h_el_dilep_lowMT_pstVeto->SetBinError( nBins, getError(h_el_dilep_lowMT_pstVeto->GetBinError(nBins),h_el_dilep_lowMT_pstVeto->GetBinError(nBins+1)) );
  h_mu_dilep_lowMT_pstVeto->SetBinError( nBins, getError(h_mu_dilep_lowMT_pstVeto->GetBinError(nBins),h_mu_dilep_lowMT_pstVeto->GetBinError(nBins+1)) );
  h_lp_dilep_lowMT_pstVeto->SetBinError( nBins, getError(h_lp_dilep_lowMT_pstVeto->GetBinError(nBins),h_lp_dilep_lowMT_pstVeto->GetBinError(nBins+1)) );

  // scale ttbar dilep by R_lep_preVeto
  h_el_dilep_lowMT_pstVeto->Multiply(R_el_preVeto);
  h_mu_dilep_lowMT_pstVeto->Multiply(R_mu_preVeto);
  h_lp_dilep_lowMT_pstVeto->Multiply(R_lp_preVeto);
  // W and ttbar->dilep subtracted data
  h_el_data_lowMT_pstVeto->Add(h_el_wjets_lowMT_pstVeto,-1); h_el_data_lowMT_pstVeto->Add(h_el_dilep_lowMT_pstVeto,-1);
  h_mu_data_lowMT_pstVeto->Add(h_mu_wjets_lowMT_pstVeto,-1); h_mu_data_lowMT_pstVeto->Add(h_mu_dilep_lowMT_pstVeto,-1);
  h_lp_data_lowMT_pstVeto->Add(h_lp_wjets_lowMT_pstVeto,-1); h_lp_data_lowMT_pstVeto->Add(h_lp_dilep_lowMT_pstVeto,-1);

  TH1F* R_el_pstVeto = (TH1F*)h_el_data_lowMT_pstVeto->Clone("R_el_pstVeto"); R_el_pstVeto->Divide(h_el_ttbar_lowMT_pstVeto);
  TH1F* R_mu_pstVeto = (TH1F*)h_mu_data_lowMT_pstVeto->Clone("R_mu_pstVeto"); R_mu_pstVeto->Divide(h_mu_ttbar_lowMT_pstVeto);
  TH1F* R_lp_pstVeto = (TH1F*)h_lp_data_lowMT_pstVeto->Clone("R_lp_pstVeto"); R_lp_pstVeto->Divide(h_lp_ttbar_lowMT_pstVeto);
  R_el_pstVeto->SetLineColor(1); R_el_pstVeto->SetLineWidth(3); R_el_pstVeto->SetTitle("electrons");
  R_mu_pstVeto->SetLineColor(1); R_mu_pstVeto->SetLineWidth(3); R_mu_pstVeto->SetTitle("muons");
  R_lp_pstVeto->SetLineColor(1); R_lp_pstVeto->SetLineWidth(3); R_lp_pstVeto->SetTitle("both");

  TCanvas * c_R_pstVeto = new TCanvas( "c_R_pstVeto","c_R_pstVeto", 1500, 500 ); c_R_pstVeto->Divide(3,1);
  c_R_pstVeto->cd(1); R_el_pstVeto->Draw(); R_el_pstVeto->GetXaxis()->SetTitle("MET");
  c_R_pstVeto->cd(2); R_mu_pstVeto->Draw(); R_mu_pstVeto->GetXaxis()->SetTitle("MET");
  c_R_pstVeto->cd(3); R_mu_pstVeto->Draw(); R_lp_pstVeto->GetXaxis()->SetTitle("MET");
  c_R_pstVeto->Update();
  c_R_pstVeto->SaveAs("plots/cr5/R_pstVeto.png");

  plotMT(pstVeto5,"pstVeto_ttbarScaled","cr5",SF_unit,SF_unit,SF_unit,R_el_pstVeto,R_mu_pstVeto,R_lp_pstVeto,R_el_preVeto,R_mu_preVeto,R_lp_preVeto);

  delete h_el_data_lowMT_preVeto  ;
  delete h_mu_data_lowMT_preVeto  ;
  delete h_lp_data_lowMT_preVeto  ;
  delete h_el_wjets_lowMT_preVeto ;
  delete h_mu_wjets_lowMT_preVeto ;
  delete h_lp_wjets_lowMT_preVeto ;
  delete h_el_ttbar_lowMT_preVeto ;
  delete h_mu_ttbar_lowMT_preVeto ;
  delete h_lp_ttbar_lowMT_preVeto ;
  delete h_el_data_lowMT_pstVeto  ;
  delete h_mu_data_lowMT_pstVeto  ;
  delete h_lp_data_lowMT_pstVeto  ;
  delete h_el_wjets_lowMT_pstVeto ;
  delete h_mu_wjets_lowMT_pstVeto ;
  delete h_lp_wjets_lowMT_pstVeto ;
  delete h_el_ttbar_lowMT_pstVeto ;
  delete h_mu_ttbar_lowMT_pstVeto ;
  delete h_lp_ttbar_lowMT_pstVeto ;
  delete h_el_dilep_lowMT_pstVeto ;
  delete h_mu_dilep_lowMT_pstVeto ;
  delete h_lp_dilep_lowMT_pstVeto ;
  delete c_R_preVeto              ;
  delete c_R_pstVeto              ;
  return;
} // runIsoTrk()

void runPrediction(TH1F* &SFW_el, TH1F* &SFW_mu, TH1F* &SFW_lp, TH1F* &SF2L_el, TH1F* &SF2L_mu, TH1F* &SF2L_lp) {

  // ===== Calc various scale factors (sec 11) =====

  // inclusive MT plots for inclusive B region
  plotMT(incBCuts,"incB_OOtB","",SF_unit,SF_unit,SF_unit,SF_unit,SF_unit,SF_unit,SF_unit,SF_unit,SF_unit);

  // fill low MT regions
  TH1F* h_el_wjets_incB_lowMT = new TH1F("h_el_wjets_incB_lowMT", "h_el_wjets_incB_lowMT", nBins, edges );
  TH1F* h_mu_wjets_incB_lowMT = new TH1F("h_mu_wjets_incB_lowMT", "h_mu_wjets_incB_lowMT", nBins, edges );
  TH1F* h_lp_wjets_incB_lowMT = new TH1F("h_lp_wjets_incB_lowMT", "h_lp_wjets_incB_lowMT", nBins, edges );
  TH1F* h_el_ttbar_incB_lowMT = new TH1F("h_el_ttbar_incB_lowMT", "h_el_ttbar_incB_lowMT", nBins, edges );
  TH1F* h_mu_ttbar_incB_lowMT = new TH1F("h_mu_ttbar_incB_lowMT", "h_mu_ttbar_incB_lowMT", nBins, edges );
  TH1F* h_lp_ttbar_incB_lowMT = new TH1F("h_lp_ttbar_incB_lowMT", "h_lp_ttbar_incB_lowMT", nBins, edges );
  TH1F* h_el_dilep_incB_lowMT = new TH1F("h_el_dilep_incB_lowMT", "h_el_dilep_incB_lowMT", nBins, edges );
  TH1F* h_mu_dilep_incB_lowMT = new TH1F("h_mu_dilep_incB_lowMT", "h_mu_dilep_incB_lowMT", nBins, edges );
  TH1F* h_lp_dilep_incB_lowMT = new TH1F("h_lp_dilep_incB_lowMT", "h_lp_dilep_incB_lowMT", nBins, edges );
  t_wjets ->Project("h_el_wjets_incB_lowMT","met",mcweight+incBCuts        +lowMT+elTrg);
  t_wjets ->Project("h_mu_wjets_incB_lowMT","met",mcweight+incBCuts        +lowMT+muTrg);
  t_wjets ->Project("h_lp_wjets_incB_lowMT","met",mcweight+incBCuts        +lowMT+lpTrg);
  t_ttbar ->Project("h_el_ttbar_incB_lowMT","met",mcweight+incBCuts+ntDilep+lowMT+elTrg);
  t_ttbar ->Project("h_mu_ttbar_incB_lowMT","met",mcweight+incBCuts+ntDilep+lowMT+muTrg);
  t_ttbar ->Project("h_lp_ttbar_incB_lowMT","met",mcweight+incBCuts+ntDilep+lowMT+lpTrg);
  t_ttbar ->Project("h_el_dilep_incB_lowMT","met",mcweight+incBCuts+isDilep+lowMT+elTrg);
  t_ttbar ->Project("h_mu_dilep_incB_lowMT","met",mcweight+incBCuts+isDilep+lowMT+muTrg);
  t_ttbar ->Project("h_lp_dilep_incB_lowMT","met",mcweight+incBCuts+isDilep+lowMT+lpTrg);
  h_el_wjets_incB_lowMT->SetBinContent( nBins, h_el_wjets_incB_lowMT->GetBinContent(nBins) + h_el_wjets_incB_lowMT->GetBinContent(nBins+1) );
  h_mu_wjets_incB_lowMT->SetBinContent( nBins, h_mu_wjets_incB_lowMT->GetBinContent(nBins) + h_mu_wjets_incB_lowMT->GetBinContent(nBins+1) );
  h_lp_wjets_incB_lowMT->SetBinContent( nBins, h_lp_wjets_incB_lowMT->GetBinContent(nBins) + h_lp_wjets_incB_lowMT->GetBinContent(nBins+1) );
  h_el_ttbar_incB_lowMT->SetBinContent( nBins, h_el_ttbar_incB_lowMT->GetBinContent(nBins) + h_el_ttbar_incB_lowMT->GetBinContent(nBins+1) );
  h_mu_ttbar_incB_lowMT->SetBinContent( nBins, h_mu_ttbar_incB_lowMT->GetBinContent(nBins) + h_mu_ttbar_incB_lowMT->GetBinContent(nBins+1) );
  h_lp_ttbar_incB_lowMT->SetBinContent( nBins, h_lp_ttbar_incB_lowMT->GetBinContent(nBins) + h_lp_ttbar_incB_lowMT->GetBinContent(nBins+1) );
  h_el_dilep_incB_lowMT->SetBinContent( nBins, h_el_dilep_incB_lowMT->GetBinContent(nBins) + h_el_dilep_incB_lowMT->GetBinContent(nBins+1) );
  h_mu_dilep_incB_lowMT->SetBinContent( nBins, h_mu_dilep_incB_lowMT->GetBinContent(nBins) + h_mu_dilep_incB_lowMT->GetBinContent(nBins+1) );
  h_lp_dilep_incB_lowMT->SetBinContent( nBins, h_lp_dilep_incB_lowMT->GetBinContent(nBins) + h_lp_dilep_incB_lowMT->GetBinContent(nBins+1) );
  h_el_wjets_incB_lowMT->SetBinError( nBins, getError(h_el_wjets_incB_lowMT->GetBinError(nBins),h_el_wjets_incB_lowMT->GetBinError(nBins+1)) );
  h_mu_wjets_incB_lowMT->SetBinError( nBins, getError(h_mu_wjets_incB_lowMT->GetBinError(nBins),h_mu_wjets_incB_lowMT->GetBinError(nBins+1)) );
  h_lp_wjets_incB_lowMT->SetBinError( nBins, getError(h_lp_wjets_incB_lowMT->GetBinError(nBins),h_lp_wjets_incB_lowMT->GetBinError(nBins+1)) );
  h_el_ttbar_incB_lowMT->SetBinError( nBins, getError(h_el_ttbar_incB_lowMT->GetBinError(nBins),h_el_ttbar_incB_lowMT->GetBinError(nBins+1)) );
  h_mu_ttbar_incB_lowMT->SetBinError( nBins, getError(h_mu_ttbar_incB_lowMT->GetBinError(nBins),h_mu_ttbar_incB_lowMT->GetBinError(nBins+1)) );
  h_lp_ttbar_incB_lowMT->SetBinError( nBins, getError(h_lp_ttbar_incB_lowMT->GetBinError(nBins),h_lp_ttbar_incB_lowMT->GetBinError(nBins+1)) );
  h_el_dilep_incB_lowMT->SetBinError( nBins, getError(h_el_dilep_incB_lowMT->GetBinError(nBins),h_el_dilep_incB_lowMT->GetBinError(nBins+1)) );
  h_mu_dilep_incB_lowMT->SetBinError( nBins, getError(h_mu_dilep_incB_lowMT->GetBinError(nBins),h_mu_dilep_incB_lowMT->GetBinError(nBins+1)) );
  h_lp_dilep_incB_lowMT->SetBinError( nBins, getError(h_lp_dilep_incB_lowMT->GetBinError(nBins),h_lp_dilep_incB_lowMT->GetBinError(nBins+1)) );

  // fill high MT regions
  TH1F* h_el_wjets_incB_hghMT = new TH1F("h_el_wjets_incB_hghMT", "h_el_wjets_incB_hghMT", nBins, edges );
  TH1F* h_mu_wjets_incB_hghMT = new TH1F("h_mu_wjets_incB_hghMT", "h_mu_wjets_incB_hghMT", nBins, edges );
  TH1F* h_lp_wjets_incB_hghMT = new TH1F("h_lp_wjets_incB_hghMT", "h_lp_wjets_incB_hghMT", nBins, edges );
  TH1F* h_el_ttbar_incB_hghMT = new TH1F("h_el_ttbar_incB_hghMT", "h_el_ttbar_incB_hghMT", nBins, edges );
  TH1F* h_mu_ttbar_incB_hghMT = new TH1F("h_mu_ttbar_incB_hghMT", "h_mu_ttbar_incB_hghMT", nBins, edges );
  TH1F* h_lp_ttbar_incB_hghMT = new TH1F("h_lp_ttbar_incB_hghMT", "h_lp_ttbar_incB_hghMT", nBins, edges );
  TH1F* h_el_dilep_incB_hghMT = new TH1F("h_el_dilep_incB_hghMT", "h_el_dilep_incB_hghMT", nBins, edges );
  TH1F* h_mu_dilep_incB_hghMT = new TH1F("h_mu_dilep_incB_hghMT", "h_mu_dilep_incB_hghMT", nBins, edges );
  TH1F* h_lp_dilep_incB_hghMT = new TH1F("h_lp_dilep_incB_hghMT", "h_lp_dilep_incB_hghMT", nBins, edges );
  t_wjets ->Project("h_el_wjets_incB_hghMT","met",mcweight+incBCuts        +hghMT+elTrg);
  t_wjets ->Project("h_mu_wjets_incB_hghMT","met",mcweight+incBCuts        +hghMT+muTrg);
  t_wjets ->Project("h_lp_wjets_incB_hghMT","met",mcweight+incBCuts        +hghMT+lpTrg);
  t_ttbar ->Project("h_el_ttbar_incB_hghMT","met",mcweight+incBCuts+ntDilep+hghMT+elTrg);
  t_ttbar ->Project("h_mu_ttbar_incB_hghMT","met",mcweight+incBCuts+ntDilep+hghMT+muTrg);
  t_ttbar ->Project("h_lp_ttbar_incB_hghMT","met",mcweight+incBCuts+ntDilep+hghMT+lpTrg);
  t_ttbar ->Project("h_el_dilep_incB_hghMT","met",mcweight+incBCuts+isDilep+hghMT+elTrg);
  t_ttbar ->Project("h_mu_dilep_incB_hghMT","met",mcweight+incBCuts+isDilep+hghMT+muTrg);
  t_ttbar ->Project("h_lp_dilep_incB_hghMT","met",mcweight+incBCuts+isDilep+hghMT+lpTrg);
  h_el_wjets_incB_hghMT->SetBinContent( nBins, h_el_wjets_incB_hghMT->GetBinContent(nBins) + h_el_wjets_incB_hghMT->GetBinContent(nBins+1) );
  h_mu_wjets_incB_hghMT->SetBinContent( nBins, h_mu_wjets_incB_hghMT->GetBinContent(nBins) + h_mu_wjets_incB_hghMT->GetBinContent(nBins+1) );
  h_lp_wjets_incB_hghMT->SetBinContent( nBins, h_lp_wjets_incB_hghMT->GetBinContent(nBins) + h_lp_wjets_incB_hghMT->GetBinContent(nBins+1) );
  h_el_ttbar_incB_hghMT->SetBinContent( nBins, h_el_ttbar_incB_hghMT->GetBinContent(nBins) + h_el_ttbar_incB_hghMT->GetBinContent(nBins+1) );
  h_mu_ttbar_incB_hghMT->SetBinContent( nBins, h_mu_ttbar_incB_hghMT->GetBinContent(nBins) + h_mu_ttbar_incB_hghMT->GetBinContent(nBins+1) );
  h_lp_ttbar_incB_hghMT->SetBinContent( nBins, h_lp_ttbar_incB_hghMT->GetBinContent(nBins) + h_lp_ttbar_incB_hghMT->GetBinContent(nBins+1) );
  h_el_dilep_incB_hghMT->SetBinContent( nBins, h_el_dilep_incB_hghMT->GetBinContent(nBins) + h_el_dilep_incB_hghMT->GetBinContent(nBins+1) );
  h_mu_dilep_incB_hghMT->SetBinContent( nBins, h_mu_dilep_incB_hghMT->GetBinContent(nBins) + h_mu_dilep_incB_hghMT->GetBinContent(nBins+1) );
  h_lp_dilep_incB_hghMT->SetBinContent( nBins, h_lp_dilep_incB_hghMT->GetBinContent(nBins) + h_lp_dilep_incB_hghMT->GetBinContent(nBins+1) );
  h_el_wjets_incB_hghMT->SetBinError( nBins, getError(h_el_wjets_incB_hghMT->GetBinError(nBins),h_el_wjets_incB_hghMT->GetBinError(nBins+1)) );
  h_mu_wjets_incB_hghMT->SetBinError( nBins, getError(h_mu_wjets_incB_hghMT->GetBinError(nBins),h_mu_wjets_incB_hghMT->GetBinError(nBins+1)) );
  h_lp_wjets_incB_hghMT->SetBinError( nBins, getError(h_lp_wjets_incB_hghMT->GetBinError(nBins),h_lp_wjets_incB_hghMT->GetBinError(nBins+1)) );
  h_el_ttbar_incB_hghMT->SetBinError( nBins, getError(h_el_ttbar_incB_hghMT->GetBinError(nBins),h_el_ttbar_incB_hghMT->GetBinError(nBins+1)) );
  h_mu_ttbar_incB_hghMT->SetBinError( nBins, getError(h_mu_ttbar_incB_hghMT->GetBinError(nBins),h_mu_ttbar_incB_hghMT->GetBinError(nBins+1)) );
  h_lp_ttbar_incB_hghMT->SetBinError( nBins, getError(h_lp_ttbar_incB_hghMT->GetBinError(nBins),h_lp_ttbar_incB_hghMT->GetBinError(nBins+1)) );
  h_el_dilep_incB_hghMT->SetBinError( nBins, getError(h_el_dilep_incB_hghMT->GetBinError(nBins),h_el_dilep_incB_hghMT->GetBinError(nBins+1)) );
  h_mu_dilep_incB_hghMT->SetBinError( nBins, getError(h_mu_dilep_incB_hghMT->GetBinError(nBins),h_mu_dilep_incB_hghMT->GetBinError(nBins+1)) );
  h_lp_dilep_incB_hghMT->SetBinError( nBins, getError(h_lp_dilep_incB_hghMT->GetBinError(nBins),h_lp_dilep_incB_hghMT->GetBinError(nBins+1)) );

  // Rmcw, Rmct = (mt tail)/(mt peak) for w,ttbar1L
  TH1F* Rmcw_el = (TH1F*)h_el_wjets_incB_hghMT->Clone("Rmcw_el"); Rmcw_el->Divide(h_el_wjets_incB_lowMT);
  TH1F* Rmcw_mu = (TH1F*)h_mu_wjets_incB_hghMT->Clone("Rmcw_mu"); Rmcw_mu->Divide(h_mu_wjets_incB_lowMT);
  TH1F* Rmcw_lp = (TH1F*)h_lp_wjets_incB_hghMT->Clone("Rmcw_lp"); Rmcw_lp->Divide(h_lp_wjets_incB_lowMT);
  TH1F* Rmct_el = (TH1F*)h_el_ttbar_incB_hghMT->Clone("Rmct_el"); Rmct_el->Divide(h_el_ttbar_incB_lowMT);
  TH1F* Rmct_mu = (TH1F*)h_mu_ttbar_incB_hghMT->Clone("Rmct_mu"); Rmct_mu->Divide(h_mu_ttbar_incB_lowMT);
  TH1F* Rmct_lp = (TH1F*)h_lp_ttbar_incB_hghMT->Clone("Rmct_lp"); Rmct_lp->Divide(h_lp_ttbar_incB_lowMT);
  Rmcw_el->SetLineColor(1); Rmcw_el->SetLineWidth(3); Rmcw_el->SetTitle("electrons");
  Rmcw_mu->SetLineColor(1); Rmcw_mu->SetLineWidth(3); Rmcw_mu->SetTitle("muons");
  Rmcw_lp->SetLineColor(1); Rmcw_lp->SetLineWidth(3); Rmcw_lp->SetTitle("both");
  Rmct_el->SetLineColor(1); Rmct_el->SetLineWidth(3); Rmct_el->SetTitle("electrons");
  Rmct_mu->SetLineColor(1); Rmct_mu->SetLineWidth(3); Rmct_mu->SetTitle("muons");
  Rmct_lp->SetLineColor(1); Rmct_lp->SetLineWidth(3); Rmct_lp->SetTitle("both");
  TCanvas * c_Rmcw = new TCanvas( "c_Rmcw","c_Rmcw", 1500, 500 ); c_Rmcw->Divide(3,1);
  c_Rmcw->cd(1); Rmcw_el->Draw(); Rmcw_el->GetXaxis()->SetTitle("MET");
  c_Rmcw->cd(2); Rmcw_mu->Draw(); Rmcw_mu->GetXaxis()->SetTitle("MET");
  c_Rmcw->cd(3); Rmcw_lp->Draw(); Rmcw_lp->GetXaxis()->SetTitle("MET");
  c_Rmcw->Update();
  c_Rmcw->SaveAs("plots/Rmcw.png");
  TCanvas * c_Rmct = new TCanvas( "c_Rmct","c_Rmct", 1500, 500 ); c_Rmct->Divide(3,1);
  c_Rmct->cd(1); Rmct_el->Draw(); Rmct_el->GetXaxis()->SetTitle("MET");
  c_Rmct->cd(2); Rmct_mu->Draw(); Rmct_mu->GetXaxis()->SetTitle("MET");
  c_Rmct->cd(3); Rmct_lp->Draw(); Rmct_lp->GetXaxis()->SetTitle("MET");
  c_Rmct->Update();
  c_Rmct->SaveAs("plots/Rmct.png");

  // Rwjet = Rmcw * SFW (also pessimistic Rtop)
  TH1F* Rwjet_el = (TH1F*)Rmcw_el->Clone("Rwjet_el"); Rwjet_el->Multiply(SFW_el);
  TH1F* Rwjet_mu = (TH1F*)Rmcw_mu->Clone("Rwjet_mu"); Rwjet_mu->Multiply(SFW_mu);
  TH1F* Rwjet_lp = (TH1F*)Rmcw_lp->Clone("Rwjet_lp"); Rwjet_lp->Multiply(SFW_lp);
  Rwjet_el->SetLineColor(1); Rwjet_el->SetLineWidth(3); Rwjet_el->SetTitle("electrons");
  Rwjet_mu->SetLineColor(1); Rwjet_mu->SetLineWidth(3); Rwjet_mu->SetTitle("muons");
  Rwjet_lp->SetLineColor(1); Rwjet_lp->SetLineWidth(3); Rwjet_lp->SetTitle("muons");
  TCanvas * c_Rwjet = new TCanvas( "c_Rwjet","c_Rwjet", 1500, 500 ); c_Rwjet->Divide(3,1);
  c_Rwjet->cd(1); Rwjet_el->Draw(); Rwjet_el->GetXaxis()->SetTitle("MET");
  c_Rwjet->cd(2); Rwjet_mu->Draw(); Rwjet_mu->GetXaxis()->SetTitle("MET");
  c_Rwjet->cd(3); Rwjet_lp->Draw(); Rwjet_lp->GetXaxis()->SetTitle("MET");
  c_Rwjet->Update();
  c_Rwjet->SaveAs("plots/Rwjet.png");

  // Rtop_opt = Rmct * SFW (optimistic Rtop)
  TH1F* Rtop_el_opt = (TH1F*)Rmct_el->Clone("Rtop_el_opt"); Rtop_el_opt->Multiply(SFW_el);
  TH1F* Rtop_mu_opt = (TH1F*)Rmct_mu->Clone("Rtop_mu_opt"); Rtop_mu_opt->Multiply(SFW_mu);
  TH1F* Rtop_lp_opt = (TH1F*)Rmct_lp->Clone("Rtop_lp_opt"); Rtop_lp_opt->Multiply(SFW_lp);
  Rtop_el_opt->SetLineColor(1); Rtop_el_opt->SetLineWidth(3); Rtop_el_opt->SetTitle("electrons");
  Rtop_mu_opt->SetLineColor(1); Rtop_mu_opt->SetLineWidth(3); Rtop_mu_opt->SetTitle("muons");
  Rtop_lp_opt->SetLineColor(1); Rtop_lp_opt->SetLineWidth(3); Rtop_lp_opt->SetTitle("both");
  TCanvas * c_Rtop_opt = new TCanvas( "c_Rtop_opt","c_Rtop_opt", 1500, 500 ); c_Rtop_opt->Divide(3,1);
  c_Rtop_opt->cd(1); Rtop_el_opt->Draw(); Rtop_el_opt->GetXaxis()->SetTitle("MET");
  c_Rtop_opt->cd(2); Rtop_mu_opt->Draw(); Rtop_mu_opt->GetXaxis()->SetTitle("MET");
  c_Rtop_opt->cd(3); Rtop_lp_opt->Draw(); Rtop_lp_opt->GetXaxis()->SetTitle("MET");
  c_Rtop_opt->Update();
  c_Rtop_opt->SaveAs("plots/Rtop_opt.png");

  // Rtop = averate of Rwjet, Rtop_opt
  TH1F* Rtop_el = (TH1F*)Rtop_el_opt->Clone("Rtop_el"); Rtop_el->SetTitle("electrons");
  TH1F* Rtop_mu = (TH1F*)Rtop_mu_opt->Clone("Rtop_mu"); Rtop_mu->SetTitle("muons");
  TH1F* Rtop_lp = (TH1F*)Rtop_lp_opt->Clone("Rtop_lp"); Rtop_mu->SetTitle("both");
  for(int i=1; i<=nBins; ++i){
    Rtop_el->SetBinContent( i, (Rwjet_el->GetBinContent(i)+Rtop_el_opt->GetBinContent(i))/2.0 );
    Rtop_mu->SetBinContent( i, (Rwjet_mu->GetBinContent(i)+Rtop_mu_opt->GetBinContent(i))/2.0 );
    Rtop_lp->SetBinContent( i, (Rwjet_lp->GetBinContent(i)+Rtop_lp_opt->GetBinContent(i))/2.0 );
    double el_err_wjet = Rwjet_el   ->GetBinError(i);
    double el_err_top  = Rtop_el_opt->GetBinError(i);
    double el_err = sqrt( el_err_wjet*el_err_wjet + el_err_top*el_err_top);
    Rtop_el->SetBinError(i,el_err);
    double mu_err_wjet = Rwjet_mu   ->GetBinError(i);
    double mu_err_top  = Rtop_mu_opt->GetBinError(i);
    double mu_err = sqrt( mu_err_wjet*mu_err_wjet + mu_err_top*mu_err_top);
    Rtop_mu->SetBinError(i,mu_err);
    double lp_err_wjet = Rwjet_lp   ->GetBinError(i);
    double lp_err_top  = Rtop_lp_opt->GetBinError(i);
    double lp_err = sqrt( lp_err_wjet*lp_err_wjet + lp_err_top*lp_err_top);
    Rtop_lp->SetBinError(i,lp_err);
  } // nBins
  TCanvas * c_Rtop = new TCanvas( "c_Rtop","c_Rtop", 1500, 500 ); c_Rtop->Divide(3,1);
  c_Rtop->cd(1); Rtop_el->Draw(); Rtop_el->GetXaxis()->SetTitle("MET");
  c_Rtop->cd(2); Rtop_mu->Draw(); Rtop_mu->GetXaxis()->SetTitle("MET");
  c_Rtop->cd(3); Rtop_lp->Draw(); Rtop_lp->GetXaxis()->SetTitle("MET");
  c_Rtop->Update();
  c_Rtop->SaveAs("plots/Rtop.png");

  // SFtop = Rtop/Rmct
  TH1F* SFtop_el = (TH1F*)Rtop_el->Clone("SFtop_el"); SFtop_el->Divide(Rmct_el); SFtop_el->SetTitle("electrons");
  TH1F* SFtop_mu = (TH1F*)Rtop_mu->Clone("SFtop_mu"); SFtop_mu->Divide(Rmct_mu); SFtop_mu->SetTitle("muons");
  TH1F* SFtop_lp = (TH1F*)Rtop_lp->Clone("SFtop_lp"); SFtop_lp->Divide(Rmct_lp); SFtop_lp->SetTitle("both");
  TCanvas * c_SFtop = new TCanvas( "c_SFtop","c_SFtop", 1500, 500 ); c_SFtop->Divide(3,1);
  c_SFtop->cd(1); SFtop_el->Draw(); SFtop_el->GetXaxis()->SetTitle("MET");
  c_SFtop->cd(2); SFtop_mu->Draw(); SFtop_mu->GetXaxis()->SetTitle("MET");
  c_SFtop->cd(3); SFtop_lp->Draw(); SFtop_lp->GetXaxis()->SetTitle("MET");
  c_SFtop->Update();
  c_SFtop->SaveAs("plots/SFtop.png");

  // ===== Calc prediction (sec 12) =====

  // === preVeto (dilep)

  plotMT(preVeto5,"preVeto_wjttb_OOtB","",SF_unit,SF_unit,SF_unit,SF_unit,SF_unit,SF_unit,SF_unit,SF_unit,SF_unit);

  // fill low MT regions
  TH1F* h_el_data_lowMT_preVeto_pred  = new TH1F("h_el_data_lowMT_preVeto_pred",  "h_el_data_lowMT_preVeto_pred",  nBins, edges );
  TH1F* h_mu_data_lowMT_preVeto_pred  = new TH1F("h_mu_data_lowMT_preVeto_pred",  "h_mu_data_lowMT_preVeto_pred",  nBins, edges );
  TH1F* h_lp_data_lowMT_preVeto_pred  = new TH1F("h_lp_data_lowMT_preVeto_pred",  "h_lp_data_lowMT_preVeto_pred",  nBins, edges );
  TH1F* h_el_wjets_lowMT_preVeto_pred = new TH1F("h_el_wjets_lowMT_preVeto_pred", "h_el_wjets_lowMT_preVeto_pred", nBins, edges );
  TH1F* h_mu_wjets_lowMT_preVeto_pred = new TH1F("h_mu_wjets_lowMT_preVeto_pred", "h_mu_wjets_lowMT_preVeto_pred", nBins, edges );
  TH1F* h_lp_wjets_lowMT_preVeto_pred = new TH1F("h_lp_wjets_lowMT_preVeto_pred", "h_lp_wjets_lowMT_preVeto_pred", nBins, edges );
  TH1F* h_el_ttbar_lowMT_preVeto_pred = new TH1F("h_el_ttbar_lowMT_preVeto_pred", "h_el_ttbar_lowMT_preVeto_pred", nBins, edges );
  TH1F* h_mu_ttbar_lowMT_preVeto_pred = new TH1F("h_mu_ttbar_lowMT_preVeto_pred", "h_mu_ttbar_lowMT_preVeto_pred", nBins, edges );
  TH1F* h_lp_ttbar_lowMT_preVeto_pred = new TH1F("h_lp_ttbar_lowMT_preVeto_pred", "h_lp_ttbar_lowMT_preVeto_pred", nBins, edges );
  t_eldata->Project("h_el_data_lowMT_preVeto_pred" ,"met",dtweight+preVeto5+lowMT+elTrg);
  t_mudata->Project("h_mu_data_lowMT_preVeto_pred" ,"met",dtweight+preVeto5+lowMT+muTrg);
  t_lpdata->Project("h_lp_data_lowMT_preVeto_pred" ,"met",dtweight+preVeto5+lowMT+lpTrg);
  t_wjets ->Project("h_el_wjets_lowMT_preVeto_pred","met",mcweight+preVeto5+lowMT+elTrg);
  t_wjets ->Project("h_mu_wjets_lowMT_preVeto_pred","met",mcweight+preVeto5+lowMT+muTrg);
  t_wjets ->Project("h_lp_wjets_lowMT_preVeto_pred","met",mcweight+preVeto5+lowMT+lpTrg);
  t_ttbar ->Project("h_el_ttbar_lowMT_preVeto_pred","met",mcweight+preVeto5+lowMT+elTrg);
  t_ttbar ->Project("h_mu_ttbar_lowMT_preVeto_pred","met",mcweight+preVeto5+lowMT+muTrg);
  t_ttbar ->Project("h_lp_ttbar_lowMT_preVeto_pred","met",mcweight+preVeto5+lowMT+lpTrg);
  h_el_data_lowMT_preVeto_pred ->SetBinContent( nBins, h_el_data_lowMT_preVeto_pred ->GetBinContent(nBins) + h_el_data_lowMT_preVeto_pred ->GetBinContent(nBins+1) );
  h_mu_data_lowMT_preVeto_pred ->SetBinContent( nBins, h_mu_data_lowMT_preVeto_pred ->GetBinContent(nBins) + h_mu_data_lowMT_preVeto_pred ->GetBinContent(nBins+1) );
  h_lp_data_lowMT_preVeto_pred ->SetBinContent( nBins, h_lp_data_lowMT_preVeto_pred ->GetBinContent(nBins) + h_lp_data_lowMT_preVeto_pred ->GetBinContent(nBins+1) );
  h_el_wjets_lowMT_preVeto_pred->SetBinContent( nBins, h_el_wjets_lowMT_preVeto_pred->GetBinContent(nBins) + h_el_wjets_lowMT_preVeto_pred->GetBinContent(nBins+1) );
  h_mu_wjets_lowMT_preVeto_pred->SetBinContent( nBins, h_mu_wjets_lowMT_preVeto_pred->GetBinContent(nBins) + h_mu_wjets_lowMT_preVeto_pred->GetBinContent(nBins+1) );
  h_lp_wjets_lowMT_preVeto_pred->SetBinContent( nBins, h_lp_wjets_lowMT_preVeto_pred->GetBinContent(nBins) + h_lp_wjets_lowMT_preVeto_pred->GetBinContent(nBins+1) );
  h_el_ttbar_lowMT_preVeto_pred->SetBinContent( nBins, h_el_ttbar_lowMT_preVeto_pred->GetBinContent(nBins) + h_el_ttbar_lowMT_preVeto_pred->GetBinContent(nBins+1) );
  h_mu_ttbar_lowMT_preVeto_pred->SetBinContent( nBins, h_mu_ttbar_lowMT_preVeto_pred->GetBinContent(nBins) + h_mu_ttbar_lowMT_preVeto_pred->GetBinContent(nBins+1) );
  h_lp_ttbar_lowMT_preVeto_pred->SetBinContent( nBins, h_lp_ttbar_lowMT_preVeto_pred->GetBinContent(nBins) + h_lp_ttbar_lowMT_preVeto_pred->GetBinContent(nBins+1) );
  h_el_data_lowMT_preVeto_pred ->SetBinError( nBins, getError(h_el_data_lowMT_preVeto_pred ->GetBinError(nBins),h_el_data_lowMT_preVeto_pred ->GetBinError(nBins+1)) );
  h_mu_data_lowMT_preVeto_pred ->SetBinError( nBins, getError(h_mu_data_lowMT_preVeto_pred ->GetBinError(nBins),h_mu_data_lowMT_preVeto_pred ->GetBinError(nBins+1)) );
  h_lp_data_lowMT_preVeto_pred ->SetBinError( nBins, getError(h_lp_data_lowMT_preVeto_pred ->GetBinError(nBins),h_lp_data_lowMT_preVeto_pred ->GetBinError(nBins+1)) );
  h_el_wjets_lowMT_preVeto_pred->SetBinError( nBins, getError(h_el_wjets_lowMT_preVeto_pred->GetBinError(nBins),h_el_wjets_lowMT_preVeto_pred->GetBinError(nBins+1)) );
  h_mu_wjets_lowMT_preVeto_pred->SetBinError( nBins, getError(h_mu_wjets_lowMT_preVeto_pred->GetBinError(nBins),h_mu_wjets_lowMT_preVeto_pred->GetBinError(nBins+1)) );
  h_lp_wjets_lowMT_preVeto_pred->SetBinError( nBins, getError(h_lp_wjets_lowMT_preVeto_pred->GetBinError(nBins),h_lp_wjets_lowMT_preVeto_pred->GetBinError(nBins+1)) );
  h_el_ttbar_lowMT_preVeto_pred->SetBinError( nBins, getError(h_el_ttbar_lowMT_preVeto_pred->GetBinError(nBins),h_el_ttbar_lowMT_preVeto_pred->GetBinError(nBins+1)) );
  h_mu_ttbar_lowMT_preVeto_pred->SetBinError( nBins, getError(h_mu_ttbar_lowMT_preVeto_pred->GetBinError(nBins),h_mu_ttbar_lowMT_preVeto_pred->GetBinError(nBins+1)) );
  h_lp_ttbar_lowMT_preVeto_pred->SetBinError( nBins, getError(h_lp_ttbar_lowMT_preVeto_pred->GetBinError(nBins),h_lp_ttbar_lowMT_preVeto_pred->GetBinError(nBins+1)) );
  TH1F* h_el_wjttb_lowMT_preVeto_pred = (TH1F*)h_el_wjets_lowMT_preVeto_pred->Clone("h_el_wjttb_lowMT_preVeto_pred"); h_el_wjttb_lowMT_preVeto_pred->Add(h_el_ttbar_lowMT_preVeto_pred);
  TH1F* h_mu_wjttb_lowMT_preVeto_pred = (TH1F*)h_mu_wjets_lowMT_preVeto_pred->Clone("h_mu_wjttb_lowMT_preVeto_pred"); h_mu_wjttb_lowMT_preVeto_pred->Add(h_mu_ttbar_lowMT_preVeto_pred);
  TH1F* h_lp_wjttb_lowMT_preVeto_pred = (TH1F*)h_lp_wjets_lowMT_preVeto_pred->Clone("h_lp_wjttb_lowMT_preVeto_pred"); h_lp_wjttb_lowMT_preVeto_pred->Add(h_lp_ttbar_lowMT_preVeto_pred);

  TH1F* SFpre_el = (TH1F*)h_el_data_lowMT_preVeto_pred->Clone("SFpre_el"); SFpre_el->Divide(h_el_wjttb_lowMT_preVeto_pred);
  TH1F* SFpre_mu = (TH1F*)h_mu_data_lowMT_preVeto_pred->Clone("SFpre_mu"); SFpre_mu->Divide(h_mu_wjttb_lowMT_preVeto_pred);
  TH1F* SFpre_lp = (TH1F*)h_lp_data_lowMT_preVeto_pred->Clone("SFpre_lp"); SFpre_lp->Divide(h_lp_wjttb_lowMT_preVeto_pred);
  SFpre_el->SetLineColor(1); SFpre_el->SetLineWidth(3); SFpre_el->SetTitle("electrons");
  SFpre_mu->SetLineColor(1); SFpre_mu->SetLineWidth(3); SFpre_mu->SetTitle("muons");
  SFpre_lp->SetLineColor(1); SFpre_lp->SetLineWidth(3); SFpre_lp->SetTitle("both");
  TCanvas * c_SFpre = new TCanvas( "c_SFpre","c_SFpre", 1500, 500 ); c_SFpre->Divide(3,1);
  c_SFpre->cd(1); SFpre_el->Draw(); SFpre_el->GetXaxis()->SetTitle("MET");
  c_SFpre->cd(2); SFpre_mu->Draw(); SFpre_mu->GetXaxis()->SetTitle("MET");
  c_SFpre->cd(3); SFpre_lp->Draw(); SFpre_lp->GetXaxis()->SetTitle("MET");
  c_SFpre->Update();
  c_SFpre->SaveAs("plots/SFpre.png");

  plotMT(preVeto5,"preVeto_wjttb_allScaled"  ,"",SFpre_el,SFpre_mu,SFpre_lp,SFpre_el,SFpre_mu,SFpre_lp,SFpre_el,SFpre_mu,SFpre_lp);

  // === postVeto

  plotMT(srCuts,"postVeto_wjttb_OOtB","",SF_unit,SF_unit,SF_unit,SF_unit,SF_unit,SF_unit,SF_unit,SF_unit,SF_unit);

  // fill low MT regions
  TH1F* h2_el_data_lowMT_pstVeto_pred  = new TH1F("h2_el_data_lowMT_pstVeto_pred",  "h2_el_data_lowMT_pstVeto_pred",  nBins, edges );
  TH1F* h2_mu_data_lowMT_pstVeto_pred  = new TH1F("h2_mu_data_lowMT_pstVeto_pred",  "h2_mu_data_lowMT_pstVeto_pred",  nBins, edges );
  TH1F* h2_lp_data_lowMT_pstVeto_pred  = new TH1F("h2_lp_data_lowMT_pstVeto_pred",  "h2_lp_data_lowMT_pstVeto_pred",  nBins, edges );
  TH1F* h2_el_wjets_lowMT_pstVeto_pred = new TH1F("h2_el_wjets_lowMT_pstVeto_pred", "h2_el_wjets_lowMT_pstVeto_pred", nBins, edges );
  TH1F* h2_mu_wjets_lowMT_pstVeto_pred = new TH1F("h2_mu_wjets_lowMT_pstVeto_pred", "h2_mu_wjets_lowMT_pstVeto_pred", nBins, edges );
  TH1F* h2_lp_wjets_lowMT_pstVeto_pred = new TH1F("h2_lp_wjets_lowMT_pstVeto_pred", "h2_lp_wjets_lowMT_pstVeto_pred", nBins, edges );
  TH1F* h2_el_ttbar_lowMT_pstVeto_pred = new TH1F("h2_el_ttbar_lowMT_pstVeto_pred", "h2_el_ttbar_lowMT_pstVeto_pred", nBins, edges );
  TH1F* h2_mu_ttbar_lowMT_pstVeto_pred = new TH1F("h2_mu_ttbar_lowMT_pstVeto_pred", "h2_mu_ttbar_lowMT_pstVeto_pred", nBins, edges );
  TH1F* h2_lp_ttbar_lowMT_pstVeto_pred = new TH1F("h2_lp_ttbar_lowMT_pstVeto_pred", "h2_lp_ttbar_lowMT_pstVeto_pred", nBins, edges );
  TH1F* h2_el_dilep_lowMT_pstVeto_pred = new TH1F("h2_el_dilep_lowMT_pstVeto_pred", "h2_el_dilep_lowMT_pstVeto_pred", nBins, edges );
  TH1F* h2_mu_dilep_lowMT_pstVeto_pred = new TH1F("h2_mu_dilep_lowMT_pstVeto_pred", "h2_mu_dilep_lowMT_pstVeto_pred", nBins, edges );
  TH1F* h2_lp_dilep_lowMT_pstVeto_pred = new TH1F("h2_lp_dilep_lowMT_pstVeto_pred", "h2_lp_dilep_lowMT_pstVeto_pred", nBins, edges );
  t_eldata->Project("h2_el_data_lowMT_pstVeto_pred" ,"met",dtweight+srCuts+lowMT        +elTrg);
  t_mudata->Project("h2_mu_data_lowMT_pstVeto_pred" ,"met",dtweight+srCuts+lowMT        +muTrg);
  t_lpdata->Project("h2_lp_data_lowMT_pstVeto_pred" ,"met",dtweight+srCuts+lowMT        +lpTrg);
  t_wjets ->Project("h2_el_wjets_lowMT_pstVeto_pred","met",mcweight+srCuts+lowMT        +elTrg);
  t_wjets ->Project("h2_mu_wjets_lowMT_pstVeto_pred","met",mcweight+srCuts+lowMT        +muTrg);
  t_wjets ->Project("h2_lp_wjets_lowMT_pstVeto_pred","met",mcweight+srCuts+lowMT        +lpTrg);
  t_ttbar ->Project("h2_el_ttbar_lowMT_pstVeto_pred","met",mcweight+srCuts+lowMT+ntDilep+elTrg);
  t_ttbar ->Project("h2_mu_ttbar_lowMT_pstVeto_pred","met",mcweight+srCuts+lowMT+ntDilep+muTrg);
  t_ttbar ->Project("h2_lp_ttbar_lowMT_pstVeto_pred","met",mcweight+srCuts+lowMT+ntDilep+lpTrg);
  t_ttbar ->Project("h2_el_dilep_lowMT_pstVeto_pred","met",mcweight+srCuts+lowMT+isDilep+elTrg);
  t_ttbar ->Project("h2_mu_dilep_lowMT_pstVeto_pred","met",mcweight+srCuts+lowMT+isDilep+muTrg);
  t_ttbar ->Project("h2_lp_dilep_lowMT_pstVeto_pred","met",mcweight+srCuts+lowMT+isDilep+lpTrg);
  h2_el_data_lowMT_pstVeto_pred ->SetBinContent( nBins, h2_el_data_lowMT_pstVeto_pred ->GetBinContent(nBins) + h2_el_data_lowMT_pstVeto_pred ->GetBinContent(nBins+1) );
  h2_mu_data_lowMT_pstVeto_pred ->SetBinContent( nBins, h2_mu_data_lowMT_pstVeto_pred ->GetBinContent(nBins) + h2_mu_data_lowMT_pstVeto_pred ->GetBinContent(nBins+1) );
  h2_lp_data_lowMT_pstVeto_pred ->SetBinContent( nBins, h2_lp_data_lowMT_pstVeto_pred ->GetBinContent(nBins) + h2_lp_data_lowMT_pstVeto_pred ->GetBinContent(nBins+1) );
  h2_el_wjets_lowMT_pstVeto_pred->SetBinContent( nBins, h2_el_wjets_lowMT_pstVeto_pred->GetBinContent(nBins) + h2_el_wjets_lowMT_pstVeto_pred->GetBinContent(nBins+1) );
  h2_mu_wjets_lowMT_pstVeto_pred->SetBinContent( nBins, h2_mu_wjets_lowMT_pstVeto_pred->GetBinContent(nBins) + h2_mu_wjets_lowMT_pstVeto_pred->GetBinContent(nBins+1) );
  h2_lp_wjets_lowMT_pstVeto_pred->SetBinContent( nBins, h2_lp_wjets_lowMT_pstVeto_pred->GetBinContent(nBins) + h2_lp_wjets_lowMT_pstVeto_pred->GetBinContent(nBins+1) );
  h2_el_ttbar_lowMT_pstVeto_pred->SetBinContent( nBins, h2_el_ttbar_lowMT_pstVeto_pred->GetBinContent(nBins) + h2_el_ttbar_lowMT_pstVeto_pred->GetBinContent(nBins+1) );
  h2_mu_ttbar_lowMT_pstVeto_pred->SetBinContent( nBins, h2_mu_ttbar_lowMT_pstVeto_pred->GetBinContent(nBins) + h2_mu_ttbar_lowMT_pstVeto_pred->GetBinContent(nBins+1) );
  h2_lp_ttbar_lowMT_pstVeto_pred->SetBinContent( nBins, h2_lp_ttbar_lowMT_pstVeto_pred->GetBinContent(nBins) + h2_lp_ttbar_lowMT_pstVeto_pred->GetBinContent(nBins+1) );
  h2_el_dilep_lowMT_pstVeto_pred->SetBinContent( nBins, h2_el_dilep_lowMT_pstVeto_pred->GetBinContent(nBins) + h2_el_dilep_lowMT_pstVeto_pred->GetBinContent(nBins+1) );
  h2_mu_dilep_lowMT_pstVeto_pred->SetBinContent( nBins, h2_mu_dilep_lowMT_pstVeto_pred->GetBinContent(nBins) + h2_mu_dilep_lowMT_pstVeto_pred->GetBinContent(nBins+1) );
  h2_lp_dilep_lowMT_pstVeto_pred->SetBinContent( nBins, h2_lp_dilep_lowMT_pstVeto_pred->GetBinContent(nBins) + h2_lp_dilep_lowMT_pstVeto_pred->GetBinContent(nBins+1) );
  h2_el_data_lowMT_pstVeto_pred ->SetBinError( nBins, getError(h2_el_data_lowMT_pstVeto_pred ->GetBinError(nBins),h2_el_data_lowMT_pstVeto_pred ->GetBinError(nBins+1)) );
  h2_mu_data_lowMT_pstVeto_pred ->SetBinError( nBins, getError(h2_mu_data_lowMT_pstVeto_pred ->GetBinError(nBins),h2_mu_data_lowMT_pstVeto_pred ->GetBinError(nBins+1)) );
  h2_lp_data_lowMT_pstVeto_pred ->SetBinError( nBins, getError(h2_lp_data_lowMT_pstVeto_pred ->GetBinError(nBins),h2_lp_data_lowMT_pstVeto_pred ->GetBinError(nBins+1)) );
  h2_el_wjets_lowMT_pstVeto_pred->SetBinError( nBins, getError(h2_el_wjets_lowMT_pstVeto_pred->GetBinError(nBins),h2_el_wjets_lowMT_pstVeto_pred->GetBinError(nBins+1)) );
  h2_mu_wjets_lowMT_pstVeto_pred->SetBinError( nBins, getError(h2_mu_wjets_lowMT_pstVeto_pred->GetBinError(nBins),h2_mu_wjets_lowMT_pstVeto_pred->GetBinError(nBins+1)) );
  h2_lp_wjets_lowMT_pstVeto_pred->SetBinError( nBins, getError(h2_lp_wjets_lowMT_pstVeto_pred->GetBinError(nBins),h2_lp_wjets_lowMT_pstVeto_pred->GetBinError(nBins+1)) );
  h2_el_ttbar_lowMT_pstVeto_pred->SetBinError( nBins, getError(h2_el_ttbar_lowMT_pstVeto_pred->GetBinError(nBins),h2_el_ttbar_lowMT_pstVeto_pred->GetBinError(nBins+1)) );
  h2_mu_ttbar_lowMT_pstVeto_pred->SetBinError( nBins, getError(h2_mu_ttbar_lowMT_pstVeto_pred->GetBinError(nBins),h2_mu_ttbar_lowMT_pstVeto_pred->GetBinError(nBins+1)) );
  h2_lp_ttbar_lowMT_pstVeto_pred->SetBinError( nBins, getError(h2_lp_ttbar_lowMT_pstVeto_pred->GetBinError(nBins),h2_lp_ttbar_lowMT_pstVeto_pred->GetBinError(nBins+1)) );
  h2_el_dilep_lowMT_pstVeto_pred->SetBinError( nBins, getError(h2_el_dilep_lowMT_pstVeto_pred->GetBinError(nBins),h2_el_dilep_lowMT_pstVeto_pred->GetBinError(nBins+1)) );
  h2_mu_dilep_lowMT_pstVeto_pred->SetBinError( nBins, getError(h2_mu_dilep_lowMT_pstVeto_pred->GetBinError(nBins),h2_mu_dilep_lowMT_pstVeto_pred->GetBinError(nBins+1)) );
  h2_lp_dilep_lowMT_pstVeto_pred->SetBinError( nBins, getError(h2_lp_dilep_lowMT_pstVeto_pred->GetBinError(nBins),h2_lp_dilep_lowMT_pstVeto_pred->GetBinError(nBins+1)) );
  TH1F* h2_el_wjttbar_lowMT_pstVeto_pred = (TH1F*)h2_el_wjets_lowMT_pstVeto_pred->Clone("h2_el_wjttbar_lowMT_pstVeto_pred");
  TH1F* h2_mu_wjttbar_lowMT_pstVeto_pred = (TH1F*)h2_mu_wjets_lowMT_pstVeto_pred->Clone("h2_mu_wjttbar_lowMT_pstVeto_pred");
  TH1F* h2_lp_wjttbar_lowMT_pstVeto_pred = (TH1F*)h2_lp_wjets_lowMT_pstVeto_pred->Clone("h2_lp_wjttbar_lowMT_pstVeto_pred");
  h2_el_wjttbar_lowMT_pstVeto_pred->Add(h2_el_ttbar_lowMT_pstVeto_pred);
  h2_mu_wjttbar_lowMT_pstVeto_pred->Add(h2_mu_ttbar_lowMT_pstVeto_pred);
  h2_lp_wjttbar_lowMT_pstVeto_pred->Add(h2_lp_ttbar_lowMT_pstVeto_pred);

  // scale dilep by SFpre
  h2_el_dilep_lowMT_pstVeto_pred->Multiply(SFpre_el);
  h2_mu_dilep_lowMT_pstVeto_pred->Multiply(SFpre_mu);
  h2_lp_dilep_lowMT_pstVeto_pred->Multiply(SFpre_lp);
  // dilep subtracted data
  h2_el_data_lowMT_pstVeto_pred->Add(h2_el_dilep_lowMT_pstVeto_pred,-1);
  h2_mu_data_lowMT_pstVeto_pred->Add(h2_mu_dilep_lowMT_pstVeto_pred,-1);
  h2_lp_data_lowMT_pstVeto_pred->Add(h2_lp_dilep_lowMT_pstVeto_pred,-1);

  TH1F* SFpst_el = (TH1F*)h2_el_data_lowMT_pstVeto_pred->Clone("SFpst_el"); SFpst_el->Divide(h2_el_wjttbar_lowMT_pstVeto_pred);
  TH1F* SFpst_mu = (TH1F*)h2_mu_data_lowMT_pstVeto_pred->Clone("SFpst_mu"); SFpst_mu->Divide(h2_mu_wjttbar_lowMT_pstVeto_pred);
  TH1F* SFpst_lp = (TH1F*)h2_lp_data_lowMT_pstVeto_pred->Clone("SFpst_lp"); SFpst_lp->Divide(h2_lp_wjttbar_lowMT_pstVeto_pred);
  SFpst_el->SetLineColor(1); SFpst_el->SetLineWidth(3); SFpst_el->SetTitle("electrons");
  SFpst_mu->SetLineColor(1); SFpst_mu->SetLineWidth(3); SFpst_mu->SetTitle("muons");
  SFpst_lp->SetLineColor(1); SFpst_lp->SetLineWidth(3); SFpst_lp->SetTitle("both");

  TCanvas * c_SFpst = new TCanvas( "c_SFpst","c_SFpst", 1500, 500 ); c_SFpst->Divide(3,1);
  c_SFpst->cd(1); SFpst_el->Draw(); SFpst_el->GetXaxis()->SetTitle("MET");
  c_SFpst->cd(2); SFpst_mu->Draw(); SFpst_mu->GetXaxis()->SetTitle("MET");
  c_SFpst->cd(3); SFpst_lp->Draw(); SFpst_lp->GetXaxis()->SetTitle("MET");
  c_SFpst->Update();
  c_SFpst->SaveAs("plots/SFpst.png");

  plotMT(srCuts,"pstVeto_wjttb_ttbarScaled"  ,"",SF_unit,SF_unit,SF_unit,SFpst_el,SFpst_mu,SFpst_lp,SFpre_el,SFpre_mu,SFpre_lp);

  // === final scale factors // SF2L_el

  TH1F* SFpst_x_Rwjet_el = (TH1F*)SFpst_el->Clone("SFpst_x_Rwjet_el"); SFpst_x_Rwjet_el->Multiply(Rwjet_el);
  TH1F* SFpst_x_Rwjet_mu = (TH1F*)SFpst_mu->Clone("SFpst_x_Rwjet_mu"); SFpst_x_Rwjet_mu->Multiply(Rwjet_mu);
  TH1F* SFpst_x_Rwjet_lp = (TH1F*)SFpst_lp->Clone("SFpst_x_Rwjet_lp"); SFpst_x_Rwjet_lp->Multiply(Rwjet_lp);
  TH1F* SFpst_x_Rtop_el  = (TH1F*)SFpst_el->Clone("SFpst_x_Rtop_el" ); SFpst_x_Rtop_el ->Multiply(Rtop_el);
  TH1F* SFpst_x_Rtop_mu  = (TH1F*)SFpst_mu->Clone("SFpst_x_Rtop_mu" ); SFpst_x_Rtop_mu ->Multiply(Rtop_mu);
  TH1F* SFpst_x_Rtop_lp  = (TH1F*)SFpst_lp->Clone("SFpst_x_Rtop_lp" ); SFpst_x_Rtop_lp ->Multiply(Rtop_lp);
  TH1F* SFpre_x_SF2L_el  = (TH1F*)SFpre_el->Clone("SFpre_x_SF2L_el" ); SFpre_x_SF2L_el ->Multiply(SF2L_el);
  TH1F* SFpre_x_SF2L_mu  = (TH1F*)SFpre_mu->Clone("SFpre_x_SF2L_mu" ); SFpre_x_SF2L_mu ->Multiply(SF2L_mu);
  TH1F* SFpre_x_SF2L_lp  = (TH1F*)SFpre_lp->Clone("SFpre_x_SF2L_lp" ); SFpre_x_SF2L_lp ->Multiply(SF2L_lp);

  cout << endl << endl << "printing prediction numbers:" << endl;

  // pythia
  getSRnumbers(srCuts,"prediction", SFpst_x_Rwjet_el , SFpst_x_Rwjet_mu , SFpst_x_Rwjet_lp ,
                                    SFpst_x_Rtop_el  , SFpst_x_Rtop_mu  , SFpst_x_Rtop_lp  ,
                                    SFpre_x_SF2L_el  , SFpre_x_SF2L_mu  , SFpre_x_SF2L_lp  );
  // data
  //getSRnumbers(srCuts,"prediction",SFpst_x_Rwjet_el ,SFpst_x_Rwjet_mu,SFpst_x_Rwjet_lp,
  //                                 SFpst_x_Rtop_el  ,SFpst_x_Rtop_mu ,SFpst_x_Rtop_lp ,
  //                                 SFpre_el         ,SFpre_mu        ,SFpre_lp        );

  cout << endl << endl << "printing OOtB numbers:" << endl;
  getSRnumbers(srCuts,"predictionOOtB", SF_unit , SF_unit , SF_unit ,
                                        SF_unit , SF_unit , SF_unit ,
                                        SF_unit , SF_unit , SF_unit ,
                                        true );
  cout << endl;

} // runPrediction()

#endif

void run8TeVbkgestPythia() {
  for(int i=1; i<=nBins; ++i) { SF_unit->SetBinContent(i,1); SF_unit->SetBinError(i,0); }

  TH1F* SFW_el = (TH1F*)SF_unit->Clone("SFW_avg_el");
  TH1F* SFW_mu = (TH1F*)SF_unit->Clone("SFW_avg_mu");
  TH1F* SFW_lp = (TH1F*)SF_unit->Clone("SFW_avg_lp");

  TH1F* SF2L_el = (TH1F*)SF_unit->Clone("SF2L_el");
  TH1F* SF2L_mu = (TH1F*)SF_unit->Clone("SF2L_mu");
  TH1F* SF2L_lp = (TH1F*)SF_unit->Clone("SF2L_lp");

  gStyle->SetOptStat(0);
  TH1::SetDefaultSumw2();

  //plotMT(baseCuts4+dilepCut,"incMet","misc",SF_unit,SF_unit,SF_unit,SF_unit,SF_unit,SF_unit,SF_unit,SF_unit,SF_unit);
  //return;

  runWJets(SFW_el, SFW_mu, SFW_lp);
  runDiLep(SF2L_el, SF2L_mu, SF2L_lp);
  runIsoTrk();
  runPrediction(SFW_el, SFW_mu, SFW_lp, SF2L_el, SF2L_mu, SF2L_lp);

} // run8TeVbkgestPythia()



