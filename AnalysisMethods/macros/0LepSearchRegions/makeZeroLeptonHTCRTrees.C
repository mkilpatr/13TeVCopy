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

      float htcut = JetKinematics::ht(jets, 20.0, 2.4);

      if(!goodvertex)      return false;
      if(nSelLeptons != 0) return false;
      if(htcut<700)        return false;
      if(nJets < 2)        return false;
      if(met->pt() < 50.)  return false;

      filler.fillEventInfo(&data, this);
      if(isMC()) filler.fillGenInfo  (&data, 0, genJets, false);
      filler.fillJetInfo  (&data, jets, bJets, met);
      //m2Filler.fillM2Info(&data,this,M2TreeFiller::TTBAR_1L);
      //cttFiller.fillTopTagInfo(&data,this,jets);
      //hettFiller.fillHettInfo(&data,this,jets,hettTops);
      return true;
    }


};

void makeZeroLeptonHTCRTrees(TString sname = "ww2l",
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

  cfgSet::ConfigSet pars = pars0lep(json);
  pars = cfgSet::zl_search_set;
  pars.corrections.eventCorrectionFile =  TString::Format("%s/src/data/corrections/eventCorr_allData.root",cfgSet::CMSSW_BASE);
  pars.jets.cleanJetsvLeptons = true;

  OneLepCRAnalyzer a(fullname, "Events", outfilename, fileindex+ 2, isMC, &pars);

  //a.analyze(10000,5000);
  a.analyze(100000);


}
