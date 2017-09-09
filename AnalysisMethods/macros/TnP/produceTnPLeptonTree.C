/*

current singlelep triggers
bool passTrigEl = ana->triggerflag & kHLT_Ele27_eta2p1_WPLoose_Gsf;
bool passTrigMu = (ana->triggerflag & kHLT_IsoMu22) || (ana->triggerflag & kHLT_IsoTkMu22);

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
    size i_passtrige ;
    size i_passtrigmu;
    size i_passmetfilters;
    size i_passmetfilters2017;
    // tnp vars
    size i_diMass      ;
    size i_probePt        ;
    size i_probeEta       ;
    size i_probeHtAlong   ;
    size i_probeAnnulus   ;
    size i_probePhi       ;
    size i_probeCharge    ;
    size i_probePassIdIso      ;
    size i_probePassIdNoIso    ;
    size i_tagMatchesTrigObj  ;
    size i_nProbesMatchedToTag  ;
    size i_nTags;

    const bool   applyGenMatch_   = true;    // require truth matching for MC tags/probes
    const double maxGenMatchDR_   = 0.3;     // DR for gen matching
    const double maxTrigMatchDR_  = 0.1;     // DR for matching tag to trigger object
    const double minMass_         = 60.0;    // min for mass window
    const double maxMass_         = 120.0;   // max for mass window
    int    lepIdChoice;                           // electron/muon
    bool   applyCHFFilter = false ;

    vector<const GenParticleF*> genLeptons_; // hold gen leptons for matching
    vector<LeptonF*>            tnpLeptons_; // hold tag and probe candidates

    cfgSet::ConfigSet LepConfigIdIso_;   // lep config to pass both id and iso
    cfgSet::ConfigSet LepConfigIdNoIso_; // lep config to pass just id, with no iso cuts

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
      i_passtrige      = data.add<bool>("","passtrige","O",0);
      i_passtrigmu     = data.add<bool>("","passtrigmu","O",0);
      i_passmetfilters = data.add<bool>("","passmetfilters","O",0);
      i_passmetfilters2017 = data.add<bool>("","passmetfilters2017","O",0);
      // tnp vars
      i_diMass          = data.addMulti<float>("","diMass",-9);
      i_probePt            = data.addMulti<float>("","probePt",-9);
      i_probeEta           = data.addMulti<float>("","probeEta",-9);
      i_probeHtAlong       = data.addMulti<float>("","probeHtAlong",-9);
      i_probeAnnulus       = data.addMulti<float>("","probeAnnulus",-9);
      i_probePhi           = data.addMulti<float>("","probePhi",-9);
      i_probeCharge        = data.addMulti<float>("","probeCharge",-9);
      i_probePassIdIso          = data.addMulti<bool>("","probePassIdIso",false);
      i_probePassIdNoIso        = data.addMulti<bool>("","probePassIdNoIso",false);
      i_tagMatchesTrigObj      = data.addMulti<bool>("","tagMatchesTrigObj",false);
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
      bool passTrigEl = this->triggerflag & kHLT_Ele27_eta2p1_WPLoose_Gsf;
      bool passTrigMu = (this->triggerflag & kHLT_IsoMu22) || (this->triggerflag & kHLT_IsoTkMu22);

      data.fill<bool>(i_passtrigmu,     this->isMC() || (this->process==defaults::DATA_SINGLEMU ? passTrigMu: false));
      data.fill<bool>(i_passtrige,      this->isMC() || (this->process==defaults::DATA_SINGLEEL ? (passTrigEl && (!passTrigMu)) : false));
      const auto &evt = this->evtInfoReader;
      bool passmetfilters = evt.HBHENoiseFilter && evt.HBHENoiseIsoFilter && evt.globalTightHalo2016Filter && evt.EcalDeadCellTriggerPrimitiveFilter && evt.goodVertices && evt.eeBadScFilter && evt.badChCand && evt.badPFMuon;
      bool passmetfilters2017 = evt.HBHENoiseFilter && evt.HBHENoiseIsoFilter && evt.globalSuperTightHalo2016Filter && evt.EcalDeadCellTriggerPrimitiveFilter && evt.goodVertices && evt.eeBadScFilter && evt.badChCand && evt.badPFMuon;
      data.fill<bool>(i_passmetfilters,  passmetfilters);
      data.fill<bool>(i_passmetfilters2017,  passmetfilters2017);

      // require event to pass trigger, so that we can match tags to trigger objects
      if (!(passTrigEl || passTrigMu)) return false;

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


      // store tag and probe candidates: allLeptons, choice of flavor, choice of genmatching if MC
      tnpLeptons_.clear();
      tnpLeptons_.reserve(allLeptons.size());
      for(auto* l : allLeptons) {
        if(l->pdgid() != lepIdChoice) continue;
        if(abs(l->eta()) > 2.1) continue;
        // apply MC truth matching
        if(isMC() && applyGenMatch_) {
          double nearDR = 0;
          int near = PhysicsUtilities::findNearestDRDeref(*l, genLeptons_, nearDR, maxGenMatchDR_, 10.0, 0, genLeptons_.size());
          if(near < 0) continue;
        }
        tnpLeptons_.push_back(l);
      }

      // require two or more tnp candidates of course
      if(tnpLeptons_.size() < 2) return false;

      // nested tnp loop: for(tag){for(probe){..}}
      int nTags = 0;
      for(auto* tag : tnpLeptons_) {
        // tags must pass id & iso
        if(tag->ismuon() && !cfgSet::isSelMuon(*(MuonF*)tag, LepConfigIdIso_.muons)) continue;
        else if(tag->iselectron() && !cfgSet::isSelElectron(*(ElectronF*)tag, LepConfigIdIso_.electrons)) continue;

        // tag must pass trig obj match in data, or gen match in mc
        bool tagMatchesTrigObj   = false;
        if(!isMC()){
          for(auto* to : triggerObjects) {
            if(lepIdChoice == 13 && (to->filterflags() & kSingleIsoTkMu22) && ( ((to->pathflags() & kHLT_IsoMu22) || (to->pathflags() & kHLT_IsoTkMu22))) 
                                                     && PhysicsUtilities::deltaR(*to,*tag) < maxTrigMatchDR_) {
              tagMatchesTrigObj      = true;
              break;
            }else if(lepIdChoice == 11 && (to->filterflags() & kSingleEle25WPTight) && (isMC() ? (to->pathflags() & kHLT_Ele25_eta2p1_WPTight_Gsf) : 
                                                     (to->pathflags() & kHLT_Ele25_eta2p1_WPTight_Gsf)) && PhysicsUtilities::deltaR(*to,*tag) < maxTrigMatchDR_) {
              tagMatchesTrigObj      = true;
              break;
            }
          }
        }else{ //isMC
          tagMatchesTrigObj = true;
        }
        if(!tagMatchesTrigObj) continue;

        // tag is now a good tag. look for probes
        nTags++;
        int nProbesMatchedToTag = 0; // number of probes to which the tag matches
        for(auto* probe : tnpLeptons_) {
      
          // probe isn't tag
          if(probe->index() == tag->index()) continue;

          // probe must have opposite charge as tag
          if(probe->q() == tag->q()) continue;

          MomentumF* dilep = new MomentumF(tag->p4() + probe->p4());

          // probe must have dilep mass in Z window
          if(dilep->mass() < minMass_ || dilep->mass() > maxMass_) continue;

          // measure the probe pass efficiencies
          bool passidiso   =  probe->ismuon() ? cfgSet::isSelMuon(*(MuonF*)probe, LepConfigIdIso_.muons) : cfgSet::isSelElectron(*(ElectronF*)probe, LepConfigIdIso_.electrons);
          bool passidnoiso =  probe->ismuon() ? cfgSet::isSelMuon(*(MuonF*)probe, LepConfigIdNoIso_.muons) : cfgSet::isSelElectron(*(ElectronF*)probe, LepConfigIdNoIso_.electrons);

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
          data.fillMulti<float>(i_probeAnnulus, (probe->pt())*(probe->annulusactivity()) );
          data.fillMulti<float>(i_probePhi, probe->phi());
          data.fillMulti<float>(i_probeCharge, probe->q());
          data.fillMulti<bool >(i_probePassIdIso, passidiso); // id and iso combined for now
          data.fillMulti<bool >(i_probePassIdNoIso, passidnoiso);
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

// remember to set the correct leptonId (11 == electrons, 13 == muons)
void produceTnPLeptonTree(TString sname = "dyll-m50-madgraph-ext",
                    const int fileindex = 0,
                    const bool isMC = true,
                    const TString fname = "/store/user/hqu/13TeV/020716/merged/dyll-m50-madgraph-ext_1_ntuple_postproc.root",
                    const double xsec = 1.0,
                    const TString outputdir = "trees",
                    const TString fileprefix = "root://cmseos:1094/",
                    const TString json=TString::Format("%s/src/data/JSON/Cert_271036-275125_13TeV_PromptReco_Collisions16_JSON.txt",getenv("CMSSW_BASE")),
                    const int leptonId = 13)
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
  cfgSet::ConfigSet cfg = pars1LCR(json); // match this lep config with the region we apply these corrections: 1LCR now.

  TnPAnalyzer a(fullname, treeName, outfilename, fileindex+2, isMC, &cfg);

  a.LepConfigIdIso_     = cfg; // will be used dynamically, ie using isSelX(config, lep) where lep is drawn from allLeptons (no selection)
  a.LepConfigIdNoIso_   = cfg;

  // set lepton flavor
  a.lepIdChoice = leptonId;

  a.analyze(10000,200000);

}
