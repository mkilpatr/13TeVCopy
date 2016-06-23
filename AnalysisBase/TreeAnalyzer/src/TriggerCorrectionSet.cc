#include "AnalysisBase/TreeAnalyzer/interface/TriggerCorrectionSet.h"
#include "AnalysisBase/TreeAnalyzer/interface/BaseTreeAnalyzer.h"

namespace ucsbsusy {

double PhotonTriggerCorrection::get(CORRTYPE corrType, double pt, double eta) {
  if (corrType==NONE) return 1;

  GraphAsymmErrorsCorrectionHelper *corr = std::abs(eta)<ETA_CUT ? &corrEB : &corrEE;
  corr->findBin(pt);
  switch(corrType){
  case NOMINAL:
    return corr->get();
    break;
  case UP:
    return corr->get() + corr->getErrorHigh();
    break;
  case DOWN:
    return corr->get() + corr->getErrorLow();
    break;
  case NONE:
    return 1;
    break;
  }
  throw std::invalid_argument("[PhotonTriggerCorrection::get] Invalid correction type!");
}




void TriggerCorrectionSet::load(TString fileName, int correctionOptions) {
  if(correctionOptions & PHOTON) {
    loadFile("TrigPhoton",fileName,correctionOptions);
    trigPhoCorr = new PhotonTriggerCorrection("TrigPhoton", file);
    corrections.push_back(trigPhoCorr);
  }

}

void TriggerCorrectionSet::processCorrection(const BaseTreeAnalyzer* ana) {
  trigPhoWeight = 1;
  if(!ana->isMC()) return;

  if(options_ & PHOTON) {
    if (ana->selectedPhotons.size()>0){
      auto pho = ana->selectedPhotons.front();
      trigPhoWeight = trigPhoCorr->get(NOMINAL, pho->pt(), pho->eta());
    }
  }

}

} /* namespace ucsbsusy */
