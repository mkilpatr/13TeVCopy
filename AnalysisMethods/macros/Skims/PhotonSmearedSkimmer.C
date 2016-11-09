#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisBase/TreeAnalyzer/interface/TreeCopier.h"
#include "AnalysisBase/TreeAnalyzer/interface/DefaultProcessing.h"
#include "AnalysisBase/TreeAnalyzer/interface/QCDRespSmearingAnalyzer.h"

using namespace std;
using namespace ucsbsusy;

TString smearTemplateName = "resTailOut_combined_filtered_newerCHEF_puWeight_weight_WoH.root";

class Copier : public TreeCopierAllBranches {
public:
  Copier(TString fileName, TString treeName, TString outFileName, size randomSeed, bool isMCTree, cfgSet::ConfigSet * pars) : TreeCopierAllBranches(fileName,treeName,outFileName,randomSeed,isMCTree,pars) {
  };
  virtual ~Copier() {};

  virtual void loadVariables() override{
    load(cfgSet::EVTINFO);
    load(cfgSet::AK4JETS,JetReader::LOADRECO | JetReader::LOADGEN | JetReader::FILLOBJ);
    load(cfgSet::PHOTONS);
  }

  virtual bool fillEvent() override {
    if(selectedPhotons.empty()) return false;
    return true;
  }

  void book() override {
  }

  virtual BaseEventAnalyzer * setupEventAnalyzer() override {
    auto * smearer = new  QCDRespSmearingCopierEventAnalyzer();
    smearer->smearOptions.respFileName   = TString(cfgSet::CMSSW_BASE) + "/src/data/corrections/2016/" + smearTemplateName;
    smearer->smearOptions.respInputName  = "JetResByFlav";
//    smearer->smearOptions.maxWindow = 1.0;
    return smearer;
  }


};


#endif

void PhotonSmearedSkimmer(TString sname = "gjets",
    const int fileindex = 0,
    const bool isMC = true,
    const TString fname = "/store/user/vdutta/13TeV/080615/merged/gjets_ht100to200_ntuple_wgtxsec.root",
    const TString outputdir = "trees",
    const TString fileprefix = "root://eoscms//eos/cms",
    const TString json="")
{

  cfgSet::loadDefaultConfigurations();
  cfgSet::ConfigSet cfg = cfgSet::zl_photon_set;
  cfg.corrections.jetResCorrType = ucsbsusy::NONE;
  cfg.photons.minPt = 180;

  printf("Processing file %d of %s sample\n", (fileindex > -1 ? fileindex : 0), sname.Data());

  if(fileindex > -1)
    sname += TString::Format("_%d",fileindex);

  TString fullname = fileprefix+fname;

  gSystem->mkdir(outputdir,true);
  TString outfilename = outputdir+"/"+sname+"_tree.root";

  Copier a(fullname, "Events", outfilename, fileindex+2, isMC, &cfg);

  a.analyze(100000);
}
