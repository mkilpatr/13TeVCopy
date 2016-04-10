#ifndef T2FBDHELPER_HH
#define T2FBDHELPER_HH

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

//std::vector<LorentzVector> topsPass;
struct TreeFiller {

  TreeFiller() {}
  size i_run              ;
  size i_lumi             ;
  size i_event            ;
  size i_weight           ;
  size i_ismc             ;
  size i_passjson         ;
  size i_passmetmht90     ;
  size i_passmetmht100    ;
  size i_prescalemetmht90 ;
  size i_prescalemetmht100;
  size i_passtrigel       ;
  size i_passtrigmu       ;
  size i_isfastsim        ;
  size i_truePUWeight     ;
  size i_normWeight       ;
  size i_btagWeight       ;
  size i_btagFastSimWeight;
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
  size i_genmet      ;
  size i_bosonpt     ;
  size i_bosoneta    ;
  size i_distoppt    ;
  size i_met         ;
  size i_metphi      ;
  size i_metnohf     ;
  size i_metnohfphi  ;
  size i_npv       ;
  size i_nvetolep  ;
  size i_nvetotau  ;
  size i_nvetohpstaus;
  size i_nvetomu   ;
  size i_nvetolele ;
  size i_ngoodgenmu;
  size i_ngoodgenele;
  size i_npromptgentau;
  size i_chhpt     ;
  size i_chheta    ;
  size i_chhdz     ;
  size i_chhmt     ;
  size i_chhdphimet;
  size i_chhtaudisc;
  size i_mupt      ;
  size i_mueta     ;
  size i_muhtalong ;
  size i_muannulus ;
  size i_elept     ;
  size i_eleeta    ;
  size i_elehtalong;
  size i_eleannulus;
  size i_trackpt   ;
  size i_tracketa  ;
  size i_trackhtalong;
  size i_taupt     ;
  size i_taueta    ;
  size i_tauhtalong;
  size i_ngenjets  ;
  size i_ngenbjets ;
  size i_njets     ;
  size i_njets30   ;
  size i_njets75   ;
  size i_nbjets    ;
  size i_nbjets30  ;
  size i_nlbjets   ;
  size i_nmbjets   ;
  size i_ntbjets   ;
  size i_mht       ;
  size i_ht        ;
  size i_ht30      ;
  size i_ht40      ;
  size i_j1pt      ;
  size i_j1eta     ;
  size i_j2pt      ;
  size i_j2eta     ;
  size i_j3pt      ;
  size i_j3eta     ;
  size i_csvj1pt   ;
  size i_csvj1eta  ;
  size i_csvj2pt   ;
  size i_csvj2eta  ;
  size i_csv1      ;
  size i_csv2      ;
  size i_dphij1met ;
  size i_dphij2met ;
  size i_dphij12met;
  size i_dphij3met ;
  size i_dphij4met ;
  size i_mtcsv1met ;
  size i_mtcsv2met ;
  size i_mtcsv12met;
  size i_dphicsv1met;
  size i_dphicsv2met;
  size i_dphicsv12met;
  size i_absdphilepw;
  size i_leptonpt  ;
  size i_leptoneta ;
  size i_leptonpdgid;
  size i_annulus   ;
  size i_leptonmatchtrigmu;
  size i_leptonmatchtrige;
  size i_mtlepmet  ;
  size i_lepton2pt ;
  size i_lepton2eta;
  size i_lepton2pdgid;
  size i_dileppt   ;
  size i_dilepeta  ;
  size i_dilepmass ;
  size i_gentop1pt;
  size i_gentop2pt;
  size i_meff;
  size i_dphitopmet;
  size i_metovsqrtht;
  size i_toppt;
  size i_httwoleadfatjet;

  size i_njl;
  size i_j1lpt;
  size i_j2lpt;
  size i_dphij1lmet;
  size i_dphij2lmet;
  size i_dphij1lj2l;
  size i_drj1lj2l;

  size i_genb1pt;
  size i_genb2pt;
  size i_genb1eta;
  size i_genb2eta;
  size i_dphigenb1genb2;
  size i_drgenb1genb2;
  size i_dphicsv1csv2;
  size i_drcsv1csv2;

  size i_blpt;
  size i_bmpt;
  size i_nonisrpt;

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
    i_run               = data->add<unsigned int>("","run","i",0);
    i_lumi              = data->add<unsigned int>("","lumi","i",0);
    i_event             = data->add<unsigned int>("","event","i",0);
    i_passjson          = data->add<bool>("","passjson","O",0);
    i_ismc              = data->add<bool >("","ismc","O",0);
    i_weight            = data->add<float>("","weight","F",0);
    i_truePUWeight      = data->add<float>("","truePUWeight","F",0);
    i_normWeight        = data->add<float>("","normWeight","F",0);
    i_btagWeight        = data->add<float>("","btagWeight","F",0);
    i_btagFastSimWeight = data->add<float>("","btagFastSimWeight","F",0);
    i_lepvetoweight     = data->add<float>("","lepvetoweight","F",0);
    i_lepselweight      = data->add<float>("","lepselweight","F",0);
    i_leptnpweight      = data->add<float>("","leptnpweight","F",0);
    i_passmetmht90      = data->add<bool>("","passmetmht90","O",0);
    i_passmetmht100     = data->add<bool>("","passmetmht100","O",0);
    i_prescalemetmht90  = data->add<bool>("","prescalemetmht90","O",0);
    i_prescalemetmht100 = data->add<bool>("","prescalemetmht100","O",0);
    i_passtrigel        = data->add<bool>("","passtrigel","O",0);
    i_passtrigmu        = data->add<bool>("","passtrigmu","O",0);
    i_isfastsim         = data->add<bool>("","isfastsim","O",0);
    i_passcscbeamhaloflt = data->add<bool>("","passcscbeamhaloflt","O",0);
    i_passeebadscflt    = data->add<bool>("","passeebadscflt","O",0);
    i_passeebadsc4flt   = data->add<bool>("","passeebadsc4flt","O",0);
    i_passhbheisoflt    = data->add<bool>("","passhbheisoflt","O",0);
    i_passhbhefltloose  = data->add<bool>("","passhbhefltloose","O",0);
    i_passhbheflttight  = data->add<bool>("","passhbheflttight","O",0);
    i_passaddmetflts    = data->add<bool>("","passaddmetflts","O",0);
    i_genmet            = data->add<float>("","genmet","F",0);
    i_bosonpt           = data->add<float>("","bosonpt","F",0);
    i_bosoneta          = data->add<float>("","bosoneta","F",0);
    i_distoppt          = data->add<float>("","distoppt","F",0);
    i_met               = data->add<float>("","met","F",0);
    i_metphi            = data->add<float>("","metphi","F",0);
    i_metnohf           = data->add<float>("","metnohf","F",0);
    i_metnohfphi        = data->add<float>("","metnohfphi","F",0);
    i_npv               = data->add<int>("","npv","I",0);
    i_nvetolep          = data->add<int>("","nvetolep","I",0);
    i_nvetotau          = data->add<int>("","nvetotau","I",0);
    i_nvetohpstaus      = data->add<int>("","nvetohpstaus","I",0);
    i_nvetomu           = data->add<int>("","nvetomu","I",0);
    i_nvetolele         = data->add<int>("","nvetolele","I",0);
    i_ngoodgenmu        = data->add<int>("","ngoodgenmu","I",0);
    i_ngoodgenele       = data->add<int>("","ngoodgenele","I",0);
    i_npromptgentau     = data->add<int>("","npromptgentau","I",0);
    i_chhpt             = data->addMulti<float>("","chhpt",0);
    i_chheta            = data->addMulti<float>("","chheta",0);
    i_chhdz             = data->addMulti<float>("","chhdz",0);
    i_chhmt             = data->addMulti<float>("","chhmt",0);
    i_chhdphimet        = data->addMulti<float>("","chhdphimet",0);
    i_chhtaudisc        = data->addMulti<float>("","chhtaudisc",0);
    i_mupt              = data->addMulti<float>("","mupt",0);
    i_mueta             = data->addMulti<float>("","mueta",0);
    i_muhtalong         = data->addMulti<float>("","muhtalong",0);
    i_muannulus         = data->addMulti<float>("","muannulus",0);
    i_elept             = data->addMulti<float>("","elept",0);
    i_eleeta            = data->addMulti<float>("","eleeta",0);
    i_elehtalong        = data->addMulti<float>("","elehtalong",0);
    i_eleannulus        = data->addMulti<float>("","eleannulus",0);
    i_trackpt           = data->addMulti<float>("","trackpt",0);
    i_tracketa          = data->addMulti<float>("","tracketa",0);
    i_trackhtalong      = data->addMulti<float>("","trackhtalong",0);
    i_taupt             = data->addMulti<float>("","taupt",0);
    i_taueta            = data->addMulti<float>("","taueta",0);
    i_tauhtalong        = data->addMulti<float>("","tauhtalong",0);
    i_ngenjets          = data->add<int>("","ngenjets","I",0);
    i_ngenbjets         = data->add<int>("","ngenbjets","I",0);
    i_njets             = data->add<int>("","njets","I",0);
    i_njets30           = data->add<int>("","njets30","I",0);
    i_njets75           = data->add<int>("","njets75","I",0);
    i_nbjets            = data->add<int>("","nbjets","I",0);
    i_nbjets30          = data->add<int>("","nbjets30","I",0);
    i_nlbjets           = data->add<int>("","nlbjets","I",0);
    i_nmbjets           = data->add<int>("","nmbjets","I",0);
    i_ntbjets           = data->add<int>("","ntbjets","I",0);
    i_mht               = data->add<float>("","mht","F",0);
    i_ht                = data->add<float>("","ht","F",0);
    i_ht30              = data->add<float>("","ht30","F",0);
    i_ht40              = data->add<float>("","ht40","F",0);
    i_j1pt              = data->add<float>("","j1pt","F",0);
    i_j1eta             = data->add<float>("","j1eta","F",0);
    i_j2pt              = data->add<float>("","j2pt","F",0);
    i_j2eta             = data->add<float>("","j2eta","F",0);
    i_j3pt              = data->add<float>("","j3pt","F",0);
    i_j3eta             = data->add<float>("","j3eta","F",0);
    i_csvj1pt           = data->add<float>("","csvj1pt","F",0);
    i_csvj1eta          = data->add<float>("","csvj1eta","F",0);
    i_csvj2pt           = data->add<float>("","csvj2pt","F",0);
    i_csvj2eta          = data->add<float>("","csvj2eta","F",0);
    i_csv1              = data->add<float>("","csv1","F",0);
    i_csv2              = data->add<float>("","csv2","F",0);
    i_dphij1met         = data->add<float>("","dphij1met","F",0);
    i_dphij2met         = data->add<float>("","dphij2met","F",0);
    i_dphij12met        = data->add<float>("","dphij12met","F",0);
    i_dphij3met         = data->add<float>("","dphij3met","F",3);
    i_dphij4met         = data->add<float>("","dphij4met","F",3);
    i_mtcsv1met         = data->add<float>("","mtcsv1met","F",0);
    i_mtcsv2met         = data->add<float>("","mtcsv2met","F",0);
    i_mtcsv12met        = data->add<float>("","mtcsv12met","F",0);
    i_dphicsv1met       = data->add<float>("","dphicsv1met","F",0);
    i_dphicsv2met       = data->add<float>("","dphicsv2met","F",0);
    i_dphicsv12met      = data->add<float>("","dphicsv12met","F",0);
    i_leptonpt          = data->add<float>("","leptonpt","F",0);
    i_leptoneta         = data->add<float>("","leptoneta","F",0);
    i_leptonpdgid       = data->add<int>("","leptonpdgid","I",0);
    i_annulus           = data->add<float>("","annulus","F",0);
    i_leptonmatchtrigmu = data->add<bool>("","leptonmatchtrigmu","O",0);
    i_leptonmatchtrige  = data->add<bool>("","leptonmatchtrige","O",0);
    i_mtlepmet       = data->add<float>("","mtlepmet","F",0);
    i_absdphilepw    = data->add<float>("","absdphilepw","F",0);
    i_lepton2pt      = data->add<float>("","lepton2pt","F",0);
    i_lepton2eta     = data->add<float>("","lepton2eta","F",0);
    i_lepton2pdgid   = data->add<int>("","lepton2pdgid","I",0);
    i_dileppt        = data->add<float>("","dileppt","F",0);
    i_dilepeta       = data->add<float>("","dilepeta","F",0);
    i_dilepmass      = data->add<float>("","dilepmass","F",0);
    i_gentop1pt      = data->add<float>("","gentoppt1","F",0);
    i_gentop2pt      = data->add<float>("","gentoppt2","F",0);
    i_meff           = data->add<float>("","meff","F",0);
    i_dphitopmet     = data->add<float>("","dphitopmet","F",0);
    i_metovsqrtht    = data->add<float>("","metovsqrtht","F",0);
    i_toppt          = data->add<float>("","toppt","F",0);
    i_httwoleadfatjet = data->add<float>("","httwoleadfatjet","F",0);

    i_njl            = data->add<int>("","njl","I",0);
    i_j1lpt          = data->add<float>("","j1lpt","F",0);
    i_j2lpt          = data->add<float>("","j2lpt","F",0);
    i_dphij1lmet     = data->add<float>("","dphij1lmet","F",0);
    i_dphij2lmet     = data->add<float>("","dphij2lmet","F",0);
    i_dphij1lj2l     = data->add<float>("","dphij1lj2l","F",0);
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

    i_blpt     = data->add<float>("","blpt","F",0);
    i_bmpt     = data->add<float>("","bmpt","F",0);
    i_nonisrpt = data->add<float>("","nonisrpt","F",0);

  }

  void fillEventInfo(TreeWriterData* data, BaseTreeAnalyzer* ana, bool lepAddedBack = false, MomentumF* metn = 0) {
    data->fill<unsigned int>(i_run, ana->run);
    data->fill<unsigned int>(i_lumi, ana->lumi);
    data->fill<unsigned int>(i_event, ana->event);
    data->fill<bool >(i_ismc, ana->isMC());
    data->fill<bool >(i_isfastsim,     ana->process==defaults::SIGNAL);
    bool hasJSON = ana->hasJSONFile();
    bool isMC = ana->isMC();
    bool passesLumi = ana->passesLumiMask();
    data->fill<bool>(i_passjson, ((!isMC) && (hasJSON) && (!passesLumi)) ? false : true);
    data->fill<float>(i_weight, ana->weight);
    data->fill<float>(i_truePUWeight,    ana->eventCorrections.getTruePUWeight());
    data->fill<float>(i_normWeight,  ana->eventCorrections.getNormWeight());
    data->fill<float>(i_btagWeight, ana->bTagCorrections.getBTagByEvtWeight());
    data->fill<float>(i_btagFastSimWeight, ana->bTagCorrections.getBTagFastSimByEvtWeight());
    data->fill<float>(i_lepvetoweight, ana->leptonCorrections.getVetoLepWeight());
    data->fill<float>(i_lepselweight, ana->leptonCorrections.getSelLepWeight());
    data->fill<float>(i_leptnpweight, ana->leptonCorrections.getTnPLepWeight());

    data->fill<bool >(i_passmetmht90     , ana->isMC() ? true : (ana->process==defaults::DATA_MET ? ana->triggerflag & kHLT_PFMET90_PFMHT90_IDTight : false));
    //    data->fill<bool >(i_prescalemetmht90 , ana->isMC() ? true : (ana->process==defaults::DATA_MET ? ana->triggerflag & kHLT_PFMET90_PFMHT90_IDTight : false));
    data->fill<bool >(i_passmetmht100, ana->isMC() ? true : (ana->process==defaults::DATA_MET ? ana->triggerflag & kHLT_PFMET100_PFMHT100_IDTight : false));
    data->fill<bool >(i_passtrigel,  ana->isMC() ? true : (ana->process==defaults::DATA_SINGLEEL ? ana->triggerflag & kHLT_Ele22_eta2p1_WPLoose_Gsf : false));
    data->fill<bool >(i_passtrigmu, ana->isMC() ? true : (ana->process==defaults::DATA_SINGLEMU ? ana->triggerflag & kHLT_IsoMu22 : false));

    // test prescale
    //    if ((ana->isMC()) && (ana->process==defaults::DATA_MET) && (ana->triggerflag & kHLT_PFMET90_PFMHT90_IDTight) ) { cout << ana->prescale() << "\n"; }
    bool prescale90_ = true; bool prescale100_ = true;
    for(auto* tI: ana->triggerInfo) {
      if (tI->trigflag()&kHLT_PFMET90_PFMHT90_IDTight)   { if (tI->prescale()==1) { prescale90_  = false; } }
      if (tI->trigflag()&kHLT_PFMET100_PFMHT100_IDTight) { if (tI->prescale()==1) { prescale100_ = false; } }
    }
    if (isMC) { prescale90_ = false; prescale100_ = false; }
    data->fill<bool>(i_prescalemetmht90,prescale90_);
    data->fill<bool>(i_prescalemetmht100,prescale100_);

    data->fill<float>(i_genmet, ana->genmet->pt());
    if(!lepAddedBack) {
      data->fill<float>(i_met, ana->met->pt());
      data->fill<float>(i_metphi, ana->met->phi());
    } else {
      data->fill<float>(i_met, metn->pt());
      data->fill<float>(i_metphi, metn->phi());
    }
    data->fill<float>(i_metnohf, ana->metNoHF->pt());
    data->fill<float>(i_metnohfphi, ana->metNoHF->phi());
    data->fill<int  >(i_npv, ana->nPV);
    data->fill<int  >(i_nvetotau, ana->nVetoedTracks);
    data->fill<int  >(i_nvetohpstaus,ana->nVetoHPSTaus);
    data->fill<int  >(i_nvetolep, ana->nSelLeptons);

    int nVetoEle = 0; int nVetoMu = 0;
    for(auto i: ana->selectedLeptons){
		  if(fabs(i->pdgid()) == 11) nVetoEle++;
		  if(fabs(i->pdgid()) == 13) nVetoMu++;
    }
    data->fill<int  >(i_nvetomu, nVetoMu);
    data->fill<int  >(i_nvetolele, nVetoEle);

    for(auto& pfc : ana->pfcandReader.pfcands) {
      if(!pfc.ischargedhadron() || pfc.pt() < 10.0 || fabs(pfc.eta()) > 2.4) continue;
      data->fillMulti<float>(i_chhpt, pfc.pt());
      data->fillMulti<float>(i_chheta, pfc.eta());
      data->fillMulti<float>(i_chhdz, pfc.dz());
      data->fillMulti<float>(i_chhmt, pfc.mt());
      data->fillMulti<float>(i_chhdphimet, fabs(pfc.dphimet()));
      data->fillMulti<float>(i_chhtaudisc, pfc.taudisc());
    } 

    if(ana->nSelLeptons > 0) {
      for(auto* l : ana->selectedLeptons) {
        if(l->ismuon()) {
          data->fillMulti<float>(i_mupt, l->pt());
          data->fillMulti<float>(i_mueta, l->eta());
          data->fillMulti<float>(i_muannulus, l->pt()*l->annulusactivity());
        } else {
          data->fillMulti<float>(i_elept, l->pt());
          data->fillMulti<float>(i_eleeta, l->eta());
          data->fillMulti<float>(i_eleannulus, l->pt()*l->annulusactivity());
        }
      }
    }

    if(ana->nVetoHPSTaus > 0) {
      for(const auto* tau : ana->vetoedTaus) {
        data->fillMulti<float>(i_taupt, tau->pt());
        data->fillMulti<float>(i_taueta, tau->eta());
      }
    }

    if(ana->selectedLepton) {
      const auto * lep = ana->selectedLepton;
      auto WP4 = lep->p4() + ana->met->p4();
      data->fill<float>(i_absdphilepw, fabs(PhysicsUtilities::deltaPhi(WP4, *lep)) );
      data->fill<float>(i_leptonpt, lep->pt());
      data->fill<float>(i_leptoneta, lep->eta());
      data->fill<int  >(i_leptonpdgid, lep->pdgid());
      data->fill<float>(i_annulus,lep->pt() *lep->annulusactivity());
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
      data->fill<float>(i_mtlepmet, JetKinematics::transverseMass(*lep, *ana->met));
      if(ana->nSelLeptons > 1) {
        const LeptonF * lep2 = 0;
        for(const auto * tLep : ana->selectedLeptons){
          if(tLep->index() == lep->index() && tLep->ismuon() == lep->ismuon()) continue;
          lep2 = tLep;
        }
        if(lep2==0)throw std::range_error("ZeroLeptonTreeHelper::fillEventInfo: You say you have at least two leptons but I only find one!");
        data->fill<float>(i_lepton2pt, lep2->pt());
        data->fill<float>(i_lepton2eta, lep2->eta());
        data->fill<int  >(i_lepton2pdgid, lep2->pdgid());
        auto dilepp4 = lep->p4() + lep2->p4();
        data->fill<float>(i_dileppt, dilepp4.pt());
        data->fill<float>(i_dilepeta, dilepp4.eta());
        data->fill<float>(i_dilepmass, dilepp4.mass());
      }
    }

    std::vector<float> gentoppt_; gentoppt_.clear();
    int nGoodGenMu = 0; int nGoodGenEle = 0; int nPromptTaus = 0;
    GenParticleF * stop1 = 0;
    GenParticleF * stop2 = 0;
    std::vector<LorentzVector> genb;   genb.clear();
    if(isMC) {
      for(auto* p : ana->genParts) {
        if(ParticleInfo::isA(ParticleInfo::p_stop1, p) and !stop1)
          stop1 = p;
        else if(ParticleInfo::isA(ParticleInfo::p_stop1, p) and !stop2)
          stop2 = p;
        const GenParticleF * genPartMom = 0;

	if (abs(p->pdgId())==6) { float tmppt_ = p->p4().pt(); gentoppt_.push_back(tmppt_); }
	if (abs(p->pdgId())==5) { genb.push_back((LorentzVector)p->p4()); }

        if (p->numberOfMothers()>0) 
          genPartMom = p->mother(0);
        else
          continue;
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
      if(stop1 && stop2) {
        MomentumF* distop = new MomentumF(stop1->p4() + stop2->p4());
        data->fill<float>(i_distoppt, distop->pt());
      }
    }
  
    //    data->fill<int  >(i_ngenjets, ngenjets);
    data->fill<int  >(i_ngenbjets, genb.size());
    if (genb.size()>0) {
      data->fill<float>(i_genb1pt, genb[0].pt());
      data->fill<float>(i_genb1eta, genb[0].eta());
    }

    if (genb.size()>1) {
      data->fill<float>(i_genb2pt, genb[1].pt());
      data->fill<float>(i_genb2eta, genb[1].eta());
      data->fill<float>(i_dphigenb1genb2,fabs(PhysicsUtilities::deltaPhi(genb[0],genb[1])) );
      data->fill<float>(i_dphigenb1genb2,fabs(PhysicsUtilities::deltaR(genb[0],genb[1])) );
    }

    data->fill<int  >(i_ngoodgenmu, nGoodGenMu);
    data->fill<int  >(i_ngoodgenele, nGoodGenEle);
    data->fill<int  >(i_npromptgentau, nPromptTaus);


    float gentop1pt_ = 0.;
    float gentop2pt_ = 0.;
    if (gentoppt_.size()>1) {
      if (gentoppt_[0]>=gentoppt_[1]) { gentop1pt_ = gentoppt_[0]; gentop2pt_ = gentoppt_[1]; }
      else                            { gentop1pt_ = gentoppt_[1]; gentop2pt_ = gentoppt_[0]; }
    }

    data->fill<float>(i_gentop1pt, gentop1pt_);
    data->fill<float>(i_gentop2pt, gentop2pt_);

  }

  void fillGenInfo(TreeWriterData* data, GenParticleF* boson, vector<GenJetF*> genjets, bool cleanjetsvboson = true) {
    data->fill<float>(i_bosonpt, boson ? boson->pt() : 0.0);
    data->fill<float>(i_bosoneta, boson ? boson->eta() : 0.0);
    int ngenjets = 0, ngenbjets = 0;
    std::vector<LorentzVector> genb;   genb.clear();
    for(auto* j : genjets) {
      if(cleanjetsvboson && boson && PhysicsUtilities::deltaR2(*j, *boson) < 0.16) continue;
      ngenjets++;
      if(fabs(j->flavor()) == JetFlavorInfo::b_jet) {
	//	genb.push_back((LorentzVector)j->p4());
        ngenbjets++;
      }
    }/*
    data->fill<int  >(i_ngenjets, ngenjets);
    data->fill<int  >(i_ngenbjets, ngenbjets);
    if (genb.size()>0) {
      data->fill<float>(i_genb1pt, genb[0].pt());
      data->fill<float>(i_genb1eta, genb[0].eta());
    }

    if (genb.size()>1) {
      data->fill<float>(i_genb2pt, genb[1].pt());
      data->fill<float>(i_genb2eta, genb[1].eta());
      data->fill<float>(i_dphigenb1genb2,fabs(PhysicsUtilities::deltaPhi(genb[0],genb[1])) );
      data->fill<float>(i_dphigenb1genb2,fabs(PhysicsUtilities::deltaR(genb[0],genb[1])) );
    }*/

  }


  void fillJetInfo(TreeWriterData* data, vector<RecoJetF*> jets, vector<RecoJetF*> bjets, MomentumF* met) {

    int njets75 = 0, njets30 = 0; 
    int ntbjets = 0, nmbjets = 0, nlbjets = 0, nbjets30 = 0;
    int njl = 0;
    LorentzVector mht_; 
    for(auto* j : jets) {
      if(j->pt() > 75.0) njets75++;
      if(j->pt() > 30.0) njets30++;
      if(j->csv() < defaults::CSV_LOOSE)  njl++;
      if(j->csv() > defaults::CSV_LOOSE)  nlbjets++;
      if(j->csv() > defaults::CSV_MEDIUM) nmbjets++;
      if(j->csv() > defaults::CSV_TIGHT)  ntbjets++;
      if(j->csv() > defaults::CSV_MEDIUM && j->pt() > 30.0) nbjets30++;

      if ( (j->pt()>30.) && (fabs(j->eta())<2.5) ) { mht_ += j->p4(); }
    }

    data->fill<int>(i_njets,    int(jets.size()));
    data->fill<int>(i_njets30,  njets30);
    data->fill<int>(i_njets75,  njets75);
    data->fill<int>(i_nbjets,   int(bjets.size()));
    data->fill<int>(i_nbjets30, nbjets30);
    data->fill<int>(i_nlbjets,  nlbjets);
    data->fill<int>(i_nmbjets,  nmbjets);
    data->fill<int>(i_ntbjets,  ntbjets);
    data->fill<int>(i_njl,  njl);
    data->fill<float>(i_mht,  mht_);
    data->fill<float>(i_ht,   JetKinematics::ht(jets, 20.0, 2.4));
    data->fill<float>(i_ht30, JetKinematics::ht(jets, 30.0, 2.4));
    data->fill<float>(i_ht40, JetKinematics::ht(jets, 40.0, 2.4));
    data->fill<float>(i_meff, JetKinematics::ht(jets, 20.0, 2.4)+(met->pt()));
    data->fill<float>(i_metovsqrtht, (met->pt())/(sqrt(JetKinematics::ht(jets, 20.0, 2.4))));

    // collection of jets that do not pass CSVL
    std::vector<LorentzVector> lvjl; 
    lvjl.clear();
    for(auto* j : jets) { if ( (j->pt()>20) && (j->csv() < defaults::CSV_LOOSE) ) { lvjl.push_back((LorentzVector)j->p4()); } }
    if (lvjl.size()>0) { 
      data->fill<float>(i_j1lpt,lvjl[0].pt()); 
      data->fill<float>(i_dphij1lmet,fabs(PhysicsUtilities::deltaPhi(lvjl[0],*met))); 
    }
    if (lvjl.size()>1) { 
      data->fill<float>(i_j2lpt,lvjl[1].pt()); 
      data->fill<float>(i_dphij2lmet,fabs(PhysicsUtilities::deltaPhi(lvjl[1],*met))); 
      data->fill<float>(i_dphij1lj2l,fabs(PhysicsUtilities::deltaPhi(lvjl[0],lvjl[1]))); 
      data->fill<float>(i_drj1lj2l,fabs(PhysicsUtilities::deltaR(lvjl[0],lvjl[1]))); 
    }

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
    if(jets.size() > 3){
      data->fill<float>(i_dphij4met, fabs(PhysicsUtilities::deltaPhi(*jets[3], *met)));
    }

    vector<RecoJetF*> jetsCSVranked;
    rankedByCSV(jets,jetsCSVranked);

    float mtcsv1met = 0.0, mtcsv2met = 0.0,dphicsv1met = 0.0,dphicsv2met = 0.0;
    if(jetsCSVranked.size() > 0) {
      mtcsv1met = JetKinematics::transverseMass(*jetsCSVranked[0], *met);
      dphicsv1met = fabs(PhysicsUtilities::deltaPhi(*jetsCSVranked[0], *met));
      data->fill<float>(i_mtcsv1met, mtcsv1met);
      data->fill<float>(i_csvj1pt, jetsCSVranked[0]->pt());
      data->fill<float>(i_csvj1eta, jetsCSVranked[0]->eta());
      data->fill<float>(i_csv1, jetsCSVranked[0]->csv());
      data->fill<float>(i_dphicsv1met, dphicsv1met);
      if(jetsCSVranked.size() == 1) {
        data->fill<float>(i_mtcsv12met, mtcsv1met);
        data->fill<float>(i_dphicsv12met, dphicsv1met);
      }
    }

    if(jetsCSVranked.size() > 1) {

      mtcsv2met = JetKinematics::transverseMass(*jetsCSVranked[1], *met);
      dphicsv2met = fabs(PhysicsUtilities::deltaPhi(*jetsCSVranked[1], *met));
      data->fill<float>(i_csvj2pt, jetsCSVranked[1]->pt());
      data->fill<float>(i_csvj2eta, jetsCSVranked[1]->eta());
      data->fill<float>(i_csv2, jetsCSVranked[1]->csv());
      data->fill<float>(i_mtcsv2met, mtcsv2met);
      data->fill<float>(i_mtcsv12met, min(mtcsv1met,mtcsv2met));
      data->fill<float>(i_dphicsv2met, dphicsv2met);
      data->fill<float>(i_dphicsv12met,min(dphicsv1met,dphicsv2met));
      data->fill<float>(i_dphicsv1csv2,fabs(PhysicsUtilities::deltaPhi(*jetsCSVranked[0], *jetsCSVranked[1])));
      data->fill<float>(i_drcsv1csv2,fabs(PhysicsUtilities::deltaR(*jetsCSVranked[0], *jetsCSVranked[1])));

    }

    
    float nonisrpt = 0.;
    for (unsigned int i0=0; i0<lvjl.size(); ++i0) { if (i0>0) { nonisrpt += lvjl[i0].pt(); } }
    data->fill<float>(i_nonisrpt, nonisrpt);

    float blpt = 0.;
    float bmpt = 0.;
    for(auto* j : jets) { 
      if ( (j->pt()>20) && (j->csv() > defaults::CSV_LOOSE) )  { blpt += j->pt(); } 
      if ( (j->pt()>20) && (j->csv() > defaults::CSV_MEDIUM) ) { bmpt += j->pt(); }
    }
    data->fill<float>(i_blpt  , blpt);
    data->fill<float>(i_bmpt  , bmpt);
   
  }

};

class ZeroLeptonAnalyzer : public TreeCopierManualBranches {

  public :
    ZeroLeptonAnalyzer(TString fileName, TString treeName, TString outfileName, size randomSeed, bool isMCTree, cfgSet::ConfigSet *pars) :
      TreeCopierManualBranches(fileName, treeName, outfileName, randomSeed, isMCTree, pars) {zIsInvisible = true; if(fileName.Contains("dy")) zIsInvisible = false;}

    const double metcut_   = 200.0 ;
    const int    minnjets_ =   2   ;
    const double minj2pt_  = 75.0  ;

    TreeFiller filler;

    virtual ~ZeroLeptonAnalyzer() {}

    void book() {
      filler.book(&data);
    }

    bool fillEvent() {

      if(!goodvertex) return false;

      std::vector<LorentzVector> lvjl; 
      lvjl.clear();
      for(auto* j : jets) { if ( (j->pt()>20) && (j->csv() < defaults::CSV_LOOSE) ) { lvjl.push_back((LorentzVector)j->p4()); } }

      bool addlep2met = false;

      if (addlep2met) {  
	if (nSelLeptons < 1    ) return false; 
	met->p4() = (LorentzVector)met->p4() + (LorentzVector)selectedLepton->p4();
	if (met->pt() < 150    ) return false;
	if (nJets     < 1      ) return false;
	if (lvjl.size()<1      ) return false;
	if (lvjl.at(0).pt()<100) return false; 
	//	if (jets.at(0)->pt() < 100)  return false;
      }
      else {
	if (met->pt() < 150    ) return false;
	if (nJets     < 1      ) return false;
	if (lvjl.size()<1      ) return false;
	if (lvjl.at(0).pt()<100) return false;
      }
      
      filler.fillEventInfo(&data, this);
      filler.fillJetInfo  (&data, jets, bJets, met);
      return true;
    }

};

#endif
