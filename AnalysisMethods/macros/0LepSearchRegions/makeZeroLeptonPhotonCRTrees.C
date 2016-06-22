#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"
#include "AnalysisMethods/macros/0LepSearchRegions/ZeroLeptonTreeHelper.hh"
#endif

// config to use: runPhotonCR.conf
// Note that QCD sample need to be processed twice, one named as "qcd-frag", and the other named as "qcd-fake"

using namespace ucsbsusy;

class PhotonCRAnalyzer : public ZeroLeptonAnalyzer {

  public :

    PhotonCRAnalyzer(TString fileName, TString treeName, TString outfileName, size randSeed, bool isMCTree,cfgSet::ConfigSet *pars) :
      ZeroLeptonAnalyzer(fileName, treeName, outfileName, randSeed, isMCTree, pars) {}

    const double DR_CUT = 0.4;

    PhotonF*         pho            = nullptr;
    bool             passDRSel      = true;
    bool             passGenMatch   = true;
    bool             flagQCDFake    = false;
    bool             flagBypassDRSel= false;
    float            drphotonparton = -1;

    size i_npho = 0;
    size i_phopt = 0;
    size i_phoeta = 0;
    size i_passgenmatch = 0;
    size i_drphotonparton = 0;

    void book() {
      ZeroLeptonAnalyzer::book();

      i_npho              = data.add<int>("","npho","I",0);
      i_phopt             = data.add<float>("","phopt","F",0);
      i_phoeta            = data.add<float>("","phoeta","F",0);
      i_passgenmatch      = data.add<bool>("","passgenmatch","O",0);
      i_drphotonparton    = data.add<float>("","drphotonparton","F",-1);
    }

    void processVariables(){
      BaseTreeAnalyzer::processVariables();

      // reset to nullptr
      pho = nullptr;
      GenParticleF* genphoton = nullptr;

      // set these flags to be true (for data and znunu)
      passDRSel = true;
      passGenMatch = true;
      drphotonparton = -1;

      if (selectedPhotons.empty()) return;
      pho = selectedPhotons.front();

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
          genphoton = gp;
          genmet->setP4(genmet->p4() + genphoton->p4());
          break;
        }
      }

      if (passGenMatch){
        double minDR = 0;
        PhysicsUtilities::findNearestDRDeref(*genphoton, partons, minDR);
        drphotonparton = minDR;
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
      if(!pho)                              return false;
      if(!passDRSel)                        return false;
      if(!goodvertex)                       return false;

      MomentumF metpluspho;
      metpluspho.setP4(met->p4() + pho->p4());

      filler.fillEventInfo(&data, this, true, &metpluspho);

      data.fill<int>(i_npho, selectedPhotons.size());
      data.fill<float>(i_phopt, pho->pt());
      data.fill<float>(i_phoeta, pho->eta());
      data.fill<bool>(i_passgenmatch, passGenMatch);
      data.fill<float>(i_drphotonparton, drphotonparton);
      return true;
    }

};

void makeZeroLeptonPhotonCRTrees(TString sname = "gjets_photoncr",
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
