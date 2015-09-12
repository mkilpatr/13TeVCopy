
#include "AnalysisBase/TreeAnalyzer/interface/EventCorrectionSet.h"
#include "AnalysisBase/TreeAnalyzer/interface/BaseTreeAnalyzer.h"

namespace ucsbsusy {


void EventCorrectionSet::load(TString fileName, int correctionOptions)
{
  loadFile("PU",fileName,correctionOptions);
  if(options_ & PU){
    puCorr = new PUCorr(file);
    corrections.push_back(puCorr);
    puCorr50NS = new PUCorr50NS(file);
    corrections.push_back(puCorr50NS);
  }
}

void EventCorrectionSet::processCorrection(const BaseTreeAnalyzer * ana) {
  puWeight =1;
  pu50NSWeight =1;
  if(!ana->isMC()) return;

  if(options_ & PU) {
    puCorr->setAxis(PUCorr::NPV,ana->nPV);
    puWeight = puCorr->get();

    bool is25NSMC = 	ana->process == defaults::TTZ || 
			ana->process == defaults::TTW || 
			ana->process == defaults::SINGLE_G ||
			ana->process == defaults::SIGNAL;
    if(ana->zIsInvisible && ana->process == defaults::SINGLE_Z) is25NSMC = true;
    puCorr50NS->setAxis(PUCorr50NS::NPV,ana->nPV);
    puCorr50NS->setAxis(PUCorr50NS::INPUT_MC,is25NSMC);
    pu50NSWeight = puCorr50NS->get();
  }
}


} /* namespace ucsbsusy */
