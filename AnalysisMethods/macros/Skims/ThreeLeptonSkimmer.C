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
    load(cfgSet::ELECTRONS);
    load(cfgSet::MUONS);
  }

  virtual bool fillEvent() {

    if(selectedLeptons.size() != 3)
      return false;
    if (selectedLeptons[0]->pt()<20)
      return false;

    bool passSecondTrigLep = false;
    for(unsigned int iL = 1; iL < selectedLeptons.size(); ++iL){
      if(selectedLeptons[iL]->pt() >= (selectedLeptons[iL]->iselectron()?15:10 ) ) passSecondTrigLep = true;
    }
    if(!passSecondTrigLep) return false;


    return true;
  }

  void book() {
  }


};


#endif

void ThreeLeptonSkimmer(string fileName,  string treeName = "TestAnalyzer/Events", string outPostfix ="skimmed", bool isMC = true) {

  cfgSet::loadDefaultConfigurations();
  cfgSet::ConfigSet pars = cfgSet::zl_dilepton_set;
  pars.selectedLeptons = cfgSet::zl_veto_leptons;


  //get the output name
  TString prefix(fileName);
  prefix.Remove(0,prefix.Last('/') + 1);
  if(prefix.First('.') >= 0) prefix.Resize(prefix.First('.'));
  TString outName = TString::Format("%s_%s.root",prefix.Data(),outPostfix.c_str());

  Copier a(fileName,treeName,outName.Data(),isMC, &pars);

  a.analyze();
}
