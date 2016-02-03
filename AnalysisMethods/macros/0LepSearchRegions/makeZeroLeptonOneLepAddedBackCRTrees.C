#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/macros/0LepSearchRegions/ZeroLeptonTreeHelper.hh"
#endif

using namespace ucsbsusy;

class OneLepCRAnalyzer : public ZeroLeptonAnalyzer {

  public :

    OneLepCRAnalyzer(TString fileName, TString treeName, TString outfileName, size randomSeed, bool isMCTree,cfgSet::ConfigSet *pars) :
      ZeroLeptonAnalyzer(fileName, treeName, outfileName,randomSeed, isMCTree, pars) {}

    bool fillEvent() {
      if(nPrimaryLeptons!=1)      return false;
      if(nSecondaryLeptons !=0 )      return false;
      MomentumF* lep = new MomentumF(selectedLeptons.at(0)->p4());
      MomentumF* metn = new MomentumF(met->p4() + lep->p4());
      if(metn->pt() < metcut_) return false;
      if(!goodvertex) return false;
   // if(nVetoedTracks > 0)     return false;
      if(nJets < 2) return false;
      if(nBJets < 0) return false;
      //      if(fabs(PhysicsUtilities::deltaPhi(*metn, *selectedLeptons[0])) > 1)        return false;

      filler.fillEventInfo(&data, this, true, metn);
      filler.fillJetInfo(&data, jets, bJets, metn);
      return true;
    }

};

void makeZeroLeptonOneLepAddedBackCRTrees(TString sname = "ttbar_onelepcr",
                                 const int fileindex = 0,
                                 const bool isMC = true,
                                 const TString fname = "/store/user/gouskos/13TeV/Spring15/20150813/ttbar-madgraphmlm-50ns_1_ntuple_postproc.root",
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

  cfgSet::ConfigSet pars = pars0lepCR(json);

/*  cfgSet::loadDefaultConfigurations();
  cfgSet::ConfigSet cfg = cfgSet::zl_lepton_set;
  cfg.jets.cleanJetsvSelectedLeptons = true;
  cfg.selectedLeptons.minEPt = 40;
  cfg.selectedLeptons.minMuPt = 30;
*/
  TString treeName = "Events";
  OneLepCRAnalyzer a(fullname, treeName, outfilename, fileindex+2, isMC, &pars);

  a.analyze(100000);

  //a.analyze(10000,100000);

}
