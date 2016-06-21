
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
void EventCorrectionSet::load(TString fileName,TString cttCorrName, int correctionOptions)
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
}

void EventCorrectionSet::processCorrection(const BaseTreeAnalyzer * ana) {

  puWeight = 1;
  truePUWeight = 1;
  normWeight = 1;
  cttWeight = 1;
  if(!ana->isMC()) return;

  if(options_ & PU) {
    puCorr->setAxis(PUCorr::NPV,ana->nPV);
    puWeight = puCorr->get();
  }

  if(options_ & TRUEPU) {
    truePUCorr->setAxis(ana->nPU);
    truePUWeight = truePUCorr->get();
  }

  if(options_ & NORM) {
    if(ana->process == defaults::TTBAR) normWeight = TTBAR_SF;
    if(ana->process == defaults::SINGLE_Z && ana->nBJets == 0) normWeight = Z_0B_SF;
    if(ana->process == defaults::SINGLE_Z && ana->nBJets == 1) normWeight = Z_1B_SF;
    if(ana->process == defaults::SINGLE_Z && ana->nBJets >= 2) normWeight = Z_2B_SF;
  }

  if(options_ & CTT) {
    bool isTTBARLike = false;
    if(ana->process == defaults::TTBAR || ana->process == defaults::SINGLE_T || ana->process == defaults::TTZ || ana->process == defaults::TTW || ana->process == defaults::SIGNAL )
      isTTBARLike = true;
    const cfgSet::ConfigSet& cfg = ana->getAnaCfg();
    cttWeight = cttCorr->process(cfg.corrections.cttEffSFType,cfg.corrections.cttMistagSFType,isTTBARLike,ana->nSelCTTTops == 0 ? -1 : ana->selectedCTTTops.front()->pt());
  }

}///EventCorrectionSet

} /* namespace ucsbsusy */
