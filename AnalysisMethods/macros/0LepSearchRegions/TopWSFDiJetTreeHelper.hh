#ifndef MISTAGSF0LEPTREEHELPER_HH
#define MISTAGSF0LEPTREEHELPER_HH

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
  
  double metcut_   = 50. ;
  int    minnjets_ = 2  ;
  
  
  void book() {
    filler.book(&data);
    extraFiller.bookMergeTopWSF0Lep(&data);
  }
  
  bool fillEvent() {

    // baseline selection
    if (!goodvertex)         return false;
    if (nJets < minnjets_)   return false;
    if (nSelLeptons > 0)     return false;
    //    if (met->pt() < metcut_) return false;


    // get two ak8 jets - back to back
    std::vector<FatJetF*> ak8jets; ak8jets.clear();
    for(unsigned int i0=0; i0<fatJets.size(); ++i0) {
      auto fatjet = fatJets.at(i0);
      if (fabs(fatjet->eta())>2.4) { continue; }
      if (fabs(fatjet->pt())<200.) { continue; }
      ak8jets.push_back(fatjet);
    }
    if (ak8jets.size()<2) { return false; }
    float dphiak8_ = fabs(PhysicsUtilities::deltaPhi(ak8jets[0]->p4(),ak8jets[1]->p4()));
    if (dphiak8_ < 1.)    { return false; }
    

    // calculate ht
    float ht_ = 0.;
    for(unsigned int i0=0; i0<jets.size(); ++i0) { ht_ += jets.at(i0)->pt(); }
    if (ht_ < 1000.) { return false; }

    // fill inclusive histograms
    filler.fillEventInfo(&data, this);
    extraFiller.fillMergeTopWSF0Lep(&data, this);
    
    return true;
  }

  protected:
    BasicVarsFiller filler;
    ExtraVarsFiller extraFiller;
};

#endif
