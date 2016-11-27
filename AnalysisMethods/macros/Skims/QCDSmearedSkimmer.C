#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisBase/TreeAnalyzer/interface/TreeCopier.h"
#include "AnalysisBase/TreeAnalyzer/interface/DefaultProcessing.h"
#include "AnalysisBase/TreeAnalyzer/interface/QCDRespSmearingAnalyzer.h"

using namespace std;
using namespace ucsbsusy;

TString smearTemplateName = "";

class Copier : public TreeCopierAllBranches {
public:
  Copier(string fileName, string treeName, string outFileName, size randSeed, bool isMCTree, cfgSet::ConfigSet * pars) : TreeCopierAllBranches(fileName,treeName,outFileName,randSeed,isMCTree,pars) {
  };
  virtual ~Copier() {};

  virtual void loadVariables() override {
    load(cfgSet::EVTINFO);
    load(cfgSet::AK4JETS,JetReader::defaultOptions | JetReader::LOADGEN);
  }

  virtual bool fillEvent() override {
    if(met->pt() < 200) return false;
//    if(nJets < 2 || jets[0]->pt() < 75) return false;
    return true;
  }

  void book() override {
  }

  virtual BaseEventAnalyzer * setupEventAnalyzer() override {
    auto * smearer = new  QCDRespSmearingCopierEventAnalyzer();
cout << TString(cfgSet::CMSSW_BASE) + "/" + smearTemplateName << endl;
    smearer->smearOptions.respFileName   = TString(cfgSet::CMSSW_BASE) + "/src/AnalysisMethods/macros/JetMETStudies/" + smearTemplateName;
    smearer->smearOptions.respInputName  = "JetResByFlav";
//    smearer->smearOptions.winType        = JetRespSmear::FLAT;
//    smearer->smearOptions.doFlatSampling = false;
//    smearer->smearOptions.maxWindow      = 5.;
    return smearer;
  }
};

#endif

void QCDSmearedSkimmer(string fileName = "root://cmsxrootd.fnal.gov//store/user/lpcstop/noreplica/13TeV/111116/qcd_orig/qcd_ht200to300_1_ntuple_postproc.root", int fileIndex = -1, string treeName = "Events", string outPostfix ="qcdSmear", string templateName = "resTailOut_combined_filtered_CHEF_puWeight_weight_WoH.root", bool isMC = true, int startEvent =-1, int maxEvents = -1) {
  cfgSet::loadDefaultConfigurations();
  cfgSet::ConfigSet cfg = cfgSet::zl_search_set;
  cfg.corrections.jetResCorrType = ucsbsusy::NONE;
  smearTemplateName = templateName;

  //get the output name
  TString prefix(fileName);
  prefix.Remove(0,prefix.Last('/') + 1);
  if(prefix.First('.') >= 0) prefix.Resize(prefix.First('.'));
  TString outName = fileIndex < 0 ? TString::Format("%s_%s.root",prefix.Data(),outPostfix.c_str()) : TString::Format("%s_%i_%s.root",prefix.Data(),fileIndex,outPostfix.c_str());

  Copier a(fileName,treeName,outName.Data(),fileIndex+2,isMC, &cfg);

  a.analyze(10000,maxEvents,startEvent);
}
