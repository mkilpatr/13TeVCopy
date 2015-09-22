#if !defined(__CINT__) || defined(__MAKECINT__)

#include "AnalysisTools/QuickRefold/interface/FormulaContainer.h"

#include<iostream>
#include "TreeReadingHelper.h"
using namespace std;


void makeComparisonTestPlots(TTree * mcTree, TTree * dataTree,TString prefix = ""){


  TString MCProc[] = {"process == 4","process == 2","process != 4 && process != 2 && process >= 1",""};
  TString MCProcNames[] = {"zll","ttbar","other",""};
  int MCProcColors[] = {StyleTools::color_znunu,StyleTools::color_ttbar,StyleTools::color_other};

  vector<PlotInfo*> vars;
  vars.push_back (new PlotInfo("met"          ,"met"          ,"#slash{E}_{T}"                              ,15,200,500));
//  vars.push_back (new PlotInfo("njets"        ,"njets"        ,"N. jets"                                    ,10,-.5,9.5));
//  vars.push_back (new PlotInfo("npv"        ,"npv"        ,"NPV"                                    ,50,-.5,49.5));
//  vars.push_back (new PlotInfo("nbjets"       ,"nbjets"       ,"N. b-jets"                                  ,4,-.5,3.5));
  vars.push_back (new PlotInfo("mtw"       ,"mtw"       ,"mtw"                                  ,20,0,300));
  vars.push_back (new PlotInfo("ptw"       ,"ptw"       ,"ptw"                                  ,20,0,500));

  TString weight = "weight*puWeight";
  TString sel[] = {"met >=200 && muon >= 1",""};
//  TString sel[] = {"zllpt >= 50 && zllpt <80",""};

  for(unsigned int iS = 0; sel[iS][0]; ++iS){
    for(unsigned int iV = 0; iV < vars.size(); ++iV){
      Plot * plot = new Plot(prefix + vars[iV]->name+TString::Format("_%i",iS),sel[iS],vars[iV]->xTitle,"Events");
      TH1F * hD  = getHistogram(dataTree,*vars[iV],"data",sel[iS]);
      if(!hD) continue;
      plot->addHist(hD,"data","",StyleTools::color_data);
      for(unsigned int iM = 0; MCProc[iM][0]; ++iM){
        TH1F * hM  = getHistogram(mcTree,*vars[iV],MCProcNames[iM], sel[iS] + " && ",  &MCProc[iM],&weight );
        if(!hM) continue;
        plot->addToStack(hM,MCProcNames[iM],MCProcColors[iM]);
      }
      TCanvas * c = new TCanvas;
      plot->setLogy();
      plot->drawRatioStack(c,true);
    }
  }
}




#endif


void TestMETScaleCorrection(const TString treeName = "Events")
{

  TFile * df = new TFile("data_zeroLep.root","read");
  TTree * dt =0;
  df->GetObject(treeName,dt);


  vector<TString> mcFileNames;
  mcFileNames.push_back("SM_zeroLep_metTestCorr.root");
  mcFileNames.push_back("SM_zeroLep_corr_metTestCorr.root");
  //  mcFileNames.push_back("SM_DiLep_corr_resp90_metCorr.root");
  //  mcFileNames.push_back("SM_DiLep_corr_resp75_metCorr.root");
  //  mcFileNames.push_back("SM_DiLep_corr_resp50_metCorr.root");
  mcFileNames.push_back("SM_zeroLep_corr_scale_metTestCorr.root");

  vector<TTree*> mcTrees(mcFileNames.size(),0);

  vector<TString> mcTreeNames;
  mcTreeNames.push_back("std");
  mcTreeNames.push_back("corr");
  //  mcTreeNames.push_back("corr res 0p90");
  //  mcTreeNames.push_back("corr res 0p75");
  //  mcTreeNames.push_back("corr res 0p50");
  mcTreeNames.push_back("corr scale");

  for(unsigned int iT = 0; iT < mcFileNames.size(); ++iT){
    TFile * tF = new TFile(mcFileNames[iT],"read");
    tF->GetObject(treeName,mcTrees[iT]);
  }

  makeComparisonTestPlots(mcTrees[0],dt,"zero_");
  makeComparisonTestPlots(mcTrees[1],dt,"zero_corr_");
  makeComparisonTestPlots(mcTrees[2],dt,"zero_corr_scale_");
}
