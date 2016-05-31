
#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"
#include "AnalysisMethods/macros/0LFBDSearchRegions/T2fbdHelper.hh"
#endif

using namespace ucsbsusy;

class ZtoLLCRAnalyzer : public ZeroLeptonAnalyzer {

  public :

    ZtoLLCRAnalyzer(TString fileName, TString treeName, TString outfileName, size randSeed, bool isMCTree, cfgSet::ConfigSet *pars) :
      ZeroLeptonAnalyzer(fileName, treeName, outfileName, randSeed, isMCTree, pars) {}

    // booking
    size i_origmet = 0;
    size i_dilepmass = 0;
    size i_iselectron = 0;
    size i_passTrig = 0;

    bool  passTrig = false;
    bool  passZtoLLSel = false;
    bool  iselectron = false;
    float origMET = 0;
    float dilepmass = 0;

    void book() {
      ZeroLeptonAnalyzer::book();

      i_origmet           = data.add<float>("","origmet","F",0);
      i_dilepmass         = data.add<float>("","dilepmass","F",0);
      i_iselectron        = data.add<bool>("", "iselectron", "O", 0);
      i_passTrig          = data.add<bool>("", "passTrig", "O", 0);
    }

    void processVariables(){
      ZeroLeptonAnalyzer::processVariables();
      origMET = met->pt();
      dilepmass = 0;

      // selections: exactly two same flavor leptons, inv mass in (80, 100)
      passZtoLLSel = false;
      if(nPrimaryLeptons == 0 || nSelLeptons != 2)
        return;

      auto lep0 = selectedLeptons.at(0);
      auto lep1 = selectedLeptons.at(1);

      if (lep0->pt()<20)
        return;

      if (lep0->iselectron() == lep1->iselectron() && lep0->q()*lep1->q() < 0){
        auto z_p4 = lep0->p4() + lep1->p4();
        passZtoLLSel = true;
        dilepmass = z_p4.mass();
        met->setP4(met->p4() + z_p4);
        metNoHF->setP4(metNoHF->p4() + z_p4);

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

      iselectron = lep0->iselectron();

      // avoid picking up same event twice
      if (process==defaults::DATA_DOUBLEEG){
        passTrig = triggerflag & kHLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ;
      }else if (process==defaults::DATA_DOUBLEMU){
        passTrig = (triggerflag & kHLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ || triggerflag & kHLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ)
            && (!(triggerflag & kHLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ));
      }else if (isMC()){
        passTrig = iselectron ? (triggerflag & kHLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ) :
            (triggerflag & kHLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ || triggerflag & kHLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ);
      }


    }


    bool fillEvent() {
      if(!passZtoLLSel)                     return false;
      if(!goodvertex)                       return false;
      if(nJets<2)                           return false;
      filler.fillEventInfo(&data, this);
      filler.fillJetInfo  (&data, jets, bJets, met);

      data.fill<float>(i_origmet, origMET);
      data.fill<float>(i_dilepmass, dilepmass);
      data.fill<bool>(i_iselectron, iselectron);
      data.fill<bool>(i_passTrig, passTrig);
      return true;
    }


};

void makeZeroLeptonZtoLLCRTreesFBD(TString sname = "dyjetstoll_cr",
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
