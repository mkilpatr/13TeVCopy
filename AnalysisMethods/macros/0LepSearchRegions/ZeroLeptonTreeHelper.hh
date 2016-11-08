#ifndef ZEROLEPTONTREEHELPER_HH
#define ZEROLEPTONTREEHELPER_HH

#include "AnalysisBase/TreeAnalyzer/interface/TreeCopier.h"
#include "AnalysisMethods/macros/0LepSearchRegions/TreeFillers/BasicVariables.hh"
#include "AnalysisMethods/macros/0LepSearchRegions/TreeFillers/ExtraVariables.hh"
//#include "AnalysisMethods/macros/0LepSearchRegions/TreeFillers/HettTopTagger.hh"
//#include "AnalysisMethods/macros/0LepSearchRegions/TreeFillers/CttTopTagger.hh"
//#include "AnalysisMethods/macros/0LepSearchRegions/TreeFillers/ResolvedTagger.hh"

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

    double metcut_   = 200.0 ;
    int    minnjets_ =   2   ;

    unsigned int minnisrj_ =   0   ;
    double       isrptcut_ =   0.0 ;

    bool   islepcr    = false;
    bool   addlep2met = false;
    bool   applyCHFFilter = false ;

    void book() {
      filler.book(&data);
      extraFiller.bookHist(outFile_);
      extraFiller.bookTest(&data);
//      extraFiller.bookSyst(&data);
//      extraFiller.bookJetMET(&data);
//      extraFiller.bookQCDAngles(&data);
//      extraFiller.bookLepton(&data);
//      extraFiller.bookGen(&data);
//      m2Filler.book(&data);
//      hettFiller.book(&data);
//      cttFiller.book(&data);
      //resFiller.book(&data);
      //resFiller.bookExtra(&data);
      //resFiller.bookFiltered(&data);
    }

    bool fillEvent() {

      // fill inclusive histograms
      extraFiller.fillHistograms(this);

      if(!goodvertex) return false;

      if(nJets < minnjets_) return false;

      if(isrJets.size() < minnisrj_) return false;
      if(minnisrj_ > 0 && isrJets[0]->pt() < isrptcut_) return false;

      if(applyCHFFilter && !cfgSet::passCHFFilter(jets)) return false;

      if (islepcr) {
        if (nSelLeptons < 1    )        return false;

        MomentumF lepplusmet;
        lepplusmet.setP4(met->p4() + selectedLepton->p4());

        if (lepplusmet.pt() < metcut_)  return false;

        filler.fillEventInfo(&data, this, addlep2met, &lepplusmet);
//        extraFiller.fillJetMETInfo(&data, this, true, &lepplusmet);
//	extraFiller.fillQCDAngles(&data, this);
      } else {
        if(met->pt() < metcut_  ) return false;

        filler.fillEventInfo(&data, this);
//        extraFiller.fillJetMETInfo(&data, this);
//	extraFiller.fillQCDAngles(&data, this);
      }

      extraFiller.fillTestVars(&data, this);
//      extraFiller.fillSystInfo(&data, this);
//      extraFiller.fillLeptonInfo(&data, this);
//      extraFiller.fillGenInfo(&data, this);
      //hettFiller.fillHettInfo(&data,this,jets,hettTops);
      //cttFiller.fillTopTagInfo(&data,this,jets);
      //resFiller.fillResolvedBasicInfo(&data,this,jets,resTops,resCands);
      //resFiller.fillResolvedExtraInfo(&data,this,jets,resTops,resCands);
      //resFiller.fillResolvedFilteredInfo(&data,this,resTops,resCands);

      return true;
    }

  protected:
    BasicVarsFiller filler;
    ExtraVarsFiller extraFiller;
//    HettTreeFiller hettFiller;
//    CttTreeFiller cttFiller;
//    ResTreeFiller resFiller;
};

#endif
