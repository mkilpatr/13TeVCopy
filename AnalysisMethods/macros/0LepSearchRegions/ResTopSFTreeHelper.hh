#ifndef RESTOPSFTREEHELPER_HH
#define RESTOPSFTREEHELPER_HH

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
  
  double metcut_   = 60.0 ;
  int    minnjets_ =   2   ;
  
  
  void book() {
    filler.book(&data);
    extraFiller.bookResTopSF(&data);
  }
  
  bool fillEvent() {
    
    // baseline selection
    if (!goodvertex)         return false;
    if (nJets < minnjets_)   return false;
    if (nSelLeptons < 1)     return false;
    if (met->pt() < metcut_) return false;

    // get get tight muons
    std::vector<MomentumF> tightmuons; tightmuons.clear();
    for (unsigned int i0=0; i0<nSelLeptons; ++i0) {
      auto lep = selectedLeptons.at(i0);
      if (fabs(lep->pdgid()) != 13)        { continue; }
      if (lep->pt()<45.)                   { continue; }
      if (fabs(lep->eta())>2.1)            { continue; }
      if (!(((MuonF*)lep)->istightmuon())) { continue; }
      if (fabs(((MuonF*)lep)->d0())>0.05)  { continue; }
      if (fabs(((MuonF*)lep)->dz())>0.1)   { continue; }
      if (((MuonF*)lep)->miniiso()>0.1)    { continue; }

      MomentumF tmpTightMuons; tmpTightMuons = lep->p4();
      tightmuons.push_back(tmpTightMuons);
    }
    if (!(tightmuons.size()==1)) return false;

  
    // get the tight bjets
    std::vector<MomentumF> csvtjets;
    for(unsigned int i0=0; i0<jets.size(); ++i0) {
      auto jet = jets.at(i0);
      if(jet->csv() > defaults::CSV_TIGHT) {
        MomentumF tmpVecCSVTJets; tmpVecCSVTJets = jet->p4();
        csvtjets.push_back(tmpVecCSVTJets); }
    }
    if (csvtjets.size()<1) return false;

    // find a b close to the lepton
    bool ak8bclose2lep_ = false;
    if (tightmuons.size()>0 && csvtjets.size()>0 ) {
      for (unsigned int i0=0; i0<csvtjets.size(); ++i0) {
        float dphilepbjet_ = fabs(PhysicsUtilities::deltaPhi(tightmuons.at(0),csvtjets[i0]));
        float drlepbjet_   = PhysicsUtilities::deltaR(tightmuons.at(0),csvtjets[i0]);
      if ( (dphilepbjet_<1.) && (drlepbjet_<1.) ) { ak8bclose2lep_ = true; }
      }
    } // end of finding a b-tag close to the tight lep
    if (!ak8bclose2lep_) return false;

    processMoreVariables();

    // get a probe jet away from muon
    bool ak8away2lep_ = false;
    for(unsigned int i0=0; i0<resMVATopCands.size(); ++i0) {
      auto fatjet = resMVATopCands.at(i0);
      if (tightmuons.size()==0) { continue; }
      if (fabs(fatjet.topcand.eta())>2.4) { continue; }
      float dphilepak8_ = fabs(PhysicsUtilities::deltaPhi(tightmuons.at(0),fatjet.topcand));
      if (dphilepak8_ < 2.3) { continue; }
      ak8away2lep_ = true;
    }
    if (!ak8away2lep_) return false;



    // fill inclusive histograms
    filler.fillEventInfo(&data, this);
    extraFiller.fillResTopSF(&data, this);
    
    return true;
  }

  protected:
    BasicVarsFiller filler;
    ExtraVarsFiller extraFiller;
};

#endif
