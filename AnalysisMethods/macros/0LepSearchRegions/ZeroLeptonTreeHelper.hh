#ifndef ZEROLEPTONTREEHELPER_HH
#define ZEROLEPTONTREEHELPER_HH

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
//#include "AnalysisMethods/macros/0LepSearchRegions/M2.hh"
#include "AnalysisMethods/macros/0LepSearchRegions/HettTopTagger.hh"
#include "AnalysisMethods/macros/0LepSearchRegions/CttTopTagger.hh"

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

std::vector<LorentzVector> topsPass;
struct TreeFiller {

  TreeFiller() {}
  size i_run       ;
  size i_lumi      ;
  size i_event     ;
  size i_weight    ;
  size i_systweights;
  size i_ismc      ;
  size i_truePUWeight;
  size i_cttWeight;
  size i_btagWeight;
  size i_btagFastSimWeight;
  size i_qcdRespTailWeight;
  size i_normWeight;
  size i_topptWeight;
  size i_passtrige ;
  size i_passtrigmu;
  size i_passtrige17e12;
  size i_passtrigmu17mu8;
  size i_passtrigmu17tkmu8;
  size i_passtrigmu8e17;
  size i_passtrigmu17e12;
  size i_passtrigphoton165;
  size i_passtright300;
  size i_passtright350;
  size i_passtright400;
  size i_passtright475;
  size i_passtright600;
  size i_passtright650;
  size i_passtright800;
  size i_passtright900;
  size i_passjson  ;
  size i_passdijetmet;
  size i_isfastsim;
  size i_pass_HBHENoiseFilter                    ;
  size i_pass_HBHENoiseIsoFilter                 ;
  size i_pass_CSCTightHalo2015Filter             ;
  size i_pass_EcalDeadCellTriggerPrimitiveFilter ;
  size i_pass_goodVertices                       ;
  size i_pass_eeBadScFilter                      ;
  size i_genmet    ;
  size i_bosonpt   ;
  size i_bosoneta  ;
  size i_distoppt  ;
  size i_lepvetoweight;
  size i_lepselweight;
  size i_leptnpweight;
  size i_met       ;
  size i_metphi    ;
  size i_metnohf   ;
  size i_metnohfphi;
  size i_npv       ;
  size i_nvetolep  ;
  size i_nvetotau  ;
  size i_nvetohpstaus;
  size i_nvetomu   ;
  size i_nvetolele ;
  size i_ngoodgenmu;
  size i_ngoodgenele;
  size i_npromptgentau;
  size i_nctt      ;
  size i_ncttstd   ;
  size i_ncttstd_dphimet;
  size i_nctt_cttmassonly;
  size i_nctt_cttmassonly_dphimet;
  size i_nfjsd60   ;
  size i_nfjpr60   ;
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
  size i_mtcsv12met_mod;
  size i_dphicsv1met;
  size i_dphicsv2met;
  size i_dphicsv12met;
  size i_absdphilepw;
  size i_leptonpt  ;
  size i_leptoneta ;
  size i_leptonpdgid;
  size i_htalonglep;
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
  size i_absdphilepmet;
  size i_nsdtopjmewp1vloose;
  size i_nsdtopjmewp1loose;
  size i_nsdtopjmewp1tight;
  size i_nsdwjmewp1loose;
  size i_nsdwjmewp1tight;


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

  bool passCHFFilter(vector<RecoJetF*> jets) {
    bool pass = true;
    for(auto* jet : jets) {
      if(jet->pt() > 20.0 && fabs(jet->eta()) < 2.5 && (!jet->genJet() || jet->genJet()->pt() < 5.0) && jet->chHadFrac() < 0.1) pass = false;
    }
    return pass;
  }

  void book(TreeWriterData* data) {
    i_run            = data->add<unsigned int>("","run","i",0);
    i_lumi           = data->add<unsigned int>("","lumi","i",0);
    i_event          = data->add<unsigned int>("","event","i",0);
    i_ismc           = data->add<bool >("","ismc","O",0);
    i_weight         = data->add<float>("","weight","F",0);
    i_systweights    = data->addMulti<float>("","systweights",0);
    i_truePUWeight   = data->add<float>("","truePUWeight","F",0);
    i_cttWeight      = data->add<float>("","cttWeight","F",0);
    i_btagWeight     = data->add<float>("","btagWeight","F",0);
    i_btagFastSimWeight     = data->add<float>("","btagFastSimWeight","F",0);
    i_qcdRespTailWeight = data->add<float>("","qcdRespTailWeight","F",0);
    i_normWeight     = data->add<float>("","normWeight","F",0);
    i_topptWeight    = data->add<float>("","topptWeight","F",1);
    i_passtrige      = data->add<bool>("","passtrige","O",0);
    i_passtrigmu     = data->add<bool>("","passtrigmu","O",0);
    i_passtrige17e12 = data->add<bool >("","passtrige17e12","O",0);
    i_passtrigmu17mu8= data->add<bool >("","passtrigmu17mu8","O",0);
    i_passtrigmu17tkmu8 = data->add<bool>("","passtrigmu17tkmu8", "O",0);
    i_passtrigmu8e17 = data->add<bool >("","passtrigmu8e17","O",0);
    i_passtrigmu17e12= data->add<bool >("","passtrigmu17e12","O",0);
    i_passtrigphoton165 = data->add<bool>("","passtrigphoton165", "O",0);
    i_passtright300  = data->add<bool>("","passtright300","O",0);
    i_passtright350  = data->add<bool>("","passtright350","O",0);
    i_passtright400  = data->add<bool>("","passtright400","O",0);
    i_passtright475  = data->add<bool>("","passtright475","O",0);
    i_passtright600  = data->add<bool>("","passtright600","O",0);
    i_passtright650  = data->add<bool>("","passtright650","O",0);
    i_passtright800  = data->add<bool>("","passtright800","O",0);
    i_passtright900  = data->add<bool>("","passtright900","O",0);
    i_passdijetmet   = data->add<bool>("","passdijetmet","O",0);
    i_passjson       = data->add<bool>("","passjson","O",0);
    i_isfastsim      = data->add<bool>("","isfastsim","O",0);
    i_pass_HBHENoiseFilter                      = data->add<bool>("","pass_HBHENoiseFilter"                   ,"O",0);
    i_pass_HBHENoiseIsoFilter                   = data->add<bool>("","pass_HBHENoiseIsoFilter"                ,"O",0);
    i_pass_CSCTightHalo2015Filter               = data->add<bool>("","pass_CSCTightHalo2015Filter"            ,"O",0);
    i_pass_EcalDeadCellTriggerPrimitiveFilter   = data->add<bool>("","pass_EcalDeadCellTriggerPrimitiveFilter","O",0);
    i_pass_goodVertices                         = data->add<bool>("","pass_goodVertices"                      ,"O",0);
    i_pass_eeBadScFilter                        = data->add<bool>("","pass_eeBadScFilter"                     ,"O",0);
    i_genmet         = data->add<float>("","genmet","F",0);
    i_bosonpt        = data->add<float>("","bosonpt","F",0);
    i_bosoneta       = data->add<float>("","bosoneta","F",0);
    i_distoppt       = data->add<float>("","distoppt","F",0);
    i_met            = data->add<float>("","met","F",0);
    i_lepvetoweight  = data->add<float>("","lepvetoweight","F",0);
    i_lepselweight   = data->add<float>("","lepselweight","F",0);
    i_leptnpweight   = data->add<float>("","leptnpweight","F",0);
    i_metphi         = data->add<float>("","metphi","F",0);
    i_metnohf        = data->add<float>("","metnohf","F",0);
    i_metnohfphi     = data->add<float>("","metnohfphi","F",0);
    i_npv            = data->add<int>("","npv","I",0);
    i_nvetolep       = data->add<int>("","nvetolep","I",0);
    i_nvetotau       = data->add<int>("","nvetotau","I",0);
    i_nvetohpstaus   = data->add<int>("","nvetohpstaus","I",0);
    i_nvetomu        = data->add<int>("","nvetomu","I",0);
    i_nvetolele      = data->add<int>("","nvetolele","I",0);
    i_ngoodgenmu     = data->add<int>("","ngoodgenmu","I",0);
    i_ngoodgenele    = data->add<int>("","ngoodgenele","I",0);
    i_npromptgentau  = data->add<int>("","npromptgentau","I",0);
    i_nctt           = data->add<int>("","nctt","I",0);
    i_ncttstd        = data->add<int>("","ncttstd","I",0);
    i_ncttstd_dphimet = data->add<int>("", "ncttstd_dphimet", "I", 0);
    i_nctt_cttmassonly=data->add<int>("","nctt_cttmassonly", "I", 0);
    i_nctt_cttmassonly_dphimet=data->add<int>("","nctt_cttmassonly_dphimet", "I", 0);
    i_nfjsd60        = data->add<int>("","nfjsd60","I",0);
    i_nfjpr60        = data->add<int>("","nfjpr60","I",0);
    i_chhpt          = data->addMulti<float>("","chhpt",0);
    i_chheta         = data->addMulti<float>("","chheta",0);
    i_chhdz          = data->addMulti<float>("","chhdz",0);
    i_chhmt          = data->addMulti<float>("","chhmt",0);
    i_chhdphimet     = data->addMulti<float>("","chhdphimet",0);
    i_chhtaudisc     = data->addMulti<float>("","chhtaudisc",0);
    i_mupt           = data->addMulti<float>("","mupt",0);
    i_mueta          = data->addMulti<float>("","mueta",0);
    i_muhtalong      = data->addMulti<float>("","muhtalong",0);
    i_muannulus      = data->addMulti<float>("","muannulus",0);
    i_elept          = data->addMulti<float>("","elept",0);
    i_eleeta         = data->addMulti<float>("","eleeta",0);
    i_elehtalong     = data->addMulti<float>("","elehtalong",0);
    i_eleannulus     = data->addMulti<float>("","eleannulus",0);
    i_trackpt        = data->addMulti<float>("","trackpt",0);
    i_tracketa       = data->addMulti<float>("","tracketa",0);
    i_trackhtalong   = data->addMulti<float>("","trackhtalong",0);
    i_taupt          = data->addMulti<float>("","taupt",0);
    i_taueta         = data->addMulti<float>("","taueta",0);
    i_tauhtalong     = data->addMulti<float>("","tauhtalong",0);
    i_ngenjets       = data->add<int>("","ngenjets","I",0);
    i_ngenbjets      = data->add<int>("","ngenbjets","I",0);
    i_njets          = data->add<int>("","njets","I",0);
    i_njets30        = data->add<int>("","njets30","I",0);
    i_njets75        = data->add<int>("","njets75","I",0);
    i_nbjets         = data->add<int>("","nbjets","I",0);
    i_nbjets30       = data->add<int>("","nbjets30","I",0);
    i_nlbjets        = data->add<int>("","nlbjets","I",0);
    i_nmbjets        = data->add<int>("","nmbjets","I",0);
    i_ntbjets        = data->add<int>("","ntbjets","I",0);
    i_ht             = data->add<float>("","ht","F",0);
    i_ht30           = data->add<float>("","ht30","F",0);
    i_ht40           = data->add<float>("","ht40","F",0);
    i_j1pt           = data->add<float>("","j1pt","F",0);
    i_j1eta          = data->add<float>("","j1eta","F",0);
    i_j2pt           = data->add<float>("","j2pt","F",0);
    i_j2eta          = data->add<float>("","j2eta","F",0);
    i_j3pt           = data->add<float>("","j3pt","F",0);
    i_j3eta          = data->add<float>("","j3eta","F",0);
    i_csvj1pt        = data->add<float>("","csvj1pt","F",0);
    i_csvj1eta       = data->add<float>("","csvj1eta","F",0);
    i_csvj2pt        = data->add<float>("","csvj2pt","F",0);
    i_csvj2eta       = data->add<float>("","csvj2eta","F",0);
    i_csv1           = data->add<float>("","csv1","F",0);
    i_csv2           = data->add<float>("","csv2","F",0);
    i_dphij1met      = data->add<float>("","dphij1met","F",0);
    i_dphij2met      = data->add<float>("","dphij2met","F",0);
    i_dphij12met     = data->add<float>("","dphij12met","F",0);
    i_dphij3met      = data->add<float>("","dphij3met","F",3);
    i_dphij4met      = data->add<float>("","dphij4met","F",3);
    i_mtcsv1met      = data->add<float>("","mtcsv1met","F",0);
    i_mtcsv2met      = data->add<float>("","mtcsv2met","F",0);
    i_mtcsv12met     = data->add<float>("","mtcsv12met","F",0);
    i_mtcsv12met_mod = data->add<float>("","mtcsv12met_mod","F",0);
    i_dphicsv1met    = data->add<float>("","dphicsv1met","F",0);
    i_dphicsv2met    = data->add<float>("","dphicsv2met","F",0);
    i_dphicsv12met   = data->add<float>("","dphicsv12met","F",0);
    i_leptonpt       = data->add<float>("","leptonpt","F",0);
    i_leptoneta      = data->add<float>("","leptoneta","F",0);
    i_leptonpdgid    = data->add<int>("","leptonpdgid","I",0);
    i_htalonglep     = data->add<float>("","htalonglep","F",0);
    i_annulus        = data->add<float>("","annulus","F",0);
    i_leptonmatchtrigmu  = data->add<bool>("","leptonmatchtrigmu","O",0);
    i_leptonmatchtrige   = data->add<bool>("","leptonmatchtrige","O",0);
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
    i_absdphilepmet       = data->add<float>("","absdphilepmet","F",0);
    i_nsdtopjmewp1vloose  = data->add<int>("","nsdtopjmewp1vloose","I",0);
    i_nsdtopjmewp1loose  = data->add<int>("","nsdtopjmewp1loose","I",0);
    i_nsdtopjmewp1tight  = data->add<int>("","nsdtopjmewp1tight","I",0);
    i_nsdwjmewp1loose    = data->add<int>("","nsdwjmewp1loose","I",0);
    i_nsdwjmewp1tight    = data->add<int>("","nsdwjmewp1tight","I",0);
  }

  bool passSoftDropTaggerFJ(FatJetF* fj,float minMass,float maxMass) {
    return ( (fj->fjSoftDropMass() > minMass) && (fj->fjSoftDropMass() < maxMass) && fabs(fj->p4().eta())<=2.4);
  }

  bool passPrunedTaggerFJ(FatJetF* fj,float minMass,float maxMass) {
    return ( (fj->fjPrunedMass() > minMass) && (fj->fjPrunedMass() < maxMass) && fabs(fj->p4().eta())<=2.4 );
  }

  bool passSoftDropTaggerCTT(CMSTopF* ctt,float minMass,float maxMass,int nsubjet, float t2ovt1) {

    float mass_   = ctt->topSoftDropMass();
    int nsubjts_  = ctt->topNsubJets();
    float t2ovt1_ = (ctt->topTau2())/(ctt->topTau1());

    return ( (mass_ > minMass) && (mass_ < maxMass) && (nsubjts_ >= nsubjet)  && (t2ovt1_<= t2ovt1));
  }


  bool passSoftDropTaggerFJ(const FatJetF* fj,float minMass,float maxMass, float tau32max, float tau21max) {
    return ( (fj->fjSoftDropMass() > minMass) && (fj->fjSoftDropMass() < maxMass) && fabs(fj->p4().eta())<=2.4 && (fj->p4().pt()>400.) && ((fj->fjTau3())/(fj->fjTau2()))<tau32max && ((fj->fjTau2())/(fj->fjTau1()))<tau21max);
  }

  void fillEventInfo(TreeWriterData* data, BaseTreeAnalyzer* ana, bool lepAddedBack = false, MomentumF* metn = 0) {
    data->fill<unsigned int>(i_run, ana->run);
    data->fill<unsigned int>(i_lumi, ana->lumi);
    data->fill<unsigned int>(i_event, ana->event);
    data->fill<bool >(i_ismc, ana->isMC());
    data->fill<float>(i_weight, ana->weight);
    for(auto wgt : *ana->evtInfoReader.systweights) {
      data->fillMulti<float>(i_systweights, wgt/ana->evtInfoReader.lhecentralweight);
    }
    data->fill<float>(i_truePUWeight,    ana->eventCorrections.getTruePUWeight());
    data->fill<float>(i_cttWeight,       ana->eventCorrections.getCTTWeight());
    data->fill<float>(i_btagWeight, ana->bTagCorrections.getBTagByEvtWeight());
    data->fill<float>(i_btagFastSimWeight, ana->bTagCorrections.getBTagFastSimByEvtWeight());
    data->fill<float>(i_qcdRespTailWeight, ana->jetAndMETCorrections.getQCDRespTailWeight());
    data->fill<float>(i_normWeight,  ana->eventCorrections.getNormWeight());
    data->fill<float>(i_topptWeight, ana->ttbarCorrections.getTopPTWeight());
    data->fill<bool >(i_passtrige,  ana->isMC() ? true : (ana->process==defaults::DATA_SINGLEEL ? ana->triggerflag & kHLT_Ele22_eta2p1_WPLoose_Gsf : false));
    data->fill<bool >(i_passtrigmu, ana->isMC() ? true : (ana->process==defaults::DATA_SINGLEMU ? ana->triggerflag & kHLT_IsoMu22 : false));
    data->fill<bool >(i_passtrige17e12, ana->triggerflag & kHLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ);
    data->fill<bool >(i_passtrigmu17mu8, ana->triggerflag & kHLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ);
    data->fill<bool >(i_passtrigmu17tkmu8, ana->triggerflag & kHLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ);
    data->fill<bool >(i_passtrigmu8e17, ana->triggerflag & kHLT_Mu8_TrkIsoVVL_Ele17_CaloIdL_TrackIdL_IsoVL);
    data->fill<bool >(i_passtrigmu17e12, ana->triggerflag & kHLT_Mu17_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL);
    data->fill<bool >(i_passtrigphoton165, ana->triggerflag & kHLT_Photon165_HE10);
    data->fill<bool >(i_passtright300, ana->isMC() ? true : (ana->process==defaults::DATA_JETHT ? ana->triggerflag & kHLT_PFHT300 : false));
    data->fill<bool >(i_passtright350, ana->isMC() ? true : (ana->process==defaults::DATA_JETHT ? ana->triggerflag & kHLT_PFHT350 : false));
    data->fill<bool >(i_passtright400, ana->isMC() ? true : (ana->process==defaults::DATA_JETHT ? ana->triggerflag & kHLT_PFHT400 : false));
    data->fill<bool >(i_passtright475, ana->isMC() ? true : (ana->process==defaults::DATA_JETHT ? ana->triggerflag & kHLT_PFHT475 : false));
    data->fill<bool >(i_passtright600, ana->isMC() ? true : (ana->process==defaults::DATA_JETHT ? ana->triggerflag & kHLT_PFHT600 : false));
    data->fill<bool >(i_passtright650, ana->isMC() ? true : (ana->process==defaults::DATA_JETHT ? ana->triggerflag & kHLT_PFHT650 : false));
    data->fill<bool >(i_passtright800, ana->isMC() ? true : (ana->process==defaults::DATA_JETHT ? ana->triggerflag & kHLT_PFHT800 : false));
    data->fill<bool >(i_passtright900, ana->isMC() ? true : (ana->process==defaults::DATA_JETHT ? ana->triggerflag & kHLT_PFHT900 : false));
    data->fill<bool >(i_passdijetmet, ana->isMC() ? true : (ana->process==defaults::DATA_HTMHT ? ana->triggerflag & kHLT_DiCentralPFJet55_PFMET110_NoiseCleaned : false));
    data->fill<bool >(i_isfastsim,     ana->process==defaults::SIGNAL);
    bool hasJSON = ana->hasJSONFile();
    bool isMC = ana->isMC();
    bool passesLumi = ana->passesLumiMask();
    data->fill<bool>(i_passjson, ((!isMC) && (hasJSON) && (!passesLumi)) ? false : true);
    data->fill<float>(i_lepvetoweight, ana->leptonCorrections.getVetoLepWeight());
    data->fill<float>(i_lepselweight, ana->leptonCorrections.getSelLepWeight());
    data->fill<float>(i_leptnpweight, ana->leptonCorrections.getTnPLepWeight());
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
    data->fill<int  >(i_nctt, ana->cttTops.size());

    topsPass.clear();
    for(auto* ctt : ana->cttTops) {
      if(cfgSet::isSelTaggedTop(*ctt)) {
        LorentzVector tmpVec; tmpVec = ctt->p4();
        topsPass.push_back(tmpVec);
      }

    }
    data->fill<int  >(i_ncttstd   , ana->nSelCTTTops   );
    data->fill<int>(i_ncttstd_dphimet, std::count_if(ana->cttTops.begin(), ana->cttTops.end(), [this, ana](CMSTopF *ctt){return cfgSet::isSelTaggedTop(*ctt) && fabs(PhysicsUtilities::deltaPhi(*ctt, *ana->met)>0.8);}));
    data->fill<int>(i_nctt_cttmassonly, std::count_if(ana->cttTops.begin(), ana->cttTops.end(), [](CMSTopF *ctt){return ctt->topCmsTopTagMass() > 140.0 && ctt->topCmsTopTagMass() < 250.0 && ctt->pt()>=400 && fabs(ctt->eta())<=2.4;}));
    data->fill<int>(i_nctt_cttmassonly_dphimet, std::count_if(ana->cttTops.begin(), ana->cttTops.end(), [ana](CMSTopF *ctt){return ctt->topCmsTopTagMass() > 140.0 && ctt->topCmsTopTagMass() < 250.0 && ctt->pt()>=400 && fabs(ctt->eta())<=2.4 && fabs(PhysicsUtilities::deltaPhi(*ctt, *ana->met)>0.8);}));
    if (ana->nSelCTTTops>0) {
      data->fill<float>(i_toppt,ana->selectedCTTTops[0]->pt());
      data->fill<float>(i_dphitopmet,fabs(PhysicsUtilities::deltaPhi(*ana->selectedCTTTops[0], *ana->met)));
    }
    else {
      data->fill<float>(i_toppt,0.);
      data->fill<float>(i_dphitopmet,0);
    }

    if (ana->cttTops.size()>1) { data->fill<float>(i_httwoleadfatjet,(ana->cttTops[0]->pt()+ana->cttTops[1]->pt())); } else { data->fill<float>(i_httwoleadfatjet,0.); }


    int nfjsd60_ = 0, nfjpr60_   = 0;
    int nsdtopjmewp1loose_ = 0, nsdtopjmewp1vloose_ = 0, nsdtopjmewp1tight_ = 0, nsdwjmewp1loose_ = 0, nsdwjmewp1tight_ = 0;
    for(auto* fj : ana->fatJets) {
      if (passSoftDropTaggerFJ(fj,60.,100000.)) { ++nfjsd60_; }
      if (passPrunedTaggerFJ(fj,60.,100000.))   { ++nfjpr60_; }
      if (passSoftDropTaggerFJ(fj,110.,210.,0.50,100.))    { ++nsdtopjmewp1tight_; }
      if (passSoftDropTaggerFJ(fj,110.,210.,0.69,100.))    { ++nsdtopjmewp1loose_; }
      if (passSoftDropTaggerFJ(fj,110.,210.,0.86,100.))    { ++nsdtopjmewp1vloose_; }
      if (passSoftDropTaggerFJ(fj,60.,110.,10.,0.45))      { ++nsdwjmewp1tight_; }
      if (passSoftDropTaggerFJ(fj,60.,110.,10.,0.60))      { ++nsdwjmewp1loose_; }
    }
    data->fill<int  >(i_nfjsd60, nfjsd60_);
    data->fill<int  >(i_nfjpr60, nfjpr60_);
    data->fill<int  >(i_nsdtopjmewp1tight,nsdtopjmewp1tight_);
    data->fill<int  >(i_nsdtopjmewp1loose,nsdtopjmewp1loose_);
    data->fill<int  >(i_nsdtopjmewp1vloose,nsdtopjmewp1vloose_);
    data->fill<int  >(i_nsdwjmewp1tight,nsdwjmewp1tight_);
    data->fill<int  >(i_nsdwjmewp1loose,nsdwjmewp1loose_);

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
        bool cleanHt = false;
        float htalong = 0;
        for(const auto* jet : ana->jets) {
          if(PhysicsUtilities::absDeltaPhi(*jet,*l) < TMath::PiOver2()) {
            htalong += jet->pt();
            if(PhysicsUtilities::deltaR(*jet,*l) < 0.4) cleanHt = true;
          }
        }
        if(cleanHt) htalong -= l->pt();
        if(l->ismuon()) {
          data->fillMulti<float>(i_mupt, l->pt());
          data->fillMulti<float>(i_mueta, l->eta());
          data->fillMulti<float>(i_muhtalong, htalong);
          data->fillMulti<float>(i_muannulus, l->pt()*l->annulusactivity());
        } else {
          data->fillMulti<float>(i_elept, l->pt());
          data->fillMulti<float>(i_eleeta, l->eta());
          data->fillMulti<float>(i_elehtalong, htalong);
          data->fillMulti<float>(i_eleannulus, l->pt()*l->annulusactivity());
        }
      }
    }

    if(ana->nVetoedTracks > 0) {
      for(const auto* tr : ana->vetoedTracks) {
        data->fillMulti<float>(i_trackpt, tr->pt());
        data->fillMulti<float>(i_tracketa, tr->eta());
        bool cleanHt = false;
        float htalong = 0;
        for(const auto* jet : ana->jets) {
          if(PhysicsUtilities::absDeltaPhi(*jet,*tr) < TMath::PiOver2()) {
            htalong += jet->pt();
            if(PhysicsUtilities::deltaR(*jet,*tr) < 0.4) cleanHt = true;
          }
        }
        if(cleanHt) htalong -= tr->pt();
        data->fillMulti<float>(i_trackhtalong, htalong);
      }
    }

    if(ana->nVetoHPSTaus > 0) {
      for(const auto* tau : ana->vetoedTaus) {
        data->fillMulti<float>(i_taupt, tau->pt());
        data->fillMulti<float>(i_taueta, tau->eta());
        bool cleanHt = false;
        float htalong = 0;
        for(const auto* jet : ana->jets) {
          if(PhysicsUtilities::absDeltaPhi(*jet,*tau) < TMath::PiOver2()) {
            htalong += jet->pt();
            if(PhysicsUtilities::deltaR(*jet,*tau) < 0.4) cleanHt = true;
          }
        }
        if(cleanHt) htalong -= tau->pt();
        data->fillMulti<float>(i_tauhtalong, htalong);
      }
    }

    float absdphilepmet = -1.;
    if(ana->selectedLepton) {
      const auto * lep = ana->selectedLepton;
      auto WP4 = lep->p4() + ana->met->p4();
      absdphilepmet = fabs(PhysicsUtilities::deltaPhi(*lep, *ana->met));
      data->fill<float>(i_absdphilepw, fabs(PhysicsUtilities::deltaPhi(WP4, *lep)) );
      data->fill<float>(i_leptonpt, lep->pt());
      data->fill<float>(i_leptoneta, lep->eta());
      data->fill<int  >(i_leptonpdgid, lep->pdgid());
      bool cleanHt = false;
      float htalonglep = 0;
      for(const auto* jet : ana->jets ){
        if (PhysicsUtilities::absDeltaPhi(*jet,*lep) < TMath::PiOver2()) {
          htalonglep += jet->pt();
          if(PhysicsUtilities::deltaR(*jet,*lep) < .4) cleanHt = true;
        }
      } // jets
      if(cleanHt) htalonglep -= lep->pt();
      data->fill<float>(i_htalonglep, htalonglep);
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
    data->fill<float>(i_absdphilepmet, absdphilepmet);

    data->fill<bool>(i_pass_HBHENoiseFilter                   ,ana->evtInfoReader.HBHENoiseFilter                   );
    data->fill<bool>(i_pass_HBHENoiseIsoFilter                ,ana->evtInfoReader.HBHENoiseIsoFilter                );
    data->fill<bool>(i_pass_CSCTightHalo2015Filter            ,ana->evtInfoReader.CSCTightHalo2015Filter            );
    data->fill<bool>(i_pass_EcalDeadCellTriggerPrimitiveFilter,ana->evtInfoReader.EcalDeadCellTriggerPrimitiveFilter);
    data->fill<bool>(i_pass_goodVertices                      ,ana->evtInfoReader.goodVertices                      );
    data->fill<bool>(i_pass_eeBadScFilter                     ,ana->evtInfoReader.eeBadScFilter                     );

    std::vector<float> gentoppt_; gentoppt_.clear();
    int nGoodGenMu = 0; int nGoodGenEle = 0; int nPromptTaus = 0;
    GenParticleF * stop1 = 0;
    GenParticleF * stop2 = 0;
    if(isMC) {
      for(auto* p : ana->genParts) {
        if(ParticleInfo::isA(ParticleInfo::p_stop1, p) and !stop1)
          stop1 = p;
        else if(ParticleInfo::isA(ParticleInfo::p_stop1, p) and !stop2)
          stop2 = p;
        const GenParticleF * genPartMom = 0;

	if (abs(p->pdgId())==6) { float tmppt_ = p->p4().pt(); gentoppt_.push_back(tmppt_); }

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

  // follows particle to final molting (eg gamma -> gamma -> gamma ...)
  // adapted from ParticleInfo(?) routine
  const GenParticleF* getFinal(const GenParticleF* particle) {
    if (!particle) return 0;
    const GenParticleF* final = particle;
    unsigned int numDaughters = 0;

    while(1==1) {
      numDaughters = final->numberOfDaughters();
      const GenParticleF* chain = 0;
      for(unsigned int iDau = 0; iDau < numDaughters; ++iDau)
        if(final->daughter(iDau)->pdgId() == particle->pdgId()) {
          chain = final->daughter(iDau); // next in chain
          break;
        }
      if(chain == 0) break;
      final = chain;
    }
    return final;
  }

  // tests if gen top decayed to a W, then to a (e/mu/tau) lepton
  bool isGenTopHadronic(GenParticleF* t) {
    if(abs(t->pdgId()) != 6) return false; // not a top
    const GenParticleF* ft = getFinal(t);
    for(unsigned int iD = 0; iD < ft->numberOfDaughters(); ++iD){
      const GenParticleF* w =  ft->daughter(iD);
      if(abs(w->pdgId()) != 24) continue;
      const GenParticleF* wf = getFinal(w);
      for(unsigned int iWD = 0; iWD < wf->numberOfDaughters(); ++iWD)
        if ((abs(wf->daughter(iWD)->pdgId()) >= 11) && (abs(wf->daughter(iWD)->pdgId()) <= 14)) return false;
    }
    return true;
  }

  void fillGenInfo(TreeWriterData* data, GenParticleF* boson, vector<GenJetF*> genjets, bool cleanjetsvboson = true) {
    data->fill<float>(i_bosonpt, boson ? boson->pt() : 0.0);
    data->fill<float>(i_bosoneta, boson ? boson->eta() : 0.0);
    int ngenjets = 0, ngenbjets = 0;
    for(auto* j : genjets) {
      if(cleanjetsvboson && boson && PhysicsUtilities::deltaR2(*j, *boson) < 0.16) continue;
      ngenjets++;
      if(fabs(j->flavor()) == JetFlavorInfo::b_jet) {
        ngenbjets++;
      }
    }
    data->fill<int  >(i_ngenjets, ngenjets);
    data->fill<int  >(i_ngenbjets, ngenbjets);
  }

  void fillJetInfo(TreeWriterData* data, vector<RecoJetF*> jets, vector<RecoJetF*> bjets, MomentumF* met) {

    int njets75 = 0, njets30 = 0;
    int ntbjets = 0, nmbjets = 0, nlbjets = 0, nbjets30 = 0;
    for(auto* j : jets) {
      if(j->pt() > 75.0) njets75++;
      if(j->pt() > 30.0) njets30++;
      if(j->csv() > defaults::CSV_LOOSE)  nlbjets++;
      if(j->csv() > defaults::CSV_MEDIUM) nmbjets++;
      if(j->csv() > defaults::CSV_TIGHT)  ntbjets++;
      if(j->csv() > defaults::CSV_MEDIUM && j->pt() > 30.0) nbjets30++;
    }
    data->fill<int>(i_njets,    int(jets.size()));
    data->fill<int>(i_njets30,  njets30);
    data->fill<int>(i_njets75,  njets75);
    data->fill<int>(i_nbjets,   int(bjets.size()));
    data->fill<int>(i_nbjets30, nbjets30);
    data->fill<int>(i_nlbjets,  nlbjets);
    data->fill<int>(i_nmbjets,  nmbjets);
    data->fill<int>(i_ntbjets,  ntbjets);
    data->fill<float>(i_ht,   JetKinematics::ht(jets, 20.0, 2.4));
    data->fill<float>(i_ht30, JetKinematics::ht(jets, 30.0, 2.4));
    data->fill<float>(i_ht40, JetKinematics::ht(jets, 40.0, 2.4));
    data->fill<float>(i_meff, JetKinematics::ht(jets, 20.0, 2.4)+(met->pt()));
    data->fill<float>(i_metovsqrtht, (met->pt())/(sqrt(JetKinematics::ht(jets, 20.0, 2.4))));


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

      int bJetFarFromTop = -1;
      if (topsPass.size()==1) {

	float dr1 = PhysicsUtilities::deltaR(*jetsCSVranked[0],topsPass[0]);
	float dr2 = PhysicsUtilities::deltaR(*jetsCSVranked[1],topsPass[0]);

	if (dr1>=dr2) { bJetFarFromTop = 0; } else { bJetFarFromTop = 1; }

      }

      mtcsv2met = JetKinematics::transverseMass(*jetsCSVranked[1], *met);
      dphicsv2met = fabs(PhysicsUtilities::deltaPhi(*jetsCSVranked[1], *met));
      data->fill<float>(i_csvj2pt, jetsCSVranked[1]->pt());
      data->fill<float>(i_csvj2eta, jetsCSVranked[1]->eta());
      data->fill<float>(i_csv2, jetsCSVranked[1]->csv());
      data->fill<float>(i_mtcsv2met, mtcsv2met);
      data->fill<float>(i_mtcsv12met, min(mtcsv1met,mtcsv2met));
      data->fill<float>(i_dphicsv2met, dphicsv2met);
      data->fill<float>(i_dphicsv12met,min(dphicsv1met,dphicsv2met));

      float mtcsv12met_mod_tmp = -1.;
      if (topsPass.size()!=1) { mtcsv12met_mod_tmp = min(mtcsv1met,mtcsv2met); }
      else                    { mtcsv12met_mod_tmp = JetKinematics::transverseMass(*jetsCSVranked[bJetFarFromTop], *met); }

      data->fill<float>(i_mtcsv12met_mod, mtcsv12met_mod_tmp);

    }

  }

};

class ZeroLeptonAnalyzer : public TreeCopierManualBranches {

  public :
    ZeroLeptonAnalyzer(TString fileName, TString treeName, TString outfileName, size randomSeed, bool isMCTree, cfgSet::ConfigSet *pars) :
      TreeCopierManualBranches(fileName, treeName, outfileName, randomSeed, isMCTree, pars) {zIsInvisible = true; if(fileName.Contains("dy")) zIsInvisible = false;}

    const double metcut_   = 200.0 ;
    const int    minnjets_ =   2   ;
    const double minj2pt_  = 75.0  ;

    bool applyCHFFilter    = false ;

    TreeFiller filler;
//    M2TreeFiller m2Filler;
//    HettTreeFiller hettFiller;
//    CttTreeFiller cttFiller;
    virtual ~ZeroLeptonAnalyzer() {}

    void book() {
      filler.book(&data);
//      m2Filler.book(&data);
//      hettFiller.book(&data);
//      cttFiller.book(&data);
    }

    bool fillEvent() {

      if(!goodvertex) return false;

      //if(nVetoedLeptons > 0)  return false;
      //if(nVetoedTracks  > 0)  return false;

      if(met->pt() < metcut_  ) return false;
      if(nJets     < minnjets_) return false;
      if(jets.at(1)->pt() < minj2pt_)  return false;

      if(applyCHFFilter && !filler.passCHFFilter(jets)) return false;

      filler.fillEventInfo(&data, this);
      filler.fillJetInfo  (&data, jets, bJets, met);
      //m2Filler.fillM2Info(&data,this,M2TreeFiller::TTBAR_1L);
      //hettFiller.fillHettInfo(&data,this,jets,hettTops);
      //cttFiller.fillTopTagInfo(&data,this,jets);
      return true;
    }

};

#endif
