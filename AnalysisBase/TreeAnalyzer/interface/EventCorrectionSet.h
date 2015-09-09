/*
 * CorrectionSet.h
 *
 *  Created on: Aug 4, 2015
 *      Author: nmccoll
 */

#ifndef ANALYSISBASE_TREEANALYZER_INTERFACE_EVENTCORRECTIONSET_H_
#define ANALYSISBASE_TREEANALYZER_INTERFACE_EVENTCORRECTIONSET_H_

#include "AnalysisBase/TreeAnalyzer/interface/CorrectionSet.h"
#include "AnalysisBase/TreeAnalyzer/interface/DefaultProcessing.h"

namespace ucsbsusy {

class PUCorr : public RefoldCorrection {
public:
  enum axes {NPV,INPUT_MC};
  PUCorr(TFile * file) : RefoldCorrection("PU",file) {}
};

class LepCorr : public HistogramCorrection {
public:
  //enum axes {NPV,INPUT_MC};
  LepCorr(TFile * file)  : HistogramCorrection("LEP",file) {}

  const unsigned int muCorrBin   = 61;
  const unsigned int muBinShift  = 0;
  const unsigned int eleCorrBin  = 62;
  const unsigned int eleBinShift = 20;
  const unsigned int tauCorrBin  = 63;
  const unsigned int tauBinShift = 40;

};

class EventCorrectionSet : public CorrectionSet {
public:
  enum  CorrectionOptions {
                            NULLOPT          = 0
                          , PU               = (1 <<  0)   ///< Correct PU
                          , LEP              = (2 <<  0)   ///< Correct LEP EFF.

  };
  EventCorrectionSet(): lepCorr(0),puCorr(0), puWeight(1),lepWeight(1){}
  virtual ~EventCorrectionSet() {};
  virtual void load(TString fileName, int correctionOptions = NULLOPT);
  virtual void processCorrection(const BaseTreeAnalyzer * ana);

  //individual accessors
  float getPUWeight() const {return puWeight;}
  float getLepWeight() const {return lepWeight;}

private:
  //Correction list
  PUCorr * puCorr;
  LepCorr * lepCorr;

  //output values
  float puWeight;
  float lepWeight;
};


} /* namespace ucsbsusy */

#endif /* ANALYSISBASE_TREEANALYZER_INTERFACE_CORRECTIONSET_H_ */
