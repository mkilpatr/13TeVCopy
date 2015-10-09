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

void checkSelection(TString name, TTree * tree){

  double jetPTs[] = {20,25,30,35,40,45,50,60,80,100,120,150,180,220,270,350,450,550,650,1000};
  int nJetPTS = 19;

  TString SELS[] = {"passBaseline >= 0","","passBaseline >= 1",""};
  TString SELNAMES[] = {"","Baseline",""};
  TString CSVSELS[] = {"csv >= 0.605","csv >= 0.890","csv >= 0.970",""};
  TString CSVNAMES[] = {"Loose","Medium","Tight",""};
  TString FLVSELS[] = {"flavor == 4","flavor == 3","(flavor == 2 || flavor == 6)","(flavor == 1 || flavor == 5)","(flavor == 0 || flavor == 9)",""};
  TString FLVNAMES[] = {"B","C","UDS","G","O",""};

//  TString FLVSELS[] = {"flavor == 4","flavor == 3","(flavor == 2 || flavor == 6)","(flavor == 1 || flavor == 5)","flavor == 0","flavor == 9", ""};
//  TString FLVNAMES[] = {"B","C","UDS","G","U","NJ",""};

//
//  TString FLVSELS[] = {"flavor == 0","flavor == 1","flavor == 2","flavor == 5","flavor == 6","flavor == 7","flavor == 8","flavor == 9",""};
//  TString FLVNAMES[] = {"0","1","2","5","6","7","8","9",""};

//
//  double jetETAs[] = {0,1.1,1.7,2.3,2.8,3.2,5};
//  int nJetETAS = 6;

  TString weightSel = "weight*puWeight";

//  TObjArray * o = new TObjArray;
for(unsigned int iS =0; SELS[iS][0]; ++iS){
  vector<TH1F*> inclHistos;
  for(unsigned int iF =0; FLVSELS[iF][0]; ++iF){
    TString sel = TString::Format("%s && %s",SELS[iS].Data(),FLVSELS[iF].Data() );
    TString histName = TString::Format("%s_%s_%i_0",name.Data(),FLVNAMES[iF].Data(), iS );
    TH1F * hD  = getHistogram(tree,"pt",histName,FLVNAMES[iF],nJetPTS,jetPTs, sel,0, &weightSel, 0 );
    inclHistos.push_back(hD);
  }

  for(unsigned int iC =0; CSVSELS[iC][0]; ++iC){
    TString stackTitle = TString::Format("%s %s %s",name.Data(), SELNAMES[iS].Data(),CSVNAMES[iC].Data());
    Plot * plot = new Plot(TString::Format("btageff_%s_%i_%i",name.Data(),iS,iC),stackTitle,"","Pass/All");


    for(unsigned int iF =0; FLVSELS[iF][0]; ++iF){
      TString sel = TString::Format("%s && %s && %s",SELS[iS].Data(),FLVSELS[iF].Data(),CSVSELS[iC].Data() );
      TString histName = TString::Format("%s_%s_%i_%i",name.Data(),FLVNAMES[iF].Data(), iS,iC+1 );
      TH1F * hD  = getHistogram(tree,"pt",histName,FLVNAMES[iF],nJetPTS,jetPTs,sel,0,&weightSel, iC+1);
//      TGraphAsymmErrors * hR = EffPlotTools::computeEffGraph(hD,inclHistos[iF]);

      hD->Divide(hD,inclHistos[iF],1,1,"b");

      int color = iF+1;
      if(color>= 10) color++;
//      plot->addGraph(hR,FLVNAMES[iF],"",color,0,color);
      plot->addHist(hD,FLVNAMES[iF],"",color,0,color);
    }

    TCanvas * c = new TCanvas;
    plot->setLogx();
    plot->setYRange(0,1);
    plot->draw(c,true,"png");
//    o->Add(c);
  }
}

//StyleTools::drawAll(o,"");

}

void checkEtaBinning(TString name, TTree * tree){

  double jetPTs[] = {20,25,30,35,40,45,50,60,80,100,120,150,180,220,270,350,450,550,650,1000};
  int nJetPTS = 19;

  double jetPT2s[] = {20,25,30,35,40,45,50,60,80,100,120,150,180,220,270,350,450,550,1000};
  int nJetPT2s = 18;

  double jetPT3s[] = {20,25,30,35,40,45,50,60,80,100,120,150,180,220,270,350,450,1000};
  int nJetPT3s = 17;

  double jetPT4s[] = {20,25,30,35,40,45,50,60,80,100,120,150,180,220,270,350,1000};
  int nJetPT4s = 16;

  double jetPTDoubles[] = {20,25,30,40,50,75,100,150,200,300,500,1000};
  int nJetPTDoubles = 11;

  double jetPTOthers[] = {20,30,35,40,45,50,60,80,1000};
  int nJetPTOthers = 8;

  double jetETAs[] = {0,1.2,2.1,2.4};
  int nJetETAS = 3;

  TString CSVSELS[] = {"csv >= 0.605","csv >= 0.890","csv >= 0.970",""};
  TString CSVNAMES[] = {"Loose","Medium","Tight",""};
  TString FLVSELS[] = {"flavor == 4","flavor == 3","(flavor == 2 || flavor == 6)","(flavor == 1 || flavor == 5)","(flavor == 0 || flavor == 9)",""};
  TString FLVNAMES[] = {"B","C","UDS","G","O",""};

  TString weightSel = "weight*puWeight";

  for(unsigned int iF =0; FLVSELS[iF][0]; ++iF){
    TString stackTitle = TString::Format("%s %s",name.Data(),FLVNAMES[iF].Data() );
    Plot * plot = new Plot(TString::Format("btageff_eta_%s_%s",name.Data(),FLVNAMES[iF].Data()),stackTitle,"","Pass/All");

    vector<TH1F*> inclHistos;
    for(unsigned int iE = 0; iE < nJetETAS+1; ++iE){
      TString sel;
      if(iE < nJetETAS)
        sel = TString::Format("( absEta >= %.2f && absEta < %.2f) && %s",jetETAs[iE],jetETAs[iE+1],FLVSELS[iF].Data() );
      else
        sel = TString::Format("%s",FLVSELS[iF].Data() );
      TString histName = TString::Format("%s_%s_%i_0",name.Data(),FLVNAMES[iF].Data(), iE );

      int thisBin = nJetPTS;
      double* thisBinning = jetPTs;

      if(iF == 4){
        thisBin =nJetPTOthers;
        thisBinning =jetPTOthers;
      } else if ( name.BeginsWith("w") && (iF == 0 || iF == 1)){
        thisBin =nJetPTDoubles;
        thisBinning =jetPTDoubles;
      }else if ( iF == 2 ) {
        thisBin =nJetPT2s;
        thisBinning =jetPT2s;
      }else if ( iF == 3) {
        thisBin =nJetPT3s;
        thisBinning =jetPT3s;
      }else if ( iF == 1) {
        thisBin =nJetPT3s;
        thisBinning =jetPT3s;
      }

      TH1F * hD  = getHistogram(tree,"pt",histName,FLVNAMES[iF],thisBin,thisBinning, sel,0, &weightSel, 0 );
      inclHistos.push_back(hD);
    }

    int n = 1;
    for(unsigned int iC =0; CSVSELS[iC][0]; ++iC){
      for(unsigned int iE = 0; iE < nJetETAS; ++iE){
        TString sel = TString::Format("( absEta >= %.2f && absEta < %.2f) && %s && %s",jetETAs[iE],jetETAs[iE+1],FLVSELS[iF].Data(), CSVSELS[iC].Data()  );

        if(iF == 4){
          sel = TString::Format(" %s && %s",FLVSELS[iF].Data(), CSVSELS[iC].Data()  );
        }        TString histName = TString::Format("%s_%s_%i_%i",name.Data(),FLVNAMES[iF].Data(), iE,iC+1 );

        int thisBin = nJetPTS;
        double* thisBinning = jetPTs;

        if(iF == 4){
          thisBin =nJetPTOthers;
          thisBinning =jetPTOthers;
        } else if ( name.BeginsWith("w") && (iF == 0 || iF == 1)){
          thisBin =nJetPTDoubles;
          thisBinning =jetPTDoubles;
        }else if ( iF == 2 ) {
          thisBin =nJetPT2s;
          thisBinning =jetPT2s;
        }else if ( iF == 3) {
          thisBin =nJetPT3s;
          thisBinning =jetPT3s;
        }else if ( iF == 1) {
          thisBin =nJetPT3s;
          thisBinning =jetPT3s;
        }

        TH1F * hD  = getHistogram(tree,"pt",histName,FLVNAMES[iF],thisBin,thisBinning,sel,0,&weightSel, iC+1);
        hD->Divide(hD,iF == 4 ? inclHistos[nJetETAS] : inclHistos[iE],1,1,"b");

        int color = n;
        if(color>= 10) color++;
        plot->addHist(hD,TString::Format("%.1f <= |#eta| < %.1f, %s",jetETAs[iE],jetETAs[iE+1],CSVNAMES[iC].Data()  ),"",color,0,color);
        n++;
      }
    }

    TCanvas * c = new TCanvas;
    plot->setLogx();
    plot->setYRange(0,1);
    plot->draw(c,true,"png");

  }

}

void makeEffs(){
  const TString treeName = "Events";
  TFile * lf = new TFile("wz_baseline.root","read");
  TTree * lt =0;
  lf->GetObject(treeName,lt);
  TFile * tf = new TFile("ttbar-powheg_baseline.root","read");
  TTree * tt =0;
  tf->GetObject(treeName,tt);

  enum axis {TYPE,FLAVOR,ETA,WP};
  QuickRefold::TH1FContainer * a = new QuickRefold::TH1FContainer("CSVEff",4);
  a->addAxis(TYPE,"TYPE",2,-.5,1.5);//nonTTBAR / TTBAR
  a->addAxis(FLAVOR,"FLAVOR",5,-.5,4.5);// B/C/UDS/G/O

  float jetETAs[] = {0,1.2,2.1,2.4};
  int nJetETAS = 3;
  a->addAxis(ETA,"ETA",nJetETAS,jetETAs);
  a->addAxis(WP,"WP",3,-.5,2.5); // L M T
  a->stopSetup();



  double jetPTs[] = {20,25,30,35,40,45,50,60,80,100,120,150,180,220,270,350,450,550,650,1000};
  int nJetPTS = 19;

  double jetPT2s[] = {20,25,30,35,40,45,50,60,80,100,120,150,180,220,270,350,450,550,1000};
  int nJetPT2s = 18;

  double jetPT3s[] = {20,25,30,35,40,45,50,60,80,100,120,150,180,220,270,350,450,1000};
  int nJetPT3s = 17;

  double jetPT4s[] = {20,25,30,35,40,45,50,60,80,100,120,150,180,220,270,350,1000};
  int nJetPT4s = 16;

  double jetPTDoubles[] = {20,25,30,40,50,75,100,150,200,300,1000};
  int nJetPTDoubles = 10;

  double jetPTOthers[] = {20,30,35,40,45,50,60,80,1000};
  int nJetPTOthers = 8;

  TString CSVSELS[] = {"csv >= 0.605","csv >= 0.890","csv >= 0.970",""};
  TString CSVNAMES[] = {"Loose","Medium","Tight",""};
  TString FLVSELS[] = {"flavor == 4","flavor == 3","(flavor == 2 || flavor == 6)","(flavor == 1 || flavor == 5)","(flavor == 0 || flavor == 9)",""};
  TString FLVNAMES[] = {"B","C","UDS","G","O",""};

  TString weightSel = "weight*puWeight";

  for(unsigned int iT = 0; iT < 2; ++iT){
    TTree * tree = iT ? tt :lt;
    TString name = iT ? "ttbar" : "wz";
    a->setBin(TYPE,iT);

    for(unsigned int iF =0; FLVSELS[iF][0]; ++iF){
      a->setBin(FLAVOR,iF);

      vector<TH1F*> inclHistos;
      TH1F*  inclusiveETA = 0;
      for(unsigned int iE = 0; iE < nJetETAS +1; ++iE){
        TString sel;
        if(iE < nJetETAS)
          sel = TString::Format("( absEta >= %.2f && absEta < %.2f) && %s",jetETAs[iE],jetETAs[iE+1],FLVSELS[iF].Data() );
        else
          sel = TString::Format("%s",FLVSELS[iF].Data() );

        TString histName = TString::Format("%s_%s_%i_0",name.Data(),FLVNAMES[iF].Data(), iE );



        int thisBin = nJetPTS;
        double* thisBinning = jetPTs;

        if(iF == 4){
          thisBin =nJetPTOthers;
          thisBinning =jetPTOthers;
        } else if ( name.BeginsWith("w") && (iF == 0 || iF == 1)){
          thisBin =nJetPTDoubles;
          thisBinning =jetPTDoubles;
        }else if ( iF == 2 ) {
          thisBin =nJetPT3s;
          thisBinning =jetPT3s;
        }else if ( iF == 3) {
          thisBin =nJetPT4s;
          thisBinning =jetPT4s;
        }else if ( iF == 1) {
          thisBin =nJetPT4s;
          thisBinning =jetPT4s;
        }else if ( iF == 0) {
          thisBin =nJetPT2s;
          thisBinning =jetPT2s;
        }
        TH1F * hD  = getHistogram(tree,"pt",histName,FLVNAMES[iF],thisBin,thisBinning, sel,0, &weightSel, 0 );
        inclHistos.push_back(hD);
      }

      int n = 1;
      for(unsigned int iC =0; CSVSELS[iC][0]; ++iC){
        a->setBin(WP,iC);
        for(unsigned int iE = 0; iE < nJetETAS; ++iE){
          a->setBin(ETA,(jetETAs[iE] + jetETAs[iE+1] )/2);

          TString sel = TString::Format("( absEta >= %.2f && absEta < %.2f) && %s && %s",jetETAs[iE],jetETAs[iE+1],FLVSELS[iF].Data(), CSVSELS[iC].Data()  );

          if(iF == 4){
            sel = TString::Format(" %s && %s",FLVSELS[iF].Data(), CSVSELS[iC].Data()  );
          }

          TString histName = TString::Format("%s_%s_%i_%i",name.Data(),FLVNAMES[iF].Data(), iE,iC+1 );

          int thisBin = nJetPTS;
          double* thisBinning = jetPTs;

          if(iF == 4){
            thisBin =nJetPTOthers;
            thisBinning =jetPTOthers;
          } else if ( name.BeginsWith("w") && (iF == 0 || iF == 1)){
            thisBin =nJetPTDoubles;
            thisBinning =jetPTDoubles;
          }else if ( iF == 2 ) {
            thisBin =nJetPT3s;
            thisBinning =jetPT3s;
          }else if ( iF == 3) {
            thisBin =nJetPT4s;
            thisBinning =jetPT4s;
          }else if ( iF == 1) {
            thisBin =nJetPT4s;
            thisBinning =jetPT4s;
          }else if ( iF == 0) {
            thisBin =nJetPT2s;
            thisBinning =jetPT2s;
          }

          TH1F * hD  = getHistogram(tree,"pt",histName,FLVNAMES[iF],thisBin,thisBinning,sel,0,&weightSel, iC+1);
          hD->Divide(hD,iF == 4 ? inclHistos[nJetETAS] : inclHistos[iE],1,1,"b");

          cout << name << " "<< FLVNAMES[iF] <<" "<< CSVNAMES[iC]<<" "<< iE << endl;
          for(unsigned int iB = 1; iB <= hD->GetNbinsX(); ++iB){
            cout << hD->GetBinContent(iB)<<" ";
            if(hD->GetBinContent(iB) == 0 || hD->GetBinContent(iB) > 0.95)
              cout << endl << "ERROR!!!"<<endl;
          }
          cout << endl;

          a->setValue(*hD);

        }
      }

    }
  }


  TFile * f = new TFile("csvEffs.root","recreate");
  a->Write();
  f->Close();


}


#endif

//plotting
//void GetBtagEff(const TString inFile="qcd_jetRes.root", const TString name = "znunu", const TString treeName = "Events")
//{
//  StyleTools::SetStyle();
//  TFile * mf = new TFile(inFile,"read");
//  TTree * mt =0;
//  mf->GetObject(treeName,mt);
////  checkSelection(name,mt);
//  checkEtaBinning(name,mt);
//}

////producing
void GetBtagEff()
{
//  TFile * oF = new TFile(outFile,"recreate");
  StyleTools::SetStyle();
  makeEffs();
}
