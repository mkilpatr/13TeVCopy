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
BaseTreeAnalyzer::BaseTreeAnalyzer(TString fileName, TString treeName, bool isMCTree,ConfigPars * pars, TString readOption) : isLoaded_(false),isProcessed_(false), reader(fileName,treeName,readOption),
    run               (0),
    lumi              (0),
    event             (0),
    weight            (1),
    process           (defaults::NUMPROCESSES),
    nPV               (0),
    rho               (0),
    nSelLeptons       (0),
    nVetoedLeptons    (0),
    nVetoedTaus       (0),
    nJets             (0),
    nBJets            (0),
    met               (0),
    genmet            (0),
    isMC_             (isMCTree),
    defaultJets       (0),
    config            (pars ? *pars : ConfigPars())
{
  clog << "Running over: " << (isMC_ ? "MC" : "data") <<endl;
  setDefaultJets(config.defaultJetCollection);
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
    case PFCANDS : {
      int defaultOptions;
      switch (config.tauVetoPreselection) {
        case MT :
          defaultOptions = PFCandidateReader::LOADRECO | PFCandidateReader::FILLOBJ | PFCandidateReader::LOADTAUVETOMT;
        case DPHI :
          defaultOptions = PFCandidateReader::LOADRECO | PFCandidateReader::FILLOBJ | PFCandidateReader::LOADTAUVETODPHI;
        default : {
          defaultOptions = PFCandidateReader::defaultOptions;
          break;
        }
      }
      reader.load(&pfcandReader, options < 0 ? defaultOptions : options, branchName == "" ? defaults::BRANCH_PFCANDS : branchName );
      break;
    }
    case GENPARTICLES : {
      int defaultOptions = GenParticleReader::defaultOptions;
      reader.load(&genParticleReader, options < 0 ? defaultOptions : options, branchName == "" ? defaults::BRANCH_GENPARTS : branchName );
      break;
    }
    case CORRAL : {
      int defaultOptions = CORRALReader::defaultOptions;
      reader.load(&corralReader, options < 0 ? defaultOptions : options, branchName == "" ? defaults::BRANCH_CORRAL : branchName );
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
  load(ELECTRONS);
  load(MUONS);
  load(PFCANDS);
  if(isMC()) load(GENPARTICLES);
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
    genmet= &evtInfoReader.genmet;
    weight=  evtInfoReader.weight;
    process =  evtInfoReader.process;
  }

  if(genParticleReader.isLoaded()){
    genParts.clear();
    genParts.reserve(genParticleReader.genParticles.size());
    for(auto& p : genParticleReader.genParticles) genParts.push_back(&p);
  }


  selectLeptons(allLeptons,selectedLeptons,vetoedLeptons,vetoedTaus,nSelLeptons,nVetoedLeptons,nVetoedTaus);

  //Picky jet corrections
  if(pickyJetReader.isLoaded() && config.correctPickyPT){
    if(!evtInfoReader.isLoaded())
      throw std::invalid_argument("BaseTreeAnalyzer::processVariables(): EventInfo must be loaded if you want to correct picky jets pt!");
    for(auto& j : pickyJetReader.recoJets)
      j.setP4(CylLorentzVectorF(correctedPickyPT(j.pt(),j.eta(),pickyJetReader.jetarea_->at(j.index()),rho),
          j.eta(),j.phi(),j.mass()) );
    std::sort(pickyJetReader.recoJets.begin(), pickyJetReader.recoJets.end(), PhysicsUtilities::greaterPT<RecoJetF>());
  }

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
  clog << "With default jet type: "  ;
  switch (type) {
     case AK4JETS : {
       setDefaultJets(&ak4Reader);
       clog << "ak4"  <<endl;
       break;
     }
     case PUPPIJETS : {
       setDefaultJets(&puppiJetsReader);
       clog << "puppi"  <<endl;
       break;
     }
     case PICKYJETS : {
       setDefaultJets(&pickyJetReader);
       clog << "picky"  <<endl;
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
  return (jet.pt() > config.minJetPt && fabs(jet.eta()) < config.maxBJetEta  && jet.csv() > defaults::CSV_MEDIUM );
}
//--------------------------------------------------------------------------------------------------
bool BaseTreeAnalyzer::isTightBJet(const RecoJetF& jet) const {
  return (jet.pt() > config.minJetPt && fabs(jet.eta()) < config.maxBJetEta  && jet.csv() > defaults::CSV_TIGHT );
}
//--------------------------------------------------------------------------------------------------
bool BaseTreeAnalyzer::isLooseBJet(const RecoJetF& jet) const {
  return (jet.pt() > config.minJetPt && fabs(jet.eta()) < config.maxBJetEta  && jet.csv() > defaults::CSV_LOOSE );
}
//--------------------------------------------------------------------------------------------------
bool BaseTreeAnalyzer::isSelElectron(const ElectronF& electron) const {
  return (electron.pt() > config.minSelEPt && fabs(electron.scEta()) < config.maxSelEETA && (electron.*config.selectedElectron)());
}
//--------------------------------------------------------------------------------------------------
bool BaseTreeAnalyzer::isVetoElectron(const ElectronF& electron) const {
  return (electron.pt() > config.minVetoEPt && fabs(electron.scEta()) < config.maxVetoEETA && (electron.*config.vetoedElectron)());
}
//--------------------------------------------------------------------------------------------------
bool BaseTreeAnalyzer::isSelMuon(const MuonF& muon) const {
  return (muon.pt() > config.minSelMuPt && fabs(muon.eta()) < config.maxSelMuETA && (muon.*config.selectedMuon)());
}
//--------------------------------------------------------------------------------------------------
bool BaseTreeAnalyzer::isVetoMuon(const MuonF& muon) const {
  return (muon.pt() > config.minVetoMuPt && fabs(muon.eta()) < config.maxVetoMuETA && (muon.*config.vetoedMuon)());
}
//--------------------------------------------------------------------------------------------------
bool BaseTreeAnalyzer::isVetoTau(const PFCandidateF& tau) const {
  return (tau.pt() > config.minVetoTauPt && fabs(tau.eta()) < config.maxVetoTauETA && (tau.*config.vetoedTau)());
}
//--------------------------------------------------------------------------------------------------
void BaseTreeAnalyzer::selectLeptons(
                   std::vector<LeptonF*>& allLeptons,
                   std::vector<LeptonF*>& selectedLeptons,
                   std::vector<LeptonF*>& vetoedLeptons,
                   std::vector<PFCandidateF*>& vetoedTaus,
                   int& nSelLeptons,
                   int& nVetoedLeptons,
                   int& nVetoedTaus
                   ){
  allLeptons.clear();
  allLeptons.reserve(electronReader.electrons.size() + muonReader.muons.size());
  for(auto& electron : electronReader.electrons)
    allLeptons.push_back(&electron);
  for(auto& muon : muonReader.muons)
    allLeptons.push_back(&muon);
  std::sort(allLeptons.begin(), allLeptons.end(), PhysicsUtilities::greaterPTDeref<LeptonF>());

  selectedLeptons.clear();
  vetoedLeptons.clear();
  for(auto* lepton : allLeptons){
    /*
    bool shouldSelect = false;
    if(config.leptonSelection != SEL_0_LEP && (lepton->ismuon() ? isSelMuon(*(MuonF*)lepton): isSelElectron(*(ElectronF*)lepton))){
      if(config.leptonSelection == SEL_ALL_LEP) shouldSelect = true;
      else if( selectedLeptons.size() == 0){
        if(config.leptonSelection == SEL_1_LEP) shouldSelect = true;
        else if(config.leptonSelection == SEL_1_MU && lepton->ismuon())shouldSelect = true;
        else if(config.leptonSelection == SEL_1_E  && !lepton->ismuon())shouldSelect = true;
      }
    }
    if(shouldSelect){
      selectedLeptons.push_back(lepton);
      continue;
    }
    */

    if (lepton->ismuon() ? isSelMuon(*(MuonF*)lepton): isSelElectron(*(ElectronF*)lepton)) { 
      selectedLeptons.push_back(lepton); 
    }

    if(lepton->ismuon() ? isVetoMuon(*(MuonF*)lepton) : isVetoElectron(*(ElectronF*)lepton) ){
      vetoedLeptons.push_back(lepton);
    }

  }

  nSelLeptons = selectedLeptons.size();
  nVetoedLeptons = vetoedLeptons.size();

  vetoedTaus.clear();
  for(auto& pfc : pfcandReader.pfcands) {
    if(isVetoTau(pfc))
      vetoedTaus.push_back(&pfc);
  }
  nVetoedTaus = vetoedTaus.size();
}
//--------------------------------------------------------------------------------------------------
void BaseTreeAnalyzer::cleanJets(JetReader * reader, std::vector<RecoJetF*>& jets, std::vector<RecoJetF*>* bJets, std::vector<RecoJetF*>* nonBJets) const {
	if((config.cleanJetsvSelectedLeptons_) && !isProcessed_)
		throw std::invalid_argument("BaseTreeAnalyzer::cleanJets(): You want to do jet cleaning but have not yet processed variables!");
	jets.clear(); jets.reserve(reader->recoJets.size());
	if(bJets){bJets->clear();}
	if(nonBJets){nonBJets->clear(); nonBJets->reserve( std::max(2,int(jets.size())) -2);}

	vector<bool> vetoJet(reader->recoJets.size(),false);
	if(config.cleanJetsvSelectedLeptons_) {
	  for(const auto* glep : selectedLeptons) {
	    double nearDR = 0;
	    int near = PhysicsUtilities::findNearestDR(*glep,reader->recoJets,nearDR);
	    if(near >= 0) vetoJet[near] = true;
	  }
	}

	for(unsigned int iJ = 0; iJ < reader->recoJets.size(); ++iJ){
	  auto& jet = reader->recoJets[iJ];
	  if(vetoJet[iJ]) continue;
	  if(!isGoodJet(jet)) continue;

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
//--------------------------------------------------------------------------------------------------
double BaseTreeAnalyzer::correctedPickyPT(double pt,double eta,double area, double rho) const {
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
