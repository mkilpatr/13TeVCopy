#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"
#include "AnalysisMethods/macros/0LFBDSearchRegions/T2fbdHelper.hh"
#endif

// config to use: runPhotonCR.conf
// Note that QCD sample need to be processed twice, one named as "qcd-frag", and the other named as "qcd-fake"

using namespace ucsbsusy;

class PhotonCRAnalyzer : public ZeroLeptonAnalyzer {

  public :

    PhotonCRAnalyzer(TString fileName, TString treeName, TString outfileName, size randSeed, bool isMCTree,cfgSet::ConfigSet *pars) :
      ZeroLeptonAnalyzer(fileName, treeName, outfileName, randSeed, isMCTree, pars) {}

    const double DR_CUT = 0.4;

    vector<GenJetF*> genJets;
    PhotonF*         pho            = nullptr;
    GenParticleF*    boson          = nullptr;
    bool             passPhotonSel  = true;
    bool             passDRSel      = true;
    bool             passGenMatch   = true;
    bool             flagQCDFake    = false;
    bool             flagBypassDRSel= false;
    float            origMET        = 0;
    float            origMETNoHF    = 0;

    size i_origmet = 0;
    size i_passtrigphoton165 = 0;
    size i_passgenmatch = 0;

    void book() {
      ZeroLeptonAnalyzer::book();

      i_origmet           = data.add<float>("","origmet","F",0);
      i_passtrigphoton165 = data.add<bool>("","passtrigphoton165", "O",0);
      i_passgenmatch      = data.add<bool>("","passgenmatch","O",0);
    }

    void processVariables(){
      BaseTreeAnalyzer::processVariables();
      origMET = met->pt();
      origMETNoHF = metNoHF->pt();

      // reset to nullptr
      pho = nullptr;
      boson = nullptr;

      // set these flags to be true (for data and znunu)
      passPhotonSel = true;
      passDRSel = true;
      passGenMatch = true;

      if (process == defaults::SINGLE_Z){
        for(auto *p : genParts)
          if(p->pdgId() == ParticleInfo::p_Z0) {
            boson = p;
            break; // end of znunu
          }
        return;
      }

      if (selectedPhotons.empty()){
        passPhotonSel = false;
        return;
      }
      pho = selectedPhotons.front();
      // add photon to met
      met->setP4(met->p4() + pho->p4());
      metNoHF->setP4(metNoHF->p4() + pho->p4());

      // clean leptons vs recoPhoton
      vector<LeptonF*> tmpLeptons;
      for (auto *lep : selectedLeptons){
        if (PhysicsUtilities::deltaR2(*pho, *lep)>0.16) tmpLeptons.push_back(lep);
      }
      selectedLeptons = tmpLeptons;
      nSelLeptons = selectedLeptons.size();

      // clean vetoedTracks vs recoPhoton
      vector<PFCandidateF*> tmpTracks;
      for (auto *trk : vetoedTracks){
        if (PhysicsUtilities::deltaR2(*pho, *trk)>0.16) tmpTracks.push_back(trk);
      }
      vetoedTracks = tmpTracks;
      nVetoedTracks = vetoedTracks.size();

      if (!isMC()) return; // end of data

      // fill genjets
      genJets.clear();
      for(auto &j : defaultJets->genJets){
        if(cfgSet::isSelGenJet(j,configSet.jets))
          genJets.push_back(&j);
        // cleaning vs boson moved to TreeFiller::fillEventInfo
      }

      passDRSel = false;
      passGenMatch = false;

      vector<GenParticleF*> partons;
      vector<GenParticleF*> genphotons;

      for (auto *p : genParts){
        if (ParticleInfo::isDocOutgoing(p->status()) && ParticleInfo::isQuarkOrGluon(p->pdgId()))
          partons.push_back(p);
        else if (ParticleInfo::isFinal(p->status()) && p->pdgId() == ParticleInfo::p_gamma){
          bool isPrompt = false;
          for (int i=0; i<p->numberOfMothers(); ++i){
            auto mother = p->mother(i);
            if (ParticleInfo::isQuark(mother->pdgId()) || mother->pdgId() == ParticleInfo::p_gamma || mother->pdgId() == ParticleInfo::p_proton){
              isPrompt = true;
              break;
            }
          }
          if (isPrompt) genphotons.push_back(p);
        }
      }

      if (genphotons.empty()){
        passDRSel = flagQCDFake;
        return;
      }

      std::sort(genphotons.begin(), genphotons.end(), PhysicsUtilities::greaterPTDeref<GenParticleF>());
      for (auto *gp : genphotons){
        if (PhysicsUtilities::deltaR2(*gp, *pho) < 0.01 && pho->pt()>0.5*gp->pt() && pho->pt()<2*gp->pt()){
          passGenMatch = true;
          boson = gp;
          genmet->setP4(genmet->p4() + boson->p4());
          break;
        }
      }

      if (passGenMatch){
        double minDR = 0;
        PhysicsUtilities::findNearestDRDeref(*boson, partons, minDR);
        if ( (minDR > DR_CUT && process == defaults::SINGLE_G) || (minDR <= DR_CUT && process == defaults::QCD) )
          passDRSel = !flagQCDFake;
      } else
        passDRSel = flagQCDFake;

      // bypass DR selection for processes other than GJets and QCD
//      if (process!=defaults::SINGLE_G && process!=defaults::QCD) //FIXME: TTG is saved as SINGLE_G
      if (flagBypassDRSel)
        passDRSel = true;

    }



    bool fillEvent() {
//      if(nVetoedLeptons > 0)                return false;
//      if(nVetoedTracks > 0)                 return false;
//      if(met->pt() < metcut_)               return false;
      if(!passPhotonSel)                    return false;
      if(!passDRSel)                        return false;
      if(!goodvertex)                       return false;
      if(nJets<2)                           return false;
     filler.fillEventInfo(&data, this);
      filler.fillJetInfo  (&data, jets, bJets, met);
//      if (isMC()) filler.fillGenInfo  (&data, boson, genJets);

      data.fill<float>(i_origmet, origMET);
      data.fill<bool >(i_passtrigphoton165, triggerflag & kHLT_Photon165_HE10);
      data.fill<bool>(i_passgenmatch, passGenMatch);
      return true;
    }

};

void makeZeroLeptonPhotonCRTreesFBD(TString sname = "gjets_photoncr",
                                 const int fileindex = 0,
                                 const bool isMC = true,
                                 const TString fname = "/store/user/vdutta/13TeV/080615/merged/gjets_ht100to200_ntuple_wgtxsec.root",
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
  cfgSet::ConfigSet pars = pars0LepPhoton(json);


  PhotonCRAnalyzer a(fullname, "Events", outfilename, fileindex+2,isMC, &pars);
  a.flagQCDFake = sname.Contains("fake");
  a.flagBypassDRSel = !(sname.Contains("qcd")||sname.Contains("gjets"));
  a.analyze(100000);
//   a.analyze(1000,10000);

}
