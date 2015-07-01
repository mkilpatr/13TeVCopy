#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"
#include "AnalysisMethods/macros/0LepSearchRegions/ZeroLeptonTreeHelper.hh"
#endif

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

      // get boson
      int PID = (process == defaults::SINGLE_G ? ParticleInfo::p_gamma : ParticleInfo::p_Z0);
      if(isMC())
        for(auto &p : genParts)
          if(p->pdgId() == PID) {
            boson = p;
            break;
          }

      // add gen photon to gen met
      if(isMC() && process == defaults::SINGLE_G)
        genmet->setP4(genmet->p4() + boson->p4());



      passPhotonSel = true;
      if(process == defaults::SINGLE_G) {
        if(!selectedPhotons.empty())
          met->setP4(met->p4() + selectedPhotons.front()->p4());
        else passPhotonSel = false;
      }


      // clean jets against boson
      jets.clear(); bJets.clear(); nonBJets.clear();
      for(auto &j : defaultJets->recoJets){

        if(!cfgSet::isSelBJet(j,pars0lep().jets)) continue;
        if(boson && PhysicsUtilities::deltaR2(j, *boson) < 0.16) continue;
        jets.push_back(&j);
        if(!cfgSet::isSelBJet(j,pars0lep().jets,defaults::CSV_MEDIUM)) bJets.push_back(&j);
        else nonBJets.push_back(&j);
      }
      nJets    = jets.size();
      nBJets   = bJets.size();

      // clean genjets
      genJets.clear();
      for(auto &j : defaultJets->genJets){
        if(!cfgSet::isSelGenJet(j,pars0lep().jets)) continue;
        if(boson && PhysicsUtilities::deltaR2(j, *boson) < 0.16) continue;
        genJets.push_back(&j);
      }

      // clean vetoedLeptons
      vector<LeptonF*> tmpLeptons;
      for(auto* l : vetoedLeptons) {
        if(boson && PhysicsUtilities::deltaR2(*l, *boson) < 0.16) continue;
        tmpLeptons.push_back(l);
      }
      vetoedLeptons = tmpLeptons;
      nVetoedLeptons = vetoedLeptons.size();

      // clean vetoedTaus
      vector<PFCandidateF*> tmpTaus;
      for(auto* t : vetoedTracks) {
        if(boson && PhysicsUtilities::deltaR2(*t, *boson) < 0.16) continue;
        tmpTaus.push_back(t);
      }
      vetoedTracks = tmpTaus;
      nVetoedTracks = vetoedTracks.size();
    }

    bool fillEvent() {
      if(nVetoedLeptons > 0)                return false;
      if(nVetoedTracks > 0)                   return false;
      if(met->pt() < metcut_)               return false;
      if(boson && fabs(boson->eta()) > 2.4) return false;
      if(!passPhotonSel)                    return false;

      filler.fillEventInfo(&data, this);
      filler.fillGenInfo(&data, boson, genJets);
      filler.fillJetInfo  (&data, jets, bJets, met);
      return true;
    }

};

void makeZeroLeptonPhotonCRTrees(TString sname = "gjets_photoncr",
                                 const int fileindex = 0,
                                 const bool isMC = true,
                                 const TString fname = "/store/user/vdutta/13TeV/080615/merged/gjets_ht100to200_ntuple_wgtxsec.root",
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
  cfgSet::ConfigSet pars = pars0LepPhoton();
  PhotonCRAnalyzer a(fullname, "TestAnalyzer/Events", outfilename, isMC, &pars);

  a.analyze(100000);

}
