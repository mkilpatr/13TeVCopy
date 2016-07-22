#include "AnalysisBase/TreeAnalyzer/interface/BaseTreeAnalyzer.h"
#include "AnalysisBase/TreeAnalyzer/interface/DefaultProcessing.h"
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"
#include "TFile.h"


namespace ucsbsusy {

const std::vector<double> LepCorr::eleCorrPtBins = {0., 20., 30., 40., 70., 100., 1000.};
const std::vector<double> LepCorr::muCorrPtBins = {0., 20., 30., 40., 70., 100., 1000.};
const std::vector<double> LepCorr::tauCorrPtBins = {0., 20., 40., 1000.};
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

  TString baseDir = TString::Format("%s/src/data/corrections/2016/lepCorMCEffsAndSFs/",getenv("CMSSW_BASE"));

  if(loadElectrons) {
    elConfNoIso = elConf;
    elConfNoIso.passISO = &ElectronISO::inclusive;
    elConfNoIso.minPT  = elConfKin.minPT  ;
    elConfNoIso.maxETA = elConfKin.maxETA ;

    TString elFileName;
    TString elMCVetoLMIdEffFileName, elMCVetoLMIsoEffFileName;
    TString elMCVetoHMIdEffFileName, elMCVetoHMIsoEffFileName;
    TString elIdHistName, elIsoHistName;
    TString elMCVetoLMIdEffHistName, elMCVetoLMIsoEffHistName;
    TString elMCVetoHMIdEffHistName, elMCVetoHMIsoEffHistName;

    // electron files and histograms
    fileEl = TFile::Open(baseDir+"scaleFactors5p7invfb.root","read");
    if(elConf.type==LeptonSelection::ZL_SEL_ELE){
      elMCVetoLMIdEffFileName  = "lepCorMCEff_LM_El_Id_SR.root";
      elMCVetoLMIsoEffFileName = "lepCorMCEff_LM_El_Iso_SR.root";
      elMCVetoHMIdEffFileName  = "lepCorMCEff_HM_El_Id_SR.root";
      elMCVetoHMIsoEffFileName = "lepCorMCEff_HM_El_Iso_SR.root";
      elIdHistName  = "GsfElectronToVeto";
      elIsoHistName = "MVAVLooseElectronToMini"; // is MiniIso < 0.1 [pt,eta]
    }
    else if(elConf.type==LeptonSelection::ZL_CTR_ELE){
      elMCVetoLMIdEffFileName  = "lepCorMCEff_LM_El_Id_CR.root";
      elMCVetoLMIsoEffFileName = "lepCorMCEff_LM_El_Iso_CR.root";
      elMCVetoHMIdEffFileName  = "lepCorMCEff_HM_El_Id_CR.root";
      elMCVetoHMIsoEffFileName = "lepCorMCEff_HM_El_Iso_CR.root";
      elIdHistName  = "GsfElectronToMedium";
      elIsoHistName = "MVAVLooseElectronToMini"; // is MiniIso < 0.1 [pt,eta]
    }
    else throw std::invalid_argument("LeptonCorectionSet::TnPCorr: Invalid option for cfgSet::zl_lepton_set.electrons.type! Use 'ZL_SEL_ELE' or 'ZL_CTR_ELE'!");

    fileMCVetoLMIdEffEl  = TFile::Open(baseDir+elMCVetoLMIdEffFileName ,"read");
    fileMCVetoLMIsoEffEl = TFile::Open(baseDir+elMCVetoLMIsoEffFileName,"read");
    fileMCVetoHMIdEffEl  = TFile::Open(baseDir+elMCVetoHMIdEffFileName ,"read");
    fileMCVetoHMIsoEffEl = TFile::Open(baseDir+elMCVetoHMIsoEffFileName,"read");
    if(!fileEl            ) throw std::invalid_argument("LeptonCorectionSet::TnPCorr: el ID/Iso file could not be found!");
    if(!fileMCVetoLMIdEffEl ) throw std::invalid_argument("LeptonCorectionSet::TnPCorr: el MC ID Eff file could not be found!");
    if(!fileMCVetoLMIsoEffEl) throw std::invalid_argument("LeptonCorectionSet::TnPCorr: el MC Iso Eff file could not be found!");
    if(!fileMCVetoHMIdEffEl ) throw std::invalid_argument("LeptonCorectionSet::TnPCorr: el MC ID Eff file could not be found!");
    if(!fileMCVetoHMIsoEffEl) throw std::invalid_argument("LeptonCorectionSet::TnPCorr: el MC Iso Eff file could not be found!");

    HistIdEl           = (TH2F*)(fileEl            ->Get(elIdHistName ));
    HistIsoEl          = (TH2F*)(fileEl            ->Get(elIsoHistName));
    HistMCVetoLMIdEffEl  = (TH2F*)(fileMCVetoLMIdEffEl ->Get("lepCorMCEff_El_Id"));
    HistMCVetoLMIsoEffEl = (TH2F*)(fileMCVetoLMIsoEffEl->Get("lepCorMCEff_El_Iso"));
    HistMCVetoHMIdEffEl  = (TH2F*)(fileMCVetoHMIdEffEl ->Get("lepCorMCEff_El_Id"));
    HistMCVetoHMIsoEffEl = (TH2F*)(fileMCVetoHMIsoEffEl->Get("lepCorMCEff_El_Iso"));
    if(!HistIdEl          ) throw std::invalid_argument("LeptonCorectionSet::TnPCorr: el ID hist could not be found!");
    if(!HistIsoEl         ) throw std::invalid_argument("LeptonCorectionSet::TnPCorr: el ISO hist could not be found!");
    if(!HistMCVetoLMIdEffEl ) throw std::invalid_argument("LeptonCorectionSet::TnPCorr: el MC ID Eff hist could not be found!");
    if(!HistMCVetoLMIsoEffEl) throw std::invalid_argument("LeptonCorectionSet::TnPCorr: el MC Iso Eff hist could not be found!");
    if(!HistMCVetoHMIdEffEl ) throw std::invalid_argument("LeptonCorectionSet::TnPCorr: el MC ID Eff hist could not be found!");
    if(!HistMCVetoHMIsoEffEl) throw std::invalid_argument("LeptonCorectionSet::TnPCorr: el MC Iso Eff hist could not be found!");
  } // loadElectrons

  if(loadMuons) {
    muConfNoIso = muConf;
    muConfNoIso.passISO = &MuonISO::inclusive;
    muConfNoIso.minPT  = muConfKin.minPT  ;
    muConfNoIso.maxETA = muConfKin.maxETA ;

    TString muIP2DFileName;
    TString muIdFileName, muIsoFileName;
    TString muMCVetoLMIdEffFileName, muMCVetoLMIsoEffFileName;
    TString muMCVetoHMIdEffFileName, muMCVetoHMIsoEffFileName;
    TString muIP2DHistName;
    TString muIdHistName, muIsoHistName;
    TString muMCVetoLMIdEffHistName, muMCVetoLMIsoEffHistName;
    TString muMCVetoHMIdEffHistName, muMCVetoHMIsoEffHistName;

    // muon files and histograms
    if(muConf.type==LeptonSelection::ZL_SEL_MU){
      muIdFileName           = "dummy.root"; // only med muon ID currently supported with SFs per twiki
      muIsoFileName          = "TnP_MuonID_NUM_MiniIsoTight_DENOM_LooseID_VAR_map_pt_eta.root";
      muIP2DFileName         = "TnP_MuonID_NUM_MediumIP2D_DENOM_LooseID_VAR_map_pt_eta.root";
      muIP2DHistName         = "pt_abseta_PLOT_pair_probeMultiplicity_bin0_&_PF_pass";
      muMCVetoLMIdEffFileName  = "lepCorMCEff_LM_Mu_Id_SR.root";
      muMCVetoLMIsoEffFileName = "lepCorMCEff_LM_Mu_Iso_SR.root";
      muMCVetoHMIdEffFileName  = "lepCorMCEff_HM_Mu_Id_SR.root";
      muMCVetoHMIsoEffFileName = "lepCorMCEff_HM_Mu_Iso_SR.root";
      //muIdHistName           = "pt_abseta_PLOT_pair_probeMultiplicity_bin0";
      muIdHistName           = "dummy";
      muIsoHistName          = "pt_abseta_PLOT_pair_probeMultiplicity_bin0_&_PF_pass";
    }
    else if(muConf.type==LeptonSelection::ZL_CTR_MU){
      muIdFileName        = "TnP_MuonID_NUM_MediumID_DENOM_generalTracks_VAR_map_pt_eta.root";
      muIsoFileName       = "TnP_MuonID_NUM_MiniIsoTight_DENOM_MediumID_VAR_map_pt_eta.root";
      muIP2DFileName         = "TnP_MuonID_NUM_TightIP2D_DENOM_MediumID_VAR_map_pt_eta.root";
      muIP2DHistName         = "pt_abseta_PLOT_pair_probeMultiplicity_bin0_&_Medium2016_pass";
      muMCVetoLMIdEffFileName  = "lepCorMCEff_LM_Mu_Id_CR.root";
      muMCVetoLMIsoEffFileName = "lepCorMCEff_LM_Mu_Iso_CR.root";
      muMCVetoHMIdEffFileName  = "lepCorMCEff_HM_Mu_Id_CR.root";
      muMCVetoHMIsoEffFileName = "lepCorMCEff_HM_Mu_Iso_CR.root";
      muIdHistName        = "pt_abseta_PLOT_pair_probeMultiplicity_bin0";
      muIsoHistName       = "pt_abseta_PLOT_pair_probeMultiplicity_bin0_&_Medium2016_pass";
    }
    else throw std::invalid_argument("LeptonCorectionSet::TnPCorr: Invalid option for cfgSet::zl_lepton_set.muons.type! Use 'ZL_SEL_MU' or 'ZL_CTR_Mu'!");

    TString muTrackerFileName      = "general_tracks_and_early_general_tracks_corr_ratio.root";
    TString muTrackerPtg10HistName = "mutrksfptg10";
    TString muTrackerPtl10HistName = "mutrksfptl10";

    fileIdMu           = TFile::Open(baseDir+muIdFileName          ,"read");
    fileIsoMu          = TFile::Open(baseDir+muIsoFileName         ,"read");
    fileMCVetoLMIdEffMu  = TFile::Open(baseDir+muMCVetoLMIdEffFileName ,"read");
    fileMCVetoLMIsoEffMu = TFile::Open(baseDir+muMCVetoLMIsoEffFileName,"read");
    fileMCVetoHMIdEffMu  = TFile::Open(baseDir+muMCVetoHMIdEffFileName ,"read");
    fileMCVetoHMIsoEffMu = TFile::Open(baseDir+muMCVetoHMIsoEffFileName,"read");
    fileTrackerMu      = TFile::Open(baseDir+muTrackerFileName     ,"read");
    fileIP2DMu         = TFile::Open(baseDir+muIP2DFileName, "read");

    if(!fileIdMu          ) throw std::invalid_argument("LeptonCorectionSet::TnPCorr: mu ID file could not be found!");
    if(!fileIsoMu         ) throw std::invalid_argument("LeptonCorectionSet::TnPCorr: mu Iso file could not be found!");
    if(!fileMCVetoLMIdEffMu ) throw std::invalid_argument("LeptonCorectionSet::TnPCorr: mu MC ID Eff file could not be found!");
    if(!fileMCVetoLMIsoEffMu) throw std::invalid_argument("LeptonCorectionSet::TnPCorr: mu MC Iso Eff file could not be found!");
    if(!fileMCVetoHMIdEffMu ) throw std::invalid_argument("LeptonCorectionSet::TnPCorr: mu MC ID Eff file could not be found!");
    if(!fileMCVetoHMIsoEffMu) throw std::invalid_argument("LeptonCorectionSet::TnPCorr: mu MC Iso Eff file could not be found!");
    if(!fileTrackerMu     ) throw std::invalid_argument("LeptonCorectionSet::TnPCorr: meow! mu tracker SF file could not be found!");
    if(!fileIP2DMu        ) throw std::invalid_argument("LeptonCorectionSet::TnPCorr: mu IP2D SF file could not be found!");

    HistIdMu           = (TH2F*)(fileIdMu          ->Get(muIdHistName ));
    HistIsoMu          = (TH2F*)(fileIsoMu         ->Get(muIsoHistName));
    HistMCVetoLMIdEffMu  = (TH2F*)(fileMCVetoLMIdEffMu ->Get("lepCorMCEff_Mu_Id"));
    HistMCVetoLMIsoEffMu = (TH2F*)(fileMCVetoLMIsoEffMu->Get("lepCorMCEff_Mu_Iso"));
    HistMCVetoHMIdEffMu  = (TH2F*)(fileMCVetoHMIdEffMu ->Get("lepCorMCEff_Mu_Id"));
    HistMCVetoHMIsoEffMu = (TH2F*)(fileMCVetoHMIsoEffMu->Get("lepCorMCEff_Mu_Iso"));
    HistMuTrackerPtg10 = (TH1F*)(fileTrackerMu->Get(muTrackerPtg10HistName));
    HistMuTrackerPtl10 = (TH1F*)(fileTrackerMu->Get(muTrackerPtl10HistName));
    HistMuIP2D         = (TH2F*)(fileIP2DMu->Get(muIP2DHistName));

    if(!HistIdMu          ) throw std::invalid_argument("LeptonCorectionSet::TnPCorr: mu ID hist could not be found!");
    if(!HistIsoMu         ) throw std::invalid_argument("LeptonCorectionSet::TnPCorr: mu ISO hist could not be found!");
    if(!HistMCVetoLMIdEffMu ) throw std::invalid_argument("LeptonCorectionSet::TnPCorr: mu MC ID Eff hist could not be found!");
    if(!HistMCVetoLMIsoEffMu) throw std::invalid_argument("LeptonCorectionSet::TnPCorr: mu MC Iso Eff hist could not be found!");
    if(!HistMCVetoHMIdEffMu ) throw std::invalid_argument("LeptonCorectionSet::TnPCorr: mu MC ID Eff hist could not be found!");
    if(!HistMCVetoHMIsoEffMu) throw std::invalid_argument("LeptonCorectionSet::TnPCorr: mu MC Iso Eff hist could not be found!");
    if(!HistMuTrackerPtg10)   throw std::invalid_argument("LeptonCorectionSet::TnPCorr: mu tracker SF hist g10 could not be found!");
    if(!HistMuTrackerPtl10)   throw std::invalid_argument("LeptonCorectionSet::TnPCorr: mu tracker SF hist l10 could not be found!");
    if(!HistMuIP2D)           throw std::invalid_argument("LeptonCorectionSet::TnPCorr: mu ID SF hist could not be found!");
  } // loadMuons

}

TnPCorr::~TnPCorr() {
  if(fileEl            ) fileEl            ->Close();
  if(fileIdMu          ) fileIdMu          ->Close();
  if(fileIsoMu         ) fileIsoMu         ->Close();
  if(fileMCVetoLMIdEffEl ) fileMCVetoLMIdEffEl ->Close();
  if(fileMCVetoLMIdEffMu ) fileMCVetoLMIdEffMu ->Close();
  if(fileMCVetoHMIdEffEl ) fileMCVetoHMIdEffEl ->Close();
  if(fileMCVetoHMIdEffMu ) fileMCVetoHMIdEffMu ->Close();
  if(fileMCVetoLMIsoEffEl) fileMCVetoLMIsoEffEl->Close();
  if(fileMCVetoLMIsoEffMu) fileMCVetoLMIsoEffMu->Close();
  if(fileMCVetoHMIsoEffEl) fileMCVetoHMIsoEffEl->Close();
  if(fileMCVetoHMIsoEffMu) fileMCVetoHMIsoEffMu->Close();
  if(fileTrackerMu     ) fileTrackerMu     ->Close();
  if(fileIP2DMu        ) fileIP2DMu        ->Close();

  delete fileEl            ;
  delete fileIdMu          ;
  delete fileIsoMu         ;
  delete fileMCVetoLMIdEffEl ;
  delete fileMCVetoLMIdEffMu ;
  delete fileMCVetoLMIsoEffEl;
  delete fileMCVetoLMIsoEffMu;
  delete fileMCVetoHMIdEffEl ;
  delete fileMCVetoHMIdEffMu ;
  delete fileMCVetoHMIsoEffEl;
  delete fileMCVetoHMIsoEffMu;
  delete fileTrackerMu     ;
  delete fileIP2DMu        ;
}

float TnPCorr::getLepWeight(LeptonF* lep, CORRTYPE elCorrType, CORRTYPE muCorrType, TString region ) const {
  float wt  = 1.0;
  int   id  = lep->pdgid();
  float pt  = lep->pt();
  float eta = lep->eta();
  float abseta = lep->absEta();
  if     (id==11 && (!cfgSet::isSelElectron(*(ElectronF*)lep,elConfKin))) return wt;
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
    sfid     = getElIDValue(pt,abseta);
    sfiso    = getElIsoValue(pt,abseta);
    sfuncid  = getElIDError(pt,abseta);
    sfunciso = getElIsoError(pt,abseta);
    if     (elCorrType  == UP  ) sfid  += sfuncid;
    else if(elCorrType  == DOWN) sfid  -= sfuncid;
    if     (elCorrType == UP  ) sfiso += sfunciso;
    else if(elCorrType == DOWN) sfiso -= sfunciso;
    effid    = getElMCIdEffValue(pt,abseta,region);
    effiso   = getElMCIsoEffValue(pt,abseta,region);
    if(cfgSet::isSelElectron(*(ElectronF*)lep, elConfNoIso)) passId    = true;
    if(cfgSet::isSelElectron(*(ElectronF*)lep, elConf)     ) passIdIso = true;
    if(effid>=1.0) throw std::invalid_argument("LeptonCorectionSet::TnPCorr: electron ID eff is >=1!");
    failIdWt = (1.0-effid*sfid)/(1.0-effid);
  }
  else if(id==13) {
    sfid      = getMuIDValue(pt,abseta);
    float trackSFvalue    = (pt > 10) ? getMuTrackerPtg10Value(eta) : getMuTrackerPtl10Value(eta);
    if(trackSFvalue == 0) throw std::invalid_argument("LeptonCorectionSet::TnPCorr::getLepWeight: muon track SF is zero!");
    float trackSFrelError = ((pt > 10) ? getMuTrackerPtg10Error(eta) : getMuTrackerPtl10Error(eta))/trackSFvalue;
    float ipSFvalue = getMuIP2DValue(pt,abseta);
    float ipSFerror = getMuIP2DError(pt,abseta);
    if(ipSFvalue == 0) throw std::invalid_argument("LeptonCorectionSet::TnPCorr::getLepWeight: muon IP SF is zero!");    
    float ipSFrelError = ipSFerror/ipSFvalue;
    sfiso     = getMuIsoValue(pt,abseta);
    float sfrelError = 0.03; // conservative july 2016 recommendation
    sfuncid   = sfid*trackSFvalue*ipSFvalue*sqrt( pow(sfrelError,2) + pow(trackSFrelError,2) + pow(ipSFrelError,2));
    sfunciso  = sfiso * sfrelError;
    sfid     *= trackSFvalue*ipSFvalue; // do only after calculating sfuncid!
    if     (muCorrType  == UP  ) sfid  += sfuncid;
    else if(muCorrType  == DOWN) sfid  -= sfuncid;
    if     (muCorrType == UP  ) sfiso += sfunciso;
    else if(muCorrType == DOWN) sfiso -= sfunciso;
    //effid    = getMuMCIdEffValue(pt,abseta); // get this from gen muons since we don't store generalTracks
    effiso   = getMuMCIsoEffValue(pt,abseta,region);
    if(cfgSet::isSelMuon(*(MuonF*)lep, muConfNoIso)) passId    = true;
    if(cfgSet::isSelMuon(*(MuonF*)lep, muConf)     ) passIdIso = true;
    failIdWt = 1.0; // handle with gen muons
  }
  if(passIdIso) {
    wt = sfid * sfiso;
  }
  else if(passId) {
    if(effiso>=1.0) throw std::invalid_argument("LeptonCorectionSet::TnPCorr: Iso eff is >=1!");
    wt = sfid * (1.0-effiso*sfiso)/(1.0-effiso);
  }
  else {
    wt = failIdWt;
  }
  if(wt < -2.0) wt = -2.0; // don't want large negative weights ... need to treat these cases better
  if(wt >  2.0) wt =  2.0; // also don't want large positive weights
  return wt;
}

// ASSUMES MUON -- tracker and ip corrections
float TnPCorr::getGenLepWeight(const GenParticleF* lep, CORRTYPE muCorrType, TString region ) const {
  float pt  = lep->pt();
  float abseta = lep->absEta();
  float eta = lep->eta();
  if     (pt <muConfKin.minPT)  return 1.0;
  else if(abseta>muConfKin.maxETA) return 1.0;
  float trackSFvalue    =  (pt > 10) ? getMuTrackerPtg10Value(eta) : getMuTrackerPtl10Value(eta);
  if(trackSFvalue == 0) throw std::invalid_argument("LeptonCorectionSet::TnPCorr::getGenLepWeight: muon track SF is zero!");
  float trackSFrelError = ((pt > 10) ? getMuTrackerPtg10Error(eta) : getMuTrackerPtl10Error(eta))/trackSFvalue;
  float ipSFvalue = getMuIP2DValue(pt,abseta);
  float ipSFerror = getMuIP2DError(pt,abseta);
  if(ipSFvalue == 0) throw std::invalid_argument("LeptonCorectionSet::TnPCorr::getGenLepWeight: muon IP SF is zero!");
  float ipSFrelError = ipSFerror/ipSFvalue;
  float sf      = getMuIDValue(pt,abseta);
  float sfrelError = 0.03; // conservative july 2016 recommendation
  float sfunc   = sf*trackSFvalue*ipSFvalue*sqrt( pow(sfrelError,2) + pow(trackSFrelError,2) + pow(ipSFrelError,2));
  sf           *= trackSFvalue*ipSFvalue; // do only after calculating sfunc!
  float eff     = getMuMCIdEffValue(pt,abseta,region);
  if     (muCorrType == UP  ) sf += sfunc;
  else if(muCorrType == DOWN) sf -= sfunc;
  if(eff>=1.0) throw std::invalid_argument("LeptonCorectionSet::TnPCorr: muon ID eff is >=1!");
  float wt = (1.0-eff*sf)/(1.0-eff);
  if(wt < -2.0) wt = -2.0; // don't want large negative weights ... need to treat these cases better
  if(wt >  2.0) wt =  2.0; // also don't want large positive weights
  return wt;
}


float TnPCorr::getEvtWeight(const std::vector<LeptonF*>& allLeptons, const std::vector<LeptonF*>& selectedLeptons, const std::vector<GenParticleF*> genParts,
                            CORRTYPE elCorrType, CORRTYPE muCorrType, TString region ) const {
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
    if     (lep->pdgid()==11 && (elCorrType==NONE)) continue;
    else if(lep->pdgid()==13 && (muCorrType==NONE)) continue;
    double nearDR = 0;
    int near = -1;
    if     (lep->pdgid()==11) near = PhysicsUtilities::findNearestDRDeref(*lep, genEl_, nearDR, 0.4);
    else if(lep->pdgid()==13) near = PhysicsUtilities::findNearestDRDeref(*lep, genMu_, nearDR, 0.4);
    if(near >= 0) weight *= getLepWeight(lep,elCorrType,muCorrType,region);
  }
  std::vector<const LeptonF*> recMu_;
  for(const auto* lep : allLeptons) if(lep->pdgid()==13) recMu_.push_back(lep);
  if(muCorrType!=NONE) {
    for(auto* lep : genMu_) {
      double nearDR = 0;
      int near = PhysicsUtilities::findNearestDRDeref(*lep, recMu_, nearDR, 0.4);
      if(near<0) weight *= getGenLepWeight(lep,muCorrType,region);
      else if(!cfgSet::isSelMuon(*(MuonF*)recMu_[near], muConfNoIso)) weight *= getGenLepWeight(lep,muCorrType,region);
    }
  }
  if(weight < -2.0) weight = -2.0; // also don't want large overall
  if(weight >  2.0) weight =  2.0; // event weight changes
  return weight;
}

void LeptonCorrectionSet::load(TString fileNameLM, TString fileNameHM,
                               const LeptonSelection::Electron elSel, const LeptonSelection::Electron secElSel,
                               const LeptonSelection::Muon     muSel, const LeptonSelection::Muon     secMuSel,
                               int correctionOptions)
{
  if(correctionOptions & LEP) {
    loadSimple("LEP",correctionOptions);
    if(options_ & LEP) {
      lepCorr = new LepCorr(fileNameLM,fileNameHM);
      corrections.push_back(lepCorr);
      setMultiPtBins(true); // hack - stick with multipt bins
      if(options_ & USE_HPSTAUS){
        setUseHPSTaus(true);
      }
      if(options_ & MULTI_PT_BINS){
        setMultiPtBins(true);
      }
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
  selLepWeightLM  = 1.;
  vetoLepWeightLM = 1.;
  tnpEvtWeightLM  = 1.;
  selLepWeightHM  = 1.;
  vetoLepWeightHM = 1.;
  tnpEvtWeightHM  = 1.;
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

    if(multiPtBins) {
      for(auto* i : ana->selectedLeptons) {
        if(fabs(i->pdgid()) == 11) {
          nSelectedElectrons++;
        }
        if(fabs(i->pdgid()) == 13) {
          nSelectedMuons++;
        }
      }
    } else {
      for(auto* i : ana->selectedLeptons) {
        if(fabs(i->pdgid()) == 11) {
          nSelectedElectrons++;
        }
        if(fabs(i->pdgid()) == 13) {
          nSelectedMuons++;
        }
      }
    }

    // tau selected region
    if (!useHPS && ana->nVetoedTracks >= 1 && nSelectedElectrons == 0 && nSelectedMuons == 0 && nPromptGenTaus >= 1) {
      if(multiPtBins) {
        for(unsigned int iptbin = 0; iptbin < LepCorr::tauCorrPtBins.size()-1; ++iptbin) {
          if(iptbin == LepCorr::tauCorrPtBins.size()-2) {
            if(ana->vetoedTracks[0]->pt() > LepCorr::tauCorrPtBins[iptbin]) {
            lepCorr->setTargetBin(LepCorr::muCorrPtBins.size() + LepCorr::eleCorrPtBins.size() + iptbin,1);
            lepCorr->setTargetBin(LepCorr::muCorrPtBins.size() + LepCorr::eleCorrPtBins.size() + iptbin,2);
            }
          } else {
            if(ana->vetoedTracks[0]->pt() > LepCorr::tauCorrPtBins[iptbin] && ana->vetoedTracks[0]->pt() <= LepCorr::tauCorrPtBins[iptbin+1]) {
              lepCorr->setTargetBin(LepCorr::muCorrPtBins.size() + LepCorr::eleCorrPtBins.size() + iptbin,1);
              lepCorr->setTargetBin(LepCorr::muCorrPtBins.size() + LepCorr::eleCorrPtBins.size() + iptbin,2);
            }
          }
        }
      } else {
        if(ana->vetoedTracks[0]->pt() > 20.0) {
          lepCorr->setTargetBin(LepCorr::tauCorrBinHighPt,1);
          lepCorr->setTargetBin(LepCorr::tauCorrBinHighPt,2);
        }else{
          lepCorr->setTargetBin(LepCorr::tauCorrBinLowPt,1);
          lepCorr->setTargetBin(LepCorr::tauCorrBinLowPt,2);
        }
      }
    }
    // HPS tau selected region
    else if (useHPS && ana->nVetoHPSTaus >= 1 && nSelectedElectrons == 0 && nSelectedMuons == 0 && nPromptGenTaus >= 1) {
      if(multiPtBins){
        lepCorr->setTargetBin(LepCorr::muCorrPtBins.size() + LepCorr::eleCorrPtBins.size() + LepCorr::tauCorrPtBins.size() - 1,1);
        lepCorr->setTargetBin(LepCorr::muCorrPtBins.size() + LepCorr::eleCorrPtBins.size() + LepCorr::tauCorrPtBins.size() - 1,2);
      }else{
        lepCorr->setTargetBin(LepCorr::hpsTauCorrBin,1);
        lepCorr->setTargetBin(LepCorr::hpsTauCorrBin,2);
      }
    }
    // fake region
    else  if ((nSelectedMuons >= 1) || (nSelectedElectrons >= 1) || (ana->nVetoedTracks >= 1)) {
      if(multiPtBins) {
        lepCorr->setTargetBin(LepCorr::muCorrPtBins.size() + LepCorr::eleCorrPtBins.size() + LepCorr::tauCorrPtBins.size(),1);
        lepCorr->setTargetBin(LepCorr::muCorrPtBins.size() + LepCorr::eleCorrPtBins.size() + LepCorr::tauCorrPtBins.size(),2);
      }else{
        lepCorr->setTargetBin(LepCorr::fakeBin,1);
        lepCorr->setTargetBin(LepCorr::fakeBin,2);
      }
    }
    // veto region
    else {
      lepCorr->setTargetBin(LepCorr::defaultBin,1); // the veto region for which vetoLepWeight = 1, selLepWeight = 0
      lepCorr->setTargetBin(LepCorr::defaultBin,2);
    }

    vetoLepWeightLM = 1 - lepCorr->get(1);
    selLepWeightLM  = lepCorr->get(1);
    vetoLepWeightHM = 1 - lepCorr->get(2);
    selLepWeightHM  = lepCorr->get(2);
  

    // adjust weights for systematic variations 

    // tau selected region
    if (!useHPS && ana->nVetoedTracks >= 1 && nSelectedElectrons == 0 && nSelectedMuons == 0 && nPromptGenTaus >= 1) {
      if((options_ & LEP_VARY_UP) || (options_ & TAU_VARY_UP)) {
        vetoLepWeightLM -= lepCorr->getError(1);
        selLepWeightLM  += lepCorr->getError(1);
        vetoLepWeightHM -= lepCorr->getError(2);
        selLepWeightHM  += lepCorr->getError(2);
      }
      else if((options_ & LEP_VARY_DOWN) || (options_ & TAU_VARY_DOWN)) {
        vetoLepWeightLM += lepCorr->getError(1);
        selLepWeightLM  -= lepCorr->getError(1);
        vetoLepWeightHM += lepCorr->getError(2);
        selLepWeightHM  -= lepCorr->getError(2);
      }
    }
    // HPS tau selected region
    else if (useHPS && ana->nVetoHPSTaus >= 1 && nSelectedElectrons == 0 && nSelectedMuons == 0 && nPromptGenTaus >= 1) {
      if((options_ & LEP_VARY_UP) || (options_ & TAU_VARY_UP)) {
        vetoLepWeightLM -= lepCorr->getError(1);
        selLepWeightLM  += lepCorr->getError(1);
        vetoLepWeightHM -= lepCorr->getError(2);
        selLepWeightHM  += lepCorr->getError(2);
      }
      else if((options_ & LEP_VARY_DOWN) || (options_ & TAU_VARY_DOWN)) {
        vetoLepWeightLM += lepCorr->getError(1);
        selLepWeightLM  -= lepCorr->getError(1);
        vetoLepWeightHM += lepCorr->getError(2);
        selLepWeightHM  -= lepCorr->getError(2);
      }
    }
    // fake region
    else  if ((nSelectedMuons >= 1) || (nSelectedElectrons >= 1) || (ana->nVetoedTracks >= 1)) {
      // keep for structure
    }
    // veto region
    else {
      // keep for structure
    }

  }

  if(options_ & TNP) {
    const cfgSet::ConfigSet& cfg = ana->getAnaCfg();
    tnpEvtWeightLM = tnpCorr->getEvtWeight(ana->allLeptons, ana->selectedLeptons,ana->genParts, cfg.corrections.tnpElCorrType, cfg.corrections.tnpMuCorrType, "LM");
    tnpEvtWeightHM = tnpCorr->getEvtWeight(ana->allLeptons, ana->selectedLeptons,ana->genParts, cfg.corrections.tnpElCorrType, cfg.corrections.tnpMuCorrType, "HM");
  }

}

} /* namespace ucsbsusy */

