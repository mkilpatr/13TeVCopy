
#include "AnalysisBase/TreeAnalyzer/interface/EventCorrectionSet.h"
#include "AnalysisBase/TreeAnalyzer/interface/BaseTreeAnalyzer.h"

namespace ucsbsusy {


void EventCorrectionSet::load(TString fileName, bool is25NSMC, int correctionOptions)
{
  inputIs25NS = is25NSMC;
  loadFile("PU",fileName,correctionOptions);
  if(options_ & PU){
    puCorr = new PUCorr(file);
    corrections.push_back(puCorr);
  }
}

void EventCorrectionSet::processVariables(const BaseTreeAnalyzer * ana) {
  npv = ana->nPV;
}

void EventCorrectionSet::setVariables() {
  if(PU) {
    puCorr->setAxis(PUCorr::NPV,npv);
    puCorr->setAxis(PUCorr::INPUT_MC,inputIs25NS);
  }
}

bool EventCorrectionSet::correctProcess(defaults::Process proc) const{
  if(proc == defaults::DATA) return false;
  if(proc > defaults::SIGNAL) return false;
  return true;
}



} /* namespace ucsbsusy */
