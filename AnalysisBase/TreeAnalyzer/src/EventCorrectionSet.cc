
#include "AnalysisBase/TreeAnalyzer/interface/EventCorrectionSet.h"
#include "AnalysisBase/TreeAnalyzer/interface/BaseTreeAnalyzer.h"
#include "AnalysisBase/TreeAnalyzer/interface/DefaultProcessing.h"

namespace ucsbsusy {


void EventCorrectionSet::load(TString fileName, int correctionOptions)
{
  if(correctionOptions & PU){
	  loadFile("PU",fileName,correctionOptions);
	  if(options_ & PU){
		  puCorr = new PUCorr(file);
		  corrections.push_back(puCorr);
	  }
  }
  if(correctionOptions & LEP){
	  loadFile("LEP",fileName,correctionOptions);
	  if(options_ & LEP){
		  lepCorr = new LepCorr(file);
		  corrections.push_back(lepCorr);
	  }
  }
}

void EventCorrectionSet::processCorrection(const BaseTreeAnalyzer * ana) {
        if(!ana->isMC()){
            puWeight =1;
	    lepWeight = 1;
            return;
        }

  if(PU) {	
        if(options_ & PU) {
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
  if(LEP) {
	  lepWeight = 1; //Will be updated later
	  int nTaggedTops = 0;
	  for (UInt_t i=0; i<ana->cttTops.size(); ++i){
		  if(cfgSet::isSelTaggedTop(*ana->cttTops.at(i)))
			  ++nTaggedTops;}

	  unsigned int binCounter = 1;
	  if (ana->met->pt() > 600) binCounter += 5;
	  else if (ana->met->pt() > 200) binCounter +=  (int) ((ana->met->pt()/100) - 2);
	  if(nTaggedTops >= 1) binCounter += 5;
	  if(ana->bJets.size() >= 2) binCounter += 10;


	  std::cout << "The met reads " << ana->met->pt();
	  std::cout << "The nTaggedTops reads " << nTaggedTops;
	  std::cout << "The nbjets reads " << ana->bJets.size();
	  std::cout << "the bin counter reads " << binCounter;
	  std::cout << "The muon yields bin is " << binCounter + LepCorr::muBinShift;
	  std::cout << "The electron yields bin is " << binCounter + LepCorr::eleBinShift;
	  std::cout << "The tau yields bin is " << binCounter + LepCorr::tauBinShift;

  }

}///EventCorrectionSet


} /* namespace ucsbsusy */
