#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TSystem.h>
#include "AnalysisBase/TreeAnalyzer/interface/BaseTreeAnalyzer.h"
#include "AnalysisBase/TreeAnalyzer/interface/JSONProcessing.h"
#endif

using namespace ucsbsusy;

class JSONAnalyzer : public BaseTreeAnalyzer {

  public :
    JSONAnalyzer(TString fileName, TString treeName, TString outfileName) : BaseTreeAnalyzer(fileName, treeName, 1, false, 0), outputFile_(outfileName), lastRunLumi_(cfgSet::JSONProcessing::RunLumiPair(0,0)), jsonProc_(new cfgSet::JSONProcessing()) {}

    virtual ~JSONAnalyzer() {}

    const TString outputDir_;
    const TString outputFile_;
    cfgSet::JSONProcessing::RunLumiPair lastRunLumi_;
    cfgSet::JSONProcessing* jsonProc_;

    void loadVariables() {
      load(cfgSet::EVTINFO);
    }

    void runEvent() {
      cfgSet::JSONProcessing::RunLumiPair rlpair (run, lumi);
      if (rlpair != lastRunLumi_) {
        lastRunLumi_ = rlpair;
        jsonProc_->addRunLumi(rlpair);
      }
    }

    void writeJSON() {
      jsonProc_->dumpJSONFile(outputFile_);
    }

};



void dumpJSON(TString sname = "met",
    const int fileindex = -1,
    const bool isMC = false,
    const TString fname = "/store/user/hqu/13TeV/271115/merged/met-2015c-05oct15_1_ntuple_postproc.root",
    const TString outputdir = "json",
    const TString fileprefix = "root://cmseos:1094/",
    const TString json="")
{

  printf("Processing file %s\n", fname.Data());

  if(fileindex > -1)
    sname += TString::Format("_%d",fileindex);

  gSystem->mkdir(outputdir,true);
  TString outputname = outputdir+"/"+sname+".json";

  TString fullname = fileprefix+fname;

  JSONAnalyzer a (fullname, "Events", outputname);

  a.analyze(100000);

  a.writeJSON();

}
