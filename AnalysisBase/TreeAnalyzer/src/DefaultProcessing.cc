
#include "AnalysisBase/TreeAnalyzer/interface/DefaultProcessing.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"


using namespace std;
using namespace ucsbsusy;

bool cfgSet::isSelBJet   (const ucsbsusy::RecoJetF& jet, const JetConfig& conf, const float minCSV ){
  if(jet.csv() <= (minCSV < -9999 ? conf.defaultCSV : minCSV  ) ) return false;
  return (jet.pt() > conf.minBJetPt && fabs(jet.eta()) < conf.maxBJetEta);
}

bool cfgSet::isSelElectron(const ucsbsusy::ElectronF& electron, const LeptonConfig& conf){
  return (electron.pt() > conf.minEPt && fabs(electron.eta()) < conf.maxEEta && (electron.*conf.selectedElectron)());
}

bool cfgSet::isSelMuon(const ucsbsusy::MuonF& muon, const LeptonConfig& conf){
  if(conf.maxMuD0 > 0 && fabs(muon.d0()) >= conf.maxMuD0) return false;
  if(conf.maxMuDz > 0 && fabs(muon.dz()) >= conf.maxMuDz) return false;
  return (muon.pt() > conf.minMuPt && fabs(muon.eta()) < conf.maxMuEta && (muon.*conf.selectedMuon)());
}

bool cfgSet::isSelTrack(const ucsbsusy::PFCandidateF& track, const TrackConfig& conf){
  if(conf.maxDz > 0 && fabs(track.dz()) >= conf.maxDz) return false;
  return (track.pt() > conf.minPt && fabs(track.eta()) < conf.maxEta && (track.*conf.selected)());
}

bool cfgSet::isSelPhoton(const ucsbsusy::PhotonF& pho, const PhotonConfig& conf       ){
  return (pho.pt() > conf.minPt && fabs(pho.eta()) < conf.maxEta && (pho.*conf.selected)());
}

void cfgSet::selectLeptons(std::vector<ucsbsusy::LeptonF*>& selectedLeptons, std::vector<ucsbsusy::LeptonF*> allLeptons, const LeptonConfig& conf){
  if(!conf.isConfig())
    throw std::invalid_argument("config::selectLeptons(): You want to do selecting but have not yet configured the selection!");

  selectedLeptons.clear();

  for(auto* lepton : allLeptons){
    if (lepton->ismuon() ? isSelMuon(*(MuonF*)lepton, conf): isSelElectron(*(ElectronF*)lepton, conf))
      selectedLeptons.push_back(lepton);
  }

}

void cfgSet::selectTracks(std::vector<ucsbsusy::PFCandidateF*>& selectedTracks, ucsbsusy::PFCandidateFCollection& allTracks, const TrackConfig& conf){
  if(!conf.isConfig())
    throw std::invalid_argument("config::selectTracks(): You want to do selecting but have not yet configured the selection!");

  selectedTracks.clear();

  for(auto& pfc : allTracks) {
    if(isSelTrack(pfc,conf))
      selectedTracks.push_back(&pfc);
  }
}

void cfgSet::selectPhotons(std::vector<ucsbsusy::PhotonF*>& selectedPhotons, ucsbsusy::PhotonFCollection& allPhotons, const PhotonConfig& conf){
  if(!conf.isConfig())
    throw std::invalid_argument("config::selectPhotons(): You want to do selecting but have not yet configured the selection!");

  selectedPhotons.clear();

  for(auto& pfc : allPhotons) {
    if(isSelPhoton(pfc,conf))
      selectedPhotons.push_back(&pfc);
  }
}

void cfgSet::selectJets(std::vector<ucsbsusy::RecoJetF*>& jets, std::vector<ucsbsusy::RecoJetF*>* bJets, std::vector<ucsbsusy::RecoJetF*>* nonBJets,
    ucsbsusy::RecoJetFCollection& allJets, const std::vector<ucsbsusy::LeptonF*>* selectedLeptons, const std::vector<ucsbsusy::PhotonF*>* selectedPhotons, const JetConfig&  conf){
  if(!conf.isConfig())
    throw std::invalid_argument("config::selectJets(): You want to do selecting but have not yet configured the selection!");

  jets.clear(); jets.reserve(allJets.size());
  if(bJets){bJets->clear();}
  if(nonBJets){nonBJets->clear(); nonBJets->reserve( std::max(2,int(jets.size())) -2);}

  vector<bool> vetoJet(allJets.size(),false);

  if(conf.cleanJetsvSelectedLeptons) {
    if(selectedLeptons == 0)
      throw std::invalid_argument("config::selectJets(): You want to do lepton cleaning but have not given a lepton list to clean!");

    for(const auto* glep : *selectedLeptons) {
      double nearDR = 0;
      int near = PhysicsUtilities::findNearestDR(*glep,allJets,nearDR,conf.cleanJetsMaxDR);
      if(near >= 0){
        vetoJet[near] = true;
      }
    }
  }

  if(conf.cleanJetsvSelectedPhotons) {
    if(selectedPhotons == 0)
      throw std::invalid_argument("config::selectJets(): You want to do cleaning but have not given a list to clean with!");

    for(const auto* glep : *selectedPhotons) {
      double nearDR = 0;
      int near = PhysicsUtilities::findNearestDR(*glep,allJets,nearDR,conf.cleanJetsMaxDR);
      if(near >= 0){
        vetoJet[near] = true;
      }
    }
  }


  for(unsigned int iJ = 0; iJ < allJets.size(); ++iJ){
    auto& jet = allJets[iJ];
    if(vetoJet[iJ]) continue;
    if(jet.pt() <= conf.minPt ) continue;
    if(fabs(jet.eta()) >= conf.maxEta ) continue;
    if(conf.applyJetID && !jet.looseid()) continue;

    jets.push_back(&jet);

    if(bJets || nonBJets){
      if(isSelBJet(jet,conf)){
        if(bJets)bJets->push_back(&jet);
      }
      else{
        if(nonBJets)nonBJets->push_back(&jet);
      }
    }
  }
}


double cfgSet::adHocPUCorr(double pt,double eta,double area, double rho){
  double constant = 1.08;
  double correction = .35;
  if(pt < 30){
    if(TMath::Abs(eta) < 1.5){
      constant = 1.05;
      correction = .50;
    } else {
      constant = 1.10;
    }
  }
  return max(0.,constant*(pt - rho*area*correction) );
}
void  cfgSet::applyAdHocPUCorr(ucsbsusy::RecoJetFCollection& jets, const std::vector<float>& jetAreas, const float rho){
  for(auto& j : jets)
    j.setP4(CylLorentzVectorF(adHocPUCorr(j.pt(),j.eta(),jetAreas[j.index()],rho),
        j.eta(),j.phi(),j.mass()) );
  std::sort(jets.begin(), jets.end(), PhysicsUtilities::greaterPT<RecoJetF>());
}


void cfgSet::processMET(ucsbsusy::MomentumF& met, const std::vector<ucsbsusy::LeptonF*>* selectedLeptons, const std::vector<ucsbsusy::PhotonF*>* selectedPhotons, const METConfig& conf){
  if(!conf.isConfig())
    throw std::invalid_argument("config::processMET(): You want to do processing but have not yet configured the processing!");

  if(conf.addSelectedLeptonsToMET){
    if(selectedLeptons == 0)
      throw std::invalid_argument("config::selectJets(): You want to add leptons to met but have not given a lepton list!");

    for(const auto* glep : *selectedLeptons) {
      met.p4() += glep->p4();
    }
  }

  if(conf.addSelectedPhotonsToMET){
    if(selectedPhotons == 0)
      throw std::invalid_argument("config::selectJets(): You want to add photons to met but have not given a photon list!");

    for(const auto* glep : *selectedPhotons) {
      met.p4() += glep->p4();
    }
  }
}
