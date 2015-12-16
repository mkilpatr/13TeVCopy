#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"
#include "AnalysisMethods/macros/0LepSearchRegions/ZeroLeptonTreeHelper.hh"
#endif

using namespace ucsbsusy;

class TTbarCRAnalyzer : public ZeroLeptonAnalyzer {

  public :

    TTbarCRAnalyzer(TString fileName, TString treeName, TString outfileName, size randSeed, bool isMCTree, cfgSet::ConfigSet *pars) :
      ZeroLeptonAnalyzer(fileName, treeName, outfileName, randSeed, isMCTree, pars), passTTbarSel(false) {}

    vector<GenJetF*> genJets;

    bool passTTbarSel;

    void processVariables(){
      BaseTreeAnalyzer::processVariables();

      genJets.clear();
      if(isMC()) {
        for(auto &j : defaultJets->genJets){
          double nearDR = 0;
          int near = PhysicsUtilities::findNearestDRDeref(j,selectedLeptons,nearDR,0.4,-1,0,selectedLeptons.size());
          if(near < 0) genJets.push_back(&j);
        }
      }

      // selections: exactly two opposite flavor leptons, inv mass > 10
      passTTbarSel = false;
      if(selectedLeptons.size() == 2){
        auto lep0 = selectedLeptons.at(0);
        auto lep1 = selectedLeptons.at(1);
        if (lep0->q() != lep1->q()){
          auto ll_p4 = lep0->p4() + lep1->p4();
          if (ll_p4.mass()>10){
            passTTbarSel = true;

            // clean vetoedTaus vs selectedLeptons
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
      }

    }


    bool fillEvent() {
      if(!passTTbarSel)                     return false;
      if(!goodvertex)                       return false;
      filler.fillEventInfo(&data, this);
      if(isMC()) filler.fillGenInfo  (&data, 0, genJets, false);
      filler.fillJetInfo  (&data, jets, bJets, met);
      return true;
    }


};

void makeZeroLeptonTTbarCRTrees(TString sname = "doubleeg-2015b-reminiaod",
                                const int fileindex = -1,
                                const bool isMC = false,
                                const TString fname = "/store/user/vdutta/13TeV/130815/merged/doubleeg-2015b-17jul15_ntuple_postproc.root",
                                const TString outputdir = "trees_090615_ttbarcr/norm",
                                const TString fileprefix = "root://cmseos:1094//eos/uscms",
                                const TString json = TString::Format("%s/src/data/JSON/Cert_246908-255031_13TeV_PromptReco_Collisions15_50ns_JSON.txt",getenv("CMSSW_BASE")))
{

  printf("Processing file %d of %s sample\n", (fileindex > -1 ? fileindex : 0), sname.Data());

  if(fileindex > -1)
    sname += TString::Format("_%d",fileindex);

  TString fullname = fileprefix+fname;
  gSystem->mkdir(outputdir,true);
  TString outfilename = outputdir+"/"+sname+"_tree.root";

  cfgSet::ConfigSet pars = pars0lep(json);
  pars = cfgSet::zl_search_set;
  pars.jets.cleanJetsvSelectedLeptons = true;
  pars.selectedLeptons = cfgSet::ol_sel_leptons;
  pars.corrections.tnpLepSel = TnPCorr::GOODPOG;
  pars.selectedLeptons.minEPt = 15.0;
  pars.selectedLeptons.maxEEta = 2.5;
  pars.selectedLeptons.minMuPt = 10.0;
  pars.selectedLeptons.maxEEta = 2.4;

  TTbarCRAnalyzer a(fullname, "Events", outfilename, fileindex+2, isMC, &pars);
  a.analyze(10000);

}
