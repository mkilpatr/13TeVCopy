/* CorrectionSet.h
 *
 *  Created on: Aug 4, 2015
 *      Author: nmccoll
 */

#ifndef ANALYSISBASE_TREEANALYZER_INTERFACE_TOPWCORRECTIONSET_H_
#define ANALYSISBASE_TREEANALYZER_INTERFACE_TOPWCORRECTIONSET_H_

#include "AnalysisBase/TreeAnalyzer/interface/CorrectionSet.h"
#include "AnalysisTools/DataFormats/interface/FatJet.h"
#include "AnalysisTools/ObjectSelection/interface/ResolvedTopMVA.h"
#include "AnalysisTools/TreeReader/interface/Defaults.h"

namespace ucsbsusy {

class SdMVACorr : public Correction {
  public:
    SdMVACorr();
    ~SdMVACorr();
    float process(int correctionOptions, const std::vector<FatJetF*> &fatjets);

    //TString fileName         = TString::Format("%s/src/data/corrections/2017/sdtopw/topw_sf_eff_20161201.root",defaults::CMSSW_BASE.c_str());
    TString fileName         = TString::Format("%s/src/data/corrections/2017/sdtopw/topw-sf-normcor-20161214-plus-20161201.root",defaults::CMSSW_BASE.c_str()); // updated Dec 14
    TString fileNameSysts    = TString::Format("%s/src/data/corrections/2017/sdtopw/topw-sys-20161213.root",defaults::CMSSW_BASE.c_str());
    TString fileNameFullFast = TString::Format("%s/src/data/corrections/dummy.root",defaults::CMSSW_BASE.c_str());

    TFile * sdMVAInputFile;
    TFile * sdMVAFullFastInputFile;
    TFile * sdMVASystsFile;

    // data/fullsim scale factors
    TH1F  * sdMVA_DataFull_toptagSF;    // tag
    TH1F  * sdMVA_DataFull_wtagSF;
    TH1F  * sdMVA_DataFull_topmistagSF; // mistag
    TH1F  * sdMVA_DataFull_wmistagSF;

    // fullsim MC efficiencies
    TH1F  * sdMVA_Full_toptagEff;       // tag
    TH1F  * sdMVA_Full_wtagEff;
    TH1F  * sdMVA_Full_topmistagEff;    // mistag
    TH1F  * sdMVA_Full_wmistagEff;

    // systematics
    TH1F  * sdMVA_Full_systs_t_ps;      // parton showering (herwig/pythia)
    TH1F  * sdMVA_Full_systs_w_ps;
    TH1F  * sdMVA_Full_systs_t_gen;     // generator (powheg/mg)
    TH1F  * sdMVA_Full_systs_w_gen;
    TH1F  * sdMVA_Full_systs_t_mis_u;   // scaling mistag SF for the subtraction of unmatched MC in the SF calculation
    TH1F  * sdMVA_Full_systs_t_mis_d;   // up, down, ...
    TH1F  * sdMVA_Full_systs_w_mis_u;
    TH1F  * sdMVA_Full_systs_w_mis_d;

    // fullsim/fastsim scale factors
    TH1F  * sdMVAFullFastSF;            // dummy
};

class ResMVATopCorr : public Correction {
  public:
    ResMVATopCorr();
    ~ResMVATopCorr();
    float process(int correctionOptions, const std::vector<TopCand> &resMVATops, const std::vector<PartonMatching::TopDecay*>& hadronicGenTops);

    //TString fileName         = TString::Format("%s/src/data/corrections/2017/restop/restop_sf_20161201.root",defaults::CMSSW_BASE.c_str());
    TString fileName         = TString::Format("%s/src/data/corrections/2017/restop/restop-sf-normcor-20161214-plus-20161201.root",defaults::CMSSW_BASE.c_str()); // updated Dec 14
    TString fileNameSysts    = TString::Format("%s/src/data/corrections/2017/sdtopw/topw-sys-20161213.root",defaults::CMSSW_BASE.c_str()); //NOT A BUG - systs are stored in same file as merged
    TString fileNameFullFast = TString::Format("%s/src/data/corrections/dummy.root",defaults::CMSSW_BASE.c_str());

    TFile * resMVATopInputFile;
    TFile * resMVAFullFastInputFile;
    TFile * resMVASystsFile;

    // data/fullsim scale factors
    TH1F  * resTop_DataFull_toptagSF;    // tag
    TH1F  * resTop_DataFull_topmistagSF; // mistag

    // fullsim MC efficiencies
    TH1F  * resTop_Full_toptagEff;       // tag
    TH1F  * resTop_Full_topmistagEff;    // mistag

    // systematics
    TH1F  * resTop_Full_systs;           // top and w systematics MC effs
    TH1F  * resTop_Full_systs_ps;        // parton showering (herwig/pythia)
    TH1F  * resTop_Full_systs_gen;       // generator (powheg/mg)
    TH1F  * resTop_Full_systs_mis_u;     // scaling mistag SF for the subtraction of unmatched MC in the SF calculation
    TH1F  * resTop_Full_systs_mis_d;     // up, down, ...

    // fullsim/fastsim scale factors
    TH1F  * resMVATopFullFastSF;

};

///// ICHEP16 OUTDATED /////
class SdTopCorr : public Correction {
  public:
  SdTopCorr(TString fileName);
  ~SdTopCorr();
  float process(CORRTYPE corrType, double maxGoodTopPT);

  TFile * sdTopinputFile;
  TH1F * sdTopDataFullSF;
  TH1F * sdTopFullFastSF;
};
///// ICHEP16 OUTDATED /////
class SdWCorr : public Correction {
  public:
  SdWCorr(TString fileName);
  ~SdWCorr();
  float process(CORRTYPE corrType, double maxGoodWPT);

  TFile * sdWinputFile;
  TH1F * sdWDataFullSF;
  TH1F * sdWFullFastSF;
};

class TopWCorrectionSet : public CorrectionSet {
public:

  // options for calculating weights (eg SDMVA) and for changing those weights with systematics (eg SYSTS_MERGED_PS)
  // for ICHEP16 weights, the W/Top UP/DOWN variations are still accessed via the corrType in src/DefaultConfigurations.cc
  // just make the bitshifts in increasing order
  enum CorrectionOptions { // 63 bits available in long long (+ 1 sgn bit?)
                          NULLOPT                    = 0
                        , SDMVA                      = (1 <<  0)   ///< SD MVA top and W tagging
                        , RESMVATOP                  = (1 <<  1)   ///< RES MVA top tagging
                        , ICHEP16SDTOP               = (1 <<  2)   ///< cut-based SD top tagging ///// ICHEP16 OUTDATED /////
                        , ICHEP16SDW                 = (1 <<  3)   ///< cut-based SD w tagging ///// ICHEP16 OUTDATED /////
                        , SYSTS_MERGED_STATS_UP      = (1 <<  4)   // systs - merged - stats - up
                        , SYSTS_RESOLVED_STATS_UP    = (1 <<  5)   // systs - resolved - stats - up
                        , SYSTS_MERGED_PS            = (1 <<  8)   // systs - merged - parton showering
                        , SYSTS_MERGED_GEN           = (1 <<  9)   // systs - merged - generator
                        , SYSTS_MERGED_MISTAG_W_UP   = (1 << 10)   // systs - merged - w mistag SF - up
                        , SYSTS_MERGED_MISTAG_W_DOWN = (1 << 11)   // systs - merged - w mistag SF - down
                        , SYSTS_MERGED_MISTAG_T_UP   = (1 << 12)   // systs - merged - t mistag SF - up
                        , SYSTS_MERGED_MISTAG_T_DOWN = (1 << 13)   // systs - merged - t mistag SF - down 
                        , SYSTS_RESOLVED_PS          = (1 << 14)   // systs - RESOLVED - parton showering
                        , SYSTS_RESOLVED_GEN         = (1 << 15)   // systs - RESOLVED - generator
                        , SYSTS_RESOLVED_MISTAG_UP   = (1 << 16)   // systs - RESOLVED - mistag SF - up
                        , SYSTS_RESOLVED_MISTAG_DOWN = (1 << 17)   // systs - RESOLVED - mistag SF - down
  };

  TopWCorrectionSet(): sdMVACorr(0), resMVATopCorr(0), sdTopCorr(0), sdWCorr(0), sdMVAWeight(1), resMVATopWeight(1), sdTopWeight(1), sdWWeight(1){}

  virtual ~TopWCorrectionSet() {};
  virtual void load(TString sdCorrName, int correctionOptions = NULLOPT);
  virtual void processCorrection(const BaseTreeAnalyzer * ana);

  //individual accessors
  float getAnySdMVAWeight(int correctionOptions, const std::vector<FatJetF*> &fatjets) const { return sdMVACorr->process(correctionOptions,fatjets); }
  float getAnyResMVATopWeight(int correctionOptions, const std::vector<TopCand> &resMVATops, const std::vector<PartonMatching::TopDecay*>& hadronicGenTops) const { return resMVATopCorr->process(correctionOptions,resMVATops, hadronicGenTops); }
  float getSdMVAWeight() const {return sdMVAWeight;}
  float getResMVATopWeight() const {return resMVATopWeight;}
  float getSdTopWeight() const {return sdTopWeight;} ///// ICHEP16 OUTDATED /////
  float getSdWWeight() const {return sdWWeight;} ///// ICHEP16 OUTDATED /////

private:
  //Correction list
  SdMVACorr * sdMVACorr;
  ResMVATopCorr * resMVATopCorr;
  SdTopCorr * sdTopCorr; ///// ICHEP16 OUTDATED /////
  SdWCorr * sdWCorr;

  //output values
  float sdMVAWeight;
  float resMVATopWeight;
  float sdTopWeight; ///// ICHEP16 OUTDATED /////
  float sdWWeight;
};

} /* namespace ucsbsusy */

#endif /* ANALYSISBASE_TREEANALYZER_INTERFACE_CORRECTIONSET_H_ */
