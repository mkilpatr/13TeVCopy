

#include "AnalysisBase/TreeAnalyzer/interface/DefaultConfigurations.h"
#include "AnalysisBase/TreeAnalyzer/interface/ConfigurationBase.h"
#include "AnalysisTools/TreeReader/interface/Defaults.h"
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"

char * cfgSet::CMSSW_BASE = getenv ("CMSSW_BASE");

cfgSet::JetConfig cfgSet::zl_search_jets("zl_search_jets");
cfgSet::JetConfig cfgSet::zl_photon_jets("zl_photon_jets");
cfgSet::JetConfig cfgSet::zl_lepton_jets("zl_lepton_jets");
cfgSet::JetConfig cfgSet::zl_dilepton_jets("zl_dilepton_jets");
cfgSet::JetConfig cfgSet::sl_search_jets("sl_search_jets");


void cfgSet::loadDefaultJetConfigurations() {
  zl_search_jets.jetCollection = AK4JETS;
  zl_search_jets.minPt                     = 20;
  zl_search_jets.maxEta                    = 2.4;
  zl_search_jets.minBJetPt                 = 20;
  zl_search_jets.maxBJetEta                = 2.4;
  zl_search_jets.defaultCSV                = defaults::CSV_MEDIUM;
  zl_search_jets.applyJetID                = true;
  zl_search_jets.applyAdHocPUCorr          = false;
  zl_search_jets.cleanJetsvLeptons         = false;
  zl_search_jets.ignoreSeconaryLeptonsWhenCleaning= false;
  zl_search_jets.cleanJetsvPhotons         = false;
  zl_search_jets.cleanJetsvTracks          = false;
  zl_search_jets.cleanJetsMaxDR            = 0.4;
  zl_search_jets.setConfig();

  zl_photon_jets = zl_search_jets;
  zl_photon_jets.cleanJetsvPhotons = true;

  zl_lepton_jets = zl_search_jets;
  zl_lepton_jets.cleanJetsvLeptons   = false;
  zl_lepton_jets.cleanJetsvTracks    = false;

  zl_dilepton_jets = zl_search_jets;
  zl_dilepton_jets.cleanJetsvLeptons = true;

  sl_search_jets = zl_search_jets;
  sl_search_jets.cleanJetsvLeptons = true;
  sl_search_jets.ignoreSeconaryLeptonsWhenCleaning   = true;
  sl_search_jets.minPt                        = 30;
  sl_search_jets.minBJetPt                    = 30;
  sl_search_jets.applyJetID                   = true;
  sl_search_jets.cleanJetsMaxDR               = 0.4;
}


cfgSet::TrackConfig cfgSet::zl_veto_tracks("zl_veto_tracks");
cfgSet::TrackConfig cfgSet::sl_veto_tracks("sl_veto_tracks");

void cfgSet::loadDefaultTrackConfigurations() {
  zl_veto_tracks.minPt     = 10;
  zl_veto_tracks.maxEta    = 2.4;
  zl_veto_tracks.mtPresel  = true;
  zl_veto_tracks.maxDz     = 0.2;
  zl_veto_tracks.selected  = &ucsbsusy::PFCandidateF::ismvavetotau;
  zl_veto_tracks.setConfig();

  sl_veto_tracks = zl_veto_tracks;
  sl_veto_tracks.mtPresel  = false;
  sl_veto_tracks.minPt     = 10;
  sl_veto_tracks.maxDz     = 0.1;
  sl_veto_tracks.selected  = &ucsbsusy::PFCandidateF::ischargedhadron;
  sl_veto_tracks.setConfig();
}

cfgSet::TauConfig cfgSet::zl_veto_taus("zl_veto_taus");
cfgSet::TauConfig cfgSet::sl_veto_taus("sl_veto_taus");

void cfgSet::loadDefaultTauConfigurations() {
  zl_veto_taus.minPt      = 20;
  zl_veto_taus.maxEta     = 2.4;
  zl_veto_taus.requireOppositeQToSelLepton = false;
  zl_veto_taus.minDeltaRFromSelLepton      = -0.4;
  zl_veto_taus.selected   = &ucsbsusy::TauF::ispoglisomvarun2new;
  zl_veto_taus.setConfig();

  sl_veto_taus = zl_veto_taus;
  sl_veto_taus.requireOppositeQToSelLepton = true;
  sl_veto_taus.selected   = &ucsbsusy::TauF::isgoodpogtau;
  sl_veto_taus.setConfig();
}

cfgSet::PhotonConfig cfgSet::zl_sel_photons("zl_sel_photons");

void cfgSet::loadDefaultPhotonConfigurations() {
  zl_sel_photons.minPt    = 200;
  zl_sel_photons.maxEta   = 2.5;
  zl_sel_photons.selected = &ucsbsusy::PhotonF::isloose;
  zl_sel_photons.usePixelSeedVeto = true;
  zl_sel_photons.useElectronVeto  = true;
  zl_sel_photons.setConfig();
}

cfgSet::CorrectionConfig cfgSet::standardCorrections("standardCorrections");

void cfgSet::loadDefaultCorrections() {
  standardCorrections.ttbarCorrections         = ucsbsusy::TtbarCorrectionSet::TOPPAIRPT;
  standardCorrections.ttbarCorrectionFile      =  TString::Format("%s/src/data/corrections/ttbarCorr.root",CMSSW_BASE);

  standardCorrections.puCorrections            = ucsbsusy::EventCorrectionSet::TRUEPU | ucsbsusy::EventCorrectionSet::CTT | ucsbsusy::EventCorrectionSet::SDTOP | ucsbsusy::EventCorrectionSet::SDW;
  standardCorrections.puCorrectionFile         =  TString::Format("%s/src/data/corrections/puWeights_2016_12p9ifb_50bins_63mb.root",CMSSW_BASE);
  standardCorrections.cttCorrectionFile        =  TString::Format("%s/src/data/corrections/cttCorr.root",CMSSW_BASE);
  standardCorrections.cttEffSFType             = ucsbsusy::NOMINAL;
  standardCorrections.cttMistagSFType          = ucsbsusy::NOMINAL;
  //standardCorrections.sdCorrectionFile         = TString::Format("%s/src/data/corrections/2016/SdSF_12p9invfb_DataFull_Corrected_and_FullFast.root",CMSSW_BASE);
  standardCorrections.sdCorrectionFile         = TString::Format("%s/src/data/corrections/2016/SdTopWTagging-DataFull-correctedbymistagrate.root",CMSSW_BASE);
  standardCorrections.sdTopCorrType            = ucsbsusy::NOMINAL;
  standardCorrections.sdWCorrType              = ucsbsusy::NOMINAL;

  standardCorrections.triggerCorrections       = ucsbsusy::TriggerCorrectionSet::PHOTON | ucsbsusy::TriggerCorrectionSet::ELECTRON | ucsbsusy::TriggerCorrectionSet::MUON | ucsbsusy::TriggerCorrectionSet::MET_OR_LEP;
  standardCorrections.triggerCorrectionFile    =  TString::Format("%s/src/data/corrections/2016/trigCorr.root",CMSSW_BASE);
  standardCorrections.trigPhotonCorrType       = ucsbsusy::NOMINAL;
  standardCorrections.trigElectronCorrType     = ucsbsusy::NOMINAL;
  standardCorrections.trigMuonCorrType         = ucsbsusy::NOMINAL;
  standardCorrections.trigMetOrLepCorrType     = ucsbsusy::NOMINAL;

  standardCorrections.wpolCorrections         = ucsbsusy::WPolCorrectionSet::WPOLWGT;

  standardCorrections.leptonCorrections        = ucsbsusy::LeptonCorrectionSet::LEP | ucsbsusy::LeptonCorrectionSet::MULTI_PT_BINS | ucsbsusy::LeptonCorrectionSet::TNP;
  standardCorrections.leptonCorrectionFile     =  TString::Format("%s/src/data/corrections/2016/lepCorMCEffsAndSFs/lepCorr_morebins_LM.root",CMSSW_BASE);
  standardCorrections.leptonCorrectionFile2    =  TString::Format("%s/src/data/corrections/2016/lepCorMCEffsAndSFs/lepCorr_morebins_HM.root",CMSSW_BASE);
  standardCorrections.tnpElCorrType            = ucsbsusy::NOMINAL;
  standardCorrections.tnpMuCorrType            = ucsbsusy::NOMINAL;
  //standardCorrections.tnpElIdCorrType          = ucsbsusy::NOMINAL;
  //standardCorrections.tnpElIsoCorrType         = ucsbsusy::NOMINAL;
  //standardCorrections.tnpMuIdCorrType          = ucsbsusy::NOMINAL;
  //standardCorrections.tnpMuIsoCorrType         = ucsbsusy::NOMINAL;

  standardCorrections.jetAndMETCorrections     = ucsbsusy::JetAndMETCorrectionSet::QCDRESPTAIL;

  standardCorrections.jetResCorrType           = ucsbsusy::NONE;
  standardCorrections.jetResFile               = TString::Format("%s/src/data/corrections/2016/ak4JetResTrends_Spring16_25nsV6_MC_PtResolution_AK4PFchs.root",CMSSW_BASE);
  standardCorrections.jetResCorrFile           = TString::Format("%s/src/data/corrections/2016/ak4JetResCorr_Spring16_25nsV6_MC_SF_AK4PFchs.root",CMSSW_BASE);

  standardCorrections.metResSystRunIType       = ucsbsusy::NONE;

  standardCorrections.jetResTailFile           = TString::Format("%s/src/data/corrections/2016/qcdJetRespTailCorr.root",CMSSW_BASE);
  standardCorrections.jetResTailCorrType       = ucsbsusy::NOMINAL;

  standardCorrections.bTagCorrections          = ucsbsusy::BTagCorrectionSet::BYEVTWEIGHT | ucsbsusy::BTagCorrectionSet::FASTSIMBYEVTWEIGHT;
  standardCorrections.bTagEffFile              =  TString::Format("%s/src/data/corrections/2016/csvEffs.root",CMSSW_BASE);
  standardCorrections.bTagSFFile               =  TString::Format("%s/src/data/corrections/2016/CSVv2_ichep.csv",CMSSW_BASE);
  standardCorrections.heavyBTagCorrType        = ucsbsusy::NOMINAL;
  standardCorrections.lightBTagCorrType        = ucsbsusy::NOMINAL;
  standardCorrections.bTagFastSimEffFile        =  TString::Format("%s/src/data/corrections/2016/csvFastSimEffs.root",CMSSW_BASE);
  standardCorrections.bTagFastSimSFFile         =  TString::Format("%s/src/data/corrections/2016/CSV_13TEV_Combined_14_7_2016.csv",CMSSW_BASE);
  standardCorrections.heavyFastSimBTagCorrType        = ucsbsusy::NOMINAL;
  standardCorrections.lightFastSimBTagCorrType        = ucsbsusy::NOMINAL;


  standardCorrections.isrCorrections     = ucsbsusy::ISRCorrectionSet::ISRCORR | ucsbsusy::ISRCorrectionSet::ISRCORRTIGHT;
  standardCorrections.isrCorrFile        =  TString::Format("%s/src/data/corrections/2016/isrCorr.root",CMSSW_BASE);
  standardCorrections.isrSigNormFile     =  TString::Format("%s/src/data/corrections/2016/isrSigNorms.root",CMSSW_BASE);
  standardCorrections.isrSigNormTightFile     =  TString::Format("%s/src/data/corrections/2016/isrSigNormsTight.root",CMSSW_BASE);
  standardCorrections.isrSigNorms.push_back("T2tt"); standardCorrections.isrSigNorms.push_back("T2bW"); standardCorrections.isrSigNorms.push_back("T2fb");
  standardCorrections.isrType = ucsbsusy::NOMINAL;
  standardCorrections.setConfig();
}

cfgSet::ConfigSet cfgSet::zl_search_set;
cfgSet::ConfigSet cfgSet::zl_lepton_set;
cfgSet::ConfigSet cfgSet::zl_dilepton_set;
cfgSet::ConfigSet cfgSet::zl_photon_set;
cfgSet::ConfigSet cfgSet::sl_search_set;


void cfgSet::loadDefaultConfigurations() {
  loadDefaultJetConfigurations();
  loadDefaultTrackConfigurations();
  loadDefaultTauConfigurations();
  loadDefaultPhotonConfigurations();
  loadDefaultCorrections();

  zl_search_set.jets                     = zl_search_jets;
  zl_search_set.electrons                = LeptonSelection::zl_sel_electrons;
  zl_search_set.muons                    = LeptonSelection::zl_sel_muons;
  zl_search_set.tracks                   = zl_veto_tracks;
  zl_search_set.taus                     = zl_veto_taus;
  zl_search_set.corrections              = standardCorrections;

  zl_lepton_set.jets                     = zl_lepton_jets;
  zl_lepton_set.electrons                = LeptonSelection::zl_ctr_sLep_electrons;
  zl_lepton_set.muons                    = LeptonSelection::zl_ctr_sLep_muons;
  zl_lepton_set.secondaryElectrons       = LeptonSelection::zl_ctr_sLep_sec_electrons;
  zl_lepton_set.secondaryMuons           = LeptonSelection::zl_ctr_sLep_sec_muons;
  zl_lepton_set.tracks                   = zl_veto_tracks;
  zl_lepton_set.corrections              = standardCorrections;

  zl_dilepton_set.jets                   = zl_dilepton_jets;
  zl_dilepton_set.electrons              = LeptonSelection::zl_ctr_diLep_electrons;
  zl_dilepton_set.muons                  = LeptonSelection::zl_ctr_diLep_muons;
  zl_dilepton_set.secondaryElectrons     = LeptonSelection::zl_ctr_diLep_sec_electrons;
  zl_dilepton_set.secondaryMuons         = LeptonSelection::zl_ctr_diLep_sec_muons;
  zl_dilepton_set.tracks                 = zl_veto_tracks;
  zl_dilepton_set.taus                   = zl_veto_taus;
  zl_dilepton_set.corrections            = standardCorrections;

  zl_photon_set.jets                     = zl_photon_jets;
  zl_photon_set.electrons                = LeptonSelection::zl_sel_electrons;
  zl_photon_set.muons                    = LeptonSelection::zl_sel_muons;
  zl_photon_set.taus                     = zl_veto_taus;
  zl_photon_set.photons                  = zl_sel_photons;
  zl_photon_set.corrections              = standardCorrections;

  sl_search_set.jets                     = sl_search_jets;
  sl_search_set.electrons                = LeptonSelection::sl_sel_electrons;
  sl_search_set.muons                    = LeptonSelection::sl_sel_muons;
  sl_search_set.secondaryElectrons       = LeptonSelection::sl_sel_sec_electrons;
  sl_search_set.secondaryMuons           = LeptonSelection::sl_sel_sec_muons;
  sl_search_set.tracks                   = sl_veto_tracks;
  sl_search_set.taus                     = sl_veto_taus;
  sl_search_set.corrections              = standardCorrections;
}

void cfgSet::setJSONFile(const TString jsonfile) {
  zl_search_set.jsonFile = jsonfile;
  zl_search_set.jsonProcessing = new JSONProcessing(jsonfile);
  zl_lepton_set.jsonFile = jsonfile;
  zl_lepton_set.jsonProcessing = new JSONProcessing(jsonfile);
  zl_dilepton_set.jsonFile = jsonfile;
  zl_dilepton_set.jsonProcessing = new JSONProcessing(jsonfile);
  zl_photon_set.jsonFile = jsonfile;
  zl_photon_set.jsonProcessing = new JSONProcessing(jsonfile);
  sl_search_set.jsonFile = jsonfile;
  sl_search_set.jsonProcessing = new JSONProcessing(jsonfile);
}

bool cfgSet::useResolvedTopMVA = false;
TString cfgSet::resolvedTopMVAFile = TString::Format("%s/src/data/resTop/resTop_xGBoost_v0.weights.xml",CMSSW_BASE);

