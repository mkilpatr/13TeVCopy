#ifndef T2FBDHELPER_HH
#define T2FBDHELPER_HH

#define SLIM_VERSION

#include "AnalysisBase/TreeAnalyzer/interface/TreeCopier.h"
#include "AnalysisTools/DataFormats/interface/CMSTop.h"
#include "AnalysisTools/Utilities/interface/JetFlavorInfo.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
#include "AnalysisTools/KinematicVariables/interface/JetKinematics.h"
#include "AnalysisBase/TreeAnalyzer/interface/DefaultProcessing.h"
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"
#include <vector>
#include "Math/LorentzVector.h"
#include "Math/VectorUtil.h"
#include "AnalysisTools/KinematicVariables/interface/mt2w.h"

using namespace ucsbsusy;

// Adjustments to default configuration
cfgSet::ConfigSet pars0lep(TString json) {
  cfgSet::loadDefaultConfigurations();
  cfgSet::setJSONFile(json);
  cfgSet::ConfigSet cfg = cfgSet::zl_search_set;
  cfg.corrections.eventCorrections |= ucsbsusy::EventCorrectionSet::NORM;
  return cfg;
}

cfgSet::ConfigSet pars0lepCR(TString json) {
  cfgSet::loadDefaultConfigurations();
  cfgSet::setJSONFile(json);
  cfgSet::ConfigSet cfg = cfgSet::zl_lepton_set;
  cfg.corrections.eventCorrections |= ucsbsusy::EventCorrectionSet::NORM;
  return cfg;
}
cfgSet::ConfigSet pars0lepDiLepCR(TString json) {
  cfgSet::loadDefaultConfigurations();
  cfgSet::setJSONFile(json);
  cfgSet::ConfigSet cfg = cfgSet::zl_dilepton_set;
  cfg.corrections.eventCorrections |= ucsbsusy::EventCorrectionSet::NORM;
  return cfg;
}

cfgSet::ConfigSet pars0LepPhoton(TString json) {
  cfgSet::loadDefaultConfigurations();
  cfgSet::setJSONFile(json);
  cfgSet::ConfigSet cfg = cfgSet::zl_photon_set;
  cfg.corrections.eventCorrections |= ucsbsusy::EventCorrectionSet::NORM;
  return cfg;
}

struct TreeFiller {

  TreeFiller() {}
  size i_run              ;
  size i_lumi             ;
  size i_event            ;
  size i_process          ;
  size i_weight           ;
  size i_ismc             ;
  size i_passjson         ;
  size i_passmetmht90     ;
  size i_passtrigel       ;
  size i_passtrigmu       ;
  size i_truePUWeight     ;
  size i_btagWeight       ;
  size i_btagFastSimWeight;
  size i_qcdRespTailWeight;
  size i_lepvetoweight    ;
  size i_lepselweight     ;
  size i_leptnpweight     ;
  size i_passcscbeamhaloflt;
  size i_passeebadscflt   ;
  size i_passeebadsc4flt  ;
  size i_passhbheisoflt   ;
  size i_passhbhefltloose ;
  size i_passhbheflttight ;
  size i_passaddmetflts   ;
  size i_met         ;
  size i_nvetolep  ;
  size i_nvetotau  ;
  size i_nvetohpstaus;
  size i_npromptgentau;
  size i_njets     ;
  size i_nbjets    ;
  size i_nlbjets   ;
  size i_mht       ;
  size i_ht        ;
  size i_j1pt      ;
  size i_csvj1pt   ;
  size i_dphij1met ;
  size i_dphij2met ;
  size i_dphij3met ;
  size i_mtcsv12met;
  size i_leptonpt  ;
  size i_leptonpdgid;
  size i_mtlepmet  ;
  size i_metovsqrtht;
  size i_njl;
  size i_j1lpt;
  size i_dphij1lmet;
  size i_ljCHEF;

#ifndef SLIM_VERSION
  size i_systweights;
  size i_topptWeight;
  size i_wpolWeightUp;
  size i_wpolWeightDn;
  size i_costhetastar;
  size i_genmet    ;
  size i_metphi    ;
  size i_npv       ;
  size i_distoppt  ;
  size i_ditoppt   ;
  size i_ngoodgenmu;
  size i_ngoodgenele;
  size i_ngenjets  ;
  size i_ngenbjets ;
  size i_ngenbquarks ;
  size i_nmbjets   ;
  size i_ntbjets   ;
  size i_j1eta     ;
  size i_j2pt      ;
  size i_j2eta     ;
  size i_j3pt      ;
  size i_j3eta     ;
  size i_csvj1eta  ;
  size i_csvj2pt   ;
  size i_csvj2eta  ;
  size i_csv1      ;
  size i_csv2      ;
  size i_dphij12met;
  size i_dphij4met ;
  size i_mtcsv1met ;
  size i_mtcsv2met ;
  size i_dphicsv1met;
  size i_dphicsv2met;
  size i_dphicsv12met;
  size i_absdphilepw;
  size i_leptoneta ;
  size i_leptonq ;
  size i_genlepq ;
  size i_leptonmatchtrigmu;
  size i_leptonmatchtrige;
  size i_meff;
  size i_j2lpt;
  size i_dphij2lmet;
  size i_dphij1lj2l;
  size i_detaj1lj2l;
  size i_drj1lj2l;

  size i_genb1pt;
  size i_genb2pt;
  size i_genb1eta;
  size i_genb2eta;
  size i_dphigenb1genb2;
  size i_drgenb1genb2;
  size i_dphicsv1csv2;
  size i_drcsv1csv2;

  size i_ngenmu;
  size i_ngenel;
  size i_ngentau;
  size i_ngenlep;
  size i_genwpt;
  size i_lp;
#endif

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

  bool passCHFFilter(const vector<RecoJetF*>& jets) {
    bool pass = true;
    for(auto* jet : jets) {
      if(jet->pt() > 20.0 && fabs(jet->eta()) < 2.5 && (!jet->genJet() || jet->genJet()->pt() < 5.0) && jet->chHadFrac() < 0.1) pass = false;
    }
    return pass;
  }

  void book(TreeWriterData* data) {
    i_run               = data->add<unsigned int>("","run","i",0);
    i_lumi              = data->add<unsigned int>("","lumi","i",0);
    i_event             = data->add<unsigned int>("","event","i",0);
    i_process           = data->add<unsigned int>("","process","i",0);
    i_passjson          = data->add<bool>("","passjson","O",0);
    i_ismc              = data->add<bool >("","ismc","O",0);
    i_weight            = data->add<float>("","weight","F",0);
    i_truePUWeight      = data->add<float>("","truePUWeight","F",0);
    i_btagWeight        = data->add<float>("","btagWeight","F",0);
    i_btagFastSimWeight = data->add<float>("","btagFastSimWeight","F",0);
    i_qcdRespTailWeight = data->add<float>("","qcdRespTailWeight","F",0);
    i_lepvetoweight     = data->add<float>("","lepvetoweight","F",0);
    i_lepselweight      = data->add<float>("","lepselweight","F",0);
    i_leptnpweight      = data->add<float>("","leptnpweight","F",0);
    i_passmetmht90      = data->add<bool>("","passmetmht90","O",0);
    i_passtrigel        = data->add<bool>("","passtrigel","O",0);
    i_passtrigmu        = data->add<bool>("","passtrigmu","O",0);
    i_passcscbeamhaloflt = data->add<bool>("","passcscbeamhaloflt","O",0);
    i_passeebadscflt    = data->add<bool>("","passeebadscflt","O",0);
    i_passeebadsc4flt   = data->add<bool>("","passeebadsc4flt","O",0);
    i_passhbheisoflt    = data->add<bool>("","passhbheisoflt","O",0);
    i_passhbhefltloose  = data->add<bool>("","passhbhefltloose","O",0);
    i_passhbheflttight  = data->add<bool>("","passhbheflttight","O",0);
    i_passaddmetflts    = data->add<bool>("","passaddmetflts","O",0);
    i_met               = data->add<float>("","met","F",0);
    i_nvetolep          = data->add<int>("","nvetolep","I",0);
    i_nvetotau          = data->add<int>("","nvetotau","I",0);
    i_nvetohpstaus      = data->add<int>("","nvetohpstaus","I",0);
    i_npromptgentau     = data->add<int>("","npromptgentau","I",0);
    i_njets             = data->add<int>("","njets","I",0);
    i_nbjets            = data->add<int>("","nbjets","I",0);
    i_nlbjets           = data->add<int>("","nlbjets","I",0);
    i_mht               = data->add<float>("","mht","F",0);
    i_ht                = data->add<float>("","ht","F",0);
    i_j1pt              = data->add<float>("","j1pt","F",0);
    i_csvj1pt           = data->add<float>("","csvj1pt","F",0);
    i_dphij1met         = data->add<float>("","dphij1met","F",0);
    i_dphij2met         = data->add<float>("","dphij2met","F",0);
    i_dphij3met         = data->add<float>("","dphij3met","F",3);
    i_mtcsv12met        = data->add<float>("","mtcsv12met","F",0);
    i_leptonpt          = data->add<float>("","leptonpt","F",0);
    i_leptonpdgid       = data->add<int>("","leptonpdgid","I",0);
    i_mtlepmet          = data->add<float>("","mtlepmet","F",0);
    i_metovsqrtht       = data->add<float>("","metovsqrtht","F",0);
    i_njl               = data->add<int>("","njl","I",0);
    i_j1lpt             = data->add<float>("","j1lpt","F",0);
    i_dphij1lmet        = data->add<float>("","dphij1lmet","F",0);
    i_ljCHEF            = data->add<float>("","ljCHEF","F",2);

#ifndef SLIM_VERSION
    i_systweights       = data->addMulti<float>("","systweights",0);
    i_topptWeight       = data->add<float>("","topptWeight","F",1);
    i_wpolWeightUp      = data->add<float>("","wpolWeightUp","F",1);
    i_wpolWeightDn      = data->add<float>("","wpolWeightDn","F",1);
    i_costhetastar      = data->add<float>("","costhetastar","F",-1);
    i_genmet            = data->add<float>("","genmet","F",0);
    i_metphi            = data->add<float>("","metphi","F",0);
    i_npv               = data->add<int>("","npv","I",0);
    i_distoppt          = data->add<float>("","distoppt","F",-1.);
    i_ditoppt           = data->add<float>("","ditoppt","F",-1.);
    i_ngoodgenmu        = data->add<int>("","ngoodgenmu","I",0);
    i_ngoodgenele       = data->add<int>("","ngoodgenele","I",0);
    i_ngenjets          = data->add<int>("","ngenjets","I",0);
    i_ngenbjets         = data->add<int>("","ngenbjets","I",0);
    i_ngenbquarks       = data->add<int>("","ngenbquarks","I",0);
    i_nmbjets           = data->add<int>("","nmbjets","I",0);
    i_ntbjets           = data->add<int>("","ntbjets","I",0);
    i_j1eta             = data->add<float>("","j1eta","F",0);
    i_j2pt              = data->add<float>("","j2pt","F",0);
    i_j2eta             = data->add<float>("","j2eta","F",0);
    i_j3pt              = data->add<float>("","j3pt","F",0);
    i_j3eta             = data->add<float>("","j3eta","F",0);
    i_csvj1eta          = data->add<float>("","csvj1eta","F",0);
    i_csvj2pt           = data->add<float>("","csvj2pt","F",0);
    i_csvj2eta          = data->add<float>("","csvj2eta","F",0);
    i_csv1              = data->add<float>("","csv1","F",0);
    i_csv2              = data->add<float>("","csv2","F",0);
    i_dphij12met        = data->add<float>("","dphij12met","F",0);
    i_dphij4met         = data->add<float>("","dphij4met","F",3);
    i_mtcsv1met         = data->add<float>("","mtcsv1met","F",0);
    i_mtcsv2met         = data->add<float>("","mtcsv2met","F",0);
    i_dphicsv1met       = data->add<float>("","dphicsv1met","F",0);
    i_dphicsv2met       = data->add<float>("","dphicsv2met","F",0);
    i_dphicsv12met      = data->add<float>("","dphicsv12met","F",0);
    i_absdphilepw    = data->add<float>("","absdphilepw","F",0);
    i_leptoneta         = data->add<float>("","leptoneta","F",0);
    i_leptonq           = data->add<float>("","leptonq","F",0);
    i_genlepq         = data->add<float>("","genlepq","F",0);
    i_leptonmatchtrigmu = data->add<bool>("","leptonmatchtrigmu","O",0);
    i_leptonmatchtrige  = data->add<bool>("","leptonmatchtrige","O",0);
    i_meff           = data->add<float>("","meff","F",0);
    i_j2lpt          = data->add<float>("","j2lpt","F",0);
    i_dphij2lmet     = data->add<float>("","dphij2lmet","F",0);
    i_dphij1lj2l     = data->add<float>("","dphij1lj2l","F",0);
    i_detaj1lj2l     = data->add<float>("","detaj1lj2l","F",0);
    i_drj1lj2l       = data->add<float>("","drj1lj2l","F",0);
    i_genb1pt        = data->add<float>("","genb1pt","F",0);
    i_genb2pt        = data->add<float>("","genb2pt","F",0);
    i_genb1eta       = data->add<float>("","genb1eta","F",0);
    i_genb2eta       = data->add<float>("","genb2eta","F",0);
    i_dphigenb1genb2 = data->add<float>("","dphigenb1genb2","F",0);
    i_drgenb1genb2   = data->add<float>("","drgenb1genb2","F",0);
    i_dphicsv1met    = data->add<float>("","dphicsv1met","F",0);
    i_dphicsv1met    = data->add<float>("","dphicsv1met","F",0);
    i_dphicsv1csv2   = data->add<float>("","dphicsv1csv2","F",0);
    i_drcsv1csv2     = data->add<float>("","drcsv1csv2","F",0);
    i_ngenmu         = data->add<int>("","ngenmu","I",0);
    i_ngenel         = data->add<int>("","ngenel","I",0);
    i_ngentau        = data->add<int>("","ngentau","I",0);
    i_ngenlep        = data->add<int>("","ngenlep","I",0);
    i_genwpt         = data->add<float>("","genwpt","F",-1);
    i_lp             = data->add<float>("","lp","F",-9);
#endif
  }

  void fillEventInfo(TreeWriterData* data, BaseTreeAnalyzer* ana, bool lepAddedBack = false, MomentumF* metn = 0) {
    data->fill<unsigned int>(i_run, ana->run);
    data->fill<unsigned int>(i_lumi, ana->lumi);
    data->fill<unsigned int>(i_event, ana->event);
    data->fill<unsigned int>(i_process, ana->process);
    data->fill<bool >(i_ismc, ana->isMC());
    bool hasJSON = ana->hasJSONFile();
    bool isMC = ana->isMC();
    bool passesLumi = ana->passesLumiMask();
    data->fill<bool>(i_passjson, ((!isMC) && (hasJSON) && (!passesLumi)) ? false : true);
    data->fill<float>(i_weight, ana->weight);
    data->fill<float>(i_truePUWeight,    ana->eventCorrections.getTruePUWeight());
    data->fill<float>(i_btagWeight, ana->bTagCorrections.getBTagByEvtWeight());
    data->fill<float>(i_btagFastSimWeight, ana->bTagCorrections.getBTagFastSimByEvtWeight());
    data->fill<float>(i_qcdRespTailWeight, ana->jetAndMETCorrections.getQCDRespTailWeight());
    data->fill<float>(i_lepvetoweight, ana->leptonCorrections.getVetoLepWeight());
    data->fill<float>(i_lepselweight, ana->leptonCorrections.getSelLepWeight());
    data->fill<float>(i_leptnpweight, ana->leptonCorrections.getTnPLepWeight());

    // the met filters
//    data->fill<bool>(i_passcscflt,ana->evtInfoReader.cscFlt);
    data->fill<bool>(i_passcscbeamhaloflt, ana->evtInfoReader.cscBeamHaloFlt);
    data->fill<bool>(i_passeebadscflt,ana->evtInfoReader.eeBadSCFlt);
    data->fill<bool>(i_passeebadsc4flt,ana->evtInfoReader.eeBadSC4Flt);
    data->fill<bool>(i_passhbheisoflt,ana->evtInfoReader.hbheIsoFlt);
    data->fill<bool>(i_passhbhefltloose,ana->evtInfoReader.hbheFltR2Loose);
    data->fill<bool>(i_passhbheflttight,ana->evtInfoReader.hbheFltR2Tight);
    data->fill<bool>(i_passaddmetflts,(ana->isMC() || (ana->evtInfoReader.badResolutionTrkFlt && ana->evtInfoReader.muonBadTrkFlt)));

    data->fill<bool >(i_passmetmht90, ana->isMC() ? true : (ana->process==defaults::DATA_MET ? ana->triggerflag & kHLT_PFMET90_PFMHT90_IDTight : false));
    data->fill<bool >(i_passtrigel, (ana->isMC() || ana->process==defaults::DATA_SINGLEEL) ? (ana->triggerflag & kHLT_Ele22_eta2p1_WPLoose_Gsf) || (ana->triggerflag & kHLT_Ele22_eta2p1_WP75_Gsf) : false);
    data->fill<bool >(i_passtrigmu, (ana->isMC() || ana->process==defaults::DATA_SINGLEMU) ? ((ana->triggerflag & kHLT_IsoMu20) || (ana->triggerflag & kHLT_IsoTkMu20)): false);

    //CH energy fraction filter
    data->fill<float >(i_ljCHEF, ana->defaultJets->jetchHadEnFrac_->size() ? ana->defaultJets->jetchHadEnFrac_->at(ana->defaultJets->recoJets[0].index())  :10);

    if(!lepAddedBack) {
      data->fill<float>(i_met, ana->met->pt());
#ifndef SLIM_VERSION
      data->fill<float>(i_metphi, ana->met->phi());
#endif
    } else {
      data->fill<float>(i_met, metn->pt());
#ifndef SLIM_VERSION
      data->fill<float>(i_metphi, metn->phi());
#endif
    }
    data->fill<int  >(i_nvetotau, ana->nVetoedTracks);
    data->fill<int  >(i_nvetohpstaus,ana->nVetoHPSTaus);
    data->fill<int  >(i_nvetolep, ana->nSelLeptons);

    if(ana->selectedLepton) {
      const auto * lep = ana->selectedLepton;
      data->fill<float>(i_leptonpt, lep->pt());
      data->fill<int  >(i_leptonpdgid, lep->pdgid());
      data->fill<float>(i_mtlepmet, JetKinematics::transverseMass(*lep, *ana->met));
#ifndef SLIM_VERSION
      auto WP4 = lep->p4() + ana->met->p4();
      data->fill<float>(i_absdphilepw, fabs(PhysicsUtilities::deltaPhi(WP4, *lep)) );
      data->fill<float>(i_leptoneta, lep->eta());
      data->fill<float>(i_leptonq, lep->q());

      float lp_ = -9.;
      if (lepAddedBack) { lp_ = ( ((lep->px())*(ana->met->px()))+((lep->py())*(ana->met->py())) )/( (ana->met->pt())*(ana->met->pt()) ); }
      else              { lp_ = ( ((lep->px())*(WP4.px()))+((lep->py())*(WP4.py())) )/( (WP4.pt())*(WP4.pt()) ); }
      data->fill<float>(i_lp,lp_);

      bool matchtrigmu = false, matchtrige = false;
      for(auto* to : ana->triggerObjects) {
        if((to->filterflags() & kSingleIsoMu22) && (to->pathflags() & kHLT_IsoMu22)) {
          if(PhysicsUtilities::deltaR(*lep, *to) < 0.05) {
            matchtrigmu = true;
          }
        }
        if((to->filterflags() & kSingleEle22) && (to->pathflags() & kHLT_Ele22_eta2p1_WPLoose_Gsf)) {
          if(PhysicsUtilities::deltaR(*lep, *to) < 0.05) {
            matchtrige = true;
          }
        }
      }
      data->fill<bool >(i_leptonmatchtrigmu, matchtrigmu);
      data->fill<bool >(i_leptonmatchtrige, matchtrige);
#endif
    }

#ifndef SLIM_VERSION
    for(auto wgt : *ana->evtInfoReader.systweights) {
      data->fillMulti<float>(i_systweights, wgt/ana->evtInfoReader.lhecentralweight);
    }
    data->fill<float>(i_topptWeight, ana->ttbarCorrections.getTopPTWeight());
    data->fill<float>(i_wpolWeightUp, ana->wpolCorrections.getWpolWeightUp());
    data->fill<float>(i_wpolWeightDn, ana->wpolCorrections.getWpolWeightDn());
    data->fill<float>(i_costhetastar, ana->wpolCorrections.getCosThetaStar());
    data->fill<float>(i_genmet, ana->genmet->pt());
    data->fill<int  >(i_npv, ana->nPV);
#endif

    if(isMC) {
      int nGoodGenMu = 0; int nGoodGenEle = 0; int nPromptTaus = 0;
      int ngenmu_ = 0; int ngenel_ = 0; int ngentau_ = 0; int ngenlep_ = 0;
      int genlepq_ = -9.;
      GenParticleF * stop1 = 0;
      GenParticleF * stop2 = 0;
      std::vector<LorentzVector> genb;   genb.clear();
      std::vector<LorentzVector> gent;   gent.clear();
      std::vector<LorentzVector> genw;   genw.clear();
      for(auto* p : ana->genParts) {
        if(ParticleInfo::isA(ParticleInfo::p_stop1, p) and !stop1)
          stop1 = p;
        else if(ParticleInfo::isA(ParticleInfo::p_stop1, p) and !stop2)
          stop2 = p;
        if (abs(p->pdgId())==5) { genb.push_back((LorentzVector)p->p4()); }
        if (abs(p->pdgId())==6) { gent.push_back((LorentzVector)p->p4()); }

        const GenParticleF * genPartMom = 0;
        if (p->numberOfMothers()>0)
          genPartMom = p->mother(0);
        else
          continue;

        if ( (abs(p->pdgId())==24) )  { genw.push_back((LorentzVector)p->p4()); }
        if ( (abs(p->pdgId())==11) && (abs(genPartMom->pdgId())==24) ) { ++ngenel_;  ++ngenlep_; genlepq_=-1.*((p->pdgId())/(abs(p->pdgId()))); }
	if ( (abs(p->pdgId())==13) && (abs(genPartMom->pdgId())==24) ) { ++ngenmu_;  ++ngenlep_; genlepq_=-1.*((p->pdgId())/(abs(p->pdgId()))); }
	if ( (abs(p->pdgId())==15) && (abs(genPartMom->pdgId())==24) ) { ++ngentau_; ++ngenlep_; genlepq_=-1.*((p->pdgId())/(abs(p->pdgId()))); }
	
        if (ParticleInfo::isA(ParticleInfo::p_eminus, p) && (ParticleInfo::isA(ParticleInfo::p_Z0, genPartMom) || ParticleInfo::isA(ParticleInfo::p_Wplus, genPartMom) || ParticleInfo::isA(ParticleInfo::p_tauminus, genPartMom)))
          nGoodGenEle++;
        else if (ParticleInfo::isA(ParticleInfo::p_muminus, p) && (ParticleInfo::isA(ParticleInfo::p_Z0, genPartMom) || ParticleInfo::isA(ParticleInfo::p_Wplus, genPartMom) || ParticleInfo::isA(ParticleInfo::p_tauminus, genPartMom)))
          nGoodGenMu++;
        else if (ParticleInfo::isA(ParticleInfo::p_tauminus, p) && (ParticleInfo::isA(ParticleInfo::p_Z0, genPartMom) || ParticleInfo::isA(ParticleInfo::p_Wplus, genPartMom))) {
          bool lepDecay = false;
          for(unsigned int itd = 0; itd < p->numberOfDaughters(); itd++) {
            const GenParticleF* dau = p->daughter(itd);
            if(ParticleInfo::isA(ParticleInfo::p_eminus, dau) || ParticleInfo::isA(ParticleInfo::p_muminus, dau)) lepDecay = true;
          }
          if(!lepDecay)
            nPromptTaus++;
        }
      }
      data->fill<int  >(i_npromptgentau, nPromptTaus);

#ifndef SLIM_VERSION
      if(stop1 && stop2) {
        MomentumF* distop = new MomentumF(stop1->p4() + stop2->p4());
        data->fill<float>(i_distoppt, distop->pt());
      }

      data->fill<int>(i_ngenel,ngenel_);
      data->fill<int>(i_ngenmu,ngenmu_);
      data->fill<int>(i_ngentau,ngentau_);
      data->fill<int>(i_ngenlep,ngenlep_);
      if (gent.size()>1) { data->fill<float>(i_ditoppt, (gent[0]+gent[1]).pt()); }
      if (genw.size()>0) { data->fill<float>(i_genwpt, genw[0].pt()); }
      data->fill<int>(i_genlepq,genlepq_);

      data->fill<int  >(i_ngenbquarks, genb.size());
      if (genb.size()>0) {
        data->fill<float>(i_genb1pt, genb[0].pt());
        data->fill<float>(i_genb1eta, genb[0].eta());
      }

      if (genb.size()>1) {
        data->fill<float>(i_genb2pt, genb[1].pt());
        data->fill<float>(i_genb2eta, genb[1].eta());
        data->fill<float>(i_dphigenb1genb2,fabs(PhysicsUtilities::deltaPhi(genb[0],genb[1])) );
        data->fill<float>(i_drgenb1genb2,  fabs(PhysicsUtilities::deltaR(genb[0],genb[1])) );
      }

      data->fill<int  >(i_ngoodgenmu, nGoodGenMu);
      data->fill<int  >(i_ngoodgenele, nGoodGenEle);
#endif

    }

  }

  void fillGenInfo(TreeWriterData* data, vector<GenJetF*> genjets) {
#ifndef SLIM_VERSION
    int ngenjets = 0, ngenbjets = 0;
    for(auto* j : genjets) {
      ngenjets++;
      if(fabs(j->flavor()) == JetFlavorInfo::b_jet) {
        ngenbjets++;
      }
    }
    data->fill<int  >(i_ngenjets, ngenjets);
    data->fill<int  >(i_ngenbjets, ngenbjets);
#endif
  }


  void fillJetInfo(TreeWriterData* data, vector<RecoJetF*> jets, vector<RecoJetF*> bjets, MomentumF* met) {

    int ntbjets = 0, nmbjets = 0, nlbjets = 0;
    int njl = 0;
    LorentzVector mht_; 
    for(auto* j : jets) {
      if(j->csv() < defaults::CSV_LOOSE)  njl++;
      if(j->csv() > defaults::CSV_LOOSE)  nlbjets++;
      if(j->csv() > defaults::CSV_MEDIUM) nmbjets++;
      if(j->csv() > defaults::CSV_TIGHT)  ntbjets++;

      if ( (j->pt()>30.) && (fabs(j->eta())<2.5) ) { mht_ += j->p4(); }
    }

    data->fill<int>(i_njets,    int(jets.size()));
    data->fill<int>(i_nbjets,   int(bjets.size()));
    data->fill<int>(i_nlbjets,  nlbjets);
    data->fill<int>(i_njl,      njl);
    data->fill<float>(i_mht,  mht_.pt());
    auto ht = JetKinematics::ht(jets, 20.0, 2.4);
    data->fill<float>(i_ht, ht);
    data->fill<float>(i_metovsqrtht, (met->pt())/(sqrt(ht)));

#ifndef SLIM_VERSION
    data->fill<int>(i_nmbjets,  nmbjets);
    data->fill<int>(i_ntbjets,  ntbjets);
    data->fill<float>(i_meff, ht + met->pt());
#endif

    // collection of jets that do not pass CSVL
    std::vector<LorentzVector> lvjl; 
    lvjl.clear();
    for(auto* j : jets) { if ( (j->pt()>20) && (j->csv() < defaults::CSV_LOOSE) ) { lvjl.push_back((LorentzVector)j->p4()); } }
    if (lvjl.size()>0) { 
      data->fill<float>(i_j1lpt,lvjl[0].pt()); 
      data->fill<float>(i_dphij1lmet,fabs(PhysicsUtilities::deltaPhi(lvjl[0],*met))); 
    }
#ifndef SLIM_VERSION
    if (lvjl.size()>1) {
      data->fill<float>(i_j2lpt,lvjl[1].pt());
      data->fill<float>(i_dphij2lmet,fabs(PhysicsUtilities::deltaPhi(lvjl[1],*met)));
      data->fill<float>(i_dphij1lj2l,fabs(PhysicsUtilities::deltaPhi(lvjl[0],lvjl[1])));
      data->fill<float>(i_detaj1lj2l,fabs(PhysicsUtilities::deltaEta(lvjl[0],lvjl[1])));
      data->fill<float>(i_drj1lj2l,fabs(PhysicsUtilities::deltaR(lvjl[0],lvjl[1])));
    }
#endif

    float dphij1met = 0.0, dphij2met = 0.0;
    if(jets.size() > 0) {
      data->fill<float>(i_j1pt, jets[0]->pt());
      dphij1met = fabs(PhysicsUtilities::deltaPhi(*jets[0], *met));
      data->fill<float>(i_dphij1met, dphij1met);
#ifndef SLIM_VERSION
      data->fill<float>(i_j1eta, jets[0]->eta());
      if(jets.size() == 1)
        data->fill<float>(i_dphij12met, dphij1met);
#endif
    }
    if(jets.size() > 1) {
      dphij2met = fabs(PhysicsUtilities::deltaPhi(*jets[1], *met));
      data->fill<float>(i_dphij2met, dphij2met);
#ifndef SLIM_VERSION
      data->fill<float>(i_j2pt, jets[1]->pt());
      data->fill<float>(i_j2eta, jets[1]->eta());
      data->fill<float>(i_dphij12met, min(dphij1met,dphij2met));
#endif
    }
    if(jets.size() > 2) {
      data->fill<float>(i_dphij3met, fabs(PhysicsUtilities::deltaPhi(*jets[2], *met)));
#ifndef SLIM_VERSION
      data->fill<float>(i_j3pt, jets[2]->pt());
      data->fill<float>(i_j3eta, jets[2]->eta());
#endif
    }

#ifndef SLIM_VERSION
    if(jets.size() > 3){
      data->fill<float>(i_dphij4met, fabs(PhysicsUtilities::deltaPhi(*jets[3], *met)));
    }
#endif

    vector<RecoJetF*> jetsCSVranked;
    rankedByCSV(jets,jetsCSVranked);

    float mtcsv1met = 0.0, mtcsv2met = 0.0,dphicsv1met = 0.0,dphicsv2met = 0.0;
    if(jetsCSVranked.size() > 0) {
      data->fill<float>(i_csvj1pt, jetsCSVranked[0]->pt());
      mtcsv1met = JetKinematics::transverseMass(*jetsCSVranked[0], *met);
#ifndef SLIM_VERSION
      dphicsv1met = fabs(PhysicsUtilities::deltaPhi(*jetsCSVranked[0], *met));
      data->fill<float>(i_mtcsv1met, mtcsv1met);
      data->fill<float>(i_csvj1eta, jetsCSVranked[0]->eta());
      data->fill<float>(i_csv1, jetsCSVranked[0]->csv());
      data->fill<float>(i_dphicsv1met, dphicsv1met);
#endif
      if(jetsCSVranked.size() == 1) {
        data->fill<float>(i_mtcsv12met, mtcsv1met);
#ifndef SLIM_VERSION
        data->fill<float>(i_dphicsv12met, dphicsv1met);
#endif
      }
    }

    if(jetsCSVranked.size() > 1) {
      mtcsv2met = JetKinematics::transverseMass(*jetsCSVranked[1], *met);
      data->fill<float>(i_mtcsv12met, min(mtcsv1met,mtcsv2met));
#ifndef SLIM_VERSION
      dphicsv2met = fabs(PhysicsUtilities::deltaPhi(*jetsCSVranked[1], *met));
      data->fill<float>(i_csvj2pt, jetsCSVranked[1]->pt());
      data->fill<float>(i_csvj2eta, jetsCSVranked[1]->eta());
      data->fill<float>(i_csv2, jetsCSVranked[1]->csv());
      data->fill<float>(i_mtcsv2met, mtcsv2met);
      data->fill<float>(i_dphicsv2met, dphicsv2met);
      data->fill<float>(i_dphicsv12met,min(dphicsv1met,dphicsv2met));
      data->fill<float>(i_dphicsv1csv2,fabs(PhysicsUtilities::deltaPhi(*jetsCSVranked[0], *jetsCSVranked[1])));
      data->fill<float>(i_drcsv1csv2,fabs(PhysicsUtilities::deltaR(*jetsCSVranked[0], *jetsCSVranked[1])));
#endif
    }
   
  }

};

class ZeroLeptonAnalyzer : public TreeCopierManualBranches {

  public :
    ZeroLeptonAnalyzer(TString fileName, TString treeName, TString outfileName, size randomSeed, bool isMCTree, cfgSet::ConfigSet *pars) :
      TreeCopierManualBranches(fileName, treeName, outfileName, randomSeed, isMCTree, pars) {
      zIsInvisible = true; if(fileName.Contains("dy")) zIsInvisible = false;
      if (fileName.Contains("/store/user/ocolegro/13TeV/030616/merged/ttbar-mg-onelep")){
        clog << "##############################################" << endl;
        clog << "######    Will correct ttbar-1l xsec   #######" << endl;
        clog << "##############################################" << endl;
        isOneLepTTbar = true;
      }
    }

    bool isOneLepTTbar = false;

    bool addlep2met = false;
    bool applyCHFFilter = false ;
    size i_origmet = 0;

    TreeFiller filler;

    virtual ~ZeroLeptonAnalyzer() {}

    void book() {
      filler.book(&data);
      i_origmet           = data.add<float>("","origmet","F",0);
    }

    void loadVariables()
    {
      load(cfgSet::EVTINFO);
      load(cfgSet::AK4JETS, JetReader::defaultOptions | (isMC() ? JetReader::LOADGEN : JetReader::NULLOPT) | JetReader::LOADJETEXTRA);
      load(cfgSet::ELECTRONS);
      load(cfgSet::MUONS);
      load(cfgSet::PHOTONS);
      load(cfgSet::TAUS);
      load(cfgSet::PFCANDS);
      load(cfgSet::CMSTOPS);
      load(cfgSet::AK8FATJETS);
      load(cfgSet::AK8PUPPIFATJETS);
      load(cfgSet::TRIGOBJS);
      if(isMC()) load(cfgSet::GENPARTICLES);
    }


    bool fillEvent() {

      if (isOneLepTTbar) weight *= 0.5;

      if(!goodvertex) return false;
      if(applyCHFFilter && !filler.passCHFFilter(jets)) return false;

      std::vector<LorentzVector> lvjl; 
      lvjl.clear();
      for(auto* j : jets) { if ( (j->pt()>20) && (j->csv() < defaults::CSV_LOOSE) ) { lvjl.push_back((LorentzVector)j->p4()); } }

      if (addlep2met) {
        if (nSelLeptons < 1    ) return false;
        if (nJets       < 2    ) return false;
        if (lvjl.size() < 1    ) return false;
        if (lvjl.at(0).pt()<200) return false;

        MomentumF lepplusmet;
        lepplusmet.setP4(met->p4() + selectedLepton->p4());
        if (lepplusmet.pt() < 200    ) return false;

        filler.fillEventInfo(&data, this, true, &lepplusmet);
        filler.fillJetInfo  (&data, jets, bJets, &lepplusmet);
        data.fill<float>(i_origmet, met->pt());
      }
      else {
	if (met->pt() < 200    ) return false;
	if (nJets     < 2      ) return false;
	if (lvjl.size()<1      ) return false;
	if (lvjl.at(0).pt()<200) return false;

        filler.fillEventInfo(&data, this);
        filler.fillJetInfo  (&data, jets, bJets, met);
      }

#ifndef SLIM_VERSION
      // fill gen jets
      vector<GenJetF*> genJets;
      for(auto &j : defaultJets->genJets){
        if(cfgSet::isSelGenJet(j,configSet.jets))
          genJets.push_back(&j);
      }
      filler.fillGenInfo  (&data, genJets);
#endif

      return true;
    }

};

#endif
