#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/macros/1LepSearchRegions/OneLeptonTreeHelper.hh"
#endif

using namespace ucsbsusy;

void makeOneLeptonSRTrees(TString sname = "singlemu",
                          const int fileindex = -1,
                          const bool isMC = false,
                          const TString fname = "/store/user/gouskos/13TeV/Spring15/20150813/singlemu-2015b-pr_ntuple_postproc.root",
                          const TString outputdir = "trees",
                          const TString fileprefix = "root://eoscms//eos/cms",
                          const TString json="")
{

  printf("Processing file %d of %s sample\n", (fileindex > -1 ? fileindex : 0), sname.Data());

  if(fileindex > -1)
    sname += TString::Format("_%d",fileindex);

  TString fullname = fileprefix+fname;

  gSystem->mkdir(outputdir,true);
  TString outfilename = outputdir+"/"+sname+"_tree.root";
  cfgSet::ConfigSet pars = pars1lep(json);

  TString treename = "Events";
  OneLeptonAnalyzer a(fullname, treename, outfilename, fileindex+2, isMC, &pars);

  a.analyze(10000);

}
