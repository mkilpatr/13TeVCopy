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

void plotTopCategories() {
  TTree * tree = getTree("T2tt_tree.root");
  TString baseSel = "(top_1_had && top_2_had && nlbjets >= 2 && nmbjets >= 1 && njets >= 5)";
  HistogramGetter * topPT1Geter = new HistogramGetter("top_1_pt","top_1_pt","top quark #it{p}_{T}",40,0,1000);
  HistogramGetter * topPT2Geter = new HistogramGetter("top_2_pt","top_2_pt","top quark #it{p}_{T}",40,0,1000);

  HistogramGetter * wPT1Geter = new HistogramGetter("top_1_W_pt","top_1_W_pt","W boson #it{p}_{T}",40,0,1000);
  HistogramGetter * wPT2Geter = new HistogramGetter("top_2_W_pt","top_2_W_pt","W boson #it{p}_{T}",40,0,1000);

  HistogramGetter * bPT1Geter = new HistogramGetter("top_1_b_pt","top_1_b_pt","b quark #it{p}_{T}",50,0,500);
  HistogramGetter * bPT2Geter = new HistogramGetter("top_2_b_pt","top_2_b_pt","b quark #it{p}_{T}",50,0,500);

  float drs[] = {99999,0.2,0.4,0.6,0.8,-1};
  TString drNames[] = {"inclusive","#DeltaR<0.2","#DeltaR<0.4","#DeltaR<0.6","#DeltaR<0.8"};


  float bpts[] = {0,50,30,20,10,-1};
  TString bptNames[] = {"inclusive","b p_{T}>50 GeV","b p_{T}>30 GeV","b p_{T}>20 GeV","b p_{T}>10 GeV"};
  TString qptNames[] = {"no overlapping jets","min q p_{T}>50 GeV","min q p_{T}>30 GeV","min q p_{T}>20 GeV","min q p_{T}>10 GeV"};

  Plot * topPT_plot = new Plot("topPT","",topPT1Geter->plotInfo->xTitle,"a.u");
  for(unsigned int iR = 0; drs[iR] > 0; ++iR ){
    TString top1_sel = TString::Format("%s && top_1_maxDR/2 < %f",baseSel.Data(),drs[iR]);
    TH1F * top1_h = topPT1Geter->getHistogram(tree,top1_sel,"","top1");
    TString top2_sel = TString::Format("%s && top_2_maxDR/2 < %f",baseSel.Data(),drs[iR]);
    TH1F * top2_h = topPT2Geter->getHistogram(tree,top2_sel,"","top2");
    top1_h->Add(top2_h);
    topPT_plot->addHist(top1_h,drNames[iR],"hist",StyleTools::colorGetter(iR),0,StyleTools::colorGetter(iR));
  }
  topPT_plot->setRatioRange(0.0,1.0);
  TCanvas * c = new TCanvas;
  topPT_plot->drawRatios(c,0,false,"pdf");


  Plot * wPT_plot = new Plot("wPT","",wPT1Geter->plotInfo->xTitle,"a.u");
  for(unsigned int iR = 0; drs[iR] > 0; ++iR ){
    TString top1_sel = TString::Format("%s && top_1_W_DR/2 < %f",baseSel.Data(),drs[iR]);
    TH1F * top1_h = wPT1Geter->getHistogram(tree,top1_sel,"","top1");
    TString top2_sel = TString::Format("%s && top_2_W_DR/2 < %f",baseSel.Data(),drs[iR]);
    TH1F * top2_h = wPT2Geter->getHistogram(tree,top2_sel,"","top2");
    top1_h->Add(top2_h);
    wPT_plot->addHist(top1_h,drNames[iR],"hist",StyleTools::colorGetter(iR),0,StyleTools::colorGetter(iR));
  }
  wPT_plot->setRatioRange(0.0,1.0);
  c = new TCanvas;
  wPT_plot->drawRatios(c,0,false,"pdf");


  Plot * topPT_failDR_plot = new Plot("topPT_failDR","",topPT1Geter->plotInfo->xTitle,"a.u");
  for(unsigned int iR = 0; drs[iR] > 0; ++iR ){
    TString top1_sel = TString::Format("%s && !(top_1_maxDR/2 < 0.8 && top_1_pt > 200) && top_1_W_DR/2 < %f",baseSel.Data(),drs[iR]);
    TH1F * top1_h = topPT1Geter->getHistogram(tree,top1_sel,"","top1");
    TString top2_sel = TString::Format("%s && !(top_2_maxDR/2 < 0.8 && top_2_pt > 200)  && top_2_W_DR/2 < %f",baseSel.Data(),drs[iR]);
    TH1F * top2_h = topPT2Geter->getHistogram(tree,top2_sel,"","top2");
    top1_h->Add(top2_h);
    topPT_failDR_plot->addHist(top1_h,drNames[iR],"hist",StyleTools::colorGetter(iR),0,StyleTools::colorGetter(iR));
  }
  topPT_failDR_plot->setRatioRange(0.0,1.0);
  c = new TCanvas;
  topPT_failDR_plot->drawRatios(c,0,false,"pdf");


  Plot * topPT_failDR_passWDR_plot = new Plot("topPT_failDR_passWDR","",topPT1Geter->plotInfo->xTitle,"a.u");
  for(unsigned int iR = 0; bpts[iR] >= 0; ++iR ){
    TString top1_sel = TString::Format("%s && !(top_1_maxDR/2 < 0.8 && top_1_pt > 200) && (top_1_W_DR/2 < 0.8 && top_1_W_pt > 200) && top_1_b_pt > %f",baseSel.Data(),bpts[iR]);
    TH1F * top1_h = topPT1Geter->getHistogram(tree,top1_sel,"","top1");
    TString top2_sel = TString::Format("%s && !(top_2_maxDR/2 < 0.8 && top_2_pt > 200) && (top_2_W_DR/2 < 0.8  && top_2_W_pt > 200) && top_2_b_pt > %f",baseSel.Data(),bpts[iR]);
    TH1F * top2_h = topPT2Geter->getHistogram(tree,top2_sel,"","top2");
    top1_h->Add(top2_h);
    topPT_failDR_passWDR_plot->addHist(top1_h,bptNames[iR],"hist",StyleTools::colorGetter(iR),0,StyleTools::colorGetter(iR));
  }
  topPT_failDR_passWDR_plot->setRatioRange(0.0,1.0);
  c = new TCanvas;
  topPT_failDR_passWDR_plot->drawRatios(c,0,false,"pdf");


  Plot * topPT_failDR_failWDR_plot = new Plot("topPT_failDR_failWDR","",topPT1Geter->plotInfo->xTitle,"a.u");
  if(true){
  TString top1_sel = TString::Format("%s && !(top_1_maxDR/2 < 0.8 && top_1_pt > 200) && !(top_1_W_DR/2 < 0.8 && top_1_W_pt > 200)",baseSel.Data());
  TH1F * top1_h = topPT1Geter->getHistogram(tree,top1_sel,"","top1");
  TString top2_sel = TString::Format("%s && !(top_2_maxDR/2 < 0.8 && top_2_pt > 200) && !(top_2_W_DR/2 < 0.8  && top_2_W_pt > 200)",baseSel.Data());
  TH1F * top2_h = topPT2Geter->getHistogram(tree,top2_sel,"","top2");
  top1_h->Add(top2_h);
  topPT_failDR_failWDR_plot->addHist(top1_h,"inclusive","hist",StyleTools::colorGetter(0),0,StyleTools::colorGetter(0));
  }

  for(unsigned int iR = 0; bpts[iR] >= 0; ++iR ){
    TString top1_sel = TString::Format("%s && !(top_1_maxDR/2 < 0.8 && top_1_pt > 200) && !(top_1_W_DR/2 < 0.8 && top_1_W_pt > 200) && (top_1_minWbDR/2 > 0.4) && (top_1_W_DR/2 > 0.4) &&  min(min(top_1_b_pt,top_1_W_j_1_pt),top_1_W_j_2_pt) > %f",baseSel.Data(),bpts[iR]);
    TH1F * top1_h = topPT1Geter->getHistogram(tree,top1_sel,"","top1");
    TString top2_sel = TString::Format("%s && !(top_2_maxDR/2 < 0.8 && top_2_pt > 200) && !(top_2_W_DR/2 < 0.8  && top_2_W_pt > 200) && (top_2_minWbDR/2 > 0.4) && (top_2_W_DR/2 > 0.4)  && min(min(top_2_b_pt,top_2_W_j_1_pt),top_2_W_j_2_pt) > %f",baseSel.Data(),bpts[iR]);
    TH1F * top2_h = topPT2Geter->getHistogram(tree,top2_sel,"","top2");
    top1_h->Add(top2_h);
    topPT_failDR_failWDR_plot->addHist(top1_h,qptNames[iR],"hist",StyleTools::colorGetter(iR+1),0,StyleTools::colorGetter(iR+1));
  }


  topPT_failDR_failWDR_plot->setRatioRange(0.0,1.0);
  c = new TCanvas;
  topPT_failDR_failWDR_plot->drawRatios(c,0,false,"pdf");


  //Categorization
  enum Cats { INC,TOP, W_B, WnB, RES, nRE, nCT};


  vector<TString> catNames(nCT);
  catNames[INC] ="inclusive";
  catNames[TOP] ="taggable top";
  catNames[W_B] ="untaggable top, taggable W, b p_{T} > 20";
  catNames[WnB] ="untaggable top, taggable W, b p_{T} < 20";
  catNames[RES] ="untaggable top, untaggable W, all quark p_{T} > 20";
  catNames[nRE] ="untaggable top, untaggable W, below thresh. quarks";

  vector<TString> cats(nCT);
  cats[INC] = "(1.0)";
//  cats.push_back("((top_1_maxDR/2 < 0.8 && top_1_pt > 200) || (top_1_W_DR/2 < 0.8 && top_1_W_pt > 200) || (min(min(top_1_b_pt,top_1_W_j_1_pt),top_1_W_j_2_pt) >= 20))");
  cats[TOP] = "(top_1_maxDR/2 < 0.8 && top_1_pt > 200)";
  cats[W_B] = "!(top_1_maxDR/2 < 0.8 && top_1_pt > 200) && (top_1_W_DR/2 < 0.8 && top_1_W_pt > 200) && (top_1_b_pt >= 20)";
  cats[WnB] = "!(top_1_maxDR/2 < 0.8 && top_1_pt > 200) && (top_1_W_DR/2 < 0.8 && top_1_W_pt > 200) && (top_1_b_pt < 20)";
  cats[RES] = "!(top_1_maxDR/2 < 0.8 && top_1_pt > 200) && !(top_1_W_DR/2 < 0.8 && top_1_W_pt > 200) && ((min(min(top_1_b_pt,top_1_W_j_1_pt),top_1_W_j_2_pt) >= 20) && (top_1_minWbDR/2 > 0.4) && (top_1_W_DR/2 > 0.4))";
  cats[nRE] = "!(top_1_maxDR/2 < 0.8 && top_1_pt > 200) && !(top_1_W_DR/2 < 0.8 && top_1_W_pt > 200) &&  !((min(min(top_1_b_pt,top_1_W_j_1_pt),top_1_W_j_2_pt) >= 20) && (top_1_minWbDR/2 > 0.4) && (top_1_W_DR/2 > 0.4))";

  vector<TString> cats2(nCT);
  cats2[INC] = "(1.0)";
//  cats2.push_back("((top_2_maxDR/2 < 0.8 && top_2_pt > 200) || (top_2_W_DR/2 < 0.8 && top_2_W_pt > 200) || (min(min(top_2_b_pt,top_2_W_j_1_pt),top_2_W_j_2_pt) >= 20))");
  cats2[TOP] = "(top_2_maxDR/2 < 0.8 && top_2_pt > 200)";
  cats2[W_B] = "!(top_2_maxDR/2 < 0.8 && top_2_pt > 200) && (top_2_W_DR/2 < 0.8 && top_2_W_pt > 200) && (top_2_b_pt >= 20)";
  cats2[WnB] = "!(top_2_maxDR/2 < 0.8 && top_2_pt > 200) && (top_2_W_DR/2 < 0.8 && top_2_W_pt > 200) && (top_2_b_pt < 20)";
  cats2[RES] = "!(top_2_maxDR/2 < 0.8 && top_2_pt > 200) && !(top_2_W_DR/2 < 0.8 && top_2_W_pt > 200) && ((min(min(top_2_b_pt,top_2_W_j_1_pt),top_2_W_j_2_pt) >= 20) && (top_2_minWbDR/2 > 0.4) && (top_2_W_DR/2 > 0.4))";
  cats2[nRE] = "!(top_2_maxDR/2 < 0.8 && top_2_pt > 200) && !(top_2_W_DR/2 < 0.8 && top_2_W_pt > 200) &&  !((min(min(top_2_b_pt,top_2_W_j_1_pt),top_2_W_j_2_pt) >= 20) && (top_2_minWbDR/2 > 0.4) && (top_2_W_DR/2 > 0.4))";

  Plot * catPlot = new Plot("cat","",topPT1Geter->plotInfo->xTitle,"a.u");
  for(unsigned int iR = 0; iR < cats.size(); ++iR ){
    TString top1_sel = TString::Format("%s && %s",baseSel.Data(),cats[iR].Data());
    TH1F * top1_h = topPT1Geter->getHistogram(tree,top1_sel,"","top1");
    TString top2_sel = TString::Format("%s && %s",baseSel.Data(),cats2[iR].Data());
    TH1F * top2_h = topPT2Geter->getHistogram(tree,top2_sel,"","top2");
    top1_h->Add(top2_h);
    catPlot->addHist(top1_h,catNames[iR],"hist",StyleTools::colorGetter(iR),0,StyleTools::colorGetter(iR));
  }
  catPlot->setRatioRange(0.0,1.0);
  c = new TCanvas;
  catPlot->drawRatios(c,0,false,"pdf");


 enum DiCats { DIN, T_T, T_W, T_R, T_N, W_W, W_R, W_N, R_R, R_N, N_N, nDC};
 vector<TString> diCats(nDC);
 diCats[DIN] = "(1.0)";
 diCats[T_T] = TString::Format("(%s && %s)",cats[TOP].Data(),cats2[TOP].Data());
 diCats[T_W] = TString::Format("((%s && ((%s)||(%s))) || (((%s)||(%s)) && %s)  )",cats[TOP].Data(),cats2[W_B].Data(),cats2[WnB].Data(),cats[W_B].Data(),cats[WnB].Data(),cats2[TOP].Data());
 diCats[T_R] = TString::Format("((%s && %s) || (%s && %s))",cats[TOP].Data(),cats2[RES].Data(),cats[RES].Data(),cats2[TOP].Data());
 diCats[T_N] = TString::Format("((%s && %s) || (%s && %s))",cats[TOP].Data(),cats2[nRE].Data(),cats[nRE].Data(),cats2[TOP].Data());
 diCats[W_W] = TString::Format("(((%s)||(%s)) && ((%s)||(%s)))",cats[W_B].Data(),cats[WnB].Data(), cats2[W_B].Data(),cats2[WnB].Data());
 diCats[W_R] = TString::Format("((%s && ((%s)||(%s))) || (((%s)||(%s)) && %s)  )",cats[RES].Data(),cats2[W_B].Data(),cats2[WnB].Data(),cats[W_B].Data(),cats[WnB].Data(),cats2[RES].Data());
 diCats[W_N] = TString::Format("((%s && ((%s)||(%s))) || (((%s)||(%s)) && %s)  )",cats[nRE].Data(),cats2[W_B].Data(),cats2[WnB].Data(),cats[W_B].Data(),cats[WnB].Data(),cats2[nRE].Data());
 diCats[R_R] = TString::Format("(%s && %s)",cats[RES].Data(),cats2[RES].Data());
 diCats[R_N]= TString::Format("((%s && %s) || (%s && %s))",cats[RES].Data(),cats2[nRE].Data(),cats[nRE].Data(),cats2[RES].Data());
 diCats[N_N] = TString::Format("(%s && %s)",cats[nRE].Data(),cats2[nRE].Data());

 TH1 * totCat = new TH1F("totCat",";category",11,-0.5,10.5);
 for(unsigned int iR = 0; iR < diCats.size(); ++iR ){
   TString di_sel = TString::Format("%s && %s",baseSel.Data(),diCats[iR].Data());
   float entries = tree->GetEntries(di_sel);
   totCat->SetBinContent(iR+1,entries);
 }
 totCat->Scale(1./totCat->GetBinContent(1));
 c = new TCanvas;
 totCat->Draw();

}

void makeCatMaps() {
  TTree * tree = getTree("T2tt_complete_tree.root");
  TString baseSel = "(top_1_had && top_2_had && nlbjets >= 2 && nmbjets >= 1 && njets >= 5 && (mass1 % 50 == 0) && (mass2 % 50 == 0 || mass2 == 1) )";


  //Categorization
  enum Cats { INC,TOP, W_B, WnB, RES, nRE, nCT};


  vector<TString> catNames(nCT);
  catNames[INC] ="inclusive";
  catNames[TOP] ="taggable top";
  catNames[W_B] ="untaggable top, taggable W, b p_{T} > 20";
  catNames[WnB] ="untaggable top, taggable W, b p_{T} < 20";
  catNames[RES] ="untaggable top, untaggable W, all quark p_{T} > 20";
  catNames[nRE] ="untaggable top, untaggable W, below thresh. quarks";

  vector<TString> cats(nCT);
  cats[INC] = "(1.0)";
  cats[TOP] = "(top_1_maxDR/2 < 0.8 && top_1_pt > 200)";
  cats[W_B] = "!(top_1_maxDR/2 < 0.8 && top_1_pt > 200) && (top_1_W_DR/2 < 0.8 && top_1_W_pt > 200) && (top_1_b_pt >= 20)";
  cats[WnB] = "!(top_1_maxDR/2 < 0.8 && top_1_pt > 200) && (top_1_W_DR/2 < 0.8 && top_1_W_pt > 200) && (top_1_b_pt < 20)";
  cats[RES] = "!(top_1_maxDR/2 < 0.8 && top_1_pt > 200) && !(top_1_W_DR/2 < 0.8 && top_1_W_pt > 200) && ((min(min(top_1_b_pt,top_1_W_j_1_pt),top_1_W_j_2_pt) >= 20) && (top_1_minWbDR/2 > 0.4) && (top_1_W_DR/2 > 0.4))";
  cats[nRE] = "!(top_1_maxDR/2 < 0.8 && top_1_pt > 200) && !(top_1_W_DR/2 < 0.8 && top_1_W_pt > 200) &&  !((min(min(top_1_b_pt,top_1_W_j_1_pt),top_1_W_j_2_pt) >= 20) && (top_1_minWbDR/2 > 0.4) && (top_1_W_DR/2 > 0.4))";

  vector<TString> cats2(nCT);
  cats2[INC] = "(1.0)";
  cats2[TOP] = "(top_2_maxDR/2 < 0.8 && top_2_pt > 200)";
  cats2[W_B] = "!(top_2_maxDR/2 < 0.8 && top_2_pt > 200) && (top_2_W_DR/2 < 0.8 && top_2_W_pt > 200) && (top_2_b_pt >= 20)";
  cats2[WnB] = "!(top_2_maxDR/2 < 0.8 && top_2_pt > 200) && (top_2_W_DR/2 < 0.8 && top_2_W_pt > 200) && (top_2_b_pt < 20)";
  cats2[RES] = "!(top_2_maxDR/2 < 0.8 && top_2_pt > 200) && !(top_2_W_DR/2 < 0.8 && top_2_W_pt > 200) && ((min(min(top_2_b_pt,top_2_W_j_1_pt),top_2_W_j_2_pt) >= 20) && (top_2_minWbDR/2 > 0.4) && (top_2_W_DR/2 > 0.4))";
  cats2[nRE] = "!(top_2_maxDR/2 < 0.8 && top_2_pt > 200) && !(top_2_W_DR/2 < 0.8 && top_2_W_pt > 200) &&  !((min(min(top_2_b_pt,top_2_W_j_1_pt),top_2_W_j_2_pt) >= 20) && (top_2_minWbDR/2 > 0.4) && (top_2_W_DR/2 > 0.4))";




 enum DiCats { DIN, T_T, T_W, T_R, T_N, W_W, W_R, W_N, R_R, R_N, N_N, nDC};
 vector<TString> diCats(nDC);
 diCats[DIN] = "(1.0)";
 diCats[T_T] = TString::Format("(%s && %s)",cats[TOP].Data(),cats2[TOP].Data());
 diCats[T_W] = TString::Format("((%s && ((%s)||(%s))) || (((%s)||(%s)) && %s)  )",cats[TOP].Data(),cats2[W_B].Data(),cats2[WnB].Data(),cats[W_B].Data(),cats[WnB].Data(),cats2[TOP].Data());
 diCats[T_R] = TString::Format("((%s && %s) || (%s && %s))",cats[TOP].Data(),cats2[RES].Data(),cats[RES].Data(),cats2[TOP].Data());
 diCats[T_N] = TString::Format("((%s && %s) || (%s && %s))",cats[TOP].Data(),cats2[nRE].Data(),cats[nRE].Data(),cats2[TOP].Data());
 diCats[W_W] = TString::Format("(((%s)||(%s)) && ((%s)||(%s)))",cats[W_B].Data(),cats[WnB].Data(), cats2[W_B].Data(),cats2[WnB].Data());
 diCats[W_R] = TString::Format("((%s && ((%s)||(%s))) || (((%s)||(%s)) && %s)  )",cats[RES].Data(),cats2[W_B].Data(),cats2[WnB].Data(),cats[W_B].Data(),cats[WnB].Data(),cats2[RES].Data());
 diCats[W_N] = TString::Format("((%s && ((%s)||(%s))) || (((%s)||(%s)) && %s)  )",cats[nRE].Data(),cats2[W_B].Data(),cats2[WnB].Data(),cats[W_B].Data(),cats[WnB].Data(),cats2[nRE].Data());
 diCats[R_R] = TString::Format("(%s && %s)",cats[RES].Data(),cats2[RES].Data());
 diCats[R_N] = TString::Format("((%s && %s) || (%s && %s))",cats[RES].Data(),cats2[nRE].Data(),cats[nRE].Data(),cats2[RES].Data());
 diCats[N_N] = TString::Format("(%s && %s)",cats[nRE].Data(),cats2[nRE].Data());

 vector<TString> diCatNames(nDC);
 diCatNames[DIN] = "inclusive";
 diCatNames[T_T] = "Two taggable tops";
 diCatNames[T_W] = "One taggable top and one W";
 diCatNames[T_R] = "One taggable top and one resolved top";
 diCatNames[T_N] = "One taggable top and one lost top";
 diCatNames[W_W] = "Two taggable Ws";
 diCatNames[W_R] = "One taggable W and one resolved top";
 diCatNames[W_N] = "One taggable W and one lost top";
 diCatNames[R_R] = "Two resolved tops";
 diCatNames[R_N] = "One resolved top and one lost top";
 diCatNames[N_N] = "Two lost tops";


 auto getHist = [&] (unsigned int iR) -> TH2D* {
   TString di_sel = TString::Format("%s && %s",baseSel.Data(),diCats[iR].Data());
   auto h = getHistogram(tree,TString::Format("sigMap_%i",iR),di_sel,"weight","mass1",25,-25,1225,"mass2",14,-25,675);
   h->SetTitle(diCatNames[iR]);
   h->SetXTitle("stop mass [GeV]");
   h->SetYTitle("LSP mass [GeV]");
   return h;

 };

 TH1 * totCat = new TH1F("totCat",";category",11,-0.5,10.5);


 TH2D * den = getHist(DIN);
 TCanvas * c = new TCanvas();
 den -> Draw("COLZ");

 for(unsigned int iR = 1; iR < diCats.size(); ++iR ){
   TH2D * num = getHist(iR);
   num->Divide(den);
   c = new TCanvas();
   num->GetZaxis()->SetRangeUser(0.0,0.50);
   num->Draw("COLZ");
 }

}

//process  ;
//sigType  ;
//mass1    ;
//mass2    ;
//mass3    ;
//weight   ;
//
//njets   ;
//ht      ;
//met      ;
//
//top_1_had         ;
//top_1_pt          ;
//top_1_maxDR       ;
//top_1_minWbDR     ;
//top_1_b_pt        ;
//top_1_W_pt        ;
//top_1_W_DR        ;
//top_1_W_j_1_pt    ;
//top_1_W_j_2_pt    ;
//
//top_2_had         ;
//top_2_pt          ;
//top_2_maxDR       ;
//top_2_minWbDR     ;
//top_2_b_pt        ;
//top_2_W_pt        ;
//top_2_W_DR        ;
//top_2_W_j_1_pt    ;
//top_2_W_j_2_pt    ;


#endif

void studyGenProperties(){
//  StyleTools::SetTDRStyle();
//  gStyle->SetTitleOffset(1.400,"Y");
//  gStyle->SetTitleOffset(0.950,"X");
//  gStyle->SetPadTopMargin   (0.08);
  gStyle->SetOptStat(0);
//  plotTopCategories();
  makeCatMaps();
}
