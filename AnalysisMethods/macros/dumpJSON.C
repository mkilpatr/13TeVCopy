#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TSystem.h>
#include "AnalysisBase/TreeAnalyzer/interface/BaseTreeAnalyzer.h"
#include "AnalysisBase/TreeAnalyzer/interface/JSONProcessing.h"
#endif

using namespace ucsbsusy;

class JSONAnalyzer : public BaseTreeAnalyzer {

  public :
    JSONAnalyzer(TString fileName, TString treeName, TString outdirName, TString outfileName) : BaseTreeAnalyzer(fileName, treeName, 1, false, 0), outputDir_(outdirName), outputFile_(outfileName), lastRunLumi_(cfgSet::JSONProcessing::RunLumiPair(0,0)), jsonProc_(new cfgSet::JSONProcessing()) {}

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
      TString outfilename = outputDir_ + "/" + outputFile_;
      gSystem->mkdir(outputDir_, true);
      jsonProc_->dumpJSONFile(outfilename);
    }

};



void dumpJSON(TString outputname = "htmht-2015c-pr_1.json",
              const TString fname = "/store/user/apatters/13TeV/290915/merged/htmht-2015c-pr_1_ntuple_postproc.root",
              const TString outputdir = "json",
              const TString fileprefix = "root://cmseos:1094/")
{

  printf("Processing file %s\n", fname.Data());

  TString fullname = fileprefix+fname;

  JSONAnalyzer a (fullname, "Events", outputdir, outputname);

  a.analyze(1000000);

  a.writeJSON();

}
