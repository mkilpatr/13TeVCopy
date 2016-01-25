/*
 * CorrectionSet.h
 *
 *  Created on: Aug 4, 2015
 *      Author: nmccoll
 */

#ifndef ANALYSISBASE_TREEANALYZER_INTERFACE_CORRECTIONSET_H_
#define ANALYSISBASE_TREEANALYZER_INTERFACE_CORRECTIONSET_H_

#include "AnalysisTools/QuickRefold/interface/Refold.h"
#include "AnalysisTools/TreeReader/interface/Defaults.h"

class TFile;

namespace ucsbsusy {
class BaseTreeAnalyzer;

enum CORRTYPE {NOMINAL, UP, DOWN, NONE};
TString corrTypeName (CORRTYPE type);

//General class that holds a single correction
class Correction {
public:
  Correction(TString corrName);
  virtual ~Correction() {}
protected:
  const TString name;
};

class RefoldCorrection : public Correction {
public:
  RefoldCorrection(TString corrName, TFile * file);
  virtual float get() const { return corr->getValue(); }
  void setAxis(unsigned int a, float value) const {corr->setBin(a,value);}
protected:
  const QuickRefold::Refold * corr;
};

class HistogramCorrection : public Correction {
public:
  HistogramCorrection(TString corrName, TFile * file);
  HistogramCorrection(TString corrName, TString fileName);
  ~HistogramCorrection();
  void setTargetBin(unsigned int a) {targetBin = a;}
  void setAxis(float value) const { targetBin = corrHist->FindFixBin(value); }
  void setAxisNoUnderOver(float value) const { targetBin = std::min(std::max(corrHist->FindFixBin(value),1),corrHist->GetNbinsX());  }
  virtual float get() const { return corrHist->GetBinContent(targetBin);}
  virtual float getError() const { return corrHist->GetBinError(targetBin);}
  TH1F* getHist()        { return corrHist; }
protected:
  mutable unsigned int targetBin;
  TFile * inputFile;
  TH1F* corrHist;

};
class Histogram2DCorrection : public Correction {
public:
  Histogram2DCorrection(TString corrName, TFile * file);
  Histogram2DCorrection(TString corrName, TString fileName);
  ~Histogram2DCorrection();
  void setTargetBinX(unsigned int a) {targetBinX = a;}
  void setTargetBinY(unsigned int a) {targetBinY = a;}
  void setXAxis(float value) const { targetBinX = corrHist->GetXaxis()->FindFixBin(value); }
  void setYAxis(float value) const { targetBinY = corrHist->GetYaxis()->FindFixBin(value); }
  void setXAxisNoUnderOver(float value) const { targetBinX = std::min(std::max(corrHist->GetXaxis()->FindFixBin(value),1),corrHist->GetNbinsX());  }
  void setYAxisNoUnderOver(float value) const { targetBinY = std::min(std::max(corrHist->GetYaxis()->FindFixBin(value),1),corrHist->GetNbinsY());  }
  virtual float get() const { return corrHist->GetBinContent(targetBinX,targetBinY);}
  virtual float getError() const { return corrHist->GetBinError(targetBinX,targetBinY);}
  TH2F* getHist()        { return corrHist; }
protected:
  mutable unsigned int targetBinX;
  mutable unsigned int targetBinY;
  TFile * inputFile;
  TH2F* corrHist;

};

class CorrectionSet {
public:
  CorrectionSet();
  virtual ~CorrectionSet();
  virtual void processCorrection(const BaseTreeAnalyzer * ana) = 0;
protected:
  virtual void loadSimple(TString correctionSetName, int correctionOptions);
  virtual void loadFile(TString correctionSetName, TString fileName, int correctionOptions);

  TString name;
  TFile * file;
  int options_;
  std::vector<Correction *> corrections;
};

} /* namespace ucsbsusy */

#endif /* ANALYSISBASE_TREEANALYZER_INTERFACE_CORRECTIONSET_H_ */
