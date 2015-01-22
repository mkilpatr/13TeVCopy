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

using namespace std;
using namespace ucsbsusy;

class Copier : public TreeCopierLoadedBranches {
public:
  Copier(string fileName, string treeName, string outFileName, bool isMCTree) : TreeCopierLoadedBranches(fileName,treeName,outFileName,isMCTree) {};
  virtual ~Copier() {};

  virtual void loadVariables(){
    load(EVTINFO);
    load(PUPPIJETS,JetReader::LOADRECO | JetReader::LOADJETSHAPE | JetReader::FILLOBJ);
    load(ELECTRONS);
    load(MUONS);
    load(TAUS);
    setDefaultJets(PUPPIJETS);
  }

  virtual bool fillEvent() {

    //lepton veto
    if(nLeptons + nTaus > 0) return false;

    //Trigger requirements
    if(PhysicsUtilities::countObjectsDeref(jets,90,2.4) < 2) return false;
    if(met->pt() < 200) return false;

    //Tight b-tag requirement
    int nTB = 0;
    for(auto* j : bJets) if(isTightBJet(*j)) nTB++;
    if(!nTB) return false;

    //5 jet requirement
    if(nJets < 5) return false;

    //QCD killing cuts
    JetReader * dJets = getDefaultJets();
    if(TMath::Abs(PhysicsUtilities::deltaPhi(dJets->recoJets[0],*met)) < .5) return false;
    if(TMath::Abs(PhysicsUtilities::deltaPhi(dJets->recoJets[1],*met)) < .5) return false;
    if(TMath::Abs(PhysicsUtilities::deltaPhi(dJets->recoJets[2],*met)) < .3) return false;

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

void preselectionSkimmer(string fileName, string processName, double crossSection, double lumi = 5, double nEvents = -1,  string treeName = "TestAnalyzer/Events", string outPostfix ="skimmed") {

  //get the output name
  TString prefix(fileName);
  prefix.Remove(0,prefix.Last('/') + 1);
  if(prefix.First('.') >= 0) prefix.Resize(prefix.First('.'));
  TString outName = TString::Format("%s_%s.root",prefix.Data(),outPostfix.c_str());

  //get the process
  defaults::Process process = defaults::NUMPROCESSES;
  for(unsigned int iP = 0; defaults::PROCESS_NAMES[iP][0]; ++iP) if(defaults::PROCESS_NAMES[iP] == processName) process = static_cast<defaults::Process>(iP);
  if(process == defaults::NUMPROCESSES) throw std::invalid_argument("Did not provide a valid process name (see defaults::PROCESS_NAMES)");

  Copier a(fileName,treeName,outName.Data(),process != defaults::DATA);
  //set weight and process
  a.weight = lumi * crossSection / ( nEvents > 0 ? nEvents : float(a.getEntries())  );
  a.process = process;

  clog << "Skimming "<< a.getEntries() <<" events of type " <<  defaults::PROCESS_NAMES[a.process] <<" and weight "<< a.weight <<" into file "<< outName << endl;
  a.analyze();
}
