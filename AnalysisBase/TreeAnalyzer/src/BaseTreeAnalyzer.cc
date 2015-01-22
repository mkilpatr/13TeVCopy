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
BaseTreeAnalyzer::BaseTreeAnalyzer(TString fileName, TString treeName, bool isMCTree, TString readOption) : isLoaded_(false),isProcessed_(false), reader(fileName,treeName,readOption),
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
    defaultJets       (0),
    cleanJetsvLeptons_(false),
    cleanJetsvTaus_   (false),
    minElePt          (10.0),
    minMuPt           (10.0),
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
    case PUPPIJETS : {
      int defaultOptions = JetReader::defaultOptions | (isMC() ? JetReader::LOADGEN : JetReader::NULLOPT);
      reader.load(&puppiJetsReader, options < 0 ? defaultOptions : options, branchName == "" ? defaults::BRANCH_PUPPIJETS : branchName);
      break;
    }
    case PICKYJETS : {
      int defaultOptions = JetReader::defaultOptions | (isMC() ? JetReader::LOADGEN : JetReader::NULLOPT);
      reader.load(&pickyJetReader, options < 0 ? defaultOptions : options, branchName == "" ? defaults::BRANCH_PICKYJETS : branchName);
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
  load(PICKYJETS);
  load(PUPPIJETS);
  load(ELECTRONS);
  load(MUONS);
  load(TAUS);
  if(isMC()) load(GENPARTICLES);
  setDefaultJets(AK4JETS);
}
//--------------------------------------------------------------------------------------------------
void BaseTreeAnalyzer::processVariables()
{
  isProcessed_ = true;

  if(evtInfoReader.isLoaded()) {
    run   = evtInfoReader.run;
    lumi  = evtInfoReader.lumi;
    event = evtInfoReader.event;
    nPV   = evtInfoReader.nPV;
    rho   = evtInfoReader.rho;
    met   = &evtInfoReader.met;
  }

  if(genParticleReader.isLoaded()){
    genParts.clear();
    genParts.reserve(genParticleReader.genParticles.size());
    for(auto& p : genParticleReader.genParticles) genParts.push_back(&p);
  }

  leptons.clear();
  leptons.reserve(electronReader.electrons.size() + muonReader.muons.size());
  taus.clear();
  taus.reserve(tauReader.taus.size());

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

  nLeptons = leptons.size();
  nTaus    = taus.size();

  jets.clear(); bJets.clear(); nonBJets.clear();
  if(defaultJets && defaultJets->isLoaded()){
    cleanJets(defaultJets,jets,&bJets,&nonBJets);
  }
  nJets    = jets.size();
  nBJets   = bJets.size();


}
//--------------------------------------------------------------------------------------------------
void BaseTreeAnalyzer::analyze(int reportFrequency, int numEvents)
{
  clog << "Running over " << (numEvents < 0 ? "all" : TString::Format("at most %i",numEvents).Data()) << " events"  <<endl;
  loadVariables();
  isLoaded_ = true;

  while(reader.nextEvent(reportFrequency)){
	isProcessed_ = false;
    if(numEvents >= 0 && getEventNumber() >= numEvents) return;
    processVariables();
    runEvent();
  }
}
//--------------------------------------------------------------------------------------------------
void BaseTreeAnalyzer::setDefaultJets(VarType type) {
  switch (type) {
     case AK4JETS : {
       setDefaultJets(&ak4Reader);
       break;
     }
     case PUPPIJETS : {
       setDefaultJets(&puppiJetsReader);
       break;
     }
     case PICKYJETS : {
       setDefaultJets(&pickyJetReader);
       break;
     }
     default : {
       cout << endl << "No valid jet for type: " << type << " found!" << endl;
       break;
     }
   }
}
//--------------------------------------------------------------------------------------------------
bool BaseTreeAnalyzer::isMediumBJet(const RecoJetF& jet) const {
  return (jet.pt() > minJetPt && fabs(jet.eta()) < maxBJetEta  && jet.csv() > defaults::CSV_MEDIUM );
}
//--------------------------------------------------------------------------------------------------
bool BaseTreeAnalyzer::isTightBJet(const RecoJetF& jet) const {
  return (jet.pt() > minJetPt && fabs(jet.eta()) < maxBJetEta  && jet.csv() > defaults::CSV_TIGHT );
}
//--------------------------------------------------------------------------------------------------
bool BaseTreeAnalyzer::isGoodElectron(const ElectronF& electron) const {
  //return (electron.pt() > minElePt && fabs(electron.scEta()) < maxEleEta && electron.isgoodpogelectron());
  return (electron.pt() > minElePt && fabs(electron.scEta()) < maxEleEta && (electron.mvaidtrig()>0.95));
  //return (electron.pt() > minElePt && fabs(electron.scEta()) < maxEleEta);
}
//--------------------------------------------------------------------------------------------------
bool BaseTreeAnalyzer::isGoodMuon(const MuonF& muon) const {
  return (muon.pt() > minMuPt && fabs(muon.eta()) < maxMuEta && muon.isgoodpogmuon());
  //return (muon.pt() > minMuPt && fabs(muon.eta()) < maxMuEta);
}
//--------------------------------------------------------------------------------------------------
bool BaseTreeAnalyzer::isGoodTau(const TauF& tau) const {
  return (tau.pt() > minTauPt && fabs(tau.eta()) < maxTauEta && tau.isgoodpogtau());
}
//--------------------------------------------------------------------------------------------------
void BaseTreeAnalyzer::cleanJets(JetReader * reader, std::vector<RecoJetF*>& jets, std::vector<RecoJetF*>* bJets, std::vector<RecoJetF*>* nonBJets) const {
	if((cleanJetsvLeptons_ || cleanJetsvTaus_) && !isProcessed_)
		throw std::invalid_argument("BaseTreeAnalyzer::cleanJets(): You want to do jet cleaning but have not yet processed variables!");
	jets.clear(); jets.reserve(reader->recoJets.size());
	if(bJets){bJets->clear();}
	if(nonBJets){nonBJets->clear(); nonBJets->reserve( std::max(2,int(jets.size())) -2);}

	for(auto& jet : reader->recoJets) {
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

		if(bJets || nonBJets){
			if(isMediumBJet(jet)){
				if(bJets)bJets->push_back(&jet);
			}
			else{
				if(nonBJets)nonBJets->push_back(&jet);
			}
		}
	}
}
