#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"
#include "AnalysisMethods/macros/0LepSearchRegions/ZeroLeptonTreeHelper.hh"
#endif

using namespace ucsbsusy;

class ZtoLLCRAnalyzer : public ZeroLeptonAnalyzer {

  public :

    ZtoLLCRAnalyzer(TString fileName, TString treeName, TString outfileName, size randSeed, bool isMCTree, cfgSet::ConfigSet *pars) :
      ZeroLeptonAnalyzer(fileName, treeName, outfileName, randSeed, isMCTree, pars) {}

    bool  passZtoLLSel = false;
    MomentumF metplusdilep;

    void processVariables(){
      ZeroLeptonAnalyzer::processVariables();

      // selections: exactly two same flavor leptons, inv mass in (80, 100)
      passZtoLLSel = false;
      if(nPrimaryLeptons == 0 || nSelLeptons != 2)
        return;

      auto lep0 = selectedLeptons.at(0);
      auto lep1 = selectedLeptons.at(1);

      if (lep0->pt()<20)
        return;

      if (lep0->iselectron() == lep1->iselectron() && lep0->q()*lep1->q() < 0){
        passZtoLLSel = true;
        metplusdilep.setP4(met->p4() + lep0->p4() + lep1->p4());

        // clean vetoedTracks vs selectedLeptons
        vector<bool> isOverlapTrack(vetoedTracks.size(),false);
        vector<PFCandidateF*> tmpTracks;
        for(const auto* lep : selectedLeptons) {
          double nearDR = 0;
          int near = PhysicsUtilities::findNearestDRDeref(*lep,vetoedTracks,nearDR,0.4,-1,0,vetoedTracks.size());
          if(near >= 0){
            isOverlapTrack[near] = true;
          }
        }
        for (unsigned int iT = 0; iT < vetoedTracks.size(); ++iT){
          if (!isOverlapTrack.at(iT))
            tmpTracks.push_back(vetoedTracks.at(iT));
        }
        vetoedTracks = tmpTracks;
        nVetoedTracks = vetoedTracks.size();
      }

    }


    bool fillEvent() {
      // fill inclusive histograms
      extraFiller.fillHistograms(this);

      if(!passZtoLLSel)                     return false;
      if(!goodvertex)                       return false;
      if(nJets < 2)                         return false;
      if(metplusdilep.pt() < 100)           return false;

      filler.fillEventInfo(&data, this, true, &metplusdilep);
      extraFiller.fillTestVars(&data, this);

      return true;
    }


};

void makeZeroLeptonZtoLLCRTrees(TString sname = "dyjetstoll_cr",
                                 const int fileindex = 0,
                                 const bool isMC = true,
                                 const TString fname = "/store/user/vdutta/13TeV/150715/74X/merged/dyjetstoll_1_ntuple_wgtxsec.root",
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

  cfgSet::ConfigSet pars = pars0lepDiLepCR(json);

  ZtoLLCRAnalyzer a(fullname, "Events", outfilename, fileindex+2, isMC, &pars);
  a.analyze(100000);
//  a.analyze(100000, 10000);


}
