#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"
#include "AnalysisMethods/macros/0LepSearchRegions/ZeroLeptonTreeHelper.hh"
#endif

//#define FILL_GEN_INFO

using namespace ucsbsusy;

class PhotonCRAnalyzer : public ZeroLeptonAnalyzer {

  public :

    PhotonCRAnalyzer(TString fileName, TString treeName, TString outfileName, bool isMCTree,cfgSet::ConfigSet *pars) :
      ZeroLeptonAnalyzer(fileName, treeName, outfileName, isMCTree, pars), boson(0), passPhotonSel(false) {}

    GenParticleF*    boson;
    vector<GenJetF*> genJets;
    bool             passPhotonSel;

    void processVariables(){
      BaseTreeAnalyzer::processVariables();

#ifdef FILL_GEN_INFO
      if(isMC()){
        // get boson
        int PID = (process == defaults::SINGLE_G ? ParticleInfo::p_gamma : ParticleInfo::p_Z0);
        for(auto *p : genParts)
          if(p->pdgId() == PID) {
            boson = p;
            break;
          }
        // fill genjets
        genJets.clear();
        for(auto &j : defaultJets->genJets){
          if(cfgSet::isSelGenJet(j,configSet.jets))
            genJets.push_back(&j);
          // cleaning vs boson moved to TreeFiller::fillEventInfo
        }
        // add gen photon to gen met
        if(process == defaults::SINGLE_G)
          genmet->setP4(genmet->p4() + boson->p4());
      }
#endif

      passPhotonSel = true;
      if (process == defaults::SINGLE_Z)
        return;
      if (selectedPhotons.empty()){
        passPhotonSel = false;
        return;
      }

      PhotonF* pho = selectedPhotons.front();
      met->setP4(met->p4() + pho->p4()); // add recoPhoton to met

      // clean vetoedLeptons vs recoPhoton
      double nearDR = 0;
      int near = PhysicsUtilities::findNearestDRDeref(*pho,vetoedLeptons,nearDR,0.4,-1,0,vetoedLeptons.size());
      if(near >= 0){
        vector<LeptonF*> tmpLeptons;
        for (int iL = 0; iL < vetoedLeptons.size(); ++iL) {
          if (iL != near) tmpLeptons.push_back(vetoedLeptons.at(iL));
        }
        vetoedLeptons = tmpLeptons;
        nVetoedLeptons = vetoedLeptons.size();
      }

      // clean vetoedTracks vs recoPhoton
      nearDR = 0;
      near = PhysicsUtilities::findNearestDRDeref(*pho,vetoedTracks,nearDR,0.4,-1,0,vetoedTracks.size());
      if(near >= 0){
        vector<PFCandidateF*> tmpTracks;
        for (int iT = 0; iT < vetoedTracks.size(); ++iT) {
          if (iT != near) tmpTracks.push_back(vetoedTracks.at(iT));
        }
        vetoedTracks = tmpTracks;
        nVetoedTracks = vetoedTracks.size();
      }



    }


    bool fillEvent() {
      if(nVetoedLeptons > 0)                return false;
      if(nVetoedTracks > 0)                 return false;
      if(met->pt() < metcut_)               return false;
      if(!passPhotonSel)                    return false;
      if(!goodvertex)                       return false;
      filler.fillEventInfo(&data, this);
      filler.fillJetInfo  (&data, jets, bJets, met);
#ifdef FILL_GEN_INFO
      filler.fillGenInfo  (&data, boson, genJets);
#endif
      return true;
    }

};

void makeZeroLeptonPhotonCRTrees(TString sname = "gjets_photoncr",
                                 const int fileindex = 0,
                                 const bool isMC = true,
                                 const TString fname = "/store/user/vdutta/13TeV/080615/merged/gjets_ht100to200_ntuple_wgtxsec.root",
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
  cfgSet::ConfigSet pars = pars0LepPhoton(json);
  PhotonCRAnalyzer a(fullname, "Events", outfilename, isMC, &pars);

 // a.analyze(1000,10000);
  a.analyze(100000);

}
