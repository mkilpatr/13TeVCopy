
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

void TtbarCorrectionSet::processCorrection(const BaseTreeAnalyzer * ana) {

  if(ana->process != defaults::TTBAR){
    topPTWeight = 1;
    return;
  }

  const GenParticleF * top1 = 0;
  const GenParticleF * top2 = 0;
  for(const auto * g : ana->genParts){
    if(TMath::Abs(g->pdgId()) != 6) continue;
    if(top1) top2 = g;
    else top1 = g;
  }
  if(!top1 || !top2)       throw std::invalid_argument("TtbarCorrectionSet::processVariables: two gen tops were not found!");
  //float topPT = top1->pt() + top2->pt();

  if(topPTCorr) {
    // commented by LG for temp
    //topPTCorr->setAxis(TOPPAIRPTCorr::TopPairPT,topPT);
    //topPTWeight = topPTCorr->get();
    // ===
    float top1PTWeight = 1.;
    if (top1->pt()>400.) { top1PTWeight = exp(0.199-0.00166*400.); } else { top1PTWeight = exp(0.199-0.00166*top1->pt()); }
    if (top2->pt()>400.) { top2PTWeight = exp(0.199-0.00166*400.); } else { top2PTWeight = exp(0.199-0.00166*top2->pt()); }
    topPTWeight = sqrt(top1PTWeight*top2PTWeight);

  }
}




} /* namespace ucsbsusy */
