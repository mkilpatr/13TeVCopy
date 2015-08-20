/*
 * CorrectionSet.h
 *
 *  Created on: Aug 4, 2015
 *      Author: nmccoll
 */

#ifndef ANALYSISBASE_TREEANALYZER_INTERFACE_EVENTCORRECTIONSET_H_
#define ANALYSISBASE_TREEANALYZER_INTERFACE_EVENTCORRECTIONSET_H_

#include "AnalysisBase/TreeAnalyzer/interface/CorrectionSet.h"
namespace ucsbsusy {

class PUCorr : public RefoldCorrection {
public:
  enum axes {NPV,INPUT_MC};
  PUCorr(TFile * file) : RefoldCorrection("PU",file) {}
};

class EventCorrectionSet : public CorrectionSet {
public:
  enum  CorrectionOptions {
                            NULLOPT         = 0
                          , PU              = (1 <<  0)   ///< Correct PU
  };
  EventCorrectionSet(): puCorr(0), npv(0), inputIs25NS(false) {}
  virtual ~EventCorrectionSet() {};
  virtual void load(TString fileName,bool is25NSMC = false, int correctionOptions = NULLOPT);
  virtual void processVariables(const BaseTreeAnalyzer * ana);
  virtual void setVariables();
  bool correctProcess(defaults::Process proc) const;


private:
  //Correction list
  PUCorr * puCorr;

  //stored variables
  float npv;
  bool inputIs25NS;
};


} /* namespace ucsbsusy */

#endif /* ANALYSISBASE_TREEANALYZER_INTERFACE_CORRECTIONSET_H_ */
