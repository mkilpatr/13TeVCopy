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

void plotHTTEfficiency() {
//  TTree * toptree = getTree("T2tt_HTT0p8_tree.root");
//  TTree * bkgtree = getTree("T2tt_ca8_tree.root");

  TTree * toptree = getTree("T2tt_HTT1p5_tree.root");
  TTree * bkgtree = getTree("Znunu_HTT1p5.root");


  TString signalBaseline = "( top_pt > 0)";
  TString signalSels[] = {
      "1.0",
//      "top_maxDR/2 < 1.5",
      "top_maxDR/2 < 1.5 && top_fj_maxDR < 1.5 && fj_pt >= 200 &&  fj_eta < 2.4",
      "top_maxDR/2 < 1.5 && top_fj_maxDR < 1.5 && fj_pt >= 200 &&  fj_eta < 2.4 && min(min(fj_w1_pt,fj_w2_pt),fj_b_pt) > 20",
      "top_maxDR/2 < 1.5 && top_fj_maxDR < 1.5 && fj_pt >= 200 &&  fj_eta < 2.4 && min(min(fj_w1_pt,fj_w2_pt),fj_b_pt) > 20 && (fj_subjet_mass > 125 && fj_subjet_mass < 200) &&fj_tau3/fj_tau2 < 0.65 && 2*fj_subjet_mass/fj_subjet_pt - fj_ropt > -.5 && fj_subjet_w_mass/fj_subjet_mass > .4 && fj_subjet_w_mass/fj_subjet_mass < .65",
      "top_maxDR/2 < 1.5 && top_fj_maxDR < 1.5 && fj_pt >= 200 &&  fj_eta < 2.4 && min(min(fj_w1_pt,fj_w2_pt),fj_b_pt) > 20 && (fj_subjet_mass > 100 && fj_subjet_mass < 300) &&fj_tau3/fj_tau2 < 0.75 && 2*fj_subjet_mass/fj_subjet_pt - fj_ropt > -.3 && fj_subjet_w_mass/fj_subjet_mass > .4 && fj_subjet_w_mass/fj_subjet_mass < .7",
      "top_maxDR/2 < 1.5 && top_fj_maxDR < 1.5 && fj_pt >= 200 &&  fj_eta < 2.4 && min(min(fj_w1_pt,fj_w2_pt),fj_b_pt) > 30 && (fj_subjet_mass > 100 && fj_subjet_mass < 300) && fj_frec  < 0.47 && fj_tau3/fj_tau2  < 0.97 && (fj_w1_csv > 0.46 ||fj_w2_csv > 0.46 ||fj_b_csv > 0.46 )",
//      "top_maxDR/2 < 1.5 && top_fj_maxDR < fj_ropt && fj_pt >= 200 &&  fj_eta < 2.4",
      ""};
  TString signalSelNames[] = {
  "inclusive",
//  "#DeltaR_{q} < 1.5",
  "quarks w/in one fj w/ p_{T} > 200 GeV",
  "+pt",
  "strict mass",
  "new wp",
  "wp",
  ""
  };

  TString bkgTypes[] = {"(process == 4)",""};
  TString bkgNames[] = {"Fake Z#rightarrow#nu#nu"};

  TString bkgBaseline = "(fj_pt >= 200 &&  fj_eta < 2.4)";
  TString bkgSels[] = {"1.0",
      "min(min(fj_w1_pt,fj_w2_pt),fj_b_pt) > 20",
      "min(min(fj_w1_pt,fj_w2_pt),fj_b_pt) > 20 && min(min(fj_w1_pt,fj_w2_pt),fj_b_pt) > 20 && (fj_subjet_mass > 125 && fj_subjet_mass < 200) &&fj_tau3/fj_tau2 < 0.65 && 2*fj_subjet_mass/fj_subjet_pt - fj_ropt > -.5 && fj_subjet_w_mass/fj_subjet_mass > .4 && fj_subjet_w_mass/fj_subjet_mass < .65",
      "min(min(fj_w1_pt,fj_w2_pt),fj_b_pt) > 20 && (fj_subjet_mass > 100 && fj_subjet_mass < 300) &&fj_tau3/fj_tau2 < 0.75 && 2*fj_subjet_mass/fj_subjet_pt - fj_ropt > -.3 && fj_subjet_w_mass/fj_subjet_mass > .4 && fj_subjet_w_mass/fj_subjet_mass < .7",
      "min(min(fj_w1_pt,fj_w2_pt),fj_b_pt) >30 && (fj_subjet_mass > 100 && fj_subjet_mass < 300) && fj_frec  < 0.47 && fj_tau3/fj_tau2  < 0.97 && (fj_w1_csv > 0.46 ||fj_w2_csv > 0.46 ||fj_b_csv > 0.46 )",
      ""};
  TString bkgSelNames[] = {"inclusive","pt","strict mass","new wp","wp",""};



  HistogramGetter * trueTopPTGeter = new HistogramGetter("top_pt","top_pt","top quark #it{p}_{T}",40,0,1000); trueTopPTGeter->setUnderOverflow(true,true);
  HistogramGetter * fakeTopPTGeter = new HistogramGetter("fj_pt","fj_pt","fat jet #it{p}_{T}",40,0,1000);     fakeTopPTGeter->setUnderOverflow(true,true);

  //signal
  Plot * true_plot = new Plot("topPT","",trueTopPTGeter->plotInfo->xTitle,"a.u");
  for(unsigned int iS = 0; signalSels[iS][0]; ++iS ){
    TString sel = TString::Format("%s && %s",signalBaseline.Data(),signalSels[iS].Data());
    TH1F * h = trueTopPTGeter->getHistogram(toptree,sel,"weight","top");
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
      TH1F * h = fakeTopPTGeter->getHistogram(bkgtree,sel,"weight","top");
      fake_plot->addHist(h,bkgSelNames[iS],"hist",StyleTools::colorGetter(iS),0,StyleTools::colorGetter(iS));
      cout << "sig " << iS <<" " << h->Integral(0,-1)<<endl;
    }
    fake_plot->setRatioRange(0.0,1.0);
    TCanvas * c = new TCanvas;
    fake_plot->drawRatios(c,0,false,"pdf");
}


};

void studyHTTVariables() {

  TTree * toptree = getTree("T2tt_850_100_HTT1p5_0_tree.root");
  TTree * bkgtree = getTree("SM_HTT1p5.root");

  TString signalBaseline = "(top_pt > 0 && top_maxDR/2 < 1.5 && top_fj_maxDR < 1.5)";
  TString bkgBaseline    = "(process != 2 || (top_pt <= 0 || top_fj_maxDR > 1.8))";
//
//
//  TString sels[] = { "(fj_pt >= 200 &&  fj_pt <= 300)","(fj_pt >= 300 &&  fj_pt <= 500)","(fj_pt >= 500 &&  fj_pt <= 900)","(fj_pt >= 900)",""};
//  TString selNames[] = { "200 < fj p_{T} < 300","300 < fj p_{T} < 500","500 < fj p_{T} < 900","900 < fj p_{T}"};
  TString sels[] = { "(fj_pt >= 300 &&  fj_pt <= 500)","(fj_pt >= 500 &&  fj_pt <= 900)","(fj_pt >= 900)",""};
  TString selNames[] = { "300 < fj p_{T} < 500","500 < fj p_{T} < 900","900 < fj p_{T}"};

//  TString selBaseline = "(fj_pt >= 200 &&  fj_eta < 2.4) && min(min(fj_w1_pt,fj_w2_pt),fj_b_pt) > 20 && max(max(fj_w1_csv,fj_w2_csv),fj_b_csv) > .46 &&(fj_subjet_mass > 125 && fj_subjet_mass < 300)&&fj_tau3/fj_tau2 < 0.65 && fj_subjet_w_bycsv_mass > 50 && fj_subjet_w_bycsv_mass < 150 && fj_subjet_w_bycsv_mass/fj_subjet_mass > .4 && 2*fj_subjet_mass/fj_subjet_pt - fj_ropt > -.3";
//  TString selBaseline = "(fj_pt >= 200 &&  fj_eta < 2.4) && min(min(fj_w1_pt,fj_w2_pt),fj_b_pt) > 20 && max(max(fj_w1_csv,fj_w2_csv),fj_b_csv) > .46 &&fj_tau3/fj_tau2 < 0.65  && fj_subjet_w_bycsv_mass/fj_subjet_mass > .4 && 2*fj_subjet_mass/fj_subjet_pt - fj_ropt > -.3";
//  TString selBaseline = "(fj_pt >= 200 &&  fj_eta < 2.4) && min(min(fj_w1_pt,fj_w2_pt),fj_b_pt) > 20 && max(max(fj_w1_csv,fj_w2_csv),fj_b_csv) > .46 &&(fj_subjet_mass > 100 && fj_subjet_mass < 250)&&fj_tau3/fj_tau2 < 0.75 && fj_subjet_w_bycsv_mass > 50 && fj_subjet_w_bycsv_mass < 125";
  TString selBaseline = "(fj_pt >= 200 &&  fj_eta < 2.4) && min(min(fj_w1_pt,fj_w2_pt),fj_b_pt) > 20 && fj_subjet_mass > 100 && fj_subjet_mass < 300 && fj_subjet_w_bycsv_mass > 50 && fj_subjet_w_bycsv_mass < 125 && fj_tau3/fj_tau2 < 0.75&& fj_subjet_w_bycsv_mass/fj_subjet_mass > 0.35 && fj_subjet_w_bycsv_mass/fj_subjet_mass < 0.65 && 2*fj_subjet_mass/fj_subjet_pt - fj_ropt > -0.3  && fj_subjet_pt > 200";
  vector<HistogramGetter*> histGs;

  //Mass
////    TString selBaseline = "(fj_pt >= 200 &&  fj_eta < 2.4 && min(min(fj_w1_pt,fj_w2_pt),fj_b_pt) > 20) && fj_subjet_mass > 100 && fj_subjet_mass < 300";
//  vector<HistogramGetter*> histGs;
//  histGs.push_back(new HistogramGetter("fj_mass","fj_mass","mass",100,0,500));                                 histGs.back()->setUnderOverflow(true,true);
//  histGs.push_back(new HistogramGetter("fj_sdMass","fj_sdMass","soft drop mass",100,0,500));                   histGs.back()->setUnderOverflow(true,true);
//  histGs.push_back(new HistogramGetter("fj_subjet_mass","fj_subjet_mass","subjet mass",100,0,500));                 histGs.back()->setUnderOverflow(true,true);
//  histGs.push_back(new HistogramGetter("fj_subjet_w_mass","fj_subjet_w_mass","subjet w mass",100,0,500));                 histGs.back()->setUnderOverflow(true,true);
//  histGs.push_back(new HistogramGetter("fj_subjet_w_bycsv_mass","fj_subjet_w_bycsv_mass","subjet w bycsv_mass",100,0,500));                 histGs.back()->setUnderOverflow(true,true);
//  histGs.push_back(new HistogramGetter("fj_b_mass","fj_b_mass","subjet b mass",100,0,500));                 histGs.back()->setUnderOverflow(true,true);

//    histGs.push_back(new HistogramGetter("topmva_new","topmva_new","topmva_new",100,-1,1));                 histGs.back()->setUnderOverflow(true,true);

  //csv
//      TString selBaseline = "(fj_pt >= 200 &&  fj_eta < 2.4&& min(min(fj_w1_pt,fj_w2_pt),fj_b_pt) > 20  && fj_mass > 100 && fj_mass < 300 )";
//    vector<HistogramGetter*> histGs;
//    histGs.push_back(new HistogramGetter("mincsv","min(min(fj_w1_csv,fj_w2_csv),fj_b_csv)","minsubjetcsv",100,0,1));                                 histGs.back()->setUnderOverflow(true,true);
//    histGs.push_back(new HistogramGetter("maxcsv","max(max(fj_w1_csv,fj_w2_csv),fj_b_csv)","maxsubjetcsv",100,0,1));                                 histGs.back()->setUnderOverflow(true,true);
//    histGs.push_back(new HistogramGetter("maxwcsv","max(fj_w1_csv,fj_w2_csv)","maxwsubjetcsv",100,0,1));                                 histGs.back()->setUnderOverflow(true,true);
//    histGs.push_back(new HistogramGetter("bcsv","fj_b_csv","bsubjetcsv",100,0,1));                                 histGs.back()->setUnderOverflow(true,true);


    //subjet info
//      TString selBaseline = "(fj_pt >= 200 &&  fj_eta < 2.4) && (fj_subjet_mass > 100 && fj_subjet_mass < 300) &&fj_tau3/fj_tau2 < 0.75 && 2*fj_subjet_mass/fj_subjet_pt - fj_ropt > -.3 && fj_subjet_w_mass/fj_subjet_mass > .4 && fj_subjet_w_mass/fj_subjet_mass < .7";
//    vector<HistogramGetter*> histGs;
//    histGs.push_back(new HistogramGetter("minsubjetpt","min(min(fj_w1_pt,fj_w2_pt),fj_b_pt)","minsubjetpt",100,0,500));                                 histGs.back()->setUnderOverflow(true,true);
//    histGs.push_back(new HistogramGetter("2mopt","2*fj_subjet_mass/fj_subjet_pt","2*fj_subjet_mass/fj_subjet_pt",100,0,2));                                 histGs.back()->setUnderOverflow(true,true);


  //DR
//    TString selBaseline = "(fj_pt >= 200 &&  fj_eta < 2.4)&& min(min(fj_w1_pt,fj_w2_pt),fj_b_pt) > 20  && fj_mass > 100 && fj_mass < 300 && fj_tau3/fj_tau2 < 0.75" ;
//  vector<HistogramGetter*> histGs;
////  histGs.push_back(new HistogramGetter("fj_ropt","fj_ropt","ropt",20,0,2));                                 histGs.back()->setUnderOverflow(true,true);
  histGs.push_back(new HistogramGetter("2moptmopt","2*fj_subjet_mass/fj_subjet_pt - fj_ropt","2*fj_subjet_mass/fj_subjet_pt - fj_ropt",100,-2,2));                                 histGs.back()->setUnderOverflow(true,true);
//    histGs.push_back(new HistogramGetter("subjet_w_massosubjet_mass","fj_subjet_w_mass/fj_subjet_mass","subjet_w_mass/subjet_mass",100,0,2));                                 histGs.back()->setUnderOverflow(true,true);
    histGs.push_back(new HistogramGetter("subjet_w_massosubjet_mass2","fj_subjet_w_bycsv_mass/fj_subjet_mass","subjet_w_bycsv_mass/subjet_mass",100,-2,2));                                 histGs.back()->setUnderOverflow(true,true);
//    histGs.push_back(new HistogramGetter("bcsv","fj_b_csv > max(fj_w1_csv,fj_w2_csv)","subjet_b_csv > max(subjet_w1_csv,subjet_w2_csv)",100,0,2));                                 histGs.back()->setUnderOverflow(true,true);
    histGs.push_back(new HistogramGetter("pt","fj_subjet_w_bycsv_pt/fj_subjet_pt","fj_W_pt/fj_subjet_pt",100,-2,2));                                 histGs.back()->setUnderOverflow(true,true);
////  histGs.push_back(new HistogramGetter("2mopt","2*fj_subjet_mass/fj_subjet_pt","2*fj_subjet_mass/fj_subjet_pt",100,0,2));                                 histGs.back()->setUnderOverflow(true,true);
////  histGs.push_back(new HistogramGetter("fj_roptcalc","fj_roptcalc","fj_roptcalc",100,0,2));                                 histGs.back()->setUnderOverflow(true,true);
////  histGs.push_back(new HistogramGetter("fj_ptforoptofj_pt","fj_ptforopt/fj_pt","fj_ptforoptofj_pt",100,0,2));                                 histGs.back()->setUnderOverflow(true,true);

//  //nsubjettiness
//  TString selBaseline = "(fj_pt >= 200 &&  fj_eta < 2.4)&& min(min(fj_w1_pt,fj_w2_pt),fj_b_pt) > 20  && fj_mass > 100 && fj_mass < 300" ;
//    vector<HistogramGetter*> histGs;
//    histGs.push_back(new HistogramGetter("2","fj_tau3/fj_tau2"            ,"#tau_{3}/#tau_{2}"      ,100,0,1)); histGs.back()->setUnderOverflow(true,true);
//    histGs.push_back(new HistogramGetter("3","fj_tau3/fj_tau1"            ,"#tau_{3}/#tau_{1}"      ,100,0,1)); histGs.back()->setUnderOverflow(true,true);
////    histGs.push_back(new HistogramGetter("6","fj_tau2/fj_tau1"            ,"#tau_{2}/#tau_{1}"      ,100,0,1)); histGs.back()->setUnderOverflow(true,true);
//    histGs.push_back(new HistogramGetter("4","fj_ropt_tau3/fj_ropt_tau2"            ,"r #tau_{3}/#tau_{2}"      ,100,0,1)); histGs.back()->setUnderOverflow(true,true);
//    histGs.push_back(new HistogramGetter("5","fj_ropt_tau3/fj_ropt_tau1"            ,"r #tau_{3}/#tau_{1}"      ,100,0,1)); histGs.back()->setUnderOverflow(true,true);
////    histGs.push_back(new HistogramGetter("1","fj_ropt_tau2/fj_ropt_tau1"            ,"r #tau_{2}/#tau_{1}"      ,100,0,1)); histGs.back()->setUnderOverflow(true,true);

  //Mass xDR
//    TString selBaseline = "(fj_pt >= 200 &&  fj_eta < 2.4)";
//  vector<HistogramGetter*> histGs;
////  histGs.push_back(new HistogramGetter("fj_mass","fj_mass","mass",100,0,500));                                 histGs.back()->setUnderOverflow(true,true);
////  histGs.push_back(new HistogramGetter("fj_sdMass","fj_sdMass","soft drop mass",100,0,500));                   histGs.back()->setUnderOverflow(true,true);
//  histGs.push_back(new HistogramGetter("fj_subjet_mass","fj_subjet_mass","subjet mass",100,0,500));                 histGs.back()->setUnderOverflow(true,true);
//  histGs.push_back(new HistogramGetter("DR*PTo2","fj_ropt*fj_pt/2","fj_ropt*fj_pt/2",100,0,500));                 histGs.back()->setUnderOverflow(true,true);
//  histGs.push_back(new HistogramGetter("DR*subjetPTo2","fj_ropt*fj_subjet_pt/2","fj_ropt*fj_subjet_pt/2",100,0,500));                 histGs.back()->setUnderOverflow(true,true);


 for(unsigned int iS = 0; sels[iS][0]; ++iS ){
   Plot * plot = new Plot(TString::Format("signal_%u",iS),TString::Format("signal, %s",selNames[iS].Data()),histGs[0]->plotInfo->xTitle,"a.u");
   for(unsigned int iV = 0; iV < histGs.size(); ++iV ){
     TString sel = TString::Format("%s && %s && %s",signalBaseline.Data(),selBaseline.Data(),sels[iS].Data());
     TH1F * h = histGs[iV]->getHistogram(toptree,sel,"weight","top");
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
     TH1F * h = histGs[iV]->getHistogram(bkgtree,sel,"weight","top");
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


void plotTopEfficiency() {

  TTree * tree = getTree("Znunu_ak8.root");

  TString signalBaseline = "( top_pt > 0)";
  TString signalSels[] = {
      "1.0",
      "top_maxDR/2 < 0.8",
//      "top_maxDR/2 < 0.8 && top_fj_maxDR < 0.8 && fj_pt >= 200 &&  fj_eta < 2.4",
      "top_maxDR/2 < 0.8 && top_fj_maxDR < 0.8 && fj_pt >= 200 &&  fj_eta < 2.4",
      "top_maxDR/2 < 0.8 && top_fj_maxDR < 0.8 && fj_pt >= 200 &&  fj_eta < 2.4 && (fj_sdMass >= 110 && fj_sdMass < 210 && fj_tau3/fj_tau2 < 0.69)",
//      "top_maxDR/2 < 0.8 && top_fj_maxDR < 0.8 && fj_pt >= 200 &&  fj_eta < 2.4 && (fj_sdMass >= 105 && fj_sdMass < 220   && fj_tau3/fj_tau2 < 0.81 && (fj_sj1_csv > 0.46 ||fj_sj2_csv > 0.46 ))",
//      "top_maxDR/2 < 0.8 && top_fj_maxDR < 0.8 && fj_pt >= 200 &&  fj_eta < 2.4 && (fj_puppi_sdMass >= 105 && fj_puppi_sdMass < 210 && fj_puppi_tau3/fj_puppi_tau2 < 0.80 && (fj_puppi_sj1_csv > 0.46 ||fj_puppi_sj2_csv > 0.46 )  )",
      ""};
//  TString signalSels[] = {"1.0","top_maxDR/2 < 0.8","top_maxDR/2 < 0.8 && fj_pt >= 200 &&  fj_eta < 2.4",""};
  TString signalSelNames[] = {
  "inclusive",
  "#DeltaR_{q} < 0.8",
  "quarks w/in one fj w/ p_{T} > 200 GeV", "",

//      "Matched FJ",
  "ICHEP tagger",
//  "loose CMS tagger",
//  "loose CMS puppi tagger",
//  "+test"

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


//  bkg

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


//Comp new to old

void compTaggers() {
  TTree * oldBkg = getTree("SM_ak8.root");
  TTree * oldSig = getTree("T2tt_850_100_ak8_0_tree.root");
  TTree * newBkg = getTree("SM_HTT1p5_addmva.root");
  TTree * newSig = getTree("T2tt_850_100_HTT1p5_0_tree_addmva.root");
  TString oldSel ="(fj_pt > 200 && fj_eta < 2.4 && fj_sdMass >= 110 && fj_sdMass < 210 && fj_tau3/fj_tau2 < 0.69)";
//  TString newsigSel = "top_maxDR/2 < 1.5 && top_fj_maxDR < 1.5 && top_pt > 0";
  TString newsigSel = "top_maxDR/2 < 1.5 && top_fj_maxDR < 1.5 && top_pt > 0";
  TString oldsigSel = "top_maxDR/2 < 0.8 && top_fj_maxDR < 0.8 && top_pt > 0";
//  TString newSel = "(fj_pt > 200 && fj_eta < 2.4 && min(min(fj_w1_pt,fj_w2_pt),fj_b_pt) > 20 && (fj_subjet_mass > 100 && fj_subjet_mass < 300) &&fj_tau3/fj_tau2 < 0.75 && 2*fj_subjet_mass/fj_subjet_pt - fj_ropt > -.3 && fj_subjet_w_mass/fj_subjet_mass > .4 && fj_subjet_w_mass/fj_subjet_mass < .7)";
//  TString newSel = "(fj_pt >= 200 &&  fj_eta < 2.4) && min(min(fj_w1_pt,fj_w2_pt),fj_b_pt) > 20 &&(fj_subjet_mass > 125 && fj_subjet_mass < 300)&&fj_tau3/fj_tau2 < 0.65  && 2*fj_subjet_mass/fj_subjet_pt - fj_ropt > -.3 && fj_subjet_w_mass/fj_subjet_mass > .4";
//  TString newSel = "(fj_pt >= 200 &&  fj_eta < 2.4) && min(min(fj_w1_pt,fj_w2_pt),fj_b_pt) > 20 && max(max(fj_w1_csv,fj_w2_csv),fj_b_csv) > .46 &&(fj_subjet_mass > 125 && fj_subjet_mass < 300)&&fj_tau3/fj_tau2 < 0.65 && fj_subjet_w_bycsv_mass > 50 && fj_subjet_w_bycsv_mass < 150 && fj_subjet_w_bycsv_mass/fj_subjet_mass > .4 && 2*fj_subjet_mass/fj_subjet_pt - fj_ropt > -.3";
//  TString newSel = "(fj_pt >= 200 &&  fj_eta < 2.4) && min(min(fj_w1_pt,fj_w2_pt),fj_b_pt) > 20 &&(fj_subjet_mass > 100 && fj_subjet_mass < 300)&&fj_tau3/fj_tau2 < 0.80 && fj_subjet_w_bycsv_mass > 50 && fj_subjet_w_bycsv_mass < 125";
//  TString newSel = "(fj_pt >= 200 &&  fj_eta < 2.4) && min(min(fj_w1_pt,fj_w2_pt),fj_b_pt) > 20 && fj_subjet_mass > 100 && fj_subjet_mass < 400 && fj_subjet_pt > 200  &&   (fj_pt > 600 ? topmva_new > .70 : (fj_pt > 400  ?  topmva_new > .60 : topmva_new > .40  ))";
//  TString newSel = "(fj_pt >= 200 &&  fj_eta < 2.4) && min(min(fj_w1_pt,fj_w2_pt),fj_b_pt) > 20 && fj_subjet_mass > 50 && fj_subjet_mass < 600 && fj_subjet_pt > 200  && topmva_new > .71";

  //  TString newSel = "(fj_pt >= 200 &&  fj_eta < 2.4) && min(min(fj_w1_pt,fj_w2_pt),fj_b_pt) > 20  && fj_subjet_mass > 125 && fj_subjet_mass < 400 && fj_subjet_w_bycsv_mass > 50 && fj_subjet_w_bycsv_mass < 125 && fj_tau3/fj_tau2 < 0.65&& fj_subjet_w_bycsv_mass/fj_subjet_mass > 0.35 && fj_subjet_w_bycsv_mass/fj_subjet_mass < 0.65 && 2*fj_subjet_mass/fj_subjet_pt - fj_ropt > -0.3  && fj_subjet_pt > 200";
  TString newSel = "(fj_pt >= 200 &&  fj_eta < 2.4) && min(min(fj_w1_pt,fj_w2_pt),fj_b_pt) > 20 && fj_subjet_pt > 300 && fj_subjet_mass > 125 && fj_subjet_mass < 400 && fj_subjet_mass < 400 && fj_subjet_w_bycsv_mass > 50 && fj_tau3/fj_tau2 < 0.66 && 2*fj_subjet_mass/fj_subjet_pt - fj_ropt > -0.4 && fj_subjet_w_bycsv_mass/fj_subjet_mass > 0.35 && fj_subjet_w_bycsv_mass/fj_subjet_mass < 0.65";
  HistogramGetter * trueTopPTGeter = new HistogramGetter("top_pt","top_pt","top quark #it{p}_{T}",40,0,1000); trueTopPTGeter->setUnderOverflow(true,true);
  HistogramGetter * fakeTopPTGeter = new HistogramGetter("fj_pt","fj_pt","fat jet #it{p}_{T}",40,0,1000);     fakeTopPTGeter->setUnderOverflow(true,true);

//  HistogramGetter * trueTopPTGeter = new HistogramGetter("topmva_new2","topmva_new","top quark #it{p}_{T}",200,-1,1); trueTopPTGeter->setUnderOverflow(true,true);
//  HistogramGetter * fakeTopPTGeter = new HistogramGetter("topmva_new","topmva_new","fat jet #it{p}_{T}",200,-1,1);     fakeTopPTGeter->setUnderOverflow(true,true);

  //signal
  Plot * true_plot = new Plot("topPT","",trueTopPTGeter->plotInfo->xTitle,"a.u");
  TString totNewSigSel = TString::Format("%s && %s",newsigSel.Data(),newSel.Data());
  TString totOldSigSel = TString::Format("%s && %s",oldsigSel.Data(),oldSel.Data());
  TH1F * s_2 = trueTopPTGeter->getHistogram(newSig,totNewSigSel,"weight","top");
  TH1F * s_3 = trueTopPTGeter->getHistogram(oldSig,totOldSigSel,"weight","top");
  true_plot->addHist(s_3,"old","hist",StyleTools::colorGetter(0),0,StyleTools::colorGetter(0));
  true_plot->addHist(s_2,"new","hist",StyleTools::colorGetter(1),0,StyleTools::colorGetter(1));
  true_plot->setRatioRange(0.0,1.0);
  TCanvas * c = new TCanvas;
  true_plot->drawRatios(c,0,false,"pdf");
//  true_plot->draw(c,false,"pdf");


  Plot * fake_plot = new Plot("fake","",fakeTopPTGeter->plotInfo->xTitle,"a.u");
  TString totNewBkgSel = TString::Format("(process != 2 || (top_pt <= 0 || top_fj_maxDR > 1.5)) && %s",newSel.Data());
  TString totOldBkgSel = TString::Format("(process != 2 || (top_pt <= 0 || top_fj_maxDR > 0.8)) && %s",oldSel.Data());
//  TString totNewBkgSel = TString::Format("(process != 2 || (top_pt >= 0 && top_fj_maxDR < 1.5)) && %s",newSel.Data());
//  TString totOldBkgSel = TString::Format("(process != 2 || (top_pt >= 0 && top_fj_maxDR < 0.8)) && %s",oldSel.Data());
  TH1F * b_2 = fakeTopPTGeter->getHistogram(newBkg,totNewBkgSel,"weight","top");
  TH1F * b_3 = fakeTopPTGeter->getHistogram(oldBkg,totOldBkgSel,"weight","top");

  fake_plot->addHist(b_3,"old","hist",StyleTools::colorGetter(0),0,StyleTools::colorGetter(0));
  fake_plot->addHist(b_2,"new","hist",StyleTools::colorGetter(1),0,StyleTools::colorGetter(1));
  fake_plot->setRatioRange(0.0,1.0);
  c = new TCanvas;
  fake_plot->drawRatios(c,0,false,"pdf");
//  fake_plot->draw(c,false,"pdf");


}

void compTaggers2() {

  vector<TString> files; vector<TString> names;
  TString sigSel = "top_maxDR/2 < 1.5 && top_fj_maxDR < 1.5 && top_pt > 0";
  TString bkgSel = "(process != 2 || (top_pt <= 0 || top_fj_maxDR > 1.8))";
  TString basesel =  "((fj_pt >= 200 &&  fj_eta < 2.4) && min(min(fj_w1_pt,fj_w2_pt),fj_b_pt) > 20 && fj_subjet_mass > 50 && fj_subjet_mass < 600 && fj_subjet_pt > 200)";
  TString var = "topmva_new";

  files.push_back("T2tt_500_325_HTT1p5_0_tree_addmva.root"); names.push_back("T2tt_500_325");
  files.push_back("T2tt_850_100_HTT1p5_0_tree_addmva.root"); names.push_back("T2tt_850_100");
  files.push_back("ttbar_HTT1p5_addmva.root"); names.push_back("ttbar");
  files.push_back("Znunu_HTT1p5_addmva.root"); names.push_back("znunu");

  Plot * plot1 = new Plot("topPT","","MVA","a.u");
  Plot * plot2 = new Plot("topPT","","MVA","a.u");
  HistogramGetter * mvaGetter = new HistogramGetter("topmva_new","topmva_new","mva",200,-1,1);     mvaGetter->setUnderOverflow(true,true);

  for(unsigned int iF = 0; iF < files.size(); ++iF){
    TTree * tree = getTree(files[iF]);
    TString sel = iF > 1 ? TString::Format("%s && %s", basesel.Data(), bkgSel.Data()) : TString::Format("%s && %s", basesel.Data(), sigSel.Data());
    TH1F * h = mvaGetter->getHistogram(tree,sel,"weight","top");
    plot1->addHist(h,names[iF],"hist",StyleTools::colorGetter(iF),0,StyleTools::colorGetter(iF));

    TH1F* h2 = (TH1F*)h->Clone();
    for(unsigned int iB = 1; iB <=h->GetNbinsX(); ++iB){
      h2->SetBinContent(iB, h->Integral(iB,-1));
    }
    plot2->addHist(h2,names[iF],"hist",StyleTools::colorGetter(iF),0,StyleTools::colorGetter(iF));
  }
  TCanvas * c = new TCanvas;
  plot1->draw(c,false,"pdf");
  TCanvas * c2 = new TCanvas;
  plot2->draw(c2,false,"pdf");

}

void compEffs() {

  vector<TString> files; vector<TString> names; vector<TString> sels;

//  files.push_back("T2tt_850_100_ak8_0_tree.root"); names.push_back("Inclusive"); sels.push_back("(top_pt > 0)");
//  files.push_back("T2tt_850_100_ak8_0_tree.root"); names.push_back("#DeltaR(q) < 0.8"); sels.push_back("(top_maxDR/2 < 0.8 && top_fj_maxDR < 0.8 && top_pt > 0)");
//  files.push_back("T2tt_850_100_ak8_0_tree.root"); names.push_back("#DeltaR(q) < 1.5"); sels.push_back("(top_maxDR/2 < 1.5 && top_fj_maxDR < 1.5 && top_pt > 0)");
  files.push_back("T2tt_850_100_ak8_0_tree.root"); names.push_back("ICHEP tagger"); sels.push_back("(top_maxDR/2 < 0.8 && top_fj_maxDR < 0.8 && top_pt > 0) && (fj_pt > 400 && fj_eta < 2.4 && fj_sdMass >= 110 && fj_sdMass < 210 && fj_tau3/fj_tau2 < 0.69)");
  files.push_back("T2tt_850_100_HTT1p5_0_tree_addmva.root"); names.push_back("HTT cut based"); sels.push_back("(top_maxDR/2 < 1.5 && top_fj_maxDR < 1.5 && top_pt > 0) && (fj_pt >= 200 &&  fj_eta < 2.4) && min(min(fj_w1_pt,fj_w2_pt),fj_b_pt) > 20 && fj_subjet_pt > 300 && fj_subjet_mass > 125 && fj_subjet_mass < 500 && fj_subjet_w_bycsv_mass > 50 && fj_subjet_w_bycsv_mass < 125 && fj_tau3/fj_tau2 < 0.70 && 2*fj_subjet_mass/fj_subjet_pt - fj_ropt > -0.4 && fj_subjet_w_bycsv_mass/fj_subjet_mass > 0.35 && fj_subjet_w_bycsv_mass/fj_subjet_mass < 0.65");
  files.push_back("T2tt_850_100_HTT1p5_0_tree_addmva.root"); names.push_back("HTT MVA loose"); sels.push_back("(top_maxDR/2 < 1.5 && top_fj_maxDR < 1.5 && top_pt > 0) && ((fj_pt >= 200 &&  fj_eta < 2.4) && min(min(fj_w1_pt,fj_w2_pt),fj_b_pt) > 20 && fj_subjet_mass > 50 && fj_subjet_mass < 600 && fj_subjet_pt > 200 && topmva_new > 0.54)");
  files.push_back("T2tt_850_100_HTT1p5_0_tree_addmva.root"); names.push_back("HTT MVA tight"); sels.push_back("(top_maxDR/2 < 1.5 && top_fj_maxDR < 1.5 && top_pt > 0) && ((fj_pt >= 200 &&  fj_eta < 2.4) && min(min(fj_w1_pt,fj_w2_pt),fj_b_pt) > 20 && fj_subjet_mass > 50 && fj_subjet_mass < 600 && fj_subjet_pt > 200 && topmva_new > 0.78)");

  files.push_back("ttbar_ak8.root"); names.push_back("ICHEP tagger"); sels.push_back("(process != 2 || (top_pt <= 0 || top_fj_maxDR > 0.8)) && (fj_pt > 400 && fj_eta < 2.4 && fj_sdMass >= 110 && fj_sdMass < 210 && fj_tau3/fj_tau2 < 0.69)");
  files.push_back("ttbar_HTT1p5_addmva.root"); names.push_back("HTT cut based"); sels.push_back("(process != 2 || (top_pt <= 0 || top_fj_maxDR > 1.5)) && (fj_pt >= 200 &&  fj_eta < 2.4) && min(min(fj_w1_pt,fj_w2_pt),fj_b_pt) > 20 && fj_subjet_pt > 300 && fj_subjet_mass > 125 && fj_subjet_mass < 500 &&  fj_subjet_w_bycsv_mass > 50 && fj_subjet_w_bycsv_mass < 125  && fj_tau3/fj_tau2 < 0.70 && 2*fj_subjet_mass/fj_subjet_pt - fj_ropt > -0.4 && fj_subjet_w_bycsv_mass/fj_subjet_mass > 0.35 && fj_subjet_w_bycsv_mass/fj_subjet_mass < 0.65");
  files.push_back("ttbar_HTT1p5_addmva.root"); names.push_back("HTT MVA loose"); sels.push_back("(process != 2 || (top_pt <= 0 || top_fj_maxDR > 1.5)) && ((fj_pt >= 200 &&  fj_eta < 2.4) && min(min(fj_w1_pt,fj_w2_pt),fj_b_pt) > 20 && fj_subjet_mass > 50 && fj_subjet_mass < 600 && fj_subjet_pt > 200 && topmva_new > 0.54)");
  files.push_back("ttbar_HTT1p5_addmva.root"); names.push_back("HTT MVA tight"); sels.push_back("(process != 2 || (top_pt <= 0 || top_fj_maxDR > 1.5)) && ((fj_pt >= 200 &&  fj_eta < 2.4) && min(min(fj_w1_pt,fj_w2_pt),fj_b_pt) > 20 && fj_subjet_mass > 50 && fj_subjet_mass < 600 && fj_subjet_pt > 200 && topmva_new > 0.78)");


  int nS = 4;
  HistogramGetter * trueTopPTGeter = new HistogramGetter("top_pt","top_pt","top quark #it{p}_{T}",40,0,1000); trueTopPTGeter->setUnderOverflow(true,true);
  HistogramGetter * fakeTopPTGeter = new HistogramGetter("fj_pt","fj_pt","fat jet #it{p}_{T}",40,0,1000);     fakeTopPTGeter->setUnderOverflow(true,true);

  Plot * plot1 = new Plot("topPT","","top quark #it{p}_{T}","a.u");
  Plot * plot2 = new Plot("fake","","fat jet #it{p}_{T}","a.u");

  for(unsigned int iF = 0; iF < files.size(); ++iF){
    TTree * tree = getTree(files[iF]);
    if(iF >= nS){
      TH1F * h = fakeTopPTGeter->getHistogram(tree,sels[iF],"weight","top");
      plot2->addHist(h,names[iF],"hist",StyleTools::colorGetter(iF-nS),0,StyleTools::colorGetter(iF-nS));
    }
    else {
    TH1F * h = trueTopPTGeter->getHistogram(tree,sels[iF],"weight","top");
    plot1->addHist(h,names[iF],"hist",StyleTools::colorGetter(iF),0,StyleTools::colorGetter(iF));
    }
  }
  TCanvas * c = new TCanvas;
  plot1->draw(c,false,"pdf");
  TCanvas * c2 = new TCanvas;
  plot2->draw(c2,false,"pdf");

}


void makeTable() {

  vector<TString> files;
  vector<TString> names;
  vector<bool>    isSig;

  files.push_back("ttbar_boostedTopTester_tree.root"    );names.push_back("ttbar");isSig.push_back(true);
  files.push_back("Znunu_boostedTopTester_tree.root"    );names.push_back("Znunu");isSig.push_back(false);
  files.push_back("T2tt_850_100_boostedTopTester_0_tree.root"    );names.push_back("T2tt_850_100");isSig.push_back(true);
  files.push_back("T2tt_500_325_boostedTopTester_0_tree.root"    );names.push_back("T2tt_500_325");isSig.push_back(true);

  vector<TString> sels;
  vector<TString> pts;
  vector<TString> matches;

  TString baseSel = "(1.0)";
  sels.push_back("Incl."               );pts.push_back("");         matches.push_back("(0.0)");
  sels.push_back("(top_CTT_pt > 0)"    );pts.push_back("top_CTT_pt");matches.push_back("(top_CTT_isMatched>0)");
//  sels.push_back("(top_HTT_mva > 0.36)");pts.push_back("top_HTT_pt");matches.push_back("(top_HTT_isMatched>0)");
  sels.push_back("(top_HTT_mva > 0.54)");pts.push_back("top_HTT_pt");matches.push_back("(top_HTT_isMatched>0)");
  sels.push_back("(top_HTT_mva > 0.78)");pts.push_back("top_HTT_pt");matches.push_back("(top_HTT_isMatched>0)");

  int nPT = 3;
  double ptBins[] = {0,300,500,1000};

  for(unsigned int iF = 0; iF < files.size(); ++iF){
    cout << endl << names[iF] << endl;
    TTree * tree = getTree(files[iF]);

    for(unsigned int iS = 0; iS < sels.size(); ++iS){
      if(pts[iS] == ""){
        HistogramGetter * histGetter = new HistogramGetter(TString::Format("%u_%u",iF,iS),"(1.0)","top quark #it{p}_{T}",1,0,2); histGetter->setUnderOverflow(true,true);
        TH1F * h = histGetter->getHistogram(tree,baseSel,"weight","top");
        cout << sels[iS] <<"\t" << h->GetBinContent(1) <<endl;
      } else {
        HistogramGetter * histGetter = new HistogramGetter(TString::Format("%u_%u",iF,iS),pts[iS],"top quark #it{p}_{T}",nPT,ptBins); histGetter->setUnderOverflow(true,true);
        TH1F * h = histGetter->getHistogram(tree,TString::Format("%s && %s",baseSel.Data(),sels[iS].Data()),"weight","top");
        cout << sels[iS] <<"\t";
        cout << h->Integral(0,-1)<< "\t";
        for(unsigned int iB = 1; iB <= h->GetNbinsX(); ++iB)
          cout << h->GetBinContent(iB)<<"\t";
        cout << endl;
        if(isSig[iF]){
          TH1F * h2 = histGetter->getHistogram(tree,TString::Format("%s && %s && %s",baseSel.Data(),sels[iS].Data(),matches[iS].Data()),"weight","top");
          cout << "Matched" <<"\t";
          cout << h2->Integral(0,-1)<< "\t";
          for(unsigned int iB = 1; iB <= h->GetNbinsX(); ++iB)
            cout << h2->GetBinContent(iB)<<"\t";
          cout << endl;
        }

      }


    }




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


  //Btagging
  TString selBaseline = "(fj_pt >= 200 &&  fj_eta < 2.4 && fj_sdMass >= 110 && fj_sdMass < 210 && fj_tau3/fj_tau2 > 0.69)";
  vector<HistogramGetter*> histGs;
  histGs.push_back(new HistogramGetter("fj_csv","fj_csv","csv",50,0,1));                                 histGs.back()->setUnderOverflow(true,true);
  histGs.push_back(new HistogramGetter("min_fj_sj1_csv_fj_sj2_csv","min(fj_sj1_csv,fj_sj2_csv)","min sj csv",50,0,1));              histGs.back()->setUnderOverflow(true,true);
  histGs.push_back(new HistogramGetter("max_fj_sj1_csv_fj_sj2_csv_","max(fj_sj1_csv,fj_sj2_csv)","max sj csv",50,0,1));                    histGs.back()->setUnderOverflow(true,true);
//  histGs.push_back(new HistogramGetter("min_fj_puppi_sj1_csv_fj_puppi_sj2_csv_","min(fj_puppi_sj1_csv,fj_puppi_sj2_csv)","min puppi sj csv",50,0,1));              histGs.back()->setUnderOverflow(true,true);
//  histGs.push_back(new HistogramGetter("max_fj_puppi_sj1_csv_fj_puppi_sj2_csv_","max(fj_puppi_sj1_csv,fj_puppi_sj2_csv)","max puppi sj csv",50,0,1));                    histGs.back()->setUnderOverflow(true,true);


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


//    //pt frac
//      TString selBaseline = "(fj_pt >= 200 &&  fj_eta < 2.4 && fj_sdMass >= 210&& fj_tau3/fj_tau2 < 0.69)";
//      vector<HistogramGetter*> histGs;
//      histGs.push_back(new HistogramGetter("1","((fj_sj1_pt > fj_sj2_pt)*fj_sj1_pt/fj_pt + !(fj_sj1_pt > fj_sj2_pt)*fj_sj2_pt/fj_pt)"  ,"max sj pt frac"      ,100,0,2)); histGs.back()->setUnderOverflow(true,true);
//      histGs.push_back(new HistogramGetter("2","(!(fj_sj1_pt > fj_sj2_pt)*fj_sj1_pt/fj_pt + (fj_sj1_pt > fj_sj2_pt)*fj_sj2_pt/fj_pt)"  ,"min sj pt frac"      ,100,0,2)); histGs.back()->setUnderOverflow(true,true);
//      histGs.push_back(new HistogramGetter("3","((fj_sj1_csv > fj_sj2_csv)*fj_sj1_pt/fj_pt + !(fj_sj1_csv > fj_sj2_csv)*fj_sj2_pt/fj_pt)"  ,"max csv sj pt frac"      ,100,0,2)); histGs.back()->setUnderOverflow(true,true);
//      histGs.push_back(new HistogramGetter("4","(!(fj_sj1_csv > fj_sj2_csv)*fj_sj1_pt/fj_pt + (fj_sj1_csv > fj_sj2_csv)*fj_sj2_pt/fj_pt)"  ,"min csv sj pt frac"      ,100,0,2)); histGs.back()->setUnderOverflow(true,true);
////      histGs.push_back(new HistogramGetter("4","((fj_puppi_sj1_pt > fj_puppi_sj2_pt)*fj_puppi_sj1_pt/fj_puppi_pt + !(fj_puppi_sj1_pt > fj_puppi_sj2_pt)*fj_puppi_sj2_pt/fj_puppi_pt)","puppi max csv sj pt frac",100,0,2)); histGs.back()->setUnderOverflow(true,true);
////      histGs.push_back(new HistogramGetter("5","(!(fj_puppi_sj1_pt > fj_puppi_sj2_pt)*fj_puppi_sj1_pt/fj_puppi_pt + (fj_puppi_sj1_pt > fj_puppi_sj2_pt)*fj_puppi_sj2_pt/fj_puppi_pt)","puppi min csv sj pt frac",100,0,2)); histGs.back()->setUnderOverflow(true,true);
////
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
//  studyMass();

//  plotHTTEfficiency();
//  studyHTTVariables();
//  compTaggers();
  makeTable();
//  compTaggers2();

//  compEffs();
}
