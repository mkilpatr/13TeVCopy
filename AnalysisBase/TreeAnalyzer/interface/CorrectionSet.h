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

//General class that holds a single correction
class Correction {
public:
  Correction(TString corrName);
  virtual ~Correction() {}
  virtual float get() const = 0;
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
  HistogramCorrection(TString corrName, TFile * file,);
  void setTargetBin(unsigned int a) {targetBin = a;}
  virtual float get() const { return corrHist->GetBinContent(targetBin);}
  const TH1F* getHist()  { return corrHist;}
protected:
const TH1F* corrHist;
unsigned int targetBin;
};

class CorrectionSet {
public:
  CorrectionSet();
  virtual void loadFile(TString correctionSetName, TString fileName, int correctionOptions);
  virtual ~CorrectionSet();
  virtual void processCorrection(const BaseTreeAnalyzer * ana) = 0;
protected:
  TString name;
  TFile * file;
  int options_;
  std::vector<Correction *> corrections;
};

} /* namespace ucsbsusy */

#endif /* ANALYSISBASE_TREEANALYZER_INTERFACE_CORRECTIONSET_H_ */
