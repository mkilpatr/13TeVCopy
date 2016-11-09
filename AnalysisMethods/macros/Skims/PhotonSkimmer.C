#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisBase/TreeAnalyzer/interface/TreeCopier.h"
#include "AnalysisBase/TreeAnalyzer/interface/DefaultProcessing.h"
#include "TSystem.h"

using namespace std;
using namespace ucsbsusy;

class Copier : public TreeCopierAllBranches {
public:
  Copier(TString fileName, TString treeName, TString outFileName, size randomSeed, bool isMCTree, cfgSet::ConfigSet * pars) : TreeCopierAllBranches(fileName,treeName,outFileName,randomSeed,isMCTree,pars) {
  };
  virtual ~Copier() {};

  virtual void loadVariables(){
    load(cfgSet::PHOTONS);
  }

  virtual bool fillEvent() {
    if(selectedPhotons.empty()) return false;
    return true;
  }

  void book() {
  }


};


#endif

void PhotonSkimmer(TString sname = "gjets",
    const int fileindex = 0,
    const bool isMC = true,
    const TString fname = "/store/user/vdutta/13TeV/080615/merged/gjets_ht100to200_ntuple_wgtxsec.root",
    const TString outputdir = "trees",
    const TString fileprefix = "root://eoscms//eos/cms",
    const TString json="")
{

  cfgSet::loadDefaultConfigurations();
  cfgSet::ConfigSet cfg = cfgSet::zl_photon_set;
  cfg.photons.minPt = 150;
  cfg.corrections = cfgSet::CorrectionConfig();

  printf("Processing file %d of %s sample\n", (fileindex > -1 ? fileindex : 0), sname.Data());

  if(fileindex > -1)
    sname += TString::Format("_%d",fileindex);

  TString fullname = fileprefix+fname;

  gSystem->mkdir(outputdir,true);
  TString outfilename = outputdir+"/"+sname+"_tree.root";

  Copier a(fullname, "Events", outfilename, fileindex+2, isMC, &cfg);

  a.analyze(100000);
}
