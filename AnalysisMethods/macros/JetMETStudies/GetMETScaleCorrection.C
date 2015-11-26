#if !defined(__CINT__) || defined(__MAKECINT__)

#include "AnalysisTools/QuickRefold/interface/FormulaContainer.h"

#include<iostream>
#include "TreeReadingHelper.h"
using namespace std;


void makeComparisonPlots(TTree * mcTree, TTree * dataTree,TString prefix = ""){


  TString MCProc[] = {"process == 4","process == 2","process != 4 && process != 2 && process >= 1",""};
  TString MCProcNames[] = {"zll","ttbar","other",""};
  int MCProcColors[] = {StyleTools::color_znunu,StyleTools::color_ttbar,StyleTools::color_other};

  vector<PlotInfo*> vars;
  vars.push_back (new PlotInfo("met"          ,"met"          ,"#slash{E}_{T}"                              ,50,0,200));
  vars.push_back (new PlotInfo("njets"        ,"njets"        ,"N. jets"                                    ,10,-.5,9.5));
  vars.push_back (new PlotInfo("npv"        ,"npv"        ,"NPV"                                    ,50,-.5,49.5));
  vars.push_back (new PlotInfo("nbjets"       ,"nbjets"       ,"N. b-jets"                                  ,4,-.5,3.5));
  vars.push_back (new PlotInfo("ht"           ,"ht"           ,"H_{T}"                                      ,50,0,1000));
  vars.push_back (new PlotInfo("htAlong"      ,"htAlong"      ,"H_{T} #parallel p_{T}(Z)"                   ,50,-200,200));
  vars.push_back (new PlotInfo("metPar"       ,"metPar"      ,"#slash{E}_{T} #parallel p_{T}(Z)"           ,50,-100,100));
  vars.push_back (new PlotInfo("metPerp"      ,"metPerp"       ,"#slash{E}_{T} #perp p_{T}(Z)"               ,50,-100,100));
  vars.push_back (new PlotInfo("zllpt"        ,"zllpt"      ,"p_{T}(Z)"                                   ,50,0,500));
  vars.push_back (new PlotInfo("htozllpt"     ,"ht/zllpt"        ,"H_{T}/p_{T}(Z)"                             ,100,0,20));
  vars.push_back (new PlotInfo("htAlongozllpt","htAlong/zllpt"     ,"(H_{T} #parallel p_{T}(Z))/p_{T}(Z)"        ,100,-10,10));
  vars.push_back (new PlotInfo("metParozllpt" ,"metPar/zllpt","(#slash{E}_{T} #parallel p_{T}(Z))/p_{T}(Z)",100,-10,10));
  vars.push_back (new PlotInfo("metPerpozllpt","metPerp/zllpt" ,"(#slash{E}_{T} #perp p_{T}(Z))/p_{T}(Z)"    ,100,-10,10));

  TString weight = "weight*puWeight";
  TString sel[] = {"zllpt >= 0","zllpt >= 0 && zllpt <25","zllpt >= 25 && zllpt <50","zllpt >= 50 && zllpt <100","zllpt >= 100 && zllpt <200","zllpt >= 200",""};
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

void getResponseValues(TTree * mcTree, TTree * dataTree){
//  double binsX[] = {0,25,50,100,200,500};
  double binsX[] = {20,30,40,50,80,130,250,500};
  TString sel = "1.0";
//  TString sel = "muon == 0";
  int nBinsX = 7;
  TString weight = "puWeight*weight";
  TProfile * prof = getProfile(mcTree,"mcTree","mc","zllpt","metPar/zllpt",nBinsX,binsX,sel,0,&weight);
  prof->SetTitle("MC");
//  prof->Draw();


//  TCanvas * cD = new TCanvas();
  TProfile * profD = getProfile(dataTree,"dataTree","data","zllpt","metPar/zllpt",nBinsX,binsX,sel);
  profD->SetTitle("Data");
//  profD->Draw();

  TF1 * formula = new TF1("response","[0] + [1]/x + [2]/(x*x)");
  TF1 * outFormula = new TF1("scaleCorr","([0] + [1]/x + [2]/(x*x)) - ([3] + [4]/x + [5]/(x*x))",20,300);


  for(unsigned int iB = 1; iB <= nBinsX; ++iB){
    cout << profD->GetBinCenter(iB) <<" >> " << profD->GetBinContent(iB) <<" :: "<<prof->GetBinContent(iB) <<" :: "<<profD->GetBinContent(iB) - prof->GetBinContent(iB) <<endl;
  }

  new TCanvas();
  profD->Fit(formula,"W");
  profD->Draw();
  cout << formula->GetParameter(0) <<" "<< formula->GetParameter(1)<<" "<< formula->GetParameter(2) << endl;
  outFormula->FixParameter(0,formula->GetParameter(0) );
  outFormula->FixParameter(1,formula->GetParameter(1) );
  outFormula->FixParameter(2,formula->GetParameter(2) );
  new TCanvas();
  prof->Fit(formula,"W");
  prof->Draw();
  cout << formula->GetParameter(0) <<" "<< formula->GetParameter(1)<<" "<< formula->GetParameter(2) << endl;
  outFormula->FixParameter(3,formula->GetParameter(0) );
  outFormula->FixParameter(4,formula->GetParameter(1) );
  outFormula->FixParameter(5,formula->GetParameter(2) );

  QuickRefold::FormulaContainer * a = new QuickRefold::FormulaContainer("METScale",1);
  a->addAxis(0,"null",1,-.5,1.5);
  a->stopSetup();
  a->setBin(0,0);
  a->setValue(*outFormula);


  new TCanvas();
  outFormula->SetTitle(" ");
  outFormula->GetXaxis()->SetTitle("p_{T}(Z)");
  outFormula->GetYaxis()->SetTitle("<Data>/<MC> (#slash{E}_{T} #parallel / p_{T}(Z))");
  outFormula->Draw();


  TFile * f = new TFile("metCorrections.root","recreate");
  a->Write();
  f->Close();
}

void compareMETResolution(vector<TTree*> mcTrees, vector<TString> mcNames, TTree * dataTree, bool doSigma,TString prefix = ""){
  int MCProcColors[] = {StyleTools::color_znunu,StyleTools::color_ttbar, StyleTools::color_comp4, StyleTools::color_comp1,StyleTools::color_other};

  vector<PlotInfo*> vars;
//  vars.push_back (new PlotInfo("njets"        ,"njets"        ,"N. jets"                                    ,3,-.5,5.5));
//  vars.push_back (new PlotInfo("ht"           ,"ht"           ,"H_{T}"                                      ,3,0,300));
//  vars.push_back (new PlotInfo("zllpt"        ,"zllpt"      ,"p_{T}(Z)"                                     ,6,0,300));

  vars.push_back (new PlotInfo("njets"        ,"njets"        ,"N. jets"                                    ,6,-.5,5.5));
  vars.push_back (new PlotInfo("ht"           ,"ht"           ,"H_{T}"                                      ,6,0,300));
  vars.push_back (new PlotInfo("zllpt"        ,"zllpt"      ,"p_{T}(Z)"                                     ,5,0,300));

  TString weight = "weight*puWeight";
  TString sel[] = {"zllpt >= 20","","zllpt >= 20 && zllpt <50","zllpt >= 50 && zllpt <75","zllpt >= 75 && zllpt <100","zllpt >= 100",""};

//  TString varY[] = {"metPar/zllpt","metPerp/zllpt",""};
//  TString varYNames[] = {"#slash{E}_{T} #parallel / p_{T}(Z)", "#slash{E}_{T} #perp / p_{T}(Z)",""  };

  TString varY[] = {"metPar","metPerp","","metPar/zllpt","metPerp/zllpt",""};
  TString varYNames[] = {"#slash{E}_{T} #parallel", "#slash{E}_{T} #perp","#slash{E}_{T} #parallel / p_{T}(Z)", "#slash{E}_{T} #perp / p_{T}(Z)",""  };

//  TString lep[] = {"muon >= -1","muon == 1","muon == 0",""};
//  TString lepName[] = {"incl","mu","e",""};
  TString lep[] = {"muon >= 0.0","","muon == 1 && abs(zlleta) < 1.4","muon == 1 && abs(zlleta) >= 1.4","muon == 0 && abs(zlleta) < 1.4","muon == 0 && abs(zlleta) >= 1.4",""};
  TString lepName[] = {"incl","mu_barrel","mu_endcap","barrel_e","endcap_e",""};

  for(unsigned int iS = 0; sel[iS][0]; ++iS){
    for(unsigned int iV = 0; iV < vars.size(); ++iV){
//      if(iS > 0 &&iV > 1) continue;
      TObjArray * o = new TObjArray;
      TObjArray * oo = new TObjArray;

      for(unsigned int iL = 0; lep[iL][0]; ++iL){
        for(unsigned int iY = 0; varY[iY][0]; ++iY){
          TString yVTitle = doSigma ? "#sigma" : "mean";
          TString yTitle = TString::Format("%s (%s)",yVTitle.Data(),varYNames[iY].Data());
          TString yTitle2 = TString::Format("%s(Data) %s %s(MC) (%s)",yVTitle.Data(), doSigma ? "/" : "-" , yVTitle.Data(),varYNames[iY].Data());
          TString thisTitle = TString::Format("%s, %s",sel[iS].Data(),lepName[iL].Data());
          TString name = TString::Format("%s%s_%i_%s_x_%s,",prefix.Data(),lepName[iL].Data(),iS,vars[iV]->name.Data(),varYNames[iY].Data());
          TString name2 = name;
          name2 += "_comp";

          Plot * plot = new Plot(name,thisTitle,vars[iV]->xTitle,yTitle);
          Plot * plotC = new Plot(name2,thisTitle,vars[iV]->xTitle,yTitle2  );

          TString thisSel = TString::Format("%s && %s", sel[iS].Data(),lep[iL].Data());
          TProfile * pD  = getProfile(dataTree,*vars[iV],varY[iY],"data",thisSel);
          TH1F * hD = getErrorHisto(pD, *vars[iV], "data",doSigma);
          plot->addHist(hD,"data","",StyleTools::color_data,0,StyleTools::color_data);

          for(unsigned int iM = 0; iM < mcTrees.size(); ++iM){
            TString weight = "puWeight*weight";
            TProfile * pM  = getProfile(mcTrees[iM],*vars[iV],varY[iY],mcNames[iM],thisSel,0,&weight);
            if(!pM) continue;
            TH1F * hM = getErrorHisto(pM, *vars[iV], mcNames[iM],doSigma);
            plot->addHist(hM,mcNames[iM],"",MCProcColors[iM],0,MCProcColors[iM]);

            TH1F * hDC = (TH1F*)hD->Clone();
            if(doSigma)
            hDC->Divide(hDC,hM,1,1,"");
            else
            hDC->Add(hM,-1);

            plotC->addHist(hDC,mcNames[iM],"",MCProcColors[iM],0,MCProcColors[iM]);

          }

//          TCanvas * c = new TCanvas;
//          plot->draw(c,false);
//          o->Add(c);
          TCanvas * c2 = new TCanvas;
          if(doSigma)
            plotC->setYRange(.75,1.25);
          else
            plotC->setYRange(-10,10);
          plotC->draw(c2,false);
          oo->Add(c2);
        }
      }

      StyleTools::drawAll(oo,TString::Format("%s_%i",vars[iV]->name.Data(),vars[iV]->nH));
    }
  }
}



#endif


void GetMETScaleCorrection(const TString treeName = "Events")
{
  StyleTools::SetStyle();

  TFile * df = new TFile("data.root","read");
  TTree * dt =0;
  df->GetObject(treeName,dt);

  vector<TString> mcFileNames;
  mcFileNames.push_back("SM_DiLep_metCorr.root");
  mcFileNames.push_back("SM_DiLep_corr_metCorr.root");
//  mcFileNames.push_back("SM_DiLep_corr_resp90_metCorr.root");
//  mcFileNames.push_back("SM_DiLep_corr_resp75_metCorr.root");
//  mcFileNames.push_back("SM_DiLep_corr_resp50_metCorr.root");
//  mcFileNames.push_back("SM_DiLep_corr_scale_metCorr.root");

  vector<TTree*> mcTrees(mcFileNames.size(),0);

  vector<TString> mcTreeNames;
  mcTreeNames.push_back("std");
  mcTreeNames.push_back("corr");
//  mcTreeNames.push_back("corr res 0p90");
//  mcTreeNames.push_back("corr res 0p75");
//  mcTreeNames.push_back("corr res 0p50");
//  mcTreeNames.push_back("corr scale");

  for(unsigned int iT = 0; iT < mcFileNames.size(); ++iT){
    TFile * tF = new TFile(mcFileNames[iT],"read");
    tF->GetObject(treeName,mcTrees[iT]);
  }

//  makeComparisonPlots(mcTrees[0],dt);
//  makeComparisonPlots(mcTrees[1],dt,"corr_");
//  makeComparisonPlots(mcTrees[2],dt,"corr_scale_");


      getResponseValues(mcTrees[0],dt);
//  compareMETResolution(mcTrees,mcTreeNames,dt,false,"offsetComp_");

//  compareMETResolution(mcTrees,mcTreeNames,dt,true,"resComp_");



}
