#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/PlotUtils/interface/Plot.hh"
#include "AnalysisMethods/PlotUtils/interface/PlotTools.hh"
#include "AnalysisTools/QuickRefold/interface/Refold.h"

#include "TDirectory.h"
#include "TTree.h"
#include "TFile.h"
#include "TMultiGraph.h"
#include<iostream>

using namespace std;


TH2D * getHistogram(TTree * tree, TString name, TString sel, TString weight,  TString xAxis, int nXBins, double xMin, double xMax, TString yAxis, int nYBins, double yMin, double yMax){
  TString selection = TString::Format("%s*(%s)",weight.Data(),sel.Data());

  TH2D * h = new TH2D(name,name,nXBins,xMin,xMax,nYBins,yMin,yMax);
  tree->Draw(TString::Format("%s:%s>>+%s",yAxis.Data(),xAxis.Data(),name.Data()),selection,"goff");
  if(h) h = (TH2D*)h->Clone();
  return h;
}

void go(TTree * tree,TFile* oF){

  enum SignalType {T2tt, T2bW, T2fb, T2tb, NUM_SIGTYPES};
  const std::string SIGNAL_NAMES[]  {"T2tt", "T2bW", "T2fb","T2tb",""};
  enum sigAxes {MASS1,MASS2,MASS3,TYPE};
  enum CORRTYPE {NOMINAL, UP, DOWN, NONE};
  const std::string CORR_NAMES[]  {"NOMINAL", "UP", "DOWN","",""};
//  const std::string WEIGHTS[]  {"corrWeightTight", "upCorrWeightTight", "downCorrWeightTight","",""};
  const std::string WEIGHTS[]  {"corrWeight", "upCorrWeight", "downCorrWeight","",""};

  for(unsigned int iS = 0; iS <= T2fb; ++iS){
    TString sel = TString::Format("sigType == %u",iS);

    QuickRefold::Refold * a = new QuickRefold::Refold(SIGNAL_NAMES[iS].c_str(),4);
    a->addAxis(MASS1,"MASS1",1051,149.5,1200.5);
    a->addAxis(MASS2,"MASS2",801,-0.5,800.5);
    a->addAxis(MASS3,"MASS3",1,-12.5,12.5);
    a->addAxis(TYPE,"TYPE",4,-.5,3.5);
    a->stopSetup();

    a->setBin(MASS3,0);

    TString name = TString::Format("%s_%s",SIGNAL_NAMES[iS].c_str(),"num");
    TH2D * num = getHistogram(tree,name,sel,"1.0","mass1",1051,149.5,1200.5,"mass2",801,-0.5,800.5);

    for(unsigned int iT = 0; iT <= DOWN; ++iT){
      a->setBin(TYPE,iT);
      TString denomName = TString::Format("%s_%s",SIGNAL_NAMES[iS].c_str(),CORR_NAMES[iT].c_str());
      TH2D * denom = getHistogram(tree,denomName,sel,WEIGHTS[iT],"mass1",1051,149.5,1200.5,"mass2",801,-0.5,800.5);

      for(unsigned int iX = 1; iX <= num->GetNbinsX(); ++iX ){
        for(unsigned int iY = 1; iY <= num->GetNbinsY(); ++iY ){
          double denCount = denom->GetBinContent(iX,iY);
          double numCount = num->GetBinContent(iX,iY);
          if(denCount == 0) continue;
          a->setBin(MASS1,denom->GetXaxis()->GetBinCenter(iX));
          a->setBin(MASS2,denom->GetYaxis()->GetBinCenter(iY));
          cout << iS <<" "<<iT<<" "<<denom->GetXaxis()->GetBinCenter(iX) <<" "<< denom->GetYaxis()->GetBinCenter(iY)<<" "<<numCount<<" "<< denCount<<" "<<numCount/denCount<<endl;
          a->setValue(numCount/denCount);
        }
      }
    }
    oF->cd();
    a->Write();
    delete a;
  }
}
#endif

//void GetJetResForTailSmear(const TString inFile="jetResSkim_orig_CombinedSample.root", const TString treeName = "Events", const TString outFile = "resTailOut_puWeight_weight.root")
void GetISRNorm(const TString inFile="isr_all.root",  const TString outFile = "isrSigNorms.root")
{
  TFile * oF = new TFile(outFile,"recreate");
  StyleTools::SetStyle();
  TFile * mf = new TFile(inFile,"read");
  TTree * mt =0;
  mf->GetObject("Events",mt);

//  getRes(mt,oF);
  go(mt,oF);

  oF->Close();
}
