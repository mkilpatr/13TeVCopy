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

  void comp(TTree * oTree,TTree * sTree){

//    TString preSel[] = {"nJ >= 0","","nJ >= 2","nJ >= 5",""};
//    TString preSel[] = {"nJ >= 2","nJ >= 5","nJ >= 2 && nB >= 2",""};
//    TString preSel[] = {"nJ >= 5","","nJ >= 5 && nB >= 1 ","nJ >= 5 && nB >= 2 ","nJ >= 5 && nB >= 1  && mtB >= 175","nJ >= 5 && nB >= 2  && mtB >= 175",""};

//    TString preSel[] = {"nJ >= 5","nJ >= 5 && nB >= 2 ","nJ >= 5 && nB >= 2  && mtB >= 175",""};
//    TString preSel[] = {"nJ >= 2","nJ >= 2 && nB >= 2 ","nJ >= 2 && nB >= 2  && mtB >= 175",""};

//    TString preSel[] = {"nJ >= 5 && mtB >= 175 && nBl >=2 && nB >= 1  && nT == 0"
////                        "nJ >= 5 && mtB >= 175 && nBl >=2 && nB == 1  && nT >= 1"
////                       ,"nJ >= 5 && mtB >= 175 && nBl >=2 && nB >= 2  && nT == 0"
////                       ,"nJ >= 5 && mtB >= 175 && nBl >=2 && nB >= 2  && nT >= 1"
//                       ,""};
//    TString preSel[] = {"nJ >= 5 && nT == 0 && nB == 1 && mtB >= 175","nJ >= 5 && nT >= 1 && nB >= 2 && mtB >= 175",""};

    TString preSel[] = {
//                          "nJ >= 5 && mtB >= 175 && nBl >=2 && nB == 1  && nT == 0",
                        "nJ >= 5 && mtB >= 175 && nBl >=2 && nB == 1  && nT >= 1",
//                        "nJ >= 5 && mtB >= 175 && nBl >=2 && nB >= 2  && nT == 0",
//                        "nJ >= 5 && mtB >= 175 && nBl >=2 && nB >= 2  && nT >= 1",
//                       ,"nJ >= 5 && mtB >= 175 && nBl >=2 && nB >= 2  && nT >= 1"
                       ""};

    TString preSel2[] = {"nJ >= 5 && mtB >= 175 && nBl >=2 && nB == 1  && nT == 0",
                         "nJ >= 5 && mtB >= 175 && nBl >=2 && nB == 1  && nT >= 1",
                         "nJ >= 5 && mtB >= 175 && nBl >=2 && nB >= 2  && nT == 0",
                         "nJ >= 5 && mtB >= 175 && nBl >=2 && nB >= 2  && nT >= 1",
//                       ,"nJ >= 5 && mtB >= 175 && nBl >=2 && nB >= 2  && nT >= 1"
                       ""};



//    TString dPhi[] = {"dPhi  < .15 && dPhi3 >= .5","dPhi >= 1 && dPhi3 >= .5",""};
    TString dPhi[] = {"dPhi < .15 ","dPhi > 1","dPhi > 1 && dPhi3 >= .5",""};
    TString var = "met";
//    TString weight = "weight*puWeight*smearWeight*2.5*(weight >= .3 && weight <= 1)";
    TString weight = "weight*puWeight*smearWeight*2.5*(weight >= .02 && weight < .05)";

//    double bins[] = {0,10,20,30,40,50,60,70,80,90,100,120,200,300,400,500,600,800};
//    double nBinsX = 17;

//    double bins[] = {150,175,200,225,250,275,300,325,350,400,500,600,700,800};
//    double nBinsX = 13;

//    double bins[] = {150,200,250,300,350,400,450,500,600,700,800};
//    double nBinsX = 10;

//    double bins[] = {250,300,400,500,600,800};
//    double nBinsX = 5;

    double bins[] = {200,225,250,275,300,325,350,375,400,425,450,500,550,600,700,800,900,1000};
    double nBinsX = 17;




    for(unsigned int iP = 0; preSel[iP][0]; ++iP){
      for(unsigned int iD = 0; dPhi[iD][0]; ++iD){
        TString title = TString::Format("%s, %s",preSel[iP].Data(),dPhi[iD].Data());
        TString name = TString::Format("testSmear_%u_%u",iP,iD);
        TString selection = TString::Format("%s && %s",preSel[iP].Data(),dPhi[iD].Data());
        Plot * plot = new Plot(name,title,"","Events");

        TH1F * hO  = getHistogram(oTree,var,name,"orig",nBinsX,bins,selection,0,&weight);
        TH1F * hS  = getHistogram(sTree,var,name,"smear",nBinsX,bins,selection,0,&weight);

        for(unsigned int iB = 0; iB <= hS->GetNbinsX(); ++iB){
          hS->SetBinError(iB,0.00001);
        }

        plot->addHist(hO,"orig","",1,0,1);
        plot->addHist(hS,"smear","",2,0,2);

        TCanvas * c = new TCanvas;
        plot->setLogy();
                plot->setXTitle("#slash{E}_{T} [GeV]");
        plot->drawRatios(c,0,true,"png");

      }

    }

//    for(unsigned int iP = 0; preSel[iP][0]; ++iP){
//        TString title = TString::Format("%s",preSel[iP].Data());
//        TString name = TString::Format("testSmear_%u",iP);
//        TString selection = TString::Format("%s && %s",preSel[iP].Data(), dPhi[0].Data());
//        Plot * plot = new Plot(name,title,"","Events");
//
//        TH1F * hO  = getHistogram(oTree,var,name,"orig",nBinsX,bins,selection,0,&weight);
//        TH1F * hS  = getHistogram(sTree,var,name,"smear",nBinsX,bins,selection,0,&weight);
//
//                for(unsigned int iB = 0; iB <= hS->GetNbinsX(); ++iB){
//                  hS->SetBinError(iB,0.00001);
//                }
//
//        selection = TString::Format("%s && %s",preSel2[iP].Data(), dPhi[1].Data());
//        TH1F * hOU  = getHistogram(oTree,var,name,"orig",nBinsX,bins,selection,0,&weight,1);
//        TH1F * hSU  = getHistogram(sTree,var,name,"smear",nBinsX,bins,selection,0,&weight,1);
//
//                for(unsigned int iB = 0; iB <= hS->GetNbinsX(); ++iB){
//                  hSU->SetBinError(iB,0.00001);
//                }
//
//        hSU->Divide(hS);
//        hOU->Divide(hO);
//
//
//        plot->addHist(hOU,"orig","",1,0,1);
//        plot->addHist(hSU,"smear","",2,0,2);
//        plot->setXTitle("#slash{E}_{T} [GeV]");
//        plot->setYTitle("SR/CR");
//        TCanvas * c = new TCanvas;
////        plot->setLogy();
//        plot->drawRatios(c,0,true,"png");
//
//
//      }


}



void studyMETSources(TTree * oTree){


  TString presel = "nJ >= 5  && nBl >=2 && leadLoss_pt > 200 && (weight < .02)";
  TString weight = "puWeight*weight";
  TString sel[] = {
//                          "nB == 0  && nT == 0",
//                          "nB == 1  && nT == 0",
                          "nB >= 2  && nT == 0",
//                          "nB >= 1  && nT >= 1",
//                          "nB >= 2  && nT >= 1",
                     ""};
  TString angleSel[] = {
      "dPhi < .15 && dPhi3 >= .5",
//      "dPhi >   1 && dPhi3 >= .5",
      ""
  };

  TString metSel[] = {
      "met >= 200",
      "met >= 300",
      "met >= 400",
      ""
  };

  vector<PlotInfo*> vars;
//  vars.push_back(new PlotInfo("leadLoss_pt" ,"leadLoss_pt"              ,"leadLoss_pt",10,0,1000));
  vars.push_back(new PlotInfo("leadLoss_pt" ,"leadLoss_pt"              ,"leadLoss_pt",10,0,3000));
  vars.push_back(new PlotInfo("leadLoss_eta" ,"TMath::Abs(leadLoss_eta)"            ,"leadLoss_eta",10,0,4));
  vars.push_back(new PlotInfo("leadLoss_flavor" ,"leadLoss_flavor"      ,"leadLoss_flavor",10,-.5,9.5));
  vars.push_back(new PlotInfo("leadLoss_res" ,"leadLoss_res"            ,"leadLoss_res",10,0,2));
  vars.push_back(new PlotInfo("leadLoss_rank" ,"leadLoss_rank"          ,"leadLoss_rank",5,-.5,4.5));
  vars.push_back(new PlotInfo("leadLoss_recorank" ,"leadLoss_recorank"  ,"leadLoss_recorank",5,-.5,4.5));

  for(unsigned int iV = 0; iV < vars.size(); ++iV){
    TObjArray * o = new TObjArray;
    for(unsigned int iA = 0; angleSel[iA][0]; ++iA){
      for(unsigned int iS = 0; sel[iS][0]; ++iS){
        TString name = TString::Format("%s_%u_%u",vars[iV]->name.Data(),iA,iS );
        TString title = TString::Format("%s, %s",angleSel[iA].Data(),sel[iS].Data());

        Plot * plot = new Plot(name,title,vars[iV]->xTitle.Data(),"Events");

        for(unsigned int iM = 0; metSel[iM][0]; ++iM){
          TString cut = TString::Format("%s && %s && %s && %s",presel.Data(), angleSel[iA].Data(), sel[iS].Data(), metSel[iM].Data());
          TH1F * h  = getHistogram(oTree,*vars[iV],TString::Format("%u",iM),cut,0,&weight);
          PlotTools::normalize(h);
          plot->addHist(h,metSel[iM],"",colorGetter(iM),0,colorGetter(iM));
        }
        TCanvas * c = new TCanvas();
        plot->setYRange(0.,1.);
        plot->draw(c,false,"png");
        o->Add(c);
      }

    }
    drawAll(o,vars[iV]->name);
  }

}


void compSimp(TTree * oTree,TTree * sTree){


  TString preSel[] = {
                      "met >= 175 && nJ >= 5 ",
                     ""};

  TString dPhi[] = {"dPhi > .4 && dPhi3 >=.4",""};
  TString weight = "weight*puWeight*smearWeight*(weight  > 15 && weight < 20)";


//    double bins[] = {250,300,400,500,600,800};
//    double nBinsX = 5;

        double bins[] = {175,200,225,250,275,300,325,350,400,500,600,700,800};
        double nBinsX = 12;

    HistogramGetter histO("met","met","#slash{E}_{T} [GeV]", nBinsX,bins);
    HistogramGetter histS("met","met","#slash{E}_{T} [GeV]", nBinsX,bins);
    histS.nBootStraps = 50;
    histS.sampleName = "smear";



  for(unsigned int iP = 0; preSel[iP][0]; ++iP){
    for(unsigned int iD = 0; dPhi[iD][0]; ++iD){
      TCanvas * c = new TCanvas;

      TString title = TString::Format("%s, %s",preSel[iP].Data(),dPhi[iD].Data());
      TString name = TString::Format("testSmear_%u_%u",iP,iD);
      TString selection = TString::Format("%s && %s",preSel[iP].Data(),dPhi[iD].Data());
      Plot * plot = new Plot(name,title,"","Events");

      TH1F * hO = histO.getHistogram(oTree,selection,weight,"orig");
      TH1F * hS = histS.getHistogramManual(sTree,selection,weight,"smear");
//      TH1F * hS = histS.getHistogram(sTree,selection,weight,"smear");

      plot->addHist(hO,"orig","",1,0,1);
      plot->addHist(hS,"smear","",2,0,2);

      plot->setLogy();
              plot->setXTitle("#slash{E}_{T} [GeV]");
      plot->drawRatios(c,0,true,"png");


    }

  }



}

  void compMulti(TTree * oTree){
    cout <<"go!"<<endl;

//    TString smearFiles[] = {"jetResTest_smear_0p1.root","jetResTest_smear_0p3.root","jetResTest_smear_all.root",""};
//    TString smearNames[] = {"W_{smear} = 0.1","W_{smear} = 0.3","W_{smear} = All",""};

//    TString smearFiles[] = {"jetResTest_smear_win0p5.root","jetResTest_smear_win0p5Flat.root","jetResTest_smear_all.root",""};
//    TString smearNames[] = {"W_{smear} = scaled","W_{smear} = scaled flat","W_{smear} = All",""};

//    TString smearFiles[] = {"jetResTest_smear_win0p5Flat_skimmed.root","jetResTest_smear_all_skimmed.root",""};
//    TString smearNames[] = {"W_{smear} = 0.01-0.5","Classic smearing",""};

        TString smearFiles[] = {"jetResTest_smear_win0p5Flat.root","jetResTest_smear_0p3.root","jetResTest_smear_all.root",""};
        TString smearNames[] = {"W_{smear} = 0.01-0.5","W_{smear} = 0.3","Classic smearing",""};

//    TString smearFiles[] = {"jetResTest_smear_0p3.root","jetResTest_smear_old_0p3.root",""};
//    TString smearNames[] = {"W_{smear} = 0.01-0.5","W_{smear} = old 0.01-0.5",""};




    vector<TTree*> smearTrees;
    for(unsigned int iT = 0; smearFiles[iT][0]; ++iT){
      TFile * sf = new TFile(smearFiles[iT],"read");
      TTree * st =0;
      sf->GetObject("Events",st);
      smearTrees.push_back(st);
    }

    TString preForm = "nJ";
    TString presel = "met >= 175";
//    TString sel1[] = {"nJ >= 2","nJ >= 5",""};
//    TString sel2[] = {"nB == 0","nB == 1","nB == 1 && nBl >=2","nB >= 2",""};
    TString sel1[] = {"nJ >= 5",""};
    TString sel2[] = {"nB >= 0",""};

//    TString preForm = "mtB";
//    TString presel = "met >= 175 && nJ >= 5";
//    TString sel1[] = {"mtB >= 175",""};
//    TString sel2[] = {"nB == 0","nB == 1","nB == 1 && nBl >=2","nB >= 2",""};

//    TString preForm = "nT";
//    TString presel = "met >= 175 && nJ >= 5";
//    TString sel1[] = {"nT >= 1",""};
//    TString sel2[] = {"nB == 0","nB == 1","nB == 1 && nBl >=2","nB >= 2",""};

//    TString preForm = "nComp_dPhi12_gt1_dPhi";
//    TString presel = "met >= 200 && nJ >= 5 && mtB >= 175 && dPhi > 1";
//    TString sel1[] = {"nT == 0","nT >= 1",""};
//    TString sel2[] = {"nB == 1 && nBl >= 2","nB >= 2",""};

//    TString preForm = "nComp_dPhi12_gt1_dPhi3_gtp5";
//    TString presel = "met >= 200 && nJ >= 5 && mtB >= 175 && dPhi > 1&& dPhi3 > .5";
//    TString presel = "met >= 200 && nJ >= 5";
//    TString sel1[] = {"nT == 0","nT >= 1",""};
//    TString sel2[] = {"nB == 1 && nBl >= 2","nB >= 2",""};


//    TString preForm = "nSR_CR";
//    TString presel = "met >= 175 && nJ >= 5 && nBl >=2 && mtB > 175 && dPhi3 > .5 && dPhi < .15";
//    TString sel1[] = {"nB == 1 && nBl >=2","nB >= 2",""};
//    TString sel2[] = {"nT == 0","nT >= 1",""};

//    TString preForm = "nSR_SR";
//    TString presel = "met >= 175 && nJ >= 5 && nBl >=2 && mtB > 175 && dPhi3 > .5 && dPhi > 1";
//    TString sel1[] = {"nB == 1 && nBl >=2","nB >= 2",""};
//    TString sel2[] = {"nT == 0","nT >= 1",""};

//    TString preForm = "dPhiComp_nB";
//    TString presel = "met >= 200 && nJ >= 5 && nBl >=2 && mtB > 175";
//    TString sel1[] = {"nB == 1 && nBl >=2","nB >= 2",""};
//    TString sel2[] = {"dPhi3 < .15 && dPhi > 1","dPhi3 > .5 && dPhi > 1",""};

//
    double bins[] = {175,200,225,250,275,300,325,350,400,500,600,700,800};
    double nBinsX = 12;


//    double bins[] = {200,250,300,400,500,600,800};
//    double nBinsX = 6;
    HistogramGetter histG("met","met","#slash{E}_{T} [GeV]", nBinsX,bins);


//    TString preForm = "nSRDPhi";
//    TString presel = "met >= 175 && nJ >= 5 && nBl >=2 && mtB > 175";
//    TString sel1[] = {"nB == 1 && nBl >=2 && nT == 0","nB >= 2 && nT == 0","nB == 1 && nBl >=2 && nT >=1","nB >= 2 && nT >=1",""};
//    TString sel2[] = {"met >= 200","met >= 400",""};
//
//    double bins[] = {0,.05,.1,.15,.2,.25,.3,.4,.5,.75,1,1.25,1.5};
//    double nBinsX = 12;
//    double bins[] = {0,.05,.1,.15,.2,.25,.3,.4,.6,.8,1};
//    double nBinsX = 10;
//    HistogramGetter histG("dPhi","dPhi","Min(#Delta#phi(#slash{E}_{T},J_{1},J_{2}))", nBinsX,bins);


//        double bins[] = {0,.1,.2,.3,.4,.5,1};
//        double nBinsX = 6;
//        HistogramGetter histG("dPhi3","dPhi3","#Delta#phi(#slash{E}_{T},J_{3})", nBinsX,bins);







    TString weight = "weight*smearWeight";



//          double bins[] = {175,180,185,190,195,200,215,225,250,275,300,325,350,400,500,600,700,800};
//          double nBinsX = 17;


//      TObjArray * o = new TObjArray;


      for(unsigned int iS1 = 0; sel1[iS1][0]; ++iS1){
      for(unsigned int iS2 = 0; sel2[iS2][0]; ++iS2){
        TString title = TString::Format("%s, %s",sel1[iS1].Data(),sel2[iS2].Data());
        TString name = TString::Format("testSmear_%s_%u_%u",preForm.Data(),iS1,iS2);
        TString selection = TString::Format("%s && %s && %s",presel.Data(),sel1[iS1].Data(),sel2[iS2].Data());

        Plot * plot = new Plot(name,title,"","Events");
        histG.nBootStraps = 0;
        TH1F * hO = histG.getHistogram(oTree,selection,weight,"Original");

        plot->addHist(hO,"Original","",1,0,1);
        for(unsigned int iT = 0; iT < smearTrees.size(); ++iT){
//          histG.nBootStraps = 50;
//          TH1F * hS = histG.getHistogramManual(smearTrees[iT],selection,weight,TString::Format("smear_%u",iT));
          TH1F * hS = histG.getHistogram(smearTrees[iT],selection,weight,TString::Format("smear_%u",iT));
          plot->addHist(hS,smearNames[iT],"",colorGetter(iT+1),0,colorGetter(iT+1));
        }
        plot->setLogy();
        plot->setXTitle(histG.plotInfo->xTitle);
        TCanvas * c = new TCanvas;
        plot->drawRatios(c,0,true,"png");
//        o->Add(c);

        cout << title << endl;
      }
      }
//      drawAll(o,"");

}

  void compMultiTF(TTree * oTree){


//    TString smearFiles[] = {"jetResTest_smear_win0p5Flat_skimmed.root","jetResTest_smear_all_skimmed.root",""};
//    TString smearNames[] = {"Original","W_{smear} = scaled flat","W_{smear} = All",""};

    TString smearFiles[] = {"jetResTest_smear_win0p5Flat_skimmed.root","","jetResTest_smear_all_skimmed.root",""};
    TString smearNames[] = {"Original","W_{smear} = 0.01-0.5","Classic smearing",""};

    vector<TTree*> smearTrees;
    smearTrees.push_back(oTree);
    for(unsigned int iT = 0; smearFiles[iT][0]; ++iT){
      TFile * sf = new TFile(smearFiles[iT],"read");
      TTree * st =0;
      sf->GetObject("Events",st);
      smearTrees.push_back(st);
    }



//    TString preForm = "TF_nB";
//    TString presel = "met >= 200 && nJ >= 5 && nBl >=2 && nT >=1 && mtB >= 175";
//    TString denSel = "dPhi < .15 && dPhi3 > .5";
//    TString numSel = "dPhi > 1 && dPhi3 > .5";
//    TString sel1[] = {"nB == 1 && nBl >=2","nB >= 2",""};


    TString preForm = "TF_all";
    TString presel = "met >= 200 && nJ >= 5 && nBl >=2 && mtB >= 175 && nB >= 1 && nBl >=2";
    TString denPreSel = "dPhi < .15 && dPhi3 > .5";
    TString numPreSel = "dPhi > 1 && dPhi3 > .5";

    TString selNum[] = {"nB == 1 && nT == 0","nB >= 2 && nT == 0","nB == 1 && nT >= 1","nB >= 2 && nT >= 1",""};
    TString selDen[] = {"nT == 0","nT == 0","nT >= 1","nT >= 1",""};

    double bins[] = {200,250,300,400,500,600,800};
    double nBinsX = 6;
    HistogramGetter histG("met","met","#slash{E}_{T} [GeV]", nBinsX,bins);

//    double bins[] = {75,100,125,150,175,200,250,300,400,500,600,800};
//    double nBinsX = 11;
//    HistogramGetter histG("met","met","#slash{E}_{T} [GeV]", nBinsX,bins);


    TString weight = "weight*smearWeight";


      for(unsigned int iS1 = 0; selNum[iS1][0]; ++iS1){
        TString title = TString::Format("%s",selNum[iS1].Data());
        TString name = TString::Format("testSmear_%s_%u",preForm.Data(),iS1);
        TString denSelelction = TString::Format("%s && %s && %s",presel.Data(),selDen[iS1].Data(),denPreSel.Data());
        TString numSelelection = TString::Format("%s && %s && %s",presel.Data(),selNum[iS1].Data(),numPreSel.Data());

        Plot * plot = new Plot(name,title,"","SR/CR");

        for(unsigned int iT = 0; iT < smearTrees.size(); ++iT){
          if(iT != 0) histG.nBootStraps = 50;
          else histG.nBootStraps = 0;

          TH1F * hSN = histG.getHistogramManual(smearTrees[iT],numSelelection,weight,denSelelction,weight,TString::Format("ratio_%u",iT));

//          TH1F * hSN = histG.getHistogramManual(smearTrees[iT],numSelelection,weight,TString::Format("num_%u",iT));
//          TH1F * hSD = histG.getHistogramManual(smearTrees[iT],denSelelction,weight,TString::Format("den_%u",iT));
//          hSN->Divide(hSD);
          plot->addHist(hSN,smearNames[iT],"",colorGetter(iT),0,colorGetter(iT));
        }

//        plot->setLogy();
        plot->setXTitle(histG.plotInfo->xTitle);
        TCanvas * c = new TCanvas;
//        plot->drawRatios(c,0,true÷,"png");
        plot->draw(c,true,"png");
//        o->Add(c);

        cout << title << endl;
      }
//      drawAll(o,"");

}



  void compBreakdown(TTree * oTree){

    cout <<"go!"<<endl;

//    TString smearFiles[] = {"jetResTest_smear_0p1.root","jetResTest_smear_0p3.root","jetResTest_smear_all.root",""};
//    TString smearNames[] = {"W_{smear} = 0.1","W_{smear} = 0.3","W_{smear} = All",""};

//    TString smearFiles[] = {"qcd_temp.root","jetResTest_smear_win0p5.root","jetResTest_smear_0p3.root","jetResTest_smear_all.root",""};
//    TString smearNames[] = {"temp","W_{smear} = scaled","W_{smear} = 0.3","W_{smear} = All",""};

    TString smearFiles[] = {"jetResTest_smear_0p1.root","jetResTest_smear_0p3.root","jetResTest_smear_all.root",""};
    TString smearNames[] = {"W_{smear} = 0.1","W_{smear} = 0.3","Classic smearing",""};

//    TString smearFiles[] = {"jetResTest_smear_win0p5Flat.root","jetResTest_smear_0p3.root","jetResTest_smear_all.root",""};
//    TString smearNames[] = {"W_{smear} = 0.01-0.5","W_{smear} = 0.3","Classic smearing",""};

//    TString smearFiles[] = {"jetResTest_smear_win0p5Flat.root","jetResTest_smear_all.root",""};
//    TString smearNames[] = {"W_{smear} = 0.01-0.5","Classic smearing",""};

//        TString smearFiles[] = {"jetResTest_smear_win0p5Flat_skimmed.root","jetResTest_smear_all_skimmed.root",""};
//        TString smearNames[] = {"W_{smear} = 0.01-0.5","Classic smearing",""};


    vector<TTree*> smearTrees;
    for(unsigned int iT = 0; smearFiles[iT][0]; ++iT){
      TFile * sf = new TFile(smearFiles[iT],"read");
      TTree * st =0;
      sf->GetObject("Events",st);
      smearTrees.push_back(st);
    }

    TString preForm = "nJ";
    TString presel = "met >= 175";
    TString sel1[] = {"nJ >= 5",""};
//    TString preForm = "mtB";
//    TString presel = "met >= 175 && nJ >= 5";
//    TString sel1[] = {"mtB >= 175",""};
//    TString sel2[] = {"nB == 0","nB == 1","nB == 1 && nBl >=2","nB >= 2",""};


//    TString presel = "met >= 175 && nJ >= 5";

//    TString preForm = "nJ5_nB2";
//    TString sel1[] = {"nB >=2",""};

//    TString preForm = "mtB_nB1";
//    TString sel1[] = {"mtB> 175 && nB ==1",""};
//    TString preForm = "nT_nB2";
//    TString sel1[] = {"nT>= 1 && nB >= 2",""};


    double bins[] = {175,180,185,190,195,200,215,225,250,275,300,325,350,400,500,600,700,800};
    double nBinsX = 17;

    HistogramGetter histG("met","met","#slash{E}_{T} [GeV]", nBinsX,bins);

//    TString preForm = "nComp_dPhi";
//    TString presel = "met >= 175 && nJ >= 5 && mtB >= 175";
//    TString sel1[] = {"nB >= 2 && nT >= 1",""};


//    TString preForm = "nSRDPhi_met600_dPhi12_nB1_nT1";
//    TString presel = "met >= 175 && nJ >= 5 && nBl >=2 && mtB > 175 && dPhi > 1";
//    TString sel1[] = {"met >= 600 && nT >= 1 && nB == 1",""};

//    TString preForm = "nSRDPhi_met600_dPhi12_nB2_nT1";
//    TString presel = "met >= 175";
//    TString sel1[] = {"nJ >= 5",""};

//    TString preForm = "nSRDPhi_met600_dPhi12_nB2_nT0";
//    TString presel = "met >= 175 && nJ >= 5 && nBl >=2 && mtB > 175 && dPhi > 1";
//    TString sel1[] = {"met >= 600 && nT == 0 && nB >= 2",""};

//    TString preForm = "nSRDPhi_met600_dPhi12_nB1_nT0";
//    TString presel = "met >= 175 && nJ >= 5 && nBl >=2 && mtB > 175 && dPhi > 1";
//    TString sel1[] = {"met >= 600 && nT == 0 && nB ==1",""};

//    double bins[] = {0,.05,.1,.15,.2,.25,.3,.4,.5,.75,1,1.25,1.5};
//    double nBinsX = 12;
//    HistogramGetter histG("dPhi","dPhi","dPhi", nBinsX,bins);

//    double bins[] = {0,.05,.1,.15,.2,.25,.3,.4,1};
//    double nBinsX = 8;
//    HistogramGetter histG("dPhi3","dPhi3","dPhi3", nBinsX,bins);

//    double bins[] = {0,.05,.1,.15,.2,.25,.3,.4,.6,.8,1};
//    double nBinsX = 10;
//    HistogramGetter histG("dPhi","dPhi","Min(#Delta#phi(#slash{E}_{T},J_{1},J_{2}))", nBinsX,bins);


    TString selW[] = {
        "weight < .015",
        "weight > .03 && weight < .034",
        "weight > .2 && weight < .3",
        "weight > .4 && weight < .5",
        "weight > 1.5 && weight < 1.9",
        "weight > 17 && weight < 18",
        "weight > 90",
        ""};
    TString selWN[] = {
                       "Bin[H_{T}>2000]",
                        "Bin[1500-H_{T}-2000]",
                       "Bin[1000-H_{T}-1500]"
                        ,"Bin[700-H_{T}-1000]",
                       "Bin[500-H_{T}-700]",
                       "Bin[300-H_{T}-500]",
                       "Bin[200-H_{T}-300]",
                       ""};



    TString weight = "weight*smearWeight";



      TObjArray * o = new TObjArray;


      for(unsigned int iS1 = 0; sel1[iS1][0]; ++iS1){
      for(unsigned int iSW = 0; selW[iSW][0]; ++iSW){
        TString title = TString::Format("%s, %s",sel1[iS1].Data(),selWN[iSW].Data());
        TString name = TString::Format("testBreakdown_%s_%u_%u",preForm.Data(),iS1,iSW);
        TString selection = TString::Format("%s && %s && %s",presel.Data(),sel1[iS1].Data(),selW[iSW].Data());

        Plot * plot = new Plot(name,title,"","Events");
//        histG.nBootStraps = 0;
        TH1F * hO = histG.getHistogram(oTree,selection,weight,"Original");
        plot->addHist(hO,"Original","",1,0,1);
        for(unsigned int iT = 0; iT < smearTrees.size(); ++iT){
//          histG.nBootStraps = 50;
//          TH1F * hS = histG.getHistogramManual(smearTrees[iT],selection,weight,TString::Format("smear_%u",iT));
          TH1F * hS = histG.getHistogram(smearTrees[iT],selection,weight,TString::Format("smear_%u",iT));
          plot->addHist(hS,smearNames[iT],"",colorGetter(iT+1),0,colorGetter(iT+1));
        }
        plot->setLogy();
        plot->setXTitle("#slash{E}_{T} [GeV]");
        TCanvas * c = new TCanvas;
        plot->drawRatios(c,0,true,"png");
        o->Add(c);

        cout << title << endl;
      }
      }
      drawAll(o,"");

}

#endif


void TestTailSmear(const TString origFile="qcd_jetRes.root",const TString smearFile="qcd_jetRes.root", const TString treeName = "Events")
{

  StyleTools::SetStyle();
  TFile * of = new TFile(origFile,"read");
  TTree * ot =0;
  of->GetObject(treeName,ot);

  TFile * sf = new TFile(smearFile,"read");
  TTree * st =0;
  sf->GetObject(treeName,st);
//
//  comp(ot,st);
//  studyMETSources(ot);
//  compSimp(ot,st);
//  compMulti(ot);
  compMultiTF(ot);
//  compBreakdown(ot);

//  TFile * of = new TFile(outFile,"read");
//
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
