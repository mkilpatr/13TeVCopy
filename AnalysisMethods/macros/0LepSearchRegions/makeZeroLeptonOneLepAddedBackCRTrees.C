#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/macros/0LepSearchRegions/ZeroLeptonTreeHelper.hh"
#endif

using namespace ucsbsusy;

class OneLepCRAnalyzer : public ZeroLeptonAnalyzer {

  public :

    OneLepCRAnalyzer(TString fileName, TString treeName, TString outfileName, bool isMCTree,cfgSet::ConfigSet *pars, DataType type=MC) :
      ZeroLeptonAnalyzer(fileName, treeName, outfileName, isMCTree, pars, type) {}



    bool fillEvent() {
      if(nSelLeptons!=1)      return false;
      MomentumF* lep = new MomentumF(selectedLeptons.at(0)->p4());
      MomentumF* metn = new MomentumF(met->p4() + lep->p4());
      if(metn->pt() < metcut_) return false;
      if(!goodvertex) return false;
   // if(nVetoedTracks > 0)     return false;
      if(nJets < 5) return false;
      if(nBJets < 1) return false;
      if(fabs(PhysicsUtilities::deltaPhi(*metn, *selectedLeptons[0])) > 1)        return false;

      filler.fillEventInfo(&data, this,0,true,metn);
      filler.fillJetInfo(&data, jets, bJets, metn);
      return true;
    }

};

void makeZeroLeptonOneLepAddedBackCRTrees(TString sname = "ttbar_onelepcr",
                                 const int fileindex = 0,
                                 const bool isMC = true,
                                 const TString fname = "/store/user/vdutta/13TeV/080615/merged/ttbar_1_ntuple_wgtxsec.root",
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

  cfgSet::loadDefaultConfigurations();
  cfgSet::ConfigSet cfg = cfgSet::zl_lepton_set;
  cfg.jets.cleanJetsvSelectedLeptons = true;
  cfg.selectedLeptons.minEPt = 40;
  cfg.selectedLeptons.minMuPt = 30;

  TString treename = isMC ? "Events" : "TestAnalyzer/Events";
  DataType type = isMC ? MC : (fname.Contains("htmht") ? HTMHT : (fname.Contains("singlemu") ? SINGLEMU : (fname.Contains("singleel") ? SINGLEEL : MC)));
  OneLepCRAnalyzer a(fullname, "Events", outfilename, isMC, &cfg, type);

  a.analyze(100000);

}
