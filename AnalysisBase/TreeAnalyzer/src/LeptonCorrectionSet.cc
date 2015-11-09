#include "AnalysisBase/TreeAnalyzer/interface/LeptonCorrectionSet.h"
#include "AnalysisBase/TreeAnalyzer/interface/BaseTreeAnalyzer.h"
#include "AnalysisBase/TreeAnalyzer/interface/DefaultProcessing.h"
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"

namespace ucsbsusy {

double TnPCorr::getLepWeight(TnPCorr* tnpCorr, LeptonF* lep, CORRTYPE elCorrType, CORRTYPE muCorrType ) const {
  int   id  = lep->pdgid();
  float pt  = lep->pt();
  float eta = lep->absEta();
  float wt = 1.0;
  float er = 0.0;
  if(id==11){
    wt = tnpCorr->getElValue(pt,eta);
    er = tnpCorr->getElError(pt,eta);
  }
  else if(id==13) {
    wt = tnpCorr->getMuValue(pt,eta);
    er = tnpCorr->getMuError(pt,eta);
  }
  std::cout << "  lep flavor: " << id
            << "\tpt: "         << pt
            << "\teta: "        << eta
            << "\twt: "         << wt
            << "\terror: "      << er
            << std::endl; // */
  if     (id==11 && elCorrType == UP  ) wt += er;
  else if(id==11 && elCorrType == DOWN) wt -= er;
  else if(id==13 && muCorrType == UP  ) wt += er;
  else if(id==13 && muCorrType == DOWN) wt -= er;
  return wt;
}

double TnPCorr::getEvtWeight(TnPCorr* tnpCorr, const std::vector<LeptonF*>& leptons, const std::vector<GenParticleF*> genParts, CORRTYPE elCorrType, CORRTYPE muCorrType ) const {
  // store gen leptons for matching
  std::vector<const GenParticleF*> genEl_;
  std::vector<const GenParticleF*> genMu_;
  for(const auto* p : genParts) {
    // check for daughters of Zs ... to be updated when we add J/Psis / Upsilons
    if(ParticleInfo::isA(ParticleInfo::p_Z0, p) || ParticleInfo::isA(ParticleInfo::p_Wplus, p)) {
      for(unsigned int id = 0; id < p->numberOfDaughters(); ++id) {
        const GenParticleF* dau = p->daughter(id);
        if     (ParticleInfo::isA(ParticleInfo::p_eminus , dau)) genEl_.push_back(dau);
        else if(ParticleInfo::isA(ParticleInfo::p_muminus, dau)) genMu_.push_back(dau);
      } // W,Z daughters
    } // genPart is a Z or W
  } // genParts
  double lepWt = 1;
  for(auto * lep : leptons) {
    double nearDR = 0;
    int near = 0;
    if     (lep->pdgid()==11) near = PhysicsUtilities::findNearestDRDeref(*lep, genEl_, nearDR, 0.4, 10.0, 0, genEl_.size());
    else if(lep->pdgid()==13) near = PhysicsUtilities::findNearestDRDeref(*lep, genMu_, nearDR, 0.4, 10.0, 0, genMu_.size());
    if(near > 0) lepWt *= getLepWeight(tnpCorr,lep,elCorrType,muCorrType);
  }
  std::cout << "event weight: " << lepWt << std::endl << std::endl;
  return lepWt;
}

void LeptonCorrectionSet::load(TString fileName, TString tnpElFileName, TString tnpMuFileName, int correctionOptions)
{
  if(correctionOptions & LEP) {
    loadFile("LEP",fileName,correctionOptions);
    if(options_ & LEP) {
      lepCorr = new LepCorr(file);
      corrections.push_back(lepCorr);
      if(options_ & USE_HPSTAUS)
        setUseHPSTaus(true);
    }
  }
  if(correctionOptions & TNP) {
    if(options_ & TNP) {
      tnpCorr = new TnPCorr(tnpElFileName,tnpMuFileName);
      corrections.push_back(tnpCorr);
    }
  }
}

void LeptonCorrectionSet::processCorrection(const BaseTreeAnalyzer * ana) {

  selLepWeight = 1;
  vetoLepWeight = 1;
  tnpEvtWeight = 1;
  if(!ana->isMC()) return;

  if(options_ & LEP) {
    int nGoodGenMu = 0; int nGoodGenEle = 0; int nPromptGenTaus = 0;
    int nSelectedElectrons = 0;  int nSelectedMuons = 0;
    ///COUNT GEN LEPS
    for(auto* p : ana->genParts) {
      const GenParticleF * genPartMom = 0;
      if (p->numberOfMothers()>0) 
        genPartMom = p->mother(0);
      else
        continue;
      // electron from W/Z/tau
      if (ParticleInfo::isA(ParticleInfo::p_eminus, p) && (ParticleInfo::isA(ParticleInfo::p_Z0, genPartMom) || ParticleInfo::isA(ParticleInfo::p_Wplus, genPartMom) || ParticleInfo::isA(ParticleInfo::p_tauminus, genPartMom)))
        nGoodGenEle++;
      // muon from W/Z/tau
      else if (ParticleInfo::isA(ParticleInfo::p_muminus, p) && (ParticleInfo::isA(ParticleInfo::p_Z0, genPartMom) || ParticleInfo::isA(ParticleInfo::p_Wplus, genPartMom) || ParticleInfo::isA(ParticleInfo::p_tauminus, genPartMom)))
        nGoodGenMu++;
      // tau from W/Z
      else if (ParticleInfo::isA(ParticleInfo::p_tauminus, p) && (ParticleInfo::isA(ParticleInfo::p_Z0, genPartMom) || ParticleInfo::isA(ParticleInfo::p_Wplus, genPartMom))) {
        bool lepDecay = false;
        for(unsigned int itd = 0; itd < p->numberOfDaughters(); itd++) {
          const GenParticleF* dau = p->daughter(itd);
          if(ParticleInfo::isA(ParticleInfo::p_eminus, dau) || ParticleInfo::isA(ParticleInfo::p_muminus, dau)) lepDecay = true;
        }
        // hadronic tau
        if(!lepDecay)
          nPromptGenTaus++;
      }
    }

    bool ishighptmu = false, ishighptele = false;
    for(auto* i : ana->vetoedLeptons) {
      if(fabs(i->pdgid()) == 11) {
        nSelectedElectrons++;
        if(i->pt() > 20.0) ishighptele = true;
      }
      if(fabs(i->pdgid()) == 13) {
        nSelectedMuons++;
        if(i->pt() > 20.0) ishighptmu = true;
      }
    }

    vetoLepWeight = 1;
    selLepWeight  = 0;

    // muon selected region
    if (nSelectedMuons >= 1 && nGoodGenMu >= 1) {
      if(ishighptmu) lepCorr->setTargetBin(LepCorr::muCorrBinHighPt);
      else           lepCorr->setTargetBin(LepCorr::muCorrBinLowPt);
    }
    // electron selected region
    else if (nSelectedElectrons >= 1 && nSelectedMuons == 0 && nGoodGenEle >= 1){
      if(ishighptele) lepCorr->setTargetBin(LepCorr::eleCorrBinHighPt);
      else            lepCorr->setTargetBin(LepCorr::eleCorrBinLowPt);
    }
    // tau selected region
    else if (!useHPS && ana->nVetoedTracks >= 1 && nSelectedElectrons == 0 && nSelectedMuons == 0 && nPromptGenTaus >= 1) {
      if(ana->vetoedTracks[0]->pt() > 20.0) lepCorr->setTargetBin(LepCorr::tauCorrBinHighPt);
      else                                  lepCorr->setTargetBin(LepCorr::tauCorrBinLowPt);
    }
    // HPS tau selected region
    else if (useHPS && ana->nVetoHPSTaus >= 1 && nSelectedElectrons == 0 && nSelectedMuons == 0 && nPromptGenTaus >= 1) {
      lepCorr->setTargetBin(LepCorr::hpsTauCorrBin);
    }
    // fake region
    else  if ((nSelectedMuons >= 1) || (nSelectedElectrons >= 1) || (ana->nVetoedTracks >= 1)) {
      lepCorr->setTargetBin(LepCorr::fakeBin);
    }
    // veto region
    else {
      lepCorr->setTargetBin(LepCorr::defaultBin);
    }

    vetoLepWeight = 1 - lepCorr->get();
    selLepWeight  = lepCorr->get();
                 
    if(options_ & LEP_VARY_UP) {
      vetoLepWeight -= lepCorr->getError();
      selLepWeight  += lepCorr->getError();
    }
    else if(options_ & LEP_VARY_DOWN) {
      vetoLepWeight += lepCorr->getError();
      selLepWeight  -= lepCorr->getError();
    }

  }

  if(options_ & TNP) {
    const cfgSet::ConfigSet& cfg = ana->getAnaCfg();
    tnpEvtWeight = tnpCorr->getEvtWeight(tnpCorr,ana->selectedLeptons,ana->genParts,cfg.corrections.tnpElCorrType,cfg.corrections.tnpMuCorrType);
  }

}

} /* namespace ucsbsusy */
