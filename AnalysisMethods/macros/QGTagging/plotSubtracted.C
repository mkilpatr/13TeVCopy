#if !defined(__CINT__) || defined(__MAKECINT__)
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include <TFile.h>
#include <TTree.h>
#include <TH2F.h>
#include <THStack.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TString.h>
#include <TStyle.h>

using namespace std;

TString treeDir = "trees/151117_gluonRwt/";

TFile* f_z_data = TFile::Open(treeDir+"data_doublemu_tree.root","READONLY");
TFile* f_g_data = TFile::Open(treeDir+"data_singlegm_tree.root","READONLY");
TFile* f_j_data = TFile::Open(treeDir+"data_jetht_tree.root"   ,"READONLY");

TFile* f_z_quark = TFile::Open(treeDir+"dyjetstoll_quarks_tree.root","READONLY");
TFile* f_z_gluon = TFile::Open(treeDir+"dyjetstoll_gluons_tree.root","READONLY");
TFile* f_z_other = TFile::Open(treeDir+"dyjetstoll_other_tree.root" ,"READONLY");

TFile* f_g_quark = TFile::Open(treeDir+"gjets_quarks_tree.root","READONLY");
TFile* f_g_gluon = TFile::Open(treeDir+"gjets_gluons_tree.root","READONLY");
TFile* f_g_other = TFile::Open(treeDir+"gjets_other_tree.root" ,"READONLY");

TFile* f_j_quark = TFile::Open(treeDir+"qcd_quarks_tree.root","READONLY");
TFile* f_j_gluon = TFile::Open(treeDir+"qcd_gluons_tree.root","READONLY");
TFile* f_j_other = TFile::Open(treeDir+"qcd_other_tree.root" ,"READONLY");

TTree* t_z_data  = (TTree*)f_z_data ->Get("Events");
TTree* t_g_data  = (TTree*)f_g_data ->Get("Events");
TTree* t_j_data  = (TTree*)f_j_data ->Get("Events");
TTree* t_z_quark = (TTree*)f_z_quark->Get("Events");
TTree* t_z_gluon = (TTree*)f_z_gluon->Get("Events");
TTree* t_z_other = (TTree*)f_z_other->Get("Events");
TTree* t_g_quark = (TTree*)f_g_quark->Get("Events");
TTree* t_g_gluon = (TTree*)f_g_gluon->Get("Events");
TTree* t_g_other = (TTree*)f_g_other->Get("Events");
TTree* t_j_quark = (TTree*)f_j_quark->Get("Events");
TTree* t_j_gluon = (TTree*)f_j_gluon->Get("Events");
TTree* t_j_other = (TTree*)f_j_other->Get("Events");

TString lum    = "0.578";
TString weight = lum+"*weight*puWeight";
TString savedir = "plotsSubtracted/";

TString central = " && abs(j0eta)<2.4";
TString ptlow   = " && j0pt>300 && j0pt<400"; //" && j0pt>040";
TString z_cuts = "passZjet && passZmass"            +ptlow; // +central;
TString g_cuts = "passGmjet"                        +ptlow; // +central;
TString j_cuts = "passDijet && passDijet3 && ht>450"+ptlow; // +central;

double findMax(double a, double b, double c=0, double d=0) {
  double max = a;
  if(b>max) max=b;
  if(c>max) max=c;
  if(d>max) max=d;
  return max;
}

void plotVar(TString var, int bins, float low, float high) {

  cout << "plotting " << var << "..." << endl;
  TH1F* h_z_data   = new TH1F("h_z_data" ,"h_z_data" ,bins,low,high); h_z_data ->SetLineWidth(3); h_z_data ->SetLineColor( 1); // black
  TH1F* h_g_data   = new TH1F("h_g_data" ,"h_g_data" ,bins,low,high); h_g_data ->SetLineWidth(3); h_g_data ->SetLineColor( 1); // black
  TH1F* h_j_data   = new TH1F("h_j_data" ,"h_j_data" ,bins,low,high); h_j_data ->SetLineWidth(3); h_j_data ->SetLineColor( 1); // black
  TH1F* h_z_quark  = new TH1F("h_z_quark","h_z_quark",bins,low,high); h_z_quark->SetLineWidth(1); h_z_quark->SetFillColor( 4); // blue
  TH1F* h_z_gluon  = new TH1F("h_z_gluon","h_z_gluon",bins,low,high); h_z_gluon->SetLineWidth(1); h_z_gluon->SetFillColor( 2); // red
  TH1F* h_z_other  = new TH1F("h_z_other","h_z_other",bins,low,high); h_z_other->SetLineWidth(1); h_z_other->SetFillColor(11); // grey
  TH1F* h_g_quark  = new TH1F("h_g_quark","h_g_quark",bins,low,high); h_g_quark->SetLineWidth(1); h_g_quark->SetFillColor( 4); // blue
  TH1F* h_g_gluon  = new TH1F("h_g_gluon","h_g_gluon",bins,low,high); h_g_gluon->SetLineWidth(1); h_g_gluon->SetFillColor( 2); // red
  TH1F* h_g_other  = new TH1F("h_g_other","h_g_other",bins,low,high); h_g_other->SetLineWidth(1); h_g_other->SetFillColor(11); // grey
  TH1F* h_j_quark  = new TH1F("h_j_quark","h_j_quark",bins,low,high); h_j_quark->SetLineWidth(1); h_j_quark->SetFillColor( 4); // blue
  TH1F* h_j_gluon  = new TH1F("h_j_gluon","h_j_gluon",bins,low,high); h_j_gluon->SetLineWidth(1); h_j_gluon->SetFillColor( 2); // red
  TH1F* h_j_other  = new TH1F("h_j_other","h_j_other",bins,low,high); h_j_other->SetLineWidth(1); h_j_other->SetFillColor(11); // grey

  t_z_data  ->Project("h_z_data" , var,      "1.0*("+z_cuts+")");
  t_z_quark ->Project("h_z_quark", var,  weight+"*("+z_cuts+")");
  t_z_gluon ->Project("h_z_gluon", var,  weight+"*("+z_cuts+")");
  t_z_other ->Project("h_z_other", var,  weight+"*("+z_cuts+")");
  t_g_data  ->Project("h_g_data" , var,      "1.0*("+g_cuts+")");
  t_g_quark ->Project("h_g_quark", var,  weight+"*("+g_cuts+")");
  t_g_gluon ->Project("h_g_gluon", var,  weight+"*("+g_cuts+")");
  t_g_other ->Project("h_g_other", var,  weight+"*("+g_cuts+")");
  t_j_data  ->Project("h_j_data" , var, "prescale*("+j_cuts+")");
  t_j_quark ->Project("h_j_quark", var,  weight+"*("+j_cuts+")");
  t_j_gluon ->Project("h_j_gluon", var,  weight+"*("+j_cuts+")");
  t_j_other ->Project("h_j_other", var,  weight+"*("+j_cuts+")");

  TH1F* sf_z_gmc = (TH1F*)h_z_data->Clone("sf_z_gmc"); sf_z_gmc->Add(h_z_quark,-1); sf_z_gmc->Add(h_z_other,-1); sf_z_gmc->Scale(h_z_gluon->Integral()/sf_z_gmc->Integral()); sf_z_gmc->Divide(h_z_gluon);
  TH1F* sf_g_gmc = (TH1F*)h_g_data->Clone("sf_g_gmc"); sf_g_gmc->Add(h_g_quark,-1); sf_g_gmc->Add(h_g_other,-1); sf_g_gmc->Scale(h_g_gluon->Integral()/sf_g_gmc->Integral()); sf_g_gmc->Divide(h_g_gluon);
  TH1F* sf_j_gmc = (TH1F*)h_j_data->Clone("sf_j_gmc"); sf_j_gmc->Add(h_j_quark,-1); sf_j_gmc->Add(h_j_other,-1); sf_j_gmc->Scale(h_j_gluon->Integral()/sf_j_gmc->Integral()); sf_j_gmc->Divide(h_j_gluon);

  TH1F* h_z_dq = (TH1F*)h_z_data->Clone("h_z_dq"); h_z_dq->Add(h_z_gluon,-1); h_z_dq->SetTitle("data-gluon"); h_z_dq ->SetLineWidth(3);
  TH1F* h_z_dg = (TH1F*)h_z_data->Clone("h_z_dg"); h_z_dg->Add(h_z_quark,-1); h_z_dg->SetTitle("data-quark"); h_z_dg ->SetLineWidth(3);
  TH1F* h_g_dq = (TH1F*)h_g_data->Clone("h_g_dq"); h_g_dq->Add(h_g_gluon,-1); h_g_dq->SetTitle("data-gluon"); h_g_dq ->SetLineWidth(3);
  TH1F* h_g_dg = (TH1F*)h_g_data->Clone("h_g_dg"); h_g_dg->Add(h_g_quark,-1); h_g_dg->SetTitle("data-quark"); h_g_dg ->SetLineWidth(3);
  TH1F* h_j_dq = (TH1F*)h_j_data->Clone("h_j_dq"); h_j_dq->Add(h_j_gluon,-1); h_j_dq->SetTitle("data-gluon"); h_j_dq ->SetLineWidth(3);
  TH1F* h_j_dg = (TH1F*)h_j_data->Clone("h_j_dg"); h_j_dg->Add(h_j_quark,-1); h_j_dg->SetTitle("data-quark"); h_j_dg ->SetLineWidth(3);

  TH1F* temp_z_dq = (TH1F*)h_z_quark->Clone("temp_z_dq"); temp_z_dq->Add(h_z_other);
  TH1F* temp_z_dg = (TH1F*)h_z_gluon->Clone("temp_z_dg"); temp_z_dg->Add(h_z_other);
  TH1F* temp_g_dq = (TH1F*)h_g_quark->Clone("temp_g_dq"); temp_g_dq->Add(h_g_other);
  TH1F* temp_g_dg = (TH1F*)h_g_gluon->Clone("temp_g_dg"); temp_g_dg->Add(h_g_other);
  TH1F* temp_j_dq = (TH1F*)h_j_quark->Clone("temp_j_dq"); temp_j_dq->Add(h_j_other);
  TH1F* temp_j_dg = (TH1F*)h_j_gluon->Clone("temp_j_dg"); temp_j_dg->Add(h_j_other);

  THStack * stack_z_q  = new THStack("stack_z_q","stack_z_q");
  TH1F* h_z_otherQ = (TH1F*)h_z_other->Clone("h_z_otherQ");
  h_z_otherQ->Scale(h_z_dq->Integral()/temp_z_dq->Integral()); stack_z_q->Add(h_z_otherQ);
  h_z_quark ->Scale(h_z_dq->Integral()/temp_z_dq->Integral()); stack_z_q->Add(h_z_quark );
  THStack * stack_g_q  = new THStack("stack_g_q","stack_g_q");
  TH1F* h_g_otherQ = (TH1F*)h_g_other->Clone("h_g_otherQ");
  h_g_otherQ->Scale(h_g_dq->Integral()/temp_g_dq->Integral()); stack_g_q->Add(h_g_otherQ);
  h_g_quark ->Scale(h_g_dq->Integral()/temp_g_dq->Integral()); stack_g_q->Add(h_g_quark );
  THStack * stack_j_q  = new THStack("stack_j_q","stack_j_q");
  TH1F* h_j_otherQ = (TH1F*)h_j_other->Clone("h_j_otherQ");
  h_j_otherQ->Scale(h_j_dq->Integral()/temp_j_dq->Integral()); stack_j_q->Add(h_j_otherQ);
  h_j_quark ->Scale(h_j_dq->Integral()/temp_j_dq->Integral()); stack_j_q->Add(h_j_quark);

  THStack * stack_z_g  = new THStack("stack_z_g","stack_z_g");
  TH1F* h_z_otherG = (TH1F*)h_z_other->Clone("h_z_otherG");
  h_z_otherG->Scale(h_z_dg->Integral()/temp_z_dg->Integral()); stack_z_g->Add(h_z_otherG);
  h_z_gluon ->Scale(h_z_dg->Integral()/temp_z_dg->Integral()); stack_z_g->Add(h_z_gluon );
  THStack * stack_g_g  = new THStack("stack_g_g","stack_g_g");
  TH1F* h_g_otherG = (TH1F*)h_g_other->Clone("h_g_otherG");
  h_g_otherG->Scale(h_g_dg->Integral()/temp_g_dg->Integral()); stack_g_g->Add(h_g_otherG);
  h_g_gluon ->Scale(h_g_dg->Integral()/temp_g_dg->Integral()); stack_g_g->Add(h_g_gluon );
  THStack * stack_j_g  = new THStack("stack_j_g","stack_j_g");
  TH1F* h_j_otherG = (TH1F*)h_j_other->Clone("h_j_otherG");
  h_j_otherG->Scale(h_j_dg->Integral()/temp_j_dg->Integral()); stack_j_g->Add(h_j_otherG);
  h_j_gluon ->Scale(h_j_dg->Integral()/temp_j_dg->Integral()); stack_j_g->Add(h_j_gluon );

  TH1F* sf_z_dq = (TH1F*)h_z_dq->Clone("sf_z_dq"); temp_z_dq->Scale(sf_z_dq->Integral()/temp_z_dq->Integral()); sf_z_dq->Divide(temp_z_dq); sf_z_dq ->SetLineWidth(3);
  TH1F* sf_z_dg = (TH1F*)h_z_dg->Clone("sf_z_dg"); temp_z_dg->Scale(sf_z_dg->Integral()/temp_z_dg->Integral()); sf_z_dg->Divide(temp_z_dg); sf_z_dg ->SetLineWidth(3);
  TH1F* sf_g_dq = (TH1F*)h_g_dq->Clone("sf_g_dq"); temp_g_dq->Scale(sf_g_dq->Integral()/temp_g_dq->Integral()); sf_g_dq->Divide(temp_g_dq); sf_g_dq ->SetLineWidth(3);
  TH1F* sf_g_dg = (TH1F*)h_g_dg->Clone("sf_g_dg"); temp_g_dg->Scale(sf_g_dg->Integral()/temp_g_dg->Integral()); sf_g_dg->Divide(temp_g_dg); sf_g_dg ->SetLineWidth(3);
  TH1F* sf_j_dq = (TH1F*)h_j_dq->Clone("sf_j_dq"); temp_j_dq->Scale(sf_j_dq->Integral()/temp_j_dq->Integral()); sf_j_dq->Divide(temp_j_dq); sf_j_dq ->SetLineWidth(3);
  TH1F* sf_j_dg = (TH1F*)h_j_dg->Clone("sf_j_dg"); temp_j_dg->Scale(sf_j_dg->Integral()/temp_j_dg->Integral()); sf_j_dg->Divide(temp_j_dg); sf_j_dg ->SetLineWidth(3);

  double max_z = findMax(h_z_otherQ->GetMaximum()+h_z_quark->GetMaximum(), h_z_otherG->GetMaximum()+h_z_gluon->GetMaximum(), h_z_dq->GetMaximum(), h_z_dg->GetMaximum());
  double max_g = findMax(h_g_otherQ->GetMaximum()+h_g_quark->GetMaximum(), h_g_otherG->GetMaximum()+h_g_gluon->GetMaximum(), h_g_dq->GetMaximum(), h_g_dg->GetMaximum());
  double max_j = findMax(h_j_otherQ->GetMaximum()+h_j_quark->GetMaximum(), h_j_otherG->GetMaximum()+h_j_gluon->GetMaximum(), h_j_dq->GetMaximum(), h_j_dg->GetMaximum());

  bool saveZ = true;
  bool saveG = true;
  bool saveJ = true;

  if(var=="j0pt") {
    if(high== 120) {                saveG = false; saveJ = false; }
    if(high== 600) { saveZ = false;                saveJ = false; }
    if(high==1000) { saveZ = false; saveG = false;                }
  } // j0pt
  if(var=="j0mult") {
    if(high==50)   {                saveG = false; saveJ = false; }
    if(high==70)   { saveZ = false;                               }
  } // j0mult

  //if(saveZ){
  //  cout << "\n   " << "scale factors for Z:" << endl;
  //  for(int i=1; i<=bins; ++i) cout << "     " << i << "\t" << sf_z_gmc->GetBinContent(i) << endl;
  //  cout << endl;
  //}
  //if(saveG){
  //  cout << "\n   " << "scale factors for G:" << endl;
  //  for(int i=1; i<=bins; ++i) cout << "     " << i << "\t" << sf_g_gmc->GetBinContent(i) << endl;
  //  cout << endl;
  //}
  //if(saveJ){
  //  cout << "\n   " << "scale factors for J:" << endl;
  //  for(int i=1; i<=bins; ++i) cout << "     " << i << "\t" << sf_j_gmc->GetBinContent(i) << endl;
  //  cout << endl;
  //}

  TCanvas * c_z_q = new TCanvas( "c_z_q","c_z_q", 500, 1000 ); c_z_q->Divide(1,2); c_z_q->cd(1);
  stack_z_q->Draw("hist"); stack_z_q->GetXaxis()->SetTitle(var); stack_z_q->SetMaximum(max_z);
  h_z_dq->Draw("samee");
  c_z_q->cd(2);
  sf_z_dq->Draw(); sf_z_dq->SetMinimum(0.0); sf_z_dq->SetMaximum(2.0);
  c_z_q->Update();
  if(saveZ) c_z_q->SaveAs(savedir+"zjets_"+var+"_quark.png");
  TCanvas * c_z_g = new TCanvas( "c_z_g","c_z_g", 500, 1000 ); c_z_g->Divide(1,2); c_z_g->cd(1);
  stack_z_g->Draw("hist"); stack_z_g->GetXaxis()->SetTitle(var); stack_z_g->SetMaximum(max_z);
  h_z_dg->Draw("samee");
  c_z_g->cd(2);
  sf_z_dg->Draw(); sf_z_dg->SetMinimum(0.0); sf_z_dg->SetMaximum(2.0);
  c_z_g->Update();
  if(saveZ) c_z_g->SaveAs(savedir+"zjets_"+var+"_gluon.png");

  TCanvas * c_g_q = new TCanvas( "c_g_q","c_g_q", 500, 1000 ); c_g_q->Divide(1,2); c_g_q->cd(1);
  stack_g_q->Draw("hist"); stack_g_q->GetXaxis()->SetTitle(var); stack_g_q->SetMaximum(max_g);
  h_g_dq->Draw("samee");
  c_g_q->cd(2);
  sf_g_dq->Draw(); sf_g_dq->SetMinimum(0.0); sf_g_dq->SetMaximum(2.0);
  c_g_q->Update();
  if(saveG) c_g_q->SaveAs(savedir+"gjets_"+var+"_quark.png");
  TCanvas * c_g_g = new TCanvas( "c_g_g","c_g_g", 500, 1000 ); c_g_g->Divide(1,2); c_g_g->cd(1);
  stack_g_g->Draw("hist"); stack_g_g->GetXaxis()->SetTitle(var); stack_g_g->SetMaximum(max_g);
  h_g_dg->Draw("samee");
  c_g_g->cd(2);
  sf_g_dg->Draw(); sf_g_dg->SetMinimum(0.0); sf_g_dg->SetMaximum(2.0);
  c_g_g->Update();
  if(saveG) c_g_g->SaveAs(savedir+"gjets_"+var+"_gluon.png");

  TCanvas * c_j_q = new TCanvas( "c_j_q","c_j_q", 500, 1000 ); c_j_q->Divide(1,2); c_j_q->cd(1);
  stack_j_q->Draw("hist"); stack_j_q->GetXaxis()->SetTitle(var); stack_j_q->SetMaximum(max_j);
  h_j_dq->Draw("samee");
  c_j_q->cd(2);
  sf_j_dq->Draw(); sf_j_dq->SetMinimum(0.0); sf_j_dq->SetMaximum(2.0);
  c_j_q->Update();
  if(saveJ) c_j_q->SaveAs(savedir+"dijet_"+var+"_quark.png");
  TCanvas * c_j_g = new TCanvas( "c_j_g","c_j_g", 500, 1000 ); c_j_g->Divide(1,2); c_j_g->cd(1);
  stack_j_g->Draw("hist"); stack_j_g->GetXaxis()->SetTitle(var); stack_j_g->SetMaximum(max_j);
  h_j_dg->Draw("samee");
  c_j_g->cd(2);
  sf_j_dg->Draw(); sf_j_dg->SetMinimum(0.0); sf_j_dg->SetMaximum(2.0);
  c_j_g->Update();
  if(saveJ) c_j_g->SaveAs(savedir+"dijet_"+var+"_gluon.png");

  cout << "   deleting pointers" << endl;
  delete h_z_data ; delete h_z_dq; delete temp_z_dq; delete sf_z_dq; delete sf_z_gmc;
  delete h_g_data ; delete h_z_dg; delete temp_z_dg; delete sf_z_dg; delete sf_g_gmc;
  delete h_j_data ; delete h_g_dq; delete temp_g_dq; delete sf_g_dq; delete sf_j_gmc;
  delete h_z_quark; delete h_g_dg; delete temp_g_dg; delete sf_g_dg;
  delete h_z_gluon; delete h_j_dq; delete temp_j_dq; delete sf_j_dq;
  delete h_z_other; delete h_j_dg; delete temp_j_dg; delete sf_j_dg;
  delete h_g_quark;
  delete h_g_gluon;
  delete h_g_other;
  delete h_j_quark;
  delete h_j_gluon;
  delete h_j_other;
  delete stack_z_q; delete stack_g_q; delete stack_j_q;
  delete stack_z_g; delete stack_g_g; delete stack_j_g;
  delete c_z_q; delete c_g_q; delete c_j_q;
  delete c_z_g; delete c_g_g; delete c_j_g;
  cout << "   done" << endl;
}

#endif

//root -b -l -q ../CMSSW_7_4_11/src/AnalysisMethods/macros/QGTagging/plotSubtracted.C+
void plotSubtracted() {

  cout << "starting plotSubtracted()" << endl;
  gStyle->SetOptStat(0);
  TH1::SetDefaultSumw2();

  plotVar("j0pt" , 30,   0,  120 ); // Z
  plotVar("j0pt" , 30,   0,  600 ); // G
  plotVar("j0pt" , 40, 200, 1000 ); // D

  plotVar("j0mult", 25, 0, 50); // Z
  plotVar("j0mult", 35, 0, 70); // J,G

  plotVar("j0eta"  , 30, -5,  5 );
  plotVar("npv"    , 34,  1, 35 );
  plotVar("rho"    , 30,  0, 30 );
  plotVar("j0ptd"  , 30,  0,  1 );
  plotVar("j0axis1", 40,  0, 10 );
  plotVar("j0axis2", 40,  0, 10 );
  plotVar("j0qgl"  , 30,  0,  1 );

} // plotSubtracted()
