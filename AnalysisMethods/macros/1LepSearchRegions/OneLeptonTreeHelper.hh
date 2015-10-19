#ifndef ONELEPTONTREEHELPER_HH
#define ONELEPTONTREEHELPER_HH

#include "AnalysisBase/TreeAnalyzer/interface/TreeCopier.h"
#include "AnalysisTools/DataFormats/interface/CMSTop.h"
#include "AnalysisTools/Utilities/interface/JetFlavorInfo.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
#include "AnalysisTools/KinematicVariables/interface/JetKinematics.h"
#include "AnalysisTools/KinematicVariables/interface/Topness.h"
#include "AnalysisTools/KinematicVariables/interface/mt2w.h"
#include "AnalysisTools/KinematicVariables/interface/chi2.h"
#include "AnalysisBase/TreeAnalyzer/interface/DefaultProcessing.h"
#include "AnalysisTools/TreeReader/interface/Defaults.h" // CSV_MEDIUM
#include "Math/VectorUtil.h"

using namespace ucsbsusy;

// Adjustments to default configuration
cfgSet::ConfigSet pars1lep(TString json) {
  cfgSet::loadDefaultConfigurations();
  cfgSet::setJSONFile(json);
  cfgSet::ConfigSet cfg = cfgSet::ol_search_set;
  //cfg.jets.cleanJetsvVetoedLeptons = true;
  return cfg;

}

struct TreeFiller {

  TreeFiller() {}

  Topness *tNess = new Topness();
  TopnessInformation *tNessInfo = new TopnessInformation();

  size i_run       ;
  size i_lumi      ;
  size i_event     ;
  size i_weight    ;
  size i_puWeight  ;
  size i_pu50NSWeight  ;
  size i_btagWeight ;
  size i_passtrige ;
  size i_passtrigmu;
  size i_passjson  ;
  size i_genmet    ;
  size i_ngenleps  ;
  size i_ngeneles  ;
  size i_ngenmus   ;
  size i_ngentaus  ;
  size i_met       ;
  size i_metnohf   ;
  size i_npv       ;
  size i_nvetotau  ;
  size i_nvetolep  ;
  size i_leppt     ;
  size i_lepeta    ;
  size i_leppdgid  ;
  size i_njets     ;
  size i_nbjets    ;
  size i_ht        ;
  size i_mt        ;
  size i_dphilw    ;
  size i_mt2w      ;
  size i_hadchi2   ;
  size i_topness   ;
  size i_dphij1met ;
  size i_dphij2met ;
  size i_dphij12met;
  size i_j1pt      ;
  size i_j2pt      ;
  size i_j1eta     ;
  size i_j2eta     ;

  void book(TreeWriterData* data) {
    i_run        = data->add<unsigned int>("","run","i",0);
    i_lumi       = data->add<unsigned int>("","lumi","i",0);
    i_event      = data->add<unsigned int>("","event","i",0);
    i_weight     = data->add<float>("","weight","F",0);
    i_puWeight   = data->add<float>("","puWeight","F",0);
    i_pu50NSWeight   = data->add<float>("","pu50NSWeight","F",0);
    i_btagWeight = data->add<float>("","btagWeight","F",0);
    i_passtrige  = data->add<bool >("","passtrige","O",0);
    i_passtrigmu = data->add<bool >("","passtrigmu","O",0);
    i_passjson   = data->add<bool>("","passjson","O",0);
    i_genmet     = data->add<float>("","genmet","F",0);
    i_ngenleps   = data->add<int  >("","ngenleps","I",0);
    i_ngeneles   = data->add<int  >("","ngeneles","I",0);
    i_ngenmus    = data->add<int  >("","ngenmus","I",0);
    i_ngentaus   = data->add<int  >("","ngentaus","I",0);
    i_met        = data->add<float>("","met","F",0);
    i_metnohf    = data->add<float>("","metnohf","F",0);
    i_npv        = data->add<int  >("","npv","I",0);
    i_nvetotau   = data->add<int  >("","nvetotau","I",0);
    i_nvetolep   = data->add<int  >("","nvetolep","I",0);
    i_leppt      = data->add<float>("","leppt","F",0);
    i_lepeta     = data->add<float>("","lepeta","F",0);
    i_leppdgid   = data->add<int  >("","leppdgid","I",0);
    i_njets      = data->add<int  >("","njets","I",0);
    i_nbjets     = data->add<int  >("","nbjets","I",0);
    i_ht         = data->add<float>("","ht","F",0);
    i_mt         = data->add<float>("","mt","F",0);
    i_dphilw     = data->add<float>("","dphilw","F",0);
    i_mt2w       = data->add<float>("","mt2w","F",0);
    i_hadchi2    = data->add<float>("","hadchi2","F",0);
    i_topness    = data->add<float>("","topness","F",0);
    i_dphij1met  = data->add<float>("","dphij1met","F",0);
    i_dphij2met  = data->add<float>("","dphij2met","F",0);
    i_dphij12met = data->add<float>("","dphij12met","F",0);
    i_j1pt       = data->add<float>("","j1pt","F",0); 
    i_j2pt       = data->add<float>("","j2pt","F",0); 
    i_j1eta      = data->add<float>("","j1eta","F",0); 
    i_j2eta      = data->add<float>("","j2eta","F",0); 
  }

  void fillEventInfo(TreeWriterData* data, BaseTreeAnalyzer* ana) {
    data->fill<unsigned int>(i_run,   ana->run);
    data->fill<unsigned int>(i_lumi,  ana->lumi);
    data->fill<unsigned int>(i_event, ana->event);
    data->fill<float>(i_weight,     ana->weight);
    data->fill<float>(i_puWeight,    ana->eventCorrections.getPUWeight());
    data->fill<float>(i_pu50NSWeight,    ana->eventCorrections.get50NSPUWeight());
    data->fill<float>(i_btagWeight, ana->bTagCorrections.getBTagByEvtWeight());
    data->fill<bool >(i_passtrige,  ana->isMC() ? ana->triggerflag & kHLT_Ele32_eta2p1_WP75_Gsf : (ana->process==defaults::DATA_SINGLEEL ? ana->triggerflag & kHLT_Ele32_eta2p1_WPLoose_Gsf : false));
    data->fill<bool >(i_passtrigmu, ana->isMC() ? ana->triggerflag & kHLT_IsoTkMu24_eta2p1 : (ana->process==defaults::DATA_SINGLEMU ? ana->triggerflag & kHLT_IsoTkMu24_eta2p1 : false));
     bool hasJSON = ana->hasJSONFile(), isMC = ana->isMC(), passesLumi = ana->passesLumiMask();
    data->fill<bool>(i_passjson, ((!isMC) && (hasJSON) && (!passesLumi)) ? false : true);
    data->fill<float>(i_genmet,   ana->genmet->pt());
    data->fill<float>(i_met,      ana->met->pt());
    data->fill<float>(i_metnohf,  ana->metNoHF->pt());
    data->fill<int  >(i_npv,      ana->nPV);
    data->fill<int  >(i_nvetolep, ana->nVetoedLeptons);
    data->fill<int  >(i_nvetotau, ana->nVetoHPSTaus);
  }

  void fillGenInfo(TreeWriterData* data, vector<GenParticleF*> genparts) {
    int ngenleps = 0, ngeneles = 0, ngenmus = 0, ngentaus = 0;
    for(auto* p : genparts) {
      const GenParticleF * mom = 0;
      const GenParticleF * gmom = 0;
      if(p->numberOfMothers()==0) continue;
      mom = p->mother(0);
      if(mom->numberOfMothers()==0) continue;
      gmom = mom->mother(0);
      while(abs(gmom->pdgId()) == 24 && gmom->numberOfMothers() > 0)
        gmom = gmom->mother(0);
      if((abs(p->pdgId()) == 12 || abs(p->pdgId()) == 14 || abs(p->pdgId()) == 16) && abs(mom->pdgId()) == 24 && abs(gmom->pdgId()) == 6) {
        ngenleps++;
        if(abs(p->pdgId()) == 12) ngeneles++;
        if(abs(p->pdgId()) == 14) ngenmus++;
        if(abs(p->pdgId()) == 16) ngentaus++;
      }
    }
    data->fill<int  >(i_ngenleps, ngenleps);
    data->fill<int  >(i_ngeneles, ngeneles);
    data->fill<int  >(i_ngenmus,  ngenmus);
    data->fill<int  >(i_ngentaus, ngentaus);
  }

  void fillObjectInfo(TreeWriterData* data, vector<RecoJetF*> jets, vector<RecoJetF*> bjets, vector<LeptonF*> leptons, MomentumF* met) {
    data->fill<int  >(i_njets,  int(jets.size()));
    data->fill<int  >(i_nbjets, int(bjets.size()));
    data->fill<float>(i_ht,     JetKinematics::ht(jets, 30.0, 2.4));

    LeptonF* lep = leptons.at(0);
    MomentumF* W = new MomentumF(lep->p4() + met->p4());
    data->fill<float>(i_leppt,    lep->pt());
    data->fill<float>(i_lepeta,   lep->eta());
    data->fill<int  >(i_leppdgid, lep->pdgid());
    data->fill<float>(i_mt,       JetKinematics::transverseMass(*lep, *met));
    data->fill<float>(i_dphilw,   fabs(PhysicsUtilities::deltaPhi(*lep, *W)));

    // conversions required for mt2w, chi2
    vector<float> csvvec;         // vector of the csv's of jets
    vector<LorentzVector> lzjets; // jets converted to lorentzvectors
    vector<float> sigma;          // vector of the number 0.1
    vector<bool > btag;           // bool vector of btags of jets
  
    LorentzVector lepvec;         // convert lep to lorentzvector
    LorentzVector templz;
    lepvec=lep->p4();
  
    // calculate csv and jets conversions, sigma (jet resolution), btag vector
    for(unsigned int n = 0; n < jets.size(); ++n){
      csvvec.push_back(jets[n]->csv());
      templz = jets[n]->p4();         // convert from RecoJetF to LorentzVector
      lzjets.push_back(templz);
  
      sigma.push_back(0.1);       // per twiki, use flat jet resolution of %10
  
      bool bjet=false;
      if(jets[n]->csv() > defaults::CSV_MEDIUM){
        bjet=true;
      }
      btag.push_back(bjet);      
    }
    
    // form ibjets and inonbjets, which are indices into jets (Pieter's code)
    // ibjets is filled with indices of all the bjets
    // whereas inonbjets is constrained in size: ibjets.size+inonbjets.size()<=3
    vector<pair<double,int> > rankedJets(jets.size());
    for(unsigned int iJ =0; iJ < jets.size(); ++iJ){
      rankedJets[iJ].first = btag[iJ];
      rankedJets[iJ].second = iJ;
    }
    std::sort(rankedJets.begin(),rankedJets.end(),PhysicsUtilities::greaterFirst<double,int>());
    vector<int> ibjets;
    vector<int> inonbjets;
    for(unsigned int iJ =0; iJ < rankedJets.size(); ++iJ){
      if(rankedJets[iJ].first>defaults::CSV_MEDIUM) 
        ibjets.push_back(rankedJets[iJ].second);
      else {
	if (ibjets.size()<2 && ibjets.size()+inonbjets.size()<3) inonbjets.push_back(rankedJets[iJ].second);
      }
    }

    data->fill<float>(i_mt2w,    calculateMT2w(lzjets, ibjets, inonbjets, lepvec, met->pt(), met->phi()));
    data->fill<float>(i_topness, tNess->findMinTopnessConfiguration(leptons,jets,met,tNessInfo));
    data->fill<float>(i_hadchi2, calculateChi2(lzjets, sigma, btag));
  
    float dphij1met = 0.0, dphij2met = 0.0;
    float j1pt = 0.0, j1eta = 0.0;
    float j2pt = 0.0, j2eta = 0.0;
    if(jets.size() > 0) {
      dphij1met = fabs(PhysicsUtilities::deltaPhi(*jets[0], *met));
      j1pt      = jets[0]->pt();
      j1eta     = jets[0]->eta();      
      data->fill<float>(i_dphij1met,  dphij1met);
      data->fill<float>(i_j1pt,      j1pt);
      data->fill<float>(i_j1eta,     j1eta);
      if(jets.size() == 1)
        data->fill<float>(i_dphij12met, dphij1met);
    }
    if(jets.size() > 1) {
      dphij2met = fabs(PhysicsUtilities::deltaPhi(*jets[1], *met));
      j2pt      = jets[1]->pt();
      j2eta     = jets[1]->eta();
      data->fill<float>(i_dphij2met,  dphij2met);
      data->fill<float>(i_dphij12met, min(dphij1met,dphij2met));
      data->fill<float>(i_j2pt,     j2pt);
      data->fill<float>(i_j2eta,    j2eta);
    }

  }

};

class OneLeptonAnalyzer : public TreeCopierManualBranches {

  public :
    OneLeptonAnalyzer(TString fileName, TString treeName, TString outfileName, bool isMCTree, cfgSet::ConfigSet *pars) :
      TreeCopierManualBranches(fileName, treeName, outfileName, isMCTree, pars) {}

    const double metcut_    = 50.0 ;
    const int    minnjets_  = 4;
    const int    minnbjets_ = 1;

    TreeFiller filler;

    virtual ~OneLeptonAnalyzer() {}

    void book() {
      filler.book(&data);
    }

    void loadVariables() {
      load(cfgSet::EVTINFO);
      load(cfgSet::AK4JETS);
      load(cfgSet::ELECTRONS);
      load(cfgSet::MUONS);
      load(cfgSet::PFCANDS);
      load(cfgSet::TAUS);
      if(isMC()) load(cfgSet::GENPARTICLES);
    }

    bool fillEvent() {

      if(nSelLeptons != 1)    return false;
      if(nVetoedLeptons > 1)  return false;
      if(met->pt() < metcut_) return false;
      if(nJets < minnjets_)   return false;
      if(nBJets < minnbjets_) return false;
      if(!goodvertex)         return false;
      
      filler.fillEventInfo    (&data, this);
      if (isMC())
        filler.fillGenInfo    (&data, genParts);
      filler.fillObjectInfo   (&data, jets, bJets, selectedLeptons, met);
      return true;
    }

};

#endif
