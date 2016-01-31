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

float fail(float E, float sf)       { return (1-E*sf)/(1-E); }
float fail2(float E, float sf)      { return fail(E,sf)*fail(E,sf); }
float dfaildE(float E)              { return -E/(1-E); }
float dfaildE2(float E)             { return dfaildE(E)*dfaildE(E); }
float dfaildsf(float E, float sf)   { return (1-sf)/((1-E)*(1-E)); }
float dfaildsf2(float E, float sf)  { return dfaildsf(E,sf)*dfaildsf(E,sf); }

float passIdIso_passIdIso(float S1, float dS1, float D1, float dD1, float S2, float dS2, float D2, float dD2) {
  return S1*D1*S2*D2*sqrt(((dS1/S1+dS2/S2)*(dS1/S1+dS2/S2) + (dD1/D1+dD2/D2)*(dD1/D1+dD2/D2)));
}
float passIdIso_passIdFailIso(float D1, float dD1, float S1, float dS1, float D2, float dD2, float E2, float dE2, float S2, float dS2) {
  //std::cout <<   "dSF*2 " << fail2(E2,S2)*( D2*D2*S1*S1*dD1*dD1 + D1*D1*S1*S1*dD2*dD2 + D1*D1*D2*D2*dS1*dS1 + 2*S1*S1*D1*D2*dD1*dD2 )
  //          << " + " << D1*D1*D2*D2*S1*S1
  //          << " * ( " << dfaildsf2(E2,S2)*dS2*dS2 << " + " << dfaildE2(E2)*dE2*dE2 << " + " << 2*fail(E2,S2)*dfaildsf(E2,S2)*dS1*dS2/S1 << " )"
  //          << std::endl;
  return sqrt(( fail2(E2,S2)*( D2*D2*S1*S1*dD1*dD1 + D1*D1*S1*S1*dD2*dD2 + D1*D1*D2*D2*dS1*dS1 + 2*S1*S1*D1*D2*dD1*dD2 )
             + D1*D1*D2*D2*S1*S1*( dfaildsf2(E2,S2)*dS2*dS2 + dfaildE2(E2)*dE2*dE2 + 2*fail(E2,S2)*dfaildsf(E2,S2)*dS1*dS2/S1 ) ));
}
float passIdIso_failId(float D1, float dD1, float S1, float dS1, float E2, float dE2, float D2, float dD2) {
  return sqrt(( fail2(E2,D2)*( S1*S1*dD1*dD1 + D1*D1*dS1*dS1 )
             + D1*D1*S1*S1*( dfaildE2(E2)*dE2*dE2 + dfaildsf2(E2,D2)*dD2*dD2 )
             + 2*S1*S1*D1*fail(E2,D2)*dD1*dD2 ));
}
float passIdFailIso_passIdFailIso(float D1, float dD1, float E1, float dE1, float S1, float dS1, float D2, float dD2, float E2, float dE2, float S2, float dS2) {
  return sqrt(( fail2(E1,S1)*fail2(E2,S2)*( D1*D1*dD2*dD2 + D2*D2*dD1*dD1 + 2*D1*D1*dD1*dD2 )
             + D1*D1*D2*D2*fail2(E2,S2)*( dfaildsf2(E1,S1)*dS1*dS1 + dfaildE2(E1)*dE1*dE1 )
             + D1*D1*D2*D2*fail2(E1,S1)*( dfaildsf2(E2,S2)*dS2*dS2 + dfaildE2(E2)*dE2*dE2 )
             + 2*D1*D1*D2*D2*fail(E1,S1)*fail(E2,S2)*dfaildsf(E1,S1)*dfaildsf(E2,S2)*dS1*dS2 ));
}
float passIdFailIso_failID(float D1, float dD1, float E1, float dE1, float S1, float dS1, float E2, float dE2, float D2, float dD2) {
  return sqrt(( fail2(E1,S1)*fail2(E2,D2)*dD1*dD1
             + D1*D1*fail2(E2,D2)*( dfaildE2(E1)*dE1*dE1 + dfaildsf2(E1,S1)*dS1*dS1 )
             + D1*D1*fail2(E1,S1)*( dfaildE2(E2)*dE2*dE2 + dfaildsf2(E2,D2)*dD2*dD2 )
             + 2*fail2(E1,S1)*D1*fail(E2,D2)*dfaildsf(E2,D2)*dD1*dD2 ));
}
float failId_failId(float E1, float dE1, float D1, float dD1, float E2, float dE2, float D2, float dD2) {
  return sqrt(( fail2(E2,D2)*( dfaildsf2(E1,D1)*dD1*dD1 + dfaildE2(E1)*dE1*dE1 )
             + fail2(E1,D1)*( dfaildsf2(E2,D2)*dD2*dD2 + dfaildE2(E2)*dE2*dE2 )
             + 2*fail(E1,D1)*fail(E2,D2)*dfaildsf(E1,D1)*dfaildsf(E2,D2)*dD1*dD2 ));
}


float TnPCorr::getLepWeight(LeptonF* lep, CORRTYPE elCorrType, CORRTYPE muCorrType, float &D, float &dD, float &ED, float &dED, float &S, float &dS, float &ES, float &dES, bool &passId, bool &passIdIso ) const {
  int   id  = lep->pdgid();
  float pt  = lep->pt();
  float eta = lep->absEta();
  float annulus = lep->annulusactivity();
  if     (id==11 && elCorrType == NONE) return 1.0;
  else if(id==13 && muCorrType == NONE) return 1.0;
  else if(id==11 && (!cfgSet::isSelElectron(*(ElectronF*)lep,elConfKin))) return 1.0;
  else if(id==13 && (!cfgSet::isSelMuon(*(MuonF*)lep        ,muConfKin))) return 1.0;
  passId    = false;
  passIdIso = false;
  if(id==11){
    D   = getElIDValue(pt,eta);
    dD  = getElIDError(pt,eta);
    S   = getElIsoValue(pt,annulus);
    dS  = getElIsoError(pt,annulus);
    ED  = getElMCIdEffValue(pt,eta);
    dED = getElMCIdEffError(pt,eta);
    ES  = getElMCIsoEffValue(pt,annulus);
    dES = getElMCIsoEffError(pt,annulus);
    if(cfgSet::isSelElectron(*(ElectronF*)lep, elConfNoIso)) passId    = true;
    if(cfgSet::isSelElectron(*(ElectronF*)lep, elConf)     ) passIdIso = true;
    //std::cout << " \t\t\t\t\t D = " << D << "\tS =" << S << "\tel" << std::endl;
  }
  else if(id==13) {
    D   = getMuIDValue(pt,eta);
    dD  = D*0.01;
    S   = getMuIsoValue(pt,annulus);
    dS  = S*0.01;
    ED  = getMuMCIdEffValue(pt,eta);
    dED = getMuMCIdEffError(pt,eta);
    ES  = getMuMCIsoEffValue(pt,annulus);
    dES = getMuMCIsoEffError(pt,annulus);
    if(cfgSet::isSelMuon(*(MuonF*)lep, muConfNoIso)) passId    = true;
    if(cfgSet::isSelMuon(*(MuonF*)lep, muConf)     ) passIdIso = true;
    //std::cout << " \t\t\t\t\t D = " << D << "\tS =" << S << "\tmu" << std::endl;
  }
  if(ED>=1.0) throw std::invalid_argument("LeptonCorectionSet::TnPCorr: ID eff is >=1!");
  if(ES>=1.0) throw std::invalid_argument("LeptonCorectionSet::TnPCorr: Iso eff is >=1!");
  float wt = 1.0;
  if     (passIdIso) { wt = D*S; /*std::cout << " \t\t\t\t\t D = " << D << "\tS =" << S << std::endl;*/ }
  else if(passId)    { wt = D*fail(ES,S); /*std::cout << "  \t\t\t\t\t D = " << D << "\tfail =" << fail(ES,S) << std::endl; */}
  else {
    if(id==13) wt  = 1.0; // this piece will come from gen muons
    else       {wt = fail(ED,D);  /*std::cout << "  \t\t\t\t\t fail =" << fail(ES,S) << std::endl;*/ }
  }
  if(wt < -2.0) wt = -2.0; // don't want large negative weights ... need to treat these cases better

  return wt;
}

float TnPCorr::getGenLepWeight(const GenParticleF* lep, CORRTYPE muCorrType, float &D, float &dD, float &E, float &dE ) const {
  if(muCorrType == NONE) return 1.0;
  float pt  = lep->pt();
  float eta = lep->absEta();
  //std::cout << "                    gen (pt,eta) = " << pt << "\t" << eta << std::endl;
  if     (pt <muConfKin.minPT)  return 1.0;
  else if(eta>muConfKin.maxETA) return 1.0;
  D  = getMuIDValue(pt,eta);
  dD = D*0.01;
  E  = getMuMCIdEffValue(pt,eta);
  dE = getMuMCIdEffError(pt,eta);
  if(E>=1.0) throw std::invalid_argument("LeptonCorectionSet::TnPCorr: gen muon ID eff is >=1!");
  float wt = 1.0*fail(E,D);
  //std::cout << "  \t\t\t\t\t fail =" << fail(E,D) << "\tgen mu" << std::endl;
  if(wt < -2.0) wt = -2.0; // don't want large negative weights ... need to treat these cases better
  return wt;
}


float TnPCorr::getEvtWeight(const std::vector<LeptonF*>& allLeptons, const std::vector<LeptonF*>& selectedLeptons, const std::vector<GenParticleF*> genParts, CORRTYPE elCorrType, CORRTYPE muCorrType ) const {
  if( (muCorrType==UP && elCorrType==DOWN) || (muCorrType==DOWN && elCorrType==UP) )
    throw std::invalid_argument("LeptonCorectionSet::TnPCorr: If electrons and muons are varied simultaneously, it must be in the same direction!");
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
  int nLeps = 0;
  //float D1=1, dD1=0, ED1=.95, dED1=0, S1=1, dS1=0, ES1=.95, dES1=0;
  //float D2=1, dD2=0, ED2=.95, dED2=0, S2=1, dS2=0, ES2=.95, dES2=0;
  float D1=1, dD1=0, ED1=0, dED1=0, S1=1, dS1=0, ES1=0, dES1=0;
  float D2=1, dD2=0, ED2=0, dED2=0, S2=1, dS2=0, ES2=0, dES2=0;
  float lep1pt=0, lep2pt=0; // use to patch gen muons into pt ordering
  float x,x1,x2,x3,x4,x5,x6,x7; // dummy vars to get weight when
  bool  y1,y2;                  // we don't need the individual pieces
  bool passId1, passIdIso1, passId2, passIdIso2;
  float weight = 1.0;
  for(auto* lep : allLeptons) {
    int id = lep->pdgid();
    if     (id==11 && elCorrType == NONE) continue;
    else if(id==13 && muCorrType == NONE) continue;
    else if(id==11 && (!cfgSet::isSelElectron(*(ElectronF*)lep,elConfKin))) continue;
    else if(id==13 && (!cfgSet::isSelMuon(*(MuonF*)lep        ,muConfKin))) continue;
    double nearDR = 0;
    int near = -1;
    if     (id==11) near = PhysicsUtilities::findNearestDRDeref(*lep, genEl_, nearDR, 0.4);
    else if(id==13) near = PhysicsUtilities::findNearestDRDeref(*lep, genMu_, nearDR, 0.4);
    if(near >= 0) {
      weight *= getLepWeight(lep,elCorrType,muCorrType,x,x1,x2,x3,x4,x5,x6,x7,y1,y2);
      //std::cout << "reco wt = " << tmp << "\t\t" << id << std::endl;
      if( (id==11 && (elCorrType==UP || elCorrType==DOWN)) || (id==13 && (muCorrType==UP || muCorrType==DOWN)) ) {
        // this should be ok to vary both electrons and muons at the same
        // time, but they need to be varied in the same direction!!!
        ++nLeps;
        if     (nLeps==1) { x = getLepWeight(lep,elCorrType,muCorrType,D1,dD1,ED1,dED1,S1,dS1,ES1,dES1,passId1,passIdIso1); lep1pt = lep->pt(); }
        else if(nLeps==2) { x = getLepWeight(lep,elCorrType,muCorrType,D2,dD2,ED2,dED2,S2,dS2,ES2,dES2,passId2,passIdIso2); lep2pt = lep->pt(); }
        // get this next piece from unmatched gen muons since the loosest muons we
        // as what the SF group used to get the ID SFs save are not as inclusive
        if(nLeps==1 && id==13 && !passId1) --nLeps;
        if(nLeps==2 && id==13 && !passId2) --nLeps;
      } // get unc for this lep
    } // matched lep to gen lep
  } // allLeptons
  if(muCorrType != NONE) {
    std::vector<const LeptonF*> recMu_;
    for(const auto* lep : allLeptons) if(lep->pdgid()==13) recMu_.push_back(lep);
    for(auto* lep : genMu_) {
      if(lep->pt() < muConfKin.minPT || lep->absEta() > muConfKin.maxETA) continue;
      double nearDR = 0;
      int near = PhysicsUtilities::findNearestDRDeref(*lep, recMu_, nearDR, 0.4);
      if(near<0 || !cfgSet::isSelMuon(*(MuonF*)recMu_[near], muConfNoIso)) {
        weight *= getGenLepWeight(lep,muCorrType,x,x1,x2,x3);
        //std::cout << "gen wt = " << tmp << std::endl;
        if(muCorrType==UP || muCorrType==DOWN) {
          // only use this muon for the correction if it's higher pt than any (potential) reco leptons being varried
          int muNum = 99;
          if(lep->pt() > lep1pt) {
           D2=D1; dD2=dD1; ED2=ED1; dED2=dED1; S2=S1; dS2=dS1; ES2=ES1; dES2=dES1;
           passId2=passId1; passIdIso2=passIdIso1;
           lep2pt = lep1pt;
           lep1pt = lep->pt();
           muNum = 1;
           ++nLeps;
          }
          else if(lep->pt() > lep2pt) {
            muNum = 2;
            lep2pt = lep->pt();
            ++nLeps;
          }
          else {
            ++nLeps;
            continue;
          }
          if     (muNum==1) { x = getGenLepWeight(lep,muCorrType,D1,dD1,ED1,dED1); passId1=false; passIdIso1=false; }
          else if(muNum==2) { x = getGenLepWeight(lep,muCorrType,D2,dD2,ED2,dED2); passId2=false; passIdIso2=false; }
        } // get unc for this lepton
      } // unmatch muon (or matched to muon that does NOT pass isolation)
    } // gen muons
  } // muCorrType != NONE
  float unc = 0;
  if(nLeps==0) unc = 0.0;
  else if(nLeps==1) {
    if(passIdIso1)   unc = D1*S1*sqrt(dD1*dD1/(D1*D1) + dS1*dS1/(S1*S1));
    else if(passId1) unc = sqrt((fail2(ES1,S1)*dD1*dD1 + D1*D1*dfaildE2(ES1)*dES1*dES1 + D1*D1*dfaildsf2(ES1,S1)*dS1*dS1));
    else             unc = sqrt(dfaildsf2(ED1,D1)*dD1*dD1 + dfaildE2(ED1)*dED1*dED1);
  } // nLeps==1
  else if(nLeps>=2) {
    if(passIdIso1){
      if(passIdIso2)   unc = passIdIso_passIdIso(S1,dS1,D1,dD1,S2,dS2,D2,dD2);
      else if(passId2) unc = passIdIso_passIdFailIso(D1,dD1,S1,dS1,D2,dD2,ES2,dES2,S2,dS2);
      else             unc = passIdIso_failId(D1,dD1,S1,dS1,ED2,dED2,D2,dD2);
    }
    else if(passId1) {
      if(passIdIso2)   unc = passIdIso_passIdFailIso(D2,dD2,S2,dS2,D1,dD1,ES1,dES1,S1,dS1);
      else if(passId2) unc = passIdFailIso_passIdFailIso(D1,dD1,ES1,dES1,S1,dS1,D2,dD2,ES2,dES2,S2,dS2);
      else             unc = passIdFailIso_failID(D1,dD1,ES1,dES1,S1,dS1,ED2,dED2,D2,dD2);
    }
    else {
      if(passIdIso2)   unc = passIdIso_failId(D2,dD2,S2,dS2,ED1,dED1,D1,dD1);
      else if(passId2) unc = passIdFailIso_failID(D2,dD2,ES2,dES2,S2,dS2,ED1,dED1,D1,dD1);
      else             unc = failId_failId(ED1,dED1,D1,dD1,ED2,dED2,D2,dD2);
    }
  } // nLeps>=2
  if(nLeps>0) {
  std::cout << "allLeps.size=" << allLeptons.size() << " nLeps=" << nLeps << " IdIso1 " << passIdIso1 << " Id1 " << passId1 << " IdIso2 " << passIdIso2 << " Id2 " << passId2
            << " wt = " << weight << "\tunc = " << unc
            //<< "\n                         " << D1 <<"\t"<< dD1 <<"\t"<< ED1 <<"\t"<< dED1 <<"\t"<< S1 <<"\t"<< dS1 <<"\t"<< ES1 <<"\t"<< dES1 << "\n"
            //<<   "                         " << D2 <<"\t"<< dD2 <<"\t"<< ED2 <<"\t"<< dED2 <<"\t"<< S2 <<"\t"<< dS2 <<"\t"<< ES2 <<"\t"<< dES2
            << std::endl;
  }
  if     ( muCorrType == UP   || elCorrType == UP  ) { weight += unc; }
  else if( muCorrType == DOWN || elCorrType == DOWN) { weight -= unc; }
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
