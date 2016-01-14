#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"
#include "AnalysisMethods/macros/0LepSearchRegions/ZeroLeptonTreeHelper.hh"
#endif

using namespace ucsbsusy;

class OneLepCRAnalyzer : public ZeroLeptonAnalyzer {

  public :

    OneLepCRAnalyzer(TString fileName, TString treeName, TString outfileName, size randSeed, bool isMCTree, cfgSet::ConfigSet *pars) :
      ZeroLeptonAnalyzer(fileName, treeName, outfileName, randSeed,isMCTree, pars), passOneLepSel(false) {}

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
      if(nPrimaryLeptons != 1)                  return false;
      if(nSecondaryLeptons != 0)                  return false;
      if(!goodvertex)                       return false;
      filler.fillEventInfo(&data, this);
      if(isMC()) filler.fillGenInfo  (&data, 0, genJets, false);
      filler.fillJetInfo  (&data, jets, bJets, met);
      filler.fillTopTagInfo(&data,this, jets);
      return true;
    }


};

void makeZeroLeptonOneLepInclCRTrees(TString sname = "ww2l",
                                     const int fileindex = -1,
                                     const bool isMC = false,
                                     const TString fname = "/store/user/hqu/13TeV/290915/merged/ww-2l2nu-powheg_1_ntuple_postproc.root",
                                     const TString outputdir = "trees_1linclcr_100615",
                                     const TString fileprefix = "root://cmseos:1094/",
                                     const TString json = TString::Format("%s/src/data/JSON/Cert_246908-257599_13TeV_PromptReco_Collisions15_25ns_JSON.txt",getenv("CMSSW_BASE")))
{

  printf("Processing file %d of %s sample\n", (fileindex > -1 ? fileindex : 0), sname.Data());

  if(fileindex > -1)
    sname += TString::Format("_%d",fileindex);

  TString fullname = fileprefix+fname;
  gSystem->mkdir(outputdir,true);
  TString outfilename = outputdir+"/"+sname+"_tree.root";

  cfgSet::ConfigSet pars = pars0lepCR(json);

  pars.jets.cleanJetsvSelectedLeptons = true;

  OneLepCRAnalyzer a(fullname, "Events", outfilename, fileindex+ 2, isMC, &pars);

  a.analyze(100000);


}
