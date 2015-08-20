#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/macros/0LepSearchRegions/ZeroLeptonTreeHelper.hh"
#endif

using namespace ucsbsusy;

class OneLepCRAnalyzer : public ZeroLeptonAnalyzer {

  public :

    OneLepCRAnalyzer(TString fileName, TString treeName, TString outfileName, bool isMCTree,cfgSet::ConfigSet *pars) :
      ZeroLeptonAnalyzer(fileName, treeName, outfileName, isMCTree, pars) {}

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

      filler.fillEventInfo(&data, this, 0, true, metn);
      filler.fillJetInfo(&data, jets, bJets, metn);
      return true;
    }

};

void makeZeroLeptonOneLepAddedBackCRTrees(TString sname = "ttbar_onelepcr",
                                 const int fileindex = 0,
                                 const bool isMC = true,
                                 const TString fname = "/store/user/gouskos/13TeV/Spring15/20150813/ttbar-madgraphmlm-50ns_1_ntuple_postproc.root",
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

  cfgSet::ConfigSet pars = pars0lepCR();

/*  cfgSet::loadDefaultConfigurations();
  cfgSet::ConfigSet cfg = cfgSet::zl_lepton_set;
  cfg.jets.cleanJetsvSelectedLeptons = true;
  cfg.selectedLeptons.minEPt = 40;
  cfg.selectedLeptons.minMuPt = 30;
*/
  TString treeName = "Events";
  OneLepCRAnalyzer a(fullname, treeName, outfilename, isMC, &pars);

  a.analyze(100000);

  //a.analyze(10000,100000);

}
