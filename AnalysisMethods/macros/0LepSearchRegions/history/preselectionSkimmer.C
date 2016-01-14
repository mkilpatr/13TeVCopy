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
  Copier(string fileName, string treeName, string outFileName, bool isMCTree, ConfigPars * pars) : TreeCopierAllBranches(fileName,treeName,outFileName,isMCTree,pars) {
  };
  virtual ~Copier() {};

  virtual void loadVariables(){
    load(EVTINFO);
    load(AK4JETS,JetReader::LOADRECO | JetReader::LOADJETSHAPE | JetReader::FILLOBJ);
    load(PICKYJETS,JetReader::LOADRECO | JetReader::LOADJETSHAPE | JetReader::FILLOBJ);
    load(ELECTRONS);
    load(MUONS);
    load(PFCANDS);
  }

  virtual bool fillEvent() {

    //Trigger requirements
    if(PhysicsUtilities::countObjectsDeref(jets,90,2.4) < 2) return false;
    if(met->pt() < 200) return false;

    //Tight b-tag requirement
    int nTAK4   = 0;
    int nTPICKY = 0;
    for(auto* j : bJets) if(isTightBJet(*j)) nTAK4++;

    vector<RecoJetF*> pickyJets;
    vector<RecoJetF*> pickyBJets;
    vector<RecoJetF*> pickyNonBJets;
    cleanJets(&pickyJetReader,pickyJets,&pickyBJets,&pickyNonBJets);
    for(auto* j : pickyBJets) if(isTightBJet(*j)) nTPICKY++;

    if(nTPICKY + nTAK4 == 0) return false;

    //5 jet requirement
    if(nJets < 4 && pickyJets.size() < 4) return false;

    //no leptons
    if(nSecondaryLeptons + nVetoedTaus > 0) return false;

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

void preselectionSkimmer(string fileName, string processName, double crossSection, double lumi = 4, double nEvents = -1,  string treeName = "TestAnalyzer/Events", string outPostfix ="skimmed") {
  BaseTreeAnalyzer::ConfigPars pars;
  pars.cleanJetsvSelectedLeptons_ = false;
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
  a.weight = lumi * crossSection *1000 / ( nEvents > 0 ? nEvents : float(a.getEntries())  );
  a.process = process;

  clog << "Skimming "<< a.getEntries() <<" events of type " <<  defaults::PROCESS_NAMES[a.process] <<" and weight "<< a.weight <<" into file "<< outName << endl;
  a.analyze();
}
