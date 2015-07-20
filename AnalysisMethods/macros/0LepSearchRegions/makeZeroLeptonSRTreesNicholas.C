#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/macros/0LepSearchRegions/ZeroLeptonTreeHelper.hh"
#include "AnalysisBase/TreeAnalyzer/interface/JetCorrections.h"
#endif

using namespace ucsbsusy;

void makeZeroLeptonSRTreesNicholas(TString sname = "T2tt_850_100",
                                   const int fileindex = -1,
                                   const bool isMC = true,
                                   const TString fname =
                                       "/store/user/vdutta/13TeV/080615/merged/T2tt_850_100_ntuple_wgtxsec.root",
                                   const double xsec = 1.0,
                                   const TString outputdir = "trees",
                                   const TString fileprefix = "root://eoscms//eos/cms")
{
    printf("Processing file %d of %s sample\n", (fileindex > -1 ? fileindex : 0),
           sname.Data());
    if (fileindex > -1)
    { sname += TString::Format("_%d", fileindex); }
    if (isMC)
    { printf("Cross section: %5.2f pb\n", xsec); }
    TString fullname = fileprefix + fname;
    gSystem->mkdir(outputdir, true);
    TString outfilename = outputdir + "/" + sname + "_tree.root";
    cfgSet::ConfigSet pars = pars0lep();
    /********** JETCORRECTION TEST **********/
    pars.jets.JES = JetCorrector::JES_UP;
    /**************************/
    ZeroLeptonAnalyzer a(fullname, "Events", outfilename, isMC, &pars);
    a.analyze(1);
}
