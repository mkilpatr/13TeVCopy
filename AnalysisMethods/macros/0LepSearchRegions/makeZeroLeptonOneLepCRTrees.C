#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/macros/0LepSearchRegions/ZeroLeptonTreeHelper.hh"
#endif

using namespace ucsbsusy;
TRandom3 rnd;

class OneLepCRAnalyzer : public ZeroLeptonAnalyzer {

  public :

    OneLepCRAnalyzer(TString fileName, TString treeName, TString outfileName, bool isMCTree,cfgSet::ConfigSet *pars, DataType type, double randSeed) :
      ZeroLeptonAnalyzer(fileName, treeName, outfileName, isMCTree, pars), datatype_(type) {rnd.SetSeed(randSeed);}// TRandom3 rnd(0);}

    DataType   datatype_;

    bool fillEvent() { 
      if(met->pt() < metcut_) return false;
      if(!goodvertex) return false;
      if(nBJets < 1) return false;
      if(nJets < 5) return false;
      if(nSelLeptons<1)      return false;
      //if(nVetoedTracks > 0)     return false;
      float maxLep = nSelLeptons;
      int whichLep = rnd.Uniform(0.,nSelLeptons);
      MomentumF* lep = new MomentumF(selectedLeptons.at(whichLep)->p4());
      MomentumF* W = new MomentumF(lep->p4() + met->p4());

      if(fabs(PhysicsUtilities::deltaPhi(*W, *lep)) > 1)        return false;
      filler.fillEventInfo(&data, this, datatype_, whichLep);
      filler.fillJetInfo(&data, jets, bJets, met);

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

  cfgSet::ConfigSet pars = pars0lepCR();

  double randSeed = fileindex + 2;
  TString treename = isMC ? "Events" : "TestAnalyzer/Events";
  DataType type = isMC ? MC : (fname.Contains("htmht") ? HTMHT : (fname.Contains("singlemu") ? SINGLEMU : (fname.Contains("singleel") ? SINGLEEL : MC)));
  OneLepCRAnalyzer a(fullname, treename, outfilename, isMC, &pars, type, randSeed);

  a.analyze(10000);

  //a.analyze(10000,100000);

}
