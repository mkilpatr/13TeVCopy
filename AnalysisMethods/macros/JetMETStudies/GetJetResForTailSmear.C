#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/PlotUtils/interface/Plot.hh"
#include "AnalysisMethods/PlotUtils/interface/PlotTools.hh"
#include "AnalysisTools/QuickRefold/interface/TObjectContainer.h"

#include "TreeReadingHelper.h"
#include "TDirectory.h"
#include "TTree.h"
#include "TFile.h"
#include "TMultiGraph.h"
#include<iostream>

using namespace std;

TString weight = "";

void getRes(TTree * tree,TFile* oF){
  float jetPTs[] = {0,50,75,100,125,150,200,250,300,400,500,700,1000,1500,4000};
  int nJetPTS = 14;
//
//  float jetPTs[] = {300,400,500};
//  int nJetPTS = 2;

  PlotInfo resPlot("res" ,"recojetpt/genjetpt"          ,"p_{T,reco}/p_{T,gen}",200,0,2);
  Plot * plot  = new Plot("jetres","shoot","","Events");
  Plot * plot2 = new Plot("jetres","shoot","","Events");
  QuickRefold::TH1DContainer * a = new QuickRefold::TH1DContainer("JetRes",1);
  a->addAxis(0,"PT",nJetPTS,jetPTs);
  a->stopSetup();
  for(int iP =0; iP < nJetPTS; ++iP){
    TString title = TString::Format("%.0f-%.0f",jetPTs[iP],jetPTs[iP+1] );
    TString cut = TString::Format("genjetpt >= %f && genjetpt < %f && genjetrank <= 1",jetPTs[iP],jetPTs[iP+1] );
    TH1D * hD  = getHistogram(hD, tree,resPlot,"qcd",cut,0,&weight);
    if(hD == 0) continue;
    PlotTools::normalize(hD);
    int color = iP+1;
    if(color>= 10) color++;
    plot->addHist((TH1F*)hD,title,"",color,0,color);
    oF->cd();
    hD = (TH1D*)hD->Clone();
    hD->Write();
    TH1D * hComp = (TH1D*)hD->Clone();
    for(unsigned int iB = hComp->GetNbinsX(); iB >= 1; --iB){
      hComp->SetBinContent(iB,hComp->Integral(0,iB) );
    }
    plot2->addHist((TH1F*)hComp,title,"",color,0,color);
    a->setBin(0,jetPTs[iP] + 10);
    a->setValue(*hComp);
  }
  TCanvas * c = new TCanvas;
  plot->setLogy();
  plot->draw(c,true,"png");
  c->Write();
  TCanvas * c2 = new TCanvas;
  plot2->draw(c2,true,"png");
  c2->Write();
  oF->cd();
  a->Write();
  delete a;
}

void getFlvDepRes(TTree * tree,TFile* oF){
  float jetPTs[] = {0,50,75,100,125,150,200,250,300,400,500,700,1000,4000};
  int nJetPTS = 13;
//  float jetPTs[] = {0,50,4000};
//  int nJetPTS = 2;
//
//  float jetPTs[] = {300,400,500};
//  int nJetPTS = 2;

  TString FLVS[] = {"flavor != 4","flavor == 4",""};
  PlotInfo resPlot("res" ,"recojetpt/genjetpt"          ,"p_{T,reco}/p_{T,gen}",200,0,2);
  Plot * plot  = new Plot("jetres","shoot","","Events");
  Plot * plot2 = new Plot("jetres","shoot","","Events");
  QuickRefold::TH1DContainer * a = new QuickRefold::TH1DContainer("JetResByFlav",2);
  a->addAxis(0,"PT",nJetPTS,jetPTs);
  a->addAxis(1,"L_or_B",2,-.5,1.5);
  a->stopSetup();
  for(int iF =0; iF < FLVS[iF][0]; ++iF){
    a->setBin(1,iF);
    for(int iP =0; iP < nJetPTS; ++iP){
      TString title = TString::Format("%s, %.0f-%.0f",FLVS[iF].Data(),jetPTs[iP],jetPTs[iP+1] );
      TString cut = TString::Format("genjetpt >= %f && genjetpt < %f && genjetrank <= 1 && %s",jetPTs[iP],jetPTs[iP+1],FLVS[iF].Data() );
      if(iP == nJetPTS -1){
        cut = TString::Format("genjetpt >= %f && genjetrank <= 1 && %s",jetPTs[iP],FLVS[iF].Data() );
      }
      TH1D * hD  = getHistogram(hD, tree,resPlot,iF == 0 ? "light" : "b",cut,0,&weight);
      if(hD == 0) continue;
      int color = iP+1;
      if(color>= 10) color++;
      plot->addHist((TH1F*)hD,title,"",color,0,color);
//fill any holes
      int binS = hD->FindBin(1.0);
      for(int iB = binS; iB > 1; --iB){
        if(hD->GetBinContent(iB -1) > 0) continue;
        double integral = hD->Integral(1,iB);
        double newV = integral/double(iB);
        for(int iB2 = 1; iB2 <= iB; ++iB2)
          hD->SetBinContent(iB2,newV);
        cout <<"For "<< title <<" the first less than one empty bin is : " << iB -1 <<" ("<< hD->GetBinCenter(iB -1) <<") all bins below this one (including one above) will now have value: "<<  newV<<endl;
        break;
      }
      for(int iB = binS; iB < hD->GetNbinsX(); ++iB){
        if(hD->GetBinContent(iB + 1) > 0) continue;
        double integral = hD->Integral(iB,hD->GetNbinsX());
        double newV = integral/double(hD->GetNbinsX() + 1 - iB);
        for(int iB2 = iB; iB2 <= hD->GetNbinsX(); ++iB2)
          hD->SetBinContent(iB2,newV);
        cout <<"For "<< title <<" the first greater than one empty bin is : " << iB + 1 <<" ("<< hD->GetBinCenter(iB + 1) <<") all bins above this one (including one below) will now have value: "<<  newV<<endl;
        break;
      }
      PlotTools::normalize(hD);
      oF->cd();
      hD = (TH1D*)hD->Clone();
      hD->Write();
      TH1D * hComp = (TH1D*)hD->Clone();
      TString temp_name = hComp->GetName();
      temp_name.Insert(temp_name.Sizeof() - ((iP + 1 < 10) && (iF == 0) ? 3 : 4), "_comp");
      cout << temp_name << endl;
      hComp->SetName(temp_name);
      for(unsigned int iB = hComp->GetNbinsX(); iB >= 1; --iB){
        hComp->SetBinContent(iB,hComp->Integral(0,iB) );
      }
      hComp->Write();
      plot2->addHist((TH1F*)hComp,title,"",color,0,color);
      a->setBin(0,jetPTs[iP] + 10);
      a->setValue(*hComp);
    }
  }
  TCanvas * c = new TCanvas;
  plot->setLogy();
  plot->draw(c,true,"png");
  c->Write();
  TCanvas * c2 = new TCanvas;
  plot2->draw(c2,true,"png");
  c2->Write();

  a->Write();
  delete a;
}

#endif

//possible weight strings to use: puWeight, puWeight*weight, puWeight*smearWeight, weight, 1.0
//void GetJetResForTailSmear(const TString inFile="jetResSkim_combined.root", const TString treeName = "Events", const TString outFile = "resTailOut_combined_puWeight_WoH.root", const TString weightString = "puWeight")
//void GetJetResForTailSmear(const TString inFile="jetResSkim_combined_filtered.root", const TString treeName = "Events", const TString outFile = "resTailOut_combined_filtered_puWeight_WoH.root", const TString weightString = "puWeight")
//void GetJetResForTailSmear(const TString inFile="jetResSkim_prev.root", const TString treeName = "Events", const TString outFile = "resTailOut_prev_puWeight_WoH.root", const TString weightString = "puWeight")
//void GetJetResForTailSmear(const TString inFile="jetResSkim_orig.root", const TString treeName = "Events", const TString outFile = "resTailOut_orig_puWeight_WoH.root", const TString weightString = "puWeight")
//void GetJetResForTailSmear(const TString inFile="jetResSkim_plus.root", const TString treeName = "Events", const TString outFile = "resTailOut_plus_puWeight_WoH.root", const TString weightString = "puWeight")
//void GetJetResForTailSmear(const TString inFile="jetResSkim_combined.root", const TString treeName = "Events", const TString outFile = "resTailOut_combined_puWeight_weight_WoH.root", const TString weightString = "puWeight*weight")
//void GetJetResForTailSmear(const TString inFile="jetResSkim_combined_filtered.root", const TString treeName = "Events", const TString outFile = "resTailOut_combined_filtered_puWeight_weight_WoH.root", const TString weightString = "puWeight*weight")
void GetJetResForTailSmear(const TString inFile="jetResSkim_combined_filtered_CHEF.root", const TString treeName = "Events", const TString outFile = "resTailOut_combined_filtered_CHEF_puWeight_weight_WoH_NORMALIZED.root", const TString weightString = "puWeight*weight")
//void GetJetResForTailSmear(const TString inFile="jetResSkim_prev.root", const TString treeName = "Events", const TString outFile = "resTailOut_prev_puWeight_weight_WoH.root", const TString weightString = "puWeight*weight")
//void GetJetResForTailSmear(const TString inFile="jetResSkim_orig.root", const TString treeName = "Events", const TString outFile = "resTailOut_orig_puWeight_weight_WoH.root", const TString weightString = "puWeight*weight")
//void GetJetResForTailSmear(const TString inFile="jetResSkim_plus.root", const TString treeName = "Events", const TString outFile = "resTailOut_plus_puWeight_weight_WoH.root", const TString weightString = "puWeight*weight")
//void GetJetResForTailSmear(const TString inFile="jetResSkim_combined_filtered_CHEF_new.root", const TString treeName = "Events", const TString outFile = "resTailOut_combined_filtered_newCHEF_puWeight_weight_WoH.root", const TString weightString = "puWeight*weight")
//void GetJetResForTailSmear(const TString inFile="jetResSkim_combined_filtered_CHEF_newer.root", const TString treeName = "Events", const TString outFile = "resTailOut_combined_filtered_newerCHEF_puWeight_weight_WoH.root", const TString weightString = "puWeight*weight")
{
  weight = weightString;
  cout << weight << endl;
  TFile * oF = new TFile(outFile,"recreate");
  StyleTools::SetStyle();
  TFile * mf = new TFile(inFile,"read");
  TTree * mt =0;
  mf->GetObject(treeName,mt);

//  getRes(mt,oF);
  getFlvDepRes(mt,oF);

  oF->Close();
//  mf->Close();
//  delete mf;
}
