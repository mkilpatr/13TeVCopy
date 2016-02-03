#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/macros/0LepSearchRegions/ZeroLeptonTreeHelper.hh"
#endif

using namespace ucsbsusy;

class OneLepCRAnalyzer : public ZeroLeptonAnalyzer {

  public :

    OneLepCRAnalyzer(TString fileName, TString treeName, TString outfileName, size randSeed, bool isMCTree,cfgSet::ConfigSet *pars) :
      ZeroLeptonAnalyzer(fileName, treeName, outfileName, randSeed, isMCTree, pars) {}

    bool fillEvent() { 
      if(met->pt() < metcut_) return false;

      if(!goodvertex) return false;

      if(nBJets < 0) return false;

      if(nJets < 2) return false;

      // skip events in PR with run number < 251584 - they are in the re-miniAOD
      bool isData = false;
      if (process > defaults::SIGNAL && process < defaults::NUMPROCESSES)
	{ isData = true; }

      bool isPR = false;
      if (datareco==defaults::PROMPT_50NS) { isPR = true; }

      bool skipPRevent = false;
      if ( (isData) && (isPR) && (run<251584) ) { skipPRevent = true; }
      if (skipPRevent) { return false; }


      if(nSelLeptons<1)      return false;
      //if(nVetoedTracks > 0)     return false;

      filler.fillEventInfo(&data, this);

      filler.fillJetInfo(&data, jets, bJets, met);

      return true;
    }

};

void makeZeroLeptonOneLepCRTrees(TString sname = "ttbar_onelepcr",
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

  cfgSet::ConfigSet pars = pars0lep(json);

  OneLepCRAnalyzer a(fullname, "Events", outfilename, fileindex + 2, isMC, &pars);

  a.analyze(1000000);

  //a.analyze(10000,100000);

}
