#include "AnalysisBase/TreeAnalyzer/interface/LeptonCorrectionSet.h"
#include "AnalysisBase/TreeAnalyzer/interface/BaseTreeAnalyzer.h"
#include "AnalysisBase/TreeAnalyzer/interface/DefaultProcessing.h"
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"
#include "TFile.h"

namespace ucsbsusy {

TnPCorr::TnPCorr(TString corrName, LEPSEL lepSel) : Correction(corrName)
{
  std::clog << "=======================================================================================================\n"
            << "=== WARNING: THERE IS A BUNCH OF HARD-CODED FILES AND HISTOGRAMS IN THE TNP LEPTON CORRECTIONS!!!!! ===\n"
            << "===                               USE AT YOUR OWN RISK!!!!!!                                        ===\n"
            << "======================================================================================================="
            << std::endl;
  TString baseDir = TString::Format("%s/src/data/corrections/",getenv ("CMSSW_BASE"));
  fileIdIsoEl     = TFile::Open(baseDir+"kinematicBinSFele.root","read");
  fileMCVetoEffEl = TFile::Open(baseDir+"tnpMCEffEl.root","read");
  fileMCVetoEffMu = TFile::Open(baseDir+"tnpMCEffMu.root","read");
  if(!fileIdIsoEl    ) throw std::invalid_argument("LepHistogramCorrection::LepHistogramCorrection: el ID/Iso file could not be found!");
  if(!fileMCVetoEffEl) throw std::invalid_argument("LepHistogramCorrection::LepHistogramCorrection: el MC Eff file could not be found!");
  if(!fileMCVetoEffMu) throw std::invalid_argument("LepHistogramCorrection::LepHistogramCorrection: mu MC Eff file could not be found!");
  TString elIDHistName, elIsoHistName, muIDHistName, muIsoHistName;

  if(lepSel==MT2VETO) {
    fileIdMu  = TFile::Open(baseDir+"TnP_MuonID_NUM_LooseID_DENOM_generalTracks_VAR_map_pt_eta.root","read");
    fileIsoMu = TFile::Open(baseDir+"TnP_MuonID_NUM_MiniIsoTight_DENOM_LooseID_VAR_map_activity_pt.root","read");
    if(!fileIdMu ) throw std::invalid_argument("LepHistogramCorrection::LepHistogramCorrection: mu ID file could not be found!");
    if(!fileIsoMu) throw std::invalid_argument("LepHistogramCorrection::LepHistogramCorrection: mu Iso file could not be found!");
    elIDHistName  = "CutBasedVeto";
    elIsoHistName = "MiniIso0p1_vs_RelActivity";
    muIDHistName  = "pt_abseta_PLOT_pair_probeMultiplicity_bin0_&_tag_combRelIsoPF04dBeta_bin0_&_tag_pt_bin0_&_tag_IsoMu20_pass";
    muIsoHistName = "SFmap";
  } // MT2Veto
  else if(lepSel==GOODPOG) {
    fileIdMu  = TFile::Open(baseDir+"tnpMuCorr_1L.root","read");
    fileIsoMu = TFile::Open(baseDir+"tnpCorr_unit.root","read");
    if(!fileIdMu)  throw std::invalid_argument("LepHistogramCorrection::LepHistogramCorrection: mu ID file could not be found!");
    if(!fileIsoMu) throw std::invalid_argument("LepHistogramCorrection::LepHistogramCorrection: mu Iso file could not be found!");
    elIDHistName  = "CutBasedMedium";
    elIsoHistName = "MiniIso0p1_vs_RelActivity";
    muIDHistName  = "TNPMU";
    muIsoHistName = "UNIT";
  } // MT2Veto
  else throw std::invalid_argument("LepHistogramCorrection::LepHistogramCorrection: Invalid option for standardCorrections.tnpLepSel! Use 'MT2VETO' or 'GOODPOG'!");

  HistIDEl        = (TH2F*)(fileIdIsoEl    ->Get(elIDHistName ));
  HistIDMu        = (TH2F*)(fileIdMu       ->Get(muIDHistName ));
  HistIsoEl       = (TH2F*)(fileIdIsoEl    ->Get(elIsoHistName));
  HistIsoMu       = (TH2F*)(fileIsoMu      ->Get(muIsoHistName));
  HistMCVetoEffEl = (TH2F*)(fileMCVetoEffEl->Get("tnpEffEl"));
  HistMCVetoEffMu = (TH2F*)(fileMCVetoEffMu->Get("tnpEffMu"));
  if(!HistIDEl       ) throw std::invalid_argument("LepHistogramCorrection::LepHistogramCorrection: el ID hist could not be found!");
  if(!HistIDMu       ) throw std::invalid_argument("LepHistogramCorrection::LepHistogramCorrection: mu ID hist could not be found!");
  if(!HistIsoEl      ) throw std::invalid_argument("LepHistogramCorrection::LepHistogramCorrection: el ISO hist could not be found!");
  if(!HistIsoMu      ) throw std::invalid_argument("LepHistogramCorrection::LepHistogramCorrection: mu ISO hist could not be found!");
  if(!HistMCVetoEffEl) throw std::invalid_argument("LepHistogramCorrection::LepHistogramCorrection: el MC Eff hist could not be found!");
  if(!HistMCVetoEffMu) throw std::invalid_argument("LepHistogramCorrection::LepHistogramCorrection: mu MC Eff hist could not be found!");
}

TnPCorr::~TnPCorr() {
  if(fileIdIsoEl    ) fileIdIsoEl    ->Close();
  if(fileIdMu       ) fileIdMu       ->Close();
  if(fileIsoMu      ) fileIsoMu      ->Close();
  if(fileMCVetoEffEl) fileMCVetoEffEl->Close();
  if(fileMCVetoEffMu) fileMCVetoEffMu->Close();
  delete fileIdIsoEl    ;
  delete fileIdMu       ;
  delete fileIsoMu      ;
  delete fileMCVetoEffEl;
  delete fileMCVetoEffMu;
}

void TnPCorr::getLepWeight(float &wt, float &vetoWt, LeptonF* lep, CORRTYPE elCorrType, CORRTYPE muCorrType ) const {
  int   id  = lep->pdgid();
  float pt  = lep->pt();
  float eta = lep->absEta();
  float annulus = lep->annulusactivity();
  if     (id==11 && elCorrType == NONE) {wt     = 1.0; return; }
  else if(id==13 && muCorrType == NONE) {vetoWt = 1.0; return; }
  float sfid     = 1.0;
  float sfiso    = 1.0;
  float sfuncid  = 0.0;
  float sfunciso = 0.0;
  float eff      = 1.0;
  if(id==11){
    sfid     = getElIDValue(pt,eta);
    sfiso    = getElIsoValue(pt,annulus);
    sfuncid  = getElIDError(pt,eta);
    sfunciso = getElIsoError(pt,annulus);
    eff      = getElMCEffValue(pt,annulus);
  }
  else if(id==13) {
    sfid     = getMuIDValue(pt,eta);
    sfiso    = getMuIsoValue(pt,annulus);
    sfuncid  = sfid *0.01;
    sfunciso = sfiso*0.01;
    eff      = getMuMCEffValue(pt,annulus);
  }
  float sf  = sfid*sfiso;
  float unc = getError(sfuncid,sfunciso);
  if     (id==11 && elCorrType == UP  ) sf += unc;
  else if(id==11 && elCorrType == DOWN) sf -= unc;
  else if(id==13 && muCorrType == UP  ) sf += unc;
  else if(id==13 && muCorrType == DOWN) sf -= unc;
  wt     = sf;
  vetoWt = eff < 1.0 ? (1.0-eff*sf)/(1.0-eff) : (1.0 - 0.99999*sf)/(1.0 - 0.99999); // don't want to divide by zero
  if (vetoWt < -10.0) vetoWt = 1.0;    // or large negative weights ... need to treat these cases better
}

float TnPCorr::getEvtWeight(const std::vector<LeptonF*>& allLeptons, const std::vector<LeptonF*>& selectedLeptons, const std::vector<GenParticleF*> genParts, CORRTYPE elCorrType, CORRTYPE muCorrType ) const {
  // store gen leptons for matching
  std::vector<const GenParticleF*> genEl_;
  std::vector<const GenParticleF*> genMu_;
  for(const auto* p : genParts) {
    if(ParticleInfo::isA(ParticleInfo::p_Z0, p) || ParticleInfo::isA(ParticleInfo::p_Wplus, p) || ParticleInfo::isA(ParticleInfo::p_tauminus, p)) {
      for(unsigned int id = 0; id < p->numberOfDaughters(); ++id) {
        const GenParticleF* dau = p->daughter(id);
        if     (ParticleInfo::isA(ParticleInfo::p_eminus , dau)) genEl_.push_back(dau);
        else if(ParticleInfo::isA(ParticleInfo::p_muminus, dau)) genMu_.push_back(dau);
      } // W,Z daughters
    } // genPart is a Z or W
  } // genParts
  float weight = 1.0;
  for(auto* lep : allLeptons) {
    double nearDR = 0;
    int near = -1;
    if     (lep->pdgid()==11) near = PhysicsUtilities::findNearestDRDeref(*lep, genEl_, nearDR, 0.4);
    else if(lep->pdgid()==13) near = PhysicsUtilities::findNearestDRDeref(*lep, genMu_, nearDR, 0.4);
    if(near >= 0) {
      float tmpWt, tmpVetoWt;
      getLepWeight(tmpWt, tmpVetoWt, lep,elCorrType,muCorrType);
      int iL = lep->index();
      bool isSelectedLep = false;
      for(auto* sLep : selectedLeptons) {
        if(iL==sLep->index() && lep->pdgid()==sLep->pdgid()) isSelectedLep = true;
      }
      if(isSelectedLep) weight *= tmpWt;
      else              weight *= tmpVetoWt;
    }
  }
  return weight;
}

void LeptonCorrectionSet::load(TString fileName, TnPCorr::LEPSEL tnpLepSel, int correctionOptions)
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
      tnpCorr = new TnPCorr("TNP",tnpLepSel);
      corrections.push_back(tnpCorr);
    }
  }
}

void LeptonCorrectionSet::processCorrection(const BaseTreeAnalyzer * ana) {

  selLepWeight  = 1;
  vetoLepWeight = 1;
  tnpEvtWeight  = 1;
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
    for(auto* i : ana->secondaryLeptons) {
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
    tnpEvtWeight = tnpCorr->getEvtWeight(ana->allLeptons, ana->selectedLeptons,ana->genParts,cfg.corrections.tnpElCorrType,cfg.corrections.tnpMuCorrType);
  }

}

} /* namespace ucsbsusy */
