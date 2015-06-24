#ifndef ANALYSISBASE_TREEANALYZER_DEFAULTCONFIGURATIONS_H
#define ANALYSISBASE_TREEANALYZER_DEFAULTCONFIGURATIONS_H


#include "AnalysisBase/TreeAnalyzer/interface/ConfigurationBase.h"
#include "AnalysisTools/TreeReader/interface/Defaults.h"

namespace cfgSet {
  extern JetConfig zl_search_jets;
  extern JetConfig zl_photon_jets;
  extern JetConfig zl_lepton_jets;
  extern JetConfig ol_search_jets;

  void loadDefaultJetConfigurations();


  extern LeptonConfig zl_sel_leptons ;
  extern LeptonConfig zl_veto_leptons;
  extern LeptonConfig ol_sel_leptons ;
  extern LeptonConfig ol_veto_leptons;

  void loadDefaultLeptonConfigurations();

  extern TrackConfig zl_veto_tracks;
  extern TrackConfig ol_veto_tracks;

  void loadDefaultTrackConfigurations();

  extern PhotonConfig zl_sel_photons;

  void loadDefaultPhotonConfigurations();

  extern METConfig zl_search_met;
  extern METConfig zl_lplus_met ;
  extern METConfig zl_pplus_met ;
  extern METConfig ol_search_met;

  void loadDefaultMETConfigurations() ;

  extern ConfigSet zl_search_set;
  extern ConfigSet zl_lepton_set;
  extern ConfigSet zl_photon_set;
  extern ConfigSet ol_search_set;

  void loadDefaultConfigurations();

}



#endif
