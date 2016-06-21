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

class PUCorr : public RefoldCorrection {
public:
  enum axes {NPV};
  PUCorr(TFile * file) : RefoldCorrection("PU",file) {}
};

class TruePUCorr : public HistogramCorrection {
  public :
    TruePUCorr(TFile* file) : HistogramCorrection("puWeight",file) {}
};

class CTTCorr : public Correction {
  public :
  CTTCorr(TString fileName) ;
  ~CTTCorr();
  float process(CORRTYPE effCorrType, CORRTYPE misCorrType, bool isTopLike, double maxGoodTopPT);

  TFile * inputFile;
  TH1F* effSF;
  TH1F* mistagSF;
};

class EventCorrectionSet : public CorrectionSet {
public:
  enum  CorrectionOptions {
                            NULLOPT          = 0
                          , PU               = (1 <<  0)   ///< Correct PU
                          , TRUEPU           = (1 <<  1)   ///< Correct PU
                          , NORM             = (1 <<  2)   ///< Incl. normalization corrections
                          , CTT              = (1 <<  3)   ///< CTT top tagging

  };
 EventCorrectionSet(): puCorr(0), truePUCorr(0), cttCorr(0), puWeight(1), truePUWeight(1), normWeight(1),cttWeight(1) {}

  virtual ~EventCorrectionSet() {};
  virtual void load(TString fileName, TString cttCorrName, int correctionOptions = NULLOPT);
  virtual void processCorrection(const BaseTreeAnalyzer * ana);

  //individual accessors
  float getPUWeight() const {return puWeight;}
  float getTruePUWeight() const {return truePUWeight;}
  float getNormWeight() const {return normWeight;}
  float getCTTWeight() const {return cttWeight;}

private:
  //Correction list
  PUCorr * puCorr;
  TruePUCorr * truePUCorr;
  CTTCorr * cttCorr;

  //output values
  float puWeight;
  float truePUWeight;
  float normWeight;
  float cttWeight;

};


} /* namespace ucsbsusy */

#endif /* ANALYSISBASE_TREEANALYZER_INTERFACE_CORRECTIONSET_H_ */
