#ifndef ANALYSISBASE_TREEANALYZER_DEFAULTCONFIGURATIONS_H
#define ANALYSISBASE_TREEANALYZER_DEFAULTCONFIGURATIONS_H


#include "AnalysisBase/TreeAnalyzer/interface/ConfigurationBase.h"
#include "AnalysisTools/TreeReader/interface/Defaults.h"

namespace cfgSet {
  extern char* CMSSW_BASE;

  extern JetConfig zl_search_jets;
  extern JetConfig zl_photon_jets;
  extern JetConfig zl_lepton_jets;
  extern JetConfig zl_dilepton_jets;
  extern JetConfig sl_search_jets;

  void loadDefaultJetConfigurations();

  extern LeptonConfig zl_sel_leptons ;
  extern LeptonConfig zl_ctr_leptons;
  extern LeptonConfig zl_ctr_sec_leptons;
  extern LeptonConfig zl_ctr_diLep_leptons;
  extern LeptonConfig sl_sel_leptons ;
  extern LeptonConfig sl_sel_sec_leptons;

  void loadDefaultLeptonConfigurations();

  extern TrackConfig zl_veto_tracks;
  extern TrackConfig sl_veto_tracks;

  void loadDefaultTrackConfigurations();

  extern TauConfig zl_veto_taus;
  extern TauConfig sl_veto_taus;
  
  void loadDefaultTauConfigurations();

  extern PhotonConfig zl_sel_photons;

  void loadDefaultPhotonConfigurations();

  extern CorrectionConfig standardCorrections;

  void loadDefaultCorrections();


  extern ConfigSet zl_search_set;
  extern ConfigSet zl_lepton_set;
  extern ConfigSet zl_photon_set;
  extern ConfigSet zl_dilepton_set;
  extern ConfigSet sl_search_set;

  void loadDefaultConfigurations();

  void setJSONFile(const TString jsonfile);

}



#endif
