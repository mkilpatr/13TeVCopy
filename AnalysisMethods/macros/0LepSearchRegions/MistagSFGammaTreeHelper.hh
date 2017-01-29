#ifndef MISTAGSFGAMMATREEHELPER_HH
#define MISTAGSFGAMMATREEHELPER_HH

#include "AnalysisBase/TreeAnalyzer/interface/TreeCopier.h"
#include "AnalysisMethods/macros/0LepSearchRegions/TreeFillers/BasicVariables.hh"
#include "AnalysisMethods/macros/0LepSearchRegions/TreeFillers/ExtraVariables.hh"

using namespace ucsbsusy;

// Adjustments to default configuration
cfgSet::ConfigSet pars0lep(TString json) {
  cfgSet::loadDefaultConfigurations();
  cfgSet::setJSONFile(json);
  cfgSet::ConfigSet cfg = cfgSet::zl_search_set;
  cfg.corrections.eventCorrections |= ucsbsusy::EventCorrectionSet::NORM;
  return cfg;
}

cfgSet::ConfigSet pars0lepCR(TString json) {
  cfgSet::loadDefaultConfigurations();
  cfgSet::setJSONFile(json);
  cfgSet::ConfigSet cfg = cfgSet::zl_lepton_set;
  cfg.corrections.eventCorrections |= ucsbsusy::EventCorrectionSet::NORM;
  return cfg;
}

cfgSet::ConfigSet pars1LCR(TString json) {
  cfgSet::loadDefaultConfigurations();
  cfgSet::setJSONFile(json);
  cfgSet::ConfigSet cfg = cfgSet::zl_search_set;
//  cfg.jets.cleanJetsvLeptons = true;  // Do not clean jets vs leptons
  cfg.electrons              = LeptonSelection::zl_ctr_sLep_electrons;
  cfg.muons                  = LeptonSelection::zl_ctr_sLep_muons;
  cfg.corrections.eventCorrections |= ucsbsusy::EventCorrectionSet::NORM;
  return cfg;
}

cfgSet::ConfigSet pars0lepDiLepCR(TString json) {
  cfgSet::loadDefaultConfigurations();
  cfgSet::setJSONFile(json);
  cfgSet::ConfigSet cfg = cfgSet::zl_dilepton_set;
  cfg.corrections.eventCorrections |= ucsbsusy::EventCorrectionSet::NORM;
  return cfg;
}

cfgSet::ConfigSet pars0LepPhoton(TString json) {
  cfgSet::loadDefaultConfigurations();
  cfgSet::setJSONFile(json);
  cfgSet::ConfigSet cfg = cfgSet::zl_photon_set;
  cfg.corrections.eventCorrections |= ucsbsusy::EventCorrectionSet::NORM;
  return cfg;
}


class TopWSFAnalyzer : public TreeCopierManualBranches {
  
public :
  TopWSFAnalyzer(TString fileName, TString treeName, TString outfileName, size randomSeed, bool isMCTree, cfgSet::ConfigSet *pars) :
    TreeCopierManualBranches(fileName, treeName, outfileName, randomSeed, isMCTree, pars) {}
  
  virtual ~TopWSFAnalyzer() {}
  
  double metcut_   = 0. ;
  int    minnjets_ = 2  ;

  const double DR_CUT = 0.4;

  PhotonF*         pho            = nullptr;
  bool             passDRSel      = true;
  bool             passGenMatch   = true;
  bool             flagQCDFake    = false;
  bool             flagBypassDRSel= false;
  float            drphotonparton = -1;
  

  void book() {
    filler.book(&data);
    extraFiller.bookMergeMistagSF0Lep(&data);
  }
  

  void processVariables(){
    BaseTreeAnalyzer::processVariables();

    // reset to nullptr
    pho = nullptr;
    GenParticleF* genphoton = nullptr;

    // set these flags to be true (for data and znunu)
    passDRSel = true;
    passGenMatch = true;
    drphotonparton = -1;

    if (selectedPhotons.empty()) return;
    pho = selectedPhotons.front();

    // clean leptons vs recoPhoton                                                                                                                                         
    vector<LeptonF*> tmpLeptons;
    for (auto *lep : selectedLeptons){
      if (PhysicsUtilities::deltaR2(*pho, *lep)>0.16) tmpLeptons.push_back(lep);
    }
    selectedLeptons = tmpLeptons;
    nSelLeptons = selectedLeptons.size();

    // clean vetoedTracks vs recoPhoton
    vector<PFCandidateF*> tmpTracks;
    for (auto *trk : vetoedTracks){
      if (PhysicsUtilities::deltaR2(*pho, *trk)>0.16) tmpTracks.push_back(trk);
    }
    vetoedTracks = tmpTracks;
    nVetoedTracks = vetoedTracks.size();

    if (!isMC()) return; // end of data


    passDRSel = false;
    passGenMatch = false;

    vector<GenParticleF*> partons;
    vector<GenParticleF*> genphotons;

    for (auto *p : genParts){
      if (ParticleInfo::isDocOutgoing(p->status()) && ParticleInfo::isQuarkOrGluon(p->pdgId()))
	partons.push_back(p);
      else if (ParticleInfo::isFinal(p->status()) && p->pdgId() == ParticleInfo::p_gamma){
	bool isPrompt = false;
	for (int i=0; i<p->numberOfMothers(); ++i){
	  auto mother = p->mother(i);
	  if (ParticleInfo::isQuark(mother->pdgId()) || mother->pdgId() == ParticleInfo::p_gamma || mother->pdgId() == ParticleInfo::p_proton){
	    isPrompt = true;
	    break;
	  }
	}
	if (isPrompt) genphotons.push_back(p);
      }
    }

    if (genphotons.empty()){
      passDRSel = flagQCDFake;
      return;
    }


    std::sort(genphotons.begin(), genphotons.end(), PhysicsUtilities::greaterPTDeref<GenParticleF>());
    for (auto *gp : genphotons){
      if (PhysicsUtilities::deltaR2(*gp, *pho) < 0.01 && pho->pt()>0.5*gp->pt() && pho->pt()<2*gp->pt()){
	passGenMatch = true;
	genphoton = gp;
	genmet->setP4(genmet->p4() + genphoton->p4());
	break;
      }
    }

    if (passGenMatch){
      double minDR = 0;
      PhysicsUtilities::findNearestDRDeref(*genphoton, partons, minDR);
      drphotonparton = minDR;
      if ( (minDR > DR_CUT && process == defaults::SINGLE_G) || (minDR <= DR_CUT && process == defaults::QCD) )
	passDRSel = !flagQCDFake;
    } else
      passDRSel = flagQCDFake;

    // bypass DR selection for processes other than GJets and QCD
    //      if (process!=defaults::SINGLE_G && process!=defaults::QCD) //FIXME: TTG is saved as SINGLE_G 
    if (flagBypassDRSel)
      passDRSel = true;

  }


  bool fillEvent() {

    // baseline selection
    if (!goodvertex)         return false;
    if (nJets < minnjets_)   return false; 
    // if (nSelLeptons > 0)     return false;
    // if (met->pt() < metcut_) return false;
    if(!pho)                              return false; 
    if(!passDRSel)                        return false; 
    if(!goodvertex)                       return false; 

    MomentumF metpluspho;
    metpluspho.setP4(met->p4() + pho->p4());

    bool ak8probe_ = false;
    for(unsigned int i0=0; i0<fatJets.size(); ++i0) {
      auto fatjet = fatJets.at(i0);
      if (fabs(fatjet->eta())>2.4) { continue; }
      if (fabs(fatjet->pt())<200.) { continue; }
      ak8probe_ = true;
    }
    if (!ak8probe_) return false;

    // calculate ht
    //float ht_ = 0.;
    //for(unsigned int i0=0; i0<jets.size(); ++i0) { ht_ += jets.at(i0)->pt(); }
    //    if (ht_ < 1000.) { return false; }

    // fill inclusive histograms
    filler.fillEventInfo(&data, this, true, &metpluspho);
    extraFiller.fillMergeMistagSF0Lep(&data, this);
    
    return true;
  }

  protected:
    BasicVarsFiller filler;
    ExtraVarsFiller extraFiller;
};

#endif
