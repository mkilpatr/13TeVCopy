#if !defined(__CINT__) || defined(__MAKECINT__)
#include "TSystem.h"
#include "AnalysisBase/TreeAnalyzer/interface/TreeCopier.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
#endif

using namespace ucsbsusy;

class TriggerAnalyzer : public TreeCopierManualBranches {

  public :
    TriggerAnalyzer(TString fileName, TString treeName, TString outfileName, bool isMCTree, cfgSet::ConfigSet *pars) :
      TreeCopierManualBranches(fileName, treeName, outfileName, isMCTree, pars) {}

    virtual ~TriggerAnalyzer() {}

    void loadVariables() {
      load(cfgSet::EVTINFO);
      load(cfgSet::AK4JETS);
      load(cfgSet::ELECTRONS);
      load(cfgSet::MUONS);
      load(cfgSet::TRIGOBJS);
      if(isMC()) load(cfgSet::GENPARTICLES);
    }

    void book() {
      i_njets         = data.add<int>  ("","njets","I",0);
      i_met           = data.add<float>("","met","F",0);
      i_passtrig      = data.add<bool> ("","passtrig","O",0);
      i_passauxtrigmu = data.add<bool> ("","passauxtrigmu","O",0);
      i_passauxtrigel = data.add<bool> ("","passauxtrigel","O",0);
      i_jetpt         = data.addMulti<float>("jet","pt",0);
      i_jeteta        = data.addMulti<float>("jet","eta",0);
      i_trigobjpt     = data.addMulti<float>("trigobj","pt",0);
      i_trigobjeta    = data.addMulti<float>("trigobj","eta",0);
      i_trigobjtype   = data.addMulti<int>  ("trigobj","type",0);
      i_trigobjoverlap= data.addMulti<bool> ("trigobj","overlap",0);
    }

    size i_njets;
    size i_met;
    size i_passtrig;
    size i_passauxtrigmu;
    size i_passauxtrigel;
    size i_jetpt;
    size i_jeteta;
    size i_trigobjpt;
    size i_trigobjeta;
    size i_trigobjtype;
    size i_trigobjoverlap;

    bool fillEvent() {

      if(!isMC() && hasJSONFile() && !passesLumiMask()) {
        return false;
      }

      data.fill<int>  (i_njets, nJets);
      data.fill<float>(i_met, met->pt());
      data.fill<bool> (i_passtrig, (triggerflag & kHLT_DiCentralPFJet55_PFMET110_NoiseCleaned));
      data.fill<bool> (i_passauxtrigmu, (triggerflag & kHLT_IsoTkMu24_eta2p1));
      data.fill<bool> (i_passauxtrigel, (triggerflag & kHLT_Ele32_eta2p1_WPLoose_Gsf));

      for(auto* jet : jets) {
        data.fillMulti<float>(i_jetpt, jet->pt());
        data.fillMulti<float>(i_jeteta, jet->eta());
      }

      bool ismutrigobj = false, iseltrigobj = false;
      for(auto* to : triggerObjects) {
        if((to->filterflags() & kSingleIsoTkMu24) && (to->pathflags() & kHLT_IsoTkMu24_eta2p1)) ismutrigobj = true;
        if((to->filterflags() & kSingleEle32) && (to->pathflags() & kHLT_Ele32_eta2p1_WPLoose_Gsf)) iseltrigobj = true;
        if(ismutrigobj || iseltrigobj) {
          data.fillMulti<float>(i_trigobjpt, to->pt());
          data.fillMulti<float>(i_trigobjeta, to->eta());
          data.fillMulti<int>  (i_trigobjtype, ismutrigobj ? 13 : 11);
          double nearDR = 0.0;
          int near = PhysicsUtilities::findNearestDRDeref(*to,jets,nearDR,0.4,10.0,0,jets.size());
          data.fillMulti<bool> (i_trigobjoverlap, near >= 0 ? true : false);
        }
      }

      return true;
    }

};

void studyTrigger(TString sname = "singleel",
                  const int fileindex = -1,
                  const bool isMC = false,
                  const TString fname = "/store/user/vdutta/13TeV/150715/74X/merged/singleel-2015b-pr_ntuple.root",
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
  cfgSet::setJSONFile("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions15/13TeV/Cert_246908-251252_13TeV_PromptReco_Collisions15_JSON.txt");
  cfgSet::ConfigSet pars = cfgSet::zl_search_set;

  TriggerAnalyzer a(fullname, "TestAnalyzer/Events", outfilename, isMC, &pars);

  a.analyze(10000,100000);

}
