
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
  //  if(!top1 || !top2)       throw std::invalid_argument("TtbarCorrectionSet::processVariables: two gen tops were not found!");
  if(!top1 || !top2) { topPTWeight = 1; return; }
  //float topPT = top1->pt() + top2->pt();


  if (ana->isTTbarPowheg){
    // TTbar Powheg
    auto wgt = [](float pt){ float x = (pt>600 ? 600 : pt); return exp(0.0615-0.0005*x); };
    float top1PTWeight = wgt(top1->pt());
    float top2PTWeight = wgt(top2->pt());;
//    topPTWeight = sqrt(top1PTWeight*top2PTWeight);
    //sumwgt, ttbarfinal = 451670 451989.
    float wgtnorm = 451670./451989.;
    topPTWeight = wgtnorm*(sqrt(top1PTWeight*top2PTWeight));
  }else{
    // TTbar MadGraph
    float top1PTWeight = 1.;
    float top2PTWeight = 1.;
    if (top1->pt()>400.) { top1PTWeight = exp(0.199-0.00166*400.); } else { top1PTWeight = exp(0.199-0.00166*top1->pt()); }
    if (top2->pt()>400.) { top2PTWeight = exp(0.199-0.00166*400.); } else { top2PTWeight = exp(0.199-0.00166*top2->pt()); }
//    topPTWeight = sqrt(top1PTWeight*top2PTWeight);
    //sumwgt, ttbarfinal = 451670 452348
    float wgtnorm = 451670./452348.;
    topPTWeight = wgtnorm*(sqrt(top1PTWeight*top2PTWeight));
  }


//  if(topPTCorr) {
    // commented by LG for temp
    //topPTCorr->setAxis(TOPPAIRPTCorr::TopPairPT,topPT);
    //topPTWeight = topPTCorr->get();
//  }
}




} /* namespace ucsbsusy */
