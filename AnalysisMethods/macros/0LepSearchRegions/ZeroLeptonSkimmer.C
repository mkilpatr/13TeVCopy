#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisBase/TreeAnalyzer/interface/TreeCopier.h"
#include "AnalysisBase/TreeAnalyzer/interface/DefaultProcessing.h"


using namespace std;
using namespace ucsbsusy;

class Copier : public TreeCopierAllBranches {
public:
  Copier(string fileName, string treeName, string outFileName, bool isMCTree, cfgSet::ConfigSet * pars) : TreeCopierAllBranches(fileName,treeName,outFileName,isMCTree,pars) {
  };
  virtual ~Copier() {};

  virtual void loadVariables(){
    load(cfgSet::EVTINFO);
    load(cfgSet::AK4JETS,JetReader::LOADRECO | JetReader::FILLOBJ);
  }

  virtual bool fillEvent() {

    if(met->pt() < 175 && metNoHF->pt() < 175) return false;
    if(nJets < 2 || jets[1]->pt() < 60) return false;
//    if(nBJets < 1) return false;
    return true;
  }

  void book() {
  }


};


#endif

void ZeroLeptonSkimmer(string fileName,  string treeName = "TestAnalyzer/Events", string outPostfix ="skimmed", bool isMC = true) {

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
