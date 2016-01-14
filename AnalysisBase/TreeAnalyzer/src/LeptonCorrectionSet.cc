#include "AnalysisBase/TreeAnalyzer/interface/BaseTreeAnalyzer.h"
#include "AnalysisBase/TreeAnalyzer/interface/DefaultProcessing.h"
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"
#include "TFile.h"

namespace ucsbsusy {

TnPCorr::TnPCorr(TString corrName,
                 const LeptonSelection::Electron elSel, const LeptonSelection::Electron secElSel,
                 const LeptonSelection::Muon     muSel, const LeptonSelection::Muon     secMuSel
                 ) : Correction(corrName)
{
  bool loadElectrons = false;
  if(elSel.type != LeptonSelection::NONE_ELE) {
    if(secElSel.type == LeptonSelection::NONE_ELE) {
      loadElectrons = true;
      elConf    = elSel;
      elConfKin = elSel;
    }
    else if(elSel.type == secElSel.type){
      loadElectrons = true;
      elConf    = elSel;
      elConfKin = elSel;
      if(secElSel.minPT  < elConfKin.minPT ) elConfKin.minPT  = secElSel.minPT;
      if(secElSel.maxETA > elConfKin.maxETA) elConfKin.maxETA = secElSel.maxETA;
    }
    else throw std::invalid_argument("LeptonCorectionSet::TnPCorr: electrons and secondaryElectrons need to be the same type for these corrections!");
  }
  else if(secElSel.type != LeptonSelection::NONE_ELE) {
    loadElectrons = true;
    elConf    = secElSel;
    elConfKin = secElSel;
  }

  bool loadMuons = false;
  if(muSel.type != LeptonSelection::NONE_MU) {
    if(secMuSel.type == LeptonSelection::NONE_MU) {
      loadMuons = true;
      muConf    = muSel;
      muConfKin = muSel;
    }
    else if(muSel.type == secMuSel.type){
      loadMuons = true;
      muConf    = muSel;
      muConfKin = muSel;
      if(secMuSel.minPT  < muConfKin.minPT ) muConfKin.minPT  = secMuSel.minPT;
      if(secMuSel.maxETA > muConfKin.maxETA) muConfKin.maxETA = secMuSel.maxETA;
    }
    else throw std::invalid_argument("LeptonCorectionSet::TnPCorr: muons and secondaryMuons need to be the same type for these corrections!");
  }
  else if(secMuSel.type != LeptonSelection::NONE_MU) {
    loadMuons = true;
    muConf    = secMuSel;
    muConfKin = secMuSel;
  }
  elConfKin.passID  = &ElectronID::inclusive;
  elConfKin.passISO = &ElectronISO::inclusive;
  muConfKin.passID  = &MuonID::inclusive;
  muConfKin.passISO = &MuonISO::inclusive;

  TString baseDir = TString::Format("%s/src/data/corrections/",getenv("CMSSW_BASE"));

  if(loadElectrons) {
    elConfNoIso = elConf;
    elConfNoIso.passISO = &ElectronISO::inclusive;
    elConfNoIso.minPT  = elConfKin.minPT  ;
    elConfNoIso.maxETA = elConfKin.maxETA ;

    TString elFileName                 , elMCVetoIdEffFileName, elMCVetoIsoEffFileName;
    TString elIdHistName, elIsoHistName, elMCVetoIdEffHistName, elMCVetoIsoEffHistName;

    // electron files and histograms
    fileEl = TFile::Open(baseDir+"kinematicBinSFele.root","read");
    if(elConf.type==LeptonSelection::ZL_SEL_ELE){
      elMCVetoIdEffFileName  = "tnpMCEffEl_ID_SR.root";
      elMCVetoIsoEffFileName = "tnpMCEffEl_Iso_SR.root";
      elIdHistName  = "CutBasedVeto";
      elIsoHistName = "MiniIso0p1_vs_RelActivity";
    }
    else if(elConf.type==LeptonSelection::ZL_CTR_ELE){
      elMCVetoIdEffFileName  = "tnpMCEffEl_ID_CR.root";
      elMCVetoIsoEffFileName = "tnpMCEffEl_Iso_CR.root";
      elIdHistName  = "CutBasedMedium";
      elIsoHistName = "MiniIso0p1_vs_RelActivity";
    }
    else throw std::invalid_argument("LeptonCorectionSet::TnPCorr: Invalid option for cfgSet::zl_lepton_set.electrons.type! Use 'ZL_SEL_ELE' or 'ZL_CTR_ELE'!");

    fileMCVetoIdEffEl  = TFile::Open(baseDir+elMCVetoIdEffFileName ,"read");
    fileMCVetoIsoEffEl = TFile::Open(baseDir+elMCVetoIsoEffFileName,"read");
    if(!fileEl            ) throw std::invalid_argument("LeptonCorectionSet::TnPCorr: el ID/Iso file could not be found!");
    if(!fileMCVetoIdEffEl ) throw std::invalid_argument("LeptonCorectionSet::TnPCorr: el MC ID Eff file could not be found!");
    if(!fileMCVetoIsoEffEl) throw std::invalid_argument("LeptonCorectionSet::TnPCorr: el MC Iso Eff file could not be found!");

    HistIdEl           = (TH2F*)(fileEl            ->Get(elIdHistName ));
    HistIsoEl          = (TH2F*)(fileEl            ->Get(elIsoHistName));
    HistMCVetoIdEffEl  = (TH2F*)(fileMCVetoIdEffEl ->Get("tnpEffIdEl"));
    HistMCVetoIsoEffEl = (TH2F*)(fileMCVetoIsoEffEl->Get("tnpEffIsoEl"));
    if(!HistIdEl          ) throw std::invalid_argument("LeptonCorectionSet::TnPCorr: el ID hist could not be found!");
    if(!HistIsoEl         ) throw std::invalid_argument("LeptonCorectionSet::TnPCorr: el ISO hist could not be found!");
    if(!HistMCVetoIdEffEl ) throw std::invalid_argument("LeptonCorectionSet::TnPCorr: el MC ID Eff hist could not be found!");
    if(!HistMCVetoIsoEffEl) throw std::invalid_argument("LeptonCorectionSet::TnPCorr: el MC Iso Eff hist could not be found!");
  } // loadElectrons

  if(loadMuons) {
    muConfNoIso = muConf;
    muConfNoIso.passISO = &MuonISO::inclusive;
    muConfNoIso.minPT  = muConfKin.minPT  ;
    muConfNoIso.maxETA = muConfKin.maxETA ;

    TString muIdFileName, muIsoFileName, muMCVetoIdEffFileName, muMCVetoIsoEffFileName;
    TString muIdHistName, muIsoHistName, muMCVetoIdEffHistName, muMCVetoIsoEffHistName;

    // muon files and histograms
    if(muConf.type==LeptonSelection::ZL_SEL_MU){
      muIdFileName           = "TnP_MuonID_NUM_LooseID_DENOM_generalTracks_VAR_map_pt_eta.root";
      muIsoFileName          = "TnP_MuonID_NUM_MiniIsoTight_DENOM_LooseID_VAR_map_activity_pt.root";
      muMCVetoIdEffFileName  = "tnpMCEffMu_ID_SR.root";
      muMCVetoIsoEffFileName = "tnpMCEffMu_Iso_SR.root";
      muIdHistName           = "pt_abseta_PLOT_pair_probeMultiplicity_bin0_&_tag_combRelIsoPF04dBeta_bin0_&_tag_pt_bin0_&_tag_IsoMu20_pass";
      muIsoHistName          = "SFmap";
    }
    else if(muConf.type==LeptonSelection::ZL_CTR_MU){
      muIdFileName        = "TnP_MuonID_NUM_MediumID_DENOM_generalTracks_VAR_map_pt_eta.root";
      muIsoFileName       = "TnP_MuonID_NUM_MiniIsoTight_DENOM_LooseID_VAR_map_activity_pt.root";
      muMCVetoIdEffFileName  = "tnpMCEffMu_ID_CR.root";
      muMCVetoIsoEffFileName = "tnpMCEffMu_Iso_CR.root";
      muIdHistName        = "pt_abseta_PLOT_pair_probeMultiplicity_bin0_&_tag_combRelIsoPF04dBeta_bin0_&_tag_pt_bin0_&_tag_IsoMu20_pass";
      muIsoHistName       = "SFmap";
    }
    else throw std::invalid_argument("LeptonCorectionSet::TnPCorr: Invalid option for cfgSet::zl_lepton_set.muons.type! Use 'ZL_SEL_MU' or 'ZL_CTR_Mu'!");

    fileIdMu           = TFile::Open(baseDir+muIdFileName          ,"read");
    fileIsoMu          = TFile::Open(baseDir+muIsoFileName         ,"read");
    fileMCVetoIdEffMu  = TFile::Open(baseDir+muMCVetoIdEffFileName ,"read");
    fileMCVetoIsoEffMu = TFile::Open(baseDir+muMCVetoIsoEffFileName,"read");
    if(!fileIdMu          ) throw std::invalid_argument("LeptonCorectionSet::TnPCorr: mu ID file could not be found!");
    if(!fileIsoMu         ) throw std::invalid_argument("LeptonCorectionSet::TnPCorr: mu Iso file could not be found!");
    if(!fileMCVetoIdEffMu ) throw std::invalid_argument("LeptonCorectionSet::TnPCorr: mu MC ID Eff file could not be found!");
    if(!fileMCVetoIsoEffMu) throw std::invalid_argument("LeptonCorectionSet::TnPCorr: mu MC Iso Eff file could not be found!");

    HistIdMu           = (TH2F*)(fileIdMu          ->Get(muIdHistName ));
    HistIsoMu          = (TH2F*)(fileIsoMu         ->Get(muIsoHistName));
    HistMCVetoIdEffMu  = (TH2F*)(fileMCVetoIdEffMu ->Get("tnpEffIdMu"));
    HistMCVetoIsoEffMu = (TH2F*)(fileMCVetoIsoEffMu->Get("tnpEffIsoMu"));
    if(!HistIdMu          ) throw std::invalid_argument("LeptonCorectionSet::TnPCorr: mu ID hist could not be found!");
    if(!HistIsoMu         ) throw std::invalid_argument("LeptonCorectionSet::TnPCorr: mu ISO hist could not be found!");
    if(!HistMCVetoIdEffMu ) throw std::invalid_argument("LeptonCorectionSet::TnPCorr: mu MC ID Eff hist could not be found!");
    if(!HistMCVetoIsoEffMu) throw std::invalid_argument("LeptonCorectionSet::TnPCorr: mu MC Iso Eff hist could not be found!");
  } // loadMuons

}

TnPCorr::~TnPCorr() {
  if(fileEl            ) fileEl            ->Close();
  if(fileIdMu          ) fileIdMu          ->Close();
  if(fileIsoMu         ) fileIsoMu         ->Close();
  if(fileMCVetoIdEffEl ) fileMCVetoIdEffEl ->Close();
  if(fileMCVetoIdEffMu ) fileMCVetoIdEffMu ->Close();
  if(fileMCVetoIsoEffEl) fileMCVetoIsoEffEl->Close();
  if(fileMCVetoIsoEffMu) fileMCVetoIsoEffMu->Close();
  delete fileEl            ;
  delete fileIdMu          ;
  delete fileIsoMu         ;
  delete fileMCVetoIdEffEl ;
  delete fileMCVetoIdEffMu ;
  delete fileMCVetoIsoEffEl;
  delete fileMCVetoIsoEffMu;
}

float TnPCorr::getLepWeight(LeptonF* lep, CORRTYPE elCorrType, CORRTYPE muCorrType ) const {
  float wt  = 1.0;
  int   id  = lep->pdgid();
  float pt  = lep->pt();
  float eta = lep->absEta();
  float annulus = lep->annulusactivity();
  if     (id==11 && elCorrType == NONE) return wt;
  else if(id==13 && muCorrType == NONE) return wt;
  else if(id==11 && (!cfgSet::isSelElectron(*(ElectronF*)lep,elConfKin))) return wt;
  else if(id==13 && (!cfgSet::isSelMuon(*(MuonF*)lep        ,muConfKin))) return wt;
  float sfid     = 1.0;
  float sfiso    = 1.0;
  float sfuncid  = 0.0;
  float sfunciso = 0.0;
  float effid    = 1.0;
  float effiso   = 1.0;
  bool passId    = false;
  bool passIdIso = false;
  float failIdWt = 1.0;
  if(id==11){
    sfid     = getElIDValue(pt,eta);
    sfiso    = getElIsoValue(pt,annulus);
    sfuncid  = getElIDError(pt,eta);
    sfunciso = getElIsoError(pt,annulus);
    effid    = getElMCIdEffValue(pt,eta);
    effiso   = getElMCIsoEffValue(pt,annulus);
    if(cfgSet::isSelElectron(*(ElectronF*)lep, elConfNoIso)) passId    = true;
    if(cfgSet::isSelElectron(*(ElectronF*)lep, elConf)     ) passIdIso = true;
    if(failIdWt>1.0) throw std::invalid_argument("LeptonCorectionSet::TnPCorr: electron ID eff is >=1!");
    failIdWt = (1.0-effid*sfid)/(1.0-effid);
  }
  else if(id==13) {
    sfid      = getMuIDValue(pt,eta);
    sfiso     = getMuIsoValue(pt,annulus);
    sfuncid   = sfid  *0.01;
    sfunciso  = sfiso *0.01;
    //effid    = getMuMCIdEffValue(pt,eta);
    effiso   = getMuMCIsoEffValue(pt,annulus);
    if(cfgSet::isSelMuon(*(MuonF*)lep, muConfNoIso)) passId    = true;
    if(cfgSet::isSelMuon(*(MuonF*)lep, muConf)     ) passIdIso = true;
  }
  if     (id==11 && elCorrType == UP  ) { sfid += sfuncid; sfiso += sfunciso; }
  else if(id==11 && elCorrType == DOWN) { sfid -= sfuncid; sfiso -= sfunciso; }
  else if(id==13 && muCorrType == UP  ) { sfid += sfuncid; sfiso += sfunciso; }
  else if(id==13 && muCorrType == DOWN) { sfid -= sfuncid; sfiso -= sfunciso; }
  if(passIdIso) wt = sfid * sfiso;
  else if(passId) {
    if(effiso>=1.0) throw std::invalid_argument("LeptonCorectionSet::TnPCorr: Iso eff is >=1!");
    wt = sfid * (1.0-effiso*sfiso)/(1.0-effiso);
  }
  else wt = failIdWt;
  if(wt < -2.0) wt = -2.0; // don't want large negative weights ... need to treat these cases better
  return wt;
}

float TnPCorr::getGenLepWeight(const GenParticleF* lep, CORRTYPE muCorrType ) const {
  if(muCorrType == NONE) return 1.0;
  float pt  = lep->pt();
  float eta = lep->absEta();
  if     (pt <muConfKin.minPT)  return 1.0;
  else if(eta>muConfKin.maxETA) return 1.0;
  float sf      = getMuIDValue(pt,eta);
  float sfunc   = sf  *0.01;
  float eff     = getMuMCIdEffValue(pt,eta);
  if     (muCorrType == UP  ) sf += sfunc;
  else if(muCorrType == DOWN) sf -= sfunc;
  if(eff>=1.0) throw std::invalid_argument("LeptonCorectionSet::TnPCorr: muon ID eff is >=1!");
  return (1.0-eff*sf)/(1.0-eff);
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
    if(near >= 0) weight *= getLepWeight(lep,elCorrType,muCorrType);
  }
  std::vector<const LeptonF*> recMu_;
  for(const auto* lep : allLeptons) if(lep->pdgid()==13) recMu_.push_back(lep);
  for(auto* lep : genMu_) {
    double nearDR = 0;
    int near = PhysicsUtilities::findNearestDRDeref(*lep, recMu_, nearDR, 0.4);
    if(near<0) weight *= getGenLepWeight(lep,muCorrType);
    else if(!cfgSet::isSelMuon(*(MuonF*)recMu_[near], muConfNoIso)) weight *= getGenLepWeight(lep,muCorrType);
  }
  return weight;
}

void LeptonCorrectionSet::load(TString fileName,
                               const LeptonSelection::Electron elSel, const LeptonSelection::Electron secElSel,
                               const LeptonSelection::Muon     muSel, const LeptonSelection::Muon     secMuSel,
                               int correctionOptions)
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
      tnpCorr = new TnPCorr("TNP", elSel, secElSel, muSel, secMuSel);
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
