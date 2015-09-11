
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
            selLepWeight = 1;
            vetoLepWeight = 1;
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
	  if (ana->met->pt() > 200 && ana->bJets.size() >= 1) std::cout << "Analyzing an event in the inclusive region " << std::endl;

	  int nGoodGenMu = 0; int nGoodGenEle = 0; int nPromptGenTaus = 0;
	  int nSelectedElectrons = 0;	int nSelectedMuons = 0;
	  ///COUNT GEN LEPS
	  for (auto i : ana->genParts)
	  {
			const GenParticleF * genPartMom = 0;
			if (i->numberOfMothers()>0) { genPartMom = i->mother(0); }
			else{continue;}
			//(ELECTRON) make sure mom is a W or a Z or a Tau
			if ( (abs(i->pdgId()) == 11) && (abs(genPartMom->pdgId()) == 23 or abs(genPartMom->pdgId()) == 24 or abs(genPartMom->pdgId()) == 15) ) {
			    //COUNT "GOOD" ELE's
				nGoodGenEle++;
			}
			//(MUON) make sure mom is a W or a Z or a Tau
			if ( (abs(i->pdgId()) == 13) && (abs(genPartMom->pdgId()) == 23 or abs(genPartMom->pdgId()) == 24 or abs(genPartMom->pdgId()) == 15) ) {
			    //COUNT "GOOD" MU'S
				nGoodGenMu++;
			}
			//(TAU) make sure mom is a W or Z
			if ( (abs(i->pdgId()) == 15) && (abs(genPartMom->pdgId()) == 23 or abs(genPartMom->pdgId()) == 24) ) {
			    //COUNT "PROMPT" TAU'S
				nPromptGenTaus++;
			}
	  }

	  if(ana->getAnaCfg().selectedLeptons.getName() == "zl_sel_leptons" or ana->getAnaCfg().selectedLeptons.getName() == "ol_sel_leptons"){
		  for(auto* i : ana->selectedLeptons)
		  {
			  if(fabs(i->pdgid()) == 11) nSelectedElectrons++;
			  if(fabs(i->pdgid()) == 13) nSelectedMuons++;
		  }
	  }
	  else{
		  for(auto* i : ana->vetoedLeptons)
		  {
			  if(fabs(i->pdgid()) == 11) nSelectedElectrons++;
			  if(fabs(i->pdgid()) == 13) nSelectedMuons++;
		  }
	 }

	  if (nSelectedMuons >= 1 && nGoodGenMu >= 1){
		  lepCorr->setTargetBin(LepCorr::muCorrBin);
		  vetoLepWeight = 1 - lepCorr->get();
		  selLepWeight  = lepCorr->get();
//		  std::cout << "The mu corr is " << selLepWeight << std::endl;
	  }
	  else if (nSelectedElectrons >= 1 && nSelectedMuons == 0 && nGoodGenEle >= 1){
		  lepCorr->setTargetBin(LepCorr::eleCorrBin);
		  vetoLepWeight = 1 - lepCorr->get();
		  selLepWeight  = lepCorr->get();
//		  std::cout << "The ele corr is " << selLepWeight << std::endl;
	  }
	  else if (ana->nVetoedTracks >= 1 && nSelectedElectrons == 0 && nSelectedMuons == 0 && nPromptGenTaus >= 0){
		  lepCorr->setTargetBin(LepCorr::tauCorrBin);
		  vetoLepWeight = 1 - lepCorr->get();
		  selLepWeight  = lepCorr->get();
//		  std::cout << "The tau corr is " << selLepWeight << std::endl;
	  }
	  else	if ((nSelectedMuons >= 1 && nGoodGenMu == 0)
			  or (nSelectedElectrons >= 1 && nSelectedMuons == 0 && nGoodGenEle == 0 )
			  or (ana->nVetoedTracks >= 1 && nSelectedElectrons == 0 && nSelectedMuons == 0 && nPromptGenTaus == 0)){
		  vetoLepWeight = 0;
		  selLepWeight  = 1;}
  }///EventCorrectionSet
}

} /* namespace ucsbsusy */
