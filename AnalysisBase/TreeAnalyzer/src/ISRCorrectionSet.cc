

#include "AnalysisBase/TreeAnalyzer/interface/ISRCorrectionSet.h"
#include "AnalysisTools/QuickRefold/interface/Refold.h"
#include "AnalysisBase/TreeAnalyzer/interface/BaseTreeAnalyzer.h"
#include "AnalysisTools/Utilities/interface/PartonMatching.h"

#include "TFile.h"


namespace ucsbsusy {


ISRCorr::ISRCorr(TString corrInput,TString sigNormInput, const std::vector<TString>& sigNormNames ) : Correction("ISR"), corrFile(0), sigNormFile(0)
{
  //Fill SF reading
  std::clog << "Loading correction file: "<< corrInput<<std::endl;
  corrFile = TFile::Open(corrInput,"read");
  if(!corrFile) throw std::invalid_argument("ISRCorr::ISRCorr: Correction file not found!");
  corr = (const TH1*)(corrFile->Get("Corr") );
  if(!corr) throw std::invalid_argument("ISRCorr::ISRCorr: Correction could not be found!");

  if(sigNormNames.size()){
    std::clog << "Loading normalization file: "<< sigNormInput<<std::endl;
    sigNormFile = TFile::Open(sigNormInput,"read");
    if(!sigNormInput) throw std::invalid_argument("ISRCorr::ISRCorr: Normalization file not found!");
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
  } else{
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

void ISRCorrectionSet::load(TString corrInput,TString normInput, const std::vector<TString>& normNames, int correctionOptions)
{
  loadSimple("ISRCorrection",correctionOptions);

  if(options_ & ISRCORR){
    isrCorr = new ISRCorr(corrInput,normInput, normNames);
    corrections.push_back(isrCorr);
  }
}

int ISRCorrectionSet::getNISRJets(const BaseTreeAnalyzer * ana) const {
  //hard coded to match derivation
  const double minJetPT = 30;
  const double maxJetETA = 2.4;

  int nISRJets = 0;
  //Please forgive me for what I am about to do
  auto& recoJetReader = *const_cast<JetReader*>(&ana->ak4Reader);
  auto& genPartReader = *const_cast<GenParticleReader*>(&ana->genParticleReader);

  std::vector<GenJetF*> genJets;
  std::vector<ucsbsusy::RecoJetF*> recoJets;
  std::vector<PartonMatching::DecayID> decays;

  recoJets.clear();
  decays.clear();
  std::vector<int> genIDX;
  for(auto& j : recoJetReader.recoJets){
    if(j.pt() < minJetPT) continue;
    if(j.absEta() > maxJetETA) continue;
    if(ana->process != defaults::SIGNAL && !j.looseid()) continue;
    recoJets.push_back(&j);
    genIDX.push_back(-1);
    if(j.genJet() == 0)  continue;
    genIDX.back() = genJets.size();
    genJets.push_back(j.genJet());
  }
//
  PartonMatching::PartonEvent * partonEvent = new PartonMatching::PartonEvent(genPartReader,recoJetReader,genJets);
  partonEvent->processSubtractedJetPTs();

  for(unsigned int iJ = 0; iJ < recoJets.size(); ++iJ){
    if(genIDX[iJ] < 0 ){
      nISRJets++;
      continue;
    }
    double newGenJETPT = partonEvent->subtractedJetPTs[genIDX[iJ]];
    double originalGENJetPT = genJets[genIDX[iJ]]->pt();
    double recoCF = originalGENJetPT == 0 ? 1 : newGenJETPT/originalGENJetPT;
    if(recoJets[iJ]->pt()*recoCF >= minJetPT) nISRJets++;
  }
  delete partonEvent;
  return nISRJets;

}

void ISRCorrectionSet::processCorrection(const BaseTreeAnalyzer * ana) {

  isrWeight =1;
  nISRJets = 0;
  if(!ana->isMC()) return;
  if( (options_ & ISRCORR) && ana->process == defaults::SIGNAL){
    const cfgSet::ConfigSet& cfg = ana->getAnaCfg();
    nISRJets = getNISRJets(ana);
    massParams[0] = ana->evtInfoReader.massparams->size() > 0 ? ana->evtInfoReader.massparams->at(0) : 0;
    massParams[1] = ana->evtInfoReader.massparams->size() > 1 ? ana->evtInfoReader.massparams->at(1) : 0;
    massParams[2] = ana->evtInfoReader.massparams->size() > 2 ? ana->evtInfoReader.massparams->at(2) : 0;
    isrWeight = isrCorr->getSignalNormCorrFactor(cfg.corrections.isrType,ana->evtInfoReader.signalType,massParams,nISRJets);
    if(isrWeight < 0) isrWeight = 1;
  }
}
} /* namespace ucsbsusy */
