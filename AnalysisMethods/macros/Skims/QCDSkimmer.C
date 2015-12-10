#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisBase/TreeAnalyzer/interface/TreeCopier.h"
#include "AnalysisBase/TreeAnalyzer/interface/DefaultProcessing.h"


using namespace std;
using namespace ucsbsusy;

class Copier : public TreeCopierLoadedBranches {
public:
  Copier(string fileName, string treeName, string outFileName, bool isMCTree, cfgSet::ConfigSet * pars) : TreeCopierLoadedBranches(fileName,treeName,outFileName,0u,isMCTree,pars) {
  };
  virtual ~Copier() {};

  virtual void loadVariables(){
    load(cfgSet::EVTINFO);
    load(cfgSet::AK4JETS,JetReader::LOADRECO | JetReader::LOADGEN | JetReader::FILLOBJ);
    load(cfgSet::CMSTOPS);
  }

  virtual bool fillEvent() {
    return true;
  }

  void book() {
  }


};


#endif

void QCDSkimmer(string fileName,  string treeName = "TestAnalyzer/Events", string outPostfix ="skimmed", bool isMC = true) {

  cfgSet::loadDefaultConfigurations();
  cfgSet::ConfigSet cfg = cfgSet::zl_search_set;

  //get the output name
  TString prefix(fileName);
  prefix.Remove(0,prefix.Last('/') + 1);
  if(prefix.First('.') >= 0) prefix.Resize(prefix.First('.'));
  TString outName = TString::Format("%s_%s.root",prefix.Data(),outPostfix.c_str());

  Copier a(fileName,treeName,outName.Data(),isMC, &cfg);

  a.analyze();
}
