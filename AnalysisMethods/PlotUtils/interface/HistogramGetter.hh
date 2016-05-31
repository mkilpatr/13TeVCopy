//--------------------------------------------------------------------------------------------------
//
//   Get TH1s from TTrees
//   This is for when you need lots of manual control
//
//--------------------------------------------------------------------------------------------------

#ifndef HISTOGRAMGETTER_HH
#define HISTOGRAMGETTER_HH

#include "TDirectory.h"
#include "TTree.h"
#include "TFile.h"
#include "TTreeFormula.h"
#include "AnalysisMethods/PlotUtils/interface/Plot.hh"
#include "AnalysisMethods/PlotUtils/interface/PlotTools.hh"

class HistogramGetter{
//Helper classes
public:


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
  int     nBins ;
  double  minX  ;
  double  maxX  ;
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
  int     nBins ;
  double* bins  ;
  SetBinsPlotInfo(TString name,TString var,TString xTitle,int nBins,const double* inbins) :
    BasicPlotInfo(name,var,xTitle), nBins(nBins){
    bins = new double[nBins+1];
    for(int iB = 0; iB < nBins+1; ++iB) bins[iB] = inbins[iB];
  }
  ~SetBinsPlotInfo() {delete [] bins;}
   TH1F* getHistogram(TString histName){
    return new TH1F(histName,var.Data(),nBins,bins);
  }
   TH2F* getHistogram2D(TString histName,int nYBins){
    return new TH2F(histName,var.Data(),nBins,bins,nYBins, -.5, nYBins -.5);
  }
};

public:
  BasicPlotInfo * plotInfo;
  HistogramGetter(BasicPlotInfo * plotInfo) : plotInfo(plotInfo), nBootStraps(0), underflow(false), overflow(true) {}
  HistogramGetter(TString name,TString var,TString xTitle,int nBins,const double* bins) :
    plotInfo(new SetBinsPlotInfo(name,var,xTitle,nBins, bins)), nBootStraps(0), underflow(false), overflow(true) {}
  HistogramGetter(TString name,TString var,TString xTitle,int nBins,double minX,double maxX) :
    plotInfo(new MinMaxPlotInfo(name,var,xTitle,nBins, minX,maxX)), nBootStraps(0), underflow(false), overflow(true) {}

  //Get a histogram with the loaded plotInfo, but set your own weight and selection string
  TH1F * getHistogram(TTree* tree,TString histSelection,TString histWeight, TString histSampleName = "");

  //Get a histogram with the loaded plotInfo, but set your own weight and selection string
  //But do it inefficienty by iterating over the tree manually....use it for bootstrapping when it is
  //MUCH MORE EFFICIENT
  TH1F * getHistogramManual(TTree* tree,TString histSelection,TString histWeight, TString histSampleName = "");

  //Get a correlated TF: numerator/denominator. Only really usefull if you are doing bootstrapping
  //Will also print out the covariance matrix from the bootstrapping
  TH1F*  getTFAndCov(TTree* tree,TString numSelection,TString numWeight, TString denSelection,TString denWeight, TString histSampleName = "");

  //For when you want to flip between bootstrapping for the same getter
  void setNBS(int newBS) {nBootStraps = newBS;}

  // Set underflow and overflow
  void setUnderOverflow(bool addUnderflow, bool addOverflow) { underflow = addUnderflow; overflow = addOverflow; }

private:
  TH1F * getHistogram(TTree* tree);
  TH1F*  getHistogramManual(TTree * tree);
  TString getFullName();
  TString getSelString();

  TString selection;
  TString weight;
  TString sampleName;
  unsigned int nBootStraps;
  bool underflow;
  bool overflow;

};

#endif
