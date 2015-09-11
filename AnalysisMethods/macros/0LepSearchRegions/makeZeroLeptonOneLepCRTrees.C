#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/macros/0LepSearchRegions/ZeroLeptonTreeHelper.hh"
#endif

using namespace ucsbsusy;
TRandom3 rnd;

class OneLepCRAnalyzer : public ZeroLeptonAnalyzer {

  public :

    OneLepCRAnalyzer(TString fileName, TString treeName, TString outfileName, bool isMCTree,cfgSet::ConfigSet *pars, double randSeed) :
      ZeroLeptonAnalyzer(fileName, treeName, outfileName, isMCTree, pars) {rnd.SetSeed(randSeed);}

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
      float maxLep = nSelLeptons;
      int whichLep = rnd.Uniform(0.,nSelLeptons);
      MomentumF* lep = new MomentumF(selectedLeptons.at(whichLep)->p4());
      MomentumF* W = new MomentumF(lep->p4() + met->p4());

      //      if(fabs(PhysicsUtilities::deltaPhi(*W, *lep)) > 1)        return false;

      filler.fillEventInfo(&data, this, whichLep);

      filler.fillJetInfo(&data, jets, bJets, met);

      return true;
    }

};

void makeZeroLeptonOneLepCRTrees(TString sname = "ttbar_onelepcr",
                                 const int fileindex = 0,
                                 const bool isMC = true,
                                 const TString fname = "/store/user/gouskos/13TeV/Spring15/20150813/ttbar-madgraphmlm-50ns_1_ntuple_postproc.root",
                                 const double xsec = 1.0,
                                 const TString outputdir = "trees",
                                 const TString fileprefix = "root://eoscms//eos/cms",
                                 const TString json="")
{

  printf("Processing file %d of %s sample\n", (fileindex > -1 ? fileindex : 0), sname.Data());

  if(fileindex > -1)
    sname += TString::Format("_%d",fileindex);

  if(isMC)
    printf("Cross section: %5.2f pb\n", xsec);

  TString fullname = fileprefix+fname;

  gSystem->mkdir(outputdir,true);
  TString outfilename = outputdir+"/"+sname+"_tree.root";

  cfgSet::ConfigSet pars = pars0lepCR(json);

  double randSeed = fileindex + 2;
  TString treename = "Events";
  OneLepCRAnalyzer a(fullname, treename, outfilename, isMC, &pars, randSeed);

  a.analyze(1000,10000);

  //a.analyze(10000,100000);

}
