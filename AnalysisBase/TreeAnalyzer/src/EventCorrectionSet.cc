
#include "AnalysisBase/TreeAnalyzer/interface/EventCorrectionSet.h"
#include "AnalysisBase/TreeAnalyzer/interface/BaseTreeAnalyzer.h"
#include "AnalysisBase/TreeAnalyzer/interface/DefaultProcessing.h"
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"
#include <TFile.h>
namespace ucsbsusy {


CTTCorr::CTTCorr(TString fileName) : Correction("CTT"), inputFile(0), effSF(0), mistagSF(0){
  inputFile = new TFile(fileName,"read");
  if(!inputFile) throw std::invalid_argument("CTTCorr::CTTCorr: file could not be found!");
  effSF = (TH1F*)(inputFile->Get("effSF") );
  if(!effSF) throw std::invalid_argument("CTTCorr::CTTCorr: effSF histogram could not be found!");
  mistagSF = (TH1F*)(inputFile->Get("mistagSF") );
  if(!mistagSF) throw std::invalid_argument("CTTCorr::CTTCorr: mistagSF histogram could not be found!");
}
CTTCorr::~CTTCorr(){
  inputFile->Close();
  delete inputFile;
}
float CTTCorr::process(CORRTYPE effCorrType, CORRTYPE misCorrType, bool isTopLike, double maxGoodTopPT){
  if(maxGoodTopPT <= 0) return 1;
  const auto* h = isTopLike ? effSF :mistagSF;
  const auto  t = isTopLike ? effCorrType : misCorrType;
  if(t == NONE) return 1;
  auto getNoUnderOver = [](float value, const TH1 * hist) -> int {return std::min(std::max(hist->FindFixBin(value),1),hist->GetNbinsX());  };
  int bin = getNoUnderOver(maxGoodTopPT,h);
  switch(t){
    case UP:      return  h->GetBinContent(bin) + h->GetBinError(bin);
    case DOWN:    return  h->GetBinContent(bin) - h->GetBinError(bin);
    case NOMINAL: return  h->GetBinContent(bin);
    default: throw std::invalid_argument("CTTCorr::process: Invalid argument!");
  }
}

SdTopCorr::SdTopCorr(TString fileName) : Correction("SdTop"), sdTopinputFile(0), sdTopDataFullSF(0), sdTopFullFastSF(0) {
  sdTopinputFile = new TFile(fileName,"read");
  if(!sdTopinputFile) throw std::invalid_argument("SdTopCorr::SdTopCorr: file could not be found!");
  sdTopDataFullSF = (TH1F*)(sdTopinputFile->Get("ak8toppasspt_over_ak8candpt_data_tagging__over__ak8toppasspt_over_ak8candpt_mc_unsmeared_tagging") );
  sdTopFullFastSF = (TH1F*)(sdTopinputFile->Get("Tops") );
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
  float sf_df = hdf->GetBinContent(binDataFull);
  float sf_ff = hff->GetBinContent(binFullFast);
  float sf_df_relerror = hdf->GetBinError(binDataFull)/sf_df;
  float sf_ff_relerror = hff->GetBinError(binFullFast)/sf_ff;
  float sf    = sf_df*sf_ff;
  float sfunc = sf_df*sf_ff*sqrt( pow(sf_df_relerror,2) + pow(sf_ff_relerror,2) );
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
  sdWDataFullSF = (TH1F*)(sdWinputFile->Get("ak8wpasspt_over_ak8candpt_data_tagging__over__ak8wpasspt_over_ak8candpt_mc_unsmeared_tagging") );
  sdWFullFastSF = (TH1F*)(sdWinputFile->Get("Ws") );
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
  float sf_df = hdf->GetBinContent(binDataFull);
  float sf_ff = hff->GetBinContent(binFullFast);
  float sf_df_relerror = hdf->GetBinError(binDataFull)/sf_df;
  float sf_ff_relerror = hff->GetBinError(binFullFast)/sf_ff;
  float sf    = sf_df*sf_ff;
  float sfunc = sf_df*sf_ff*sqrt( pow(sf_df_relerror,2) + pow(sf_ff_relerror,2) );
  switch(corrType){
    case UP:      return  sf + sfunc;
    case DOWN:    return  sf - sfunc;
    case NOMINAL: return  sf;
    default: throw std::invalid_argument("SdWCorr::process: Invalid argument!");
  }
}

void EventCorrectionSet::load(TString fileName,TString cttCorrName, TString sdCorrName, int correctionOptions)
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
  if(correctionOptions & CTT) {
      cttCorr = new CTTCorr(cttCorrName);
      corrections.push_back(cttCorr);
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
  cttWeight = 1;
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

  if(options_ & CTT) {
    bool isTTBARLike = false;
    if(ana->process == defaults::TTBAR || ana->process == defaults::SINGLE_T || ana->process == defaults::TTZ || ana->process == defaults::TTW || ana->process == defaults::SIGNAL )
      isTTBARLike = true;
    const cfgSet::ConfigSet& cfg = ana->getAnaCfg();
    cttWeight = cttCorr->process(cfg.corrections.cttEffSFType,cfg.corrections.cttMistagSFType,isTTBARLike,ana->nSelCTTTops == 0 ? -1 : ana->selectedCTTTops.front()->pt());

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
