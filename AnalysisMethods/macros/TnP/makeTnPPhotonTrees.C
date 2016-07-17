
bool dbg = false;
/*

current singlelep triggers
bool passTrigEl = ana->triggerflag & kHLT_Ele27_eta2p1_WPLoose_Gsf;
bool passTrigMu = (ana->triggerflag & kHLT_IsoMu22) || (ana->triggerflag & kHLT_IsoTkMu22);

current photon triggers
data->fill<bool>(i_passtrigphoton165, ana->isMC() || (ana->triggerflag & kHLT_Photon165_HE10));

*/

#if !defined(__CINT__) || defined(__MAKECINT__)
#include "TSystem.h"
#include "AnalysisBase/TreeAnalyzer/interface/BaseTreeAnalyzer.h"
#include "AnalysisBase/TreeAnalyzer/interface/DefaultProcessing.h"
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
#include "AnalysisMethods/macros/0LepSearchRegions/ZeroLeptonTreeHelper.hh" // to ensure lep configs up-to-date
#endif

// class to create a tree with the information needed to run the Tag&Probe efficiency calculations
// we apply truth matching for lepton candidates in MC, and trigger matching for the tag
// deltaR values for the matching are configurable
// mass window will have to be adjusted if we fit non-Z resonances

using namespace ucsbsusy;
//using namespace std; // remove for committing?

class TnPAnalyzer : public TreeCopierManualBranches {

  public :
      TnPAnalyzer(TString fileName, TString treeName, TString outfileName, size randomSeed, bool isMCTree, cfgSet::ConfigSet *pars) :
        TreeCopierManualBranches(fileName, treeName, outfileName, randomSeed, isMCTree, pars) {}

    virtual ~TnPAnalyzer() {}
    //event vars
    size i_run       ;
    size i_lumi      ;
    size i_event     ;
    size i_process   ;
    size i_weight    ;
    size i_truePUWeight ;
    size i_trigEleWeight;
    size i_lepselweight ;
    size i_leptnpweight ;
    // trigger and filters
    size i_ismc      ;
    size i_passjson   ;
    size i_passtrige ;
    //size i_passtrigmu;
    size i_passmetfilters;
    // tnp vars
    size i_mass;
    size i_pt;
    size i_eta;
    size i_abseta;
    size i_htalong;
    size i_phi;
    size i_pass;
/*
    size i_diMass      ;
    size i_probePt        ;
    size i_probeEta       ;
    size i_probeHtAlong   ;
    //size i_probeAnnulus   ;
    size i_probePhi       ;
    //size i_probeCharge    ;
    size i_probePass;
    //size i_probePassIdIso      ;
    //size i_probePassIdNoIso    ;
*/
    size i_tagMatchesTrigObj  ;
    size i_nProbesMatchedToTag  ;
    size i_nTags;


    const bool   applyGenMatch_   = true;    // require truth matching for MC tags/probes
    const double maxGenMatchDR_   = 0.3;     // DR for gen matching
    const double maxTrigMatchDR_  = 0.1;     // DR for matching tag to trigger object
    const double minMass_         = 60.0;    // min for mass window
    const double maxMass_         = 120.0;   // max for mass window
    bool  applyPixelSeedVeto      = true;    // default in DefaultConfigurations is true
    bool  applyCHFFilter          = false;
    int   lepIdChoice             = 11; // never change
    bool  useHardestTag           = false;
    bool  useHardestProbe         = false;
    bool  useHardestMatchedProbe  = false;
    LeptonF* hardestTag = 0;
    PhotonF* hardestProbe = 0;

    vector<const GenParticleF*> genLeptons_; // hold gen leptons for matching
    vector<LeptonF*>            tnpLeptons_; // hold tag lepton candidates
    vector<PhotonF*>            tnpPhotons_; // hold probe photon candidates

    cfgSet::ConfigSet LepConfig_;   // tag lep config
    cfgSet::PhotonConfig PhoConfig_;   // probe photon config, for id & iso

    void book() {
      // event vars
      i_run            = data.add<unsigned int>("","run","i",0);
      i_lumi           = data.add<unsigned int>("","lumi","i",0);
      i_event          = data.add<unsigned int>("","event","i",0);
      i_process        = data.add<unsigned int>("","process","i",0);
      i_ismc           = data.add<bool >("","ismc","O",0);
      i_weight         = data.add<float>("","weight","F",0);
      i_truePUWeight   = data.add<float>("","truePUWeight","F",0);
      i_trigEleWeight  = data.add<float>("","trigEleWeight","F",0);
      i_lepselweight   = data.add<float>("","lepselweight","F",0);
      i_leptnpweight   = data.add<float>("","leptnpweight","F",0);
      // Trigger and filters
      i_passjson       = data.add<bool>("","passjson","O",0);
      i_passtrige      = data.add<bool>("","passtrige","O",0);
      //i_passtrigmu     = data.add<bool>("","passtrigmu","O",0);
      i_passmetfilters = data.add<bool>("","passmetfilters","O",0);
      // tnp vars
      i_mass          = data.add<float>("","mass","F",0);
      i_pt            = data.add<float>("","pt","F",0);
      i_eta           = data.add<float>("","eta","F",0);
      i_abseta           = data.add<float>("","abseta","F",0);
      i_htalong       = data.add<float>("","htalong","F",0);
      i_phi           = data.add<float>("","phi","F",0);
      i_pass          = data.add<bool>("","pass","O",false);
      i_tagMatchesTrigObj       = data.add<bool>("","tagMatchesTrigObj","O",false);
      i_nProbesMatchedToTag      = data.add<int>("","nProbesMatchedToTag","I",0);
/*
      i_mass          = data.addMulti<float>("","mass",-9);
      i_pt            = data.addMulti<float>("","pt",-9);
      i_eta           = data.addMulti<float>("","eta",-9);
      i_abseta           = data.addMulti<float>("","abseta",-9);
      i_htalong       = data.addMulti<float>("","htalong",-9);
      i_phi           = data.addMulti<float>("","phi",-9);
      i_pass          = data.addMulti<bool>("","pass",false);
      i_tagMatchesTrigObj       = data.addMulti<bool>("","tagMatchesTrigObj",false);
      i_nProbesMatchedToTag      = data.addMulti<int>("","nProbesMatchedToTag",0);
*/
      i_nTags                    = data.add<int  >("","nTags","I",0);
    }

    bool fillEvent() {

      // apply some event filters
      if(!goodvertex) return false;
      if(applyCHFFilter && !cfgSet::passCHFFilter(jets)) return false;

      // fill event vars
      data.fill<unsigned int>(i_run, this->run);
      data.fill<unsigned int>(i_lumi, this->lumi);
      data.fill<unsigned int>(i_event, this->event);
      data.fill<unsigned int>(i_process, this->process);
      data.fill<bool >(i_ismc,   this->isMC());
      data.fill<float>(i_weight, this->weight);
      data.fill<float>(i_truePUWeight,       this->eventCorrections.getTruePUWeight());
      data.fill<float>(i_trigEleWeight,      this->triggerCorrections.getTrigEleWeight());
      data.fill<float>(i_lepselweight,       this->leptonCorrections.getSelLepWeight());
      data.fill<float>(i_leptnpweight,       this->leptonCorrections.getTnPLepWeight());

      // fill Trigger and filters
      data.fill<bool>(i_passjson,       this->isMC() || (this->hasJSONFile() && this->passesLumiMask()));
      bool passTrigEl = this->triggerflag & kHLT_Ele27_eta2p1_WPLoose_Gsf;
      bool passTrigMu = (this->triggerflag & kHLT_IsoMu22) || (this->triggerflag & kHLT_IsoTkMu22);
      //data.fill<bool>(i_passtrigmu,     this->isMC() || (this->process==defaults::DATA_SINGLEMU ? passTrigMu: false));
      data.fill<bool>(i_passtrige,      this->isMC() || (this->process==defaults::DATA_SINGLEEL ? (passTrigEl && (!passTrigMu)) : false));

      const auto &evt = this->evtInfoReader;
      bool passmetfilters = evt.HBHENoiseFilter && evt.HBHENoiseIsoFilter && evt.globalTightHalo2016Filter && evt.EcalDeadCellTriggerPrimitiveFilter && evt.goodVertices && evt.eeBadScFilter && evt.badChCand && evt.badPFMuon;
      data.fill<bool>(i_passmetfilters,  passmetfilters);

      // require event to pass trigger, so that we can match tags to trigger objects
      //if (!(passTrigEl || passTrigMu)) return false;
      if(!(this->isMC() || passTrigEl)) return false;

      if(dbg) std::cout << "*******************plugged event vars" << std::endl;

      // store prompt [from Z] gen leps [just EL/MU]
      if(isMC() && applyGenMatch_) {
        genLeptons_.clear();
        genLeptons_.reserve(2);
        for(const auto* p : genParts) {
          // check for daughters of Zs ... to be updated when we add J/Psis / Upsilons
          if(ParticleInfo::isA(ParticleInfo::p_Z0, p)) {
            for(unsigned int id = 0; id < p->numberOfDaughters(); ++id) {
              const GenParticleF* dau = p->daughter(id);
              if(lepIdChoice == 11 && ParticleInfo::isA(ParticleInfo::p_eminus, dau)){
                genLeptons_.push_back(dau);
              }else if(lepIdChoice == 13 && ParticleInfo::isA(ParticleInfo::p_muminus, dau)){
                genLeptons_.push_back(dau);
              }
            }
          }
        }
      }//genlep

      if(dbg) std::cout << "plugged gen leps: " << genLeptons_.size() << std::endl;

      // store tag lep candidates: allLeptons, choice of flavor, choice of genmatching if MC
      tnpLeptons_.clear();
      tnpLeptons_.reserve(allLeptons.size());
      float hardestTagPt = -1;
      for(auto* l : allLeptons) {
        if(l->pdgid() != lepIdChoice) continue;
        if(abs(l->eta()) > 2.1 || l->pt() < 30) continue;
        if(isMC() && applyGenMatch_) {
          double nearDR = 0;
          int near = PhysicsUtilities::findNearestDRDeref(*l, genLeptons_, nearDR, maxGenMatchDR_, 10.0, 0, genLeptons_.size());
          if(near < 0) continue;
        }

        // tags must pass id & iso
        //if(tag->ismuon() && !cfgSet::isSelMuon(*(MuonF*)tag, LepConfig_.muons)) continue;
        //else if(tag->iselectron() && !cfgSet::isSelElectron(*(ElectronF*)tag, LepConfig_.electrons)) continue;
        if(l->iselectron() && !cfgSet::isSelElectron(*(ElectronF*)l, LepConfig_.electrons)) continue;

        if(dbg) std::cout << "tag passed id/iso " << std::endl;

        // tag must pass trig obj match in data, or gen match in mc
        bool tagMatchesTrigObj   = false;
        if(!isMC()){
          for(auto* to : triggerObjects) {
            if ( (to->pathflags() & kHLT_Ele27_eta2p1_WPLoose_Gsf) && PhysicsUtilities::deltaR(*to,*l) < maxTrigMatchDR_){ //no to->filterflags() for photons?
              tagMatchesTrigObj = true;
              break;
            }
          }
        }else{ // isMC
          tagMatchesTrigObj = true;
        }
        //data.fillMulti<bool >(i_tagMatchesTrigObj, tagMatchesTrigObj); // just to record how often it happens
        data.fill<bool >(i_tagMatchesTrigObj, tagMatchesTrigObj); // just to record how often it happens

        if(!tagMatchesTrigObj) continue;

        if(dbg) std::cout << "tag passed trig obj matching: " << l->pt() << std::endl;

        tnpLeptons_.push_back(l);
        if( useHardestTag && (l->pt() > hardestTagPt) ) {
          hardestTag = l;
          hardestTagPt = l->pt();
        }
      }

      if(dbg) std::cout << "plugged tag leps: " << tnpLeptons_.size() << std::endl;
      for(auto* l : tnpLeptons_) {
        if(dbg) std::cout << "Tag lepton: " << l->p4() << std::endl;
      }

      // store probe photon candidates: allPhotons
      // see AnalysisBase/TreeAnalyzer/src/DefaultProcessing.cc
      tnpPhotons_.clear();
      tnpPhotons_.reserve(this->photonReader.photons.size());
      vector<PhotonF*> allPhotons_;
      for(unsigned int i = 0; i < photonReader.photons.size(); i++){
        allPhotons_.push_back(&photonReader.photons[i]);
      }
      float hardestProbePt = -1;
      for(auto* pho : allPhotons_) {
        if(dbg) std::cout << "photon in allPhotons: " << pho << " " << pho->pt() << " " << pho->passElectronVeto() << " " << pho->eta() << std::endl;
        //if(pho->passElectronVeto()) continue; /// uncomment for real run. want denom to fail electron veto.
        if(fabs(pho->eta())>2.1 || pho->pt() < 20) continue;
        tnpPhotons_.push_back(pho);
        if(dbg) std::cout << "** added a photon to tnpPhotons_" << std::endl;
        if( useHardestProbe && (pho->pt() > hardestProbePt) ) {
          hardestProbe = pho;
          hardestProbePt = pho->pt();
        }
      }

      if(dbg) std::cout << "plugged probe photons: " << tnpPhotons_.size() << std::endl;
      for(auto* p : tnpPhotons_){
        if(dbg) std::cout << "Probe photons: " << p << " " << p->p4() << std::endl;
      }

/* copied from defaultprocessing
bool cfgSet::isSelPhoton(const ucsbsusy::PhotonF& pho, const PhotonConfig& conf       ){
  if (conf.usePixelSeedVeto && pho.hasPixelSeed())         return false;
  if (conf.useElectronVeto  && (!pho.passElectronVeto()) ) return false;
  return (pho.pt() > conf.minPt && fabs(pho.eta()) < conf.maxEta && (pho.*conf.selected)());
}
...
  for(auto& pfc : allPhotons) {
    if(isSelPhoton(pfc,conf))
      selectedPhotons.push_back(&pfc);
  }
*/

      // require two or more tnp candidates of course
      if(tnpLeptons_.size() < 1 || tnpPhotons_.size() < 1) return false;

      if(dbg) std::cout << "have enough tnps to continue" << std::endl;

      // nested tnp loop: for(tag){for(probe){..}}
      int nTags = 0;
      for(auto* tag : tnpLeptons_) {
        if( useHardestTag && hardestTag && (tag != hardestTag) ) continue;
        if(dbg) std::cout << "tag pt: " << tag->pt() << std::endl;
        nTags++;
        int nProbesMatchedToTag = 0; // number of probes to which the tag matches
        PhotonF* hardestMatchedProbe = 0;
        float    hardestMatchedProbePt = -1;
        bool hardpasspho = false; // scope

        for(auto* probe : tnpPhotons_) {

          //if(useHardestProbe && (nProbesMatchedToTag>0)) continue;
          if(useHardestProbe && probe != hardestProbe) continue;
          if(dbg) std::cout << "looking at probe: " << probe->pt() << std::endl;

          // probe isn't tag
          if(probe->index() == tag->index()) continue;

          if(dbg) std::cout << "passed index" << std::endl;

          MomentumF* dilep = new MomentumF(tag->p4() + probe->p4());

          if(dbg) std::cout << "dilep mass: " << dilep->mass() << std::endl;

          // probe must have dilep mass in Z window
          if(dilep->mass() < minMass_ || dilep->mass() > maxMass_) continue;

          if(dbg) std::cout << "passed mass window " << std::endl;

          // measure the probe pass efficiencies
          bool passpho = ((*probe).*PhoConfig_.selected)();

          if(dbg) std::cout << "probe pass: " << passpho << std::endl;

          if(applyPixelSeedVeto) passpho &= !(probe->hasPixelSeed());

          if(dbg) std::cout << "probe pass 2: " << passpho << std::endl;

          // extra variables ... 
          float htalong = -9;
          bool cleanHttag   = false;
          bool cleanHtprobe = false;
          for(const auto* jet : jets ){
            if (PhysicsUtilities::absDeltaPhi(*jet,*probe) < TMath::PiOver2()) {
              htalong += jet->pt();
              if(PhysicsUtilities::deltaR(*jet,*tag  ) < 0.4) cleanHttag   = true;
             if(PhysicsUtilities::deltaR(*jet,*probe) < 0.4) cleanHtprobe = true;
             }
          }
          if(cleanHttag)   htalong -= tag->pt();
          if(cleanHtprobe) htalong -= probe->pt();

          if(!useHardestMatchedProbe){
            // fill probe vars in case of multiple matching probes
            data.fill<float>(i_mass, dilep->mass());
            data.fill<float>(i_pt, probe->pt());
            data.fill<float>(i_eta, probe->eta());
            data.fill<float>(i_abseta, abs(probe->eta()));
            data.fill<float>(i_phi, probe->phi());
            data.fill<bool >(i_pass, passpho);
            data.fill<float>(i_htalong, htalong);
/*
            data.fillMulti<float>(i_mass, dilep->mass());
            data.fillMulti<float>(i_pt, probe->pt());
            data.fillMulti<float>(i_eta, probe->eta());
            data.fillMulti<float>(i_abseta, abs(probe->eta()));
            data.fillMulti<float>(i_phi, probe->phi());
            data.fillMulti<bool >(i_pass, passpho);
            data.fillMulti<float>(i_htalong, htalong);
*/
            ++nProbesMatchedToTag;
          }else{ // useHardestMatchedProbe
            if(probe->pt() > hardestMatchedProbePt){
              hardestMatchedProbe = probe;
              hardestMatchedProbePt = probe->pt();
            }
          }
          delete dilep;
        }//forProbe

        // fill probe vars in case of one (hardest) matching probe
        if(useHardestMatchedProbe && hardestMatchedProbePt > 0){
          PhotonF* probe = hardestMatchedProbe;
          MomentumF* dilep = new MomentumF(tag->p4() + probe->p4());

          // measure the probe pass efficiencies
          bool passpho = ((*probe).*PhoConfig_.selected)();
          if(applyPixelSeedVeto) passpho &= !(probe->hasPixelSeed());

          // extra variables ...
          float htalong = -9;
          bool cleanHttag   = false;
          bool cleanHtprobe = false;
          for(const auto* jet : jets ){
            if (PhysicsUtilities::absDeltaPhi(*jet,*probe) < TMath::PiOver2()) {
              htalong += jet->pt();
              if(PhysicsUtilities::deltaR(*jet,*tag  ) < 0.4) cleanHttag   = true;
             if(PhysicsUtilities::deltaR(*jet,*probe) < 0.4) cleanHtprobe = true;
             }
          }
          if(cleanHttag)   htalong -= tag->pt();
          if(cleanHtprobe) htalong -= probe->pt();

          data.fill<float>(i_mass, dilep->mass());
          data.fill<float>(i_pt, probe->pt());
          data.fill<float>(i_eta, probe->eta());
          data.fill<float>(i_abseta, abs(probe->eta()));
          data.fill<float>(i_phi, probe->phi());
          data.fill<bool >(i_pass, passpho);
          data.fill<float>(i_htalong, htalong);
/*
          data.fillMulti<float>(i_mass, dilep->mass());
          data.fillMulti<float>(i_pt, probe->pt());
          data.fillMulti<float>(i_eta, probe->eta());
          data.fillMulti<float>(i_abseta, abs(probe->eta()));
          data.fillMulti<float>(i_phi, probe->phi());
          data.fillMulti<bool >(i_pass, passpho);
          data.fillMulti<float>(i_htalong, htalong);
*/
          ++nProbesMatchedToTag;

          delete dilep;
        }

        // fill tag vars
        //data.fillMulti<bool >(i_tagMatchesTrigObj, tagMatchesTrigObj);
//        data.fillMulti<int  >(i_nProbesMatchedToTag, nProbesMatchedToTag);
        data.fill<int  >(i_nProbesMatchedToTag, nProbesMatchedToTag);
      }//forTag

      // fill event vars
      data.fill<int  >(i_nTags, nTags);

      return true;
    }//fillEvent

};

void makeTnPPhotonTrees(TString sname = "dyee-m50-powheg",
                    const int fileindex = -1,
                    const bool isMC = false,
                    const TString fname = "/store/user/apatters/13TeV/12072016/merged/dyee_10_ntuple_postproc.root",
                    const TString outputdir = "photontrees",
                    const TString fileprefix = "root://cmseos:1094/",
                    const TString json=TString::Format("%s/src/data/JSON/Cert_271036-275783_13TeV_PromptReco_Collisions16_JSON.txt",getenv("CMSSW_BASE")))
{

  printf("Processing file %d of %s sample\n", (fileindex > -1 ? fileindex : 0), sname.Data());

  if(fileindex > -1)
    sname += TString::Format("_%d",fileindex);

  TString fullname = fileprefix+fname;

  gSystem->mkdir(outputdir,true);
  TString outfilename = outputdir+"/"+sname+"_tree.root";

  TString treeName = "Events";

  // see AnalysisBase/TreeAnalyzer/src/DefaultConfigurations.cc
  cfgSet::ConfigSet cfg    = pars1LCR(json); // match with desired region
  cfgSet::ConfigSet phocfg = pars0LepPhoton(json); // for photon id & iso
  //phocfg.photons.selected         = &ucsbsusy::PhotonF::isloose; // default photon configuration. medium or tight also available
  //phocfg.photons.minPt            = 20;   // don't need to change - done manually above
  //phocfg.photons.maxEta           = 2.1;  // --
  //phocfg.photons.usePixelSeedVeto = true; // --
  //phocfg.photons.useElectronVeto  = true; // --

  TnPAnalyzer a(fullname, treeName, outfilename, fileindex+2, isMC, &cfg);

  a.LepConfig_             = cfg; // for tag electrons
  a.PhoConfig_             = phocfg.photons; // for tag photons
  a.applyPixelSeedVeto     = false; // true in our FW. toggle me.
  a.useHardestTag          = true; // uses hardest tag in event
  a.useHardestProbe        = false; // use with caution. uses only the hardest probe in _event_, which usually doesn't match a tag!
  a.useHardestMatchedProbe = true; // uses the hardest probe matched to each tag

  a.analyze(10000,-1);
}
