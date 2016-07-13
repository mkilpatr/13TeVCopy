#include "AnalysisBase/TreeAnalyzer/interface/TriggerCorrectionSet.h"
#include "AnalysisBase/TreeAnalyzer/interface/BaseTreeAnalyzer.h"
#include "TMath.h"

namespace ucsbsusy {

double PhotonTriggerCorrection::get(CORRTYPE corrType, double pt, double eta) {
  if (corrType==NONE) return 1;

  GraphAsymmErrorsCorrectionHelper *corr = std::abs(eta)<ETA_EB ? &corrEB : &corrEE;
  corr->findBin(pt);
  switch(corrType){
  case NOMINAL:
    return corr->get();
    break;
  case UP:
    return corr->get() + corr->getErrorHigh();
    break;
  case DOWN:
    return corr->get() - corr->getErrorLow();
    break;
  case NONE:
    return 1;
    break;
  }
  throw std::invalid_argument("[PhotonTriggerCorrection::get] Invalid correction type!");
}

double ElectronTriggerCorrection::get(CORRTYPE corrType, double pt, double eta) {
  if (corrType==NONE) return 1;

  unsigned idx = TMath::BinarySearch(eta_range.size(), eta_range.data(), std::abs(eta));
  if (idx >= corrs.size()) std::range_error("[ElectronTriggerCorrection::get] Invalid eta value!");
  GraphAsymmErrorsCorrectionHelper &corr = corrs.at(idx);

  corr.findBin(pt);
  switch(corrType){
  case NOMINAL:
    return corr.get();
    break;
  case UP:
    return corr.get() + corr.getErrorHigh();
    break;
  case DOWN:
    return corr.get() - corr.getErrorLow();
    break;
  case NONE:
    return 1;
    break;
  }
  throw std::invalid_argument("[ElectronTriggerCorrection::get] Invalid correction type!");
}

double MuonTriggerCorrection::get(CORRTYPE corrType, double pt, double eta) {
  if (corrType==NONE) return 1;

  unsigned idx = TMath::BinarySearch(eta_range.size(), eta_range.data(), std::abs(eta));
  if (idx >= corrs.size()) std::range_error("[MuonTriggerCorrection::get] Invalid eta value!");
  GraphAsymmErrorsCorrectionHelper &corr = corrs.at(idx);

  corr.findBin(pt);
  switch(corrType){
  case NOMINAL:
    return corr.get();
    break;
  case UP:
    return corr.get() + corr.getErrorHigh();
    break;
  case DOWN:
    return corr.get() - corr.getErrorLow();
    break;
  case NONE:
    return 1;
    break;
  }
  throw std::invalid_argument("[MuonTriggerCorrection::get] Invalid correction type!");
}

double MetORLepTriggerCorrection::get(CORRTYPE corrType, int leppdgid, double leppt, double lepeta, double met) {
  if (corrType==NONE) return 1;

  unsigned idx = leppdgid == 13 ? TMath::BinarySearch(mu_eta_range.size(), mu_eta_range.data(), std::abs(lepeta)) : TMath::BinarySearch(el_eta_range.size(), el_eta_range.data(), std::abs(lepeta));

  if(leppdgid == 13 && idx >= corrsMetOrMu.size()) idx = corrsMetOrMu.size() - 1;
  else if(leppdgid == 11 && idx >= corrsMetOrEl.size()) idx = corrsMetOrEl.size() - 1;

  EfficiencyCorrectionHelper &corr = leppdgid == 13 ? corrsMetOrMu.at(idx) : corrsMetOrEl.at(idx);

  corr.findBin(leppt, met)
;
  switch(corrType){
  case NOMINAL:
    return corr.get();
    break;
  case UP:
    return corr.get() + corr.getErrorHigh();
    break;
  case DOWN:
    return corr.get() - corr.getErrorLow();
    break;
  case NONE:
    return 1;
    break;
  }
  throw std::invalid_argument("[MetORLepTriggerCorrection::get] Invalid correction type!");
}


void TriggerCorrectionSet::load(TString fileName, int correctionOptions) {
  if(correctionOptions & PHOTON) {
    loadFile("TrigPhoton",fileName,correctionOptions);
    trigPhoCorr = new PhotonTriggerCorrection("TrigPhoton", file);
    corrections.push_back(trigPhoCorr);
  }
  if(correctionOptions & ELECTRON) {
    loadFile("TrigElectron",fileName,correctionOptions);
    trigEleCorr = new ElectronTriggerCorrection("TrigElectron", file);
    corrections.push_back(trigEleCorr);
  }
  if(correctionOptions & MUON) {
    loadFile("TrigMuon",fileName,correctionOptions);
    trigMuCorr = new MuonTriggerCorrection("TrigMuon", file);
    corrections.push_back(trigMuCorr);
  }
  if(correctionOptions & MET_OR_LEP) {
    loadFile("TrigMetOrLep",fileName,correctionOptions);
    trigMetOrLepCorr = new MetORLepTriggerCorrection("TrigMetOrLep", file);
    corrections.push_back(trigMetOrLepCorr);
  }

}

void TriggerCorrectionSet::processCorrection(const BaseTreeAnalyzer* ana) {
  trigPhoWeight = 1;
  trigEleWeight = 1;
  trigMuWeight  = 1;
  trigMetOrMuWeight = 1;
  trigMetOrElWeight = 1;
  if(!ana->isMC()) return;

  const auto &cfg = ana->getAnaCfg().corrections;
  if(options_ & PHOTON) {
    if (ana->selectedPhotons.size()>0){
      auto pho = ana->selectedPhotons.front();
      trigPhoWeight = trigPhoCorr->get(cfg.trigPhotonCorrType, pho->pt(), pho->eta());
    }
  }
  if(options_ & ELECTRON) {
    auto lep = ana->selectedLepton;
    if (lep && lep->iselectron()){
      trigEleWeight = trigEleCorr->get(cfg.trigElectronCorrType, lep->pt(), lep->eta());
    }
  }
  if(options_ & MUON) {
    auto lep = ana->selectedLepton;
    if (lep && lep->ismuon()){
      trigMuWeight = trigMuCorr->get(cfg.trigMuonCorrType, lep->pt(), lep->eta());
    }
  }
  if(options_ & MET_OR_LEP) {
    auto lep = ana->selectedLepton;
    if (lep && lep->ismuon()){
      trigMetOrMuWeight = trigMetOrLepCorr->get(cfg.trigMetOrLepCorrType, lep->pdgid(), lep->pt(), lep->eta(), ana->met->pt());
    }
    else if (lep && lep->iselectron()){
      trigMetOrElWeight = trigMetOrLepCorr->get(cfg.trigMetOrLepCorrType, lep->pdgid(), lep->pt(), lep->eta(), ana->met->pt());
    }
  }

}

} /* namespace ucsbsusy */

