#if !defined(__CINT__) || defined(__MAKECINT__)
#include "TSystem.h"
#include "AnalysisBase/TreeAnalyzer/interface/BaseTreeAnalyzer.h"
#include "AnalysisBase/TreeAnalyzer/interface/DefaultProcessing.h"
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
#endif

// class to create a tree with the information needed to run the Tag&Probe efficiency calculations
// we apply truth matching for lepton candidates in MC, and trigger matching for the tag
// deltaR values for the matching are configurable
// mass window will have to be adjusted if we fit non-Z resonances


using namespace ucsbsusy;
//using namespace std; // remove for committing?

cfgSet::ConfigSet pars1LCR(TString json) {
  cfgSet::loadDefaultConfigurations();
  cfgSet::setJSONFile(json);
  cfgSet::ConfigSet cfg = cfgSet::zl_search_set;
//  cfg.jets.cleanJetsvLeptons = true;  // Do not clean jets vs leptons
  cfg.electrons              = LeptonSelection::zl_ctr_sLep_electrons;
  cfg.muons                  = LeptonSelection::zl_ctr_sLep_muons;
  cfg.corrections.eventCorrections |= ucsbsusy::EventCorrectionSet::NORM;
  return cfg;
}

class TnPAnalyzer : public BaseTreeAnalyzer {

  public :
    TnPAnalyzer(TString fileName, TString treeName, TString outfileName, bool isMCTree, cfgSet::ConfigSet *pars) :
      BaseTreeAnalyzer(fileName, treeName, isMCTree, pars),
      outFileName_(outfileName),
      treeweight(1),
      mass(0),
      pt(0),
      eta(0),
      abseta(0),
      htalong(0),
      annulus(0),
      phi(0),
      charge(0),
      pass(0),
      matchnum(0),
      passid(0),
      passiso(0)
    {
      if(fileName.Contains("dyjets")) zIsInvisible = false;
      setupTree();
    }

    const bool   applyGenMatch_   = true;    // require truth matching for MC tags/probes
    const double maxGenMatchDR_   = 0.3;     // DR for gen matching
    const double maxTrigMatchDR_  = 0.1;     // DR for matching tag to trigger object
    const double minMass_         = 60.0;    // min for mass window
    const double maxMass_         = 120.0;   // max for mass window

    bool   is50ns_;                          // for PU weight
    int    lepId_;                           // electron/muon

    vector<const GenParticleF*> genLeptons_; // hold gen leptons for matching
    vector<LeptonF*>            tnpLeptons_; // hold tag and probe candidates

    cfgSet::LeptonConfig  goodLeptonConfig_; // lepton selection

    // tree vars
    float treeweight;
    float mass;
    float pt;
    float eta;
    float abseta;
    float htalong;
    float annulus;
    float phi;
    float charge;
    int   pass;
    int   trgmatch;
    int   matchnum;
    int   passid;
    int   passiso;

    // output
    TString outFileName_;
    TFile*  outFile_;
    TTree*  outTree_;

    virtual ~TnPAnalyzer() {
      outFile_->cd();
      outTree_->Write();
      outFile_->Close();
      delete outFile_;
    }

    void setupTree() {
      // initialize tree with probe candidates
      outFile_ = new TFile(outFileName_,"RECREATE");
      outFile_->cd();
      outTree_ = new TTree("Probes","");
      outTree_->Branch("run",&run,"run/i");
      outTree_->Branch("lumi",&lumi,"lumi/i");
      outTree_->Branch("event",&event,"event/i");
      outTree_->Branch("weight",&treeweight,"weight/F");
      outTree_->Branch("npv",&nPV,"npv/I");
      outTree_->Branch("rho",&rho,"rho/F");
      outTree_->Branch("mass",&mass,"mass/F");
      outTree_->Branch("pt",&pt,"pt/F");
      outTree_->Branch("eta",&eta,"eta/F");
      outTree_->Branch("abseta",&abseta,"abseta/F");
      outTree_->Branch("htalong",&htalong,"htalong/F");
      outTree_->Branch("annulus",&annulus,"annulus/F");
      outTree_->Branch("phi",&phi,"phi/F");
      outTree_->Branch("charge",&charge,"charge/I");
      outTree_->Branch("pass",&pass,"pass/I");
      outTree_->Branch("trgmatch",&trgmatch,"trgmatch/I");
      outTree_->Branch("matchnum",&matchnum,"matchnum/I");
      // to be added: separate id and iso
      outTree_->Branch("passid",&passid,"passid/I");
      outTree_->Branch("passiso",&passiso,"passiso/I");
    }

    void loadVariables() {
      load(cfgSet::EVTINFO);
      load(cfgSet::AK4JETS,JetReader::LOADRECO | JetReader::FILLOBJ);
      load(cfgSet::ELECTRONS);
      load(cfgSet::MUONS);
      load(cfgSet::TRIGOBJS);
      if(isMC()) load(cfgSet::GENPARTICLES);
    }

    void processVariables() {
      BaseTreeAnalyzer::processVariables();

      // store gen leptons for matching
      if(isMC() && applyGenMatch_) {
        genLeptons_.clear();
        genLeptons_.reserve(2);
        for(const auto* p : genParts) {
          // check for daughters of Zs ... to be updated when we add J/Psis / Upsilons
          if(ParticleInfo::isA(ParticleInfo::p_Z0, p)) {
            for(unsigned int id = 0; id < p->numberOfDaughters(); ++id) {
              const GenParticleF* dau = p->daughter(id);
              if(lepId_ == 11 && ParticleInfo::isA(ParticleInfo::p_eminus, dau))
                genLeptons_.push_back(dau);
              else if(lepId_ == 13 && ParticleInfo::isA(ParticleInfo::p_muminus, dau))
                genLeptons_.push_back(dau);
            }
          }
        }
      }

      // fill lepton tag/probe candidates
      tnpLeptons_.clear();
      tnpLeptons_.reserve(allLeptons.size());
      for(auto* l : allLeptons) {
        if(l->pdgid() != lepId_) continue;
        // apply MC truth matching
        if(isMC() && applyGenMatch_) {
          double nearDR = 0;
          int near = PhysicsUtilities::findNearestDRDeref(*l, genLeptons_, nearDR, maxGenMatchDR_, 10.0, 0, genLeptons_.size());
          if(near < 0) continue;
        }
        tnpLeptons_.push_back(l);
      }

    }

    void runEvent() {
      // good vertex
      if(!goodvertex) return;

      // apply json
      if(!isMC() && hasJSONFile() && !passesLumiMask()) {
        return;
      }

      // don't use negative weighted MC events
      if(weight < 0) return;

      // need two leptons
      if(tnpLeptons_.size() < 2) return;

      // apply trigger to event
      ////if(lepId_ == 13 && !(triggerflag & kHLT_IsoTkMu24_eta2p1)) return; // kHLT_IsoMu24_eta2p1
      if(lepId_ == 13 && !( (triggerflag & kHLT_IsoMu22) || (triggerflag & kHLT_IsoTkMu22) )) return;
      else if(lepId_ == 11) {
        ////if (!isMC() && !(triggerflag & kHLT_Ele27_eta2p1_WPLoose_Gsf)) return;
        ////else if (isMC() && !(triggerflag & kHLT_Ele22_eta2p1_WP75_Gsf)) return;
        if (    !isMC() && !(triggerflag & kHLT_Ele25_eta2p1_WPTight_Gsf)) return;
        else if( isMC() && !(triggerflag & kHLT_Ele25_eta2p1_WPTight_Gsf)) return;
      }

      // tnp selection
      matchnum = 0;
      for(auto* tag : tnpLeptons_) {
        // tag has to be a good lepton
        if(tag->ismuon() && !cfgSet::isSelMuon(*(MuonF*)tag, goodLeptonConfig_)) continue;
        else if(tag->iselectron() && !cfgSet::isSelElectron(*(ElectronF*)tag, goodLeptonConfig_)) continue;

        // check the trigger turn-on
        ////if(lepId_==13 && tag->pt()<27) continue;
        if(lepId_==13 && tag->pt()<22) continue; // at trig for now (bin?)
        else if(lepId_==11 && tag->pt()<25) continue;
        ////else if(lepId_==11 && tag->pt()<35) continue;
        if(fabs(tag->eta())>2.1) continue;

        // require trigger matching of lepton to trigger object
        trgmatch = false;
        bool trigmatch = false;
        for(auto* to : triggerObjects) {
          ////if(lepId_ == 13 && (to->filterflags() & kSingleIsoTkMu24) && (to->pathflags() & kHLT_IsoTkMu24_eta2p1) && PhysicsUtilities::deltaR(*to,*tag) < maxTrigMatchDR_) {
          if(lepId_ == 13 && (to->filterflags() & kSingleIsoTkMu22) && ( (to->pathflags() & kHLT_IsoMu22 || (to->pathflags() & kHLT_IsoTkMu22)) && PhysicsUtilities::deltaR(*to,*tag) < maxTrigMatchDR_) {
            trgmatch = true;
            trigmatch = true;
            break;
          }
          ////else if(lepId_ == 11 && (to->filterflags() & kSingleEle27) && (isMC() ? (to->pathflags() & kHLT_Ele22_eta2p1_WP75_Gsf) : 
          ////                                                                        (to->pathflags() & kHLT_Ele27_eta2p1_WPLoose_Gsf)) && PhysicsUtilities::deltaR(*to,*tag) < maxTrigMatchDR_) {
          else if(lepId_ == 11 && (to->filterflags() & kSingleEle25WPTight) && (isMC() ? (to->pathflags() & kHLT_Ele25_eta2p1_WPTight_Gsf) : 
                                                                                         (to->pathflags() & kHLT_Ele25_eta2p1_WPTight_Gsf)) && PhysicsUtilities::deltaR(*to,*tag) < maxTrigMatchDR_) {
            trgmatch = true;
            trigmatch = true;
            break;
          }
        }
        if(lepId_==11 && isMC()) trigmatch=true; // ??
        if(!trigmatch) continue; // trigmatch is a hack to allow failing MC ele to pass? trgmatch is recorded in tree; trigmatch isn't.

        for(auto* probe : tnpLeptons_) {
          // make sure it's not the tag
          if(probe->index() == tag->index()) continue;

          // require opposite charge
          if(probe->q() == tag->q()) continue;

          MomentumF* dilep = new MomentumF(tag->p4() + probe->p4());

          // mass window
          if(dilep->mass() < minMass_ || dilep->mass() > maxMass_) continue;

          // get probe pass decision ... need to add separate id/iso decisions
          pass = probe->ismuon() ? cfgSet::isSelMuon(*(MuonF*)probe, goodLeptonConfig_) : cfgSet::isSelElectron(*(ElectronF*)probe, goodLeptonConfig_);

          // in principle should be accounting for negative weight events here
          ////treeweight = is50ns_ ? weight*eventCorrections.get50NSPUWeight() : weight*eventCorrections.getPUWeight();
          treeweight = weight;

          mass    = dilep->mass();
          pt      = probe->pt();
          eta     = probe->eta();
          abseta  = fabs(probe->eta());
          phi     = probe->phi();
          charge  = probe->q();
          annulus = pt * probe->annulusactivity();

          // hadronic activity
          htalong = 0;
          bool cleanHttag   = false;
          bool cleanHtprobe = false;
          for(const auto* jet : jets ){
            if (PhysicsUtilities::absDeltaPhi(*jet,*probe) < TMath::PiOver2()) {
              htalong += jet->pt();
              if(PhysicsUtilities::deltaR(*jet,*tag  ) < .4) cleanHttag   = true;
              if(PhysicsUtilities::deltaR(*jet,*probe) < .4) cleanHtprobe = true;
            }
          }
          if(cleanHttag)   htalong -= tag->pt();
          if(cleanHtprobe) htalong -= pt; // probe pt

          // fill tree
          ++matchnum;
          outTree_->Fill();

          // NB: at the moment if there are multiple T-P pairs in an event, we save all of them. Might want to try the other options
        }
      }

    }

};

// remember to set the correct leptonId (11 == electrons, 13 == muons)
void produceTnPTree(TString sname = "dyjets",
                    const int fileindex = 0,
                    const bool isMC = true,
                    const TString fname = "/store/user/jzabel/13TeV/110616/merged/dy1jetstoll-m50-madgraph_1_ntuple_postproc.root",
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

  ////cfgSet::loadDefaultConfigurations();
  ////cfgSet::setJSONFile(TString::Format("%s/src/data/JSON/%s", getenv("CMSSW_BASE"), json.Data()));
  ////cfgSet::ConfigSet cfg = cfgSet::zl_search_set;
  cfgSet::ConfigSet cfg = pars1LCR(json); // match this lep config with the region we apply these corrections: 1LCR now.
  a.goodLeptonConfig_ = cfg;

  TString treeName = "Events";
  TnPAnalyzer a(fullname, treeName, outfilename, isMC, &cfg);

  // defines the lepton selection
  ////a.goodLeptonConfig_ = cfgSet::zl_sel_leptons;

  //a.is50ns_ = fname.Contains("50ns") || json.Contains("50ns");
  a.is50ns_ = false;

  // lepton flavour
  a.lepId_ = leptonId;

  a.analyze(10000);

}
