#ifndef TREEREADINGHELPER_H
#define TREEREADINGHELPER_H

#include "TDirectory.h"
#include "TTree.h"
#include "TFile.h"
#include "TTreeFormula.h"
#include "AnalysisMethods/PlotUtils/interface/Plot.hh"
#include "AnalysisMethods/PlotUtils/interface/PlotTools.hh"

int colorGetter(int counter){
  int color = counter+1;
  if(color>= 10) color++;
  return color;
}


class BasicPlotInfo{
public:
  TString var   ;
  TString xTitle;
  TString name  ;
  mutable unsigned int nH;
  unsigned int getN() const {return ++nH;}
  BasicPlotInfo(TString name,TString var,TString xTitle):
    var   (var   ),
    xTitle(xTitle),
    name  (name  ),
    nH(0)
  {};
  virtual ~BasicPlotInfo(){}
  virtual TH1F* getHistogram(TString histName) = 0;
  virtual TH2F* getHistogram2D(TString histName,int nYBins) = 0;
};

class MinMaxPlotInfo : public BasicPlotInfo{
public:
  double  minX  ;
  double  maxX  ;
  int     nBins ;
  MinMaxPlotInfo(TString name,TString var,TString xTitle,int nBins,double minX,double maxX) :
    BasicPlotInfo(name,var,xTitle), nBins(nBins), minX(minX), maxX(maxX){}
  TH1F* getHistogram(TString histName){
   return new TH1F(histName,var.Data(),nBins,minX,maxX);
 }
  TH2F* getHistogram2D(TString histName,int nYBins){
   return new TH2F(histName,var.Data(),nBins,minX,maxX,nYBins, -.5, nYBins -.5);
 }
};

class SetBinsPlotInfo : public BasicPlotInfo{
public:
  double* bins  ;
  int     nBins ;
  SetBinsPlotInfo(TString name,TString var,TString xTitle,int nBins,double* bins) :
    BasicPlotInfo(name,var,xTitle), nBins(nBins), bins(bins){}
   TH1F* getHistogram(TString histName){
    return new TH1F(histName,var.Data(),nBins,bins);
  }
   TH2F* getHistogram2D(TString histName,int nYBins){
    return new TH2F(histName,var.Data(),nBins,bins,nYBins, -.5, nYBins -.5);
  }
};



class HistogramGetter{
public:
  BasicPlotInfo * plotInfo;
  HistogramGetter(BasicPlotInfo * plotInfo) : plotInfo(plotInfo), nBootStraps(0), underOverflow(true) {}
  HistogramGetter(TString name,TString var,TString xTitle,int nBins,double* bins) :
    plotInfo(new SetBinsPlotInfo(name,var,xTitle,nBins, bins)), nBootStraps(0), underOverflow(true) {}
  HistogramGetter(TString name,TString var,TString xTitle,int nBins,double minX,double maxX) :
    plotInfo(new MinMaxPlotInfo(name,var,xTitle,nBins, minX,maxX)), nBootStraps(0), underOverflow(true) {}


  TString selection;
  TString weight;
  TString sampleName;
  int nBootStraps;
  bool underOverflow;

  TH1F * getHistogramManual(TTree* tree,TString histSelection,TString histWeight, TString histSampleName = ""){
    selection = histSelection;
    weight = histWeight;
    sampleName = histSampleName;
    return getHistogramManual(tree);
  }

  TH1F * getHistogram(TTree* tree,TString histSelection,TString histWeight, TString histSampleName = ""){
    selection = histSelection;
    weight = histWeight;
    sampleName = histSampleName;
    return getHistogram(tree);
  }

  TH1F * getHistogram(TTree* tree){
    TString name = getFullName();
    TString sel = getSelString();
    TH1F * h = plotInfo->getHistogram(name);
    h->Sumw2();

    tree->Draw(TString::Format("%s>>+%s",plotInfo->var.Data(),name.Data()),sel,"goff");
//    if(h) h = (TH1F*)h->Clone();

    if(underOverflow){
      PlotTools::toUnderflow(h);
      PlotTools::toOverflow(h);
    }
    if(nBootStraps > 0){
      processBootStrap(tree,h,name,sel);
    }


    return h;
  }


  TH1F*  getHistogramManual(TTree* tree,TString numSelection,TString numWeight, TString denSelection,TString denWeight, TString histSampleName = ""){

    sampleName = histSampleName;
    TTreeFormula *varFormula = new TTreeFormula("form2",plotInfo->var, tree);

    selection = numSelection;
    weight = numWeight;
    TString nameN = getFullName();
    TString selN = getSelString();
    TH1F * hN = plotInfo->getHistogram(nameN);
    hN->Sumw2();

    TTreeFormula *selNFormula = new TTreeFormula("form1",selN, tree);

    selection = denSelection;
    weight = denWeight;
    TString nameD = getFullName();
    TString selD = getSelString();
    TH1F * hD = plotInfo->getHistogram(nameD);
    hD->Sumw2();

    TTreeFormula *selDFormula = new TTreeFormula("form2",selD, tree);


    vector<TTreeFormula *> bootFormulas(nBootStraps);

    TH2F *hbN = 0;
    TH2F *hbD = 0;

    if(nBootStraps){
      TString nameS = plotInfo->name += "_bN";
      hbN  = plotInfo->getHistogram2D(nameS,nBootStraps);
      nameS = plotInfo->name += "_bD";
      hbD  = plotInfo->getHistogram2D(nameS,nBootStraps);

      for(unsigned int iB = 0; iB < nBootStraps; ++iB){
        bootFormulas[iB] = new TTreeFormula(TString::Format("boostForm_%u",iB),TString::Format("bootstrapWeight[%u]",iB), tree);
      }
    }


    int nEntries = tree->GetEntries();
    for (int i= 0;i<nEntries;i++) {
     tree->GetEntry(i);
     double var      = varFormula->EvalInstance();
     double weightN   = selNFormula->EvalInstance();
     double weightD   = selDFormula->EvalInstance();
     if(weightD == 0 && weightN == 0) continue;

     if(weightN) hN->Fill(var,weightN);
     if(weightD) hD->Fill(var,weightD);

     for(unsigned int iB = 0; iB < nBootStraps; ++iB){
       bootFormulas[iB]->GetNdata();
       double bootWeight = bootFormulas[iB]->EvalInstance();
       if(bootWeight && weightN)
         hbN->Fill(var,iB,weightN*bootWeight);
       if(bootWeight && weightD)
         hbD->Fill(var,iB,weightD*bootWeight);
     }
    }

   delete selNFormula;
   delete selDFormula;
   delete varFormula;
   for(unsigned int iB = 0; iB < nBootStraps; ++iB)
     delete bootFormulas[iB];

   if(underOverflow){
     PlotTools::toUnderflow(hN);
     PlotTools::toOverflow(hN);
     PlotTools::toUnderflow(hD);
     PlotTools::toOverflow(hD);
     if(nBootStraps){
       PlotTools::toUnderflowX(hbN);
       PlotTools::toOverflowX(hbN);
       PlotTools::toUnderflowX(hbD);
       PlotTools::toOverflowX(hbD);
     }
   }

   hN->Divide(hD);

   if(nBootStraps){
     hbN->Divide(hbD);
//     new TCanvas();
//     hb->Draw("COLZ");
     for(unsigned int iB = 0; iB <= hbN->GetNbinsX() +1; ++iB){
       double s = 0;
       double ss = 0;

       for(unsigned int iBS = 0; iBS < nBootStraps; ++iBS){
         s += hbN->GetBinContent(iB,iBS+1);
         ss += hbN->GetBinContent(iB,iBS+1)*hbN->GetBinContent(iB,iBS+1);
       }
       double stdDev = TMath::Sqrt(nBootStraps*ss - s*s)/nBootStraps;
       hN->SetBinError(iB,stdDev);
     }
     delete hbN;
   }

   return hN;
  }



  TH1F*  getHistogramManual(TTree * tree){

    TString name = getFullName();
    TString sel = getSelString();
    TH1F * h = plotInfo->getHistogram(name);
    h->Sumw2();

    TTreeFormula *selFormula = new TTreeFormula("form1",sel, tree);
    TTreeFormula *varFormula = new TTreeFormula("form2",plotInfo->var, tree);

    vector<TTreeFormula *> bootFormulas(nBootStraps);

    TH2F *hb = 0;

    if(nBootStraps){
      TString nameS = plotInfo->name += "_b";
      hb  = plotInfo->getHistogram2D(nameS,nBootStraps);

      for(unsigned int iB = 0; iB < nBootStraps; ++iB){
        bootFormulas[iB] = new TTreeFormula(TString::Format("boostForm_%u",iB),TString::Format("bootstrapWeight[%u]",iB), tree);
      }
    }


    int nEntries = tree->GetEntries();
    for (int i= 0;i<nEntries;i++) {
     tree->GetEntry(i);
     double var      = varFormula->EvalInstance();
     double weight   = selFormula->EvalInstance();
     if(weight == 0) continue;

     h->Fill(var,weight);

     for(unsigned int iB = 0; iB < nBootStraps; ++iB){
       bootFormulas[iB]->GetNdata();
       double bootWeight = bootFormulas[iB]->EvalInstance();
       if(bootWeight)
         hb->Fill(var,iB,weight*bootWeight);
     }
    }

   delete selFormula;
   delete varFormula;
   for(unsigned int iB = 0; iB < nBootStraps; ++iB)
     delete bootFormulas[iB];

   if(underOverflow){
     PlotTools::toUnderflow(h);
     PlotTools::toOverflow(h);
     if(nBootStraps){
       PlotTools::toUnderflowX(hb);
       PlotTools::toOverflowX(hb);
     }
   }

   if(nBootStraps){
//     new TCanvas();
//     hb->Draw("COLZ");
     for(unsigned int iB = 0; iB <= h->GetNbinsX() +1; ++iB){
       double s = 0;
       double ss = 0;

       for(unsigned int iBS = 0; iBS < nBootStraps; ++iBS){
         s += hb->GetBinContent(iB,iBS+1);
         ss += hb->GetBinContent(iB,iBS+1)*hb->GetBinContent(iB,iBS+1);
       }
       double stdDev = TMath::Sqrt(nBootStraps*ss - s*s)/nBootStraps;
       h->SetBinError(iB,stdDev);
     }
     delete hb;
   }

   return h;
  }

private:

  void processBootStrap(TTree* tree, TH1* origHist, TString origName, TString origSel){
    TString nameS = origName += "_s";
    TH1F * hs  = plotInfo->getHistogram(nameS);
    TString nameSS = origName += "_ss";
    TH1F * hss = plotInfo->getHistogram(nameSS);
    TString nameT = origName += "_t";
    TH1F * ht = plotInfo->getHistogram(nameT);

    cout << "-------"<<endl;
    for(unsigned int iB = 0; iB <= origHist->GetNbinsX()+1; ++iB){
      cout << origHist->GetBinContent(iB) <<" ";
    } cout << endl;

    for(unsigned int iN = 0; iN < nBootStraps; ++iN){
      TString sel = origSel == "" ? TString::Format("bootstrapWeight[%u]",iN) : TString::Format("%s*bootstrapWeight[%u]",origSel.Data(),iN);
      tree->Draw(TString::Format("%s>>+%s",plotInfo->var.Data(),nameT.Data()),sel,"goff");

      if(underOverflow){
        PlotTools::toUnderflow(ht);
        PlotTools::toOverflow(ht);
      }

      for(unsigned int iB = 0; iB <= ht->GetNbinsX() +1; ++iB){
        hs->SetBinContent(iB, hs->GetBinContent(iB) + ht->GetBinContent(iB) );
        hss->SetBinContent(iB, hss->GetBinContent(iB) + ht->GetBinContent(iB)*ht->GetBinContent(iB) );
      }

      for(unsigned int iB = 0; iB <= origHist->GetNbinsX()+1; ++iB){
        cout << ht->GetBinContent(iB) <<" ";
      } cout << endl;
      for(unsigned int iB = 0; iB <= origHist->GetNbinsX()+1; ++iB){
        cout << hs->GetBinContent(iB) <<" ";
      } cout << endl;
      for(unsigned int iB = 0; iB <= origHist->GetNbinsX()+1; ++iB){
        cout << hss->GetBinContent(iB) <<" ";
      } cout << endl;


      ht->Reset();
    }

    for(unsigned int iB = 0; iB <= origHist->GetNbinsX() +1; ++iB){
      double s = hs->GetBinContent(iB);
      double ss = hss->GetBinContent(iB);
      double stdDev = TMath::Sqrt(nBootStraps*ss - s*s)/nBootStraps;
      origHist->SetBinError(iB,stdDev);
    }

    for(unsigned int iB = 0; iB <= origHist->GetNbinsX()+1; ++iB){
      cout << origHist->GetBinError(iB) <<" ";
    } cout << endl;

  }

  TString getFullName(){
    if(sampleName != "")
      return TString::Format("%s_%s_%i",sampleName.Data(), plotInfo->name.Data(), plotInfo->getN());
    else
      return TString::Format("%s_%i",plotInfo->name.Data(), plotInfo->getN());
  }

  TString getSelString() {
    TString selString("");
    if(weight != ""){
      selString = weight;
      if(selection != "")
        selString += TString::Format("*(%s)",selection.Data());
    } else if (selection != ""){
      selString = selection;
    }

    return selString;

  }


};


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

TH1F * getHistogram(TTree * tree,const PlotInfo& info,TString sample,TString selection,TString * extraSel = 0, TString * weight =0){
  return getHistogram(tree,info.var, info.name,sample,info.nBins,info.minX,info.maxX,selection,extraSel,weight,info.getN());
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
  for(unsigned int iB = 0; iB <= xInfo.nBins; ++iB){
    h->SetBinContent(iB,fillErrors ? prof->GetBinError(iB) : prof->GetBinContent(iB));
    if(prof->GetBinEffectiveEntries(iB))h->SetBinError(iB,fillErrors ? prof->GetBinError(iB)/TMath::Sqrt(2*prof->GetBinEffectiveEntries(iB)) : prof->GetBinError(iB)/TMath::Sqrt(prof->GetBinEffectiveEntries(iB)));
  }
  return h;
}


#endif
