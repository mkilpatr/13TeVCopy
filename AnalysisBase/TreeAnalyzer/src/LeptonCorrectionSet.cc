//
// strategy as of jan 30 2017
// by event reweighting
// recommendations are per
//   (data/fullsim) https://twiki.cern.ch/twiki/bin/view/CMS/SUSLeptonSF
//   (fullsim/fastsim) https://twiki.cern.ch/twiki/bin/view/CMS/SUSLeptonSFMC
//
// see getLepWeight section for details of current recommendations, and how we apply all the SFs/Effs/Systematics
// search for #CHANGENAMES for locations of file names / histogram names for updating
//
// "TnPCorr": corrects e/mu effs
// ------------------------------
//
// lepton control region (inverted veto): 
//   evt weight "lepselweight" = product over each selected lepton of [SF] ~ P(DATA)/P(MC) with effs cancelling
// veto region: 
//   evt weight "lepvetoweight" = product over each lep failing ID of [ (1-eff(ID)*SF(ID))/(1-eff(ID)) ]
//                               *product over each lep passing ID but failing ISO of [ SF_ID*(1-eff(ISO)*SF(ISO))/(1-eff(ISO)) ]
// where effs are measured in CRs (>=1 gen lep) of ttbarplusw MC and SFs are provided centrally.
// these effs are different enough after HM / LM baselines that we split them.
//
// "LepCorr": corrects tau effs
// ------------------------------
//
//  evt weight "leptnpweight" = [ Data(CR_tau) - SF_NORM*MC(CR_tau,0gentau) ]/[ SF_NORM*MC(CR_tau,1gentau) ]
//  measured in a CR_tau = at least one reco tau, zero reco el/mu.
//  different enough after HM / LM baseline that we split them.
//
// ------------------------------
//  for leps:
//    MC_corr(SR) = SF*MC(SR)
//  for taus:
//    MC_corr(SR) = MC(SR) + (1-Corr_tau)*MC(CR_tau,1gentau)
//  achieve both using cutstring
//    lepselweight*lepvetoweight*leptnpweight*( (nvetolep==0) && (nvetotau==0 || (ismc && npromptgentau>0)) )
//  focusing on tau part:
//    leptnpweight*(nvetotau==0 || (ismc && npromptgentau>0))
//                            = 1*(nvetotau == 0) for data
//                            OR
//                            = 1*(nvetotau == 0) + (1-Corr_tau)*(nvetotau>=1 && npromptgentau>0) 
//                            = 1*MC(SR) + (1-Corr_tau)*MC(CR_tau, 1gentau) for MC as promised
//
//

#include "AnalysisBase/TreeAnalyzer/interface/BaseTreeAnalyzer.h"
#include "AnalysisBase/TreeAnalyzer/interface/DefaultProcessing.h"
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"
#include "TFile.h"

namespace ucsbsusy {


// load the needed files & histos
LepCorr::LepCorr() : Correction("LEP") {
  bool dbg = false;
  if(dbg) std::cout << "[LeptonCorrectionSet::LepCorr::LepCorr]" << std::endl;

  //#CHANGENAMES
  TString strFileLepLM   =  TString::Format("%s/src/data/corrections/2017/lep/lepCorr_LM.root",getenv("CMSSW_BASE"));
  TString strFileLepHM   =  TString::Format("%s/src/data/corrections/2017/lep/lepCorr_HM.root",getenv("CMSSW_BASE"));

  fileLepLM              = TFile::Open(strFileLepLM,"read");
  fileLepHM              = TFile::Open(strFileLepHM,"read");
  if(!fileLepLM)           throw std::invalid_argument("LeptonCorectionSet::LepCorr: Lep LM file could not be found!");
  if(!fileLepHM)           throw std::invalid_argument("LeptonCorectionSet::LepCorr: Lep HM file could not be found!");

  TString strHistLepLM   = "LEP";
  TString strHistLepHM   = "LEP";
  histLepLM              = (TH1F*)(fileLepLM->Get(strHistLepLM));
  histLepHM              = (TH1F*)(fileLepHM->Get(strHistLepHM));
  if(!histLepLM)           throw std::invalid_argument("LeptonCorectionSet::LepCorr: Lep LM hist could not be found!");
  if(!histLepHM)           throw std::invalid_argument("LeptonCorectionSet::LepCorr: Lep HM hist could not be found!");
}

LepCorr::~LepCorr() {
  // close then delete file ptrs?
}

// load the needed configs, files, & histos
// change any file or histo names in this function
TnPCorr::TnPCorr(const LeptonSelection::Electron elSel, const LeptonSelection::Electron secElSel,
                 const LeptonSelection::Muon     muSel, const LeptonSelection::Muon     secMuSel
                 ) : Correction("TNP") {
  bool dbg = false;
  if(dbg) std::cout << "[LeptonCorrectionSet::TnPCorr::TnPCorr]" << std::endl;

  /* // sanity checks, I can't get the logic correct. with our FW structure we can't violate anything here.
  // check these primary/secondary lep configs. must be 0l (SEL) or 1l (CTR) and equal.
  if(    (   elSel.type != LeptonSelection::ZL_SEL_ELE &&    elSel.type != LeptonSelection::ZL_CTR_ELE) || // el are 0l or 1l?
         (secElSel.type != LeptonSelection::ZL_SEL_ELE && secElSel.type != LeptonSelection::ZL_CTR_ELE) ||
         (   muSel.type != LeptonSelection::ZL_SEL_MU  &&    muSel.type != LeptonSelection::ZL_CTR_MU ) || // mu are 0l or 1l?
         (secMuSel.type != LeptonSelection::ZL_SEL_MU  && secMuSel.type != LeptonSelection::ZL_CTR_MU ) ||
         (   elSel.type != secElSel.type)                                                               || // el primary/secondary are same?
         (   muSel.type != secMuSel.type)                                                               ){ // mu primary/secondary are same?
    throw std::invalid_argument("LeptonCorectionSet::TnPCorr: el/mu primary/secondary configs must be equal and either ZL_SEL_X (veto & inverted veto regions) or ZL_CTR_X (quality single-lepton region).");
  }
  */

  isCR = (muSel.type == LeptonSelection::ZL_CTR_MU);
  assert(isCR == (elSel.type == LeptonSelection::ZL_CTR_ELE)); // do ELE and MU agree on isCR?

  // load lepton configs and make configs: ConfKin (neither Id nor Iso), ConfNoIso (Id but no iso), and the main Conf (Id and Iso). 
  elConf              = elSel; 
  elConfKin           = elSel;
  elConfNoIso         = elSel;
  elConfKin.minPT     = std::min( elSel.minPT,  secElSel.minPT  ); // use loosest pt/eta between primary/secondary configs
  elConfKin.maxETA    = std::max( elSel.maxETA, secElSel.maxETA );
  elConfKin.passID    = &ElectronID::inclusive;
  elConfKin.passISO   = &ElectronISO::inclusive;
  elConfNoIso.minPT   = elConfKin.minPT  ; // NB: reusing those loosest pt/eta
  elConfNoIso.maxETA  = elConfKin.maxETA ;
  elConfNoIso.passISO = &ElectronISO::inclusive;

  // same for muons
  muConf              = muSel;
  muConfKin           = muSel;
  muConfNoIso         = muSel;
  muConfKin.minPT     = std::min( muSel.minPT,  secMuSel.minPT  );
  muConfKin.maxETA    = std::max( muSel.maxETA, secMuSel.maxETA );
  muConfKin.passID    = &MuonID::inclusive;
  muConfKin.passISO   = &MuonISO::inclusive;
  muConfNoIso.minPT   = muConfKin.minPT  ;
  muConfNoIso.maxETA  = muConfKin.maxETA ;
  muConfNoIso.passISO = &MuonISO::inclusive;


  TString oldBaseDir = TString::Format("%s/src/data/corrections/2016/lepCorMCEffsAndSFs/",getenv("CMSSW_BASE"));
  TString baseDir    = TString::Format("%s/src/data/corrections/2017/lep/",getenv("CMSSW_BASE"));

  // load electron files, different for 0l/1l configs
  // ----------------------------------------------------------------------------------------------------------------
  //#CHANGENAMES
  TString strElSfIdIso                 = "scaleFactors_BtoH_36p4_Moriond17.root";
  TString strElSfTracker               = "egammaEffi.txt_EGM2D_Moriond17.root";
  //TString strElSfFullFastId            = (isCR) ? "sf_el_mediumCB.root"           : "sf_el_vetoCB.root";
  TString strElSfFullFastIdIso         = (isCR) ? "sf_el_mediumCB_mini01.root"    : "sf_el_vetoCB_mini01.root"; // for 1lep Ctr, ele is med miniIso (0.2) rather... no bother for now.
  TString strElMCEffsLM                = "lepCorMCEff_LM.root";
  TString strElMCEffsHM                = "lepCorMCEff_HM.root";
  //TString strElMCEffsLMId              = "lepCorMCEff_LM_El_Id_" +TString(isCR ? "CR.root" : "SR.root");
  //TString strElMCEffsLMIso             = "lepCorMCEff_LM_El_Iso_"+TString(isCR ? "CR.root" : "SR.root");
  //TString strElMCEffsHMId              = "lepCorMCEff_HM_El_Id_" +TString(isCR ? "CR.root" : "SR.root");
  //TString strElMCEffsHMIso             = "lepCorMCEff_HM_El_Iso_"+TString(isCR ? "CR.root" : "SR.root");

  fileElSf                             = TFile::Open(baseDir + strElSfIdIso,"read"); // for el, contains id & iso hists
  fileElSfTracker                      = TFile::Open(baseDir + strElSfTracker,"read");
  //fileElSfFullFastId                   = TFile::Open(baseDir + "/fastsim/"+strElSfFullFastId,"read");
  fileElSfFullFastIdIso                = TFile::Open(baseDir + "/fastsim/"+strElSfFullFastIdIso,"read");
  fileElMCEffsLM                       = TFile::Open(baseDir + strElMCEffsLM,"read");
  fileElMCEffsHM                       = TFile::Open(baseDir + strElMCEffsHM,"read");
  //fileElMCEffsLMId                     = TFile::Open(oldBaseDir + strElMCEffsLMId,"read");
  //fileElMCEffsHMId                     = TFile::Open(oldBaseDir + strElMCEffsHMId,"read");
  //fileElMCEffsLMIso                    = TFile::Open(oldBaseDir + strElMCEffsLMIso,"read");
  //fileElMCEffsHMIso                    = TFile::Open(oldBaseDir + strElMCEffsHMIso,"read");
  if(!fileElSf)                        throw std::invalid_argument("LeptonCorectionSet::TnPCorr: ele SF file could not be found!");
  if(!fileElSfTracker)                 throw std::invalid_argument("LeptonCorectionSet::TnPCorr: ele SF tracker file could not be found!");
  //if(!fileElSfFullFastId)              throw std::invalid_argument("LeptonCorectionSet::TnPCorr: ele SF fullfast id file could not be found!");
  if(!fileElSfFullFastIdIso)           throw std::invalid_argument("LeptonCorectionSet::TnPCorr: ele SF fullfast iso file could not be found!");
  if(!fileElMCEffsLM)                  throw std::invalid_argument("LeptonCorectionSet::TnPCorr: ele MC lm effs file could not be found!");
  if(!fileElMCEffsHM)                  throw std::invalid_argument("LeptonCorectionSet::TnPCorr: ele MC hm effs file could not be found!");
  //if(!fileElMCEffsLMId)                throw std::invalid_argument("LeptonCorectionSet::TnPCorr: ele MC lm effs id file could not be found!");
  //if(!fileElMCEffsLMIso)               throw std::invalid_argument("LeptonCorectionSet::TnPCorr: ele MC lm effs iso file could not be found!");
  //if(!fileElMCEffsHMId)                throw std::invalid_argument("LeptonCorectionSet::TnPCorr: ele MC hm effs id file could not be found!");
  //if(!fileElMCEffsHMIso)               throw std::invalid_argument("LeptonCorectionSet::TnPCorr: ele MC hm effs iso file could not be found!");

  // load electron histos from electron files
  //#CHANGENAMES
  TString strHistElSfId                = (isCR) ? "GsfElectronToCutBasedSpring15M"  : "GsfElectronToCutBasedSpring15V";
  TString strHistElSfIso               = (isCR) ? "MVAVLooseElectronToMini2" : "MVAVLooseElectronToMini2";
  TString strHistElSfTracker           = "EGamma_SF2D";
  TString strHistElSfFullFastIdIso     = "histo3D";
  TString strHistElMCEffs              = "lepCorMCEff_El_"; // lepCorMCEff_El_Id NB: plus "Id"/"Iso" below

  histElSfId                           = (TH2F*)(fileElSf->Get(strHistElSfId));
  histElSfIso                          = (TH2F*)(fileElSf->Get(strHistElSfIso));
  histElSfTracker                      = (TH2F*)(fileElSfTracker->Get(strHistElSfTracker));
  //histElSfFullFastId                   = (TH2F*)(fileElSfFullFastId ->Get(strHistElSfFullFastIdIso));
  histElSfFullFastIdIso                = (TH3D*)(fileElSfFullFastIdIso->Get(strHistElSfFullFastIdIso));
  histElMCEffsLMId                     = (TH2F*)(fileElMCEffsLM->Get(strHistElMCEffs+"Id_"+(isCR ? "CR" : "SR"))); // format lepCorMCEff_Mu_Iso_CR
  histElMCEffsLMIso                    = (TH2F*)(fileElMCEffsLM->Get(strHistElMCEffs+"Iso_"+(isCR ? "CR" : "SR")));
  histElMCEffsHMId                     = (TH2F*)(fileElMCEffsHM->Get(strHistElMCEffs+"Id_"+(isCR ? "CR" : "SR")));
  histElMCEffsHMIso                    = (TH2F*)(fileElMCEffsHM->Get(strHistElMCEffs+"Iso_"+(isCR ? "CR" : "SR")));
  //histElMCEffsLMId                     = (TH2F*)(fileElMCEffsLMId->Get(strHistElMCEffs+"Id")); // format lepCorMCEff_Mu_Iso_CR
  //histElMCEffsLMIso                    = (TH2F*)(fileElMCEffsLMIso->Get(strHistElMCEffs+"Iso"));
  //histElMCEffsHMId                     = (TH2F*)(fileElMCEffsHMId->Get(strHistElMCEffs+"Id"));
  //histElMCEffsHMIso                    = (TH2F*)(fileElMCEffsHMIso->Get(strHistElMCEffs+"Iso"));
  if(!histElSfId)                      throw std::invalid_argument("LeptonCorrectionSet::TnpCorr: ele SF id histo not found!");
  if(!histElSfIso)                     throw std::invalid_argument("LeptonCorrectionSet::TnpCorr: ele SF iso histo not found!");
  if(!histElSfTracker)                 throw std::invalid_argument("LeptonCorrectionSet::TnpCorr: ele SF tracker histo not found!");
  //if(!histElSfFullFastId)              throw std::invalid_argument("LeptonCorrectionSet::TnpCorr: ele SF fullfast id histo not found!");
  if(!histElSfFullFastIdIso)           throw std::invalid_argument("LeptonCorrectionSet::TnpCorr: ele SF fullfast iso histo not found!");
  if(!histElMCEffsLMId)                throw std::invalid_argument("LeptonCorrectionSet::TnpCorr: ele SF lm id histo not found!");
  if(!histElMCEffsLMIso)               throw std::invalid_argument("LeptonCorrectionSet::TnpCorr: ele SF lm iso histo not found!");
  if(!histElMCEffsHMId)                throw std::invalid_argument("LeptonCorrectionSet::TnpCorr: ele SF hm id histo not found!");
  if(!histElMCEffsHMIso)               throw std::invalid_argument("LeptonCorrectionSet::TnpCorr: ele SF hm iso histo not found!");

  // load muon files, different for 0l/1l configs
  // ----------------------------------------------------------------------------------------------------------------
  //#CHANGENAMES
  TString strMuSfId                    = (isCR) ? "TnP_NUM_MediumID_DENOM_generalTracks_VAR_map_pt_eta.root" : // LooseID (0l), MediumID (1l)
                                                  "TnP_NUM_LooseID_DENOM_generalTracks_VAR_map_pt_eta.root";
  TString strMuSfIso                   = (isCR) ? "TnP_NUM_MiniIsoTight_DENOM_MediumID_VAR_map_pt_eta.root" : // same miniIso for 0l/1l over diff IDs
                                                  "TnP_NUM_MiniIsoTight_DENOM_LooseID_VAR_map_pt_eta.root";
  TString strMuSfIp2d                  = (isCR) ? "TnP_NUM_TightIP2D_DENOM_MediumID_VAR_map_pt_eta.root" : // MediumIp2D (0l), TightIp2D(1l) over diff IDs
                                                  "TnP_NUM_MediumIP2D_DENOM_LooseID_VAR_map_pt_eta.root";
  TString strMuSfTracker               = "dummyMuSfTracker.root"; // "general_tracks_and_early_general_tracks_corr_ratio.root"; // NOT YET AVAILABLE (Jan 30). created dummy with SF 1 err 0
  //TString strMuSfFullFastId            = (isCR) ? "sf_mu_medium.root"             : "sf_mu_loose.root";
  TString strMuSfFullFastIdIso         = (isCR) ? "sf_mu_mediumID_mini02.root"    : "sf_mu_looseID_mini02.root";
  //TString strMuSfFullFastIp2d          = (isCR) ? "sf_mu_tightIP2D.root"          : "sf_mu_looseIP2D.root";
  TString strMuMCEffsLM                = "lepCorMCEff_LM.root";
  TString strMuMCEffsHM                = "lepCorMCEff_HM.root";
  //TString strMuMCEffsLMId              = "lepCorMCEff_LM_Mu_Id_" +TString(isCR ? "CR.root" : "SR.root");
  //TString strMuMCEffsLMIso             = "lepCorMCEff_LM_Mu_Iso_"+TString(isCR ? "CR.root" : "SR.root");
  //TString strMuMCEffsHMId              = "lepCorMCEff_HM_Mu_Id_" +TString(isCR ? "CR.root" : "SR.root");
  //TString strMuMCEffsHMIso             = "lepCorMCEff_HM_Mu_Iso_"+TString(isCR ? "CR.root" : "SR.root");

  fileMuSfId                           = TFile::Open(baseDir + strMuSfId,"read");
  fileMuSfIso                          = TFile::Open(baseDir + strMuSfIso,"read");
  fileMuSfIp2d                         = TFile::Open(baseDir + strMuSfIp2d,"read");
  fileMuSfTracker                      = TFile::Open(baseDir + strMuSfTracker,"read");
  //fileMuSfFullFastId                   = TFile::Open(baseDir + "/fastsim/"+strMuSfFullFastId,"read");
  fileMuSfFullFastIdIso                = TFile::Open(baseDir + "/fastsim/"+strMuSfFullFastIdIso,"read");
  //fileMuSfFullFastIp2d                 = TFile::Open(baseDir + "/fastsim/"+strMuSfFullFastIp2d,"read");
  fileMuMCEffsLM                       = TFile::Open(baseDir + strMuMCEffsLM,"read");
  fileMuMCEffsHM                       = TFile::Open(baseDir + strMuMCEffsHM,"read");
  //fileMuMCEffsLMId                     = TFile::Open(oldBaseDir + strMuMCEffsLMId,"read");
  //fileMuMCEffsHMId                     = TFile::Open(oldBaseDir + strMuMCEffsHMId,"read");
  //fileMuMCEffsLMIso                    = TFile::Open(oldBaseDir + strMuMCEffsLMIso,"read");
  //fileMuMCEffsHMIso                    = TFile::Open(oldBaseDir + strMuMCEffsHMIso,"read");
  if(!fileMuSfId)                      throw std::invalid_argument("LeptonCorectionSet::TnPCorr: mu sf id file could not be found!");
  if(!fileMuSfIso)                     throw std::invalid_argument("LeptonCorectionSet::TnPCorr: mu sf iso file could not be found!");
  if(!fileMuSfIp2d)                    throw std::invalid_argument("LeptonCorectionSet::TnPCorr: mu sf ip2d file could not be found!");
  //if(!fileMuSfFullFastId)              throw std::invalid_argument("LeptonCorectionSet::TnPCorr: mu sf fullfast id file could not be found!");
  if(!fileMuSfFullFastIdIso)           throw std::invalid_argument("LeptonCorectionSet::TnPCorr: mu sf fullfast iso file could not be found!");
  //if(!fileMuSfFullFastIp2d)            throw std::invalid_argument("LeptonCorectionSet::TnPCorr: mu sf fullfast ip2d file could not be found!");
  if(!fileMuSfTracker)                 throw std::invalid_argument("LeptonCorectionSet::TnPCorr: mu sf tracker file could not be found!");
  if(!fileMuMCEffsLM)                  throw std::invalid_argument("LeptonCorectionSet::TnPCorr: mu mc eff lm file could not be found!");
  if(!fileMuMCEffsHM)                  throw std::invalid_argument("LeptonCorectionSet::TnPCorr: mu mc eff hm file could not be found!");
  //if(!fileMuMCEffsLMId)                throw std::invalid_argument("LeptonCorectionSet::TnPCorr: mu MC lm effs id file could not be found!");
  //if(!fileMuMCEffsLMIso)               throw std::invalid_argument("LeptonCorectionSet::TnPCorr: mu MC lm effs iso file could not be found!");
  //if(!fileMuMCEffsHMId)                throw std::invalid_argument("LeptonCorectionSet::TnPCorr: mu MC hm effs id file could not be found!");
  //if(!fileMuMCEffsHMIso)               throw std::invalid_argument("LeptonCorectionSet::TnPCorr: mu MC hm effs iso file could not be found!");

  // load muon histos
  //#CHANGENAMES
  TString strHistMuSfId                = "SF"; // yup
  TString strHistMuSfIso               = "SF";
  TString strHistMuSfIp2d              = "SF";
  TString strHistMuSfTrackerGt10       = "mutrksfptg10"; // pt>10 tracker corrections
  TString strHistMuSfTrackerLt10       = "mutrksfptl10"; // pt<10
  //TString strHistMuSfFullFastId        = "histo2D";
  TString strHistMuSfFullFastIdIso     = "histo3D";
  //TString strHistMuSfFullFastIp2d      = "histo2D"; // hey! that's the combination to my luggage!
  TString strHistMuMCEffs              = "lepCorMCEff_Mu_"; //NB: plus "Id"/"Iso" below

  histMuSfId                           = (TH2F*)(fileMuSfId->Get(strHistMuSfId));
  histMuSfIso                          = (TH2F*)(fileMuSfIso->Get(strHistMuSfIso));
  histMuSfIp2d                         = (TH2F*)(fileMuSfIp2d->Get(strHistMuSfIp2d));
  histMuSfTrackerGt10                  = (TH1F*)(fileMuSfTracker->Get(strHistMuSfTrackerGt10));
  histMuSfTrackerLt10                  = (TH1F*)(fileMuSfTracker->Get(strHistMuSfTrackerLt10));
  //histMuSfFullFastId                   = (TH2F*)(fileMuSfFullFastId->Get(strHistMuSfFullFastId));
  histMuSfFullFastIdIso                = (TH3D*)(fileMuSfFullFastIdIso->Get(strHistMuSfFullFastIdIso));
  //histMuSfFullFastIp2d                 = (TH2F*)(fileMuSfFullFastIp2d->Get(strHistMuSfFullFastIp2d));
  histMuMCEffsLMId                     = (TH2F*)(fileMuMCEffsLM->Get(strHistMuMCEffs+"Id_"+(isCR ? "CR" : "SR"))); // format lepCorMCEff_Mu_Iso_CR
  histMuMCEffsLMIso                    = (TH2F*)(fileMuMCEffsLM->Get(strHistMuMCEffs+"Iso_"+(isCR ? "CR" : "SR")));
  histMuMCEffsHMId                     = (TH2F*)(fileMuMCEffsHM->Get(strHistMuMCEffs+"Id_"+(isCR ? "CR" : "SR")));
  histMuMCEffsHMIso                    = (TH2F*)(fileMuMCEffsHM->Get(strHistMuMCEffs+"Iso_"+(isCR ? "CR" : "SR")));
  //histMuMCEffsLMId                     = (TH2F*)(fileMuMCEffsLMId->Get(strHistMuMCEffs+"Id")); // format lepCorMCEff_Mu_Iso_CR
  //histMuMCEffsLMIso                    = (TH2F*)(fileMuMCEffsLMIso->Get(strHistMuMCEffs+"Iso"));
  //histMuMCEffsHMId                     = (TH2F*)(fileMuMCEffsHMId->Get(strHistMuMCEffs+"Id"));
  //histMuMCEffsHMIso                    = (TH2F*)(fileMuMCEffsHMIso->Get(strHistMuMCEffs+"Iso"));
  if(!histMuSfId)                      throw std::invalid_argument("LeptonCorrectionSet::TnpCorr: mu sf id histo not found!");
  if(!histMuSfIso)                     throw std::invalid_argument("LeptonCorrectionSet::TnpCorr: mu sf iso histo not found!");
  if(!histMuSfIp2d)                    throw std::invalid_argument("LeptonCorrectionSet::TnpCorr: mu sf ip2d histo not found!");
  if(!histMuSfTrackerGt10)             throw std::invalid_argument("LeptonCorrectionSet::TnpCorr: mu sf tracker gt10 histo not found!");
  if(!histMuSfTrackerLt10)             throw std::invalid_argument("LeptonCorrectionSet::TnpCorr: mu sf tracker lt10 histo not found!");
  //if(!histMuSfFullFastId)              throw std::invalid_argument("LeptonCorrectionSet::TnpCorr: mu sf full/fast id histo not found!");
  if(!histMuSfFullFastIdIso)           throw std::invalid_argument("LeptonCorrectionSet::TnpCorr: mu sf full/fast iso histo not found!");
  //if(!histMuSfFullFastIp2d)            throw std::invalid_argument("LeptonCorrectionSet::TnpCorr: mu sf full/fast ip2d histo not found!");
  if(!histMuMCEffsLMId)                throw std::invalid_argument("LeptonCorrectionSet::TnpCorr: mu mc eff lm id histo not found!");
  if(!histMuMCEffsLMIso)               throw std::invalid_argument("LeptonCorrectionSet::TnpCorr: mu mc eff lm iso histo not found!");
  if(!histMuMCEffsHMId)                throw std::invalid_argument("LeptonCorrectionSet::TnpCorr: mu mc eff hm id histo not found!");
  if(!histMuMCEffsHMIso)               throw std::invalid_argument("LeptonCorrectionSet::TnpCorr: mu mc eff hm iso histo not found!");
}

TnPCorr::~TnPCorr() {
  // close then delete file ptrs?
}

// getLepWeight
//
// Moriond17 recommendations
// ---------------------------
// per:
//   (data/fullsim) https://twiki.cern.ch/twiki/bin/view/CMS/SUSLeptonSF
//   (fullsim/fastsim) https://twiki.cern.ch/twiki/bin/view/CMS/SUSLeptonSFMC
//
// electrons:
//   apply id+iso+tracker SFs. use hist errors as systs (<=2% level). add 1% extra syst if pT < 20 or > 80.
// muons:
//   apply ip2d+id+iso SFs. IGNORE hist errors (stats). flat 3% systs per muon, work ongoing to reduce. tracker SFs not yet available.
//
// fastsim:
//    just one idiso SF.
//    ip2d and miniIso cuts don't require additional full/fastsim SFs per latest presentation:
//      (slide 4) https://indico.cern.ch/event/458586/contributions/1127094/attachments/1183175/1713902/khvastunov_SUSY_6Nov_2015.pdf
//    ignore hist errors (stats, <~ 2%)
//    binned in 3d (pt,eta,pu) -- strongly recommended to do 3d not 2d
//    systs percentages to apply are:
//               (10-20,20-30,30+ GeV)
//      muon id loose +miniiso       3/1/1
//      muon id medium +miniiso      3/1/1
//      electron id veto + miniiso   5/5/5
//      electron id medium + miniiso 10/8/5
//
float TnPCorr::getLepWeight(LeptonF* lep, CORRTYPE elCorrType, CORRTYPE muCorrType, bool isGenMuon, bool isLM, bool isFastSim, bool isCR, int nPV) {
  bool dbg = false;
  if(dbg) std::cout << "[LeptonCorrectionSet::TnPCorr::getLepWeight] " << 
                     TString::Format("with isGenMuon %d, isLM %d, isFastSim %d, isCR %d, nPV %d: ",isGenMuon,isLM,isFastSim,isCR,nPV) << std::endl;

  int   pdgid  = abs(lep->pdgid()); // already abs
  float pt     = lep->pt();
  float eta    = lep->eta();
  float abseta = TMath::Abs(eta);
  if(dbg) std::cout << "[LeptonCorrectionSet::TnPCorr::getLepWeight] " << TString::Format("lep pdgid %d, pt %.3f, eta %.3f",pdgid,pt,eta) << std::endl;

  // if lep fails both id and iso (Kin config), return with 1
  if(!isGenMuon){
    if(pdgid == 11 && (!cfgSet::isSelElectron(*(ElectronF*)lep,elConfKin))) return 1;
    if(pdgid == 13 && (!cfgSet::isSelMuon(    *(MuonF*)lep    ,muConfKin))) return 1;
  }else{
    if(pt < muConfKin.minPT) return 1;
    if(abseta > muConfKin.maxETA) return 1;
  }

  // sfs and effs to fill
  float SfId            = 1;
  float SfIso           = 1;
  //float SfIp2d          = 1;
  float SfTracker       = 1; // which hist depends on pT
  float SfFullFastIdIso = 1;
  float SfMCEffsId      = 1; // which hist depends on LM/HM
  float SfMCEffsIso     = 1;

  // uncertainties in sfs, where recommended, to fill
  float UncSfId        = 0;
  float UncSfIso       = 0;
  //float UncSfIp2d      = 0;
  float UncSfTracker   = 0;
  float UncSfMCEffsId  = 0;
  float UncSfMCEffsIso = 0;

  bool  passId    = false;
  bool  passIso   = false;
  float weight    = 1;

  bool isEl = (pdgid == 11);
  if(!isEl && (pdgid != 13)) assert(!"possible - pdgId neither 11 nor 13");

  if(dbg) std::cout << "[LeptonCorrectionSet::TnPCorr::getLepWeight] filling SFs. isEl? " << isEl << std::endl;
  if(isEl) {
    // load ip2d, id, iso, tracker. fastsim: load idiso
    SfId               = getbincontent2d(pt,abseta,histElSfId);   // (pt,abseta)
    UncSfId            = getbinerror2d  (pt,abseta,histElSfId);
    SfIso              = getbincontent2d(pt,abseta,histElSfIso);  // (pt,abseta)
    UncSfIso           = getbinerror2d  (pt,abseta,histElSfIso);
    SfTracker          = getbincontent2d(eta,pt,histElSfTracker); // (**eta**,pt), a 1d graph is available
    UncSfTracker       = getbinerror2d(pt,abseta,histElSfTracker);
    SfFullFastIdIso    = (!isFastSim) ? 1 : getbincontent3d(pt,abseta,nPV,histElSfFullFastIdIso); // (pt,abseta,npv)
    SfMCEffsId         = (isLM) ? getbincontent2d(pt,abseta,histElMCEffsLMId) // (pt,abseta)
                                : getbincontent2d(pt,abseta,histElMCEffsHMId);
    UncSfMCEffsId      = (isLM) ? getbinerror2d  (pt,abseta,histElMCEffsLMId)  
                                : getbinerror2d  (pt,abseta,histElMCEffsHMId);
    SfMCEffsIso         = (isLM) ? getbincontent2d(pt,abseta,histElMCEffsLMIso) // (pt,abseta)
                                 : getbincontent2d(pt,abseta,histElMCEffsHMIso);
    UncSfMCEffsIso      = (isLM) ? getbinerror2d  (pt,abseta,histElMCEffsLMIso)  
                                 : getbinerror2d  (pt,abseta,histElMCEffsHMIso);
    if(dbg) std::cout << "[LeptonCorrectionSet::TnPCorr::getLepWeight] is electron! \n " << 
                      TString::Format("SfId %.3f +/- %.3f, SfIso %.3f +/- %.3f, SfTracker %.3f +/- %.3f, SfFullFastIdIso %.3f, SfMCEffsId %.3f +/- %.3f, SfMCEffsIso %.3f +/- %.3f", 
                      SfId, UncSfId, SfIso, UncSfIso, SfTracker,UncSfTracker,SfFullFastIdIso,SfMCEffsId,UncSfMCEffsId,SfMCEffsIso,UncSfMCEffsIso) << std::endl;

    // Moriond17: extra 1% for low or high pT, and fastsim has special table of extra systs
    float extrasyst = ( pt < 20 || pt > 80 ) ? 0.01 : 0;
    float extrafastsyst = 0;
    if(isFastSim){
      if(!isCR) extrafastsyst = 0.05;
      else extrafastsyst = pt < 10 ? 0.10 : (pt < 20 ? 0.08 : 0.05);
    }
    if(dbg) std::cout << "[LeptonCorrectionSet::TnPCorr::getLepWeight] " << TString::Format("extrasyst %.3f, extrafastsyst %.3f",extrasyst,extrafastsyst) << std::endl;

    // fold all SFs into just the Id/Iso SFs
    SfId  *= SfTracker;
    SfIso *= SfFullFastIdIso;
    if(dbg) std::cout << "[LeptonCorrectionSet::TnPCorr::getLepWeight] " << TString::Format("folded SFs into id/iso: SfId %.3f, SfIso %.3f",SfId,SfIso) << std::endl;

    // calculate systematics on folded Id/Iso SFs
    UncSfId  = SfId*SfTracker*sqrt( pow(UncSfId/SfId,2) + pow(UncSfTracker/SfTracker,2) );
    UncSfIso = SfFullFastIdIso*UncSfIso + extrasyst + extrafastsyst;
    if(dbg) std::cout << "[LeptonCorrectionSet::TnPCorr::getLepWeight] " << TString::Format("id/iso unc: UncSfId %.3f, UncSfIso %.3f",UncSfId,UncSfIso) << std::endl;

    // if systematics requested, propagate the uncertainties to the SFs
    if(elCorrType == UP) {
      SfId += UncSfId; SfIso += UncSfIso;
    }else if(elCorrType == DOWN){
      SfId -= UncSfId; SfIso -= UncSfIso;
    }
    if(dbg) std::cout << "[LeptonCorrectionSet::TnPCorr::getLepWeight] " << TString::Format("after systematics propagated: SfId %.3f, SfIso %.3f",SfId,SfIso) << std::endl;

    // for purpose of correction formula, check if passes Id (iso turned off) or Iso
    passId  = cfgSet::isSelElectron(*(ElectronF*)lep, elConfNoIso);
    passIso = cfgSet::isSelElectron(*(ElectronF*)lep, elConf);
    if(dbg) std::cout << "[LeptonCorrectionSet::TnPCorr::getLepWeight] " << TString::Format("passId %d, passIso %d",passId,passIso) << std::endl;
  }
  // if muon
  else if(!isEl){
    // load ip2d, id, iso. fastsim: load idiso
    SfId               = getbincontent2d(pt,abseta,histMuSfId);   // (pt,abseta)
    UncSfId            = getbinerror2d  (pt,abseta,histMuSfId);
    SfIso              = getbincontent2d(pt,abseta,histMuSfIso);  // (pt,abseta)
    UncSfIso           = getbinerror2d  (pt,abseta,histMuSfIso);
    //SfTracker          = getbincontent2d(eta,pt,histMuSfTracker); // (**eta**,pt), a 1d graph is available
    //UncSfTracker       = getbinerror2d(pt,abseta,histMuSfTracker);
    SfFullFastIdIso    = (!isFastSim) ? 1 : getbincontent3d(pt,abseta,nPV,histMuSfFullFastIdIso); // (pt,abseta,npv)
    SfMCEffsId         = (isLM) ? getbincontent2d(pt,abseta,histMuMCEffsLMId) // (pt,abseta)
                                : getbincontent2d(pt,abseta,histMuMCEffsHMId);
    UncSfMCEffsId      = (isLM) ? getbinerror2d  (pt,abseta,histMuMCEffsLMId)  
                                : getbinerror2d  (pt,abseta,histMuMCEffsHMId);
    SfMCEffsIso        = (isLM) ? getbincontent2d(pt,abseta,histMuMCEffsLMIso) // (pt,abseta)
                                : getbincontent2d(pt,abseta,histMuMCEffsHMIso);
    UncSfMCEffsIso     = (isLM) ? getbinerror2d  (pt,abseta,histMuMCEffsLMIso)  
                                : getbinerror2d  (pt,abseta,histMuMCEffsHMIso);

    if(dbg) std::cout << "[LeptonCorrectionSet::TnPCorr::getLepWeight] is muon! \n " << 
                      TString::Format("SfId %.3f +/- %.3f, SfIso %.3f +/- %.3f, SfFullFastIdIso %.3f, SfMCEffsId %.3f +/- %.3f, SfMCEffsIso %.3f +/- %.3f", 
                      SfId, UncSfId, SfIso, UncSfIso, SfFullFastIdIso,SfMCEffsId,UncSfMCEffsId,SfMCEffsIso,UncSfMCEffsIso) << std::endl;

    // Moriond17: extra 3% for time being, and fastsim has special table of extra systs
    float extrasyst = 0.03;
    float extrafastsyst = 0;
    if(isFastSim){
      extrafastsyst = pt < 10 ? 0.03 : (pt < 20 ? 0.01 : 0.01);
    }
    if(dbg) std::cout << "[LeptonCorrectionSet::TnPCorr::getLepWeight] " << TString::Format("extrasyst %.3f, extrafastsyst %.3f",extrasyst,extrafastsyst) << std::endl;

    // fold all SFs into just the Id/Iso SFs
    //SfId  *= SfTracker;
    SfIso *= SfFullFastIdIso;
    if(dbg) std::cout << "[LeptonCorrectionSet::TnPCorr::getLepWeight] " << TString::Format("folded SFs into id/iso: SfId %.3f, SfIso %.3f",SfId,SfIso) << std::endl;

    // calculate systematics on folded Id/Iso SFs
    UncSfId  = SfId*SfTracker*sqrt( pow(UncSfId/SfId,2) + pow(UncSfTracker/SfTracker,2) );
    UncSfIso = SfFullFastIdIso*UncSfIso + extrasyst + extrafastsyst;
    if(dbg) std::cout << "[LeptonCorrectionSet::TnPCorr::getLepWeight] " << TString::Format("id/iso unc: UncSfId %.3f, UncSfIso %.3f",UncSfId,UncSfIso) << std::endl;

    // if systematics requested, propagate the uncertainties to the SFs
    if(muCorrType == UP) {
      SfId += UncSfId; SfIso += UncSfIso;
    }else if(muCorrType == DOWN){
      SfId -= UncSfId; SfIso -= UncSfIso;
    }
    if(dbg) std::cout << "[LeptonCorrectionSet::TnPCorr::getLepWeight] " << TString::Format("after systematics propagated: SfId %.3f, SfIso %.3f",SfId,SfIso) << std::endl;

    // for purpose of correction formula, check if passes Id (iso turned off) or Iso
    passId  = cfgSet::isSelMuon(*(MuonF*)lep, muConfNoIso);
    passIso = cfgSet::isSelMuon(*(MuonF*)lep, muConf);
    if(dbg) std::cout << "[LeptonCorrectionSet::TnPCorr::getLepWeight] " << TString::Format("passId %d, passIso %d",passId,passIso) << std::endl;
  }

  // calculate the weights given just effid, effiso, sfid, and sfiso
  if(passId && passIso){
    weight = SfId*SfIso;
  }else if(passId){
    weight = SfId*(1-SfIso*SfMCEffsIso)/(1-SfMCEffsIso);
  }else{
    if(isEl){
      weight = (1-SfId*SfMCEffsId)/(1-SfMCEffsId);
    }else{
      weight = 1; // if muon fails id, set weight=1 and it's handled in getEvtWeight by using gen muons
    }
  }
  if(dbg) std::cout << "[LeptonCorrectionSet::TnPCorr::getLepWeight] final weight is " << weight << std::endl;

  // special case: we're given a gen muon
  if(isGenMuon){
    weight = (1-SfId*SfMCEffsId)/(1-SfMCEffsId);
    if(dbg) std::cout << "[LeptonCorrectionSet::TnPCorr::getLepWeight] but weight, theres more! (was a gen muon): " << weight << std::endl;
  }

  weight = std::min(std::max(weight,-2.0f),2.0f); // cap at +/- 2
  return weight;
}

// the weight for muons which fail Id are handled using gen muons (b/c we don't save anything looser...). See getEvtWeight for details.
float TnPCorr::getGenLepWeight(const GenParticleF* lep, CORRTYPE elCorrType, CORRTYPE muCorrType, bool isLM, bool isFastSim, bool isCR, int nPV ) {
  bool dbg = false;
  if(dbg) std::cout << "[LeptonCorrectionSet::TnPCorr::getGenLepWeight] starting getGenLepWeight. Using gen to fake muon of pt " << lep->pt() << std::endl;

  // convert GenParticleF to a LeptonF to imitate a reco muon
  LeptonF * muon = new LeptonF(lep->p4()); // walks like a muon
  muon->setPdgId(13);                      // talks like a muon
  muon->setIsMuon(true);                   // totally a muon (need this to pass first few lines of getLepWeight)

  bool isGenMuon = true;
  float weight = getLepWeight(muon, elCorrType, muCorrType, isGenMuon, isLM, isFastSim, isCR, nPV);
  if(dbg) std::cout << "[LeptonCorrectionSet::TnPCorr::getGenLepWeight] fake muon got weight " << weight << std::endl;

  delete muon;
  return weight;
}

float TnPCorr::getEvtWeight(const std::vector<LeptonF*>& allLeptons, const std::vector<LeptonF*>& selectedLeptons, const std::vector<GenParticleF*> genParts,
                            CORRTYPE elCorrType, CORRTYPE muCorrType, bool isLM, bool isFastSim, bool isCR, int nPV) {
  bool dbg = false;
  if(dbg) std::cout << "[LeptonCorrectionSet::TnPCorr::getEvtWeight] starting getevtweight " 
                    << TString::Format("with isLM %d, isFastSim %d, isCR %d, nPV %d",isLM,isFastSim,isCR,nPV) << std::endl;

  // store gen el/mu with W/Z/tau mothers
  std::vector<const GenParticleF*> genEl_;
  std::vector<const GenParticleF*> genMu_;
  for(const auto* p : genParts) {
    if(ParticleInfo::isA(ParticleInfo::p_Z0, p) || ParticleInfo::isA(ParticleInfo::p_Wplus, p) || ParticleInfo::isA(ParticleInfo::p_tauminus, p)) {
      for(unsigned int id = 0; id < p->numberOfDaughters(); ++id) {
        const GenParticleF* dau = p->daughter(id);
        if     (ParticleInfo::isA(ParticleInfo::p_eminus , dau)) genEl_.push_back(dau);
        else if(ParticleInfo::isA(ParticleInfo::p_muminus, dau)) genMu_.push_back(dau);
      }
    }
  }
  if(dbg) std::cout << "[LeptonCorrectionSet::TnPCorr::getEvtWeight] " << TString::Format("found: genEl %lu, genMu %lu",genEl_.size(),genMu_.size()) << std::endl;

  // match (dr 0.4) those gen el/mu to allLeptons
  float weight = 1.0;
  for(auto* lep : allLeptons) {
    if     (lep->pdgid()==11 && (elCorrType==NONE)) continue;
    else if(lep->pdgid()==13 && (muCorrType==NONE)) continue;
    double nearDR = 0;
    int near = -1;
    if     (lep->pdgid()==11) near = PhysicsUtilities::findNearestDRDeref(*lep, genEl_, nearDR, 0.4);
    else if(lep->pdgid()==13) near = PhysicsUtilities::findNearestDRDeref(*lep, genMu_, nearDR, 0.4);
    if(dbg) std::cout << "[LeptonCorrectionSet::TnPCorr::getEvtWeight] looping thru allLeptons for nearness to genEl or genMu: " << TString::Format("near index %d, nearDR %.3f",near,nearDR) << std::endl;
    if(near >= 0) weight *= getLepWeight(lep,elCorrType,muCorrType,false,isLM,isFastSim,isCR,nPV);
  }
  if(dbg) std::cout << "[LeptonCorrectionSet::TnPCorr::getEvtWeight] matching those to allLeptons and calling getLepWeight -> new weight of " << weight << std::endl;

  // but that left out muons failing Id (it set weight=1 in that case). we need looser muons. use gen muons as substitute.
  // store reco mu from allLeptons
  std::vector<const LeptonF*> recMu_;
  for(const auto* lep : allLeptons) if(lep->pdgid()==13) recMu_.push_back(lep);
  if(dbg) std::cout << "[LeptonCorrectionSet::TnPCorr::getEvtWeight] found recMus (and will now loop through genMus): " << recMu_.size() << std::endl;
  if(muCorrType!=NONE) {
    for(auto* lep : genMu_) {
      double nearDR = 0;
      int near = PhysicsUtilities::findNearestDRDeref(*lep, recMu_, nearDR, 0.4);
      if(dbg) std::cout << "[LeptonCorrectionSet::TnPCorr::getEvtWeight] in loop thru genMu. May call getGenLepWeight. nearness of this genMu to any recMu: " << TString::Format("near index %d, nearDR %.3f",near,nearDR) << std::endl;
      if(near<0) weight *= getGenLepWeight(lep,elCorrType,muCorrType,isLM,isFastSim,isCR,nPV);
      else if(!cfgSet::isSelMuon(*(MuonF*)recMu_[near], muConfNoIso)) weight *= getGenLepWeight(lep,elCorrType,muCorrType,isLM,isFastSim,isCR,nPV);
    }
  }
  if(dbg) std::cout << "[LeptonCorrectionSet::TnPCorr::getEvtWeight] done looping thru genMu. -> new weight of  " << weight << std::endl << std::endl;

  // cap weights at 2
  if(weight < -2.0) weight = -2.0;
  if(weight >  2.0) weight =  2.0;
  return weight;
}

void LeptonCorrectionSet::load(const LeptonSelection::Electron elSel, const LeptonSelection::Electron secElSel,
                               const LeptonSelection::Muon     muSel, const LeptonSelection::Muon     secMuSel,
                               int correctionOptions)
{
  name = "LEP";
  options_ = correctionOptions;

  if(correctionOptions & USE_HPSTAUS){ // user may switch back to HPS taus
    setUseHPSTaus(true);
  }

  if(correctionOptions & LEP) {
    lepCorr = new LepCorr();
    corrections.push_back(lepCorr);
  }
  if(correctionOptions & TNP) {
    tnpCorr = new TnPCorr(elSel, secElSel, muSel, secMuSel);
    corrections.push_back(tnpCorr);
  }
}

void countRecoLepsAndGenTaus(const BaseTreeAnalyzer * ana, int &nPromptGenTaus, int &nSelectedElectrons, int &nSelectedMuons){
  nPromptGenTaus = 0, nSelectedElectrons = 0, nSelectedMuons = 0;

  // count reco leptons
  for(auto* i : ana->selectedLeptons) {
    if(fabs(i->pdgid()) == 11) { nSelectedElectrons++; }
    if(fabs(i->pdgid()) == 13) { nSelectedMuons++;     }
  }

  // count gen hadronic taus
  for(auto* p : ana->genParts) {
    const GenParticleF * genPartMom = 0;
    if(p->numberOfMothers() < 1) continue;
    genPartMom = p->mother(0);

    // tau from W/Z
    if (ParticleInfo::isA(ParticleInfo::p_tauminus, p) && (ParticleInfo::isA(ParticleInfo::p_Z0, genPartMom) || ParticleInfo::isA(ParticleInfo::p_Wplus, genPartMom))) {
      bool lepDecay = false;
      // hadronic?
      for(unsigned int itd = 0; itd < p->numberOfDaughters(); itd++) {
        const GenParticleF* dau = p->daughter(itd);
        if(ParticleInfo::isA(ParticleInfo::p_eminus, dau) || ParticleInfo::isA(ParticleInfo::p_muminus, dau)) lepDecay = true;
      }
      if(!lepDecay) { nPromptGenTaus++; }
    }
  }

}

void LeptonCorrectionSet::processCorrection(const BaseTreeAnalyzer * ana) {
  bool dbg = false;
  if(dbg) std::cout << "[LeptonCorrectionSet::processCorrection]" << std::endl;

  selLepWeightLM  = 1.;
  selLepWeightHM  = 1.;
  vetoLepWeightLM = 1.;
  vetoLepWeightHM = 1.;
  tnpEvtWeightLM  = 1.;
  tnpEvtWeightHM  = 1.;
  if(!ana->isMC()) return;

  if(options_ & LEP) {
    if(dbg) std::cout << std::endl << "[LeptonCorrectionSet::processCorrection] option LEP" << std::endl;

    // lep histo bins
    const std::vector<double> eleCorrPtBins = {0., 20., 30., 40., 70., 100., 1000.};
    const std::vector<double> muCorrPtBins  = {0., 20., 30., 40., 70., 100., 1000.};
    const std::vector<double> tauCorrPtBins = {0., 20., 40., 1000.};
    const unsigned int nEleBins = eleCorrPtBins.size()-1;
    const unsigned int nMuBins  = muCorrPtBins.size()-1;
    const unsigned int nTauBins = tauCorrPtBins.size()-1;
    // convenient bin numbers
    const unsigned int binDefaultZeroCorr   = 1;
    const unsigned int binEleFirst          = binDefaultZeroCorr + 1;
    const unsigned int binEleLast           = binEleFirst + nEleBins-1;
    const unsigned int binMuFirst           = binEleLast + 1;
    const unsigned int binMuLast            = binMuFirst  + nMuBins-1;
    const unsigned int binTauFirst          = binMuLast  + 1;
    const unsigned int binTauLast           = binTauFirst + nTauBins-1;
    const unsigned int binHpsTaus           = binTauLast + 1;
    const unsigned int binFakes             = binHpsTaus + 1;

    // determine region
    int nPromptGenTaus = 0, nSelectedElectrons = 0, nSelectedMuons = 0;
    countRecoLepsAndGenTaus(ana, nPromptGenTaus, nSelectedElectrons, nSelectedMuons);
    enum REGION { TAU_SELECTED, HPS_SELECTED, FAKE, VETO };
    REGION region;
    if(     !useHPS && ana->nVetoedTracks >= 1 && nSelectedElectrons == 0 && nSelectedMuons == 0 && nPromptGenTaus >= 1) region = REGION::TAU_SELECTED;
    else if( useHPS && ana->nVetoedTracks >= 1 && nSelectedElectrons == 0 && nSelectedMuons == 0 && nPromptGenTaus >= 1) region = REGION::HPS_SELECTED;
    else if( nSelectedMuons >= 1 || nSelectedElectrons >= 1 || ana->nVetoedTracks >= 1) region = REGION::FAKE;
    else region = REGION::VETO;

    if(dbg) std::cout << "[LeptonCorrectionSet::processCorrection] " 
                      << TString::Format("Region %d. Found %d vetoed tracks, %d prompt hadronic gen taus, %d selected electrons, %d selected muons", 
                                         region, ana->nVetoedTracks, nPromptGenTaus, nSelectedElectrons, nSelectedMuons) << std::endl;
    //if(dbg) std::cout << "[LeptonCorrectionSet::processCorrection] lepCorr bins: " << TString::Format("binEleFirst is %d, binMuFirst %d, binTauFirst %d, binFakes %d, binDefaultZeroCorr %d",
    //                                                                                      binEleFirst,binMuFirst,binTauFirst,binFakes,binDefaultZeroCorr) << std::endl;

    // determine bin in Lep hists to use
    int bin = binDefaultZeroCorr;
    if(region == TAU_SELECTED){
      // several pT bins. seek out bin corresponding to our tau pt
      float trackPt = ana->vetoedTracks[0]->pt();
      if(dbg) std::cout << "[LeptonCorrectionSet::processCorrection] region is TAU_SELECTED, searching for pT bin for " << trackPt << std::endl;
      for(unsigned int iBin = binTauFirst; iBin <= binTauLast; iBin++){
        //float binLowPt  = tauCorrPtBins[iBin - binTauFirst];
        float binHighPt = tauCorrPtBins[iBin - binTauFirst + 1];
        if(trackPt > binHighPt && iBin != binTauLast){ continue; } // try next bin if there is one
        bin = iBin; // use this bin
        break;
      }
    }else if(region == HPS_SELECTED){
      bin = binHpsTaus;
    }else if(region == FAKE){
      bin = binFakes;
    }else if(region == VETO){
      bin = binDefaultZeroCorr;
    }else{ assert(!"Possible: region altered outside enum"); }

    // fetch corr from Lep hists
    if(dbg) std::cout << "[LeptonCorrectionSet::processCorrection] fetching Corr from bin " << bin << std::endl;
    float corrLM, corrHM, uncCorrLM, uncCorrHM;
    corrLM = lepCorr->histLepLM->GetBinContent(bin);
    corrHM = lepCorr->histLepHM->GetBinContent(bin);
    uncCorrLM = lepCorr->histLepLM->GetBinError(bin);
    uncCorrHM = lepCorr->histLepHM->GetBinError(bin);
    if(dbg) std::cout << "[LeptonCorrectionSet::processCorrection] " << TString::Format("corrLM %.3f +/- %.3f, corrHM %.3f +/- %.3f", corrLM, uncCorrLM, corrHM, uncCorrHM) << std::endl;

    // apply systematics to Corr
    if(region == TAU_SELECTED || region == HPS_SELECTED){
      if( (options_ & LEP_VARY_UP) || (options_ & TAU_VARY_UP) ){
        corrLM += uncCorrLM; corrHM += uncCorrHM;
      }else if( (options_ & LEP_VARY_DOWN) || (options_ & TAU_VARY_DOWN) ){
        corrLM -= uncCorrLM; corrHM -= uncCorrHM;
      }
    }

    // determine weights (Corr or 1-Corr)
    vetoLepWeightLM = 1 - corrLM;
    selLepWeightLM  = corrLM;
    vetoLepWeightHM = 1 - corrHM;
    selLepWeightHM  = corrHM;
    if(dbg) std::cout << "[LeptonCorrectionSet::processCorrection] " << TString::Format("LEP weights finally: vetoLM  %.3f / selLM  %.3f / vetoHM  %.3f / selHM  %.3f", vetoLepWeightLM, selLepWeightLM, vetoLepWeightHM, selLepWeightHM) << std::endl;

  }//if option LEP

  if(options_ & TNP) {
    if(dbg) std::cout << std::endl << "[LeptonCorrectionSet::processCorrection] option TNP" << std::endl;
    const cfgSet::ConfigSet& cfg = ana->getAnaCfg();
    bool isFastSim = ana->evtInfoReader.isfastsim;
    bool isCR = tnpCorr->isCR;
    int nPV = ana->nPV;
    tnpEvtWeightLM = tnpCorr->getEvtWeight(ana->allLeptons, ana->selectedLeptons,ana->genParts, cfg.corrections.tnpElCorrType, cfg.corrections.tnpMuCorrType, true,isFastSim, isCR, nPV);
    tnpEvtWeightHM = tnpCorr->getEvtWeight(ana->allLeptons, ana->selectedLeptons,ana->genParts, cfg.corrections.tnpElCorrType, cfg.corrections.tnpMuCorrType, false,isFastSim, isCR, nPV);
    if(dbg) std::cout << "[LeptonCorrectionSet::processCorrection] " << TString::Format("TNP weights finally: LM %.3f / HM %.3f", tnpEvtWeightLM, tnpEvtWeightHM) << std::endl;
  }

}

} /* namespace ucsbsusy */
