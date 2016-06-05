
#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/macros/0LFBDSearchRegions/T2fbdHelper.hh"
#include<TSystem.h>
#endif

using namespace ucsbsusy;

void makeZeroLeptonSRTreesFBD(TString sname = "htmht",
			      const int fileindex = -1,
			      const bool isMC = false,
			      const TString fname = "/store/user/gouskos/13TeV/Spring15/20150813/htmht-2015b-pr_ntuple_postproc.root",
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
  cfgSet::ConfigSet pars = pars0lep(json);
  
  pars.corrections.ttbarCorrections |= ucsbsusy::TtbarCorrectionSet::TOPPAIRPT;

  // disable jetID for FastSim
  if (sname.Contains("T2tt") || sname.Contains("T2tb") || sname.Contains("T2bW") || sname.Contains("T2fbd")) pars.jets.applyJetID = false;

  TString treeName = "Events";
  //TString treeName = "TestAnalyzer/Events";
  ZeroLeptonAnalyzer a(fullname, treeName, outfilename, fileindex+2, isMC, &pars);

  // CHF filter for FastSim
  if (sname.Contains("T2tt") || sname.Contains("T2tb") || sname.Contains("T2bW") || sname.Contains("T2fbd")) a.applyCHFFilter = true;

  a.analyze(1000000);
  //  a.analyze(10000,50000);

}
