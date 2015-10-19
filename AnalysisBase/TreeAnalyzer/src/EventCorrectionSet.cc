
#include "AnalysisBase/TreeAnalyzer/interface/EventCorrectionSet.h"
#include "AnalysisBase/TreeAnalyzer/interface/BaseTreeAnalyzer.h"
#include "AnalysisBase/TreeAnalyzer/interface/DefaultProcessing.h"
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"

namespace ucsbsusy {


void EventCorrectionSet::load(TString fileName, int correctionOptions)
{

  if(correctionOptions & PU) {
    loadFile("PU",fileName,correctionOptions);
    if(options_ & PU) {
      puCorr = new PUCorr(file);
      corrections.push_back(puCorr);
      puCorr50NS = new PUCorr50NS(file);
      corrections.push_back(puCorr50NS);
    }
  }
  if(correctionOptions & TRUEPU) {
    loadFile("puWeight",fileName,correctionOptions);
    if(options_ & TRUEPU) {
      truePUCorr = new TruePUCorr(file);
      corrections.push_back(truePUCorr);
    }
  }
}

void EventCorrectionSet::processCorrection(const BaseTreeAnalyzer * ana) {

  puWeight = 1;
  pu50NSWeight = 1;
  truePUWeight = 1;
  normWeight = 1;
  if(!ana->isMC()) return;

  if(options_ & PU) {
    puCorr->setAxis(PUCorr::NPV,ana->nPV);
    puWeight = puCorr->get();

    bool is25NSMC =   ana->process == defaults::TTZ || 
      ana->process == defaults::TTW || 
      ana->process == defaults::SINGLE_G ||
       ana->process == defaults::SIGNAL;
    if(ana->zIsInvisible && ana->process == defaults::SINGLE_Z) is25NSMC = true;
    puCorr50NS->setAxis(PUCorr50NS::NPV,ana->nPV);
    puCorr50NS->setAxis(PUCorr50NS::INPUT_MC,is25NSMC);
    pu50NSWeight = puCorr50NS->get();
  }

  if(options_ & TRUEPU) {
    truePUCorr->setAxis(ana->nPU);
    truePUWeight = truePUCorr->get();
  }

  if(options_ & NORM) {
    if(ana->process == defaults::TTBAR) normWeight = TTBAR_SF;
    if(ana->process == defaults::SINGLE_Z && ana->nBJets == 0) normWeight = Z_0B_SF;
    if(ana->process == defaults::SINGLE_Z && ana->nBJets == 1) normWeight = Z_1B_SF;
    if(ana->process == defaults::SINGLE_Z && ana->nBJets >= 2) normWeight = Z_2B_SF;
  }

}///EventCorrectionSet

} /* namespace ucsbsusy */
