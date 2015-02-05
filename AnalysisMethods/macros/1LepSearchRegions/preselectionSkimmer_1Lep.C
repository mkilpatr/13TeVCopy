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
#include "AnalysisTools/TreeReader/interface/Defaults.h"
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
#include "AnalysisTools/Parang/interface/Panvariate.h"


using namespace std;
using namespace ucsbsusy;

class Copier : public TreeCopierAllBranches {
public:
  Copier(string fileName, string treeName, string outFileName, bool isMCTree, ConfigPars* pars) : TreeCopierAllBranches(fileName,treeName,outFileName,isMCTree,pars) {

  };
  virtual ~Copier() {};

  virtual bool fillEvent() {
    if(nSelLeptons == 0) return false;
    if(met->pt() < 150) return false;

    //we need to count picky jets
    vector<RecoJetF*> pickyJets;
    cleanJets(&pickyJetReader,pickyJets,0,0);
    if(nJets < 3 && pickyJets.size() < 3) return false;
    return true;
  }

  void book() {
    iWeight      = data.add<float>       ("","weight"      ,"F",weight);
    iProcess     = data.add<size8>       ("","process"     ,"b",process);
  }
  size iWeight;
  size iProcess;

  defaults::Process process;
  float weight;

};


#endif

/*
 * process -> string from defaults::PROCESS_NAMES
 * crossSection -> processCrossSection
 * lumi -> luminosity that you wish to scale for
 * nEvents -> number of events in sample (if < 0 take it from the tree)
 *
 */

void preselectionSkimmer_1Lep(string fileName, string processName, double crossSection, double lumi = 4, double nEvents = -1,  string treeName = "TestAnalyzer/Events", string outPostfix ="skimmed") {

  BaseTreeAnalyzer::ConfigPars pars;
  pars.cleanJetsvSelectedLeptons_ = true;
  pars.leptonSelection = BaseTreeAnalyzer::SEL_1_LEP;
  pars.defaultJetCollection = BaseTreeAnalyzer::AK4JETS;

  //get the output name
  TString prefix(fileName);
  prefix.Remove(0,prefix.Last('/') + 1);
  if(prefix.First('.') >= 0) prefix.Resize(prefix.First('.'));
  TString outName = TString::Format("%s_%s.root",prefix.Data(),outPostfix.c_str());

  //get the process
  defaults::Process process = defaults::NUMPROCESSES;
  for(unsigned int iP = 0; defaults::PROCESS_NAMES[iP][0]; ++iP) if(defaults::PROCESS_NAMES[iP] == processName) process = static_cast<defaults::Process>(iP);
  if(process == defaults::NUMPROCESSES) throw std::invalid_argument("Did not provide a valid process name (see defaults::PROCESS_NAMES)");

  Copier a(fileName,treeName,outName.Data(),process != defaults::DATA, &pars);
  //set weight and process
  a.weight = lumi * 1000*crossSection / ( nEvents > 0 ? min(nEvents,double(a.getEntries())) : double(a.getEntries())  );
  a.process = process;

  clog << "Skimming "<< a.getEntries() <<" events of type " <<  defaults::PROCESS_NAMES[a.process] <<" and weight "<< a.weight <<" into file "<< outName << endl;
  a.analyze(10000,nEvents);
}
