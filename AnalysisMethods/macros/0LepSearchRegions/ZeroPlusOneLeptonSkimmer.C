/*
 * Uses the tree copier to copy over a tree!
 *
 * We use the loaded branches option so that we make the tree smaller
 *
 * Use this guy as a base class or an example!
 *
 */
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
    load(cfgSet::AK4JETS,JetReader::LOADRECO | JetReader::LOADJETSHAPE | JetReader::FILLOBJ);
    load(cfgSet::PICKYJETS,JetReader::LOADRECO | JetReader::LOADJETSHAPE | JetReader::FILLOBJ);
//    load(cfgSet::ELECTRONS);
    load(cfgSet::MUONS);
//    load(cfgSet::PFCANDS);
  }

  virtual bool fillEvent() {

    MomentumF cutMET = *met;

    cfgSet::processMET(cutMET,&selectedLeptons,0,cfgSet::zl_lplus_met);
    if(cutMET.pt() < 200 && met->pt() < 200) return false;

    if(nJets < 5) return false;
    if(nBJets < 1) return false;
    return true;
  }

  void book() {
  }


};


#endif

/*
 * process -> string from defaults::PROCESS_NAMES
 * crossSection -> processCrossSection
 * lumi -> luminosity that you wish to scale for
 * nEvents -> number of events in sample (if < 0 take it from the tree)
 *
 */

void ZeroPlusOneLeptonSkimmer(string fileName,  string treeName = "TestAnalyzer/Events", string outPostfix ="skimmed") {

  cfgSet::loadDefaultConfigurations();
  cfgSet::ConfigSet cfg = cfgSet::zl_search_set;
  cfg.vetoedLeptons.selectedMuon = (&MuonF::ismultiisovetomuonl);
  cfg.vetoedLeptons.selectedElectron = (&ElectronF::ismultiisovetoelectronl);

  //get the output name
  TString prefix(fileName);
  prefix.Remove(0,prefix.Last('/') + 1);
  if(prefix.First('.') >= 0) prefix.Resize(prefix.First('.'));
  TString outName = TString::Format("%s_%s.root",prefix.Data(),outPostfix.c_str());

  Copier a(fileName,treeName,outName.Data(),true, &cfg);

  a.analyze();
}
