#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/macros/1LepSearchRegions/OneLeptonTreeHelper.hh"
#endif

using namespace ucsbsusy;

void makeOneLeptonSRTrees(TString sname = "singlemu",
                          const int fileindex = -1,
                          const bool isMC = false,
                          const TString fname = "/store/user/vdutta/13TeV/210715/merged/singlemu-2015b-pr_ntuple.root",
                          const double xsec = 1.0,
                          const TString outputdir = "trees",
                          const TString fileprefix = "root://eoscms//eos/cms")
{

  printf("Processing file %d of %s sample\n", (fileindex > -1 ? fileindex : 0), sname.Data());

  if(fileindex > -1)
    sname += TString::Format("_%d",fileindex);

  if(isMC)
    printf("Cross section: %5.2f pb\n", xsec);

  TString fullname = fileprefix+fname;

  gSystem->mkdir(outputdir,true);
  TString outfilename = outputdir+"/"+sname+"_tree.root";
  cfgSet::ConfigSet pars = pars1lep();

  TString treename = isMC ? "Events" : "TestAnalyzer/Events";
  DataType type = isMC ? MC : (fname.Contains("singlemu") ? SINGLEMU : (fname.Contains("singleel") ? SINGLEEL : MC));
  OneLeptonAnalyzer a(fullname, treename, outfilename, isMC, &pars, type);

  a.analyze(10000);

}
