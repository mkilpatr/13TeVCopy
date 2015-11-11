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

cfgSet::ConfigSet pars0LepPhoton(TString json) {
  cfgSet::loadDefaultConfigurations();
  cfgSet::setJSONFile(json);
  cfgSet::ConfigSet cfg = cfgSet::zl_photon_set;
  cfg.corrections.eventCorrections |= ucsbsusy::EventCorrectionSet::NORM;
  return cfg;
}

std::vector<LorentzVector> topsPass;
std::vector<LorentzVector> topCands;

struct TreeFiller {

  TreeFiller() {}
  size i_run       ;
  size i_lumi      ;
  size i_event     ;
  size i_weight    ;
  size i_ismc      ;
  size i_puWeight  ;
  size i_pu50NSWeight  ;
  size i_truePUWeight;
  size i_btagWeight;
  size i_normWeight;
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
  size i_passcscflt;
  size i_passeebadscflt;
  size i_passhbheflt;
  size i_passhbhefixflt;
  size i_passhbhefltloose;
  size i_passhbheflttight;
  size i_genmet    ;
  size i_bosonpt   ;
  size i_bosoneta  ;
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
  size i_nsellep   ;
  size i_nctt      ;
  size i_ncttstd   ;
  size i_nfjsdwa   ;
  size i_nfjsdwb   ;
  size i_nfjsdwc   ;
  size i_nfjsdta   ;
  size i_nfjsdtb   ;
  size i_ncttsdwa0 ;
  size i_ncttsdwa1 ;
  size i_ncttsdwa2 ;
  size i_ncttsdwa12;
  size i_ncttsdwb  ;
  size i_ncttsdwc  ;
  size i_ncttsdta  ;
  size i_ncttsdtb  ;
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
  size i_njets60   ;
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
  size i_bclose2lep;
  size i_ntopcand  ;
  size i_ntoppass  ;
  size i_topcandpt ;
  size i_topcandeta;
  size i_toppasspt ;
  size i_toppasseta;
  size i_topcandrawmass;
  size i_topcandtrimmedmass;
  size i_topcandprunedmass;
  size i_topcandsoftdropmass;
  size i_topcandcmstoptagmass;
  size i_topcandtau1;
  size i_topcandtau2;
  size i_topcandtau3;
  size i_topcandnsubjets;
  size i_topcandminmass;

  bool passCTTSelection(CMSTopF* ctt) {
    return (ctt->topRawMass() > 140.0 && ctt->topRawMass() < 250.0 && ctt->topMinMass() > 50.0 && ctt->topNsubJets() >= 3);
  } 

  bool passSoftDropTaggerFJ(FatJetF* fj,float minMass,float maxMass) {
    return ( (fj->fjSoftDropMass() > minMass) && (fj->fjSoftDropMass() < maxMass) );
  } 

  bool passSoftDropTaggerCTT(CMSTopF* ctt,float minMass,float maxMass,int nsubjet, float t2ovt1) {
    
    float mass_   = ctt->topSoftDropMass();
    int nsubjts_  = ctt->topNsubJets();
    float t2ovt1_ = (ctt->topTau2())/(ctt->topTau1());

    return ( (mass_ > minMass) && (mass_ < maxMass) && (nsubjts_ >= nsubjet)  && (t2ovt1_<= t2ovt1));
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
    i_run            = data->add<unsigned int>("","run","i",0);
    i_lumi           = data->add<unsigned int>("","lumi","i",0);
    i_event          = data->add<unsigned int>("","event","i",0);
    i_ismc           = data->add<bool >("","ismc","O",0);
    i_weight         = data->add<float>("","weight","F",0);
    i_puWeight       = data->add<float>("","puWeight","F",0);
    i_pu50NSWeight   = data->add<float>("","pu50NSWeight","F",0);
    i_truePUWeight   = data->add<float>("","truePUWeight","F",0);
    i_btagWeight     = data->add<float>("","btagWeight","F",0);
    i_normWeight     = data->add<float>("","normWeight","F",0);
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
    i_passcscflt     = data->add<bool>("","passcscflt","O",0);
    i_passeebadscflt = data->add<bool>("","passeebadscflt","O",0);
    i_passhbheflt    = data->add<bool>("","passhbheflt","O",0);
    i_passhbhefixflt = data->add<bool>("","passhbhefixflt","O",0);
    i_passhbhefltloose    = data->add<bool>("","passhbhefltloose","O",0);
    i_passhbheflttight    = data->add<bool>("","passhbheflttight","O",0);
    i_genmet         = data->add<float>("","genmet","F",0);
    i_bosonpt        = data->add<float>("","bosonpt","F",0);
    i_bosoneta       = data->add<float>("","bosoneta","F",0);
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
    i_nsellep        = data->add<int>("","nsellep","I",0);
    i_nctt           = data->add<int>("","nctt","I",0);
    i_ncttstd        = data->add<int>("","ncttstd","I",0);
    i_nfjsdwa        = data->add<int>("","nfjsdwa","I",0);
    i_nfjsdwb        = data->add<int>("","nfjsdwb","I",0);
    i_nfjsdwc        = data->add<int>("","nfjsdwc","I",0);
    i_nfjsdta        = data->add<int>("","nfjsdta","I",0);
    i_nfjsdtb        = data->add<int>("","nfjsdtb","I",0);
    i_ncttsdwa0      = data->add<int>("","ncttsdwa0","I",0);
    i_ncttsdwa1      = data->add<int>("","ncttsdwa1","I",0);
    i_ncttsdwa2      = data->add<int>("","ncttsdwa2","I",0);
    i_ncttsdwa12     = data->add<int>("","ncttsdwa12","I",0);
    i_ncttsdwb       = data->add<int>("","ncttsdwb","I",0);
    i_ncttsdwc       = data->add<int>("","ncttsdwc","I",0);
    i_ncttsdta       = data->add<int>("","ncttsdta","I",0);
    i_ncttsdtb       = data->add<int>("","ncttsdtb","I",0);
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
    i_njets60        = data->add<int>("","njets60","I",0);
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
    i_bclose2lep     = data->add<bool>("","bclose2lep","O",0);
    i_ntopcand       = data->add<unsigned int>("","ntopcand","i",0);
    i_ntoppass       = data->add<unsigned int>("","ntoppass","i",0);
    i_topcandpt      = data->add<float>("","topcandpt","F",0);
    i_topcandeta     = data->add<float>("","topcandeta","F",0);
    i_toppasspt      = data->add<float>("","toppasspt","F",0);
    i_toppasseta     = data->add<float>("","toppasseta","F",0);
    i_topcandrawmass       = data->add<float>("","topcandrawmass","F",0);
    i_topcandtrimmedmass   = data->add<float>("","topcandtrimmedmass","F",0);
    i_topcandprunedmass    = data->add<float>("","topcandprunedmass","F",0);
    i_topcandsoftdropmass  = data->add<float>("","topcandsoftdropmass","F",0);
    i_topcandcmstoptagmass = data->add<float>("","topcandcmstoptagmass","F",0);
    i_topcandtau1 = data->add<float>("","topcandtau1","F",0);
    i_topcandtau2 = data->add<float>("","topcandtau2","F",0);
    i_topcandtau3 = data->add<float>("","topcandtau3","F",0);
    i_topcandnsubjets = data->add<int>("","topcandnsubjets","I",0);
    i_topcandminmass = data->add<float>("","topcandminmass","F",0);

  }

  void fillEventInfo(TreeWriterData* data, BaseTreeAnalyzer* ana, int randomLepton = 0, bool lepAddedBack = false, MomentumF* metn = 0) {
    data->fill<unsigned int>(i_run, ana->run);
    data->fill<unsigned int>(i_lumi, ana->lumi);
    data->fill<unsigned int>(i_event, ana->event);
    data->fill<bool >(i_ismc, ana->isMC());
    data->fill<float>(i_weight, ana->weight);
    data->fill<float>(i_puWeight,    ana->eventCorrections.getPUWeight());
    data->fill<float>(i_pu50NSWeight,    ana->eventCorrections.get50NSPUWeight());
    data->fill<float>(i_truePUWeight,    ana->eventCorrections.getTruePUWeight());
    data->fill<float>(i_btagWeight, ana->bTagCorrections.getBTagByEvtWeight());
    data->fill<float>(i_normWeight,  ana->eventCorrections.getNormWeight());
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
    data->fill<int  >(i_nvetolep, ana->nVetoedLeptons);

    int nVetoEle = 0; int nVetoMu = 0;
    for(auto i: ana->vetoedLeptons){
		  if(fabs(i->pdgid()) == 11) nVetoEle++;
		  if(fabs(i->pdgid()) == 13) nVetoMu++;
    }
    data->fill<int  >(i_nvetomu, nVetoMu);
    data->fill<int  >(i_nvetolele, nVetoEle);
    data->fill<int  >(i_nsellep, ana->nSelLeptons);
    data->fill<int  >(i_nctt, ana->cttTops.size());
    int ncttstd    = 0;
    int ncttsdwa0  = 0;
    int ncttsdwa1  = 0;
    int ncttsdwa2  = 0;
    int ncttsdwa12 = 0;
    int ncttsdwb   = 0;
    int ncttsdwc   = 0;
    int ncttsdta   = 0;
    int ncttsdtb   = 0;

    topCands.clear();
    topsPass.clear();
    for(auto* ctt : ana->cttTops) {

      LorentzVector tmpVecCand; tmpVecCand = ctt->p4();
      topCands.push_back(tmpVecCand);

      if(passCTTSelection(ctt)) { ncttstd++;
	LorentzVector tmpVec; tmpVec = ctt->p4();
	topsPass.push_back(tmpVec);
      }

      if (passSoftDropTaggerCTT(ctt,  60., 99999., -1, 10  )) { ++ncttsdwa0;  }
      if (passSoftDropTaggerCTT(ctt,  60., 99999.,  2, 10  )) { ++ncttsdwa1;  }
      if (passSoftDropTaggerCTT(ctt,  60., 99999., -1,  0.6)) { ++ncttsdwa2;  }
      if (passSoftDropTaggerCTT(ctt,  60., 99999.,  2,  0.6)) { ++ncttsdwa12; }
      if (passSoftDropTaggerCTT(ctt,  60.,   120., -1, 10  )) { ++ncttsdwb;   }
      if (passSoftDropTaggerCTT(ctt,  60.,   150., -1, 10  )) { ++ncttsdwc;   }
      if (passSoftDropTaggerCTT(ctt, 150., 99999., -1, 10  )) { ++ncttsdta;   }
      if (passSoftDropTaggerCTT(ctt, 150.,   250., -1, 10  )) { ++ncttsdtb;   }
    }
    data->fill<int  >(i_ncttstd   , ncttstd   );
    data->fill<int  >(i_ncttsdwa0 , ncttsdwa0 );
    data->fill<int  >(i_ncttsdwa1 , ncttsdwa1 );
    data->fill<int  >(i_ncttsdwa2 , ncttsdwa2 );
    data->fill<int  >(i_ncttsdwa12, ncttsdwa12);
    data->fill<int  >(i_ncttsdwb  , ncttsdwb  );
    data->fill<int  >(i_ncttsdwc  , ncttsdwc  );
    data->fill<int  >(i_ncttsdta  , ncttsdta  );
    data->fill<int  >(i_ncttsdtb  , ncttsdtb  );
    int nfjsdwa = 0;
    int nfjsdwb = 0;
    int nfjsdwc = 0;
    int nfjsdta = 0;
    int nfjsdtb = 0;
    for(auto* fj : ana->fatJets) {
      if(passSoftDropTaggerFJ(fj,  60., 99999.)) { ++nfjsdwa; } //if (passSoftDropTaggerFJ(fj,60.,99999.))  { ++nfjsdwa; }
      if(passSoftDropTaggerFJ(fj,  60.,   120.)) { ++nfjsdwb; } //if (passSoftDropTaggerFJ(fj,60.,120.))    { ++nfjsdwb; }
      if(passSoftDropTaggerFJ(fj,  60.,   150.)) { ++nfjsdwc; } //if (passSoftDropTaggerFJ(fj,60.,150.))    { ++nfjsdwc; }
      if(passSoftDropTaggerFJ(fj, 150., 99999.)) { ++nfjsdta; } //if (passSoftDropTaggerFJ(fj,150.,99999.)) { ++nfjsdta; }
      if(passSoftDropTaggerFJ(fj, 150.,   250.)) { ++nfjsdtb; } //if (passSoftDropTaggerFJ(fj,150.,250.))   { ++nfjsdtb; }
    }
    data->fill<int  >(i_nfjsdwa, nfjsdwa);
    data->fill<int  >(i_nfjsdwb, nfjsdwb);
    data->fill<int  >(i_nfjsdwc, nfjsdwc);
    data->fill<int  >(i_nfjsdta, nfjsdta);
    data->fill<int  >(i_nfjsdtb, nfjsdtb);

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

    if(ana->nSelLeptons > 0) {
      MomentumF* lep = new MomentumF(ana->selectedLeptons.at(randomLepton)->p4());
      MomentumF* W = new MomentumF(ana->selectedLeptons.at(randomLepton)->p4() + ana->met->p4());
      data->fill<float>(i_absdphilepw, fabs(PhysicsUtilities::deltaPhi(*W, *lep)) );
      data->fill<float>(i_leptonpt, lep->pt());
      data->fill<float>(i_leptoneta, lep->eta());
      data->fill<int  >(i_leptonpdgid, ana->selectedLeptons.at(randomLepton)->pdgid());
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
      data->fill<float>(i_annulus, lep->pt() * ana->selectedLeptons.at(randomLepton)->annulusactivity());
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
        int index2 = (randomLepton==0) ? 1 : 0;
        auto* lep2 = ana->selectedLeptons.at(index2);
        data->fill<float>(i_lepton2pt, lep2->pt());
        data->fill<float>(i_lepton2eta, lep2->eta());
        data->fill<int  >(i_lepton2pdgid, lep2->pdgid());
        MomentumF* dilep = new MomentumF(lep->p4() + lep2->p4());
        data->fill<float>(i_dileppt, dilep->pt());
        data->fill<float>(i_dilepeta, dilep->eta());
        data->fill<float>(i_dilepmass, dilep->mass());
      }
    }

    data->fill<bool>(i_passcscflt,ana->evtInfoReader.cscFlt);
    data->fill<bool>(i_passeebadscflt,ana->evtInfoReader.eeBadSCFlt);
    data->fill<bool>(i_passhbheflt,ana->evtInfoReader.hbheFlt);
    data->fill<bool>(i_passhbhefixflt,ana->evtInfoReader.hbheFixFlt);
    data->fill<bool>(i_passhbhefltloose,ana->evtInfoReader.hbheFltR2Loose);
    data->fill<bool>(i_passhbheflttight,ana->evtInfoReader.hbheFltR2Tight);

    int nGoodGenMu = 0; int nGoodGenEle = 0; int nPromptTaus = 0;
    if(isMC) {
      for(auto* p : ana->genParts) {
        const GenParticleF * genPartMom = 0;
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
    }
    data->fill<int  >(i_ngoodgenmu, nGoodGenMu);
    data->fill<int  >(i_ngoodgenele, nGoodGenEle);
    data->fill<int  >(i_npromptgentau, nPromptTaus);
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


  void fillTopTagInfo(TreeWriterData* data, BaseTreeAnalyzer* ana, vector<RecoJetF*> jets) {

    bool bClose2Lep_ = false;    
    unsigned int ntopcand_ = 0;   
    unsigned int ntoppass_ = 0;
    float topCandPt_ = -9.; float topCandEta_ = -9.;
    float topPassPt_ = -9.; float topPassEta_ = -9.;

    float topcandrawmass_       = -9.;
    float topcandtrimmedmass_   = -9.;
    float topcandprunedmass_    = -9.;
    float topcandsoftdropmass_  = -9.;
    float topcandcmstoptagmass_ = -9.;
    float topcandtau1_ = -9.;
    float topcandtau2_ = -9.;
    float topcandtau3_ = -9.;
    int topcandnsubjets_ = -1;
    float topcandminmass_ = -9.;

    
    vector<LorentzVector> csvmjets;
    for(auto* j : jets) {

      if(j->csv() > defaults::CSV_MEDIUM) {
	LorentzVector tmpVecCSVMJets; tmpVecCSVMJets = j->p4();
	csvmjets.push_back(tmpVecCSVMJets); }
    }


    if (ana->nSelLeptons == 0) {

      unsigned int countTopTags = 0;
      unsigned int indx = 99;
      for (auto* ctt : ana->cttTops) {

	float maxFatJetPt_ = -1.;
	float fatJetPt_ = ctt->p4().pt();
	if (fatJetPt_>maxFatJetPt_) { indx = countTopTags; }
	
	++countTopTags; 
      }

      if (indx<99) {
	ntopcand_ = 1;
	topCandPt_ = ana->cttTops[indx]->p4().pt(); topCandEta_ = ana->cttTops[indx]->p4().eta();
	topcandrawmass_ = ana->cttTops[indx]->topRawMass();
	topcandtrimmedmass_ = ana->cttTops[indx]->topTrimmedMass();
	topcandprunedmass_ = ana->cttTops[indx]->topPrunedMass();
	topcandsoftdropmass_ = ana->cttTops[indx]->topSoftDropMass();
	topcandcmstoptagmass_ = ana->cttTops[indx]->topCmsTopTagMass(); 
	topcandtau1_ = ana->cttTops[indx]->topTau1();
	topcandtau2_ = ana->cttTops[indx]->topTau2();
	topcandtau3_ = ana->cttTops[indx]->topTau3();
	topcandnsubjets_ = ana->cttTops[indx]->topNsubJets();
	topcandminmass_ = ana->cttTops[indx]->topMinMass();

	if (passCTTSelection(ana->cttTops[indx])) { 
	  ntoppass_ = 1;
	  topPassPt_ = ana->cttTops[indx]->p4().pt(); topPassEta_ = ana->cttTops[indx]->p4().eta(); }
      }     
      
    } // end of zero leptons


    if (ana->nSelLeptons > 0) {
      MomentumF* lep = new MomentumF(ana->selectedLeptons.at(0)->p4());

      for (unsigned int i0=0; i0<csvmjets.size(); ++i0) {
	float dRLepBJet_ = PhysicsUtilities::deltaR(*lep,csvmjets[i0]);
	if (dRLepBJet_<(3.14/2.)) { bClose2Lep_ = true; } 
      }

      unsigned int countTopTags = 0;
      unsigned int indx = 99;
      for (auto* ctt : ana->cttTops) {

	float dRLepCTT_ = PhysicsUtilities::deltaR(*lep,ctt->p4());
	float maxFatJetPt_ = -1.;

	if (dRLepCTT_>=(3.14/2.)) { 
	  float fatJetPt_ = ctt->p4().pt();
	  if (fatJetPt_>maxFatJetPt_) { indx = countTopTags; }
	}

	++countTopTags; 
      }
      
      if (indx<99) { 
	ntopcand_ = 1;
	topCandPt_ = ana->cttTops[indx]->p4().pt(); topCandEta_ = ana->cttTops[indx]->p4().eta();
	topcandrawmass_ = ana->cttTops[indx]->topRawMass();
	topcandtrimmedmass_ = ana->cttTops[indx]->topTrimmedMass();
	topcandprunedmass_ = ana->cttTops[indx]->topPrunedMass();
	topcandsoftdropmass_ = ana->cttTops[indx]->topSoftDropMass();
	topcandcmstoptagmass_ = ana->cttTops[indx]->topCmsTopTagMass();
	topcandtau1_ = ana->cttTops[indx]->topTau1();
	topcandtau2_ = ana->cttTops[indx]->topTau2();
	topcandtau3_ = ana->cttTops[indx]->topTau3();
	topcandnsubjets_ = ana->cttTops[indx]->topNsubJets();
	topcandminmass_ = ana->cttTops[indx]->topMinMass();

	if (passCTTSelection(ana->cttTops[indx])) { 
	  ntoppass_ = 1;
	  topPassPt_ = ana->cttTops[indx]->p4().pt(); topPassEta_ = ana->cttTops[indx]->p4().eta(); }
	
      }     
      
    } // end of at least one lepton
    

    data->fill<bool>(i_bclose2lep, bClose2Lep_);
    data->fill<unsigned int>(i_ntopcand, ntopcand_);
    data->fill<unsigned int>(i_ntoppass, ntoppass_);
    data->fill<float>(i_topcandpt, topCandPt_);
    data->fill<float>(i_topcandrawmass, topcandrawmass_);
    data->fill<float>(i_topcandtrimmedmass, topcandtrimmedmass_);
    data->fill<float>(i_topcandprunedmass, topcandprunedmass_);
    data->fill<float>(i_topcandsoftdropmass, topcandsoftdropmass_);
    data->fill<float>(i_topcandcmstoptagmass, topcandcmstoptagmass_);
    data->fill<float>(i_topcandtau1, topcandtau1_);
    data->fill<float>(i_topcandtau2, topcandtau2_);
    data->fill<float>(i_topcandtau3, topcandtau3_);
    data->fill<int>(i_topcandnsubjets, topcandnsubjets_);
    data->fill<float>(i_topcandminmass, topcandminmass_);
    data->fill<float>(i_topcandeta, topCandEta_);
    data->fill<float>(i_toppasspt, topPassPt_);
    data->fill<float>(i_toppasseta, topPassEta_);
    
  } // end of fillTopTagInfo


  void fillJetInfo(TreeWriterData* data, vector<RecoJetF*> jets, vector<RecoJetF*> bjets, MomentumF* met) {

    int njets60 = 0, njets30 = 0;
    int ntbjets = 0, nmbjets = 0, nlbjets = 0, nbjets30 = 0;
    for(auto* j : jets) {
      if(j->pt() > 60.0) njets60++;
      if(j->pt() > 30.0) njets30++;
      if(j->csv() > defaults::CSV_LOOSE)  nlbjets++;
      if(j->csv() > defaults::CSV_MEDIUM) nmbjets++;
      if(j->csv() > defaults::CSV_TIGHT)  ntbjets++;
      if(j->csv() > defaults::CSV_MEDIUM && j->pt() > 30.0) nbjets30++;
    }
    data->fill<int>(i_njets,    int(jets.size()));
    data->fill<int>(i_njets30,  njets30);
    data->fill<int>(i_njets60,  njets60);
    data->fill<int>(i_nbjets,   int(bjets.size()));
    data->fill<int>(i_nbjets30, nbjets30);
    data->fill<int>(i_nlbjets,  nlbjets);
    data->fill<int>(i_nmbjets,  nmbjets);
    data->fill<int>(i_ntbjets,  ntbjets);
    data->fill<float>(i_ht,   JetKinematics::ht(jets, 20.0, 2.4));
    data->fill<float>(i_ht30, JetKinematics::ht(jets, 30.0, 2.4));
    data->fill<float>(i_ht40, JetKinematics::ht(jets, 40.0, 2.4));

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

    const double metcut_   = 175.0 ;
    const int    minnjets_ =   2   ;

    TreeFiller filler;

    virtual ~ZeroLeptonAnalyzer() {}

    void book() {
      filler.book(&data);
    }

    bool fillEvent() {

      if(!goodvertex) return false;

      //if(nVetoedLeptons > 0)  return false;
      //if(nVetoedTracks  > 0)  return false;

      if(met->pt() < metcut_  ) return false;
      if(nJets     < minnjets_) return false;

      filler.fillEventInfo(&data, this);
      filler.fillJetInfo  (&data, jets, bJets, met);
      filler.fillTopTagInfo(&data,this, jets);
      return true;
    }

};

#endif
