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
#include "TRegexp.h"

using namespace std;
using namespace ucsbsusy;

class Copier : public TreeCopierAllBranches {
public:
  Copier(string fileName, string treeName, string outFileName, bool isMCTree, double nPosEvents, double nNegEvents, string inputFile) : TreeCopierAllBranches(fileName,treeName,outFileName,1,isMCTree,0), nPos(nPosEvents), nNeg(nNegEvents), genwgtsign(1.0), xsecFile(0), xsecLookup(0) {
    if(isMC() && inputFile != "" && inputFile != "NONE") {
      xsecFile = new TFile(TString::Format("%s/src/data/xsecs/%s",getenv("CMSSW_BASE"),inputFile.c_str()));
      assert(xsecFile);
      xsecLookup = (TH1D*)xsecFile->Get("xsecs");
      assert(xsecLookup);
    }
  }

  virtual ~Copier() {};

  void loadVariables() {
    load(cfgSet::EVTINFO);
  }

  void book() {
    if(isMC()) {
      iXSWeight  = data.add<float> ("","wgtXSec"  ,"F",xsecweight);
      iGenWeight = data.add<float> ("","evtWgtGen","F",1);
      iXSec      = data.add<float> ("","xsection" ,"F",xsec);
    }
    iProcess     = data.add<size8> ("","process"  ,"b",process);
    if(!isMC())
      iDataReco  = data.add<size8> ("","datareco"  ,"b",datareco);
  }

  void processVariables() {
    isProcessed_ = true;
    genwgtsign = isMC() && evtInfoReader.genweight < 0 ? -1.0 : 1.0;
    if(xsecLookup) {
      xsec = xsecLookup->Interpolate(evtInfoReader.massPar1);
      xsecweight = !isMC() ? 1. : (lumi * xsec * 1000 / ( nPos > 0 ? (nPos + nNeg) : float(getEntries())  ));
    }
  }

  bool fillEvent() {
    if(isMC()) {
      data.fill<float>(iGenWeight, genwgtsign / (1.0 - (2.0*(nNeg/(nPos+nNeg)))));
      if(xsecLookup) {
        data.fill<float>(iXSec, xsec);
        data.fill<float>(iXSWeight, xsecweight);
      }
      if(qcdGenJets5Scale < 1 && evtInfoReader.nStandardGenJets >= 5){
        data.fill<float>(iXSWeight, qcdGenJets5Scale * xsecweight);
      }
    }
    return true;
  }

  size iXSWeight;
  size iGenWeight;
  size iProcess;
  size iDataReco;
  size iXSec;

  double nPos;
  double nNeg;

  double lumi;

  defaults::Process process;
  defaults::DataReco datareco;
  float xsecweight;
  float xsec;
  float genwgtsign;

  TFile* xsecFile;
  TH1D*  xsecLookup;

  double qcdGenJets5Scale;
};


#endif

/*
 * process -> string from defaults::PROCESS_NAMES
 * crossSection -> processCrossSection
 * lumi -> luminosity that you wish to scale for
 * nEvents -> number of events in sample (if < 0 take it from the tree)
 *
 */
//AddWgt2UCSBntuples("root://eoscms//eos/cms//eos/cms/store/user/gouskos/13TeV/Phys14/20150503/merged/wjets_ht600toInf_ntuple.root","wjets_ht600toInf",100,1.,4581841,0,"TestAnalyzer/Events","wgt")

void AddWgt2UCSBntuples(string fileName, string processName, double crossSection, double lumi = 1, double nPosEvents = -1, double nNegEvents = 0, string treeName = "TestAnalyzer/Events", string outPostfix ="skimmed", string xsecFile = "", double wgtsf = 1) {

  //get the output name
  TString outName(fileName);
  outName.ReplaceAll(".root", "_"+outPostfix+".root");
  outName.Remove(0,outName.Last('/')+1);

  //get the process
  defaults::Process process = defaults::NUMPROCESSES;
  for(unsigned int iP = 0; defaults::PROCESS_NAMES[iP][0]; ++iP) if(defaults::PROCESS_NAMES[iP] == processName) process = static_cast<defaults::Process>(iP);
  if(process == defaults::NUMPROCESSES) throw std::invalid_argument("Did not provide a valid process name (see defaults::PROCESS_NAMES)");

  bool isMC = process > defaults::DATA && process <= defaults::SIGNAL;

  defaults::DataReco datareco = defaults::MC;
  if(!isMC) {
    TString recostr = outName;
    recostr.ReplaceAll("_ntuple_"+outPostfix+".root","");
    recostr.Remove(0, recostr.First('-')+1);
    TRegexp ext1("_[0-9]+");
    if (recostr.Index(ext1) != TString::kNPOS) recostr.Remove(recostr.Index(ext1));
    TRegexp ext2("-[1-9]");
    if (recostr.Index(ext2) != TString::kNPOS) recostr.Remove(recostr.Index(ext2));
    for(unsigned int iP = 0; defaults::DATA_RECO_NAMES[iP] != "invalid"; ++iP) if(defaults::DATA_RECO_NAMES[iP] == recostr) datareco = static_cast<defaults::DataReco>(iP);
    if(datareco == defaults::MC) throw std::invalid_argument("Did not provide a valid data reco name (see defaults::DATA_RECO_NAMES)");
  }

  Copier a(fileName,treeName,outName.Data(),isMC,nPosEvents,nNegEvents,xsecFile);

  //set weight and process
  a.xsecweight  = !isMC ? 1. : (lumi * crossSection *1000 / ( nPosEvents > 0 ? (nPosEvents + nNegEvents) : float(a.getEntries())  ));
  a.xsec        = !isMC ? 1. : crossSection;
  a.lumi        = lumi;
  a.process     = process;
  a.datareco    = datareco;
  a.qcdGenJets5Scale = wgtsf;

  if (isMC) clog << "Copying  "<< a.getEntries() <<" events of type " <<  defaults::PROCESS_NAMES[a.process] <<" and xsec weight "<< a.xsecweight << " with fraction of negative weight events " << nNegEvents/(nPosEvents+nNegEvents) << " into file "<< outName << endl;
  else      clog << "Copying  "<< a.getEntries() <<" events of type " <<  defaults::PROCESS_NAMES[a.process] << " into file " << outName << endl;
  a.analyze();
}
