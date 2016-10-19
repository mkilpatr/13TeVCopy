#ifndef ANALYSISBASE_TREEANALYZER_DEFAULTPROCESSING_H
#define ANALYSISBASE_TREEANALYZER_DEFAULTPROCESSING_H


#include "AnalysisBase/TreeAnalyzer/interface/ConfigurationBase.h"
#include "AnalysisTools/DataFormats/interface/Jet.h"
#include "AnalysisTools/DataFormats/interface/CMSTop.h"
#include "AnalysisTools/DataFormats/interface/FatJet.h"

namespace cfgSet{
  bool isSelGenJet   (const ucsbsusy::GenJetF& jet, const JetConfig& conf     );
  bool isSelBJet   (const ucsbsusy::RecoJetF& jet, const JetConfig& conf, const float minCSV = -10000       );
  bool isSelElectron(const ucsbsusy::ElectronF& electron, const LeptonSelection::Electron& conf);
  bool isSelMuon(const ucsbsusy::MuonF& muon, const LeptonSelection::Muon& conf);
  bool isSelTrack(ucsbsusy::PFCandidateF& track, const ucsbsusy::MomentumF* met, const TrackConfig& conf);
  bool isSelTau(const ucsbsusy::TauF& tau, const std::vector<ucsbsusy::LeptonF*>& selectedLeptons, const TauConfig& conf);
  bool isSelPhoton(const ucsbsusy::PhotonF& pho, const PhotonConfig& conf       );
  bool isSelTaggedTop(const ucsbsusy::CMSTopF& top); //Config Pending
  bool isSoftDropTagged(const ucsbsusy::FatJetF* fj, float minPT, float minMass, float maxMass, float tau32max, float tau21max, float min_highest_csv=-1);

  void selectLeptons(std::vector<ucsbsusy::LeptonF*>& selectedLeptons  ,std::vector<ucsbsusy::LeptonF*> allLeptons, const LeptonSelection::Electron& electronConf,const LeptonSelection::Muon& muonConf, std::vector<ucsbsusy::LeptonF*>* nonSelectedLeptons = 0);
  void selectTracks(std::vector<ucsbsusy::PFCandidateF*>& selectedTracks, ucsbsusy::PFCandidateFCollection& allTracks, const ucsbsusy::MomentumF* met, const TrackConfig& conf);
  void selectTaus(std::vector<ucsbsusy::TauF*>& selectedTaus, std::vector<ucsbsusy::LeptonF*>& selectedLeptons, ucsbsusy::TauFCollection& allTaus, const TauConfig& conf);
  void selectPhotons(std::vector<ucsbsusy::PhotonF*>& selectedPhotons, ucsbsusy::PhotonFCollection& allPhotons, const PhotonConfig& conf);

  void selectJets(std::vector<ucsbsusy::RecoJetF*>& jets, std::vector<ucsbsusy::RecoJetF*>* bJets, std::vector<ucsbsusy::RecoJetF*>* nonBJets,
		  ucsbsusy::RecoJetFCollection& allJets, const std::vector<ucsbsusy::LeptonF*>* selectedLeptons,
      const std::vector<ucsbsusy::LeptonF*>* primaryLeptons, const std::vector<ucsbsusy::PhotonF*>* selectedPhotons,
		  const std::vector<ucsbsusy::PFCandidateF*>* vetoedTracks, const JetConfig&  conf);
  void selectISRJets(const std::vector<ucsbsusy::RecoJetF*>& jets, std::vector<ucsbsusy::RecoJetF*>* isrJets);

  double adHocPUCorr(double pt,double eta,double area, double rho);
  void  applyAdHocPUCorr(ucsbsusy::RecoJetFCollection& jets, const std::vector<float>& jetAreas, const float rho);

  void sortByCSV(std::vector<ucsbsusy::RecoJetF*> &jets);
  bool passCHFFilter(const std::vector<ucsbsusy::RecoJetF*>& jets);

//  void processMET(ucsbsusy::MomentumF& met, const std::vector<ucsbsusy::LeptonF*>* selectedLeptons, const std::vector<ucsbsusy::PhotonF*>* selectedPhotons, const METConfig& conf);

}



#endif
