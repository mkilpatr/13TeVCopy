#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"
#include "AnalysisMethods/macros/0LepSearchRegions/ZeroLeptonTreeHelper.hh"
#endif

using namespace ucsbsusy;

class OneLepCRAnalyzer : public ZeroLeptonAnalyzer {

  public :

    OneLepCRAnalyzer(TString fileName, TString treeName, TString outfileName, bool isMCTree, cfgSet::ConfigSet *pars) :
      ZeroLeptonAnalyzer(fileName, treeName, outfileName, isMCTree, pars), passOneLepSel(false) {}

    vector<GenJetF*> genJets;

    bool passOneLepSel;

    void processVariables(){
      BaseTreeAnalyzer::processVariables();

      genJets.clear();
      if(isMC()) {
        for(auto &j : defaultJets->genJets){
          genJets.push_back(&j);
        }
      }

    }


    bool fillEvent() {
      if(nSelLeptons != 1)                  return false;
      if(!goodvertex)                       return false;
      filler.fillEventInfo(&data, this);
      if(isMC()) filler.fillGenInfo  (&data, 0, genJets, false);
      filler.fillJetInfo  (&data, jets, bJets, met);
      return true;
    }


};

void makeZeroLeptonOneLepInclCRTrees(TString sname = "ww2l",
                                     const int fileindex = -1,
                                     const bool isMC = false,
                                     const TString fname = "/store/user/hqu/13TeV/290915/merged/ww-2l2nu-powheg_1_ntuple_postproc.root",
                                     const double xsec = 1.0,
                                     const TString outputdir = "trees_1linclcr_100615",
                                     const TString fileprefix = "root://cmseos:1094/",
                                     const TString json = TString::Format("%s/src/data/JSON/Cert_246908-257599_13TeV_PromptReco_Collisions15_25ns_JSON.txt",getenv("CMSSW_BASE")))
{

  printf("Processing file %d of %s sample\n", (fileindex > -1 ? fileindex : 0), sname.Data());

  if(fileindex > -1)
    sname += TString::Format("_%d",fileindex);

  if(isMC)
    printf("Cross section: %5.2f pb\n", xsec);

  TString fullname = fileprefix+fname;
  gSystem->mkdir(outputdir,true);
  TString outfilename = outputdir+"/"+sname+"_tree.root";

  cfgSet::ConfigSet pars = pars0lep(json);
  pars = cfgSet::zl_search_set;
  pars.corrections.eventCorrectionFile =  TString::Format("%s/src/data/corrections/eventCorr_allData.root",cfgSet::CMSSW_BASE);
  pars.jets.cleanJetsvSelectedLeptons = true;
  pars.selectedLeptons = cfgSet::ol_sel_leptons;
  pars.selectedLeptons.minEPt = 30.0;
  pars.selectedLeptons.maxEEta = 2.1;
  pars.selectedLeptons.maxED0 = 0.02;
  pars.selectedLeptons.maxEDz = 0.1;
  pars.selectedLeptons.minMuPt = 30.0;
  pars.selectedLeptons.maxMuEta = 2.4;
  pars.vetoedLeptons = cfgSet::ol_veto_leptons;

  OneLepCRAnalyzer a(fullname, "Events", outfilename, isMC, &pars);

  a.analyze(100000);


}
