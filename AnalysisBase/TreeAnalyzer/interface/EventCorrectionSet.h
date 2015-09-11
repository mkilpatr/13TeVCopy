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

class LepCorr : public HistogramCorrection {
public:
  //enum axes {NPV,INPUT_MC};
  LepCorr(TFile * file)  : HistogramCorrection("LEP",file) {}
  void setCorrType(unsigned int a) {corrType = a;}
static const unsigned int muCorrBin   = 1;
static const unsigned int eleCorrBin  = 2;
static const unsigned int tauCorrBin  = 3;
unsigned int corrType;
enum LepCorrOptions
{
    VARY_UP, // assigned 0
    VARY_DOWN, // assigned 1
    VARY_NONE, // assigned 2
};
};

class EventCorrectionSet : public CorrectionSet {
public:
  enum  CorrectionOptions {
                            NULLOPT          = 0
                          , PU               = (1 <<  0)   ///< Correct PU
                          , LEP              = (1 <<  1)   ///< Correct LEP EFF.

  };
  EventCorrectionSet(): puCorr(0), lepCorr(0), puWeight(1),vetoLepWeight(1),selLepWeight(1){}
  virtual ~EventCorrectionSet() {};
  virtual void load(TString fileName, int correctionOptions = NULLOPT);
  virtual void processCorrection(const BaseTreeAnalyzer * ana);

  //individual accessors
  float getPUWeight() const {return puWeight;}
  float getVetoLepWeight() const {return vetoLepWeight;}
  float getSelLepWeight() const {return selLepWeight;}


private:
  //Correction list
  PUCorr * puCorr;
  LepCorr * lepCorr;

  //output values
  float puWeight;
  float vetoLepWeight;
  float selLepWeight;
};


} /* namespace ucsbsusy */

#endif /* ANALYSISBASE_TREEANALYZER_INTERFACE_CORRECTIONSET_H_ */
