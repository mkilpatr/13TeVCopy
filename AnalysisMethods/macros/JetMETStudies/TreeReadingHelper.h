#ifndef TREEREADINGHELPER_H
#define TREEREADINGHELPER_H

#include "TDirectory.h"
#include "TTree.h"
#include "TFile.h"
#include "TTreeFormula.h"
#include "AnalysisMethods/PlotUtils/interface/Plot.hh"
#include "AnalysisMethods/PlotUtils/interface/PlotTools.hh"


struct PlotInfo{
  double  minX  ;
  double  maxX  ;
  int     nBins ;
  TString var   ;
  TString xTitle;
  TString name  ;
  mutable unsigned int nH;
  unsigned int getN() const {return ++nH;}
  PlotInfo(TString name,TString var,TString xTitle,int nBins,double minX,double maxX):
    minX  (minX  ),
    maxX  (maxX  ),
    nBins (nBins ),
    var   (var   ),
    xTitle(xTitle),
    name  (name  ),
    nH(0)
  {}
    ;
};

TH1F * getHistogram(TTree * tree, TString var, TString histName, TString histSample, int nB, double* bins, TString selection, TString * extraSel = 0, TString * weight =0, unsigned int iH = 0){
  TString name = TString::Format("%s_%s_%i",histName.Data(), histSample.Data(),iH);
  TString sel = selection;
  if(extraSel)
    sel += *extraSel;
  if(weight)
    sel = TString::Format("%s*(%s)",weight->Data(),sel.Data());

  TH1F * h = new TH1F(name,var.Data(),nB,bins);
  h->Sumw2();
  tree->Draw(TString::Format("%s>>+%s",var.Data(),name.Data()),sel,"goff");
  if(h) h = (TH1F*)h->Clone();
  PlotTools::toUnderflow(h);
  PlotTools::toOverflow(h);

  return h;
}

template<typename T> T* getHistogram(T* hist, TTree * tree, TString var, TString histName, TString histSample, int nB, double* bins, TString selection, TString * extraSel = 0, TString * weight =0, unsigned int iH = 0){
  TString name = TString::Format("%s_%s_%i",histName.Data(), histSample.Data(),iH);
  TString sel = selection;
  if(extraSel)
    sel += *extraSel;
  if(weight)
    sel = TString::Format("%s*(%s)",weight->Data(),sel.Data());

  hist = new T(name,var.Data(),nB,bins);
  hist->Sumw2();
  tree->Draw(TString::Format("%s>>+%s",var.Data(),name.Data()),sel,"goff");
  if(hist) hist = (T*)hist->Clone();
  PlotTools::toUnderflow(hist);
  PlotTools::toOverflow(hist);
  return hist;
}

TH1F * getHistogram(TTree * tree, TString var, TString histName, TString histSample, int nB, double minX, double maxX, TString selection, TString * extraSel = 0, TString * weight =0, unsigned int iH = 0){
  TString name = TString::Format("%s_%s_%i",histName.Data(), histSample.Data(),iH);
  TString sel = selection;
  if(extraSel)
    sel += *extraSel;
  if(weight)
    sel = TString::Format("%s*(%s)",weight->Data(),sel.Data());

  TH1F * h = new TH1F(name,var.Data(),nB,minX,maxX);
  h->Sumw2();
  tree->Draw(TString::Format("%s>>+%s",var.Data(),name.Data()),sel,"goff");
  if(h) h = (TH1F*)h->Clone();
  PlotTools::toUnderflow(h);
  PlotTools::toOverflow(h);
  return h;
}

template<typename T> T* getHistogram(T* hist, TTree * tree, TString var, TString histName, TString histSample, int nB, double minX, double maxX, TString selection, TString * extraSel = 0, TString * weight =0, unsigned int iH = 0){
  TString name = TString::Format("%s_%s_%i",histName.Data(), histSample.Data(),iH);
  TString sel = selection;
  if(extraSel)
    sel += *extraSel;
  if(weight)
    sel = TString::Format("%s*(%s)",weight->Data(),sel.Data());

  hist = new T(name,var.Data(),nB,minX,maxX);
  hist->Sumw2();
  tree->Draw(TString::Format("%s>>+%s",var.Data(),name.Data()),sel,"goff");
  if(hist) hist = (T*)hist->Clone();
  PlotTools::toUnderflow(hist);
  PlotTools::toOverflow(hist);
  return hist;
}

TH1F * getHistogram(TTree * tree,const PlotInfo& info,TString sample,TString selection,TString * extraSel = 0, TString * weight =0){
  return getHistogram(tree,info.var, info.name,sample,info.nBins,info.minX,info.maxX,selection,extraSel,weight,info.getN());
}

template<typename T> T* getHistogram(T* hist, TTree * tree,const PlotInfo& info,TString sample,TString selection,TString * extraSel = 0, TString * weight =0){
  return getHistogram(hist, tree,info.var, info.name,sample,info.nBins,info.minX,info.maxX,selection,extraSel,weight,info.getN());
}

TProfile * getProfile(TTree * tree,TString histName, TString histSample,TString xVar, TString yVar, int nBinsx, double* binsX, TString selection, TString * extraSel = 0, TString *weight = 0, unsigned int iH = 0){
  TString name = TString::Format("%s_%s_%i",histName.Data(), histSample.Data(),iH);
  TString sel = selection;
  if(extraSel)
    sel += *extraSel;
  if(weight)
    sel = TString::Format("%s*(%s)",weight->Data(),sel.Data());
  TProfile * h = new TProfile(name,xVar.Data(),nBinsx,binsX,"s");
  h->Sumw2();
  tree->Draw(TString::Format("%s:%s>>+%s",yVar.Data(),xVar.Data(),name.Data()),sel,"goffprofs");
  if(h) h = (TProfile*)h->Clone();
  return h;
}
TProfile * getProfile(TTree * tree,TString xVar, TString yVar, TString histName, TString histSample, int nBinsx, double xMin, double xMax, TString selection, TString * extraSel = 0, TString *weight = 0, unsigned int iH = 0){
  TString name = TString::Format("%s_%s_%i",histName.Data(), histSample.Data(),iH);
  TString sel = selection;
  if(extraSel)
    sel += *extraSel;
  if(weight)
    sel = TString::Format("%s*(%s)",weight->Data(),sel.Data());
  TProfile * h = new TProfile(name,xVar.Data(),nBinsx,xMin,xMax,"s");
  h->Sumw2();
  tree->Draw(TString::Format("%s:%s>>+%s",yVar.Data(),xVar.Data(),name.Data()),sel,"goffprofs");
  if(h) h = (TProfile*)h->Clone();
  return h;
}
TProfile * getProfile(TTree * tree,const PlotInfo& xInfo, TString yVar, TString sample,TString selection,TString * extraSel = 0, TString * weight =0){
  return getProfile(tree,xInfo.var, yVar, xInfo.name,sample,xInfo.nBins,xInfo.minX,xInfo.maxX,selection,extraSel,weight,xInfo.getN());
}

TH1F * getErrorHisto(TProfile * prof, PlotInfo& xInfo, TString histSample, bool fillErrors  = true){
  TString name = TString::Format("prof_%s_%s_%i",xInfo.name.Data(), histSample.Data(),xInfo.nH);
  TH1F * h = new TH1F(name,xInfo.var.Data(),xInfo.nBins,xInfo.minX,xInfo.maxX);
  for(int iB = 0; iB <= xInfo.nBins; ++iB){
    h->SetBinContent(iB,fillErrors ? prof->GetBinError(iB) : prof->GetBinContent(iB));
    if(prof->GetBinEffectiveEntries(iB))h->SetBinError(iB,fillErrors ? prof->GetBinError(iB)/TMath::Sqrt(2*prof->GetBinEffectiveEntries(iB)) : prof->GetBinError(iB)/TMath::Sqrt(prof->GetBinEffectiveEntries(iB)));
  }
  return h;
}


#endif
