
#include "AnalysisBase/TreeAnalyzer/interface/EventCorrectionSet.h"
#include "AnalysisBase/TreeAnalyzer/interface/BaseTreeAnalyzer.h"
#include "AnalysisBase/TreeAnalyzer/interface/DefaultProcessing.h"
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"

namespace ucsbsusy {


void EventCorrectionSet::load(TString fileName, int correctionOptions)
{

  if(correctionOptions & PU){
  loadFile("PU",fileName,correctionOptions);
  if(options_ & PU){
    puCorr = new PUCorr(file);
    corrections.push_back(puCorr);
    puCorr50NS = new PUCorr50NS(file);
    corrections.push_back(puCorr50NS);
	  }
  }
  if(correctionOptions & LEP){
	  loadFile("LEP",fileName,correctionOptions);
	  if(options_ & LEP){
		  lepCorr = new LepCorr(file);
		  lepCorr->setCorrType(ucsbsusy::LepCorr::VARY_NONE);
		  corrections.push_back(lepCorr);
	  }
 }
}

void EventCorrectionSet::processCorrection(const BaseTreeAnalyzer * ana) {

  puWeight =1;
  pu50NSWeight =1;
  selLepWeight = 1;
  vetoLepWeight = 1;
  normWeight = 1;
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

  if(options_ & NORM) {
    if(ana->process == defaults::TTBAR) normWeight = TTBAR_SF;
    if(ana->process == defaults::SINGLE_Z && ana->nBJets == 0) normWeight = Z_0B_SF;
    if(ana->process == defaults::SINGLE_Z && ana->nBJets == 1) normWeight = Z_1B_SF;
    if(ana->process == defaults::SINGLE_Z && ana->nBJets >= 2) normWeight = Z_2B_SF;
  }

   if(options_ & LEP) {

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
			     	bool lepDecay = false;
			    //COUNT "PROMPT" TAU'S
				for(unsigned int itd = 0; itd < i->numberOfDaughters(); itd++) {
					const GenParticleF* dau = i->daughter(itd);
				        if(ParticleInfo::isA(ParticleInfo::p_eminus, dau) || ParticleInfo::isA(ParticleInfo::p_muminus, dau)) lepDecay = true;}
					if(!lepDecay)
					nPromptGenTaus++;
			}
	  }

		  for(auto* i : ana->vetoedLeptons)
		  {
			  if(fabs(i->pdgid()) == 11) nSelectedElectrons++;
			  if(fabs(i->pdgid()) == 13) nSelectedMuons++;
		  }
	 
	  vetoLepWeight = 1;
	  selLepWeight  = 0;
	  if (nSelectedMuons >= 1 && nGoodGenMu >= 1){
		  lepCorr->setTargetBin(LepCorr::muCorrBin);
	  }
	  else if (nSelectedElectrons >= 1 && nSelectedMuons == 0 && nGoodGenEle >= 1){
		  lepCorr->setTargetBin(LepCorr::eleCorrBin);
	  }
	  else if (ana->nVetoedTracks >= 1 && nSelectedElectrons == 0 && nSelectedMuons == 0 && nPromptGenTaus >= 1){
		  lepCorr->setTargetBin(LepCorr::tauCorrBin);
	  }
          else  if ((nSelectedMuons >= 1) or (nSelectedElectrons >= 1) or (ana->nVetoedTracks >= 1) ){
       		  lepCorr->setTargetBin(LepCorr::fakeBin);
          }
	          vetoLepWeight = 1 - lepCorr->get();
                  selLepWeight  = lepCorr->get();
                 
		  if(lepCorr->corrType == ucsbsusy::LepCorr::VARY_UP) {
                  vetoLepWeight -= lepCorr->getError();
                  selLepWeight+= lepCorr->getError();}

                  if(lepCorr->corrType == ucsbsusy::LepCorr::VARY_DOWN) {
                  vetoLepWeight += lepCorr->getError();
                  selLepWeight -= lepCorr->getError();}

  }///EventCorrectionSet
}

} /* namespace ucsbsusy */
