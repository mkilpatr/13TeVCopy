//--------------------------------------------------------------------------------------------------
// 
// DefaultAnalyzer
// 
// Inherits from BaseTreeAnalyzer and implements a default selection and cleaning on the objects.
// 
// DefaultAnalyzer.cc created on Mon Nov 3 22:59:28 CET 2014 
// 
//--------------------------------------------------------------------------------------------------

#include "AnalysisBase/TreeAnalyzer/interface/DefaultAnalyzer.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"


using namespace ucsbsusy;

const float DefaultAnalyzer::minElePt	= 20.0;
const float DefaultAnalyzer::minMuPt	= 20.0;
const float DefaultAnalyzer::minTauPt	= 20.0;
const float DefaultAnalyzer::minJetPt	= 30.0;
const float DefaultAnalyzer::minBJetPt	= 30.0;
const float DefaultAnalyzer::maxEleEta	= 2.5;
const float DefaultAnalyzer::maxMuEta	= 2.4;
const float DefaultAnalyzer::maxTauEta	= 2.3;
const float DefaultAnalyzer::maxJetEta	= 2.4;
const float DefaultAnalyzer::maxBJetEta	= 2.4;
const float DefaultAnalyzer::minDR	= 0.4;

//--------------------------------------------------------------------------------------------------
DefaultAnalyzer::DefaultAnalyzer(TString fileName, TString treeName, bool isMCTree, TString readOption) : 
  BaseTreeAnalyzer(fileName, treeName, isMCTree, readOption),
  met(0),
  cleanJetsvLeptons(true), 
  cleanJetsvTaus(false)
{}

//--------------------------------------------------------------------------------------------------
void DefaultAnalyzer::defaultLoad()
{
  load(EVTINFO);
  load(AK4JETS);
  load(ELECTRONS);
  load(MUONS);
  load(TAUS);
}
//--------------------------------------------------------------------------------------------------
inline bool DefaultAnalyzer::isGoodJet(const RecoJetF& jet) const {
  return (jet.pt() > minJetPt && fabs(jet.eta()) < maxJetEta);
}
//--------------------------------------------------------------------------------------------------
inline bool DefaultAnalyzer::isMediumBJet(const RecoJetF& jet) const {
  return (jet.pt() > minJetPt && fabs(jet.eta()) < maxBJetEta  && jet.csv() > defaults::CSV_MEDIUM );
}
//--------------------------------------------------------------------------------------------------
inline bool DefaultAnalyzer::isTightBJet(const RecoJetF& jet) const {
  return (jet.pt() > minJetPt && fabs(jet.eta()) < maxBJetEta  && jet.csv() > defaults::CSV_TIGHT );
}
//--------------------------------------------------------------------------------------------------
inline bool DefaultAnalyzer::isGoodElectron(const ElectronF& electron) const {
  return (electron.pt() > minElePt && fabs(electron.scEta()) < maxEleEta && electron.isgoodpogelectron());
}
//--------------------------------------------------------------------------------------------------
inline bool DefaultAnalyzer::isGoodMuon(const MuonF& muon) const {
  return (muon.pt() > minMuPt && fabs(muon.eta()) < maxMuEta && muon.isgoodpogmuon());
}
//--------------------------------------------------------------------------------------------------
inline bool DefaultAnalyzer::isGoodTau(const TauF& tau) const {
  return (tau.pt() > minTauPt && fabs(tau.eta()) < maxTauEta && tau.isgoodpogtau());
}

//--------------------------------------------------------------------------------------------------
void DefaultAnalyzer::process()
{
  leptons.resize(0);
  leptons.reserve(electronReader.electrons.size() + muonReader.muons.size());
  taus.clear();
  taus.reserve(tauReader.taus.size());
  jets.clear();
  jets.reserve(ak4Reader.recoJets.size());
  bJets.clear();
  nonBJets.clear();
  nonBJets.reserve(ak4Reader.recoJets.size());


  if(evtInfoReader.isLoaded())
    met = &evtInfoReader.met;

  if(electronReader.isLoaded())
    for(auto& electron : electronReader.electrons)
      if(isGoodElectron(electron))
        leptons.push_back(&electron);

  if(muonReader.isLoaded())
    for(auto& muon : muonReader.muons)
      if(isGoodMuon(muon))
        leptons.push_back(&muon);

  if(tauReader.isLoaded())
    for(auto& tau : tauReader.taus)
      if(isGoodTau(tau))
        taus.push_back(&tau);

  if(ak4Reader.isLoaded())
    for(auto& jet : ak4Reader.recoJets) {
      if(!isGoodJet(jet)) continue;

      bool cleanjet = true;
      if(cleanJetsvLeptons) {
        for(const auto* glep : leptons) {
          if(PhysicsUtilities::deltaR(*glep, jet) < minDR) {
            cleanjet = false;
            break;
          }
        }
      }
      if(cleanJetsvTaus) {
        for(const auto* gtau : taus) {
          if(PhysicsUtilities::deltaR(*gtau, jet) < minDR) {
            cleanjet = false;
            break;
          }
        }
      }
      if(!cleanjet) continue;

      jets.push_back(&jet);
      if(isMediumBJet(jet))
        bJets.push_back(&jet);
      else
        nonBJets.push_back(&jet);
    }
}

//--------------------------------------------------------------------------------------------------
bool DefaultAnalyzer::nextEvent(int reportFrequency)
{

  // get collections for this event
  bool keepgoing = BaseTreeAnalyzer::nextEvent(reportFrequency);

  // apply object selection/cleaning
  if(keepgoing) process();

  return keepgoing;

}
