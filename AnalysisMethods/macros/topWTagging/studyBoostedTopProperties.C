#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/PlotUtils/interface/HistogramGetter.hh"
#include "TDirectory.h"
#include "TFile.h"

using namespace std;

TTree * getTree(TString filename){
  TFile * sf = new TFile(filename,"read");
  TTree * st =0;
  sf->GetObject("Events",st);
  return st;
}

TH2D * getHistogram(TTree * tree, TString name, TString sel, TString weight,  TString xAxis, int nXBins, double xMin, double xMax, TString yAxis, int nYBins, double yMin, double yMax){
  TString selection = TString::Format("%s*(%s)",weight.Data(),sel.Data());

  TH2D * h = new TH2D(name,name,nXBins,xMin,xMax,nYBins,yMin,yMax);
  tree->Draw(TString::Format("%s:%s>>+%s",yAxis.Data(),xAxis.Data(),name.Data()),selection,"goff");
  if(h) h = (TH2D*)h->Clone();
  return h;
}


void plotTopEfficiency() {

  TTree * tree = getTree("All_tree.root");

  TString signalBaseline = "(mass1 == 1000 && mass2 == 1 && top_pt > 0)";
  TString signalSels[] = {
//      "1.0",
//      "top_maxDR/2 < 0.8",
//      "top_maxDR/2 < 0.8 && top_fj_maxDR < 0.8 && fj_pt >= 200 &&  fj_eta < 2.4",
      "top_maxDR/2 < 0.8 && top_fj_maxDR < 0.8 && fj_pt >= 200 &&  fj_eta < 2.4",
      "top_maxDR/2 < 0.8 && top_fj_maxDR < 0.8 && fj_pt >= 200 &&  fj_eta < 2.4 && (fj_sdMass >= 110 && fj_sdMass < 210 && fj_tau3/fj_tau2 < 0.69)",
      "top_maxDR/2 < 0.8 && top_fj_maxDR < 0.8 && fj_pt >= 200 &&  fj_eta < 2.4 && (fj_sdMass >= 105 && fj_sdMass < 220   && fj_tau3/fj_tau2 < 0.81 && (fj_sj1_csv > 0.46 ||fj_sj2_csv > 0.46 ))",
      "top_maxDR/2 < 0.8 && top_fj_maxDR < 0.8 && fj_pt >= 200 &&  fj_eta < 2.4 && (fj_puppi_sdMass >= 105 && fj_puppi_sdMass < 210 && fj_puppi_tau3/fj_puppi_tau2 < 0.80 && (fj_puppi_sj1_csv > 0.46 ||fj_puppi_sj2_csv > 0.46 )  )",
      ""};
//  TString signalSels[] = {"1.0","top_maxDR/2 < 0.8","top_maxDR/2 < 0.8 && fj_pt >= 200 &&  fj_eta < 2.4",""};
  TString signalSelNames[] = {
//  "inclusive",
//  "#DeltaR_{q} < 0.8",
//  "quarks w/in one fj w/ p_{T} > 200 GeV", "",

      "Matched FJ",
  "ICHEP tagger",
  "loose CMS tagger",
  "loose CMS puppi tagger",
  "+test"

  };


  TString bkgTypes[] = {"(mass1 == 1000 && mass2 == 1 && fj_mindr_p2 >= 0.8)","(process == 2 && fj_mindr_p2 >= 0.8)","(process == 4)",""};
  TString bkgNames[] = {"Fake signal","Fake t#bar{t}","Fake Z#rightarrow#nu#nu"};

  TString bkgBaseline = "(fj_pt >= 200 &&  fj_eta < 2.4)";
  TString bkgSels[] = {"1.0",
      "(fj_sdMass >= 110 && fj_sdMass < 210 && fj_tau3/fj_tau2 < 0.69)",
      "(fj_sdMass >= 105 && fj_sdMass < 220   && fj_tau3/fj_tau2 < 0.81 && (fj_sj1_csv > 0.46 ||fj_sj2_csv > 0.46 ))",
      "(fj_puppi_sdMass >= 105 && fj_puppi_sdMass < 210 && fj_puppi_tau3/fj_puppi_tau2 < 0.80 && (fj_puppi_sj1_csv > 0.46 ||fj_puppi_sj2_csv > 0.46 )  )",
      ""};
  TString bkgSelNames[] = {"inclusive","ICHEP tagger","loose CMS tagger","loose CMS puppi tagger"};


//  //Relative
//  TString signalBaseline = "(mass1 == 1000 && mass2 == 1 && top_pt > 0 && top_maxDR/2 < 0.8 && top_fj_maxDR < 0.9 && fj_pt >= 400 &&  fj_eta < 2.4 )";
//  TString signalSels[] = {"1.0",
//      "(fj_sdMass >= 110 && fj_sdMass < 210 && fj_tau3/fj_tau2 < 0.69)",
////      "(fj_mass >= 125)",
////      "(fj_mass > 125 && (fj_sj1_csv > 0.46 ||fj_sj2_csv > 0.46 ) )",
////      "(fj_mass > 125 && (fj_sj1_csv > 0.46 ||fj_sj2_csv > 0.46 ) && fj_mass < 300 && fj_tau3/fj_tau2 < 0.75)",
////      "(fj_mass > 125 && (fj_sj1_csv > 0.46 ||fj_sj2_csv > 0.46 ) && fj_mass < 300 && fj_tau3/fj_tau2 < 0.69 )",
////      "(fj_mass > 125 && (fj_sj1_csv > 0.46 ||fj_sj2_csv > 0.46 ) && fj_mass < 300 && fj_tau3/fj_tau2 < min(0.8,.000167*fj_pt) + .633 )",
//      "(fj_sdMass >= 110 && fj_sdMass < 210  && fj_tau3/fj_tau2 < min(0.8,.000167*fj_pt) + .633 )",
//      "(fj_mass > 150 && (fj_sj1_csv > 0.46 ||fj_sj2_csv > 0.46 ) && fj_mass < 300 && fj_tau3/fj_tau2 < min(0.8,.000167*fj_pt) + .633 )",
////      "(fj_tau3/fj_tau2 < 0.69)",
////      "(fj_sdMass >= 110 && fj_sdMass < 210 && fj_tau3/fj_tau2 < 0.69)",
////      "(fj_sdMass >= 50 && fj_sdMass < 210 && fj_tau3/fj_tau2 < 0.69)",
////      "(fj_sdMass >= 110 && fj_sdMass < 300 && fj_tau3/fj_tau2 < 0.69)",
////      "(fj_sdMass >= 110 && fj_sdMass < 210 && fj_tau3/fj_tau2 < 0.80)",
//      ""};
//  TString signalSelNames[] = {"Gen+FJ",
//            "+current tagger",
//            "loosen low m",
//            "loosen low m high m",
//            "loosen low m high m csv" ,
//            "loosen low m high m tau"
//  };
//
//  TString bkgTypes[] = {//"(mass1 == 1000 && mass2 == 1 && fj_mindr_p2 >= 0.8)","(process == 2 && fj_mindr_p2 >= 0.8)",
//      "(process == 4)",""};
//  TString bkgNames[] = {"Fake signal","Fake t#bar{t}","Fake Z#rightarrow#nu#nu"};
//
//  TString bkgBaseline = "(fj_pt >= 400 &&  fj_eta < 2.4)";
//  TString bkgSels[] = {//"1.0",
//      "(fj_sdMass >= 110 && fj_sdMass < 210 && fj_tau3/fj_tau2 < 0.69)",
////      "(fj_mass > 125 && (fj_sj1_csv > 0.46 ||fj_sj2_csv > 0.46 ) )",
////      "(fj_mass > 125 && (fj_sj1_csv > 0.46 ||fj_sj2_csv > 0.46 ) && fj_mass < 300 && fj_tau3/fj_tau2 < 0.75)",
////      "(fj_mass > 125 && (fj_sj1_csv > 0.46 ||fj_sj2_csv > 0.46 ) && fj_mass < 300 && fj_tau3/fj_tau2 < 0.69 )",
////      "(fj_mass > 125 && (fj_sj1_csv > 0.46 ||fj_sj2_csv > 0.46 ) && fj_mass < 300 && fj_tau3/fj_tau2 < min(0.8,.000167*fj_pt) + .633 )",
//      "(fj_sdMass >= 110 && fj_sdMass < 210   && fj_tau3/fj_tau2 < min(0.8,.000167*fj_pt) + .633 )",
//      "(fj_mass > 150 && (fj_sj1_csv > 0.46 ||fj_sj2_csv > 0.46 ) && fj_mass < 300 && fj_tau3/fj_tau2 < min(0.8,.000167*fj_pt) + .633 )",
////      "(fj_tau3/fj_tau2 < 0.69)",
////      "(fj_sdMass >= 110 && fj_sdMass < 210 && fj_tau3/fj_tau2 < 0.69)",
////      "(fj_sdMass >= 50 && fj_sdMass < 210 && fj_tau3/fj_tau2 < 0.69)",
////      "(fj_sdMass >= 110 && fj_sdMass < 300 && fj_tau3/fj_tau2 < 0.69)",
////      "(fj_sdMass >= 110 && fj_sdMass < 210 && fj_tau3/fj_tau2 < 0.80)",
//      ""};
//
//  TString bkgSelNames[] = {//"inclusive",
//      "+current tagger",
//      "loosen low m",
//      "loosen low m high m",
//      "loosen low m high m csv" ,
//      "loosen low m high m tau"
//  };


  HistogramGetter * trueTopPTGeter = new HistogramGetter("top_pt","top_pt","top quark #it{p}_{T}",40,0,1000); trueTopPTGeter->setUnderOverflow(true,true);
  HistogramGetter * fakeTopPTGeter = new HistogramGetter("fj_pt","fj_pt","fat jet #it{p}_{T}",40,0,1000);     fakeTopPTGeter->setUnderOverflow(true,true);

  //signal
  Plot * true_plot = new Plot("topPT","",trueTopPTGeter->plotInfo->xTitle,"a.u");
  for(unsigned int iS = 0; signalSels[iS][0]; ++iS ){
    TString sel = TString::Format("%s && %s",signalBaseline.Data(),signalSels[iS].Data());
    TH1F * h = trueTopPTGeter->getHistogram(tree,sel,"weight","top");
    true_plot->addHist(h,signalSelNames[iS],"hist",StyleTools::colorGetter(iS),0,StyleTools::colorGetter(iS));
    cout << "bkg " << iS <<" " << h->Integral(0,-1)<<endl;
  }
  true_plot->setRatioRange(0.0,1.0);
  TCanvas * c = new TCanvas;
  true_plot->drawRatios(c,0,false,"pdf");


  //bkg

  for(unsigned int iB = 0; bkgTypes[iB][0]; ++iB){
    Plot * fake_plot = new Plot(TString::Format("fake_%u",iB),bkgNames[iB],fakeTopPTGeter->plotInfo->xTitle,"a.u");
    for(unsigned int iS = 0; bkgSels[iS][0]; ++iS ){
      TString sel = TString::Format("%s && %s && %s",bkgBaseline.Data(),bkgTypes[iB].Data(), bkgSels[iS].Data());
      TH1F * h = fakeTopPTGeter->getHistogram(tree,sel,"weight","top");
      fake_plot->addHist(h,bkgSelNames[iS],"hist",StyleTools::colorGetter(iS),0,StyleTools::colorGetter(iS));
      cout << "sig " << iS <<" " << h->Integral(0,-1)<<endl;
    }
    fake_plot->setRatioRange(0.0,1.0);
    TCanvas * c = new TCanvas;
    fake_plot->drawRatios(c,0,false,"pdf");
  }



}


void studyMass() {

  TTree * tree = getTree("All_tree.root");

  TString signalBaseline = "(mass1 == 1000 && mass2 == 1 && top_pt > 0 && top_maxDR/2 < 0.8 && top_fj_maxDR < 0.8)";
  TString bkgBaseline    = "(process == 4)";


//  TString sels[] = { "(fj_pt >= 400 &&  fj_pt <= 600)","(fj_pt >= 600 &&  fj_pt <= 900)","(fj_pt >= 900)",""};
//  TString selNames[] = { "400 < fj p_{T} < 600","600 < fj p_{T} < 900","900 < fj p_{T}"};

  TString sels[] = { "(fj_pt >= 400 &&  fj_pt <= 600)","(fj_pt >= 900)",""};
  TString selNames[] = { "400 < fj p_{T} < 600","900 < fj p_{T}"};

  //Mass
//  TString selBaseline = "(fj_pt >= 200 &&  fj_eta < 2.4 && fj_tau3/fj_tau2 < 0.69)";
//  vector<HistogramGetter*> histGs;
//  histGs.push_back(new HistogramGetter("fj_mass","fj_mass","mass",100,0,500));                                 histGs.back()->setUnderOverflow(true,true);
//  histGs.push_back(new HistogramGetter("fj_prunedMass","fj_prunedMass","pruned mass",100,0,500));              histGs.back()->setUnderOverflow(true,true);
//  histGs.push_back(new HistogramGetter("fj_sdMass","fj_sdMass","soft drop mass",100,0,500));                   histGs.back()->setUnderOverflow(true,true);
//  histGs.push_back(new HistogramGetter("fj_puppi_mass","fj_puppi_mass","puppi mass",100,0,500));                 histGs.back()->setUnderOverflow(true,true);
//  histGs.push_back(new HistogramGetter("fj_puppi_sdMass","fj_puppi_sdMass","puppi soft drop mass",100,0,500)); histGs.back()->setUnderOverflow(true,true);


//  //Btagging
//  TString selBaseline = "(fj_pt >= 200 &&  fj_eta < 2.4 && fj_sdMass >= 110 && fj_sdMass < 210 && fj_tau3/fj_tau2 < 0.69)";
//  vector<HistogramGetter*> histGs;
//  histGs.push_back(new HistogramGetter("fj_csv","fj_csv","csv",50,0,1));                                 histGs.back()->setUnderOverflow(true,true);
//  histGs.push_back(new HistogramGetter("min_fj_sj1_csv_fj_sj2_csv","min(fj_sj1_csv,fj_sj2_csv)","min sj csv",50,0,1));              histGs.back()->setUnderOverflow(true,true);
//  histGs.push_back(new HistogramGetter("max_fj_sj1_csv_fj_sj2_csv_","max(fj_sj1_csv,fj_sj2_csv)","max sj csv",50,0,1));                    histGs.back()->setUnderOverflow(true,true);
////  histGs.push_back(new HistogramGetter("min_fj_puppi_sj1_csv_fj_puppi_sj2_csv_","min(fj_puppi_sj1_csv,fj_puppi_sj2_csv)","min puppi sj csv",50,0,1));              histGs.back()->setUnderOverflow(true,true);
////  histGs.push_back(new HistogramGetter("max_fj_puppi_sj1_csv_fj_puppi_sj2_csv_","max(fj_puppi_sj1_csv,fj_puppi_sj2_csv)","max puppi sj csv",50,0,1));                    histGs.back()->setUnderOverflow(true,true);
//

//    //subjet mass
//    TString selBaseline = "(fj_pt >= 200 &&  fj_eta < 2.4 && fj_sdMass >= 110 && fj_sdMass < 210 && fj_tau3/fj_tau2 < 0.69)";
//    vector<HistogramGetter*> histGs;
//    histGs.push_back(new HistogramGetter("2","min(fj_sj1_mass,fj_sj2_mass)"            ,"min sj mass"      ,100,0,500)); histGs.back()->setUnderOverflow(true,true);
//    histGs.push_back(new HistogramGetter("3","max(fj_sj1_mass,fj_sj2_mass)"            ,"max sj mass"      ,100,0,500)); histGs.back()->setUnderOverflow(true,true);
////    histGs.push_back(new HistogramGetter("4","min(fj_puppi_sj1_mass,fj_puppi_sj2_mass)","min puppi sj mass",100,0,500)); histGs.back()->setUnderOverflow(true,true);
////    histGs.push_back(new HistogramGetter("5","max(fj_puppi_sj1_mass,fj_puppi_sj2_mass)","max puppi sj mass",100,0,500)); histGs.back()->setUnderOverflow(true,true);


//    //subjet mass x csv
//    TString selBaseline = "(fj_pt >= 200 &&  fj_eta < 2.4 && fj_sdMass >= 110 && fj_sdMass < 210 && fj_tau3/fj_tau2 < 0.69)";
//    vector<HistogramGetter*> histGs;
//        histGs.push_back(new HistogramGetter("2","min(fj_sj1_mass,fj_sj2_mass)"            ,"min sj mass"      ,100,0,500)); histGs.back()->setUnderOverflow(true,true);
//        histGs.push_back(new HistogramGetter("3","max(fj_sj1_mass,fj_sj2_mass)"            ,"max sj mass"      ,100,0,500)); histGs.back()->setUnderOverflow(true,true);
//    histGs.push_back(new HistogramGetter("4","((fj_sj1_csv > fj_sj2_csv)*fj_sj1_mass + !(fj_sj1_csv > fj_sj2_csv)*fj_sj2_mass)"            ,"max csv sj mass"      ,100,0,500)); histGs.back()->setUnderOverflow(true,true);
//    histGs.push_back(new HistogramGetter("5","(!(fj_sj1_csv > fj_sj2_csv)*fj_sj1_mass + (fj_sj1_csv > fj_sj2_csv)*fj_sj2_mass)"            ,"min csv sj mass"      ,100,0,500)); histGs.back()->setUnderOverflow(true,true);
////    histGs.push_back(new HistogramGetter("4","((fj_puppi_sj1_csv > fj_puppi_sj2_csv)*fj_puppi_sj1_mass + !(fj_puppi_sj1_csv > fj_puppi_sj2_csv)*fj_puppi_sj2_mass)"            ,"puppi max csv sj mass"      ,100,0,500)); histGs.back()->setUnderOverflow(true,true);
////    histGs.push_back(new HistogramGetter("5","(!(fj_puppi_sj1_csv > fj_puppi_sj2_csv)*fj_puppi_sj1_mass + (fj_puppi_sj1_csv > fj_puppi_sj2_csv)*fj_puppi_sj2_mass)"            ,"puppi min csv sj mass"      ,100,0,500)); histGs.back()->setUnderOverflow(true,true);


    //pt frac
      TString selBaseline = "(fj_pt >= 200 &&  fj_eta < 2.4 && fj_sdMass >= 210&& fj_tau3/fj_tau2 < 0.69)";
      vector<HistogramGetter*> histGs;
      histGs.push_back(new HistogramGetter("1","((fj_sj1_pt > fj_sj2_pt)*fj_sj1_pt/fj_pt + !(fj_sj1_pt > fj_sj2_pt)*fj_sj2_pt/fj_pt)"  ,"max sj pt frac"      ,100,0,2)); histGs.back()->setUnderOverflow(true,true);
      histGs.push_back(new HistogramGetter("2","(!(fj_sj1_pt > fj_sj2_pt)*fj_sj1_pt/fj_pt + (fj_sj1_pt > fj_sj2_pt)*fj_sj2_pt/fj_pt)"  ,"min sj pt frac"      ,100,0,2)); histGs.back()->setUnderOverflow(true,true);
      histGs.push_back(new HistogramGetter("3","((fj_sj1_csv > fj_sj2_csv)*fj_sj1_pt/fj_pt + !(fj_sj1_csv > fj_sj2_csv)*fj_sj2_pt/fj_pt)"  ,"max csv sj pt frac"      ,100,0,2)); histGs.back()->setUnderOverflow(true,true);
      histGs.push_back(new HistogramGetter("4","(!(fj_sj1_csv > fj_sj2_csv)*fj_sj1_pt/fj_pt + (fj_sj1_csv > fj_sj2_csv)*fj_sj2_pt/fj_pt)"  ,"min csv sj pt frac"      ,100,0,2)); histGs.back()->setUnderOverflow(true,true);
//      histGs.push_back(new HistogramGetter("4","((fj_puppi_sj1_pt > fj_puppi_sj2_pt)*fj_puppi_sj1_pt/fj_puppi_pt + !(fj_puppi_sj1_pt > fj_puppi_sj2_pt)*fj_puppi_sj2_pt/fj_puppi_pt)","puppi max csv sj pt frac",100,0,2)); histGs.back()->setUnderOverflow(true,true);
//      histGs.push_back(new HistogramGetter("5","(!(fj_puppi_sj1_pt > fj_puppi_sj2_pt)*fj_puppi_sj1_pt/fj_puppi_pt + (fj_puppi_sj1_pt > fj_puppi_sj2_pt)*fj_puppi_sj2_pt/fj_puppi_pt)","puppi min csv sj pt frac",100,0,2)); histGs.back()->setUnderOverflow(true,true);
//
//      //DR
//      TString selBaseline = "(fj_pt >= 200 &&  fj_eta < 2.4 && fj_sdMass >= 110 && fj_sdMass < 210 && fj_tau3/fj_tau2 < 0.69)";
//      vector<HistogramGetter*> histGs;
//      histGs.push_back(new HistogramGetter("2","fj_drsj"                  ,"sj #DeltaR"            ,100,0,2)); histGs.back()->setUnderOverflow(true,true);
////      histGs.push_back(new HistogramGetter("5","fj_puppi_drsj"            ,"puppi sj #DeltaR"      ,100,0,1)); histGs.back()->setUnderOverflow(true,true);
//
//      histGs.push_back(new HistogramGetter("4","fj_drsj*fj_pt/(2*175)"            ,"sj #DeltaR*p_{T}/(2*175)"      ,100,0,2)); histGs.back()->setUnderOverflow(true,true);
////      histGs.push_back(new HistogramGetter("7","fj_puppi_drsj*fj_puppi_pt/(2*175)","puppi sj #DeltaR*p_{T}",100,0,2)); histGs.back()->setUnderOverflow(true,true);

  //nsubjettiness
//    TString selBaseline = "(fj_pt >= 200 &&  fj_eta < 2.4 && fj_sdMass >= 110 && fj_sdMass < 210 && max(fj_sj1_csv,fj_sj2_csv) > 0.8)";
//    vector<HistogramGetter*> histGs;
//    histGs.push_back(new HistogramGetter("2","fj_tau3/fj_tau2"            ,"#tau_{3}/#tau_{2}"      ,100,0,1)); histGs.back()->setUnderOverflow(true,true);
//    histGs.push_back(new HistogramGetter("3","fj_tau3/fj_tau1"            ,"#tau_{3}/#tau_{1}"      ,100,0,1)); histGs.back()->setUnderOverflow(true,true);
//    histGs.push_back(new HistogramGetter("6","fj_tau2/fj_tau1"            ,"#tau_{2}/#tau_{1}"      ,100,0,1)); histGs.back()->setUnderOverflow(true,true);
//    histGs.push_back(new HistogramGetter("4","fj_puppi_tau3/fj_puppi_tau2","puppi #tau_{3}/#tau_{2}",100,0,1)); histGs.back()->setUnderOverflow(true,true);
//    histGs.push_back(new HistogramGetter("5","fj_puppi_tau3/fj_puppi_tau1","puppi #tau_{3}/#tau_{1}",100,0,1)); histGs.back()->setUnderOverflow(true,true);
//    histGs.push_back(new HistogramGetter("7","fj_puppi_tau2/fj_puppi_tau1","puppi #tau_{2}/#tau_{1}",100,0,1)); histGs.back()->setUnderOverflow(true,true);

//  //nsubjettiness
//      TString selBaseline = "(fj_pt >= 200 &&  fj_eta < 2.4 && fj_sdMass >= 110 && fj_sdMass < 210)";
//    vector<HistogramGetter*> histGs;
//    histGs.push_back(new HistogramGetter("2","fj_tau3"            ,"#tau_{3}"      ,100,0,1)); histGs.back()->setUnderOverflow(true,true);
//    histGs.push_back(new HistogramGetter("3","fj_tau2"            ,"#tau_{2}"      ,100,0,1)); histGs.back()->setUnderOverflow(true,true);
//    histGs.push_back(new HistogramGetter("6","fj_tau1"            ,"#tau_{1}"      ,100,0,1)); histGs.back()->setUnderOverflow(true,true);
//        histGs.push_back(new HistogramGetter("4","fj_tau3/fj_tau2"            ,"#tau_{3}/#tau_{2}"      ,100,0,1)); histGs.back()->setUnderOverflow(true,true);
//        histGs.push_back(new HistogramGetter("5","fj_tau3/fj_tau1"            ,"#tau_{3}/#tau_{1}"      ,100,0,1)); histGs.back()->setUnderOverflow(true,true);
//        histGs.push_back(new HistogramGetter("7","fj_tau2/fj_tau1"            ,"#tau_{2}/#tau_{1}"      ,100,0,1)); histGs.back()->setUnderOverflow(true,true);
//
////        vector<HistogramGetter*> histGs;
////        histGs.push_back(new HistogramGetter("2","fj_tau3*fj_pt"            ,"#tau_{3}"      ,100,0,300)); histGs.back()->setUnderOverflow(true,true);
////        histGs.push_back(new HistogramGetter("3","fj_tau2*fj_pt"            ,"#tau_{2}"      ,100,0,300)); histGs.back()->setUnderOverflow(true,true);
////        histGs.push_back(new HistogramGetter("6","fj_tau1*fj_pt"            ,"#tau_{1}"      ,100,0,300)); histGs.back()->setUnderOverflow(true,true);

 for(unsigned int iS = 0; sels[iS][0]; ++iS ){
   Plot * plot = new Plot(TString::Format("signal_%u",iS),TString::Format("signal, %s",selNames[iS].Data()),histGs[0]->plotInfo->xTitle,"a.u");
   for(unsigned int iV = 0; iV < histGs.size(); ++iV ){
     TString sel = TString::Format("%s && %s && %s",signalBaseline.Data(),selBaseline.Data(),sels[iS].Data());
     TH1F * h = histGs[iV]->getHistogram(tree,sel,"weight","top");
     plot->addHist(h,histGs[iV]->plotInfo->xTitle,"hist",StyleTools::colorGetter(iV),0,StyleTools::colorGetter(iV));
   }
   plot->setRatioRange(0.0,2.0);
   TCanvas * c = new TCanvas;
//   plot->drawRatios(c,0,false,"pdf");
   plot->setHeader("","");
   plot->draw(c,false,"pdf");
 }


 for(unsigned int iS = 0; sels[iS][0]; ++iS ){
   Plot * plot = new Plot(TString::Format("bkg_%u",iS),TString::Format("bkg, %s",selNames[iS].Data()),histGs[0]->plotInfo->xTitle,"a.u");
   for(unsigned int iV = 0; iV < histGs.size(); ++iV ){
     TString sel = TString::Format("%s && %s && %s",selBaseline.Data(),bkgBaseline.Data(),sels[iS].Data());
     TH1F * h = histGs[iV]->getHistogram(tree,sel,"weight","top");
     plot->addHist(h,histGs[iV]->plotInfo->xTitle,"hist",StyleTools::colorGetter(iV),0,StyleTools::colorGetter(iV));
   }
   plot->setRatioRange(0.0,2.0);
   plot->setHeader("","");
   TCanvas * c = new TCanvas;
//   plot->drawRatios(c,0,false,"pdf");
   plot->draw(c,false,"pdf");
 }


//
//
//  //bkg
//
//  for(unsigned int iB = 0; bkgTypes[iB][0]; ++iB){
//    Plot * fake_plot = new Plot(TString::Format("fake_%u",iB),bkgNames[iB],fakeTopPTGeter->plotInfo->xTitle,"a.u");
//    for(unsigned int iS = 0; bkgSels[iS][0]; ++iS ){
//      TString sel = TString::Format("%s && %s && %s",bkgBaseline.Data(),bkgTypes[iB].Data(), bkgSels[iS].Data());
//      TH1F * h = fakeTopPTGeter->getHistogram(tree,sel,"weight","top");
//      fake_plot->addHist(h,bkgSelNames[iS],"hist",StyleTools::colorGetter(iS),0,StyleTools::colorGetter(iS));
//    }
//    fake_plot->setRatioRange(0.0,1.0);
//    TCanvas * c = new TCanvas;
//    fake_plot->drawRatios(c,0,false,"pdf");
//  }



}


#endif

void studyBoostedTopProperties(){
//  StyleTools::SetTDRStyle();
//  gStyle->SetTitleOffset(1.400,"Y");
//  gStyle->SetTitleOffset(0.950,"X");
//  gStyle->SetPadTopMargin   (0.08);
  gStyle->SetOptStat(0);
//  plotTopEfficiency();
  studyMass();
}
