
bool dbg = true;
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
    // trigger and filters
    size i_ismc      ;
    size i_passjson   ;
    //size i_passtrige ;
    //size i_passtrigmu;
    size i_passtrigphoton165;
    size i_passmetfilters;
    // tnp vars
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

    vector<const GenParticleF*> genLeptons_; // hold gen leptons for matching
    vector<LeptonF*>            tnpLeptons_; // hold tag and probe candidates
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
      // Trigger and filters
      i_passjson       = data.add<bool>("","passjson","O",0);
      //i_passtrige      = data.add<bool>("","passtrige","O",0);
      //i_passtrigmu     = data.add<bool>("","passtrigmu","O",0);
      i_passtrigphoton165 = data.add<bool>("","passtrigphoton165","O",0);
      i_passmetfilters = data.add<bool>("","passmetfilters","O",0);
      // tnp vars
      i_diMass          = data.addMulti<float>("","diMass",-9);
      i_probePt            = data.addMulti<float>("","probePt",-9);
      i_probeEta           = data.addMulti<float>("","probeEta",-9);
      i_probeHtAlong       = data.addMulti<float>("","probeHtAlong",-9);
      //i_probeAnnulus       = data.addMulti<float>("","probeAnnulus",-9);
      i_probePhi           = data.addMulti<float>("","probePhi",-9);
      //i_probeCharge        = data.addMulti<float>("","probeCharge",-9);
      i_probePass          = data.addMulti<bool>("","probePass",false);
      //i_probePassIdIso          = data.addMulti<bool>("","probePassIdIso",false);
      //i_probePassIdNoIso        = data.addMulti<bool>("","probePassIdNoIso",false);
      i_tagMatchesTrigObj       = data.addMulti<bool>("","tagMatchesTrigObj",false);
      i_nProbesMatchedToTag      = data.addMulti<int>("","nProbesMatchedToTag",0);
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

      // fill Trigger and filters
      data.fill<bool>(i_passjson,       this->isMC() || (this->hasJSONFile() && this->passesLumiMask()));
      //bool passTrigEl = this->triggerflag & kHLT_Ele27_eta2p1_WPLoose_Gsf;
      //bool passTrigMu = (this->triggerflag & kHLT_IsoMu22) || (this->triggerflag & kHLT_IsoTkMu22);
      //data.fill<bool>(i_passtrigmu,     this->isMC() || (this->process==defaults::DATA_SINGLEMU ? passTrigMu: false));
      //data.fill<bool>(i_passtrige,      this->isMC() || (this->process==defaults::DATA_SINGLEEL ? (passTrigEl && (!passTrigMu)) : false));
      data.fill<bool>(i_passtrigphoton165, this->isMC() || (this->triggerflag & kHLT_Photon165_HE10));

      const auto &evt = this->evtInfoReader;
      bool passmetfilters = evt.HBHENoiseFilter && evt.HBHENoiseIsoFilter && evt.globalTightHalo2016Filter && evt.EcalDeadCellTriggerPrimitiveFilter && evt.goodVertices && evt.eeBadScFilter && evt.badChCand && evt.badPFMuon;
      data.fill<bool>(i_passmetfilters,  passmetfilters);

      // require event to pass trigger, so that we can match tags to trigger objects
      //if (!(passTrigEl || passTrigMu)) return false;
      //if(!passtrigphoton165) return false; // uncomment for final results

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
      for(auto* l : allLeptons) {
        if(l->pdgid() != lepIdChoice) continue;
        if(abs(l->eta()) > 2.1 || l->pt() < 30) continue;
        if(isMC() && applyGenMatch_) {
          double nearDR = 0;
          int near = PhysicsUtilities::findNearestDRDeref(*l, genLeptons_, nearDR, maxGenMatchDR_, 10.0, 0, genLeptons_.size());
          if(near < 0) continue;
        }
        tnpLeptons_.push_back(l);
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
      for(auto* pho : allPhotons_) {
        if(dbg) std::cout << "photon in allPhotons: " << pho << " " << pho->pt() << " " << pho->passElectronVeto() << " " << pho->eta() << std::endl;
        //if(pho->passElectronVeto()) continue; /// uncomment for real run. want denom to fail electron veto.
        if(fabs(pho->eta())>2.1 || pho->pt() < 20) continue;
        tnpPhotons_.push_back(pho);
        if(dbg) std::cout << "** added a photon to tnpPhotons_" << std::endl;
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

        if(dbg) std::cout << "tag pt: " << tag->pt() << std::endl;

        // tags must pass id & iso
        //if(tag->ismuon() && !cfgSet::isSelMuon(*(MuonF*)tag, LepConfig_.muons)) continue;
        //else if(tag->iselectron() && !cfgSet::isSelElectron(*(ElectronF*)tag, LepConfig_.electrons)) continue;
        if(tag->iselectron() && !cfgSet::isSelElectron(*(ElectronF*)tag, LepConfig_.electrons)) continue;

        if(dbg) std::cout << "tag passed id/iso " << std::endl;

        // tag must pass trig obj match in data, or gen match in mc
        bool tagMatchesTrigObj   = false;
        if(!isMC()){
          for(auto* to : triggerObjects) {
            if ( (to->pathflags() &  kHLT_Photon165_HE10) ){ //no to->filterflags() for photons?
              tagMatchesTrigObj = true;
              break;
            }
          }
        }else{ // isMC
          tagMatchesTrigObj = true;
        }

        if(!tagMatchesTrigObj) continue;

        if(dbg) std::cout << "tag passed trig obj matching: " << tag->pt() << std::endl;

        // tag is now a good tag. look for probes
        nTags++;
        int nProbesMatchedToTag = 0; // number of probes to which the tag matches
        for(auto* probe : tnpPhotons_) {

          if(dbg) std::cout << "looking at probe: " << probe->pt() << std::endl;

          // probe isn't tag
          if(probe->index() == tag->index()) continue;

          // probe must have opposite charge as tag
          //if(probe->q() == tag->q()) continue;

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

          // fill probe vars
          data.fillMulti<float>(i_diMass, dilep->mass());
          data.fillMulti<float>(i_probePt, probe->pt());
          data.fillMulti<float>(i_probeEta, probe->eta());
          //data.fillMulti<float>(i_probeAnnulus, (probe->pt())*(probe->annulusactivity()) );
          data.fillMulti<float>(i_probePhi, probe->phi());
          //data.fillMulti<float>(i_probeCharge, probe->q());
          data.fillMulti<bool >(i_probePass, passpho);
          //data.fillMulti<bool >(i_probePassIdIso, passidiso); // id and iso combined for now
          //data.fillMulti<bool >(i_probePassIdNoIso, passidnoiso);
          data.fillMulti<float>(i_probeHtAlong, htalong);

          ++nProbesMatchedToTag;

          // NB: at the moment if there are multiple T-P pairs in an event, we save all of them. Might want to try the other options
        }//forProbe

        // fill tag vars
        data.fillMulti<bool >(i_tagMatchesTrigObj, tagMatchesTrigObj);
        data.fillMulti<int  >(i_nProbesMatchedToTag, nProbesMatchedToTag);
      }//forTag

      // fill event vars
      data.fill<int  >(i_nTags, nTags);

      return true;
    }//fillEvent

};

void produceTnPPhotonTree(TString sname = "dyee-m50-powheg",
                    const int fileindex = 0,
                    const bool isMC = true,
                    const TString fname = "/store/user/apatters/13TeV/12072016/merged/dyee_10_ntuple_postproc.root",
                    const double xsec = 1.0,
                    const TString outputdir = "photontrees",
                    const TString fileprefix = "root://cmseos:1094/",
                    const TString json=TString::Format("%s/src/data/JSON/Cert_271036-275783_13TeV_PromptReco_Collisions16_JSON.txt",getenv("CMSSW_BASE")))
{

  printf("Processing file %d of %s sample\n", (fileindex > -1 ? fileindex : 0), sname.Data());

  if(fileindex > -1)
    sname += TString::Format("_%d",fileindex);

  if(isMC)
    printf("Cross section: %5.2f pb\n", xsec);

  TString fullname = fileprefix+fname;

  gSystem->mkdir(outputdir,true);
  TString outfilename = outputdir+"/"+sname+"_tree.root";

  TString treeName = "Events";

  // see AnalysisBase/TreeAnalyzer/src/DefaultConfigurations.cc
  cfgSet::ConfigSet cfg    = pars1LCR(json); // match with desired region
  cfgSet::ConfigSet phocfg = pars0LepPhoton(json); // for photon id & iso
  //phocfg.photons.selected           = &ucsbsusy::PhotonF::isloose; // default photon configuration. medium or tight also available
  //phocfg.photons.minPt            = 20;   // don't need to change - done manually above
  //phocfg.photons.maxEta           = 2.1;  // --
  //phocfg.photons.usePixelSeedVeto = true; // --
  //phocfg.photons.useElectronVeto  = true; // --

  TnPAnalyzer a(fullname, treeName, outfilename, fileindex+2, isMC, &cfg);

  a.LepConfig_ = cfg; // for tag electrons
  a.PhoConfig_ = phocfg.photons; // for tag photons
  a.applyPixelSeedVeto = false;

  if(dbg) std::cout << "analyzing" << std::endl;

  a.analyze(10000,1000);

}
