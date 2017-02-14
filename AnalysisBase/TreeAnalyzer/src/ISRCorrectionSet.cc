

#include "AnalysisBase/TreeAnalyzer/interface/ISRCorrectionSet.h"
#include "AnalysisTools/QuickRefold/interface/Refold.h"
#include "AnalysisBase/TreeAnalyzer/interface/BaseTreeAnalyzer.h"
#include "AnalysisTools/Utilities/interface/PartonMatching.h"
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"

#include "TFile.h"


namespace ucsbsusy {


ISRCorr::ISRCorr(TString corrInput,TString sigNormInput, const std::vector<TString>& sigNormNames, const std::vector<TString>& bkgNormNames ) : Correction("ISR"), corrFile(0), sigNormFile(0)
{
  //Fill SF reading
  std::clog << "Loading correction file: "<< corrInput<<std::endl;
  corrFile = TFile::Open(corrInput,"read");
  if(!corrFile) throw std::invalid_argument("ISRCorr::ISRCorr: Correction file not found!");
  corr = (const TH1*)(corrFile->Get("Corr") );
  if(!corr) throw std::invalid_argument("ISRCorr::ISRCorr: Correction could not be found!");

  if(sigNormNames.size() + bkgNormNames.size()){
	    std::clog << "Loading normalization file: "<< sigNormInput<<std::endl;
	    sigNormFile = TFile::Open(sigNormInput,"read");
	    if(!sigNormInput) throw std::invalid_argument("ISRCorr::ISRCorr: Normalization file not found!");
  }

  if(sigNormNames.size()){
    std::clog << "And normalizations for: ";
    for(unsigned int iN = 0; iN < sigNormNames.size(); ++iN){
      std::clog << sigNormNames[iN] <<" ";

      defaults::SignalType type = defaults::NUM_SIGTYPES;
      for(int iS = 0; iS < defaults::NUM_SIGTYPES; ++iS  ){
        if(sigNormNames[iN] != defaults::SIGNAL_NAMES[iS]) continue;
        type = defaults::SignalType(iS);
        break;
      }
      if(type == defaults::NUM_SIGTYPES) throw std::invalid_argument(TString::Format("ISRCorr::ISRCorr: Normalization (%s) is not in the list of defaults::SignalType!",sigNormNames[iN].Data()));

      const auto * norm = (const QuickRefold::Refold*)(sigNormFile->Get(sigNormNames[iN]) );
      if(!norm)  throw std::invalid_argument(TString::Format("ISRCorr::ISRCorr: Normalization (%s) could not be found!",sigNormNames[iN].Data()));
      sigNorms[type] = norm;
    }
    std::clog << std::endl;
  }

  if(bkgNormNames.size()){
    std::clog << "And normalizations for: ";
    for(unsigned int iN = 0; iN < bkgNormNames.size(); ++iN){
      std::clog << bkgNormNames[iN] <<" ";

      defaults::Process type = defaults::NUMPROCESSES;
      for(int iS = 0; iS < defaults::NUMPROCESSES; ++iS  ){
        if(bkgNormNames[iN] != defaults::PROCESS_NAMES[iS]) continue;
        type = defaults::Process(iS);
        break;
      }
      if(type == defaults::NUMPROCESSES) throw std::invalid_argument(TString::Format("ISRCorr::ISRCorr: Normalization (%s) is not in the list of defaults::Process!",bkgNormNames[iN].Data()));

      const auto * norm = (const QuickRefold::Refold*)(sigNormFile->Get(bkgNormNames[iN]) );
      if(!norm)  throw std::invalid_argument(TString::Format("ISRCorr::ISRCorr: Normalization (%s) could not be found!",bkgNormNames[iN].Data()));
      bkgNorms[type] = norm;
    }
    std::clog << std::endl;
  }

  if(sigNormNames.size() + bkgNormNames.size() == 0){
    std::clog << "Warning: Not loading any ISR normalizations!"<< sigNormInput<<std::endl;
  }
}

ISRCorr::~ISRCorr(){
  if(corrFile){
    corrFile->Close();
    delete corrFile;
  }

  if(sigNormFile){
    sigNormFile->Close();
    delete sigNormFile;
  }
}

double ISRCorr::getCorrFactor(CORRTYPE type, int nISRJets) const {
  if(type == NONE) return 1;
  const int bin =  std::min(std::max(corr->FindFixBin(nISRJets),1),corr->GetNbinsX());
  const double cf =  corr->GetBinContent(bin);
  if(type == NOMINAL){
    return cf;
  } else if(type == UP){
    return cf + TMath::Abs(1 - cf)*.5;
  } else {
    return cf - TMath::Abs(1 - cf)*.5 ;
  }

}
double ISRCorr::getSignalNormFactor(CORRTYPE type, const defaults::SignalType sigType, const std::vector<int>& massParams ) const {
  if(type == NONE) return 1;
  auto normIt = sigNorms.find(sigType);
  if(normIt == sigNorms.end()){
    return -1;
  }
  const auto* norm = normIt->second;
  norm->setBin(MASS1,massParams[0]);
  norm->setBin(MASS2,massParams[1]);
  norm->setBin(MASS3,massParams[2]);
  norm->setBin(TYPE,type);
  return norm->getValue();
}

double ISRCorr::getBKGNormFactor(CORRTYPE type, const defaults::Process process) const {
  if(type == NONE) return 1;
  auto normIt = bkgNorms.find(process);
  if(normIt == bkgNorms.end()){
    return -1;
  }
  const auto* norm = normIt->second;
  norm->setBin(MASS1,type); //ICKY....only one axis for bkg!!
  return norm->getValue();
}

void ISRCorrectionSet::load(TString corrInput,TString normInput, TString normTightInput, const std::vector<TString>& normNames,const std::vector<TString>& bkgNormNames, int correctionOptions)
{
  loadSimple("ISRCorrection",correctionOptions);

  if(options_ & ISRCORR){
    isrCorr = new ISRCorr(corrInput,normInput, normNames,bkgNormNames);
    corrections.push_back(isrCorr);
  }
  if(options_ & ISRCORRTIGHT){
    isrCorrTight = new ISRCorr(corrInput,normTightInput, normNames,bkgNormNames);
    corrections.push_back(isrCorrTight);
  }
}

int ISRCorrectionSet::getNISRJets(const BaseTreeAnalyzer * ana, bool tight) const {
  //hard coded to match derivation
  const double minJetPT = 30;
  const double maxJetETA = 2.4;

  //For the tight version, let's count W/Z->leptons
  std::vector<MomentumF> leptons;
  if(tight)
  for(const auto& p : ana->genParticleReader.genParticles){
	if(!ParticleInfo::isLepton(p.pdgId())) continue;
	if(p.numberOfMothers() != 1) continue;
	if(!(ParticleInfo::isEWKBoson(p.mother(0)->pdgId()) || ParticleInfo::isBSM(p.mother(0)->pdgId()) )) continue;
	leptons.emplace_back(p.p4());
  }

  int nISRJets = 0;
  for(const auto& j : ana->ak4Reader.recoJets){
    if(j.pt() < minJetPT) continue;
    if(j.absEta() > maxJetETA) continue;
    if(ana->process != defaults::SIGNAL && !j.looseid()) continue;
    if(j.decayMatch()) continue;
    bool lepMatch = false;
    for(const auto& l : leptons){
    	if(PhysicsUtilities::deltaR2(l,j) < 0.3*0.3 ) {lepMatch = true; break;}
    }
    if(lepMatch) continue;
    nISRJets++;
  }

  return nISRJets;

}

void ISRCorrectionSet::processCorrection(const BaseTreeAnalyzer * ana) {

  isrWeight =1;
  nISRJets = 0;
  nISRJetsTight = 0;

  if(!ana->isMC()) return;
  if(!(ana->process == defaults::SIGNAL || ana->process == defaults::TTBAR)) return;
  const cfgSet::ConfigSet& cfg = ana->getAnaCfg();
  massParams[0] = ana->evtInfoReader.massparams->size() > 0 ? ana->evtInfoReader.massparams->at(0) : 0;
  massParams[1] = ana->evtInfoReader.massparams->size() > 1 ? ana->evtInfoReader.massparams->at(1) : 0;
  massParams[2] = ana->evtInfoReader.massparams->size() > 2 ? ana->evtInfoReader.massparams->at(2) : 0;

  if( (options_ & ISRCORR)) {
    nISRJets = getNISRJets(ana);
    isrWeight = ana->process == defaults::SIGNAL ? isrCorr->getSignalNormCorrFactor(cfg.corrections.isrType,ana->evtInfoReader.signalType,massParams,nISRJets)
    		:isrCorr->getBKGNormCorrFactor(cfg.corrections.isrType, ana->process,nISRJets);
    if(isrWeight < 0) isrWeight = 1;
  }
  if( (options_ & ISRCORRTIGHT)){
    nISRJetsTight = getNISRJets(ana,true);
    isrWeightTight = ana->process == defaults::SIGNAL ? isrCorrTight->getSignalNormCorrFactor(cfg.corrections.isrType,ana->evtInfoReader.signalType,massParams,nISRJetsTight)
    		: isrCorrTight->getBKGNormCorrFactor(cfg.corrections.isrType, ana->process,nISRJetsTight);
    if(nISRJetsTight < 0) nISRJetsTight = 1;
  }
}
} /* namespace ucsbsusy */
