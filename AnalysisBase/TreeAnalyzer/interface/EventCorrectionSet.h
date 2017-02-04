/*
 * CorrectionSet.h
 *
 *  Created on: Aug 4, 2015
 *      Author: nmccoll
 */

#ifndef ANALYSISBASE_TREEANALYZER_INTERFACE_EVENTCORRECTIONSET_H_
#define ANALYSISBASE_TREEANALYZER_INTERFACE_EVENTCORRECTIONSET_H_

#include "AnalysisBase/TreeAnalyzer/interface/CorrectionSet.h"
#include "AnalysisTools/DataFormats/interface/FatJet.h"
#include "AnalysisTools/ObjectSelection/interface/ResolvedTopMVA.h"

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

class TruePUCorr : public Correction {
  public :
    TruePUCorr(TFile* file) :
      Correction("puWeight"), corr_nominal("puWeight_nominal",file), corr_up("puWeight_up",file), corr_down("puWeight_down",file) {}
    void setTargetBin(float targetBin) { targetBin_ = targetBin; }
    float get(CORRTYPE syst_type) const {
      switch(syst_type){
        case NOMINAL:
          corr_nominal.setAxisNoUnderOver(targetBin_);
          return corr_nominal.get();
        case UP:
          corr_up.setAxisNoUnderOver(targetBin_);
          return corr_up.get();
        case DOWN:
          corr_down.setAxisNoUnderOver(targetBin_);
          return corr_down.get();
        default:
          throw std::invalid_argument("[TruePUCorr:get] Corr type can only be NOMINAL, UP or DOWN!");
      }
    }
  private:
    HistogramCorrection corr_nominal;
    HistogramCorrection corr_up;
    HistogramCorrection corr_down;
    float targetBin_ = 0;
};

class EventCorrectionSet : public CorrectionSet {
public:
  enum  CorrectionOptions {
                            NULLOPT          = 0
                          , PU               = (1 <<  0)   ///< Correct PU
                          , TRUEPU           = (1 <<  1)   ///< Correct PU
                          , NORM             = (1 <<  2)   ///< Incl. normalization corrections
  };
 EventCorrectionSet(): puCorr(0), truePUCorr(0), puWeight(1), normWeight(1) {}

  virtual ~EventCorrectionSet() {};
  virtual void load(TString fileName, int correctionOptions = NULLOPT);
  virtual void processCorrection(const BaseTreeAnalyzer * ana);

  //individual accessors
  float getPUWeight() const {return puWeight;}
  float getTruePUWeight(CORRTYPE syst_type = NOMINAL) const { return truePUCorr->get(syst_type); }
  float getNormWeight() const {return normWeight;}

private:
  //Correction list
  PUCorr * puCorr;
  TruePUCorr * truePUCorr;

  //output values
  float puWeight;
  float normWeight;
};


} /* namespace ucsbsusy */

#endif /* ANALYSISBASE_TREEANALYZER_INTERFACE_CORRECTIONSET_H_ */
