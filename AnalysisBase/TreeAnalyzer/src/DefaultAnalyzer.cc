//--------------------------------------------------------------------------------------------------
// 
// DefaultAnalyzer
// 
// Inherits from BaseTreeAnalyzer and implements a default selection and cleaning on the objects.
// 
// DefaultAnalyzer.cc created on Mon Nov 3 22:59:28 CET 2014 
// 
//--------------------------------------------------------------------------------------------------

#include "AnalysisBase/TreeAnalyzer/interface/DefaultAnalyzer.h"

using namespace ucsbsusy;

const float DefaultAnalyzer::minElePt	= 20.0;
const float DefaultAnalyzer::minMuPt	= 20.0;
const float DefaultAnalyzer::minTauPt	= 20.0;
const float DefaultAnalyzer::minJetPt	= 30.0;
const float DefaultAnalyzer::minBJetPt	= 30.0;
const float DefaultAnalyzer::maxEleEta	= 2.5;
const float DefaultAnalyzer::maxMuEta	= 2.4;
const float DefaultAnalyzer::maxTauEta	= 2.3;
const float DefaultAnalyzer::maxJetEta	= 2.4;
const float DefaultAnalyzer::maxBJetEta	= 2.4;
const float DefaultAnalyzer::minBJetCSV	= 0.679;
const float DefaultAnalyzer::minDR	= 0.4;

//--------------------------------------------------------------------------------------------------
DefaultAnalyzer::DefaultAnalyzer(TString fileName, TString treeName, bool isMCTree, TString readOption) : 
  BaseTreeAnalyzer(fileName, treeName, isMCTree, readOption), 
  cleanJetsvLeptons(true), 
  cleanJetsvTaus(false)
{

  goodLeptons = new LeptonFCollection();
  goodTaus = new TauFCollection();
  cleanJets = new RecoJetFCollection();
  cleanBJets = new RecoJetFCollection();
  cleanNonBJets = new RecoJetFCollection();
  load(EVTINFO);
  load(AK4JETS, JetReader::LOADRECO | JetReader::FILLOBJ);
  load(ELECTRONS);
  load(MUONS);
  load(TAUS);

}

//--------------------------------------------------------------------------------------------------
DefaultAnalyzer::~DefaultAnalyzer()
{

  goodLeptons->clear();
  cleanJets->clear();
  cleanBJets->clear();
  cleanNonBJets->clear();
  delete goodLeptons;
  delete cleanJets;
  delete cleanBJets;
  delete cleanNonBJets;

}

//--------------------------------------------------------------------------------------------------
void DefaultAnalyzer::process()
{

  goodLeptons->clear();
  goodTaus->clear();
  cleanJets->clear();
  cleanBJets->clear();
  cleanNonBJets->clear();


  for(auto& electron : (*electrons)) {
    if(electron.pt() > minElePt && fabs(electron.scEta()) < maxEleEta && electron.isgoodpogelectron())
      goodLeptons->push_back(electron);
  }

  for(auto& muon : (*muons)) {
    if(muon.pt() > minMuPt && fabs(muon.eta()) < maxMuEta && muon.isgoodpogmuon())
      goodLeptons->push_back(muon);
  }

  for(auto& tau : (*taus)) {
    if(tau.pt() > minTauPt && fabs(tau.eta()) < maxTauEta && tau.isgoodpogtau())
      goodTaus->push_back(tau);
  }

  for(auto& jet : (*ak4Jets)) {
    if(jet.pt() <= minJetPt || fabs(jet.eta()) >= maxJetEta) continue;

    bool cleanjet = true;
    if(cleanJetsvLeptons) {
      for(auto& glep : (*goodLeptons)) {
	if(PhysicsUtilities::deltaR(glep, jet) < minDR) {
	  cleanjet = false;
	  break;
	}
      }
    }
    if(cleanJetsvTaus) {
      for(auto& gtau : (*goodTaus)) {
	if(PhysicsUtilities::deltaR(gtau, jet) < minDR) {
	  cleanjet = false;
	  break;
	}
      }
    }
 
   if(!cleanjet) continue;

    cleanJets->push_back(jet);

    if(jet.csv() > minBJetCSV && fabs(jet.eta()) < maxBJetEta) {
      cleanBJets->push_back(jet);
    }
    else {
      cleanNonBJets->push_back(jet);
    }
  }

}

//--------------------------------------------------------------------------------------------------
bool DefaultAnalyzer::nextEvent(int reportFrequency)
{

  // get collections for this event
  bool keepgoing = BaseTreeAnalyzer::nextEvent(reportFrequency);

  // apply object selection/cleaning
  if(keepgoing) process();

  return keepgoing;

}
