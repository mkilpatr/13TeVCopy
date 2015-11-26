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

const double TTBAR_SF = 0.93;
const double Z_0B_SF  = 1.02;
const double Z_1B_SF  = 1.00;
const double Z_2B_SF  = 1.28;

class PUCorr50NS : public RefoldCorrection {
public:
  enum axes {NPV,INPUT_MC};
  PUCorr50NS(TFile * file) : RefoldCorrection("PU50NS",file) {}
};

class PUCorr : public RefoldCorrection {
public:
  enum axes {NPV};
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
static const unsigned int fakeBin     = 4;
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
                          , NORM             = (1 <<  2)   ///< Incl. normalization corrections

  };
 EventCorrectionSet(): puCorr50NS(0), puCorr(0), lepCorr(0), puWeight(1), pu50NSWeight(1), vetoLepWeight(1), selLepWeight(1), normWeight(1) {}

  virtual ~EventCorrectionSet() {};
  virtual void load(TString fileName, int correctionOptions = NULLOPT);
  virtual void processCorrection(const BaseTreeAnalyzer * ana);

  //individual accessors
  float getPUWeight() const {return puWeight;}
  float getVetoLepWeight() const {return vetoLepWeight;}
  float getSelLepWeight() const {return selLepWeight;}
  float get50NSPUWeight() const {return pu50NSWeight;}
  float getNormWeight() const {return normWeight;}

private:
  //Correction list
  PUCorr50NS * puCorr50NS;
  PUCorr * puCorr;
  LepCorr * lepCorr;

  //output values
  float puWeight;
  float pu50NSWeight;
  float vetoLepWeight;
  float selLepWeight;
  float normWeight;

};


} /* namespace ucsbsusy */

#endif /* ANALYSISBASE_TREEANALYZER_INTERFACE_CORRECTIONSET_H_ */
