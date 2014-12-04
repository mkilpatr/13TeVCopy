//--------------------------------------------------------------------------------------------------
// 
// BaseTreeAnalyzer
// 
//
// 
//--------------------------------------------------------------------------------------------------

#include "AnalysisBase/TreeAnalyzer/interface/BaseTreeAnalyzer.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
#include "AnalysisTools/TreeReader/interface/Defaults.h"

using namespace std;
using namespace ucsbsusy;

//--------------------------------------------------------------------------------------------------
BaseTreeAnalyzer::BaseTreeAnalyzer(TString fileName, TString treeName, bool isMCTree, TString readOption) : isLoaded_(false), reader(fileName,treeName,readOption),
    run               (0),
    lumi              (0),
    event             (0),
    nPV               (0),
    rho               (0),
    nLeptons          (0),
    nTaus             (0),
    nJets             (0),
    nBJets            (0),
    met               (0),
    isMC_             (isMCTree),
    cleanJetsvLeptons_(false),
    cleanJetsvTaus_   (false),
    minElePt          (20.0),
    minMuPt           (20.0),
    minTauPt          (20.0),
    minJetPt          (30.0),
    minBJetPt         (30.0),
    maxEleEta         (2.5 ),
    maxMuEta          (2.4 ),
    maxTauEta         (2.3 ),
    maxJetEta         (2.4 ),
    maxBJetEta        (2.4 ),
    minJetLepDR       (0.4 )
{
  clog << "Running over: " << (isMC_ ? "MC" : "data") <<endl;
}

//--------------------------------------------------------------------------------------------------
void BaseTreeAnalyzer::load(VarType type, int options, string branchName)
{
  switch (type) {
    case EVTINFO : {
      reader.load(&evtInfoReader, 0, "");
      break;
    }
    case AK4JETS : {
      int defaultOptions = JetReader::defaultOptions | (isMC() ? JetReader::LOADGEN : JetReader::NULLOPT);
      reader.load(&ak4Reader, options < 0 ? defaultOptions : options, branchName == "" ? defaults::BRANCH_AK4JETS : branchName);
      break;
    }
    case ELECTRONS : {
      int defaultOptions = ElectronReader::defaultOptions;
      reader.load(&electronReader, options < 0 ? defaultOptions : options, branchName == "" ? defaults::BRANCH_ELECTRONS : branchName );
      break;
    }
    case MUONS : {
      int defaultOptions = MuonReader::defaultOptions;
      reader.load(&muonReader, options < 0 ? defaultOptions : options, branchName == "" ? defaults::BRANCH_MUONS : branchName );
      break;
    }
    case TAUS : {
      int defaultOptions = TauReader::defaultOptions;
      reader.load(&tauReader, options < 0 ? defaultOptions : options, branchName == "" ? defaults::BRANCH_TAUS : branchName );
      break;
    }
    case GENPARTICLES : {
      int defaultOptions = GenParticleReader::defaultOptions;
      reader.load(&genParticleReader, options < 0 ? defaultOptions : options, branchName == "" ? defaults::BRANCH_GENPARTS : branchName );
      break;
    }
    default : {
      cout << endl << "No settings for type: " << type << " found!" << endl;
      break;
    }
  }
}
//--------------------------------------------------------------------------------------------------
void BaseTreeAnalyzer::loadVariables()
{
  load(EVTINFO);
  load(AK4JETS);
  load(ELECTRONS);
  load(MUONS);
  load(TAUS);
  if(isMC()) load(GENPARTICLES);
}
//--------------------------------------------------------------------------------------------------
void BaseTreeAnalyzer::processVariables()
{
  leptons.clear();
  leptons.reserve(electronReader.electrons.size() + muonReader.muons.size());
  taus.clear();
  taus.reserve(tauReader.taus.size());
  jets.clear();
  jets.reserve(ak4Reader.recoJets.size());
  bJets.clear();
  nonBJets.clear();
  nonBJets.reserve(ak4Reader.recoJets.size());


  if(evtInfoReader.isLoaded()) {
    run   = evtInfoReader.run;
    lumi  = evtInfoReader.lumi;
    event = evtInfoReader.event;
    nPV   = evtInfoReader.nPV;
    rho   = evtInfoReader.rho;
    met   = &evtInfoReader.met;
  }

  if(electronReader.isLoaded())
    for(auto& electron : electronReader.electrons)
      if(isGoodElectron(electron))
        leptons.push_back(&electron);

  if(muonReader.isLoaded())
    for(auto& muon : muonReader.muons)
      if(isGoodMuon(muon))
        leptons.push_back(&muon);

  std::sort(leptons.begin(), leptons.end(), PhysicsUtilities::greaterPTDeref<LeptonF>());

  if(tauReader.isLoaded())
    for(auto& tau : tauReader.taus)
      if(isGoodTau(tau))
        taus.push_back(&tau);

  if(ak4Reader.isLoaded())
    for(auto& jet : ak4Reader.recoJets) {
      if(!isGoodJet(jet)) continue;

      bool cleanjet = true;
      if(cleanJetsvLeptons_) {
        for(const auto* glep : leptons) {
          if(PhysicsUtilities::deltaR(*glep, jet) < minJetLepDR) {
            cleanjet = false;
            break;
          }
        }
      }
      if(cleanJetsvTaus_) {
        for(const auto* gtau : taus) {
          if(PhysicsUtilities::deltaR(*gtau, jet) < minJetLepDR) {
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

  nLeptons = leptons.size();
  nTaus    = taus.size();
  nJets    = jets.size();
  nBJets   = bJets.size();


  if(genParticleReader.isLoaded()){
    genParts.clear();
    genParts.reserve(genParticleReader.genParticles.size());
    for(auto& p : genParticleReader.genParticles) genParts.push_back(&p);
  }

}
//--------------------------------------------------------------------------------------------------
void BaseTreeAnalyzer::analyze(int reportFrequency)
{
  loadVariables();
  isLoaded_ = true;

  while(reader.nextEvent(reportFrequency)){
    processVariables();
    runEvent();
  }
}
//--------------------------------------------------------------------------------------------------
inline bool BaseTreeAnalyzer::isMediumBJet(const RecoJetF& jet) const {
  return (jet.pt() > minJetPt && fabs(jet.eta()) < maxBJetEta  && jet.csv() > defaults::CSV_MEDIUM );
}
//--------------------------------------------------------------------------------------------------
inline bool BaseTreeAnalyzer::isTightBJet(const RecoJetF& jet) const {
  return (jet.pt() > minJetPt && fabs(jet.eta()) < maxBJetEta  && jet.csv() > defaults::CSV_TIGHT );
}
//--------------------------------------------------------------------------------------------------
inline bool BaseTreeAnalyzer::isGoodElectron(const ElectronF& electron) const {
  return (electron.pt() > minElePt && fabs(electron.scEta()) < maxEleEta && electron.isgoodpogelectron());
}
//--------------------------------------------------------------------------------------------------
inline bool BaseTreeAnalyzer::isGoodMuon(const MuonF& muon) const {
  return (muon.pt() > minMuPt && fabs(muon.eta()) < maxMuEta && muon.isgoodpogmuon());
}
//--------------------------------------------------------------------------------------------------
inline bool BaseTreeAnalyzer::isGoodTau(const TauF& tau) const {
  return (tau.pt() > minTauPt && fabs(tau.eta()) < maxTauEta && tau.isgoodpogtau());
}
