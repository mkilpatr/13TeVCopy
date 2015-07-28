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

using namespace std;
using namespace ucsbsusy;

class Copier : public TreeCopierAllBranches {
public:
  Copier(string fileName, string treeName, string outFileName, bool isMCTree, double nPosEvents, double nNegEvents) : TreeCopierAllBranches(fileName,treeName,outFileName,isMCTree), nPos(nPosEvents), nNeg(nNegEvents), genwgtsign(1.0) {};

  virtual ~Copier() {};

  void loadVariables() {
    load(cfgSet::EVTINFO);
  }

  void book() {
    iXSWeight  = data.add<float> ("","wgtXSec"  ,"F",xsecweight);
    iGenWeight = data.add<float> ("","evtWgtGen","F",1);
    iXSec      = data.add<float> ("","xsection" ,"F",xsec);
    iProcess   = data.add<size8> ("","process"  ,"b",process);
  }

  void processVariables() {
    isProcessed_ = true;
    genwgtsign = evtInfoReader.genweight < 0 ? -1.0 : 1.0;
  }

  bool fillEvent() {
    data.fill<float>(iGenWeight, genwgtsign / (1.0 - (2.0*(nNeg/(nPos+nNeg)))));
    return true;
  }

  size iXSWeight;
  size iGenWeight;
  size iProcess;
  size iXSec;

  double nPos;
  double nNeg;

  defaults::Process process;
  float xsecweight;
  float xsec;
  float genwgtsign;

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

void AddWgt2UCSBntuples(string fileName, string processName, double crossSection, double lumi = 1, double nPosEvents = -1, double nNegEvents = 0, string treeName = "TestAnalyzer/Events", string outPostfix ="skimmed") {

  //get the output name
  TString outName(fileName);
  outName.ReplaceAll(".root", "_"+outPostfix+".root");
  outName.Remove(0,outName.Last('/')+1);

  //get the process
  defaults::Process process = defaults::NUMPROCESSES;
  for(unsigned int iP = 0; defaults::PROCESS_NAMES[iP][0]; ++iP) if(defaults::PROCESS_NAMES[iP] == processName) process = static_cast<defaults::Process>(iP);
  if(process == defaults::NUMPROCESSES) throw std::invalid_argument("Did not provide a valid process name (see defaults::PROCESS_NAMES)");

  Copier a(fileName,treeName,outName.Data(),process != defaults::DATA, nPosEvents, nNegEvents);

  //set weight and process
  a.xsecweight  = lumi * crossSection *1000 / ( nPosEvents > 0 ? (nPosEvents + nNegEvents) : float(a.getEntries())  );
  a.xsec    = crossSection;
  a.process = process;

  clog << "Copying  "<< a.getEntries() <<" events of type " <<  defaults::PROCESS_NAMES[a.process] <<" and xsec weight "<< a.xsecweight << " with fraction of negative weight events " << nNegEvents/(nPosEvents+nNegEvents) << " into file "<< outName << endl;
  a.analyze();
}
