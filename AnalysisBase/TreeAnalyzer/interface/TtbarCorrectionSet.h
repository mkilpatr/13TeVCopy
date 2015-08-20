/*
 * CorrectionSet.h
 *
 *  Created on: Aug 4, 2015
 *      Author: nmccoll
 */

#ifndef ANALYSISBASE_TREEANALYZER_INTERFACE_TTBARCORRECTIONSET_H_
#define ANALYSISBASE_TREEANALYZER_INTERFACE_TTBARCORRECTIONSET_H_

#include "AnalysisBase/TreeAnalyzer/interface/CorrectionSet.h"
namespace ucsbsusy {

class TOPPAIRPTCorr : public RefoldCorrection {
public:
  enum axes {TopPairPT};
  TOPPAIRPTCorr(TFile * file) : RefoldCorrection("TOPPAIRPT",file) {}
};

class TtbarCorrectionSet : public CorrectionSet {
public:
  enum  CorrectionOptions {
                            NULLOPT         = 0
                          , TOPPAIRPT       = (1 <<  0)   ///< Correct ZPT
  };
  TtbarCorrectionSet(): topPTCorr(0), topPT(0) {}
  virtual ~TtbarCorrectionSet() {};
  virtual void load(TString fileName, int correctionOptions = NULLOPT);
  virtual void processVariables(const BaseTreeAnalyzer * ana);
  virtual void setVariables();
  bool correctProcess(defaults::Process proc) const;


private:
  //Correction list
  TOPPAIRPTCorr * topPTCorr;

  //stored variables
  float topPT;
};


} /* namespace ucsbsusy */

#endif /* ANALYSISBASE_TREEANALYZER_INTERFACE_CORRECTIONSET_H_ */
