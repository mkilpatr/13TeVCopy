#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/macros/0LepSearchRegions/ZeroLeptonTreeHelper.hh"
#endif

using namespace ucsbsusy;

class OneLepCRAnalyzer : public ZeroLeptonAnalyzer {

  public :

    OneLepCRAnalyzer(TString fileName, TString treeName, TString outfileName, bool isMCTree,cfgSet::ConfigSet *pars) :
      ZeroLeptonAnalyzer(fileName, treeName, outfileName, isMCTree, pars), corrmet(new MomentumF()), passOneLepSel(false) {}

    MomentumF*   corrmet;
    bool         passOneLepSel;
    bool 		 passOneVeto;
    void processVariables() {
      BaseTreeAnalyzer::processVariables();

      // add lepton to corrected met
      //passOneLepSel = selectedLeptons.size() == 1;
      passOneVeto = vetoedLeptons.size() == 1;
      corrmet->setP4(met->p4());
      if(vetoedLeptons.size())
      //  corrmet->setP4(met->p4() + selectedLeptons[0]->p4());
    	  corrmet->setP4(met->p4() + vetoedLeptons[0]->p4());

    }

    bool fillEvent() {
      if(met->pt() < metcut_) return false;
      if(!goodvertex) return false;
     // if(!passOneLepSel)      return false;
      if(!passOneVeto) return false;
     // if(fabs(PhysicsUtilities::deltaPhi(*corrmet, *selectedLeptons[0])) > 1)
      if(fabs(PhysicsUtilities::deltaPhi(*met, *vetoedLeptons[0])) > 1)
        return false;

      filler.fillEventInfo(&data, this);
      filler.fillJetInfo  (&data, jets, bJets, met);
      return true;
    }

};

void makeZeroLeptonOneLepCRTrees(TString sname = "ttbar_onelepcr",
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


  cfgSet::ConfigSet pars = cfgSet::zl_search_set;
	pars.vetoedLeptons.selectedMuon = (&MuonF::ismultiisovetomuonl);
	pars.vetoedLeptons.selectedElectron = (&ElectronF::ismultiisovetoelectronl);
//  pars1lepcr->minSelMuPt = 5;
 // pars1lepcr->minSelEPt  = 5;
 // pars1lepcr->cleanJetsvSelectedLeptons_ = true;

  OneLepCRAnalyzer a(fullname, "Events", outfilename, isMC, &pars);

  a.analyze(100000);

}
