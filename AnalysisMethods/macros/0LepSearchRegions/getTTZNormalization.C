#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/PlotUtils/interface/Plot.hh"
#include "AnalysisMethods/PlotUtils/interface/EffPlotTools.hh"
#include "AnalysisMethods/PlotUtils/interface/PlotTools.hh"
#include "AnalysisTools/QuickRefold/interface/TObjectContainer.h"
#include "TreeReadingHelper.h"
#include "TDirectory.h"
#include "TTree.h"
#include "TFile.h"
#include "TMultiGraph.h"
#include<iostream>

using namespace std;

TString ttzSel = "(minlepPT >= 15 &&nbjets >= 1 && njets >= 4 && passMed && nlbjets >= 2 && onZ == 1 && passTrig >= 1 )";
TString ttzHighPTSel = "(minlepPT >= 15 &&nbjets >= 1 && njets >= 4 && passMed && nlbjets >= 2 && onZ == 1 && ptzll >= 100 && passTrig >= 1)";

TString ttz2BSel = "(minlepPT >= 15 &&nbjets >= 2 && njets >= 4 && passMed && nlbjets >= 2 && onZ == 1 && passTrig >= 1)";
TString ttz2BHighPTSel = "(minlepPT >= 15 &&nbjets >= 2 && njets >= 4 && passMed && nlbjets >= 2 && onZ == 1 && ptzll >= 100 && passTrig >= 1)";

TString noZSel = "(minlepPT >= 15 &&nbjets >= 1 && njets >= 4 && passMed && nlbjets >= 2 && onZ == 0 && passTrig >= 1)";
TString noZHighPTSel = "(minlepPT >= 15 &&nbjets >= 1 && njets >= 4 && passMed && nlbjets >= 2 && onZ == 0 && maxDiLepPt >= 100 && passTrig >= 1)";
TString noBSel = "(minlepPT >= 15 &&nbjets == 0 && njets >= 4 && passMed  && onZ == 1 && passTrig >= 1)";
TString noBHighPTSel = "(minlepPT >= 15 &&nbjets == 0 && njets >= 4 && passMed  && onZ == 1 && ptzll >= 100 && passTrig >= 1)";

TString ttZLikeProc = "*(extProcess == 1 || extProcess == 9)";
TString ttbarLikeProc = "*(extProcess > 1 && extProcess < 9 )";
TString zLikeProc = "*(extProcess > 9)";

TString stdProc[] = {ttZLikeProc,ttbarLikeProc,zLikeProc,""};
TString stdProcNames[] = {"t#bar{t}Z+tZ","bkg. w/o Z","bkg. w/ Z",""};
int stdProcColors[] = {StyleTools::color_ttZ,StyleTools::color_ttbar,StyleTools::color_znunu};


//[reg][MC]
void printSFAndUnc(vector<vector<double> > M, vector<vector<double> > EM, vector<double>  D ){
  {
      double M11 = M[0][0]; double M21 = M[1][0]; double M31 = M[2][0];
      double M12 = M[0][1]; double M22 = M[1][1]; double M32 = M[2][1];
      double M13 = M[0][2]; double M23 = M[1][2]; double M33 = M[2][2];

      double EM11 = EM[0][0]; double EM21 = EM[1][0]; double EM31 = EM[2][0];
      double EM12 = EM[0][1]; double EM22 = EM[1][1]; double EM32 = EM[2][1];
      double EM13 = EM[0][2]; double EM23 = EM[1][2]; double EM33 = EM[2][2];

      double D1 = D[0];
      double D2 = D[1];
      double D3 = D[2];

      double A1 = -((-(D3*M13*M22) + D3*M12*M23 + D2*M13*M32 - D1*M23*M32 - D2*M12*M33 + D1*M22*M33)/(M13*M22*M31 - M12*M23*M31 - M13*M21*M32 + M11*M23*M32 + M12*M21*M33 - M11*M22*M33));
      double A2 = -((D3*M13*M21 - D3*M11*M23 - D2*M13*M31 + D1*M23*M31 + D2*M11*M33 - D1*M21*M33)/(M13*M22*M31 - M12*M23*M31 - M13*M21*M32 + M11*M23*M32 + M12*M21*M33 - M11*M22*M33));
      double A3 = -((D3*M12*M21 - D3*M11*M22 - D2*M12*M31 + D1*M22*M31 + D2*M11*M32 - D1*M21*M32)/(-(M13*M22*M31) + M12*M23*M31 + M13*M21*M32 - M11*M23*M32 - M12*M21*M33 + M11*M22*M33));

      double A1E2 =(EM33*pow(M13*M22 - M12*M23,2)*pow(D3*M12*M21 - D3*M11*M22 - D2*M12*M31 + D1*M22*M31 + D2*M11*M32 - D1*M21*M32,2) + EM23*pow(D3*M12*M21 - D3*M11*M22 - D2*M12*M31 + D1*M22*M31 + D2*M11*M32 - D1*M21*M32,2)*pow(M13*M32 - M12*M33,2) +
       EM32*pow(M13*M22 - M12*M23,2)*pow(D3*M13*M21 - D3*M11*M23 - D2*M13*M31 + D1*M23*M31 + D2*M11*M33 - D1*M21*M33,2) + EM22*pow(M13*M32 - M12*M33,2)*pow(D3*M13*M21 - D3*M11*M23 - D2*M13*M31 + D1*M23*M31 + D2*M11*M33 - D1*M21*M33,2) +
       EM13*pow(D3*M12*M21 - D3*M11*M22 - D2*M12*M31 + D1*M22*M31 + D2*M11*M32 - D1*M21*M32,2)*pow(M23*M32 - M22*M33,2) + EM12*pow(D3*M13*M21 - D3*M11*M23 - D2*M13*M31 + D1*M23*M31 + D2*M11*M33 - D1*M21*M33,2)*pow(M23*M32 - M22*M33,2) +
       EM31*pow(M13*M22 - M12*M23,2)*pow(D3*M13*M22 - D3*M12*M23 - D2*M13*M32 + D1*M23*M32 + D2*M12*M33 - D1*M22*M33,2) + EM21*pow(M13*M32 - M12*M33,2)*pow(D3*M13*M22 - D3*M12*M23 - D2*M13*M32 + D1*M23*M32 + D2*M12*M33 - D1*M22*M33,2) +
       EM11*pow(M23*M32 - M22*M33,2)*pow(D3*M13*M22 - D3*M12*M23 - D2*M13*M32 + D1*M23*M32 + D2*M12*M33 - D1*M22*M33,2) + D3*pow(M13*M22 - M12*M23,2)*pow(M13*M22*M31 - M12*M23*M31 - M13*M21*M32 + M11*M23*M32 + M12*M21*M33 - M11*M22*M33,2) +
       D2*pow(M13*M32 - M12*M33,2)*pow(M13*M22*M31 - M12*M23*M31 - M13*M21*M32 + M11*M23*M32 + M12*M21*M33 - M11*M22*M33,2) + D1*pow(M23*M32 - M22*M33,2)*pow(M13*M22*M31 - M12*M23*M31 - M13*M21*M32 + M11*M23*M32 + M12*M21*M33 - M11*M22*M33,2))/
     pow(M13*M22*M31 - M12*M23*M31 - M13*M21*M32 + M11*M23*M32 + M12*M21*M33 - M11*M22*M33,4);
      double A2E2 = (EM33*pow(M13*M21 - M11*M23,2)*pow(D3*M12*M21 - D3*M11*M22 - D2*M12*M31 + D1*M22*M31 + D2*M11*M32 - D1*M21*M32,2) + EM23*pow(D3*M12*M21 - D3*M11*M22 - D2*M12*M31 + D1*M22*M31 + D2*M11*M32 - D1*M21*M32,2)*pow(M13*M31 - M11*M33,2) +
       EM13*pow(D3*M12*M21 - D3*M11*M22 - D2*M12*M31 + D1*M22*M31 + D2*M11*M32 - D1*M21*M32,2)*pow(M23*M31 - M21*M33,2) + EM32*pow(M13*M21 - M11*M23,2)*pow(D3*M13*M21 - D3*M11*M23 - D2*M13*M31 + D1*M23*M31 + D2*M11*M33 - D1*M21*M33,2) +
       EM22*pow(M13*M31 - M11*M33,2)*pow(D3*M13*M21 - D3*M11*M23 - D2*M13*M31 + D1*M23*M31 + D2*M11*M33 - D1*M21*M33,2) + EM12*pow(M23*M31 - M21*M33,2)*pow(D3*M13*M21 - D3*M11*M23 - D2*M13*M31 + D1*M23*M31 + D2*M11*M33 - D1*M21*M33,2) +
       EM31*pow(M13*M21 - M11*M23,2)*pow(D3*M13*M22 - D3*M12*M23 - D2*M13*M32 + D1*M23*M32 + D2*M12*M33 - D1*M22*M33,2) + EM21*pow(M13*M31 - M11*M33,2)*pow(D3*M13*M22 - D3*M12*M23 - D2*M13*M32 + D1*M23*M32 + D2*M12*M33 - D1*M22*M33,2) +
       EM11*pow(M23*M31 - M21*M33,2)*pow(D3*M13*M22 - D3*M12*M23 - D2*M13*M32 + D1*M23*M32 + D2*M12*M33 - D1*M22*M33,2) + D3*pow(M13*M21 - M11*M23,2)*pow(M13*M22*M31 - M12*M23*M31 - M13*M21*M32 + M11*M23*M32 + M12*M21*M33 - M11*M22*M33,2) +
       D2*pow(M13*M31 - M11*M33,2)*pow(M13*M22*M31 - M12*M23*M31 - M13*M21*M32 + M11*M23*M32 + M12*M21*M33 - M11*M22*M33,2) + D1*pow(M23*M31 - M21*M33,2)*pow(M13*M22*M31 - M12*M23*M31 - M13*M21*M32 + M11*M23*M32 + M12*M21*M33 - M11*M22*M33,2))/
     pow(M13*M22*M31 - M12*M23*M31 - M13*M21*M32 + M11*M23*M32 + M12*M21*M33 - M11*M22*M33,4);
      double A3E2 = (EM33*pow(M12*M21 - M11*M22,2)*pow(D3*M12*M21 - D3*M11*M22 - D2*M12*M31 + D1*M22*M31 + D2*M11*M32 - D1*M21*M32,2) + EM23*pow(M12*M31 - M11*M32,2)*pow(D3*M12*M21 - D3*M11*M22 - D2*M12*M31 + D1*M22*M31 + D2*M11*M32 - D1*M21*M32,2) +
       EM13*pow(M22*M31 - M21*M32,2)*pow(D3*M12*M21 - D3*M11*M22 - D2*M12*M31 + D1*M22*M31 + D2*M11*M32 - D1*M21*M32,2) + EM32*pow(M12*M21 - M11*M22,2)*pow(D3*M13*M21 - D3*M11*M23 - D2*M13*M31 + D1*M23*M31 + D2*M11*M33 - D1*M21*M33,2) +
       EM22*pow(M12*M31 - M11*M32,2)*pow(D3*M13*M21 - D3*M11*M23 - D2*M13*M31 + D1*M23*M31 + D2*M11*M33 - D1*M21*M33,2) + EM12*pow(M22*M31 - M21*M32,2)*pow(D3*M13*M21 - D3*M11*M23 - D2*M13*M31 + D1*M23*M31 + D2*M11*M33 - D1*M21*M33,2) +
       EM31*pow(M12*M21 - M11*M22,2)*pow(D3*M13*M22 - D3*M12*M23 - D2*M13*M32 + D1*M23*M32 + D2*M12*M33 - D1*M22*M33,2) + EM21*pow(M12*M31 - M11*M32,2)*pow(D3*M13*M22 - D3*M12*M23 - D2*M13*M32 + D1*M23*M32 + D2*M12*M33 - D1*M22*M33,2) +
       EM11*pow(M22*M31 - M21*M32,2)*pow(D3*M13*M22 - D3*M12*M23 - D2*M13*M32 + D1*M23*M32 + D2*M12*M33 - D1*M22*M33,2) + D3*pow(M12*M21 - M11*M22,2)*pow(M13*M22*M31 - M12*M23*M31 - M13*M21*M32 + M11*M23*M32 + M12*M21*M33 - M11*M22*M33,2) +
       D2*pow(M12*M31 - M11*M32,2)*pow(M13*M22*M31 - M12*M23*M31 - M13*M21*M32 + M11*M23*M32 + M12*M21*M33 - M11*M22*M33,2) + D1*pow(M22*M31 - M21*M32,2)*pow(M13*M22*M31 - M12*M23*M31 - M13*M21*M32 + M11*M23*M32 + M12*M21*M33 - M11*M22*M33,2))/
     pow(M13*M22*M31 - M12*M23*M31 - M13*M21*M32 + M11*M23*M32 + M12*M21*M33 - M11*M22*M33,4);

      cout << A1 <<" " << TMath::Sqrt(A1E2) <<endl
           << A2 <<" " << TMath::Sqrt(A2E2) <<endl
           << A3 <<" " << TMath::Sqrt(A3E2) <<endl ;
  }

}


void getNormalization(TTree * tree) {
  TString SELS[] = {ttzSel,ttzHighPTSel,ttz2BSel,ttz2BHighPTSel,noZSel,noZHighPTSel,noBSel,noBHighPTSel,""};
  TString SELNAMES[] = {"ttzSel","ttzHighPTSelt","ttz2BSel","ttz2BHighPTSel","noZSel","noZHighPTSel","noBSel","noBHighPTSel",""};

  TString PROC[] = {"*(process == 7)","*(process == 9)","*(process == 2)","*(process==4)","*(process == 8)","*(process == 6)",""};
  TString PROCNAMES[] = {"t#bar{t}Z","VV","t#bar{t}","Z#rightarrowll","t#bar{t}W","tV",""};

  TString weight = "4*weight";
  TString var = "ptzll";
  TString varTitle = "p_{T} (Z)";

  vector<vector<float> > yields(8);

  vector<vector<double> > groupYields(3,vector<double>(3)); //ttz ttbar Z
  vector<vector<double> > groupUnc(3,vector<double>(3));


  for(unsigned int iS = 0; SELS[iS][0]; ++iS){

    TH1F * hTN  = getHistogram(tree,"extProcess",TString::Format("ttZNorm_total_%i",iS),"all",13,-.5,12.5,SELS[iS],0,&weight);
    hTN->SetTitle(SELNAMES[iS]);
    new TCanvas();
    hTN->Draw();
    double total = hTN->Integral();
    double ttzTotal = hTN->GetBinContent(2) + hTN->GetBinContent(10);
    cout <<SELNAMES[iS] <<" "<< total <<" "<< ttzTotal <<" "<< (total-ttzTotal)/total <<" "<< TMath::Sqrt(total)/ttzTotal << endl;

    for(unsigned int iB = 1; iB <= hTN->GetNbinsX(); ++iB){
      yields[iS].push_back(hTN->GetBinContent(iB));
    }

    if(iS == 0 || iS == 4 || iS == 6){
      int fillB = 0;
      if(iS == 4) fillB = 1;
      if(iS == 6) fillB = 2;
      groupYields[fillB][0] += hTN->GetBinContent(2);
      groupYields[fillB][0] += hTN->GetBinContent(10);
      groupYields[fillB][1] += hTN->GetBinContent(3);
      groupYields[fillB][1] += hTN->GetBinContent(4);
      groupYields[fillB][1] += hTN->GetBinContent(5);
      groupYields[fillB][1] += hTN->GetBinContent(6);
      groupYields[fillB][1] += hTN->GetBinContent(7);
      groupYields[fillB][1] += hTN->GetBinContent(8);
      groupYields[fillB][1] += hTN->GetBinContent(9);
      groupYields[fillB][2] += hTN->GetBinContent(11);
      groupYields[fillB][2] += hTN->GetBinContent(12);
      groupYields[fillB][2] += hTN->GetBinContent(13);

      groupUnc[fillB][0] += hTN->GetBinError(2) * hTN->GetBinError(2) ;
      groupUnc[fillB][0] += hTN->GetBinError(10)* hTN->GetBinError(10);
      groupUnc[fillB][1] += hTN->GetBinError(3) * hTN->GetBinError(3) ;
      groupUnc[fillB][1] += hTN->GetBinError(4) * hTN->GetBinError(4) ;
      groupUnc[fillB][1] += hTN->GetBinError(5) * hTN->GetBinError(5) ;
      groupUnc[fillB][1] += hTN->GetBinError(6) * hTN->GetBinError(6) ;
      groupUnc[fillB][1] += hTN->GetBinError(7) * hTN->GetBinError(7) ;
      groupUnc[fillB][1] += hTN->GetBinError(8) * hTN->GetBinError(8) ;
      groupUnc[fillB][1] += hTN->GetBinError(9) * hTN->GetBinError(9) ;
      groupUnc[fillB][2] += hTN->GetBinError(11)* hTN->GetBinError(11);
      groupUnc[fillB][2] += hTN->GetBinError(12)* hTN->GetBinError(12);
      groupUnc[fillB][2] += hTN->GetBinError(13)* hTN->GetBinError(13);
    }



    Plot * plot = new Plot(TString::Format("ttZNorm_%i",iS),SELNAMES[iS],"","");
    for(unsigned int iP = 0; PROC[iP][0]; ++iP){
      TH1F * h  = getHistogram(tree,var,TString::Format("ttZNorm_%u_%u",iS,iP),PROCNAMES[iP],6,0,300,SELS[iS],&PROC[iP],&weight);
      plot->addHist(h,PROCNAMES[iP],"",colorGetter(iP),0,colorGetter(iP));
//      plot->addToStack(h,PROCNAMES[iP],colorGetter(iP));
    }




//    TCanvas * c = new TCanvas;
//    plot->setXTitle("p_{T} (Z)");
//    plot->setTitle(" ");
//    plot->draw(c,false,"png");
  }

  cout << endl;
  for(unsigned int iS =0; iS < 8; ++iS){
    cout <<SELNAMES[iS] <<" \t";
  }
  cout << endl;

    for(unsigned int iB =0; iB < yields[0].size(); ++iB){
      for(unsigned int iS =0; iS < 8; ++iS){
      cout << yields[iS][iB] <<" \t";
    }
      cout << endl;
    }
  cout << endl;

  cout << endl;


  for(unsigned int iB =0; iB < groupYields[0].size(); ++iB){
    for(unsigned int iS =0; iS < 3; ++iS){
    cout << TString::Format("double M%u%u = %f",iS+1,iB+1,groupYields[iS][iB]) <<"; ";
  }
    cout << endl;
  }
cout << endl;

for(unsigned int iB =0; iB < groupYields[0].size(); ++iB){
  for(unsigned int iS =0; iS < 3; ++iS){
    cout << TString::Format("double EM%u%u = %f",iS+1,iB+1,groupUnc[iS][iB]) <<"; ";
}
  cout << endl;
}
cout << endl;

  vector<double> D(3);
  D[0] = groupYields[0][0] + groupYields[0][1] + groupYields[0][2];
  D[1] = groupYields[1][0] + groupYields[1][1] + groupYields[1][2];
  D[2] = groupYields[2][0] + groupYields[2][1] + groupYields[2][2];

      printSFAndUnc(groupYields,groupUnc,D);

      cout << endl;



}


void nMinusOnePlots(TTree * tree) {



    TString SELS[] = {"(nbjets >= 1 && njets >= 4 && passMed && nlbjets >= 2 && onZ == 1 && ptzll >= 100)",
        "(minlepPT >= 15 &&nbjets >= 1 && njets >= 4 && passMed && nlbjets >= 2)",
        "(minlepPT >= 15 &&nbjets >= 1 && njets >= 4 && passMed && nlbjets >= 2 && onZ == 1)",
        "(minlepPT >= 15 &&nbjets >= 1 && njets >= 2 && passMed && nlbjets >= 2 && onZ == 1 && ptzll >= 100)"
        ,"(minlepPT >= 15 &&nbjets >= 1 && njets >= 4 && passMed && nlbjets >= 1 && onZ == 1 && ptzll >= 100)"
        ,"(minlepPT >= 15 &&nbjets >= 0 && njets >= 4 && passMed && nlbjets >= 0 && onZ == 1 && ptzll >= 100)"

        ,""};
    TString SELNAMES[] = {"3rd lepton p_{T} >= 5", "No Z req.","p_{T} Z >= 0 GeV","N. jets >= 2","N. light b-Tags >= 1","N. medium b-Tags >= 0 (N. light >= 0)",""};
    TString SELSHORT[] = {"noLepPT", "noZ","noZPT","noNJ","noBL","noB",""};

  TString weight = "4*weight";

  vector<PlotInfo*> vars;
  vars.push_back (new PlotInfo("njets"        ,"njets"        ,"N. jets"                                    ,10,-.5,9.5));
  vars.push_back (new PlotInfo("nbjets"       ,"nbjets"       ,"N. b-jets"                                  ,4,-.5,3.5));
  vars.push_back (new PlotInfo("nlbjets"       ,"nlbjets"       ,"N. loose b-jets"                                  ,4,-.5,3.5));
  vars.push_back (new PlotInfo("ptzll"        ,"ptzll"        ,"p_{T}(Z)"                                   ,10,0,500));
  vars.push_back (new PlotInfo("minlepPT"        ,"minlepPT"        ,"minlepPT"                                   ,20,0,100));

  for(unsigned int iS = 0; SELS[iS][0]; ++iS){

    for(unsigned int iV = 0; iV < vars.size(); ++iV){
      Plot * plot = new Plot(TString::Format("ttZOneMinusPlots_%s_%s",SELSHORT[iS].Data(),vars[iV]->name.Data()),SELNAMES[iS],"","");
      for(unsigned int iP = 0; stdProc[iP][0]; ++iP){
        TH1F * h  = getHistogram(tree,*vars[iV],iP,SELS[iS],&stdProc[iP],&weight);
        PlotTools::normalize(h);
        plot->addHist(h,stdProcNames[iP],"",stdProcColors[iP],0,stdProcColors[iP]);
      }
      TCanvas * c = new TCanvas;
      plot->setXTitle(vars[iV]->xTitle);
      plot->setHeader(" "," ");
//      plot->draw(c,true,"png");
      plot->drawRatios(c,0, true,"png");
    }
  }
}

void DataMCComparisons(TTree * mcTree,TTree * dataTree){



  TString weight = "lumiWeight*puWeight*weight";

  TString SELS[] = {ttzHighPTSel,ttzSel,"(minlepPT >= 15 &&nbjets >= 1 && njets >= 2 && passMed && nlbjets >= 2 && onZ == 1 && ptzll >= 100 && passTrig >= 1)",
      noZSel,"(minlepPT >= 15 &&nbjets >= 1 && njets >= 4 && passMed && nlbjets >= 1 && onZ == 0 && passTrig >= 1)","(minlepPT >= 15 &&nbjets >= 1 && njets >= 2 && passMed && nlbjets >= 2 && onZ == 0 && passTrig >= 1)",
      noBHighPTSel,noBSel,"(minlepPT >= 15 &&nbjets == 0 && njets >= 2 && passMed  && onZ == 1 && ptzll >= 100  && passTrig >= 1)"
      ,""};

  TString SELNAMES[] = {"t#bar{t}Z Reg","t#bar{t}Z Reg, Z p_{T} >= 0","t#bar{t}Z Reg, N. jets >= 2",
      "No Z Reg","No Z Reg, N. light b-tags >= 1","No Z Reg, N. jets >= 2",
      "No b-Tag Reg","No b-Tag Reg, Z p_{T} >= 0","No b-Tag Reg, N. jets >= 2",
      ""};
TString SELSHORTNAMES[] = {"ttZ","ttZLowPT","ttZLowNJ",
    "noZ","noZLowB","noZLowNJ",
    "noB","noBLowPT","noBLowNJ",
    ""
};


vector<PlotInfo*> vars;
vars.push_back (new PlotInfo("njets"        ,"njets"        ,"N. jets"                                    ,8,1.5,9.5));
vars.push_back (new PlotInfo("ptzll"        ,"ptzll"        ,"p_{T}(Z)"                                   ,10,0,500));

for(unsigned int iS = 0; SELS[iS][0]; ++iS){

  for(unsigned int iV = 0; iV < vars.size(); ++iV){
    Plot * plot = new Plot(TString::Format("ttZDataMCPlots_atNLOZ_%s_%s",SELSHORTNAMES[iS].Data(),vars[iV]->name.Data()),SELNAMES[iS],"","");

    TH1F * hD  = getHistogram(dataTree,*vars[iV],"data",SELS[iS]);
    plot->addHist(hD,"data","",StyleTools::color_data,0,StyleTools::color_data);

    for(unsigned int iP = 0; stdProc[iP][0]; ++iP){
      TH1F * h  = getHistogram(mcTree,*vars[iV],iP,SELS[iS],&stdProc[iP],&weight);
//      plot->addHist(h,stdProcNames[iP],"",stdProcColors[iP],0,stdProcColors[iP]);
      plot->addToStack(h,stdProcNames[iP],stdProcColors[iP]);
    }
    TCanvas * c = new TCanvas;
    plot->setXTitle(vars[iV]->xTitle);
    plot->setHeader("13 TeV, L = 0.57 pb^{-1}"," ");
//      plot->draw(c,true,"png");
    plot->drawRatioStack(c,true,"png");
  }
}

}


void testVariables(TTree * tree) {

//  TString SELS[] = {"(nbjets >= 1 && nlbjets >= 2 && njets >= 4 )","","",""};
//  TString SELNAMES[] = {"nJ >= 4, nBJ >= 1, nLB >= 2","","",""};

    TString SELS[] = {"(nbjets >= 1 && njets >= 4 && passMed && ptzll >= 100 && nlbjets >= 2 && minlepPT >= 10)","","",""};
    TString SELNAMES[] = {"nJ >= 4, nBJ >= 1","","",""};

  TString PROC[] = {"*(process == 7)","*(process == 9)","*(process == 2)","*(process==4)","*(process == 8)","*(process == 6)",""};
  TString PROCNAMES[] = {"t#bar{t}Z","VV","t#bar{t}","Z#rightarrowll","t#bar{t}W","tV",""};

  TString weight = "4*weight";

  vector<PlotInfo*> vars;
  vars.push_back (new PlotInfo("met"          ,"met"          ,"#slash{E}_{T}"                              ,20,0,200));
  vars.push_back (new PlotInfo("njets"        ,"njets"        ,"N. jets"                                    ,10,-.5,9.5));
  vars.push_back (new PlotInfo("nbjets"       ,"nbjets"       ,"N. b-jets"                                  ,4,-.5,3.5));
  vars.push_back (new PlotInfo("nlbjets"       ,"nlbjets"       ,"N. loose b-jets"                                  ,4,-.5,3.5));
  vars.push_back (new PlotInfo("ntbjets"       ,"ntbjets"       ,"N. tight b-jets"                                  ,4,-.5,3.5));
  vars.push_back (new PlotInfo("ht"           ,"ht"           ,"H_{T}"                                      ,20,0,1000));
  vars.push_back (new PlotInfo("ptzll"        ,"ptzll"        ,"p_{T}(Z)"                                   ,10,0,500));
  vars.push_back (new PlotInfo("mtcsv12metNoLep"        ,"mtcsv12metNoLep"        ,"m_{T} (b)"                                   ,10,0,300));
  vars.push_back (new PlotInfo("ptw"        ,"ptw"        ,"pt(W)"                                   ,20,0,200));
  vars.push_back (new PlotInfo("minlepPT"        ,"minlepPT"        ,"minlepPT"                                   ,20,0,100));
  //  vars.push_back (new PlotInfo("nctt"        ,"nctt"        ,"# of fat jets"                                   ,5,-.5,4.5));
//  vars.push_back (new PlotInfo("ncttstd"        ,"ncttstd"        ,"# of ctt"                                   ,5,-.5,4.5));
//  vars.push_back (new PlotInfo("clean_ncttstd"        ,"clean_ncttstd"        ,"# of cleaned ctt"                                   ,5,-.5,4.5));


  for(unsigned int iS = 0; SELS[iS][0]; ++iS){

    for(unsigned int iV = 0; iV < vars.size(); ++iV){
      Plot * plot = new Plot(TString::Format("ttZNorm_%i",iS),SELNAMES[iS],"","");
      for(unsigned int iP = 0; PROC[iP][0]; ++iP){
        TH1F * h  = getHistogram(tree,*vars[iV],PROCNAMES[iP],SELS[iS],&PROC[iP],&weight);
        PlotTools::normalize(h);
        plot->addHist(h,PROCNAMES[iP],"",colorGetter(iP),0,colorGetter(iP));
      }
      TCanvas * c = new TCanvas;
      plot->setXTitle(vars[iV]->xTitle);
      plot->draw(c,false,"png");
    }
  }
}

int colorMap(int procInt){
  switch (procInt){
  case 0 : return StyleTools::color_ttZ;
  case 1 : return StyleTools::color_wjets;
  case 2 : return StyleTools::color_ttbar;
  case 3 : return StyleTools::color_znunu;
  case 4 : return StyleTools::color_ttW;
  case 5 : return StyleTools::color_tW;
  }
  return StyleTools::color_qcd;
}


#endif

void getTTZNormalization(TString fileName, TString dataFileName = "data.root", TString treeName = "Events")
{
  StyleTools::SetStyle();
    TFile * mf = new TFile(fileName,"read");
    TTree * mt =0;
    mf->GetObject(treeName,mt);

    TFile * df = new TFile(dataFileName,"read");
    TTree * dt =0;
    df->GetObject(treeName,dt);

  getNormalization(mt);
//    nMinusOnePlots(mt);
//    testVariables(mt);

//  DataMCComparisons(mt,dt);
}
