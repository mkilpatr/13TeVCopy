#ifndef ZEROLEPTONTREEHELPER_HH
#define ZEROLEPTONTREEHELPER_HH

#include "AnalysisBase/TreeAnalyzer/interface/TreeCopier.h"
#include "AnalysisTools/ObjectSelection/interface/ResolvedTopMVA.h"
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


class ZeroLeptonAnalyzer : public TreeCopierManualBranches {

  public :
    ZeroLeptonAnalyzer(TString fileName, TString treeName, TString outfileName, size randomSeed, bool isMCTree, cfgSet::ConfigSet *pars) :
      TreeCopierManualBranches(fileName, treeName, outfileName, randomSeed, isMCTree, pars) {}

    virtual ~ZeroLeptonAnalyzer() {}

    const double metcut_   = 200.0 ;
    const int    minnjets_ =   2   ;

    bool applyTightPresel = false;

    bool   islepcr    = false;
    bool   addlep2met = false;
    bool   applyCHFFilter = false ;
    bool   useGenMET = false;

    void book() {
      filler.book(&data);
      extraFiller.bookQCDAngles(&data);
      extraFiller.bookWTag(&data);
//      extraFiller.bookSyst(&data);
      extraFiller.bookJetMET(&data);
      extraFiller.bookLepton(&data);
      extraFiller.bookGen(&data);
      extraFiller.bookTopFrameTagger(&data);
      extraFiller.bookHist(outFile_);
    }

    bool fillEvent() {

      // check if using genMET
      if (useGenMET) met = genmet;

      // fill inclusive histograms
      extraFiller.fillHistograms(this);

      if(!goodvertex) return false;

	cout << "goodvertex" << endl;
      if(nJets < minnjets_) return false;

      if (applyTightPresel){
        bool passLM = met->pt()/(std::sqrt(JetKinematics::ht(jets)))>10 && ak8isrJets.size() && ak8isrJets.front()->pt()>200;
        bool passHM = nJets>=5 && nBJets>=1;
        if (!passLM && !passHM) return false;
      }

      if(applyCHFFilter && !cfgSet::passCHFFilter(jets)) return false;

	cout << "PassFilter" << endl;
      if (islepcr) {
        if (nSelLeptons < 1    )        return false;

        MomentumF lepplusmet;
        lepplusmet.setP4(met->p4() + selectedLepton->p4());

        if (lepplusmet.pt() < metcut_)  return false;

        processMoreVariables(); // call this before filling, but after all preselections
        filler.fillEventInfo(&data, this, addlep2met, &lepplusmet);
      } else {
        if(met->pt() < metcut_  ) return false;

        processMoreVariables(); // call this before filling, but after all preselections
        filler.fillEventInfo(&data, this);
      }

	cout << "Fill TestVars" << endl;
//      extraFiller.fillTestVars(&data, this);
//      extraFiller.fillQCDAngles(&data, this);
////      extraFiller.fillSystInfo(&data, this);
//      extraFiller.fillWTagInfo(&data, this);
//      extraFiller.fillLeptonInfo(&data, this);
//      extraFiller.fillGenInfo(&data, this);
//      extraFiller.fillTopFrameTaggerInfo(&data, this);
      return true;
    }

  protected:
    BasicVarsFiller filler;
    ExtraVarsFiller extraFiller;
};

#endif
