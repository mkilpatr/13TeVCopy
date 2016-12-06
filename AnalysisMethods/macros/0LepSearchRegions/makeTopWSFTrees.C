#if !defined(__CINT__) || defined(__MAKECINT__)
//#include "AnalysisMethods/macros/0LepSearchRegions/TopWSFTreeHelper.hh"
//#include "AnalysisMethods/macros/0LepSearchRegions/MistagSFNb0TreeHelper.hh"
//#include "AnalysisMethods/macros/0LepSearchRegions/MistagSF0LepTreeHelper.hh"
//#include "AnalysisMethods/macros/0LepSearchRegions/MistagSFGammaTreeHelper.hh"
//#include "AnalysisMethods/macros/0LepSearchRegions/TopWSFDiJetTreeHelper.hh"
//#include "AnalysisMethods/macros/0LepSearchRegions/ResTopSFTreeHelper.hh"
#include "AnalysisMethods/macros/0LepSearchRegions/MistagResTopSF0LepTreeHelper.hh"
#endif

using namespace ucsbsusy;

void makeTopWSFTrees(TString sname = "T2tt_750_100",
                           const int fileindex = -1,
                           const bool isMC = true,
                           const TString fname = "/store/user/apatters/13TeV/271115/merged/T2tt_750_100_ntuple_postproc.root",
                           const TString outputdir = "trees",
                           const TString fileprefix = "root://cmseos:1094/",
                           const TString json=TString::Format("%s/src/data/JSON/Cert_246908-260627_13TeV_PromptReco_Collisions15_25ns_JSON_v2.txt",getenv("CMSSW_BASE")))
{

  printf("Processing file %d of %s sample\n", (fileindex > -1 ? fileindex : 0), sname.Data());

  if(fileindex > -1)
    sname += TString::Format("_%d",fileindex);

  TString fullname = fileprefix+fname;

  gSystem->mkdir(outputdir,true);
  TString outfilename = outputdir+"/"+sname+"_tree.root";
  cfgSet::ConfigSet pars = pars0lep(json);

  pars.corrections.ttbarCorrections |= ucsbsusy::TtbarCorrectionSet::TOPPAIRPT;


  TString treeName = "Events";
  TopWSFAnalyzer a(fullname, treeName, outfilename, fileindex+2, isMC, &pars);



  a.analyze(1000000);

  //a.analyze(10000,100000);

}
