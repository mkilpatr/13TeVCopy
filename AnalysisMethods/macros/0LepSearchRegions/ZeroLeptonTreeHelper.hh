#ifndef ZEROLEPTONTREEHELPER_HH
#define ZEROLEPTONTREEHELPER_HH

#include "AnalysisBase/TreeAnalyzer/interface/TreeCopier.h"
#include "AnalysisTools/DataFormats/interface/CMSTop.h"
#include "AnalysisTools/Utilities/interface/JetFlavorInfo.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
#include "AnalysisTools/KinematicVariables/interface/JetKinematics.h"
#include "AnalysisBase/TreeAnalyzer/interface/DefaultProcessing.h"

using namespace ucsbsusy;

// Adjustments to default configuration
cfgSet::ConfigSet pars0lep() {
  cfgSet::loadDefaultConfigurations();
  cfgSet::ConfigSet cfg = cfgSet::zl_search_set;
  return cfg;
}

cfgSet::ConfigSet pars0LepPhoton() {
  cfgSet::loadDefaultConfigurations();
  cfgSet::ConfigSet cfg = cfgSet::zl_photon_set;
  return cfg;
}


struct TreeFiller {

  TreeFiller() {}

  size i_run       ;
  size i_lumi      ;
  size i_event     ;
  size i_weight    ;
  size i_genmet    ;
  size i_bosonpt   ;
  size i_bosoneta  ;
  size i_met       ;
  size i_npv       ;
  size i_nvetotau  ;
  size i_nvetolep  ;
  size i_nsellep   ;
  size i_nctt      ;
  size i_ncttstd   ;
  size i_ngenjets  ;
  size i_ngenbjets ;
  size i_njets     ;
  size i_njets60   ;
  size i_nbjets    ;
  size i_ntbjets   ;
  size i_ht        ;
  size i_j1pt      ;
  size i_j1eta     ;
  size i_j2pt      ;
  size i_j2eta     ;
  size i_j3pt      ;
  size i_j3eta     ;
  size i_b1pt      ;
  size i_b1eta     ;
  size i_b2pt      ;
  size i_b2eta     ;
  size i_dphij1met ;
  size i_dphij2met ;
  size i_dphij12met;
  size i_dphij3met ;
  size i_mtb1met   ;
  size i_mtb2met   ;
  size i_mtcsv1met ;
  size i_mtcsv2met ;
  size i_mtb12met  ;
  size i_absdphilepw;
  size i_mtlepmet   ;


  bool passCTTSelection(CMSTopF* ctt) {
    return (ctt->fJMass() > 140.0 && ctt->fJMass() < 250.0 && ctt->minMass() > 50.0 && ctt->nSubJets() >= 3);
  }

  void rankedByCSV(vector<RecoJetF*> inJets, vector<RecoJetF*>& outJets) {
    outJets.clear();
    outJets.resize(inJets.size());
    vector<pair<double,int> > rankedJets(inJets.size());

    for(unsigned int iJ =0; iJ < inJets.size(); ++iJ){
      rankedJets[iJ].first = inJets[iJ]->csv();
      rankedJets[iJ].second = iJ;
    }

    std::sort(rankedJets.begin(),rankedJets.end(),PhysicsUtilities::greaterFirst<double,int>());
    for(unsigned int iJ =0; iJ < inJets.size(); ++iJ) {
      outJets[iJ] = inJets[rankedJets[iJ].second];
    }
  }

  void book(TreeWriterData* data) {
    i_run        = data->add<unsigned int>("","run","i",0);
    i_lumi       = data->add<unsigned int>("","lumi","i",0);
    i_event      = data->add<unsigned int>("","event","i",0);
    i_weight     = data->add<float>("","weight","F",0);
    i_genmet     = data->add<float>("","genmet","F",0);
    i_bosonpt    = data->add<float>("","bosonpt","F",0);
    i_bosoneta   = data->add<float>("","bosoneta","F",0);
    i_met        = data->add<float>("","met","F",0);
    i_npv        = data->add<int  >("","npv","I",0);
    i_nvetotau   = data->add<int  >("","nvetotau","I",0);
    i_nvetolep   = data->add<int  >("","nvetolep","I",0);
    i_nsellep    = data->add<int  >("","nsellep","I",0);
    i_nctt       = data->add<int  >("","nctt","I",0);
    i_ncttstd    = data->add<int  >("","ncttstd","I",0);
    i_ngenjets   = data->add<int  >("","ngenjets","I",0);
    i_ngenbjets  = data->add<int  >("","ngenbjets","I",0);
    i_njets      = data->add<int  >("","njets","I",0);
    i_njets60    = data->add<int  >("","njets60","I",0);
    i_nbjets     = data->add<int  >("","nbjets","I",0);
    i_ntbjets    = data->add<int  >("","ntbjets","I",0);
    i_ht         = data->add<float>("","ht","F",0);
    i_j1pt       = data->add<float>("","j1pt","F",0);
    i_j1eta      = data->add<float>("","j1eta","F",0);
    i_j2pt       = data->add<float>("","j2pt","F",0);
    i_j2eta      = data->add<float>("","j2eta","F",0);
    i_j3pt       = data->add<float>("","j3pt","F",0);
    i_j3eta      = data->add<float>("","j3eta","F",0);
    i_b1pt       = data->add<float>("","b1pt","F",0);
    i_b1eta      = data->add<float>("","b1eta","F",0);
    i_b2pt       = data->add<float>("","b2pt","F",0);
    i_b2eta      = data->add<float>("","b2eta","F",0);
    i_dphij1met  = data->add<float>("","dphij1met","F",0);
    i_dphij2met  = data->add<float>("","dphij2met","F",0);
    i_dphij12met = data->add<float>("","dphij12met","F",0);
    i_dphij3met  = data->add<float>("","dphij3met","F",0);
    i_mtb1met    = data->add<float>("","mtb1met","F",0);
    i_mtb2met    = data->add<float>("","mtb2met","F",0);
    i_mtcsv1met  = data->add<float>("","mtcsv1met","F",0);
    i_mtcsv2met  = data->add<float>("","mtcsv2met","F",0);
    i_mtb12met   = data->add<float>("","mtb12met","F",0);
    i_absdphilepw = data->add<float>("","absdphilepw","F",0);
    i_mtlepmet    = data->add<float>("","mtlepmet","F",0);
  }

  void fillEventInfo(TreeWriterData* data, BaseTreeAnalyzer* ana, int randomLepton = 0, bool lepAddedBack = false, MomentumF* metn = 0) {
    data->fill<unsigned int>(i_run, ana->run);
    data->fill<unsigned int>(i_lumi, ana->lumi);
    data->fill<unsigned int>(i_event, ana->event);
    data->fill<float>(i_weight, ana->weight);
    data->fill<float>(i_genmet, ana->genmet->pt());
    if (!lepAddedBack)
      data->fill<float>(i_met, ana->met->pt());
    else
      data->fill<float>(i_met, metn->pt());

    data->fill<int  >(i_npv, ana->nPV);
    data->fill<int  >(i_nvetotau, ana->nVetoedTracks);
    data->fill<int  >(i_nvetolep, ana->nVetoedLeptons);
    data->fill<int  >(i_nsellep, ana->nSelLeptons);
    data->fill<int  >(i_nctt, ana->cttTops.size());
    int ncttstd = 0;
    for(auto* ctt : ana->cttTops) {
      if(passCTTSelection(ctt)) ncttstd++;
    }
    data->fill<int  >(i_ncttstd, ncttstd);
    if(ana->nSelLeptons > randomLepton) {
      MomentumF* lep = new MomentumF(ana->selectedLeptons.at(randomLepton)->p4());
      MomentumF* W = new MomentumF(lep->p4() + ana->met->p4());
      data->fill<float>(i_absdphilepw, fabs(PhysicsUtilities::deltaPhi(*W, *lep)) );
      data->fill<float>(i_mtlepmet, JetKinematics::transverseMass(*lep, *ana->met));
    }

  }

  void fillGenInfo(TreeWriterData* data, GenParticleF* boson, vector<GenJetF*> genjets, bool cleanjetsvboson = true) {
    data->fill<float>(i_bosonpt, boson->pt());
    data->fill<float>(i_bosoneta, boson->eta());
    int ngenjets = 0, ngenbjets = 0;
    for(auto* j : genjets) {
      if(cleanjetsvboson && PhysicsUtilities::deltaR2(*j, *boson) < 0.16) continue;
      ngenjets++;
      if(fabs(j->flavor()) == JetFlavorInfo::b_jet) ngenbjets++;
    }
    data->fill<int  >(i_ngenjets, ngenjets);
    data->fill<int  >(i_ngenbjets, ngenbjets);
  }

  void fillJetInfo(TreeWriterData* data, vector<RecoJetF*> jets, vector<RecoJetF*> bjets, MomentumF* met) {
    int njets60 = 0;
    for(auto* j : jets) {
      if(j->pt() > 60.0) njets60++;
    }
    int ntbjets = 0;
    for(auto* b : bjets) {
      if(b->csv() > defaults::CSV_TIGHT) ntbjets++;
    }
    data->fill<int  >(i_njets, int(jets.size()));
    data->fill<int  >(i_njets60, njets60);
    data->fill<int  >(i_nbjets, int(bjets.size()));
    data->fill<int  >(i_ntbjets, ntbjets);
    data->fill<float>(i_ht, JetKinematics::ht(jets, 20.0, 2.4));

    float dphij1met = 0.0, dphij2met = 0.0;
    if(jets.size() > 0) {
      data->fill<float>(i_j1pt, jets[0]->pt());
      data->fill<float>(i_j1eta, jets[0]->eta());
      dphij1met = fabs(PhysicsUtilities::deltaPhi(*jets[0], *met));
      data->fill<float>(i_dphij1met, dphij1met);
      if(jets.size() == 1)
        data->fill<float>(i_dphij12met, dphij1met);
    }
    if(jets.size() > 1) {
      data->fill<float>(i_j2pt, jets[1]->pt());
      data->fill<float>(i_j2eta, jets[1]->eta());
      dphij2met = fabs(PhysicsUtilities::deltaPhi(*jets[1], *met));
      data->fill<float>(i_dphij2met, dphij2met);
      data->fill<float>(i_dphij12met, min(dphij1met,dphij2met));
    }
    if(jets.size() > 2) {
      data->fill<float>(i_j3pt, jets[2]->pt());
      data->fill<float>(i_j3eta, jets[2]->eta());
      data->fill<float>(i_dphij3met, fabs(PhysicsUtilities::deltaPhi(*jets[2], *met)));
    }
    if(bjets.size() > 0) {
      data->fill<float>(i_b1pt, bjets[0]->pt());
      data->fill<float>(i_b1eta, bjets[0]->eta());
      data->fill<float>(i_mtb1met, JetKinematics::transverseMass(*bjets[0], *met));
    }
    if(bjets.size() > 1) {
      data->fill<float>(i_b2pt, bjets[1]->pt());
      data->fill<float>(i_b2eta, bjets[1]->eta());
      data->fill<float>(i_mtb2met, JetKinematics::transverseMass(*bjets[1], *met));
    }

    vector<RecoJetF*> jetsCSVranked;
    rankedByCSV(jets,jetsCSVranked);
    float mtcsv1met = 0.0, mtcsv2met = 0.0;
    if(jetsCSVranked.size() > 0) {
      mtcsv1met = JetKinematics::transverseMass(*jetsCSVranked[0], *met);
      data->fill<float>(i_mtcsv1met, mtcsv1met);
      if(jetsCSVranked.size() == 1)
        data->fill<float>(i_mtb12met, mtcsv1met);
    }
    if(jetsCSVranked.size() > 1) {
      mtcsv2met = JetKinematics::transverseMass(*jetsCSVranked[1], *met);
      data->fill<float>(i_mtcsv2met, mtcsv2met);
      data->fill<float>(i_mtb12met, min(mtcsv1met,mtcsv2met));
    }
  }

};

class ZeroLeptonAnalyzer : public TreeCopierManualBranches {

  public :
    ZeroLeptonAnalyzer(TString fileName, TString treeName, TString outfileName, bool isMCTree, cfgSet::ConfigSet *pars) :
      TreeCopierManualBranches(fileName, treeName, outfileName, isMCTree, pars) {}

    const double metcut_ = 200.0 ;

    TreeFiller filler;

    virtual ~ZeroLeptonAnalyzer() {}

    void book() {
      filler.book(&data);
    }

    bool fillEvent() {
      if(nVetoedLeptons > 0)  return false;
      if(nVetoedTracks > 0)   return false;
      if(met->pt() < metcut_) return false;
      if(!goodvertex)         return false;
      filler.fillEventInfo(&data, this);
      filler.fillJetInfo  (&data, jets, bJets, met);
      return true;
    }

};

#endif
