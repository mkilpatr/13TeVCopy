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


void testEfficiencies(){

  TString SAMPLES[] = {"znunu_test.root","ttbar_test.root","wlnu_test.root",""};
  TString SAMPLENAMES[] = {"Z#rightarrow#nu#nu","t#bar{t}","W#rightarrowl#nu",""};
  vector<TTree *> trees;
  for(unsigned int iS = 0; SAMPLES[iS][0]; ++iS){
    TFile * f = new TFile(SAMPLES[iS],"read");
    trees.push_back(0);
    f->GetObject("Events",trees.back());
  }

  bool combineSyst = true;

  TString baseSel = "passBaseline >= 1";



  TString VARS[] = {"nBL","nBM","nBT",""};
  TString VARNAMES[] = {"# of loose b-Tags","# of medium b-Tags","# of tight b-Tags",""};
  TString WEIGHTS[] = {"weight*puWeight","weight*puWeight*btWeight","weight*puWeight*btUlWeight","weight*puWeight*btDlWeight","weight*puWeight*btUHWeight","weight*puWeight*btDHWeight",""};
  TString WEIGHTNAMES[] = {"None","Nominal","Up Light","Down Light","Up Heavy","Down Heavy",""};


  for(unsigned int iS = 0; SAMPLES[iS][0]; ++iS){

    for(unsigned int iWP = 0; VARS[iWP][0]; ++iWP){
      TString stackTitle = TString::Format("%s;%s",SAMPLENAMES[iS].Data(),VARNAMES[iWP].Data() );
      Plot * plot = new Plot(TString::Format("btagtest_%u_%u",iS,iWP),stackTitle,"","");
      vector<TH1F*> histograms;
      for(unsigned int iW = 0; WEIGHTS[iW][0]; ++iW){
        TString histName = TString::Format("%s_%s_%i_0",SAMPLENAMES[iS].Data(),VARS[iWP].Data(), iW );
        TH1F * h  = getHistogram(trees[iS],VARS[iWP],histName,WEIGHTNAMES[iW],4,.5,4.5,baseSel,0,&WEIGHTS[iW]);
        histograms.push_back(h);
      }

      for(unsigned int iH = 0; iH < histograms.size(); ++iH){
        for(unsigned int iB = 0; iB <= histograms[iH]->GetNbinsX()+1; ++iB){
          histograms[iH]->SetBinError(iB,0);
        }
      }

      if(combineSyst){
        plot->addHist(histograms[0],WEIGHTNAMES[0],"",1,0,1); //None;
        plot->addHist(histograms[1],WEIGHTNAMES[1],"",2,0,2); //Nominal;

        TH1F * upSyst = (TH1F*)histograms[1]->Clone();
        TH1F * downSyst = (TH1F*)histograms[1]->Clone();

        for(unsigned int iB = 0; iB <= upSyst->GetNbinsX()+1; ++iB){
          double var1 = histograms[2]->GetBinContent(iB) - histograms[3]->GetBinContent(iB);
          double var2 = histograms[4]->GetBinContent(iB) - histograms[5]->GetBinContent(iB);
          double syst = 0.5*TMath::Sqrt(var1*var1+var2*var2);
          upSyst->SetBinContent(iB,histograms[1]->GetBinContent(iB) +syst );
          downSyst->SetBinContent(iB,histograms[1]->GetBinContent(iB) - syst );
        }
        plot->addHist(upSyst,"Up","",3,0,3); //up;
        plot->addHist(downSyst,"Down","",4,0,4); //down;

      } else {
        for(unsigned int iH = 0; iH < histograms.size(); ++iH){
          int color = iH + 1;
          if(color>= 10) color++;
          plot->addHist(histograms[iH],WEIGHTNAMES[iH],"",color,0,color);
        }

      }


      TCanvas * c = new TCanvas;
      plot->setXTitle(VARNAMES[iWP].Data());
      plot->drawRatios(c,0,true,"png");
    }

  }
}

void getUnc()
{
    TString TYPE[] = {"zeroLepSR","oneLepSR",""};
    TString VAR[] = {"NONE","NOMINAL","UL","DL","UH","DH",""};
    TString VARN[] = {"None","Nominal","Up light","Down light","Up heavy","Down heavy",""};

//    TString WGT[] = {"weight*puWeight*btagWeight*lepvetoweight * ((nvetolep==0 && nvetohpstaus==0) || (ismc && (ngoodgenele>0 || ngoodgenmu>0 || npromptgentau>0)))",
//                     "weight*puWeight*btagWeight*lepselweight * (nvetolep>0 && absdphilepw<1)",""};

    TString WGT[] = {"weight*btagWeight * (nsellep==0 && nvetohpstaus==0)",
                     "weight*btagWeight * (nsellep>0 && mtlepmet<100)",""};

    TString SEL = "(passcscflt) && (passeebadscflt) && (passhbheflt) && (met>250 && j1pt>75 && j2pt>75 && njets>=5 && nlbjets>=2 && nbjets>=1 && dphij12met>1 && dphij3met>0.5 && mtcsv12met>175)";

    TString BINS[] = {"*(nbjets==1 && ncttstd == 0)","*(nbjets>=2 && ncttstd == 0)","*(nbjets==1 && ncttstd > 0)","*(nbjets>=2 && ncttstd > 0)",""};
    TString BINONES[] = {"*(nbjets>=1 && ncttstd == 0)","*(nbjets>=1 && ncttstd == 0)","*(nbjets>=1 && ncttstd > 0)","*(nbjets>=1 && ncttstd > 0)",""};
    TString BINNAMES[] = {"1_0","2_0","1_1","2_1"};

    double bins[] = {250,300,400,500,600,700};
    int nBins = 5;

    bool combineSyst = false;


    vector<TTree*> treesZero;
    vector<TTree*> treesOne;
    for(unsigned int iV = 0; VAR[iV][0]; ++iV){
        TFile * f = new TFile("zeroLepSR/" + VAR[iV] + ".root" ,"read");
        TTree * t = 0;
        f->GetObject("Events",t);
        treesZero.push_back(t);
    }
    for(unsigned int iV = 0; VAR[iV][0]; ++iV){
        TFile * f = new TFile("oneLepSR/" + VAR[iV] + ".root" ,"read");
        TTree * t = 0;
        f->GetObject("Events",t);
        treesOne.push_back(t);
    }


    //plot comp
//    for(unsigned int iT = 0; TYPE[iT][0]; ++iT){
//        Plot * plot = new Plot(TString::Format("test_%u",iT),TYPE[iT],"","");
//        for(unsigned int iV = 0; VAR[iV][0]; ++iV){
//            TTree * t = treesZero[iV];
//            if(iT == 1) t = treesOne[iV];
//            TH1F * h  = getHistogram(t,"met",TString::Format("%u_%u_0",iT,iV),VAR[iV],nBins,bins,SEL,0,&WGT[iT]);
//
//            int color = iV + 1;
//            if(color>= 10) color++;
//            plot->addHist(h,VAR[iV],"",color,0,color);
//        }
//
//        TCanvas * c = new TCanvas;
//        plot->drawRatios(c,0,true,"png");
//    }

    // get TF

    vector<vector<float> >  systVal(4); //"UL","DL","UH","DH"
    for(unsigned int iB = 0; BINS[iB][0]; ++iB){
      Plot * plot = new Plot(TString::Format("TF_%u",iB),BINS[iB],"","");
      vector<TH1F*> histograms;
      vector<double> systValues;
      for(unsigned int iV = 0; VAR[iV][0]; ++iV){

        TH1F * hZero  = getHistogram(treesZero[iV],"met",TString::Format("zero_%u_%u_0",iB,iV),VAR[iV],nBins,bins,SEL,&BINS[iB],&WGT[0]);
        TH1F * hOne  = getHistogram(treesOne[iV],"met",TString::Format("one_%u_%u_0",iB,iV),VAR[iV],nBins,bins,SEL,&BINONES[iB],&WGT[1]);
        hZero->Divide(hOne);
        histograms.push_back(hZero);
      }
      for(unsigned int iH = 0; iH < histograms.size(); ++iH){
        for(unsigned int iB = 0; iB <= histograms[iH]->GetNbinsX()+1; ++iB){
          histograms[iH]->SetBinError(iB,0);
        }
      }

      if(combineSyst){
        plot->addHist(histograms[0],VAR[0],"",1,0,1); //None;
        plot->addHist(histograms[1],VAR[1],"",2,0,2); //Nominal;

        TH1F * upSyst = (TH1F*)histograms[1]->Clone();
        TH1F * downSyst = (TH1F*)histograms[1]->Clone();

        for(unsigned int iB2 = 0; iB2 <= upSyst->GetNbinsX()+1; ++iB2){
          double var1 = histograms[2]->GetBinContent(iB2) - histograms[3]->GetBinContent(iB2);
          double var2 = histograms[4]->GetBinContent(iB2) - histograms[5]->GetBinContent(iB2);
          double syst = 0.5*TMath::Sqrt(var1*var1+var2*var2);
          upSyst->SetBinContent(iB2,histograms[1]->GetBinContent(iB2) +syst );
          downSyst->SetBinContent(iB2,histograms[1]->GetBinContent(iB2) - syst );
        }
        plot->addHist(upSyst,"Up","",3,0,3); //up;
        plot->addHist(downSyst,"Down","",4,0,4); //down;
      } else {
        for(unsigned int iH = 0; iH < histograms.size(); ++iH){
          int color = iH + 1;
          if(color>= 10) color++;
          plot->addHist(histograms[iH],VAR[iH],"",color,0,color);
        }
        for(unsigned int iH = 2; iH < histograms.size(); ++iH){
          for(unsigned int iB2 = 1; iB2 <= histograms[iH]->GetNbinsX(); ++iB2){
            systVal[iH-2].push_back(histograms[iH]->GetBinContent(iB2)/histograms[1]->GetBinContent(iB2));
          }
        }
        for(unsigned int iH = 0; iH < systVal[0].size(); ++iH)
          cout << systVal[0][iH] <<" ";
        cout << endl;
      }
      TCanvas * c = new TCanvas;
      plot->drawRatios(c,0,true,"png");

    }

    vector<TH1F*> systPlot;  //"UL","DL","UH","DH"
    Plot * ratioPL = new Plot("TRL","","","");
    Plot * ratioPH = new Plot("TRH","","","");
    for(unsigned int iT = 0; iT < 4; ++iT){
      TH1F * h = new TH1F(TString::Format("TR_%u",iT),"",20,-.5,19.5);
      for(unsigned int iB2 = 0; iB2 < systVal[iT].size(); ++iB2 )
        h->SetBinContent(iB2+1,systVal[iT][iB2]);
      int color = iT + 1;
      if(color>= 10) color++;
      if(iT <= 1)
        ratioPL->addHist(h,VARN[iT + 2],"",color,0,color);
      else
        ratioPH->addHist(h,VARN[iT + 2],"",color,0,color);

    }
    ratioPL->setHeader("","");
    ratioPH->setHeader("","");
    ratioPL->setYRange(0.8,1.2);
    ratioPH->setYRange(0.8,1.2);
    TCanvas * c = new TCanvas;
    ratioPL->draw(c,true,"png");
    TCanvas * c2 = new TCanvas;
    ratioPH->draw(c2,true,"png");

////     get UNC
//    for(unsigned int iT = 0; TYPE[iT][0]; ++iT){
//      cout << endl << endl <<" "<< TYPE[iT]<<endl;
//    for(unsigned int iB = 0; BINS[iB][0]; ++iB){
//      vector<TH1F*> histograms;
//      for(unsigned int iV = 0; VAR[iV][0]; ++iV){
//        TH1F *h = 0;
//        if(iT == 0){
//          h  = getHistogram(treesZero[iV],"met",TString::Format("zero_%u_%u_0",iB,iV),VAR[iV],nBins,bins,SEL,&BINS[iB],&WGT[0]);
//        } else {
//          h  = getHistogram(treesOne[iV],"met",TString::Format("one_%u_%u_0",iB,iV),VAR[iV],nBins,bins,SEL,&BINONES[iB],&WGT[1]);
//        }
//        histograms.push_back(h);
//      }
//
//        for(unsigned int iB2 = 1; iB2 <= histograms[0]->GetNbinsX(); ++iB2){
//        cout <<TString::Format("bin_%.0f_%s\tbtageff_light_ttbarW\tttbar+W\t%.2f\n",bins[iB2-1],BINNAMES[iB].Data(),(histograms[2]->GetBinContent(iB2)-histograms[1]->GetBinContent(iB2))/histograms[1]->GetBinContent(iB2) + 1.0);
//        cout <<TString::Format("bin_%.0f_%s\tbtageff_heavy_ttbarW\tttbar+W\t%.2f\n",bins[iB2-1],BINNAMES[iB].Data(),(histograms[4]->GetBinContent(iB2)-histograms[1]->GetBinContent(iB2))/histograms[1]->GetBinContent(iB2) + 1.0);
//      }
//    }
//    }




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
//  makeEffs();
//  testEfficiencies();
  getUnc();
}
