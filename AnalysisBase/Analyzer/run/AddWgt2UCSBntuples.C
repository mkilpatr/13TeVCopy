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


  void book() {
    iWeight  = data.add<float> ("","wgtXSec" ,"F",weight);
    iXSec    = data.add<float> ("","xsection","F",xsec);
    iProcess = data.add<size8> ("","process" ,"b",process);
  }
  size iWeight;
  size iProcess;
  size iXSec;

  defaults::Process process;
  float weight;
  float xsec;

};


#endif

/*
 * process -> string from defaults::PROCESS_NAMES
 * crossSection -> processCrossSection
 * lumi -> luminosity that you wish to scale for
 * nEvents -> number of events in sample (if < 0 take it from the tree)
 *
 */
//AddWgt2UCSBntuples("root://eoscms//eos/cms//eos/cms/store/user/gouskos/13TeV/Phys14/20150503/merged/wjets_ht600toInf_ntuple.root","wjets_ht600toInf",100,1.,4581841,"TestAnalyzer/Events","wgt")

void AddWgt2UCSBntuples(string fileName, string processName, double crossSection, double lumi = 1, double nEvents = -1,  string treeName = "TestAnalyzer/Events", string outPostfix ="skimmed") {
  BaseTreeAnalyzer::ConfigPars pars;
  pars.cleanJetsvSelectedLeptons_ = false;
  pars.defaultJetCollection = BaseTreeAnalyzer::AK4JETS;

  //get the output name
  TString outName(fileName);
  outName.ReplaceAll(".root", "_"+outPostfix+".root");

  //get the process
  defaults::Process process = defaults::NUMPROCESSES;
  for(unsigned int iP = 0; defaults::PROCESS_NAMES[iP][0]; ++iP) if(defaults::PROCESS_NAMES[iP] == processName) process = static_cast<defaults::Process>(iP);
  if(process == defaults::NUMPROCESSES) throw std::invalid_argument("Did not provide a valid process name (see defaults::PROCESS_NAMES)");

  Copier a(fileName,treeName,outName.Data(),process != defaults::DATA, &pars);
  //set weight and process
  a.weight  = lumi * crossSection *1000 / ( nEvents > 0 ? nEvents : float(a.getEntries())  );
  a.xsec    = crossSection;
  a.process = process;

  clog << "Copying  "<< a.getEntries() <<" events of type " <<  defaults::PROCESS_NAMES[a.process] <<" and weight "<< a.weight <<" into file "<< outName << endl;
  a.analyze();
}
