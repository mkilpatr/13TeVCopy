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

struct hettTop {
  RecoJetF jet1, jet2, jet3; // pt-ordered subjets
  RecoJetF jet123;           // hett top, p4 is sum of p4 of j1,j2,j3
  float    m12, m13, m23;      // -1 if not relevant: trijet(y/y/y), dijet(y/n/n), monojet(n/n/n)
  float    m123;    // NOT USED except to compare approximation m123 ~ m3jet in trijets. m123 is the more natural.
  float    m3jet;   // always filled, with: m3jet (trijet), m12 (W dijet + aux jet), or m1 (monojet)
  bool     trijet;  // true iff trijet: jets 1,2,3 filled
  bool     dijet;   // true iff W "dijet": jet1 = W "dijet", jet2 = auxillary jet
  bool     monojet; // true iff top "monojet": jet1 = top
};
std::vector<hettTop > hettTops= {}; // objects, not ptrs, otherwise there's some funny business

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
//  size i_passcscflt; // to be added back later
  size i_passcscbeamhaloflt;
  size i_passeebadscflt;
  size i_passeebadsc4flt;
  size i_passhbheisoflt;
  size i_passhbhefltloose;
  size i_passhbheflttight;
  size i_passaddmetflts;
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

  // hett top tagging
  // matched
  size i_hettnmatchedrecotops;
  size i_hetthardmatchednsubjets;
  size i_hetthardmatchedgentoppt;
  size i_hetthardmatchedgentopeta;
  size i_hetthardmatchedrecopt;
  size i_hetthardmatchedrecoeta;
  size i_hetthardmatchedrecom3jet;
  size i_hetthardmatchedrecom12;
  size i_hetthardmatchedrecom13;
  size i_hetthardmatchedrecom23;
  size i_hetthardmatchedrecoj1pt;
  size i_hetthardmatchedrecoj1eta;
  size i_hetthardmatchedrecoj2pt;
  size i_hetthardmatchedrecoj2eta;
  size i_hetthardmatchedrecoj3pt;
  size i_hetthardmatchedrecoj3eta;
  // unmatched
  size i_hettnrecotops;
  size i_hetthardnsubjets;
  size i_hetthardgentoppt;
  size i_hetthardgentopeta;
  size i_hetthardrecopt;
  size i_hetthardrecoeta;
  size i_hetthardrecom3jet;
  size i_hetthardrecom12;
  size i_hetthardrecom13;
  size i_hetthardrecom23;
  size i_hetthardrecoj1pt;
  size i_hetthardrecoj1eta;
  size i_hetthardrecoj2pt;
  size i_hetthardrecoj2eta;
  size i_hetthardrecoj3pt;
  size i_hetthardrecoj3eta; 

  // below is for top / w tagging and SF extraction
  size i_absdphilepmet;
  size i_sfbclose2lep;
  size i_sfncttcand  ;
  size i_sfcttcandpt ;
  size i_sfcttcandeta;
  size i_sfcttcandrawmass;
  size i_sfcttcandtrimmedmass;
  size i_sfcttcandprunedmass;
  size i_sfcttcandsoftdropmass;
  size i_sfcttcandcmstoptagmass;
  size i_sfcttcandtau1;
  size i_sfcttcandtau2;
  size i_sfcttcandtau3;
  size i_sfcttcandnsubjets;
  size i_sfcttcandminmass;
  size i_sfncttpass  ;
  size i_sfcttpasspt ;
  size i_sfcttpasseta;
  size i_sfnfjcand  ;
  size i_sffjcandpt ;
  size i_sffjcandeta;
  size i_sfntopsdpass  ;
  size i_sftopsdpasspt ;
  size i_sftopsdpasseta;
  size i_sfnwsdpass  ;
  size i_sfwsdpasspt ;
  size i_sfwsdpasseta;
  size i_wcandmass;
  size i_wcandpt;
  size i_wcandeta;
  size i_wpasspt;
  size i_wpasseta;
  size i_sfcttcandptdr1;
  size i_sfcttcandptdr2;
  size i_sfcttcandptdr3;
  size i_sfcttcandetadr1;
  size i_sfcttcandetadr2;
  size i_sfcttcandetadr3;
  size i_sfcttpassptdr1;
  size i_sfcttpassptdr2;
  size i_sfcttpassptdr3;
  size i_sfcttpassetadr1;
  size i_sfcttpassetadr2;
  size i_sfcttpassetadr3;
  size i_nhadronicgentops;
  size i_sfcttcandptdr6;
  size i_sfcttpassptdr6;
  size i_sfcttcandetadr6;
  size i_sfcttpassetadr6;
  size i_sfcttcandptdr6hard;
  size i_sfcttcandetadr6hard;
  size i_sfcttpassptdr6hard;
  size i_sfcttpassetadr6hard;

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
//    i_passcscflt     = data->add<bool>("","passcscflt","O",0);
    i_passcscbeamhaloflt = data->add<bool>("","passcscbeamhaloflt","O",0);
    i_passeebadscflt = data->add<bool>("","passeebadscflt","O",0);
    i_passeebadsc4flt = data->add<bool>("","passeebadsc4flt","O",0);
    i_passhbheisoflt  = data->add<bool>("","passhbheisoflt","O",0);
    i_passhbhefltloose    = data->add<bool>("","passhbhefltloose","O",0);
    i_passhbheflttight  = data->add<bool>("","passhbheflttight","O",0);
    i_passaddmetflts    = data->add<bool>("","passaddmetflts","O",0);
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

    // hett
    i_hettnmatchedrecotops         = data->add<int  >("","hettnmatchedrecotops","I",0);
    i_hetthardmatchednsubjets     = data->add<int  >("","hetthardmatchednsubjets","I",0);
    i_hetthardmatchedgentoppt     = data->add<float>("","hetthardmatchedgentoppt","F",0);
    i_hetthardmatchedgentopeta    = data->add<float>("","hetthardmatchedgentopeta","F",0);
    i_hetthardmatchedrecopt       = data->add<float>("","hetthardmatchedrecopt","F",0);
    i_hetthardmatchedrecoeta      = data->add<float>("","hetthardmatchedrecoeta","F",0);
    i_hetthardmatchedrecom3jet    = data->add<float>("","hetthardmatchedrecom3jet","F",0);
    i_hetthardmatchedrecom12      = data->add<float>("","hetthardmatchedrecom12","F",0);
    i_hetthardmatchedrecom13      = data->add<float>("","hetthardmatchedrecom13","F",0);
    i_hetthardmatchedrecom23      = data->add<float>("","hetthardmatchedrecom23","F",0);
    i_hetthardmatchedrecoj1pt     = data->add<float>("","hetthardmatchedrecoj1pt","F",0);
    i_hetthardmatchedrecoj1eta    = data->add<float>("","hetthardmatchedrecoj1eta","F",0);
    i_hetthardmatchedrecoj2pt     = data->add<float>("","hetthardmatchedrecoj2pt","F",0);
    i_hetthardmatchedrecoj2eta    = data->add<float>("","hetthardmatchedrecoj2eta","F",0);
    i_hetthardmatchedrecoj3pt     = data->add<float>("","hetthardmatchedrecoj3pt","F",0);
    i_hetthardmatchedrecoj3eta    = data->add<float>("","hetthardmatchedrecoj3eta","F",0);

    i_hettnrecotops               = data->add<int  >("","hettnrecotops","I",0);
    i_hetthardnsubjets            = data->add<int  >("","hetthardnsubjets","I",0);
    i_hetthardgentoppt            = data->add<float>("","hetthardgentoppt","F",0);
    i_hetthardgentopeta           = data->add<float>("","hetthardgentopeta","F",0); 
    i_hetthardrecopt              = data->add<float>("","hetthardrecopt","F",0);
    i_hetthardrecoeta             = data->add<float>("","hetthardrecoeta","F",0);
    i_hetthardrecom3jet           = data->add<float>("","hetthardrecom3jet","F",0);
    i_hetthardrecom12             = data->add<float>("","hetthardrecom12","F",0);
    i_hetthardrecom13             = data->add<float>("","hetthardrecom13","F",0);
    i_hetthardrecom23             = data->add<float>("","hetthardrecom23","F",0);
    i_hetthardrecoj1pt            = data->add<float>("","hetthardrecoj1pt","F",0);
    i_hetthardrecoj1eta           = data->add<float>("","hetthardrecoj1eta","F",0);
    i_hetthardrecoj2pt            = data->add<float>("","hetthardrecoj2pt","F",0);
    i_hetthardrecoj2eta           = data->add<float>("","hetthardrecoj2eta","F",0);
    i_hetthardrecoj3pt            = data->add<float>("","hetthardrecoj3pt","F",0);
    i_hetthardrecoj3eta           = data->add<float>("","hetthardrecoj3eta","F",0);

    // ctt
    i_absdphilepmet       = data->add<float>("","absdphilepmet","F",0);
    i_sfbclose2lep     = data->add<bool>("","sfbclose2lep","O",0);
    i_sfncttcand       = data->add<unsigned int>("","sfncttcand","i",0);
    i_sfcttcandpt      = data->add<float>("","sfcttcandpt","F",0);
    i_sfcttcandeta     = data->add<float>("","sfcttcandeta","F",0);
    i_sfcttcandrawmass       = data->add<float>("","sfcttcandrawmass","F",0);
    i_sfcttcandtrimmedmass   = data->add<float>("","sfcttcandtrimmedmass","F",0);
    i_sfcttcandprunedmass    = data->add<float>("","sfcttcandprunedmass","F",0);
    i_sfcttcandsoftdropmass  = data->add<float>("","sfcttcandsoftdropmass","F",0);
    i_sfcttcandcmstoptagmass = data->add<float>("","sfcttcandcmstoptagmass","F",0);
    i_sfcttcandtau1 = data->add<float>("","sfcttcandtau1","F",0);
    i_sfcttcandtau2 = data->add<float>("","sfcttcandtau2","F",0);
    i_sfcttcandtau3 = data->add<float>("","sfcttcandtau3","F",0);
    i_sfcttcandnsubjets = data->add<int>("","sfcttcandnsubjets","I",0);
    i_sfcttcandminmass = data->add<float>("","sfcttcandminmass","F",0);
    i_sfncttpass       = data->add<unsigned int>("","sfncttpass","i",0);
    i_sfcttpasspt      = data->add<float>("","sfcttpasspt","F",0);
    i_sfcttpasseta     = data->add<float>("","sfcttpasseta","F",0);
    i_sfnfjcand       = data->add<unsigned int>("","sfnfjcand","i",0);
    i_sffjcandpt      = data->add<float>("","sffjcandpt","F",0);
    i_sffjcandeta     = data->add<float>("","sffjcandeta","F",0);
    i_sfntopsdpass       = data->add<unsigned int>("","sfntopsdpass","i",0);
    i_sftopsdpasspt      = data->add<float>("","sftopsdpasspt","F",0);
    i_sftopsdpasseta     = data->add<float>("","sftopsdpasseta","F",0);
    i_sfnwsdpass       = data->add<unsigned int>("","sfnwsdpass","i",0);
    i_sfwsdpasspt      = data->add<float>("","sfwsdpasspt","F",0);
    i_sfwsdpasseta     = data->add<float>("","sfwsdpasseta","F",0);
    i_wcandmass = data->addMulti<float>("","wcandmass",0);
    i_wcandpt   = data->addMulti<float>("","wcandpt",0);
    i_wcandeta  = data->addMulti<float>("","wcandeta",0);
    i_wpasspt   = data->addMulti<float>("","wpasspt",0);
    i_wpasseta  = data->addMulti<float>("","wpasseta",0);
    i_sfcttcandptdr1 = data->addMulti<float>("","sfcttcandptdr1",0);
    i_sfcttcandptdr2= data->addMulti<float>("","sfcttcandptdr2",0);
    i_sfcttcandptdr3= data->addMulti<float>("","sfcttcandptdr3",0);
    i_sfcttcandetadr1= data->addMulti<float>("","sfcttcandetadr1",0);
    i_sfcttcandetadr2= data->addMulti<float>("","sfcttcandetadr2",0);
    i_sfcttcandetadr3= data->addMulti<float>("","sfcttcandetadr3",0);
    i_sfcttpassptdr1= data->addMulti<float>("","sfcttpassptdr1",0);
    i_sfcttpassptdr2= data->addMulti<float>("","sfcttpassptdr2",0);
    i_sfcttpassptdr3= data->addMulti<float>("","sfcttpassptdr3",0);
    i_sfcttpassetadr1= data->addMulti<float>("","sfcttpassetadr1",0);
    i_sfcttpassetadr2= data->addMulti<float>("","sfcttpassetadr2",0);
    i_sfcttpassetadr3= data->addMulti<float>("","sfcttpassetadr3",0);
    i_sfcttcandptdr6hard = data->addMulti<float>("","sfcttcandptdr6hard",0);
    i_sfcttcandetadr6hard = data->addMulti<float>("","sfcttcandetadr6hard",0);
    i_sfcttpassptdr6hard = data->addMulti<float>("","sfcttpassptdr6hard",0);
    i_sfcttpassetadr6hard =data->addMulti<float>("","sfcttpassetadr6hard",0);
    i_nhadronicgentops = data->add<int>("","nhadronicgentops","i",0);
    i_sfcttcandptdr6 = data->addMulti<float>("","sfcttcandptdr6",0);
    i_sfcttcandetadr6= data->addMulti<float>("","sfcttcandetadr6",0);
    i_sfcttpassptdr6= data->addMulti<float>("","sfcttpassptdr6",0);
    i_sfcttpassetadr6= data->addMulti<float>("","sfcttpassetadr6",0);

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


    int nfjsd60_   = 0;
    int nfjpr60_   = 0;
    for(auto* fj : ana->fatJets) {
      if (passSoftDropTaggerFJ(fj,60.,100000.)) { ++nfjsd60_; }
      if (passPrunedTaggerFJ(fj,60.,100000.))   { ++nfjpr60_; }

    }
    data->fill<int  >(i_nfjsd60, nfjsd60_);
    data->fill<int  >(i_nfjpr60, nfjpr60_);

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

//    data->fill<bool>(i_passcscflt,ana->evtInfoReader.cscFlt);
    data->fill<bool>(i_passcscbeamhaloflt, ana->evtInfoReader.cscBeamHaloFlt);
    data->fill<bool>(i_passeebadscflt,ana->evtInfoReader.eeBadSCFlt);
    data->fill<bool>(i_passeebadsc4flt,ana->evtInfoReader.eeBadSC4Flt);
    data->fill<bool>(i_passhbheisoflt,ana->evtInfoReader.hbheIsoFlt);
    data->fill<bool>(i_passhbhefltloose,ana->evtInfoReader.hbheFltR2Loose);
    data->fill<bool>(i_passhbheflttight,ana->evtInfoReader.hbheFltR2Tight);
    data->fill<bool>(i_passaddmetflts,(ana->isMC() || (ana->evtInfoReader.badResolutionTrkFlt && ana->evtInfoReader.muonBadTrkFlt)));

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



  // UNTESTED perform loose cone match to, eg CTT-tagged tops or FatJets
  // suggest 0.6 for AK8 jets and 0.4 for AK4
  template<typename T> // hettTop, CMSTop, MomentumF, FatJetF, RecoJetF, LorentzVector, so many!
  hettTop* isNearHettTaggedTop(T* jet) {
    for(int i = 0 ; i < hettTops.size() ; ++i) {
      hettTop htop = hettTops[i];
      float htoppt_ = htop.jet123.p4().pt();
      if (PhysicsUtilities::deltaR(htop.jet123, *jet) < 0.6) return &hettTops[i];
    }
    return 0; // esp. if fillHettInfo hasn't been called
  }
  // UNTESTED same, for selected CTTTops
  template<typename T>
  CMSTopF* isNearCTTSelectedTop(BaseTreeAnalyzer* ana, T* jet) {
    for(int i = 0 ; i < ana->selectedCTTTops.size() ; ++i) {
      CMSTopF* cmstop = ana->selectedCTTTops[i];
      float cmstoppt_ = cmstop->p4().pt();
      if (PhysicsUtilities::deltaR(cmstop, *jet) < 0.6) return ana->selectedCTTTops[i];
    }
    return 0;
  }

  // fill HETT branches
  void fillHettInfo(TreeWriterData* data, BaseTreeAnalyzer* ana, vector<RecoJetF*> jets, vector<hettTop> hettTops) {
    bool dbg = false;
    if(dbg) cout << endl << endl << endl <<  "**** NEW EVENT ***" << endl;

    float mW   =  80.4;
    float mTop = 173.4;
    float rMin = 0.85*mW/mTop;
    float rMax = 1.25*mW/mTop;

    if(dbg) cout << "Filling HETT info" << endl;
    for(auto* jet : jets) { // print ak4 info
      if(dbg) cout   
        << "AK4 jets pt,eta: " << left
        << setw(10) << jet->pt() 
        << setw(10) << jet->eta() << endl;
    }

    RecoJetF jet1, jet2, jet3;
    int nJets = jets.size();
    if(dbg) cout << "nJets: " << nJets << endl;

    // unique ak4 triplets
    for (int i = 0 ; i < nJets-2 ; ++i) {
      for(int j = i+1 ; j < nJets-1 ; ++j) {
        for(int k = j+1 ; k < nJets ; ++k) {
          if(dbg) cout << "triplet index: " << i << " " << j << " " << k << endl;

          jet1 = jets[i]->p4(); jet2 = jets[j]->p4(); jet3 = jets[k]->p4();
          RecoJetF jet123 = jet1.p4() + jet2.p4() + jet3.p4();
          float m12  = (jet1.p4() + jet2.p4()).mass();
          float m13  = (jet1.p4() + jet3.p4()).mass();
          float m23  = (jet2.p4() + jet3.p4()).mass();     
          float m123 = jet123.mass(); // note m123 ~ m3jet for p_i^2 ~ 0. they use m3jet.
          float m3jet = sqrt(m12*m12+m13*m13+m23*m23);
          if(dbg) cout << "jet1: " << jet1.p4() << endl;
          if(dbg) cout << "jet2: " << jet2.p4() << endl;
          if(dbg) cout << "jet3: " << jet3.p4() << endl;
          if(dbg) cout << "jet123: " << jet123.p4() << endl;          
          if(dbg) cout << "m3jet: " << m3jet << endl;
          if(dbg) cout << "m123: " << m123 << endl;
          if(dbg) cout << "m12: " << m12 << endl;
          if(dbg) cout << "m23: " << m23 << endl;
          if(dbg) cout << "m13: " << m13 << endl;

          // condition (i)
          if(dbg) cout << "deltaR 123,1: " << PhysicsUtilities::deltaR(jet123,jet1) << endl;
          if(dbg) cout << "deltaR 123,2: " << PhysicsUtilities::deltaR(jet123,jet2) << endl;
          if(dbg) cout << "deltaR 123,3: " << PhysicsUtilities::deltaR(jet123,jet3) << endl;
          if((PhysicsUtilities::deltaR(jet123,jet1) > 1.5) ||
             (PhysicsUtilities::deltaR(jet123,jet2) > 1.5) || 
             (PhysicsUtilities::deltaR(jet123,jet3) > 1.5)) continue;
          
          // condition (ii)
          if((m3jet > 250.) || (m3jet < 100.)) continue;

          // condition (iii) has subconditions A1 A2 B C    
          bool condA1 = (0.2 < atan(m13/m12)) &
                        (1.3 > atan(m13/m12));
          if(dbg) cout << "condition A1 atan: " << atan(m13/m12) << " " << condA1 << endl;

          bool condA2 = (rMin < m23/m3jet) & 
                        (rMax > m23/m3jet);
          if(dbg) cout << "condition A2 m23/m3jet: " << m23/m3jet << " " << condA2 << endl;
          bool condA = condA1 & condA2;

          // condition B
          bool condB = (pow(rMin,2)*(1+pow(m13/m12,2)) < 1-pow(m23/m3jet,2)) &
                       (pow(rMax,2)*(1+pow(m13/m12,2)) > 1-pow(m23/m3jet,2)) &
                       (m23/m3jet > 0.35);
          if(dbg) cout << "condition B" << endl;
          if(dbg) cout << "LHS: " << pow(rMin,2)*(1+pow(m13/m12,2)) << endl;
          if(dbg) cout << "CEN: " << 1-pow(m23/m3jet,2) << endl;
          if(dbg) cout << "RHS: " << pow(rMax,2)*(1+pow(m13/m12,2)) << endl;
          if(dbg) cout << "AN ratio m23/m3jet: " << m23/m3jet << endl;
          if(dbg) cout << "condB: " << condB << endl; 
          
          // condition C is condition B with jet 2 <-> 3
          bool condC = (pow(rMin,2)*(1+pow(m12/m13,2)) < 1-pow(m23/m3jet,2)) &
                       (pow(rMax,2)*(1+pow(m12/m13,2)) > 1-pow(m23/m3jet,2)) &
                       (m23/m3jet > 0.35);
          if(dbg) cout << "condition C" << endl;
          if(dbg) cout << "LHS: " << pow(rMin,2)*(1+pow(m12/m13,2)) << endl;
          if(dbg) cout << "CEN: " << 1-pow(m23/m3jet,2) << endl;
          if(dbg) cout << "RHS: " << pow(rMax,2)*(1+pow(m12/m13,2)) << endl;
          if(dbg) cout << "condC: " << condC << endl;

          // enforce either A or B or C
          if (!(condA || condB || condC)) continue;

          if(dbg) cout << "**Candidate trijet was found!" << endl;
          hettTop cand = {}; // fill and submit this hett candidate          
          cand.trijet = true; cand.dijet = false; cand.monojet = false; // need to set all three for some reason
          cand.jet1 = jet1; cand.jet2 = jet2; cand.jet3 = jet3; cand.jet123 = jet123;
          cand.m12   = m12;   cand.m23   = m23;   cand.m13   = m13;   cand.m123   = m123;   cand.m3jet = m3jet;
          hettTops.push_back(cand);
        }
      }
    }

    if(dbg) {
      cout << endl << "Size of hettTops before trijet cleaning: " << hettTops.size() << endl;
      for(int i = 0 ; i < hettTops.size() ; ++i) { // print hettTops
        cout << "j1 " << hettTops[i].jet1.p4() << endl;
        cout << "j2 " << hettTops[i].jet2.p4() << endl;
        cout << "j3 " << hettTops[i].jet3.p4() << endl;
        cout << "j123 " << hettTops[i].jet123.p4() << endl;
        cout << "m12 " << hettTops[i].m12 << endl;
        cout << "m13 " << hettTops[i].m13 << endl;
        cout << "m23 " << hettTops[i].m23 << endl;
        cout << "m123 " << hettTops[i].m123 << endl;
        cout << "m3jet " << hettTops[i].m3jet << endl;
        cout << "tri/di/mono " << hettTops[i].trijet << " " <<  hettTops[i].dijet << " " << hettTops[i].monojet << endl;
      }
    }

    // remove (all but one of the) trijets who share a jet
    if(dbg) cout << endl << "Cleaning trijets" << endl;
    for(int i = 0 ; i < nJets ; ++i) {
      int prev = -1, toErase = -1;
      for(int j = 0 ; j < hettTops.size() ; ++j) {
        bool isPresent = (jets[i]->p4() == hettTops[j].jet1.p4()) |
			 (jets[i]->p4() == hettTops[j].jet2.p4()) |
                         (jets[i]->p4() == hettTops[j].jet3.p4());       
        if(isPresent && prev > -1 ) { // this jet is in two hettTops
          if(dbg) cout << "Overlap found, trijet indices: " << prev << " " << j << endl;
          toErase = (abs(hettTops[prev].m3jet - mTop) >
                     abs(hettTops[j   ].m3jet - mTop)) ? prev : j; // erase one with largest gap from mTop
        }
        if(isPresent) prev = j;        
      }
      if(toErase > -1) {
        if(dbg) cout << "Erasing trijet with index " << toErase << endl;
        hettTops.erase(hettTops.begin() + toErase); 
        i = 0; // iterate procedure for rare case of > 2 trijets sharing a jet
      }
    }

    if(dbg) {        
      cout << endl <<  "Size of hettTops after trijet cleaning: " << hettTops.size() << endl;
      for(int i = 0 ; i < hettTops.size() ; ++i) { // print hettTops
        cout << "j1 " << hettTops[i].jet1.p4() << endl;
        cout << "j2 " << hettTops[i].jet2.p4() << endl;
        cout << "j3 " << hettTops[i].jet3.p4() << endl;
        cout << "j123 " << hettTops[i].jet123.p4() << endl;
        cout << "m12 " << hettTops[i].m12 << endl;
        cout << "m13 " << hettTops[i].m13 << endl;
        cout << "m23 " << hettTops[i].m23 << endl;
        cout << "m123 " << hettTops[i].m123 << endl;
        cout << "m3jet " << hettTops[i].m3jet << endl;
        cout << "tri/di/mono " << hettTops[i].trijet << " " <<  hettTops[i].dijet << " " << hettTops[i].monojet << endl;
      }
    }

    // find dijets and monojets (boosted scenarios)
    for(auto jet : jets) {
      if(dbg) cout << endl << "Considering jet for mono/dijet: " << jet->mass() << " " << jet->p4() << endl;
      // ignore mono/dijet if already used in a trijet
      bool jetUsed = false;
      for(auto t : hettTops) {
        jetUsed |= ((jet->p4() == t.jet1.p4()) |
                    (jet->p4() == t.jet2.p4()) |
                    (jet->p4() == t.jet3.p4()));
      }
      if(jetUsed) {
        if(dbg) cout << "Jet with this p4 was already in a trijet " << jet->p4() << endl;
        continue;
      }

      // boosted top "monojet", which stands alone
      if ((jet->mass() > 110.) && (jet->mass() < 220.)) {
        if(dbg) cout << "Found monojet with p4 " << jet->p4() << endl;
        hettTop cand = {}; // fill and submit this hett candidate
        cand.monojet = true; cand.dijet = false; cand.trijet = false;
        cand.jet1 = *jet; cand.jet123 = cand.jet1;
        cand.m12 = -1.; cand.m23 = -1.; cand.m13 = -1.; cand.m123 = -1.; 
        cand.m3jet = jet->mass();
        hettTops.push_back(cand);
      }

      // boosted W "dijet", which requires an auxillary jet
      else if ((jet->mass() > 70.) && (jet->mass() < 110.)) {
        if(dbg) cout << "Found potential dijet with p4 " << jet->p4() << endl;
        int iCandAux = -1;
        for(int i = 0 ; i < nJets ; ++i) { // find an auxillary jet for the W
          if(dbg) cout << "Considering aux jet " << jets[i]->p4() << endl;
          if(jets[i]->p4() == jet->p4()) {
            if(dbg) cout << "skipped self jet" << endl;
            continue;
          }
          bool isUsed = false; // aux jet cannot be in a trijet already
          for(auto t : hettTops) {
            isUsed |= ((jets[i]->p4() == t.jet1.p4()) |
                       (jets[i]->p4() == t.jet2.p4()) |
                       (jets[i]->p4() == t.jet3.p4()));
          }
          if(isUsed) {
            if(dbg) cout << "aux jet already used in trijet " << jets[i]->p4() << endl;
            continue;
          }
          // two conditions on dijet + jet system
          float massDijet        = jet->mass();
          float massDijetPlusJet = (jet->p4()+jets[i]->p4()).mass();
          if(dbg) cout << "massDijet and DijetPlusJet: " << massDijet << " " << massDijetPlusJet << endl;
          bool condA = (rMin < massDijet/massDijetPlusJet) & 
                       (rMax > massDijet/massDijetPlusJet);
          bool condB = (100. < massDijetPlusJet) &
                       (250. > massDijetPlusJet);
          if(dbg) cout << "Dijet conditions A, B: " << condA << " " << condB << endl;
          if(condA && condB) {
            iCandAux = i;
            break; // do we want more candidates or to break here?
          }
        }//aux
        if(iCandAux > -1) { // did suitable aux jet exist?
          if(dbg) cout << "Found aux for the dijet" << endl;
          hettTop cand = {}; // fill and submit this hett top candidate
          cand.dijet = true; cand.monojet = false; cand.trijet = false;
          if(dbg) cout << "iCandAux: " << iCandAux << endl;
          if(dbg) cout << "jet1 being filled with " << jet->p4() << endl;
          if(dbg) cout << "jet2 being filled with " << jets[iCandAux]->p4() << endl;
          cand.jet1 = *jet; cand.jet2 = *jets[iCandAux]; cand.jet123 = cand.jet1.p4()+cand.jet2.p4();
          cand.m13 = -1.; cand.m23 = -1.; cand.m123 = -1.;
          cand.m12   = (jet->p4() + jets[iCandAux]->p4()).mass(); 
          cand.m3jet = (jet->p4() + jets[iCandAux]->p4()).mass();
          hettTops.push_back(cand);
        }
      }//ifW
    }//mono/dijets

    if(dbg) {
      cout << endl << "Size of hettTops after mono/dijet discovery: " << hettTops.size() << endl;
      for(int i = 0 ; i < hettTops.size() ; ++i) { // print hettTops
        cout << "j1 " << hettTops[i].jet1.p4() << endl;
        cout << "j2 " << hettTops[i].jet2.p4() << endl;
        cout << "j3 " << hettTops[i].jet3.p4() << endl;
        cout << "j123 " << hettTops[i].jet123.p4() << endl;
        cout << "m12 " << hettTops[i].m12 << endl;
        cout << "m13 " << hettTops[i].m13 << endl;
        cout << "m23 " << hettTops[i].m23 << endl;
        cout << "m123 " << hettTops[i].m123 << endl;
        cout << "m3jet " << hettTops[i].m3jet << endl;
        cout << "tri/di/mono " << hettTops[i].trijet << " " <<  hettTops[i].dijet << " " << hettTops[i].monojet << endl;
      }
    }

    // mc efficiency
    if(dbg) cout << endl << endl << "Efficiency work" << endl;
   
    int   hettnrecotops_       = hettTops.size();
    // properties of hardest (unmatched) hett top (per-jet basis)
    int   hetthardnsubjets_    = -1;
    float hetthardgentoppt_    = -9.; 
    float hetthardgentopeta_   = -9.; 
    float hetthardrecopt_      = -9.; 
    float hetthardrecoeta_     = -9.; 
    float hetthardrecom3jet_   = -9.; 
    float hetthardrecom12_     = -9.; 
    float hetthardrecom13_     = -9.; 
    float hetthardrecom23_     = -9.; 
    float hetthardrecoj1pt_    = -9.; 
    float hetthardrecoj1eta_   = -9.; 
    float hetthardrecoj2pt_    = -9.; 
    float hetthardrecoj2eta_   = -9.; 
    float hetthardrecoj3pt_    = -9.; 
    float hetthardrecoj3eta_   = -9.; 

    // properties of hardest gen-matched hett top (per-jet basis)
    int   hettnmatchedrecotops_     = -1; // number of reco tops matched to hardest gen top
    int   hetthardmatchednsubjets_  = -1;
    float hetthardmatchedgentoppt_  = -9.; 
    float hetthardmatchedgentopeta_ = -9.; 
    float hetthardmatchedrecopt_    = -9.; 
    float hetthardmatchedrecoeta_   = -9.; 
    float hetthardmatchedrecom3jet_ = -9.; 
    float hetthardmatchedrecom12_   = -9.; 
    float hetthardmatchedrecom13_   = -9.; 
    float hetthardmatchedrecom23_   = -9.; 
    float hetthardmatchedrecoj1pt_  = -9.; 
    float hetthardmatchedrecoj1eta_ = -9.; 
    float hetthardmatchedrecoj2pt_  = -9.; 
    float hetthardmatchedrecoj2eta_ = -9.; 
    float hetthardmatchedrecoj3pt_  = -9.; 
    float hetthardmatchedrecoj3eta_ = -9.; 

    // unmatched properties
    // find hardest reco top
    int iHardestHett_ = -1;
//    hettTop* hardHett_ = 0; // fixme so 'hard' object doesn't lose scope out of for{}
    float hardhettpt_ = -99.; // the running hardest reco top pt
    if(dbg) cout << "finding hardest unmatched reco top..." << endl;
//    for (hettTop htop : hettTops) { // fixme --see above
    for(int i = 0 ; i < hettTops.size() ; ++i) {
      hettTop htop = hettTops[i];
      float htoppt_ = htop.jet123.p4().pt();
      if(dbg) cout << "this hett has pt " << htoppt_ << endl;
      if(htoppt_ < hardhettpt_) {
        if(dbg) cout << "harder hett has been found" << endl;
        continue; // we've already found harder reco tops
      }
      if(dbg) cout << "was hardest hett seen " << endl;
      hardhettpt_ = htoppt_; // update hardest reco top pt
//      hardHett_ = htop; // fixme --see above
      iHardestHett_ = i;
    }
    // fill properties of hardest unmatched reco top
    if(iHardestHett_ > -1) {
      hettTop hardHett_  = hettTops[iHardestHett_];
      hetthardrecopt_    = hardHett_.jet123.p4().pt();
      hetthardrecoeta_   = hardHett_.jet123.p4().eta();
      hetthardrecom3jet_ = hardHett_.m3jet;
      hetthardrecom12_   = hardHett_.m12;
      hetthardrecom13_   = hardHett_.m13;
      hetthardrecom23_   = hardHett_.m23;
      hetthardrecoj1pt_  = hardHett_.jet1.p4().pt();
      hetthardrecoj1eta_ = hardHett_.jet1.p4().eta();
      hetthardrecoj2pt_  = hardHett_.jet2.p4().pt();
      hetthardrecoj2eta_ = hardHett_.jet2.p4().eta();
      hetthardrecoj3pt_  = hardHett_.jet3.p4().pt();
      hetthardrecoj3eta_ = hardHett_.jet3.p4().eta();      
      if(hardHett_.m13 < 0) { // either dijet or monojet
        if(hardHett_.m12 < 0) { // monojet
          hetthardnsubjets_ = 1;
        } else { // dijet                   
          hetthardnsubjets_ = 2;
        }
      } else { // trijet
        hetthardnsubjets_ = 3;
      }
      if(dbg) cout << "hardest hett has pt: " << hardHett_.jet123.p4().pt() << endl;
      if(dbg) cout << "hardest hett has eta: " << hardHett_.jet123.p4().eta() << endl;
      if(dbg) cout << "hardest hett has phi: " << hardHett_.jet123.p4().phi() << endl;
      if(dbg) cout << "hardest hett has m3jet: " << hardHett_.m3jet << endl;
      if(dbg) cout << "hardest hett nsubjets: " << hetthardnsubjets_ << endl;
    }//if hardHett

    if(ana->isMC()) { 

      // gen-matched properties
      // find hardest gen top
      GenParticleF* hardGenTop_ = 0;
      float hardgentoppt_ = -99.; // the running hardest gen top pt
      if(dbg) cout << "finding hardest gen top" << endl;
      for(auto* p : ana->genParts) {
        if ((abs(p->pdgId())==6) && isGenTopHadronic(p)) { // only want gen tops
          float gentoppt_ = p->p4().pt();
          if(dbg) cout << "this gen top has pt: " << gentoppt_ << endl;
          if (gentoppt_ < hardgentoppt_) {
            if(dbg) cout << "harder gen tops have been found" << endl;
            continue; // we've already found harder gen tops
          }
          hardgentoppt_ = gentoppt_; // update hardest gen top pt
          hardGenTop_ = p;
        }
      }

      if(hardGenTop_) {
        if(dbg) cout << "hardest gen top has pt " << hardgentoppt_ << endl;
        if(dbg) cout << "hardest gen top has eta " << hardGenTop_->p4().eta() << endl;
        if(dbg) cout << "hardest gen top has phi " << hardGenTop_->p4().phi() << endl;

        // find hardest reco top which gen-matches our hardest gen top
//        hettTop* hardRecoTop_ = 0; // fixme --see above
        int iHardestReco_ = -1;
        float hardmatchedrecotoppt_ = -99.; // the running hardest reco top pt
        hettnmatchedrecotops_ = 0; // must restart count at zero here (default above is -1 meaning N/A)
        if(dbg) cout << "finding hardest matching reco top" << endl;
        for(int i = 0 ; i < hettTops.size() ; ++i) {
//        for (hettTop recotop : hettTops) {// fixme --see above
          hettTop recotop = hettTops[i];
          if(dbg) cout << "this hett has deltaR wrt hardest gen top of " << PhysicsUtilities::deltaR(recotop.jet123, *hardGenTop_) << endl;
          if (PhysicsUtilities::deltaR(recotop.jet123, *hardGenTop_) < 0.4) { // only want matched reco tops
            ++hettnmatchedrecotops_;
            float matchedrecotoppt_ = recotop.jet123.p4().pt();
            if(dbg) cout << "matched hett has pt " << recotop.jet123.p4().pt() << endl;
            if(dbg) cout << "matched hett has eta " << recotop.jet123.p4().eta() << endl;        
            if(matchedrecotoppt_ < hardmatchedrecotoppt_) {
              if(dbg) cout << "harder matched hett has been found" << endl;
              continue; // we've already found harder matched reco tops
            }
            hardmatchedrecotoppt_     = matchedrecotoppt_; // update hardest matched reco top pt
            hetthardmatchedgentoppt_  = hardGenTop_->p4().pt();  // must fill pt/eta here, or later do = (hettnmatchedrecotops_>0) ? hardGenTop_->p4().pt() : -99.;
            hetthardmatchedgentopeta_ = hardGenTop_->p4().eta(); //
            if(dbg) cout << "setting hardest matched hett" << endl;
//            hardRecoTop_ = &recotop; // fixme --see above
            iHardestReco_ = i;
          }
        }
//        if(hardRecoTop_) {// fixme --see above
        if(iHardestReco_ > -1) {
          hettTop* hardRecoTop_ = &hettTops[iHardestReco_];
          if(dbg) cout << "hardest matched reco jet has pt " << hardRecoTop_->jet123.p4().pt() << endl;
          if(dbg) cout << "hardest matched reco jet has eta " << hardRecoTop_->jet123.p4().eta() << endl;
          if(dbg) cout << "hardest matched reco jet has m3jet " << hardRecoTop_->m3jet << endl;
          if(dbg) cout << "hardest matched reco jet has m12 " << hardRecoTop_->m12 << endl;
          if(dbg) cout << "hardest matched reco jet has m13 " << hardRecoTop_->m13 << endl;
          if(dbg) cout << "hardest matched reco jet has m23 " << hardRecoTop_->m23 << endl;
          if(dbg) cout << "hardest matched reco jet has j1pt " << hardRecoTop_->jet1.p4().pt() << endl;
          if(dbg) cout << "hardest matched reco jet has j1eta " << hardRecoTop_->jet1.p4().eta() << endl;
          if(dbg) cout << "hardest matched reco jet has j2pt " << hardRecoTop_->jet2.p4().pt() << endl;
          if(dbg) cout << "hardest matched reco jet has j2eta " << hardRecoTop_->jet2.p4().eta() << endl;
          if(dbg) cout << "hardest matched reco jet has j3pt " << hardRecoTop_->jet3.p4().pt() << endl;
          if(dbg) cout << "hardest matched reco jet has j3eta " << hardRecoTop_->jet3.p4().eta() << endl;
          hetthardmatchedrecopt_    = hardRecoTop_->jet123.p4().pt();
          hetthardmatchedrecoeta_   = hardRecoTop_->jet123.p4().eta();
          hetthardmatchedrecom3jet_ = hardRecoTop_->m3jet;
          hetthardmatchedrecom12_   = hardRecoTop_->m12;
          hetthardmatchedrecom13_   = hardRecoTop_->m13;
          hetthardmatchedrecom23_   = hardRecoTop_->m23;
          hetthardmatchedrecoj1pt_  = hardRecoTop_->jet1.p4().pt();
          hetthardmatchedrecoj1eta_ = hardRecoTop_->jet1.p4().eta();
          hetthardmatchedrecoj2pt_  = hardRecoTop_->jet2.p4().pt();
          hetthardmatchedrecoj2eta_ = hardRecoTop_->jet2.p4().eta();
          hetthardmatchedrecoj3pt_  = hardRecoTop_->jet3.p4().pt();
          hetthardmatchedrecoj3eta_ = hardRecoTop_->jet3.p4().eta();
          if(hardRecoTop_->m13 < 0) { // either dijet or monojet
            if(hardRecoTop_->m12 < 0) { // monojet
              hetthardmatchednsubjets_ = 1;
            } else { // dijet
              hetthardmatchednsubjets_ = 2;
            }
          } else { // trijet
            hetthardmatchednsubjets_ = 3;
          }

        }//if hardRecoTop
      }//if hardGenTop

      if(dbg) cout << "hettnmatchedrecotops was " << hettnmatchedrecotops_ << endl;
      if(dbg) cout << "hetthardmatchednsubjets was " << hetthardmatchednsubjets_ << endl;

    }//isMC

      // fill branches
      data->fill<int  >(i_hettnrecotops       ,hettnrecotops_     );
      data->fill<int  >(i_hetthardnsubjets    ,hetthardnsubjets_  );
      data->fill<float>(i_hetthardgentoppt    ,hetthardgentoppt_  );
      data->fill<float>(i_hetthardgentopeta   ,hetthardgentopeta_ );
      data->fill<float>(i_hetthardrecopt      ,hetthardrecopt_    );
      data->fill<float>(i_hetthardrecoeta     ,hetthardrecoeta_   );
      data->fill<float>(i_hetthardrecom3jet   ,hetthardrecom3jet_ );
      data->fill<float>(i_hetthardrecom12     ,hetthardrecom12_   );
      data->fill<float>(i_hetthardrecom13     ,hetthardrecom13_   );
      data->fill<float>(i_hetthardrecom23     ,hetthardrecom23_   );
      data->fill<float>(i_hetthardrecoj1pt    ,hetthardrecoj1pt_  );
      data->fill<float>(i_hetthardrecoj1eta   ,hetthardrecoj1eta_ );
      data->fill<float>(i_hetthardrecoj2pt    ,hetthardrecoj2pt_  );
      data->fill<float>(i_hetthardrecoj2eta   ,hetthardrecoj2eta_ );
      data->fill<float>(i_hetthardrecoj3pt    ,hetthardrecoj3pt_  );
      data->fill<float>(i_hetthardrecoj3eta   ,hetthardrecoj3eta_ );
      data->fill<int  >(i_hettnmatchedrecotops       ,hettnmatchedrecotops_     );
      data->fill<int  >(i_hetthardmatchednsubjets    ,hetthardmatchednsubjets_  );
      data->fill<float>(i_hetthardmatchedgentoppt    ,hetthardmatchedgentoppt_  );
      data->fill<float>(i_hetthardmatchedgentopeta   ,hetthardmatchedgentopeta_ );
      data->fill<float>(i_hetthardmatchedrecopt      ,hetthardmatchedrecopt_    );
      data->fill<float>(i_hetthardmatchedrecoeta     ,hetthardmatchedrecoeta_   );
      data->fill<float>(i_hetthardmatchedrecom3jet   ,hetthardmatchedrecom3jet_ );
      data->fill<float>(i_hetthardmatchedrecom12     ,hetthardmatchedrecom12_   );
      data->fill<float>(i_hetthardmatchedrecom13     ,hetthardmatchedrecom13_   );
      data->fill<float>(i_hetthardmatchedrecom23     ,hetthardmatchedrecom23_   );
      data->fill<float>(i_hetthardmatchedrecoj1pt    ,hetthardmatchedrecoj1pt_  );
      data->fill<float>(i_hetthardmatchedrecoj1eta   ,hetthardmatchedrecoj1eta_ );
      data->fill<float>(i_hetthardmatchedrecoj2pt    ,hetthardmatchedrecoj2pt_  );
      data->fill<float>(i_hetthardmatchedrecoj2eta   ,hetthardmatchedrecoj2eta_ );
      data->fill<float>(i_hetthardmatchedrecoj3pt    ,hetthardmatchedrecoj3pt_  );
      data->fill<float>(i_hetthardmatchedrecoj3eta   ,hetthardmatchedrecoj3eta_ );

    return;
  }//fillHettInfo

  // fill HPTT (CTT) branches
  void fillTopTagInfo(TreeWriterData* data, BaseTreeAnalyzer* ana, vector<RecoJetF*> jets) {

    bool sfbclose2lep_ = false;    
    unsigned int sfncttcand_      = 0;   
    float sfcttcandpt_            = -9.; 
    float sfcttcandeta_           = -9.;
    float sfcttcandrawmass_       = -9.;
    float sfcttcandtrimmedmass_   = -9.;
    float sfcttcandprunedmass_    = -9.;
    float sfcttcandsoftdropmass_  = -9.;
    float sfcttcandcmstoptagmass_ = -9.;
    float sfcttcandtau1_          = -9.;
    float sfcttcandtau2_          = -9.;
    float sfcttcandtau3_          = -9.;
    int sfcttcandnsubjets_        = -1;
    float sfcttcandminmass_       = -9.;

    unsigned int sfncttpass_ = 0;
    float sfcttpasspt_       = -9.;
    float sfcttpasseta_      = -9.;

    unsigned int sfnfjcand_ = 0;   
    float sffjcandpt_       = -9.;
    float sffjcandeta_      = -9.;

    unsigned int sfntopsdpass_ = 0;
    float sftopsdpasspt_       = -9.; 
    float sftopsdpasseta_      = -9.;

    unsigned int sfnwsdpass_ = 0;
    float sfwsdpasspt_       = -9.;
    float sfwsdpasseta_      = -9.;

    int nhadronicgentops_ = 0;

    // ctt selection efficiency
    // strategy: for each gen top, loop thru reco cttTops, match one or multi with dR cone, record pT spectra for efficiency
    if(ana->isMC()) {

      //indices of hardest cands for each matching cone size
      // 1,2,3 are placeholders, 6 is default dr=0.6
      unsigned int indxctt = 99;
      float maxcttpt_ = -1.;
      unsigned int indxctt1 = 99;
      float maxcttpt1_ = -1.;
      unsigned int indxctt2 = 99;
      float maxcttpt2_ = -1.;
      unsigned int indxctt3 = 99;
      float maxcttpt3_ = -1.;
      unsigned int indxcttpass1 = 99;
      float maxcttpasspt1_ = -1.;
      unsigned int indxcttpass2 = 99;
      float maxcttpasspt2_ = -1.;
      unsigned int indxcttpass3 = 99;
      float maxcttpasspt3_ = -1.;

      // to be sorted for duplicates later
      vector<float> canddr1pt, canddr1eta, canddr2pt, canddr2eta, canddr3pt, canddr3eta;
      vector<float> passdr1pt, passdr1eta, passdr2pt, passdr2eta, passdr3pt, passdr3eta;
      vector<float> canddr6pt, canddr6eta, passdr6pt, passdr6eta;

      for(auto* p : ana->genParts) {
        if ((abs(p->pdgId())==6) && isGenTopHadronic(p)) {
          nhadronicgentops_++;
          unsigned int countctttags = 0;
          for (auto* ctt : ana->cttTops) {
            float cttpt_ = ctt->p4().pt();

            // for variable dr cones, want matching efficiency (cand = passed ctt selection; pass = then passed dR matching)
            if (cfgSet::isSelTaggedTop(*ctt)) {

              // cone 1 (dR 0.2)
              if (cttpt_>maxcttpt1_) { indxctt1 = countctttags; maxcttpt1_ = cttpt_; } // index of hardest cand
              canddr1pt.push_back(ctt->pt()); canddr1eta.push_back(ctt->p4().eta());
              if(PhysicsUtilities::deltaR(*ctt, *p) < 0.2) {
                if (cttpt_>maxcttpasspt1_) { indxcttpass1 = countctttags; maxcttpasspt1_ = cttpt_; } // index of hardest pass
                passdr1pt.push_back(ctt->pt()); passdr1eta.push_back(ctt->p4().eta());
              }

              // cone 2 (dR 0.5)
              if (cttpt_>maxcttpt2_) { indxctt2 = countctttags; maxcttpt2_ = cttpt_; } // index of hardest cand
              canddr2pt.push_back(ctt->pt()); canddr2eta.push_back(ctt->p4().eta());
              if(PhysicsUtilities::deltaR(*ctt, *p) < 0.5) {
                if (cttpt_>maxcttpasspt2_) { indxcttpass2 = countctttags; maxcttpasspt2_ = cttpt_; } // index of hardest pass
                passdr2pt.push_back(ctt->pt()); passdr2eta.push_back(ctt->p4().eta());
              }

              // cone 3 (dR 0.8)
              if (cttpt_>maxcttpt3_) { indxctt3 = countctttags; maxcttpt3_ = cttpt_; } // index of hardest cand
              canddr3pt.push_back(ctt->pt()); canddr3eta.push_back(ctt->p4().eta());
              if(PhysicsUtilities::deltaR(*ctt, *p) < 0.8) {
                if (cttpt_>maxcttpasspt3_) { indxcttpass3 = countctttags; maxcttpasspt3_ = cttpt_; } // index of hardest pass
                passdr3pt.push_back(ctt->pt()); passdr3eta.push_back(ctt->p4().eta());
              }

            }//if ctt selected

            // default cone (dR 0.6)
            // not matching eff here. rather want eff of ctt selection after matching.
            // multiple jets per event!
            if(PhysicsUtilities::deltaR(*ctt, *p) < 0.6) {
              if (cttpt_>maxcttpt_) { indxctt = countctttags; maxcttpt_ = cttpt_; } // index of hardest 0.6 matched ctt jet
              canddr6pt.push_back(ctt->p4().pt()); canddr6eta.push_back(ctt->p4().eta());
              if(cfgSet::isSelTaggedTop(*ctt)) {passdr6pt.push_back(ctt->p4().pt()); passdr6eta.push_back(ctt->p4().eta());}
            }

            ++countctttags;
          }//for ctt tops
        }//if hadronic top
      }//for gen particles

      // vectors to be de-duplicate
      vector<vector<float> > v_undup;
      v_undup.push_back(canddr1pt); v_undup.push_back(canddr1eta);
      v_undup.push_back(canddr2pt); v_undup.push_back(canddr2eta);
      v_undup.push_back(canddr3pt); v_undup.push_back(canddr3eta);
      v_undup.push_back(passdr1pt); v_undup.push_back(passdr1eta);
      v_undup.push_back(passdr2pt); v_undup.push_back(passdr2eta);
      v_undup.push_back(passdr3pt); v_undup.push_back(passdr3eta);
      v_undup.push_back(canddr6pt); v_undup.push_back(canddr6eta);
      v_undup.push_back(passdr6pt); v_undup.push_back(passdr6eta);
      
      for(int ii = 0; ii < v_undup.size(); ii++) {
        set<float> s_undup(v_undup[ii].begin(),v_undup[ii].end());
        v_undup[ii].assign(s_undup.begin(),s_undup.end());
      }

      // push dr cone variables       
/*
      for(unsigned int i = 0; i < v_undup[0].size(); ++i) {data->fillMulti<float>(i_sfcttcandptdr1,v_undup[0][i]); }
      for(unsigned int i = 0; i < v_undup[1].size(); ++i) {data->fillMulti<float>(i_sfcttcandetadr1,v_undup[1][i]); }
      for(unsigned int i = 0; i < v_undup[2].size(); ++i) {data->fillMulti<float>(i_sfcttcandptdr2,v_undup[2][i]); }
      for(unsigned int i = 0; i < v_undup[3].size(); ++i) {data->fillMulti<float>(i_sfcttcandetadr2,v_undup[3][i]); }
      for(unsigned int i = 0; i < v_undup[4].size(); ++i) {data->fillMulti<float>(i_sfcttcandptdr3,v_undup[4][i]); }
      for(unsigned int i = 0; i < v_undup[5].size(); ++i) {data->fillMulti<float>(i_sfcttcandetadr3,v_undup[5][i]); }

      for(unsigned int i = 0; i < v_undup[6].size(); ++i) {data->fillMulti<float>(i_sfcttpassptdr1,v_undup[6][i]); }
      for(unsigned int i = 0; i < v_undup[7].size(); ++i) {data->fillMulti<float>(i_sfcttpassetadr1,v_undup[7][i]); }
      for(unsigned int i = 0; i < v_undup[8].size(); ++i) {data->fillMulti<float>(i_sfcttpassptdr2,v_undup[8][i]); }
      for(unsigned int i = 0; i < v_undup[9].size(); ++i) {data->fillMulti<float>(i_sfcttpassetadr2,v_undup[9][i]); }
      for(unsigned int i = 0; i < v_undup[10].size(); ++i) {data->fillMulti<float>(i_sfcttpassptdr3,v_undup[10][i]); }
      for(unsigned int i = 0; i < v_undup[11].size(); ++i) {data->fillMulti<float>(i_sfcttpassetadr3,v_undup[11][i]); }
*/

      for(unsigned int i = 0; i < v_undup[12].size(); ++i) {data->fillMulti<float>(i_sfcttcandptdr6,v_undup[12][i]); }
      for(unsigned int i = 0; i < v_undup[13].size(); ++i) {data->fillMulti<float>(i_sfcttcandetadr6,v_undup[13][i]); }
      for(unsigned int i = 0; i < v_undup[14].size(); ++i) {data->fillMulti<float>(i_sfcttpassptdr6,v_undup[14][i]); }
      for(unsigned int i = 0; i < v_undup[15].size(); ++i) {data->fillMulti<float>(i_sfcttpassetadr6,v_undup[15][i]); }

      // hardest gen matched to 0.6
      if (indxctt<99) {
        data->fillMulti<float>(i_sfcttcandptdr6hard        , ana->cttTops[indxctt]->p4().pt());
        data->fillMulti<float>(i_sfcttcandetadr6hard       , ana->cttTops[indxctt]->p4().eta());
        if(cfgSet::isSelTaggedTop(*ana->cttTops[indxctt])) {
          data->fillMulti<float>(i_sfcttpassptdr6hard      , ana->cttTops[indxctt]->p4().pt());
          data->fillMulti<float>(i_sfcttpassetadr6hard      , ana->cttTops[indxctt]->p4().eta());
        }
      }

      // hardest gen matched other cones
      // cone 1
      if (indxctt1<99) {
        auto *ctt = ana->cttTops[indxctt1];
        data->fillMulti<float>(i_sfcttcandptdr1        , ctt->p4().pt());
        data->fillMulti<float>(i_sfcttcandetadr1       , ctt->p4().eta());
      }
      if (indxcttpass1<99) {
        auto *ctt = ana->cttTops[indxcttpass1];
        data->fillMulti<float>(i_sfcttpassptdr1      , ctt->p4().pt());
        data->fillMulti<float>(i_sfcttpassetadr1      , ctt->p4().eta());
      }
      // cone 2
      if (indxctt2<99) {
        auto *ctt = ana->cttTops[indxctt2];
        data->fillMulti<float>(i_sfcttcandptdr2        , ctt->p4().pt());
        data->fillMulti<float>(i_sfcttcandetadr2       , ctt->p4().eta());
      }
      if (indxcttpass2<99) {
        auto *ctt = ana->cttTops[indxcttpass2];
        data->fillMulti<float>(i_sfcttpassptdr2      , ctt->p4().pt());
        data->fillMulti<float>(i_sfcttpassetadr2      , ctt->p4().eta());
      }
      // cone 3
      if (indxctt3<99) {
        auto *ctt = ana->cttTops[indxctt3];
        data->fillMulti<float>(i_sfcttcandptdr3        , ctt->p4().pt());
        data->fillMulti<float>(i_sfcttcandetadr3       , ctt->p4().eta());
      }
      if (indxcttpass3<99) {
        auto *ctt = ana->cttTops[indxcttpass3];
        data->fillMulti<float>(i_sfcttpassptdr3      , ctt->p4().pt());
        data->fillMulti<float>(i_sfcttpassetadr3      , ctt->p4().eta());
      }

    }//if MC

    vector<LorentzVector> csvmjets;
    for(auto* j : jets) {

      if(j->csv() > defaults::CSV_MEDIUM) {
	LorentzVector tmpVecCSVMJets; tmpVecCSVMJets = j->p4();
	csvmjets.push_back(tmpVecCSVMJets); }
    }


    if (ana->nSelLeptons == 0) {

      // do it for ctt fatjets
      unsigned int countctttags = 0;
      unsigned int indxctt = 99;
      float maxcttpt_ = -1.;
      for (auto* ctt : ana->cttTops) {

	float cttpt_ = ctt->p4().pt();
	if (cttpt_>maxcttpt_) { indxctt = countctttags; maxcttpt_ = cttpt_;}
	
	++countctttags; 
      }

      if (indxctt<99) {
	sfncttcand_             = 1;
	sfcttcandpt_            = ana->cttTops[indxctt]->p4().pt(); 
	sfcttcandeta_           = ana->cttTops[indxctt]->p4().eta();
	sfcttcandrawmass_       = ana->cttTops[indxctt]->topRawMass();
	sfcttcandtrimmedmass_   = ana->cttTops[indxctt]->topTrimmedMass();
	sfcttcandprunedmass_    = ana->cttTops[indxctt]->topPrunedMass();
	sfcttcandsoftdropmass_  = ana->cttTops[indxctt]->topSoftDropMass();
	sfcttcandcmstoptagmass_ = ana->cttTops[indxctt]->topCmsTopTagMass(); 
	sfcttcandtau1_          = ana->cttTops[indxctt]->topTau1();
	sfcttcandtau2_          = ana->cttTops[indxctt]->topTau2();
	sfcttcandtau3_          = ana->cttTops[indxctt]->topTau3();
	sfcttcandnsubjets_      = ana->cttTops[indxctt]->topNsubJets();
	sfcttcandminmass_       = ana->cttTops[indxctt]->topMinMass();

	if (cfgSet::isSelTaggedTop(*ana->cttTops[indxctt])) {
	  sfncttpass_   = 1;
	  sfcttpasspt_  = ana->cttTops[indxctt]->p4().pt();
	  sfcttpasseta_ = ana->cttTops[indxctt]->p4().eta(); }
      }     
      

      // do it for ak8 fatjets
      unsigned int countfjtags = 0;
      unsigned int indxfj = 99;
      float maxfjpt_ = -1.;
      for (auto* fj : ana->fatJets) {

	float fjpt_ = fj->p4().pt();
	if (fjpt_>maxfjpt_) { indxfj = countfjtags; maxfjpt_ = fjpt_;}
	
	++countfjtags; 
      }

      if (indxfj<99) {
	sfnfjcand_   = 1;
	sffjcandpt_  = ana->fatJets[indxfj]->p4().pt();
	sffjcandeta_ = ana->fatJets[indxfj]->p4().eta();
	
	if (passSoftDropTaggerFJ(ana->fatJets[indxfj],120.,220.)) { 
	  sfntopsdpass_   = 1;
	  sftopsdpasspt_  = ana->fatJets[indxfj]->p4().pt(); 
	  sftopsdpasseta_ = ana->fatJets[indxfj]->p4().eta(); }

	if (passSoftDropTaggerFJ(ana->fatJets[indxfj],60.,100000.)) {
	  sfnwsdpass_   = 1;
	  sfwsdpasspt_  = ana->fatJets[indxfj]->p4().pt(); 
	  sfwsdpasseta_ = ana->fatJets[indxfj]->p4().eta(); }

      }


    } // end of zero leptons


    if (ana->nSelLeptons > 0) {
      MomentumF* lep = new MomentumF(ana->selectedLeptons.at(0)->p4());

      for (unsigned int i0=0; i0<csvmjets.size(); ++i0) {
	float drlepbjet_ = PhysicsUtilities::deltaR(*lep,csvmjets[i0]);
	if (drlepbjet_<(3.14/2.)) { sfbclose2lep_ = true; } 
      }

      // do it for ctt tags
      unsigned int countctttags = 0;
      unsigned int indxctt = 99;
      float maxcttpt_ = -1.;
      for (auto* ctt : ana->cttTops) {

	float drlepctt_ = PhysicsUtilities::deltaR(*lep,ctt->p4());

	if (drlepctt_>=(3.14/2.)) { 
	  float cttpt_ = ctt->p4().pt();
	  if (cttpt_>maxcttpt_) { indxctt = countctttags; maxcttpt_ = cttpt_;}
	}
	
	++countctttags; 
      }
      
      if (indxctt<99) { 
	sfncttcand_             = 1;
	sfcttcandpt_            = ana->cttTops[indxctt]->p4().pt(); 
	sfcttcandeta_           = ana->cttTops[indxctt]->p4().eta();
	sfcttcandrawmass_       = ana->cttTops[indxctt]->topRawMass();
	sfcttcandtrimmedmass_   = ana->cttTops[indxctt]->topTrimmedMass();
	sfcttcandprunedmass_    = ana->cttTops[indxctt]->topPrunedMass();
	sfcttcandsoftdropmass_  = ana->cttTops[indxctt]->topSoftDropMass();
	sfcttcandcmstoptagmass_ = ana->cttTops[indxctt]->topCmsTopTagMass();
	sfcttcandtau1_          = ana->cttTops[indxctt]->topTau1();
	sfcttcandtau2_          = ana->cttTops[indxctt]->topTau2();
	sfcttcandtau3_          = ana->cttTops[indxctt]->topTau3();
	sfcttcandnsubjets_      = ana->cttTops[indxctt]->topNsubJets();
	sfcttcandminmass_       = ana->cttTops[indxctt]->topMinMass();

	if (cfgSet::isSelTaggedTop(*ana->cttTops[indxctt])) {
	  sfncttpass_     = 1;
	  sfcttpasspt_  = ana->cttTops[indxctt]->p4().pt(); 
	  sfcttpasseta_ = ana->cttTops[indxctt]->p4().eta(); }
	
      }     
      
      // do it for ak8 fatjets
      unsigned int countfjtags = 0;
      unsigned int indxfj = 99;
      float maxfjpt_ = -1.;
      for (auto* fj : ana->fatJets) {

	float drlepfj_ = PhysicsUtilities::deltaR(*lep,fj->p4());

	if (drlepfj_>=(3.14/2.)) { 
	  float fjpt_ = fj->p4().pt();
	  if (fjpt_>maxfjpt_) { indxfj = countfjtags; maxfjpt_ = fjpt_;}
	}
	
	++countfjtags; 
      }

      if (indxfj<99) {
	sfnfjcand_   = 1;
	sffjcandpt_  = ana->fatJets[indxfj]->p4().pt(); 
	sffjcandeta_ = ana->fatJets[indxfj]->p4().eta();
	
	if (passSoftDropTaggerFJ(ana->fatJets[indxfj],120.,220.)) { 
	  sfntopsdpass_   = 1;
	  sftopsdpasspt_  = ana->fatJets[indxfj]->p4().pt(); 
	  sftopsdpasseta_ = ana->fatJets[indxfj]->p4().eta(); }

	if (passSoftDropTaggerFJ(ana->fatJets[indxfj],60.,100000.)) {
	  sfnwsdpass_   = 1;
	  sfwsdpasspt_  = ana->fatJets[indxfj]->p4().pt(); 
	  sfwsdpasseta_ = ana->fatJets[indxfj]->p4().eta(); }

      }



    } // end of at least one lepton
    

    data->fill<bool>(i_sfbclose2lep           , sfbclose2lep_);
    data->fill<unsigned int>(i_sfncttcand     , sfncttcand_);
    data->fill<float>(i_sfcttcandpt           , sfcttcandpt_);
    data->fill<float>(i_sfcttcandeta          , sfcttcandeta_);
    data->fill<float>(i_sfcttcandrawmass      , sfcttcandrawmass_);
    data->fill<float>(i_sfcttcandtrimmedmass  , sfcttcandtrimmedmass_);
    data->fill<float>(i_sfcttcandprunedmass   , sfcttcandprunedmass_);
    data->fill<float>(i_sfcttcandsoftdropmass , sfcttcandsoftdropmass_);
    data->fill<float>(i_sfcttcandcmstoptagmass, sfcttcandcmstoptagmass_);
    data->fill<float>(i_sfcttcandtau1         , sfcttcandtau1_);
    data->fill<float>(i_sfcttcandtau2         , sfcttcandtau2_);
    data->fill<float>(i_sfcttcandtau3         , sfcttcandtau3_);
    data->fill<int>(i_sfcttcandnsubjets       , sfcttcandnsubjets_);
    data->fill<float>(i_sfcttcandminmass      , sfcttcandminmass_);
    data->fill<unsigned int>(i_sfncttpass     , sfncttpass_);
    data->fill<float>(i_sfcttpasspt           , sfcttpasspt_);
    data->fill<float>(i_sfcttpasseta          , sfcttpasseta_);
    data->fill<unsigned int>(i_sfnfjcand     , sfnfjcand_);
    data->fill<float>(i_sffjcandpt           , sffjcandpt_);
    data->fill<float>(i_sffjcandeta          , sffjcandeta_);
    data->fill<unsigned int>(i_sfntopsdpass  , sfntopsdpass_);
    data->fill<float>(i_sftopsdpasspt        , sftopsdpasspt_);
    data->fill<float>(i_sftopsdpasseta       , sftopsdpasseta_);
    data->fill<unsigned int>(i_sfnwsdpass    , sfnwsdpass_);
    data->fill<float>(i_sfwsdpasspt          , sfwsdpasspt_);
    data->fill<float>(i_sfwsdpasseta         , sfwsdpasseta_);
    data->fill<int  >(i_nhadronicgentops      , nhadronicgentops_);

    for(auto* fj : ana->fatJets) {

      data->fillMulti<float>(i_wcandmass, fj->fjSoftDropMass());
      data->fillMulti<float>(i_wcandpt  , fj->p4().pt());
      data->fillMulti<float>(i_wcandeta , fj->p4().eta());

      if (passSoftDropTaggerFJ(fj,60.,100000.)) {

        data->fillMulti<float>(i_wpasspt  , fj->p4().pt());
        data->fillMulti<float>(i_wpasseta , fj->p4().eta());
      }

    }

    
  } // end of fillTopTagInfo


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
      if(jets.at(1)->pt() < minj2pt_)  return false;

      filler.fillEventInfo(&data, this);
      filler.fillJetInfo  (&data, jets, bJets, met);
//      filler.fillTopTagInfo(&data,this, jets);
      return true;
    }

};

#endif
