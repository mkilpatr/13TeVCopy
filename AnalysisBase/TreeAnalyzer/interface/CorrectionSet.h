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
