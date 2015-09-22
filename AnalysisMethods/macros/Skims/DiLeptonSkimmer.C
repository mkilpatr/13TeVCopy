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
//    load(cfgSet::EVTINFO);
    load(cfgSet::ELECTRONS);
    load(cfgSet::MUONS);
  }

  virtual bool fillEvent() {

    if(nSelLeptons < 1) return false;
    if(nVetoedLeptons != 2) return false;
    if(vetoedLeptons[0]->pt() < 20) return false;
    if(vetoedLeptons[1]->pt() < (vetoedLeptons[1]->iselectron() ? 15 : 10)  ) return false;

    return true;
  }

  void book() {
  }


};


#endif

void DiLeptonSkimmer(string fileName,  string treeName = "TestAnalyzer/Events", string outPostfix ="skimmed", bool isMC = true) {

  cfgSet::loadDefaultConfigurations();
  cfgSet::ConfigSet cfg = cfgSet::ol_search_set;
  cfg.selectedLeptons.minMuPt  = 10;
  cfg.selectedLeptons.maxMuEta = 2.4;
  cfg.selectedLeptons.minEPt  = 10;
  cfg.selectedLeptons.maxEEta = 2.4;

  //get the output name
  TString prefix(fileName);
  prefix.Remove(0,prefix.Last('/') + 1);
  if(prefix.First('.') >= 0) prefix.Resize(prefix.First('.'));
  TString outName = TString::Format("%s_%s.root",prefix.Data(),outPostfix.c_str());

  Copier a(fileName,treeName,outName.Data(),isMC, &cfg);

  a.analyze();
}
