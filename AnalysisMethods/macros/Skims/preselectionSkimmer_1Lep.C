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

class Copier : public TreeCopierLoadedBranches {
public:
  Copier(string fileName, string treeName, string outFileName, bool isMCTree, cfgSet::ConfigSet * pars) : TreeCopierLoadedBranches(fileName,treeName,outFileName,isMCTree,pars) {
  };
  virtual ~Copier() {};

  virtual void loadVariables(){
    load(cfgSet::EVTINFO);
    if(isMC())load(cfgSet::AK4JETS,JetReader::LOADRECO | JetReader::LOADTOPASSOC | JetReader::FILLOBJ | JetReader::LOADGEN);
    else load(cfgSet::AK4JETS,JetReader::LOADRECO | JetReader::LOADTOPASSOC | JetReader::FILLOBJ);
    load(cfgSet::ELECTRONS);
    load(cfgSet::MUONS);
    load(cfgSet::TRIGOBJS);
    load(cfgSet::TAUS);
    if(isMC()) load(cfgSet::GENPARTICLES);

  }

  virtual bool fillEvent() {

    if(nJets < 2) return false;
    if(nSelLeptons == 0 || nVetoedLeptons > 2) return false;
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

void preselectionSkimmer_1Lep(string fileName,  string treeName = "TestAnalyzer/Events", string outPostfix ="skimmed", bool isMC = true) {

  cfgSet::loadDefaultConfigurations();
  cfgSet::ConfigSet cfg = cfgSet::ol_search_set;
  cfg.jets.minPt = 20;
  cfg.jets.minBJetPt = 20;
  cfg.jets.cleanJetsvVetoedLeptons = true;

  //get the output name
  TString prefix(fileName);
  prefix.Remove(0,prefix.Last('/') + 1);
  if(prefix.First('.') >= 0) prefix.Resize(prefix.First('.'));
  TString outName = TString::Format("%s_%s.root",prefix.Data(),outPostfix.c_str());

  Copier a(fileName,treeName,outName.Data(),isMC, &cfg);

  a.analyze();
}
