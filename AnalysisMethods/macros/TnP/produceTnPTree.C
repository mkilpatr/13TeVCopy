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
      phi(0),
      charge(0),
      pass(0),
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
    float phi;
    float charge;
    int   pass;
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
      outTree_->Branch("phi",&phi,"phi/F");
      outTree_->Branch("charge",&charge,"charge/I");
      outTree_->Branch("pass",&pass,"pass/I");
      // to be added: separate id and iso
      outTree_->Branch("passid",&passid,"passid/I");
      outTree_->Branch("passiso",&passiso,"passiso/I");
    }

    void loadVariables() {
      load(cfgSet::EVTINFO);
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

      // apply trigger
      if(lepId_ == 13 && !(triggerflag & kHLT_IsoMu24_eta2p1)) return;
      else if(lepId_ == 11) {
        if (!isMC() && !(triggerflag & kHLT_Ele32_eta2p1_WPLoose_Gsf)) return;
        else if (isMC() && !(triggerflag & kHLT_Ele32_eta2p1_WP75_Gsf)) return;
      }

      // tnp selection
      for(auto* tag : tnpLeptons_) {
        // tag has to be a good lepton
        if(tag->ismuon() && !cfgSet::isSelMuon(*(MuonF*)tag, goodLeptonConfig_)) continue;
        else if(tag->iselectron() && !cfgSet::isSelElectron(*(ElectronF*)tag, goodLeptonConfig_)) continue;

        // require trigger matching
        bool trigmatch = false;
        for(auto* to : triggerObjects) {
          if(lepId_ == 13 && (to->filterflags() & kSingleIsoMu24) && (to->pathflags() & kHLT_IsoMu24_eta2p1) && PhysicsUtilities::deltaR(*to,*tag) < maxTrigMatchDR_) {
            trigmatch = true;
            break;
          }
          else if(lepId_ == 11 && (to->filterflags() & kSingleEle32) && (isMC() ? (to->pathflags() & kHLT_Ele32_eta2p1_WP75_Gsf) : (to->pathflags() & kHLT_Ele32_eta2p1_WPLoose_Gsf)) && PhysicsUtilities::deltaR(*to,*tag) < maxTrigMatchDR_) {
            trigmatch = true;
            break;
          }
        }
        if(!trigmatch) continue;

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
          treeweight = is50ns_ ? weight*eventCorrections.get50NSPUWeight() : weight*eventCorrections.getPUWeight();

          mass    = dilep->mass();
          pt      = probe->pt();
	  eta     = probe->eta();
          abseta  = fabs(probe->eta());
          phi     = probe->phi();
          charge  = probe->q();

          // fill tree
          outTree_->Fill();

          // NB: at the moment if there are multiple T-P pairs in an event, we save all of them. Might want to try the other options
        }
      }

    }

};

// remember to set the correct leptonId (11 == electrons, 13 == muons)
void produceTnPTree(TString sname = "dyjets_el",
                    const int fileindex = 0,
                    const bool isMC = true,
                    const TString fname = "/store/user/ocolegro/13TeV/130815/merged/dyjetstoll_mll50-madgraphmlm-50ns_1_ntuple_postproc.root",
                    const double xsec = 1.0,
                    const TString outputdir = "trees",
                    const TString fileprefix = "root://cmseos:1094/",
                    const TString json="Cert_246908-255031_13TeV_PromptReco_Collisions15_50ns_JSON_v2.txt",
                    const int leptonId = 11)
{

  printf("Processing file %d of %s sample\n", (fileindex > -1 ? fileindex : 0), sname.Data());

  if(fileindex > -1)
    sname += TString::Format("_%d",fileindex);

  if(isMC)
    printf("Cross section: %5.2f pb\n", xsec);

  TString fullname = fileprefix+fname;

  gSystem->mkdir(outputdir,true);
  TString outfilename = outputdir+"/"+sname+"_tree.root";

  cfgSet::loadDefaultConfigurations();
  cfgSet::setJSONFile(TString::Format("%s/src/data/JSON/%s", getenv("CMSSW_BASE"), json.Data()));
  cfgSet::ConfigSet cfg = cfgSet::zl_search_set;

  TString treeName = "Events";
  TnPAnalyzer a(fullname, treeName, outfilename, isMC, &cfg);

  // defines the lepton selection
  a.goodLeptonConfig_ = cfgSet::zl_sel_leptons;

  a.is50ns_ = fname.Contains("50ns") || json.Contains("50ns");

  // lepton flavour
  a.lepId_ = leptonId;

  a.analyze(10000);

}
