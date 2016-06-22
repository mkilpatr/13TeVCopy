
#include "AnalysisBase/TreeAnalyzer/interface/DefaultProcessing.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
#include "AnalysisTools/KinematicVariables/interface/JetKinematics.h"


using namespace std;
using namespace ucsbsusy;

bool cfgSet::isSelGenJet   (const ucsbsusy::GenJetF& jet, const JetConfig& conf){
  return (jet.pt() > conf.minPt && fabs(jet.eta()) < conf.maxEta);
}


bool cfgSet::isSelBJet   (const ucsbsusy::RecoJetF& jet, const JetConfig& conf, const float minCSV ){
  if (jet.pt() <= conf.minBJetPt || fabs(jet.eta()) >= conf.maxBJetEta) return false;
  if(jet.csv() > (minCSV < -9999 ? conf.defaultCSV : minCSV  ) ) return true;
  return false;
}

bool cfgSet::isSelElectron(const ucsbsusy::ElectronF& electron, const LeptonSelection::Electron& conf){
  if(conf.maxD0 > 0 && fabs(electron.d0()) >= conf.maxD0) return false;
  if(conf.maxDz > 0 && fabs(electron.dz()) >= conf.maxDz) return false;
  if(electron.pt() <= conf.minPT) return false;
  if(electron.absEta() >= conf.maxETA) return false;
  return (*conf.passID)(&electron) && (*conf.passISO)(&electron);
}

bool cfgSet::isSelMuon(const ucsbsusy::MuonF& muon, const LeptonSelection::Muon& conf){
  if(conf.maxD0 > 0 && fabs(muon.d0()) >= conf.maxD0) return false;
  if(conf.maxDz > 0 && fabs(muon.dz()) >= conf.maxDz) return false;
  if(muon.pt() <= conf.minPT) return false;
  if(muon.absEta() >= conf.maxETA) return false;
  return (*conf.passID)(&muon) && (*conf.passISO)(&muon);
}

bool cfgSet::isSelTrack(ucsbsusy::PFCandidateF& track, const ucsbsusy::MomentumF* met, const TrackConfig& conf){
  if(conf.maxDz > 0 && fabs(track.dz()) >= conf.maxDz) return false;
  MomentumF* cand = new MomentumF(track.p4());
  if(track.nearPhoton().pt() > -1.) cand->setP4(cand->p4() + track.nearPhoton().p4());
  track.setMt(JetKinematics::transverseMass(*cand,*met));
  delete cand;
  if(conf.mtPresel && track.mt() > defaults::TAU_MTCUT_VETO) return false;
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
  return (top.topCmsTopTagMass() > 140.0 && top.topCmsTopTagMass() < 250.0 && top.topMinMass() > 50.0 && top.topNsubJets() >= 3 && top.p4().pt()>=400. && fabs(top.p4().eta())<=2.4);
}

bool cfgSet::isSoftDropTagged(const ucsbsusy::FatJetF* fj, float minPT, float minMass, float maxMass, float tau32max, float tau21max) {
  float tau21 = fj->fjTau1() > 0 ? fj->fjTau2()/fj->fjTau1() : 1e9;
  float tau32 = fj->fjTau2() > 0 ? fj->fjTau3()/fj->fjTau2() : 1e9;
  return (fj->fjSoftDropMass() > minMass && (fj->fjSoftDropMass() < maxMass) && fabs(fj->eta())<=2.4 && (fj->pt()>minPT) && tau32<tau32max && tau21<tau21max);
}

void cfgSet::selectLeptons(std::vector<ucsbsusy::LeptonF*>& selectedLeptons, std::vector<ucsbsusy::LeptonF*> allLeptons, const LeptonSelection::Electron& electronConf,const LeptonSelection::Muon& muonConf, std::vector<ucsbsusy::LeptonF*>* nonSelectedLeptons){
  if(!electronConf.isConfig || !muonConf.isConfig)
    throw std::invalid_argument("config::selectLeptons(): You want to do selecting but have not yet configured the selection!");

  selectedLeptons.clear();

  for(auto* lepton : allLeptons){
    if (lepton->ismuon() ? isSelMuon(*(MuonF*)lepton, muonConf): isSelElectron(*(ElectronF*)lepton, electronConf))
      selectedLeptons.push_back(lepton);
    else if(nonSelectedLeptons)
      nonSelectedLeptons->push_back(lepton);
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
			const std::vector<ucsbsusy::LeptonF*>* primaryLeptons, const std::vector<ucsbsusy::PhotonF*>* selectedPhotons,
			const std::vector<ucsbsusy::PFCandidateF*>* vetoedTracks, const JetConfig&  conf){
  if(!conf.isConfig())
    throw std::invalid_argument("config::selectJets(): You want to do selecting but have not yet configured the selection!");

  jets.clear(); jets.reserve(allJets.size());
  if(bJets){bJets->clear();}
  if(nonBJets){nonBJets->clear(); nonBJets->reserve( std::max(2,int(jets.size())) -2);}

  vector<bool> vetoJet(allJets.size(),false);

  if(conf.cleanJetsvLeptons){
    auto cleanLeptons = conf.ignoreSeconaryLeptonsWhenCleaning ? primaryLeptons : selectedLeptons;
    if(cleanLeptons == 0)
      throw std::invalid_argument("config::selectJets(): You want to do lepton cleaning but have not given a lepton list to clean!");
    for(const auto* glep : *cleanLeptons) {
      double nearDR = 0;
      int near = PhysicsUtilities::findNearestDR(*glep,allJets,nearDR,conf.cleanJetsMaxDR,conf.minPt,0,allJets.size());
      if(near >= 0){
        vetoJet[near] = true;
      }
    }
  }

  if(conf.cleanJetsvPhotons) {
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


  if(conf.cleanJetsvTracks) {
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

void cfgSet::sortByCSV(std::vector<ucsbsusy::RecoJetF*>& jets) {
  std::sort(jets.begin(), jets.end(), [](RecoJetF *a, RecoJetF *b){return a->csv() > b->csv();});
}

bool cfgSet::passCHFFilter(const std::vector<ucsbsusy::RecoJetF*>& jets) {
  bool pass = true;
  for(auto* jet : jets) {
    if(jet->pt() > 20.0 && fabs(jet->eta()) < 2.5 && (!jet->genJet() || jet->genJet()->pt() < 5.0) && jet->chHadFrac() < 0.1) pass = false;
  }
  return pass;
}

void cfgSet::selectISRJets(const std::vector<ucsbsusy::RecoJetF*>& jets, std::vector<ucsbsusy::RecoJetF*>* isrJets) {
  isrJets->clear();
  for (auto *j : jets){
    if (j->csv() < defaults::CSV_LOOSE) isrJets->push_back(j);
  }
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
