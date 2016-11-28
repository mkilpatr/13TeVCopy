
#include "AnalysisBase/TreeAnalyzer/interface/EventCorrectionSet.h"
#include "AnalysisBase/TreeAnalyzer/interface/BaseTreeAnalyzer.h"
#include "AnalysisBase/TreeAnalyzer/interface/DefaultProcessing.h"
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"
#include <TFile.h>
namespace ucsbsusy {

SdMVATopCorr::SdMVATopCorr(TString fileName) : Correction("SdMVATop"), sdMVATopInputFile(0), sdMVATopDataFullSF(0), sdMVATopFullFastSF(0) {
  sdMVATopInputFile = new TFile(fileName,"read");
  if(!sdMVATopInputFile) throw std::invalid_argument("SdMVATopCorr::SdMVATopCorr: file could not be found!");
  sdMVATopDataFullSF = (TH1F*)( sdMVATopInputFile->Get("dummy") );
  sdMVATopFullFastSF = (TH1F*)( sdMVATopInputFile->Get("dummy") );
  if(!sdMVATopDataFullSF) throw std::invalid_argument("SdMVATopCorr::SdMVATopCorr: data/fullsim eff SF histograms could not be found!");
  if(!sdMVATopFullFastSF) throw std::invalid_argument("SdMVATopCorr::SdMVATopCorr: fullsim/fastsim eff SF histograms could not be found!");
}
SdMVATopCorr::~SdMVATopCorr(){
  sdMVATopInputFile->Close();
  delete sdMVATopInputFile;
}
float SdMVATopCorr::process(CORRTYPE corrType, const std::vector<FatJetF*> &sdMVATops){
  if(corrType == NONE) return 1;
  if(sdMVATops.size() < 1) return 1;
  float maxGoodPT = sdMVATops.at(0)->pt();
  if(maxGoodPT <= 0.)  return 1;

  auto getNoUnderOver = [](float value, const TH1 * hist) -> int {return std::min(std::max(hist->FindFixBin(value),1),hist->GetNbinsX());  };
  TH1F * hdf = sdMVATopDataFullSF;
  TH1F * hff = sdMVATopFullFastSF;
  int binDataFull = getNoUnderOver(maxGoodPT,hdf);
  int binFullFast = getNoUnderOver(maxGoodPT,hff);

  // current strategy (Nov 26, dummy)
  //          for data/fullsim corrections:
  //             for W tag, XXX.
  //             for top tag, XXX.
  //           for full/fastsim correction:
  //             for top tag, XXX.
  //             for w tag, XXX.
  //             for both, XXX.

  float sf_df     = hdf->GetBinContent(binDataFull);
  float sf_df_unc = TMath::Abs(1 - sf_df)*.5;
  float sf_ff     = hff->GetBinContent(binFullFast);
  float sf_ff_unc = TMath::Abs(1 - sf_ff)*.5;

  float sf    = sf_ff*sf_df;
  float sfunc = sf*sqrt( pow(sf_df_unc/sf_df,2) + pow(sf_ff_unc/sf_ff,2) );

  switch(corrType){
    case UP:      return  sf + sfunc;
    case DOWN:    return  sf - sfunc;
    case NOMINAL: return  sf;
    default: throw std::invalid_argument("SdMVATopCorr::process: Invalid argument!");
  }
}

SdMVAWCorr::SdMVAWCorr(TString fileName) : Correction("SdMVAW"), sdMVAWInputFile(0), sdMVAWDataFullSF(0), sdMVAWFullFastSF(0) {
  sdMVAWInputFile = new TFile(fileName,"read");
  if(!sdMVAWInputFile) throw std::invalid_argument("SdMVAWCorr::SdMVAWCorr: file could not be found!");
  sdMVAWDataFullSF = (TH1F*)( sdMVAWInputFile->Get("dummy") );
  sdMVAWFullFastSF = (TH1F*)( sdMVAWInputFile->Get("dummy") );
  if(!sdMVAWDataFullSF) throw std::invalid_argument("SdMVAWCorr::SdMVAWCorr: data/fullsim eff SF histograms could not be found!");
  if(!sdMVAWFullFastSF) throw std::invalid_argument("SdMVAWCorr::SdMVAWCorr: fullsim/fastsim eff SF histograms could not be found!");
}
SdMVAWCorr::~SdMVAWCorr(){
  sdMVAWInputFile->Close();
  delete sdMVAWInputFile;
}

float SdMVAWCorr::process(CORRTYPE corrType, const std::vector<FatJetF*> &sdMVAWs){
  if(corrType == NONE) return 1;
  if(sdMVAWs.size() < 1) return 1;
  float maxGoodPT = sdMVAWs.at(0)->pt();

  if(maxGoodPT <= 0.)  return 1;
  auto getNoUnderOver = [](float value, const TH1 * hist) -> int {return std::min(std::max(hist->FindFixBin(value),1),hist->GetNbinsX());  };
  TH1F * hdf = sdMVAWDataFullSF;
  TH1F * hff = sdMVAWFullFastSF;
  int binDataFull = getNoUnderOver(maxGoodPT,hdf);
  int binFullFast = getNoUnderOver(maxGoodPT,hff);

  // current strategy (Nov 26, dummy)
  //          for data/fullsim corrections:
  //             for top tag, XXX.
  //             for W tag, XXX.
  //           for full/fastsim correction:
  //             for top tag, XXX.
  //             for w tag, XXX.
  //             for both, XXX.

  float sf_df     = hdf->GetBinContent(binDataFull);
  float sf_df_unc = TMath::Abs(1 - sf_df)*.5;
  float sf_ff     = hff->GetBinContent(binFullFast);
  float sf_ff_unc = TMath::Abs(1 - sf_ff)*.5;

  float sf    = sf_ff*sf_df;
  float sfunc = sf*sqrt( pow(sf_df_unc/sf_df,2) + pow(sf_ff_unc/sf_ff,2) );

  switch(corrType){
    case UP:      return  sf + sfunc;
    case DOWN:    return  sf - sfunc;
    case NOMINAL: return  sf;
    default: throw std::invalid_argument("SdMVAWCorr::process: Invalid argument!");
  }
}

ResMVATopCorr::ResMVATopCorr(TString fileName) : Correction("ResMVATop"), resMVATopInputFile(0), resMVATopDataFullSF(0), resMVATopFullFastSF(0) {
  resMVATopInputFile = new TFile(fileName,"read");
  if(!resMVATopInputFile) throw std::invalid_argument("ResMVATopCorr::ResMVATopCorr: file could not be found!");
  resMVATopDataFullSF = (TH1F*)( resMVATopInputFile->Get("dummy") );
  resMVATopFullFastSF = (TH1F*)( resMVATopInputFile->Get("dummy") );
  if(!resMVATopDataFullSF) throw std::invalid_argument("ResMVATopCorr::ResMVATopCorr: data/fullsim eff SF histograms could not be found!");
  if(!resMVATopFullFastSF) throw std::invalid_argument("ResMVATopCorr::ResMVATopCorr: fullsim/fastsim eff SF histograms could not be found!");
}
ResMVATopCorr::~ResMVATopCorr(){
  resMVATopInputFile->Close();
  delete resMVATopInputFile;
}
float ResMVATopCorr::process(CORRTYPE corrType, const std::vector<TopCand> &resMVATops){
  if(corrType == NONE) return 1;
  if(resMVATops.size() < 1) return 1;
  float maxGoodPT = resMVATops.at(0).topcand.pt();
  if(maxGoodPT <= 0.)  return 1;

  auto getNoUnderOver = [](float value, const TH1 * hist) -> int {return std::min(std::max(hist->FindFixBin(value),1),hist->GetNbinsX());  };
  TH1F * hdf = resMVATopDataFullSF;
  TH1F * hff = resMVATopFullFastSF;
  int binDataFull = getNoUnderOver(maxGoodPT,hdf);
  int binFullFast = getNoUnderOver(maxGoodPT,hff);

  // current strategy (Nov 26, dummy)
  //          for data/fullsim corrections:
  //             for W tag, XXX.
  //             for top tag, XXX.
  //           for full/fastsim correction:
  //             for top tag, XXX.
  //             for w tag, XXX.
  //             for both, XXX.

  float sf_df     = hdf->GetBinContent(binDataFull);
  float sf_df_unc = TMath::Abs(1 - sf_df)*.5;
  float sf_ff     = hff->GetBinContent(binFullFast);
  float sf_ff_unc = TMath::Abs(1 - sf_ff)*.5;

  float sf    = sf_ff*sf_df;
  float sfunc = sf*sqrt( pow(sf_df_unc/sf_df,2) + pow(sf_ff_unc/sf_ff,2) );

  switch(corrType){
    case UP:      return  sf + sfunc;
    case DOWN:    return  sf - sfunc;
    case NOMINAL: return  sf;
    default: throw std::invalid_argument("ResMVATopCorr::process: Invalid argument!");
  }
}

SdTopCorr::SdTopCorr(TString fileName) : Correction("SdTop"), sdTopinputFile(0), sdTopDataFullSF(0), sdTopFullFastSF(0) {
  sdTopinputFile = new TFile(fileName,"read");
  if(!sdTopinputFile) throw std::invalid_argument("SdTopCorr::SdTopCorr: file could not be found!");
  //sdTopDataFullSF = (TH1F*)(sdTopinputFile->Get("ak8toppasspt_over_ak8candpt_data_tagging__over__ak8toppasspt_over_ak8candpt_mc_unsmeared_tagging") );
  sdTopDataFullSF = (TH1F*)(sdTopinputFile->Get("ak8toppasspt_over_ak8candpt_data___over__ak8toppasspt_over_ak8candpt_mc_unsmeared_") );
  sdTopFullFastSF = (TH1F*)(sdTopinputFile->Get("ak8toppasspt_over_ak8candpt_t2tt-850-100-full_t2tt-850-100__over__ak8toppasspt_over_ak8candpt_t2tt-850-100-fast_t2tt-850-100") );
  if(!(sdTopDataFullSF || sdTopFullFastSF)) throw std::invalid_argument("SdTopCorr::SdTopCorr: eff SF histograms could not be found!");
}
SdTopCorr::~SdTopCorr(){
  sdTopinputFile->Close();
  delete sdTopinputFile;
}
float SdTopCorr::process(CORRTYPE corrType, double maxGoodTopPT){
  if(maxGoodTopPT <= 0.) return 1;
  if(corrType == NONE) return 1;
  auto getNoUnderOver = [](float value, const TH1 * hist) -> int {return std::min(std::max(hist->FindFixBin(value),1),hist->GetNbinsX());  };
  TH1F * hdf = sdTopDataFullSF;
  TH1F * hff = sdTopFullFastSF;
  int binDataFull = getNoUnderOver(maxGoodTopPT,hdf);
  int binFullFast = getNoUnderOver(maxGoodTopPT,hff);

  // strategy: for data/full correction:
  //             for W tag, assign 3% uncertainty below 700 GeV, 10% above 700 GeV. no correction (consistent w/1).
  //             for top tag, correct per-bin and assign 1/2 of the corr as unc
  //           for full/fastsim correction:
  //             for top tag, correct per-bin the (400,450) and (800,inf) bins
  //             for w tag, correct per-bin the (200-250) bin
  //             for both, unc of 1/2 the difference from 1 if bin was corrected, 3% to all other bins which weren't corrected

  float sf_df     = hdf->GetBinContent(binDataFull);
  float sf_df_unc = TMath::Abs(1 - sf_df)*.5;

  float sf_ff     = 1.;
  float sf_ff_unc = 0.;
  if( (maxGoodTopPT > 400 && maxGoodTopPT < 450) || maxGoodTopPT > 800){ 
    sf_ff     = hff->GetBinContent(binFullFast);
    sf_ff_unc = TMath::Abs(1 - sf_ff)*.5;
  }else{
    sf_ff = 1.;
    sf_ff_unc = 0.03;
  }

  float sf    = sf_ff*sf_df;
  float sfunc = sf*sqrt( pow(sf_df_unc/sf_df,2) + pow(sf_ff_unc/sf_ff,2) );

  switch(corrType){
    case UP:      return  sf + sfunc;
    case DOWN:    return  sf - sfunc;
    case NOMINAL: return  sf;
    default: throw std::invalid_argument("SdTopCorr::process: Invalid argument!");
  }
}

SdWCorr::SdWCorr(TString fileName) : Correction("SdW"), sdWinputFile(0), sdWDataFullSF(0), sdWFullFastSF(0) {
  sdWinputFile = new TFile(fileName,"read");
  if(!sdWinputFile) throw std::invalid_argument("SdWCorr::SdWCorr: file could not be found!");
  //sdWDataFullSF = (TH1F*)(sdWinputFile->Get("ak8wpasspt_over_ak8candpt_data_tagging__over__ak8wpasspt_over_ak8candpt_mc_unsmeared_tagging") );
  sdWDataFullSF = (TH1F*)(sdWinputFile->Get("ak8wpasspt_over_ak8candpt_data___over__ak8wpasspt_over_ak8candpt_mc_unsmeared_") );
  sdWFullFastSF = (TH1F*)(sdWinputFile->Get("ak8wpasspt_over_ak8candpt_t2tt-850-100-full_t2tt-850-100__over__ak8wpasspt_over_ak8candpt_t2tt-850-100-fast_t2tt-850-100") );
  if(!(sdWDataFullSF || sdWFullFastSF)) throw std::invalid_argument("SdWCorr::SdWCorr: eff SF histograms could not be found!");
}
SdWCorr::~SdWCorr(){
  sdWinputFile->Close();
  delete sdWinputFile;
}
float SdWCorr::process(CORRTYPE corrType, double maxGoodWPT){
  if(maxGoodWPT <= 0.) return 1;
  if(corrType == NONE) return 1;
  auto getNoUnderOver = [](float value, const TH1 * hist) -> int {return std::min(std::max(hist->FindFixBin(value),1),hist->GetNbinsX());  };
  TH1F * hdf = sdWDataFullSF;
  TH1F * hff = sdWFullFastSF;
  int binDataFull = getNoUnderOver(maxGoodWPT,hdf);
  int binFullFast = getNoUnderOver(maxGoodWPT,hff);

  // strategy: for data/full correction:
  //             for W tag, assign 3% uncertainty below 700 GeV, 10% above 700 GeV. no correction (consistent w/1).
  //             for top tag, correct per-bin and assign 1/2 of the corr as unc
  //           for full/fastsim correction:
  //             for top tag, correct per-bin the (400,450) and (800,inf) bins
  //             for w tag, correct per-bin the (200-250) bin
  //             for both, unc of 1/2 the difference from 1 if bin was corrected, 3% to all other bins which weren't corrected

  float sf_df     = hdf->GetBinContent(binDataFull);
  float sf_df_unc = TMath::Abs(1 - sf_df)*.5;
  sf_df     = 1.; // overrride
  sf_df_unc = (maxGoodWPT < 700) ? 0.03 : 0.10;

  float sf_ff     = 1.;
  float sf_ff_unc = 0.;
  if( (maxGoodWPT > 200 && maxGoodWPT < 250) ){
    sf_ff     = hff->GetBinContent(binFullFast);
    sf_ff_unc = TMath::Abs(1 - sf_ff)*.5;
  }else{
    sf_ff = 1.;
    sf_ff_unc = 0.03;
  }

  float sf    = sf_ff*sf_df;
  float sfunc = sf*sqrt( pow(sf_df_unc/sf_df,2) + pow(sf_ff_unc/sf_ff,2) );

  switch(corrType){
    case UP:      return  sf + sfunc;
    case DOWN:    return  sf - sfunc;
    case NOMINAL: return  sf;
    default: throw std::invalid_argument("SdWCorr::process: Invalid argument!");
  }
}

void EventCorrectionSet::load(TString fileName, TString sdMVACorrName, TString resMVATopCorrName, TString sdCorrName, int correctionOptions)
{

  if(correctionOptions & PU) {
    loadFile("PU",fileName,correctionOptions);
    if(options_ & PU) {
      puCorr = new PUCorr(file);
      corrections.push_back(puCorr);
    }
  }
  if(correctionOptions & TRUEPU) {
    loadFile("puWeight",fileName,correctionOptions);
    if(options_ & TRUEPU) {
      truePUCorr = new TruePUCorr(file);
      corrections.push_back(truePUCorr);
    }
  }
  if(correctionOptions & SDMVATOP) {
      sdMVATopCorr = new SdMVATopCorr(sdMVACorrName);
      corrections.push_back(sdMVATopCorr);
  }
  if(correctionOptions & SDMVAW) {
      sdMVAWCorr = new SdMVAWCorr(sdMVACorrName);
      corrections.push_back(sdMVAWCorr);
  }
  if(correctionOptions & RESMVATOP) {
      resMVATopCorr = new ResMVATopCorr(resMVATopCorrName);
      corrections.push_back(resMVATopCorr);
  }
  if(correctionOptions & SDTOP) {
      sdTopCorr = new SdTopCorr(sdCorrName);
      corrections.push_back(sdTopCorr);
  }
  if(correctionOptions & SDW) {
      sdWCorr = new SdWCorr(sdCorrName);
      corrections.push_back(sdWCorr);
  }
}

void EventCorrectionSet::processCorrection(const BaseTreeAnalyzer * ana) {


  puWeight = 1;
  truePUWeight = 1;
  normWeight = 1;
  sdMVATopWeight = 1;
  sdMVAWWeight = 1;
  resMVATopWeight = 1;
  sdTopWeight = 1;
  sdWWeight = 1;
  if(!ana->isMC()) return;

  if(options_ & PU) {
    puCorr->setAxis(PUCorr::NPV,ana->nPV);
    puWeight = puCorr->get();
  }

  if(options_ & TRUEPU) {
    truePUCorr->setAxis(std::min(ana->nPU,float(37.0)));
    truePUWeight = truePUCorr->get();
  }

  if(options_ & NORM) {
    if(ana->process == defaults::TTBAR) normWeight = TTBAR_SF;
    if(ana->process == defaults::SINGLE_Z && ana->nBJets == 0) normWeight = Z_0B_SF;
    if(ana->process == defaults::SINGLE_Z && ana->nBJets == 1) normWeight = Z_1B_SF;
    if(ana->process == defaults::SINGLE_Z && ana->nBJets >= 2) normWeight = Z_2B_SF;
  }

  defaults::SignalType type = ana->evtInfoReader.signalType;
  //bool isFastSim = false;
  //if(ana->process == defaults::SIGNAL){ isFastSim=true; }

  if(options_ & SDMVATOP) {
    const cfgSet::ConfigSet& cfg = ana->getAnaCfg();
    sdMVATopWeight = sdMVATopCorr->process(cfg.corrections.sdMVATopCorrType, ana->sdMVATopTight);
  }

  if(options_ & SDMVAW) {
    const cfgSet::ConfigSet& cfg = ana->getAnaCfg();
    sdMVAWWeight = sdMVAWCorr->process(cfg.corrections.sdMVAWCorrType, ana->sdMVAWTight);
  }

  if(options_ & RESMVATOP) {
    const cfgSet::ConfigSet& cfg = ana->getAnaCfg();
    resMVATopWeight = resMVATopCorr->process(cfg.corrections.resMVATopCorrType, ana->resMVATopMedium);
  }

  if(options_ & SDTOP) {
    if(ana->process == defaults::SIGNAL && type == defaults::T2tt){
      const cfgSet::ConfigSet& cfg = ana->getAnaCfg();
      sdTopWeight = sdTopCorr->process(cfg.corrections.sdTopCorrType,ana->nSelSdTops == 0 ? -1 : ana->selectedSdTops.front()->pt());
    }else{
      sdTopWeight = 1.0;
    }
  }

  if(options_ & SDW) {
    if(ana->process == defaults::SIGNAL && (type == defaults::T2tt || type == defaults::T2bW)){
      const cfgSet::ConfigSet& cfg = ana->getAnaCfg();
      sdWWeight = sdWCorr->process(cfg.corrections.sdWCorrType,ana->nSelSdWs == 0 ? -1 : ana->selectedSdWs.front()->pt());
    }else{
      sdWWeight = 1.0;
    }
  }

}///EventCorrectionSet

} /* namespace ucsbsusy */
