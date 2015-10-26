
#include "AnalysisBase/TreeAnalyzer/interface/DefaultProcessing.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
#include "AnalysisTools/KinematicVariables/interface/JetKinematics.h"


using namespace std;
using namespace ucsbsusy;

bool cfgSet::isSelGenJet   (const ucsbsusy::GenJetF& jet, const JetConfig& conf){
  return (jet.pt() > conf.minPt && fabs(jet.eta()) < conf.maxEta);
}


bool cfgSet::isSelBJet   (const ucsbsusy::RecoJetF& jet, const JetConfig& conf, const float minCSV ){
  if(jet.csv() <= (minCSV < -9999 ? conf.defaultCSV : minCSV  ) ) return false;
  return (jet.pt() > conf.minBJetPt && fabs(jet.eta()) < conf.maxBJetEta);
}

bool cfgSet::isSelElectron(const ucsbsusy::ElectronF& electron, const LeptonConfig& conf){
  if(conf.maxED0 > 0 && fabs(electron.d0()) >= conf.maxED0) return false;
  if(conf.maxEDz > 0 && fabs(electron.dz()) >= conf.maxEDz) return false;
  return (electron.pt() > conf.minEPt && fabs(electron.eta()) < conf.maxEEta && (electron.*conf.selectedElectron)());
}

bool cfgSet::isSelMuon(const ucsbsusy::MuonF& muon, const LeptonConfig& conf){
  if(conf.maxMuD0 > 0 && fabs(muon.d0()) >= conf.maxMuD0) return false;
  if(conf.maxMuDz > 0 && fabs(muon.dz()) >= conf.maxMuDz) return false;
  return (muon.pt() > conf.minMuPt && fabs(muon.eta()) < conf.maxMuEta && (muon.*conf.selectedMuon)());
}

bool cfgSet::isSelTrack(ucsbsusy::PFCandidateF& track, const ucsbsusy::MomentumF* met, const TrackConfig& conf){
  if(conf.maxDz > 0 && fabs(track.dz()) >= conf.maxDz) return false;
  MomentumF* cand = new MomentumF(track.p4());
  if(track.nearPhoton().pt() > -1.) cand->setP4(cand->p4() + track.nearPhoton().p4());
  track.setMt(JetKinematics::transverseMass(*cand,*met));
  if(conf.mtPresel && JetKinematics::transverseMass(*cand,*met) > defaults::TAU_MTCUT_VETO) return false;
  return (track.pt() > conf.minPt && fabs(track.eta()) < conf.maxEta && (track.*conf.selected)());
}

bool cfgSet::isSelTau(const ucsbsusy::TauF& tau, const std::vector<ucsbsusy::LeptonF*>& selectedLeptons, const TauConfig& conf){
  if(conf.minPt  > 0 && tau.pt()     < conf.minPt ) return false;
  if(conf.maxEta > 0 && tau.absEta() > conf.maxEta) return false;
  bool pass = (tau.*conf.selected)();
  // future option to use > 1 selected lepton here
  if(conf.requireOppositeQToSelLepton && selectedLeptons.size() > 0) pass = pass & (tau.q()*selectedLeptons.at(0)->q() < 0);
  if(conf.minDeltaRFromSelLepton > 0  && selectedLeptons.size() > 0) pass = pass & (PhysicsUtilities::deltaR(tau.p4(), selectedLeptons.at(0)->p4()) > conf.minDeltaRFromSelLepton);
  return pass;
}

bool cfgSet::isSelPhoton(const ucsbsusy::PhotonF& pho, const PhotonConfig& conf       ){
  if (conf.usePixelSeedVeto && pho.hasPixelSeed())         return false;
  if (conf.useElectronVeto  && (!pho.passElectronVeto()) ) return false;
  return (pho.pt() > conf.minPt && fabs(pho.eta()) < conf.maxEta && (pho.*conf.selected)());
}

bool cfgSet::isSelTaggedTop(const ucsbsusy::CMSTopF& top){
    bool boolVal = false;
    float topRawMass   = top.topRawMass();
    float inTopMinMass  = top.topMinMass();
    int   topNsubJets = top.topNsubJets();

    if ( ((topRawMass)>140.) && ((topRawMass)<250.) && ((inTopMinMass)>50.) && ((topNsubJets)>=3) ) { boolVal = true; }
    return boolVal;
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

void cfgSet::selectTracks(std::vector<ucsbsusy::PFCandidateF*>& selectedTracks, ucsbsusy::PFCandidateFCollection& allTracks, const MomentumF* met, const TrackConfig& conf){
  if(!conf.isConfig())
    throw std::invalid_argument("config::selectTracks(): You want to do selecting but have not yet configured the selection!");

  selectedTracks.clear();

  for(auto& pfc : allTracks) {
    if(isSelTrack(pfc,met,conf))
      selectedTracks.push_back(&pfc);
  }
}

// must not preceed selectedLeptons
void cfgSet::selectTaus(std::vector<ucsbsusy::TauF*>& selectedTaus, std::vector<ucsbsusy::LeptonF*>& selectedLeptons, ucsbsusy::TauFCollection& allTaus, const TauConfig& conf){
  if(!conf.isConfig())
    throw std::invalid_argument("config::selectTaus(): You want to do selecting but have not yet configured the selection!");
    
  selectedTaus.clear();
  
  for(auto& tau : allTaus){
    if(isSelTau(tau,selectedLeptons,conf))
     selectedTaus.push_back(&tau);
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
			ucsbsusy::RecoJetFCollection& allJets, const std::vector<ucsbsusy::LeptonF*>* selectedLeptons, 
			const std::vector<ucsbsusy::LeptonF*>* vetoedLeptons, const std::vector<ucsbsusy::PhotonF*>* selectedPhotons, 
			const std::vector<ucsbsusy::PFCandidateF*>* vetoedTracks, const JetConfig&  conf){
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
      int near = PhysicsUtilities::findNearestDR(*glep,allJets,nearDR,conf.cleanJetsMaxDR,conf.minPt,0,allJets.size());
      if(near >= 0){
        vetoJet[near] = true;
      }
    }
  }

  if(conf.cleanJetsvVetoedLeptons) {
    if(vetoedLeptons == 0)
      throw std::invalid_argument("config::selectJets(): You want to do lepton cleaning but have not given a lepton list to clean!");

    for(const auto* glep : *vetoedLeptons) {
      double nearDR = 0;
      int near = PhysicsUtilities::findNearestDR(*glep,allJets,nearDR,conf.cleanJetsMaxDR,conf.minPt,0,allJets.size());
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
      int near = PhysicsUtilities::findNearestDR(*glep,allJets,nearDR,conf.cleanJetsMaxDR,conf.minPt,0,allJets.size());
      if(near >= 0){
        vetoJet[near] = true;
      }
    }
  }


  if(conf.cleanJetsvVetoedTracks) {
    if(vetoedTracks == 0)
      throw std::invalid_argument("config::selectJets(): You want to do track cleaning but have not given a track list to clean!");

    for(const auto* glep : *vetoedTracks) {
      double nearDR = 0;
      int near = PhysicsUtilities::findNearestDR(*glep,allJets,nearDR,conf.cleanJetsMaxDR,conf.minPt,0,allJets.size());
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

/*
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
}*/
