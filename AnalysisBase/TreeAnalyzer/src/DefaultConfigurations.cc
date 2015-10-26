
#include "AnalysisBase/TreeAnalyzer/interface/DefaultConfigurations.h"
#include "AnalysisBase/TreeAnalyzer/interface/ConfigurationBase.h"
#include "AnalysisTools/TreeReader/interface/Defaults.h"
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"

char * cfgSet::CMSSW_BASE = getenv ("CMSSW_BASE");

cfgSet::JetConfig cfgSet::zl_search_jets("zl_search_jets");
cfgSet::JetConfig cfgSet::zl_photon_jets("zl_photon_jets");
cfgSet::JetConfig cfgSet::zl_lepton_jets("zl_lepton_jets");
cfgSet::JetConfig cfgSet::zl_dilepton_jets("zl_dilepton_jets");
cfgSet::JetConfig cfgSet::ol_search_jets("ol_search_jets");


void cfgSet::loadDefaultJetConfigurations() {
  zl_search_jets.jetCollection = AK4JETS;
  zl_search_jets.minPt                     = 20;
  zl_search_jets.maxEta                    = 2.4;
  zl_search_jets.minBJetPt                 = 20;
  zl_search_jets.maxBJetEta                = 2.4;
  zl_search_jets.defaultCSV                = defaults::CSV_MEDIUM;
  zl_search_jets.applyJetID                = true;
  zl_search_jets.applyAdHocPUCorr          = false;
  zl_search_jets.cleanJetsvSelectedLeptons = false;
  zl_search_jets.cleanJetsvVetoedLeptons   = false;
  zl_search_jets.cleanJetsvSelectedPhotons = false;
  zl_search_jets.cleanJetsvVetoedTracks    = false;
  zl_search_jets.cleanJetsMaxDR            = 0.4;
  zl_search_jets.setConfig();

  zl_photon_jets = zl_search_jets;
  zl_photon_jets.cleanJetsvSelectedPhotons = true;

  zl_lepton_jets = zl_search_jets;
  zl_lepton_jets.cleanJetsvSelectedLeptons = false;
  zl_lepton_jets.cleanJetsvVetoedTracks    = false;

  zl_dilepton_jets = zl_search_jets;
  zl_dilepton_jets.cleanJetsvSelectedLeptons = true;

  ol_search_jets = zl_search_jets;
  ol_search_jets.cleanJetsvSelectedLeptons = true;
  ol_search_jets.cleanJetsvVetoedLeptons   = false;
  ol_search_jets.minPt                     = 30;
  ol_search_jets.minBJetPt                 = 30;
  ol_search_jets.applyJetID                = true;
  ol_search_jets.cleanJetsMaxDR            = 0.4;
}

cfgSet::LeptonConfig cfgSet::zl_sel_leptons ("zl_sel_leptons") ;
cfgSet::LeptonConfig cfgSet::zl_veto_leptons("zl_veto_leptons");
cfgSet::LeptonConfig cfgSet::zl_loose_leptons("zl_loose_leptons");
cfgSet::LeptonConfig cfgSet::ol_sel_leptons ("ol_sel_leptons") ;
cfgSet::LeptonConfig cfgSet::ol_veto_leptons("ol_veto_leptons");

void cfgSet::loadDefaultLeptonConfigurations() {
  zl_sel_leptons.minEPt            = 5;
  zl_sel_leptons.maxEEta           = 2.4;
  zl_sel_leptons.selectedElectron  = &ucsbsusy::ElectronF::ismt2vetoelectron;
  zl_sel_leptons.minMuPt           = 5;
  zl_sel_leptons.maxMuEta          = 2.4;
  zl_sel_leptons.maxMuD0           = 0.1;
  zl_sel_leptons.maxMuDz           = 0.5;
  zl_sel_leptons.selectedMuon      = &ucsbsusy::MuonF::ismt2vetomuon;
  zl_sel_leptons.setConfig();

  zl_veto_leptons = zl_sel_leptons;
 // zl_veto_leptons.selectedElectron  = &ucsbsusy::ElectronF::isvetoelectron;
 // zl_veto_leptons.selectedMuon      = &ucsbsusy::MuonF::isvetomuon;

  zl_loose_leptons = zl_sel_leptons;
  zl_loose_leptons.minEPt             = 15;
  zl_loose_leptons.selectedElectron   = &ucsbsusy::ElectronF::islooseelectron;
  zl_loose_leptons.minMuPt            = 10;
  zl_loose_leptons.selectedMuon       = &ucsbsusy::MuonF::isloosemuon;

  ol_sel_leptons = zl_sel_leptons;
  ol_sel_leptons.minEPt            = 40;
  ol_sel_leptons.maxEEta           = 2.1;
  ol_sel_leptons.selectedElectron  = &ucsbsusy::ElectronF::isgoodpogelectron;
  ol_sel_leptons.minMuPt           = 30;
  ol_sel_leptons.maxMuEta          = 2.1;
  ol_sel_leptons.maxMuD0           = 0.02;
  ol_sel_leptons.maxMuDz           = 0.1;
  ol_sel_leptons.selectedMuon      = &ucsbsusy::MuonF::isgoodpogmuon;

  ol_veto_leptons = zl_sel_leptons;
  ol_veto_leptons.minEPt            = 5;
  ol_veto_leptons.maxEEta           = 2.4;
  ol_veto_leptons.selectedElectron  = &ucsbsusy::ElectronF::isvetopogelectron;
  ol_veto_leptons.minMuPt           = 5;
  ol_veto_leptons.maxMuEta          = 2.4;
  ol_veto_leptons.maxMuD0           = 0.1;
  ol_veto_leptons.maxMuDz           = 0.5;
  ol_veto_leptons.selectedMuon      = &ucsbsusy::MuonF::isvetomuon;
}

cfgSet::TrackConfig cfgSet::zl_veto_tracks("zl_veto_tracks");
cfgSet::TrackConfig cfgSet::ol_veto_tracks("ol_veto_tracks");

void cfgSet::loadDefaultTrackConfigurations() {
  zl_veto_tracks.minPt     = 10;
  zl_veto_tracks.maxEta    = 2.4;
  zl_veto_tracks.mtPresel  = true;
  zl_veto_tracks.maxDz     = 0.2;
  zl_veto_tracks.selected  = &ucsbsusy::PFCandidateF::ismvavetotau;
  zl_veto_tracks.setConfig();

  ol_veto_tracks = zl_veto_tracks;
  ol_veto_tracks.minPt     = 5;
  ol_veto_tracks.maxDz     = -1;
  ol_veto_tracks.selected  = &ucsbsusy::PFCandidateF::ismvavetotau;
  ol_veto_tracks.setConfig();
}

cfgSet::TauConfig cfgSet::zl_veto_taus("zl_veto_taus");
cfgSet::TauConfig cfgSet::ol_veto_taus("ol_veto_taus");

void cfgSet::loadDefaultTauConfigurations() {
  zl_veto_taus.minPt      = 20;
  zl_veto_taus.maxEta     = 2.4;
  zl_veto_taus.requireOppositeQToSelLepton = false;
  zl_veto_taus.minDeltaRFromSelLepton      = 0.4;
  zl_veto_taus.selected   = &ucsbsusy::TauF::ishpsvetotau;
  zl_veto_taus.setConfig();

  ol_veto_taus = zl_veto_taus;
  ol_veto_taus.setConfig();
}

cfgSet::PhotonConfig cfgSet::zl_sel_photons("zl_sel_photons");

void cfgSet::loadDefaultPhotonConfigurations() {
  zl_sel_photons.minPt    = 180;
  zl_sel_photons.maxEta   = 2.5;
  zl_sel_photons.selected = &ucsbsusy::PhotonF::isloose;
  zl_sel_photons.usePixelSeedVeto = true;
  zl_sel_photons.useElectronVeto  = false;
  zl_sel_photons.setConfig();
}

cfgSet::CorrectionConfig cfgSet::standardCorrections("standardCorrections");

void cfgSet::loadDefaultCorrections() {
  standardCorrections.ttbarCorrections      = ucsbsusy::TtbarCorrectionSet::NULLOPT;
  standardCorrections.ttbarCorrectionFile   =  TString::Format("%s/src/data/corrections/ttbarCorr.root",CMSSW_BASE);
  standardCorrections.eventCorrections      = ucsbsusy::EventCorrectionSet::PU;
  standardCorrections.eventCorrectionFile   =  TString::Format("%s/src/data/corrections/eventCorr.root",CMSSW_BASE);
  standardCorrections.puCorrections         = ucsbsusy::EventCorrectionSet::NULLOPT;
  standardCorrections.puCorrectionFile      =  TString::Format("%s/src/data/corrections/puCorr.root",CMSSW_BASE);
  standardCorrections.leptonCorrections     = ucsbsusy::LeptonCorrectionSet::LEP;
  standardCorrections.leptonCorrectionFile  =  TString::Format("%s/src/data/corrections/lepCorr.root",CMSSW_BASE);

  standardCorrections.jetAndMETCorrections  = ucsbsusy::JetAndMETCorrectionSet::NULLOPT;
  standardCorrections.jetResCorr            = 1;
  standardCorrections.jetResFile            = TString::Format("%s/src/data/corrections/ak4JetResTrends.root",CMSSW_BASE);

  standardCorrections.bTagCorrections       = ucsbsusy::BTagCorrectionSet::NULLOPT;
  standardCorrections.bTagEffFile           =  TString::Format("%s/src/data/corrections/csvEffs.root",CMSSW_BASE);
  standardCorrections.bTagSFFile            =  TString::Format("%s/src/data/corrections/CSVv2.csv",CMSSW_BASE);
  standardCorrections.heavyBTagCorrType     = ucsbsusy::NOMINAL;
  standardCorrections.lightBTagCorrType     = ucsbsusy::NOMINAL;
  standardCorrections.setConfig();
}

cfgSet::ConfigSet cfgSet::zl_search_set;
cfgSet::ConfigSet cfgSet::zl_lepton_set;
cfgSet::ConfigSet cfgSet::zl_dilepton_set;
cfgSet::ConfigSet cfgSet::zl_photon_set;
cfgSet::ConfigSet cfgSet::ol_search_set;


void cfgSet::loadDefaultConfigurations() {
  loadDefaultJetConfigurations();
  loadDefaultLeptonConfigurations();
  loadDefaultTrackConfigurations();
  loadDefaultTauConfigurations();
  loadDefaultPhotonConfigurations();
  loadDefaultCorrections();

  zl_search_set.jets            = zl_search_jets;
  zl_search_set.vetoedLeptons   = zl_veto_leptons;
  zl_search_set.selectedLeptons = zl_sel_leptons;
  zl_search_set.vetoedTracks    = zl_veto_tracks;
  zl_search_set.vetoedTaus      = zl_veto_taus;
  zl_search_set.corrections     = standardCorrections;

  zl_lepton_set.jets            = zl_lepton_jets;
  zl_lepton_set.selectedLeptons = zl_sel_leptons;
  zl_lepton_set.corrections     = standardCorrections;
  zl_lepton_set.vetoedLeptons   = zl_sel_leptons;
  zl_lepton_set.vetoedTracks    = zl_veto_tracks;
  zl_lepton_set.corrections     = standardCorrections;

  zl_dilepton_set.jets            = zl_dilepton_jets;
  zl_dilepton_set.selectedLeptons = zl_loose_leptons;
  zl_dilepton_set.vetoedTracks    = zl_veto_tracks;
  zl_dilepton_set.vetoedTaus      = zl_veto_taus;
  zl_dilepton_set.corrections     = standardCorrections;

  zl_photon_set.jets            = zl_photon_jets;
  zl_photon_set.vetoedLeptons   = zl_veto_leptons;
  zl_photon_set.vetoedTracks    = zl_veto_tracks;
  zl_photon_set.vetoedTaus      = zl_veto_taus;
  zl_photon_set.selectedPhotons = zl_sel_photons;
  zl_photon_set.corrections     = standardCorrections;

  ol_search_set.jets            = ol_search_jets;
  ol_search_set.selectedLeptons = ol_sel_leptons;
  ol_search_set.vetoedLeptons   = ol_veto_leptons;
  ol_search_set.vetoedTracks    = ol_veto_tracks;
  ol_search_set.vetoedTaus      = ol_veto_taus;
  ol_search_set.corrections     = standardCorrections;
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
  ol_search_set.jsonFile = jsonfile;
  ol_search_set.jsonProcessing = new JSONProcessing(jsonfile);
}



