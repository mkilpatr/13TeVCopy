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
  Correction(TString corrName, TFile * file);
  virtual ~Correction() {}
  virtual float get() const { return corr->getValue(); }
protected:
  const QuickRefold::Refold * corr;
  const TString name;
};


class CorrectionSet {
public:
  CorrectionSet();
  virtual void loadFile(TString correctionSetName, TString fileName, int correctionOptions);
  virtual ~CorrectionSet();

  virtual bool correctProcess(defaults::Process proc) const = 0;
  virtual void processVariables(const BaseTreeAnalyzer * ana) = 0;
  virtual void setVariables() = 0;
  virtual float getTotalCorrection() const;


protected:
  TString name;
  TFile * file;
  int options_;
  std::vector<Correction *> corrections;
};

} /* namespace ucsbsusy */

#endif /* ANALYSISBASE_TREEANALYZER_INTERFACE_CORRECTIONSET_H_ */
