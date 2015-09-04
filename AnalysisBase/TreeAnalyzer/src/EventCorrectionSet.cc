
#include "AnalysisBase/TreeAnalyzer/interface/EventCorrectionSet.h"
#include "AnalysisBase/TreeAnalyzer/interface/BaseTreeAnalyzer.h"

namespace ucsbsusy {


void EventCorrectionSet::load(TString fileName, int correctionOptions)
{
  loadFile("PU",fileName,correctionOptions);
  if(options_ & PU){
    puCorr = new PUCorr(file);
    corrections.push_back(puCorr);
  }
}

void EventCorrectionSet::processCorrection(const BaseTreeAnalyzer * ana) {
  if(!ana->isMC()){
    puWeight =1;
    return;
  }

  if(PU) {
    bool is25NSMC = 	ana->process == defaults::TTZ || 
			ana->process == defaults::TTW || 
			ana->process == defaults::SINGLE_G ||
			ana->process == defaults::SIGNAL;
    if(ana->zIsInvisible && ana->process == defaults::SINGLE_Z) is25NSMC = true;
    puCorr->setAxis(PUCorr::NPV,ana->nPV);
    puCorr->setAxis(PUCorr::INPUT_MC,is25NSMC);
    puWeight = puCorr->get();
  }
}


} /* namespace ucsbsusy */
