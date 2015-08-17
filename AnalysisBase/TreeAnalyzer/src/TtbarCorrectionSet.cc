
#include "AnalysisBase/TreeAnalyzer/interface/TtbarCorrectionSet.h"
#include "AnalysisBase/TreeAnalyzer/interface/BaseTreeAnalyzer.h"

namespace ucsbsusy {


void TtbarCorrectionSet::load(TString fileName, int correctionOptions)
{
  loadFile("Ttbar",fileName,correctionOptions);
  if(options_ & TOPPAIRPT){
    topPTCorr = new TOPPAIRPTCorr(file);
    corrections.push_back(topPTCorr);
  }
}

void TtbarCorrectionSet::processVariables(const BaseTreeAnalyzer * ana) {
  const GenParticleF * top1 = 0;
  const GenParticleF * top2 = 0;
  for(const auto * g : ana->genParts){
    if(TMath::Abs(g->pdgId()) != 6) continue;
    if(top1) top2 = g;
    else top1 = g;
  }
  if(!top1 || !top2)       throw "TtbarCorrectionSet::processVariables: two gen tops were not found!";
  topPT = top1->pt() + top2->pt();
}

void TtbarCorrectionSet::setVariables() {
  if(topPTCorr) topPTCorr->setAxis(TOPPAIRPTCorr::TopPairPT,topPT);
}

bool TtbarCorrectionSet::correctProcess(defaults::Process proc) const{
  return proc == defaults::TTBAR ? true : false;
}



} /* namespace ucsbsusy */
