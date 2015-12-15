#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/PlotUtils/interface/Plot.hh"
#include "AnalysisMethods/PlotUtils/interface/PlotTools.hh"
#include "AnalysisTools/QuickRefold/interface/TF1Container.h"

#include "TreeReadingHelper.h"
#include "TDirectory.h"
#include "TTree.h"
#include "TFile.h"
#include "TMultiGraph.h"
#include<iostream>

using namespace std;

  void getRes(TTree * tree,TFile* oF){

  double jetPTs[] = {20,25,30,40,50,60,80,100,120,150,180,220,270,350,450,550,700,900,1200,2000};
  int nJetPTS = 19;

  double jetETAs[] = {0,1.1,1.7,2.3,2.8,3.2,5};
  int nJetETAS = 6;

 PlotInfo resPlot("res" ,"recojetpt/genjetpt"          ,"p_{T,reco}/p_{T,gen}",50,0,2);
 TF1 * gausFitter =new TF1("gausFitter","gaus",0,2);
 TMultiGraph * mg = new TMultiGraph("meanGraph","meanGraph");
 TMultiGraph * sg = new TMultiGraph("sigGraph","sigGraph");

  for(unsigned int iE =0; iE < nJetETAS; ++iE){
    TString stackTitle = TString::Format("%.1f-%.1f",jetETAs[iE],jetETAs[iE+1]);
    Plot * plot = new Plot(TString::Format("jetres_%i",iE),stackTitle,"","Events");
    TGraphErrors * graphMean = new TGraphErrors();
    TGraphErrors * graphSig  = new TGraphErrors();
    for(unsigned int iP =0; iP < nJetPTS; ++iP){
      TString title = TString::Format("%.0f-%.0f",jetPTs[iP],jetPTs[iP+1] );
      TString cut = TString::Format("TMath::Abs(genjeteta) >= %f && TMath::Abs(genjeteta) < %f && genjetpt >= %f && genjetpt < %f ",jetETAs[iE],jetETAs[iE+1],jetPTs[iP],jetPTs[iP+1] );
      TString weight = "puWeight";
      TH1F * hD  = getHistogram(tree,resPlot,"qcd",cut,0,&weight);
      if(hD == 0) continue;
      PlotTools::normalize(hD);
      gausFitter->SetParameters(1,1,.1);
      hD->Fit(gausFitter,"QS");
      double mean = gausFitter->GetParameter(1);
      double meanE = gausFitter->GetParError(1);
      double sig  = gausFitter->GetParameter(2);
      double sigE = gausFitter->GetParError(2);
      graphMean->SetPoint(iP ,(jetPTs[iP]+jetPTs[iP+1])/2,mean);
      graphMean->SetPointError(iP ,0,meanE);
      graphSig->SetPoint(iP ,(jetPTs[iP]+jetPTs[iP+1])/2,sig);
      graphSig->SetPointError(iP ,0,sigE);
      int color = iP+1;
      if(color>= 10) color++;
      hD->SetTitle(stackTitle);
      plot->addHist(hD,title,"",color,0,color);
      oF->cd();
      hD->Write();
    }
    TCanvas * c = new TCanvas;
    plot->setLogy();
    plot->draw(c,true,"png");

    graphMean->SetTitle(stackTitle);
    graphMean->SetMarkerStyle(20);
    graphMean->SetMarkerColor(iE+1);
    mg->Add(graphMean);
    graphSig->SetTitle(stackTitle);
    graphSig->SetMarkerStyle(20);
    graphSig->SetMarkerColor(iE+1);
    sg->Add(graphSig);
  }
  TCanvas * c1 = new TCanvas();
  mg->Draw("AP");
  c1->BuildLegend();
  oF->cd();
  c1->Write("mean");
  c1 = new TCanvas();
  sg->Draw("AP");
  c1->BuildLegend();
  oF->cd();
  c1->Write("sigma");
  oF->Close();
}


void getTrend(TFile* iF){
  TCanvas * can = 0;
  iF->GetObject("sigma",can);
  TMultiGraph * mg = (TMultiGraph* )can->GetPrimitive("sigGraph");
  TList * l = mg->GetListOfGraphs();

//  TFormula * trendFormula = new TFormula("trendFormula","[0] + [1]/x + [2]/(x*x)");
//  TF1 * trendFitter =new TF1("trendFitter"
  TF1 * trendFitter =new TF1("trendFitter","[0] + [1]/x + [2]/(x*x)");


  QuickRefold::TF1Container * a = new QuickRefold::TF1Container("AK4ResTrend",1);
  float jetETAs[] = {0,1.1,1.7,2.3,2.8,3.2,5};
  int nJetETAS = 6;
  a->addAxis(0,"ETA",nJetETAS,jetETAs);
  a->stopSetup();


  for(unsigned int iG = 0; iG < l->GetSize(); ++iG){
   TGraphErrors * g = (TGraphErrors*)l->At(iG);
   g->Fit(trendFitter);
   cout  << g->GetTitle() <<" "<< trendFitter->GetParameter(0) <<" "<< trendFitter->GetParameter(1) <<" "<< trendFitter->GetParameter(2) <<endl;
   cout << (jetETAs[iG]  + jetETAs[iG+1])/2 << endl;
   a->setBin(0,(jetETAs[iG]  + jetETAs[iG+1])/2);

   a->setValue(*trendFitter);
  }

  can->Draw();

  TFile * f = new TFile("ak4JetResTrends.root","recreate");
  a->Write();
  f->Close();
}


#endif


void GetJetRes(const TString inFile="jetResSkim_orig.root", const TString treeName = "Events", const TString outFile = "resCoreOut.root")
{
  TFile * oF = new TFile(outFile,"recreate");
  StyleTools::SetStyle();
  TFile * mf = new TFile(inFile,"read");
  TTree * mt =0;
  mf->GetObject(treeName,mt);
//
  getRes(mt,oF);

//  TFile * of = new TFile(outFile,"read");
//  if(of){
//    getTrend(of);
//  }

//  TFile * f = new TFile("ak4JetResTrends.root","read");
//  const QuickRefold::TF1Container * a;
//  f->GetObject("AK4ResTrend",a);
//  a->getValue(3.0);
//  cout << a->getValue(3.0).Eval(20)<<endl;
//
//  f->Close();
//  delete f;

}
