#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisBase/TreeAnalyzer/interface/TreeCopier.h"
#include "AnalysisBase/TreeAnalyzer/interface/DefaultProcessing.h"
#include "AnalysisBase/TreeAnalyzer/interface/QCDRespSmearingAnalyzer.h"


using namespace std;
using namespace ucsbsusy;

class Copier : public TreeCopierAllBranches {
public:
  Copier(string fileName, string treeName, string outFileName, size randSeed, bool isMCTree, cfgSet::ConfigSet * pars) : TreeCopierAllBranches(fileName,treeName,outFileName,randSeed,isMCTree,pars) {
  };
  virtual ~Copier() {};

  virtual void loadVariables(){
    load(cfgSet::EVTINFO);
    load(cfgSet::AK4JETS,JetReader::LOADRECO | JetReader::LOADGEN | JetReader::FILLOBJ);
  }

  virtual bool fillEvent() {
    if(met->pt() < 100) return false;
//    if(nJets < 2 || jets[0]->pt() < 75) return false;
    return true;
  }

  void book() {
  }

  virtual BaseEventAnalyzer * setupEventAnalyzer() override {
    auto * smearer = new  QCDRespSmearingCopierEventAnalyzer();
    return smearer;
  }

};


#endif

void QCDSmearedSkimmer(string fileName, int fileIndex = -1,  string treeName = "Events", string outPostfix ="qcdSmearSkim", bool isMC = true, int startEvent =-1, int maxEvents = -1) {

  cfgSet::loadDefaultConfigurations();
  cfgSet::ConfigSet cfg = cfgSet::zl_search_set;
  cfg.corrections.jetResCorrType = ucsbsusy::NONE;

  //get the output name
  TString prefix(fileName);
  prefix.Remove(0,prefix.Last('/') + 1);
  if(prefix.First('.') >= 0) prefix.Resize(prefix.First('.'));
  TString outName = fileIndex < 0 ? TString::Format("%s_%s.root",prefix.Data(),outPostfix.c_str()) : TString::Format("%s_%i_%s.root",prefix.Data(),fileIndex,outPostfix.c_str());

  Copier a(fileName,treeName,outName.Data(),fileIndex+2,isMC, &cfg);

  a.analyze(10000,maxEvents,startEvent);
}
