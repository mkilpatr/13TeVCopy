#ifndef BASICVARIABLES_HH
#define BASICVARIABLES_HH

#include "AnalysisTools/DataFormats/interface/CMSTop.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
#include "AnalysisTools/KinematicVariables/interface/JetKinematics.h"
#include "AnalysisBase/TreeAnalyzer/interface/DefaultProcessing.h"
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"

using namespace ucsbsusy;

struct BasicVarsFiller {

  BasicVarsFiller() {}

  // Process and weights
  size i_run       ;
  size i_lumi      ;
  size i_event     ;
  size i_process   ;
  size i_ismc      ;
  size i_weight    ;
  size i_truePUWeight;
  size i_btagWeight;
  size i_btagFastSimWeight;
  size i_qcdRespTailWeight;
  size i_topptWeight;
  size i_lepvetoweightLM;
  size i_lepselweightLM;
  size i_lepvetoweightHM;
  size i_lepselweightHM;
  size i_leptnpweightLM;
  size i_leptnpweightHM;
  size i_isrWeight;
  size i_isrWeightTight;
  size i_sdtopFastSimWeight;
  size i_sdwFastSimWeight;

  // Trigger and filters
  size i_passjson  ;
  size i_passmetmht100;
  size i_passtrige ;
  size i_passtrigmu;
  size i_passtriglepOR;
  size i_passtrigphoton165;
  size i_passtrigphoOR;
  size i_passtright800;
  size i_passtrigdilep;
  size i_passtrigdilepOR;
  size i_j1chEnFrac  ;
  size i_passmetfilters;
//  size i_pass_HBHENoiseFilter                    ;
//  size i_pass_HBHENoiseIsoFilter                 ;
//  size i_pass_globalTightHalo2016Filter          ;
//  size i_pass_EcalDeadCellTriggerPrimitiveFilter ;
//  size i_pass_goodVertices                       ;
//  size i_pass_eeBadScFilter                      ;
//  size i_pass_badChCand                          ;
//  size i_pass_badPFMuon                          ;

  // Basic event variables
  size i_genmet    ;
  size i_origmet   ;
  size i_met       ;
  size i_npv       ;
  size i_nvetolep  ;
  size i_nvetotau  ;
  size i_nvetohpstaus;
  size i_ncttstd   ;
  size i_nsdtoploose;
  size i_nsdwloose;

  // Jet & MET variables
  size i_njets     ;
  size i_nbjets    ;
  size i_nlbjets   ;
  size i_ntbjets   ;
  size i_ht        ;
  size i_j1pt      ;
  size i_j2pt      ;
  size i_dphij1met ;
  size i_dphij2met ;
  size i_dphij3met ;
  size i_dphij4met ;
  size i_mtcsv1met ;
  size i_mtcsv2met ;
  size i_mtcsv12met;
  size i_metovsqrtht;
  size i_njl;
  size i_j1lpt;
  size i_csvj1pt   ;
  size i_csvj2pt   ;
  size i_dphij1lmet;
  size i_nivf;

  // Lepton variables
  size i_leptonpt  ;
  size i_leptoneta ;
  size i_leptonpdgid;
  size i_leptonq   ;
  size i_mtlepmet  ;
  size i_lp        ;
  size i_dileppt   ;
  size i_dilepmass ;
  size i_recolepinwortop;

  // Gen-level variables
  size i_ngoodgenmu;
  size i_ngoodgenele;
  size i_npromptgentau;
  size i_genwpt;

  void book(TreeWriterData *data) {
    // Process and weights
    i_run            = data->add<unsigned int>("","run","i",0);
    i_lumi           = data->add<unsigned int>("","lumi","i",0);
    i_event          = data->add<unsigned int>("","event","i",0);
    i_process        = data->add<unsigned int>("","process","i",0);
    i_ismc           = data->add<bool >("","ismc","O",0);
    i_weight         = data->add<float>("","weight","F",0);
    i_truePUWeight   = data->add<float>("","truePUWeight","F",0);
    i_btagWeight     = data->add<float>("","btagWeight","F",0);
    i_btagFastSimWeight = data->add<float>("","btagFastSimWeight","F",0);
    i_qcdRespTailWeight = data->add<float>("","qcdRespTailWeight","F",0);
    i_topptWeight    = data->add<float>("","topptWeight","F",1);
    i_lepvetoweightLM  = data->add<float>("","lepvetoweightLM","F",0);
    i_lepselweightLM   = data->add<float>("","lepselweightLM","F",0);
    i_lepvetoweightHM  = data->add<float>("","lepvetoweightHM","F",0);
    i_lepselweightHM   = data->add<float>("","lepselweightHM","F",0);
    i_leptnpweightLM = data->add<float>("","leptnpweightLM","F",0);
    i_leptnpweightHM = data->add<float>("","leptnpweightHM","F",0);
    i_isrWeight      = data->add<float>("","isrWeight","F",0);
    i_isrWeightTight = data->add<float>("","isrWeightTight","F",0);
    i_sdtopFastSimWeight = data->add<float>("","sdtopFastSimWeight","F",0);
    i_sdwFastSimWeight   = data->add<float>("","sdwFastSimWeight","F",0);

    // Trigger and filters
    i_passjson       = data->add<bool>("","passjson","O",0);
    i_passmetmht100  = data->add<bool>("","passmetmht100","O",0);
    i_passtrige      = data->add<bool>("","passtrige","O",0);
    i_passtrigmu     = data->add<bool>("","passtrigmu","O",0);
    i_passtriglepOR  = data->add<bool>("","passtriglepOR","O",0);
    i_passtrigphoton165 = data->add<bool>("","passtrigphoton165", "O",0);
    i_passtrigphoOR  = data->add<bool>("","passtrigphoOR", "O",0);
    i_passtright800  = data->add<bool>("","passtright800","O",0);
    i_passtrigdilep  = data->add<bool>("", "passtrigdilep", "O", 0);
    i_passtrigdilepOR   = data->add<bool>("", "passtrigdilepOR", "O", 0);
    i_j1chEnFrac     = data->add<float>("","j1chEnFrac","F",2);
    i_passmetfilters = data->add<bool>("","passmetfilters","O",0);
//    i_pass_HBHENoiseFilter                      = data->add<bool>("","pass_HBHENoiseFilter"                   ,"O",0);
//    i_pass_HBHENoiseIsoFilter                   = data->add<bool>("","pass_HBHENoiseIsoFilter"                ,"O",0);
//    i_pass_globalTightHalo2016Filter            = data->add<bool>("","pass_globalTightHalo2016Filter"         ,"O",0);
//    i_pass_EcalDeadCellTriggerPrimitiveFilter   = data->add<bool>("","pass_EcalDeadCellTriggerPrimitiveFilter","O",0);
//    i_pass_goodVertices                         = data->add<bool>("","pass_goodVertices"                      ,"O",0);
//    i_pass_eeBadScFilter                        = data->add<bool>("","pass_eeBadScFilter"                     ,"O",0);
//    i_pass_badChCand                            = data->add<bool>("","pass_badChCand"                         ,"O",0);
//    i_pass_badPFMuon                            = data->add<bool>("","pass_badPFMuon"                         ,"O",0);

    // Basic event variables
    i_genmet         = data->add<float>("","genmet","F",0);
    i_origmet        = data->add<float>("","origmet","F",0);
    i_met            = data->add<float>("","met","F",0);
    i_npv            = data->add<int>("","npv","I",0);
    i_nvetolep       = data->add<int>("","nvetolep","I",0);
    i_nvetotau       = data->add<int>("","nvetotau","I",0);
    i_nvetohpstaus   = data->add<int>("","nvetohpstaus","I",0);
    i_ncttstd        = data->add<int>("","ncttstd","I",0);
    i_nsdtoploose    = data->add<int>("","nsdtoploose","I",0);
    i_nsdwloose      = data->add<int>("","nsdwloose","I",0);

    // Jet & MET variables
    i_njets          = data->add<int>("","njets","I",0);
    i_nbjets         = data->add<int>("","nbjets","I",0);
    i_nlbjets        = data->add<int>("","nlbjets","I",0);
    i_ntbjets        = data->add<int>("","ntbjets","I",0);
    i_ht             = data->add<float>("","ht","F",0);
    i_j1pt           = data->add<float>("","j1pt","F",0);
    i_j2pt           = data->add<float>("","j2pt","F",0);
    i_dphij1met      = data->add<float>("","dphij1met","F",0);
    i_dphij2met      = data->add<float>("","dphij2met","F",0);
    i_dphij3met      = data->add<float>("","dphij3met","F",3);
    i_dphij4met      = data->add<float>("","dphij4met","F",3);
    i_mtcsv1met      = data->add<float>("","mtcsv1met","F",0);
    i_mtcsv2met      = data->add<float>("","mtcsv2met","F",0);
    i_mtcsv12met     = data->add<float>("","mtcsv12met","F",0);
    i_metovsqrtht    = data->add<float>("","metovsqrtht","F",0);
    i_njl            = data->add<int>("","njl","I",0);
    i_j1lpt          = data->add<float>("","j1lpt","F",0);
    i_dphij1lmet     = data->add<float>("","dphij1lmet","F",0);
    i_csvj1pt        = data->add<float>("","csvj1pt","F",0);
    i_csvj2pt        = data->add<float>("","csvj2pt","F",0);
    i_nivf           = data->add<int>("","nivf","I",0);

    // Lepton variables
    i_leptonpt       = data->add<float>("","leptonpt","F",0);
    i_leptoneta      = data->add<float>("","leptoneta","F",0);
    i_leptonpdgid    = data->add<int>("","leptonpdgid","I",0);
    i_leptonq        = data->add<int>("","leptonq","I",0);
    i_mtlepmet       = data->add<float>("","mtlepmet","F",0);
    i_lp             = data->add<float>("","lp","F",-9);
    i_dileppt        = data->add<float>("","dileppt","F",0);
    i_dilepmass      = data->add<float>("","dilepmass","F",0);
    i_recolepinwortop = data->add<bool>("","recolepinwortop","O",0);

    // Gen-level variables
    i_ngoodgenmu     = data->add<int>("","ngoodgenmu","I",0);
    i_ngoodgenele    = data->add<int>("","ngoodgenele","I",0);
    i_npromptgentau  = data->add<int>("","npromptgentau","I",0);
    i_genwpt         = data->add<float>("","genwpt","F",-1);

  }

  void fillEventInfo(TreeWriterData *data, const BaseTreeAnalyzer* ana, bool useModifiedMET = false, MomentumF* metn = 0) {

    const auto &jets = ana->jets;
    const MomentumF *met = useModifiedMET ? metn : ana->met;
    /* current set-up:
     * if useModifiedMET = true: all variables will be calcuated with metn, except for mtlepmet and WP4
     * else: use original met for everything, but the "met" variable will be *metn* if given
     */


    // Process and weights
    data->fill<unsigned int>(i_run, ana->run);
    data->fill<unsigned int>(i_lumi, ana->lumi);
    data->fill<unsigned int>(i_event, ana->event);
    data->fill<unsigned int>(i_process, ana->process);
    data->fill<bool >(i_ismc,   ana->isMC());
    data->fill<float>(i_weight, ana->weight);
    data->fill<float>(i_truePUWeight,       ana->eventCorrections.getTruePUWeight());
    data->fill<float>(i_btagWeight,         ana->bTagCorrections.getBTagByEvtWeight());
    data->fill<float>(i_btagFastSimWeight,  ana->bTagCorrections.getBTagFastSimByEvtWeight());
    data->fill<float>(i_qcdRespTailWeight,  ana->jetAndMETCorrections.getQCDRespTailWeight());
    data->fill<float>(i_topptWeight,        ana->ttbarCorrections.getTopPTWeight());
    data->fill<float>(i_lepvetoweightLM,    ana->leptonCorrections.getVetoLepWeightLM());
    data->fill<float>(i_lepselweightLM,     ana->leptonCorrections.getSelLepWeightLM());
    data->fill<float>(i_lepvetoweightHM,    ana->leptonCorrections.getVetoLepWeightHM());
    data->fill<float>(i_lepselweightHM,     ana->leptonCorrections.getSelLepWeightHM());
    data->fill<float>(i_leptnpweightLM,     ana->leptonCorrections.getTnPLepWeightLM());
    data->fill<float>(i_leptnpweightHM,     ana->leptonCorrections.getTnPLepWeightHM());
    data->fill<float>(i_isrWeight,          ana->isrCorrections.getISRWeight());
    data->fill<float>(i_isrWeightTight,          ana->isrCorrections.getISRWeightTight());
    data->fill<float>(i_sdtopFastSimWeight, ana->eventCorrections.getSdTopWeight());
    data->fill<float>(i_sdwFastSimWeight, ana->eventCorrections.getSdWWeight());

    // Trigger and filters
    data->fill<bool>(i_passjson,       ana->isMC() || (ana->hasJSONFile() && ana->passesLumiMask()));

    // HT trigger:
    bool passTrigPFHT800 = ana->triggerflag & kHLT_PFHT800;
    data->fill<bool >(i_passtright800, ana->isMC() ? true : (ana->process==defaults::DATA_JETHT ? passTrigPFHT800 : false));

    // METMHT trigger:
    bool passTrigMETMHT100 = (ana->triggerflag & kHLT_PFMET100_PFMHT100_IDTight) || (ana->triggerflag & kHLT_PFMETNoMu100_PFMHTNoMu100_IDTight);
    data->fill<bool>(i_passmetmht100,  ana->isMC() || (ana->process==defaults::DATA_MET ? passTrigMETMHT100 : false));

    // photon trigger: HLT_Photon165_HE10 || HLT_CaloJet500_NoJetID(JetHT) || HLT_ECALHT800(DoubleEG)
    bool passTrigPho165 = ana->triggerflag & kHLT_Photon165_HE10;
    bool passTrigCaloJet500 = ana->triggerflag & kHLT_CaloJet500_NoJetID;
    bool passTrigECALHT800 = ana->triggerflag & kHLT_ECALHT800;
    bool passtrigphoOR = (ana->process==defaults::DATA_SINGLEPHO && passTrigPho165)
        || (ana->process==defaults::DATA_JETHT && !passTrigPho165 && passTrigCaloJet500)
        || (ana->process==defaults::DATA_DOUBLEEG && !passTrigPho165 && !passTrigCaloJet500 && passTrigECALHT800);
    data->fill<bool>(i_passtrigphoton165, ana->isMC() || (ana->process==defaults::DATA_SINGLEPHO ? passTrigPho165 : false));
    data->fill<bool>(i_passtrigphoOR,  ana->isMC() || passtrigphoOR);

    // single lepton trigger
    bool passTrigEl = ana->triggerflag & kHLT_Ele27_eta2p1_WPLoose_Gsf;
    bool passTrigMu = (ana->triggerflag & kHLT_IsoMu22) || (ana->triggerflag & kHLT_IsoTkMu22);
    data->fill<bool>(i_passtrigmu,     ana->isMC() || (ana->process==defaults::DATA_SINGLEMU ? passTrigMu: false));
    data->fill<bool>(i_passtrige,      ana->isMC() || (ana->process==defaults::DATA_SINGLEEL ? (passTrigEl && (!passTrigMu)) : false));

    // single lepton trigger OR:
    // SingleMuon: kHLT_IsoMu22 || kHLT_IsoTkMu22 || kHLT_Mu15_IsoVVVL_PFHT(350|400|600)(_PFMET50)
    // SingleElectron: kHLT_Ele27_eta2p1_WPLoose_Gsf || kHLT_Ele15_IsoVVVL_PFHT(350|400|600)(_PFMET50)
    // MET: kHLT_PFMET100_PFMHT100_IDTight || kHLT_PFMETNoMu100_PFMHTNoMu100_IDTight
    // JetHT: HLT_CaloJet500_NoJetID
    // DoubleEG: kHLT_ECALHT800
    bool passTrigMuHT = (ana->triggerflag & kHLT_IsoMu22) || (ana->triggerflag & kHLT_IsoTkMu22)
        || (ana->triggerflag & kHLT_Mu15_IsoVVVL_PFHT350_PFMET50) || (ana->triggerflag & kHLT_Mu15_IsoVVVL_PFHT400_PFMET50)
        || (ana->triggerflag & kHLT_Mu15_IsoVVVL_PFHT350) || (ana->triggerflag & kHLT_Mu15_IsoVVVL_PFHT400) || (ana->triggerflag & kHLT_Mu15_IsoVVVL_PFHT600);
    bool passTrigElHT = (ana->triggerflag & kHLT_Ele27_eta2p1_WPLoose_Gsf)
        || (ana->triggerflag & kHLT_Ele15_IsoVVVL_PFHT350_PFMET50) || (ana->triggerflag & kHLT_Ele15_IsoVVVL_PFHT400_PFMET50)
        || (ana->triggerflag & kHLT_Ele15_IsoVVVL_PFHT350) || (ana->triggerflag & kHLT_Ele15_IsoVVVL_PFHT400) || (ana->triggerflag & kHLT_Ele15_IsoVVVL_PFHT600);
    bool passtriglepOR = (ana->process==defaults::DATA_SINGLEMU && passTrigMuHT)
        || (ana->process==defaults::DATA_SINGLEEL && !passTrigMuHT && passTrigElHT)
        || (ana->process==defaults::DATA_MET && !passTrigMuHT && !passTrigElHT && passTrigMETMHT100)
        || (ana->process==defaults::DATA_JETHT && !passTrigMuHT && !passTrigElHT && !passTrigMETMHT100 && passTrigCaloJet500)
        || (ana->process==defaults::DATA_DOUBLEEG && !passTrigMuHT && !passTrigElHT && !passTrigMETMHT100 && !passTrigCaloJet500 && passTrigECALHT800);
    data->fill<bool>(i_passtriglepOR,  ana->isMC() || passtriglepOR);

    // dilepton trigger
    bool passTrigElEl = ana->triggerflag & kHLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ;
    bool passTrigMuMu = (ana->triggerflag & kHLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ) || (ana->triggerflag & kHLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ);
    data->fill<bool>(i_passtrigdilep,  ana->isMC() || ((ana->process==defaults::DATA_DOUBLEMU && passTrigMuMu) || (ana->process==defaults::DATA_DOUBLEEG && passTrigElEl && (!passTrigMuMu))));

    // dilepton trigger OR:
    // DoubleMu: kHLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ || kHLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ || kHLT_Mu30_TkMu11
    // DoubleEG: kHLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ || kHLT_DoubleEle33_CaloIdL_GsfTrkIdVL_MW
    // SingleMuon: kHLT_Mu50 (incl. HLT_TkMu50)
    // SingleElectron: kHLT_Ele105_CaloIdVT_GsfTrkIdT
    bool passTrigMuMuOR = (ana->triggerflag & kHLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ) || (ana->triggerflag & kHLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ) || (ana->triggerflag & kHLT_Mu30_TkMu11);
    bool passTrigElElOR = (ana->triggerflag & kHLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ) || (ana->triggerflag & kHLT_DoubleEle33_CaloIdL_GsfTrkIdVL_MW);
    bool passTrigMu50 = (ana->triggerflag & kHLT_Mu50);
    bool passTrigEle105 = (ana->triggerflag & kHLT_Ele105_CaloIdVT_GsfTrkIdT);
    bool passtrigdilepOR = (ana->process==defaults::DATA_DOUBLEMU && passTrigMuMuOR)
        || (ana->process==defaults::DATA_DOUBLEEG && !passTrigMuMuOR && passTrigElElOR)
        || (ana->process==defaults::DATA_SINGLEMU && !passTrigMuMuOR && !passTrigElElOR && passTrigMu50)
        || (ana->process==defaults::DATA_SINGLEEL && !passTrigMuMuOR && !passTrigElElOR && !passTrigMu50 && passTrigEle105);
    data->fill<bool>(i_passtrigdilepOR,  ana->isMC() || passtrigdilepOR);

    data->fill<float>(i_j1chEnFrac, jets.front()->chHadFrac());  // take the leading jet after cleaning/jetid/etc.
//    data->fill<float>(i_j1chEnFrac, ana->defaultJets->recoJets.front().chHadFrac());

    const auto &evt = ana->evtInfoReader;
    bool passmetfilters = evt.HBHENoiseFilter && evt.HBHENoiseIsoFilter && evt.globalTightHalo2016Filter && evt.EcalDeadCellTriggerPrimitiveFilter && evt.goodVertices && evt.eeBadScFilter && evt.badChCand && evt.badPFMuon;
    data->fill<bool>(i_passmetfilters,  passmetfilters);
//    data->fill<bool>(i_pass_HBHENoiseFilter                   ,ana->evtInfoReader.HBHENoiseFilter                   );
//    data->fill<bool>(i_pass_HBHENoiseIsoFilter                ,ana->evtInfoReader.HBHENoiseIsoFilter                );
//    data->fill<bool>(i_pass_globalTightHalo2016Filter         ,ana->evtInfoReader.globalTightHalo2016Filter         );
//    data->fill<bool>(i_pass_EcalDeadCellTriggerPrimitiveFilter,ana->evtInfoReader.EcalDeadCellTriggerPrimitiveFilter);
//    data->fill<bool>(i_pass_goodVertices                      ,ana->evtInfoReader.goodVertices                      );
//    data->fill<bool>(i_pass_eeBadScFilter                     ,ana->evtInfoReader.eeBadScFilter                     );
//    data->fill<bool>(i_pass_badChCand                         ,ana->evtInfoReader.badChCand                         );
//    data->fill<bool>(i_pass_badPFMuon                         ,ana->evtInfoReader.badPFMuon                         );

    // Basic event variables
    data->fill<float>(i_genmet, ana->genmet->pt());
    data->fill<float>(i_origmet, ana->met->pt());
    data->fill<float>(i_met, metn ? metn->pt() : ana->met->pt()); // fill modified met if given
    data->fill<int  >(i_npv, ana->nPV);
    data->fill<int  >(i_nvetolep, ana->nSelLeptons);
    data->fill<int  >(i_nvetotau, ana->nVetoedTracks);
    data->fill<int  >(i_nvetohpstaus,ana->nVetoHPSTaus);
    data->fill<int  >(i_ncttstd,  ana->nSelCTTTops);

    data->fill<int  >(i_nsdtoploose,  ana->selectedSdTops.size());
    data->fill<int  >(i_nsdwloose,    ana->selectedSdWs.size());

    // Jet & MET variables
    int ntbjets = 0, nlbjets = 0;
    for(const auto* j : jets) {
      if(j->csv() > defaults::CSV_LOOSE)  nlbjets++;
      if(j->csv() > defaults::CSV_TIGHT)  ntbjets++;
    }
    data->fill<int>(i_njets,    ana->nJets);
    data->fill<int>(i_nbjets,   ana->nBJets);
    data->fill<int>(i_nlbjets,  nlbjets);
    data->fill<int>(i_ntbjets,  ntbjets);
    auto ht = JetKinematics::ht(jets, 20.0, 2.4);
    data->fill<float>(i_ht,     ht);
    data->fill<float>(i_metovsqrtht, (met->pt())/(sqrt(ht)));

    if(jets.size() > 0) {
      data->fill<float>(i_j1pt, jets[0]->pt());
      data->fill<float>(i_dphij1met, fabs(PhysicsUtilities::deltaPhi(*jets[0], *met)));
    }
    if(jets.size() > 1) {
      data->fill<float>(i_j2pt, jets[1]->pt());
      data->fill<float>(i_dphij2met, fabs(PhysicsUtilities::deltaPhi(*jets[1], *met)));
    }
    if(jets.size() > 2) {
      data->fill<float>(i_dphij3met, fabs(PhysicsUtilities::deltaPhi(*jets[2], *met)));
    }
    if(jets.size() > 3){
      data->fill<float>(i_dphij4met, fabs(PhysicsUtilities::deltaPhi(*jets[3], *met)));
    }

    vector<RecoJetF*> jetsCSVranked(jets);
    cfgSet::sortByCSV(jetsCSVranked);
    double mtcsv1met = -99, mtcsv2met = -99, mtcsv12met = -99;
    if(jetsCSVranked.size() > 0) {
      data->fill<float>(i_csvj1pt, jetsCSVranked[0]->pt());
      if(jetsCSVranked[0]->csv() > defaults::CSV_LOOSE){
        mtcsv1met = JetKinematics::transverseMass(*jetsCSVranked[0], *met);
        mtcsv12met = mtcsv1met;
      }
      if(jetsCSVranked.size() > 1){
        data->fill<float>(i_csvj2pt, jetsCSVranked[1]->pt());
        if (jetsCSVranked[1]->csv() > defaults::CSV_LOOSE){
          mtcsv2met = JetKinematics::transverseMass(*jetsCSVranked[1], *met);
          mtcsv12met = min(mtcsv1met, mtcsv2met);
        }
      }
    }
    data->fill<float>(i_mtcsv1met, mtcsv1met);
    data->fill<float>(i_mtcsv2met, mtcsv2met);
    data->fill<float>(i_mtcsv12met, mtcsv12met);

    data->fill<int>(i_njl, ana->isrJets.size());
    if (ana->isrJets.size() > 0) {
      data->fill<float>(i_j1lpt,      ana->isrJets[0]->pt());
      data->fill<float>(i_dphij1lmet, fabs(PhysicsUtilities::deltaPhi(*ana->isrJets[0],*met)));
    }

    int nivf_ = 0;
    for (unsigned int iivf=0; iivf<ana->SVs.size(); ++iivf) {

      //DR between jets
      float mindrjetivf = 999999.;
      for (unsigned int ij = 0; ij<ana->jets.size(); ++ij) {

        if ( (ana->jets[ij]->pt()>20.)  && (fabs(ana->jets[ij]->eta())<2.4)) {
          float tmpdr =  PhysicsUtilities::deltaR(ana->SVs[iivf]->p4(), ana->jets[ij]->p4());
          if (tmpdr<mindrjetivf) { mindrjetivf = tmpdr; }
        }
      }

      if (fabs(ana->SVs[iivf]->svdxy()) < 3  &&
          ana->SVs[iivf]->svCosSVPV() > 0.98 &&
          ana->SVs[iivf]->pt()< 20.          &&
          ana->SVs[iivf]->svNTracks() >=3    &&
          mindrjetivf                 > 0.4  &&
          ((ana->SVs[iivf]->svd3D())/(ana->SVs[iivf]->svd3Derr())) > 4 ) { ++nivf_; }
    }
    data->fill<int>(i_nivf, nivf_);

    // Lepton variables
    if(ana->selectedLepton) {
      const auto * lep = ana->selectedLepton;
      auto WP4 = lep->p4() + ana->met->p4();
      data->fill<float>(i_leptonpt,     lep->pt());
      data->fill<float>(i_leptoneta,    lep->eta());
      data->fill<int  >(i_leptonpdgid,  lep->pdgid());
      data->fill<int  >(i_leptonq,      lep->q());
      data->fill<float>(i_mtlepmet,     JetKinematics::transverseMass(*lep, *ana->met)); // use the original met for MT
      data->fill<float>(i_lp,           (lep->px()*WP4.px() + lep->py()*WP4.py()) / (WP4.pt()*WP4.pt()) );

      double drrecoleprecow_ = 999.;
      double drrecoleprecotop_ = 999.;
      for (const auto *fj : ana->selectedSdWs) {
        double tmdr_ = PhysicsUtilities::deltaR(*fj, *lep);
        if (tmdr_<drrecoleprecow_) { drrecoleprecow_ = tmdr_; }
      }

      for (const auto *fj : ana->selectedSdTops) {
        double tmdr_ = PhysicsUtilities::deltaR(*fj, *lep);
        if (tmdr_<drrecoleprecotop_) { drrecoleprecotop_ = tmdr_; }
      }
      if ((drrecoleprecow_<0.8) || (drrecoleprecotop_<0.8)) { data->fill<bool>(i_recolepinwortop, true); }
      else { data->fill<bool>(i_recolepinwortop, false); }

      if(ana->nSelLeptons > 1) {
        auto dilepp4 = ana->selectedLeptons.at(0)->p4() + ana->selectedLeptons.at(1)->p4();
        data->fill<float>(i_dileppt, dilepp4.pt());
        data->fill<float>(i_dilepmass, dilepp4.mass());
      }
    }

    // Gen-level variables
    if(ana->isMC()) {
      int nGoodGenMu = 0; int nGoodGenEle = 0; int nPromptTaus = 0;
      std::vector<GenParticleF*> genw;
      for(auto* p : ana->genParts) {
        if (abs(p->pdgId())==24){ genw.push_back(p); }

        if (p->numberOfMothers()==0) continue;

        const auto *genPartMom = p->mother(0);
        if (ParticleInfo::isA(ParticleInfo::p_eminus, p) && (ParticleInfo::isA(ParticleInfo::p_Z0, genPartMom) || ParticleInfo::isA(ParticleInfo::p_Wplus, genPartMom) || ParticleInfo::isA(ParticleInfo::p_tauminus, genPartMom)))
          nGoodGenEle++;
        else if (ParticleInfo::isA(ParticleInfo::p_muminus, p) && (ParticleInfo::isA(ParticleInfo::p_Z0, genPartMom) || ParticleInfo::isA(ParticleInfo::p_Wplus, genPartMom) || ParticleInfo::isA(ParticleInfo::p_tauminus, genPartMom)))
          nGoodGenMu++;
        else if (ParticleInfo::isA(ParticleInfo::p_tauminus, p) && (ParticleInfo::isA(ParticleInfo::p_Z0, genPartMom) || ParticleInfo::isA(ParticleInfo::p_Wplus, genPartMom))) {
          bool lepDecay = false;
          for(unsigned int itd = 0; itd < p->numberOfDaughters(); itd++) {
            const GenParticleF* dau = p->daughter(itd);
            if(ParticleInfo::isA(ParticleInfo::p_eminus, dau) || ParticleInfo::isA(ParticleInfo::p_muminus, dau)) {
              lepDecay = true;
              break;
            }
          }
          if(!lepDecay)
            nPromptTaus++;
        }
      }
      data->fill<int  >(i_ngoodgenmu, nGoodGenMu);
      data->fill<int  >(i_ngoodgenele, nGoodGenEle);
      data->fill<int  >(i_npromptgentau, nPromptTaus);
      if (genw.size()>0) { data->fill<float>(i_genwpt, genw[0]->pt()); }
    }
  }

};


#endif
