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

class TruePUCorr : public HistogramCorrection {
  public :
    TruePUCorr(TFile* file) : HistogramCorrection("puWeight",file) {}
};

class SdMVACorr : public Correction {
  public:
    SdMVACorr(TString fileName, TString fileNameFullFast);
    ~SdMVACorr();
    float process(CORRTYPE corrType, const std::vector<FatJetF*> &fatjets);

    TFile * sdMVAInputFile;
    TFile * sdMVAFullFastInputFile;

    TH1F  * sdMVA_DataFull_toptagSF;    // top and w tag SFs
    TH1F  * sdMVA_DataFull_wtagSF;
    TH1F  * sdMVA_DataFull_topmistagSF; // top and w mistag SFs
    TH1F  * sdMVA_DataFull_wmistagSF;
    TH1F  * sdMVA_Full_toptagEff;       // top and w tag fullsim MC effs
    TH1F  * sdMVA_Full_wtagEff;
    TH1F  * sdMVA_Full_topmistagEff;    // top and w mistag fullsim MC effs
    TH1F  * sdMVA_Full_wmistagEff;

    TH1F  * sdMVAFullFastSF;            // dummy - fullsim to fastsim SF
};

class ResMVATopCorr : public Correction {
  public:
    ResMVATopCorr(TString fileName);
    ~ResMVATopCorr();
    float process(CORRTYPE corrType, const std::vector<TopCand> &resMVATops);

    TFile * resMVATopInputFile;
    TH1F  * resMVATopDataFullSF;
    TH1F  * resMVATopFullFastSF;
};

class SdTopCorr : public Correction {
  public:
  SdTopCorr(TString fileName);
  ~SdTopCorr();
  float process(CORRTYPE corrType, double maxGoodTopPT);

  TFile * sdTopinputFile;
  TH1F * sdTopDataFullSF;
  TH1F * sdTopFullFastSF;
};
class SdWCorr : public Correction {
  public:
  SdWCorr(TString fileName);
  ~SdWCorr();
  float process(CORRTYPE corrType, double maxGoodWPT);

  TFile * sdWinputFile;
  TH1F * sdWDataFullSF;
  TH1F * sdWFullFastSF;
};

class EventCorrectionSet : public CorrectionSet {
public:
  enum  CorrectionOptions {
                            NULLOPT          = 0
                          , PU               = (1 <<  0)   ///< Correct PU
                          , TRUEPU           = (1 <<  1)   ///< Correct PU
                          , NORM             = (1 <<  2)   ///< Incl. normalization corrections
                          , SDMVA            = (1 <<  3)   ///< SD MVA top and W tagging
                          , RESMVATOP        = (1 <<  4)   ///< RES MVA top tagging
                          , SDTOP            = (1 <<  5)   ///< SD top tagging
                          , SDW              = (1 <<  6)   ///< SD w tagging
  };
 EventCorrectionSet(): puCorr(0), truePUCorr(0), sdMVACorr(0), resMVATopCorr(0), sdTopCorr(0), sdWCorr(0), puWeight(1), truePUWeight(1), normWeight(1), sdMVAWeight(1), resMVATopWeight(1), sdTopWeight(1),sdWWeight(1){}

  virtual ~EventCorrectionSet() {};
  virtual void load(TString fileName, TString sdMVACorrName, TString sdMVACorrNameFullFast, TString resMVACorrName, TString sdCorrName, int correctionOptions = NULLOPT);
  virtual void processCorrection(const BaseTreeAnalyzer * ana);

  //individual accessors
  float getPUWeight() const {return puWeight;}
  float getTruePUWeight() const {return truePUWeight;}
  float getNormWeight() const {return normWeight;}
  float getSdMVAWeight() const {return sdMVAWeight;}
  float getResMVATopWeight() const {return resMVATopWeight;}
  float getSdTopWeight() const {return sdTopWeight;}
  float getSdWWeight() const {return sdWWeight;}

private:
  //Correction list
  PUCorr * puCorr;
  TruePUCorr * truePUCorr;
  SdMVACorr * sdMVACorr;
  ResMVATopCorr * resMVATopCorr;
  SdTopCorr * sdTopCorr;
  SdWCorr * sdWCorr;

  //output values
  float puWeight;
  float truePUWeight;
  float normWeight;
  float sdMVAWeight;
  float resMVATopWeight;
  float sdTopWeight;
  float sdWWeight;

};


} /* namespace ucsbsusy */

#endif /* ANALYSISBASE_TREEANALYZER_INTERFACE_CORRECTIONSET_H_ */
