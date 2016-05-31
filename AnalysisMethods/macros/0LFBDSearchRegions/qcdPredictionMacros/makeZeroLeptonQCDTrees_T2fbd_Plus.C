#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/macros/0LFBDSearchRegions/T2fbdHelper.hh"
#endif

using namespace ucsbsusy;

bool pairCompare_gt(const pair <float, pair <int, int> >& firstElem, const pair <float, pair <int, int> >& secondElem);

class ZeroLeptonQCDAnalyzer : public ZeroLeptonAnalyzer {

  public :
    std::vector<size8>*  savedSmearWeights;
    size i_bs                     ;
//    size i_trueResp               ;
//    size i_trueRespGenInd         ;
//    size i_trueRespRecoInd        ;
//    size i_trueRespFlv            ;
//    size i_trueRespPT             ;
//    size i_trueRespGenPT          ;
//    size i_pseudoResp             ;
//    size i_pseudoRespGenInd       ;
//    size i_pseudoRespRecoInd      ;
//    size i_pseudoRespPT           ;
//    size i_pseudoRespCSV          ;
//    size i_pseudoRespPseudoGenPT  ;
//    size i_pseudoRespPseudoGenHT  ;
//    size i_pseudoRespPassFilter   ;
//    size i_pseudoResp_4           ;
//    size i_pseudoRespGenInd_4     ;
//    size i_pseudoRespRecoInd_4    ;
//    size i_pseudoRespPT_4         ;
//    size i_pseudoRespCSV_4        ;
//    size i_pseudoRespPseudoGenPT_4;
//    size i_pseudoRespPseudoGenHT_4;
//    size i_pseudoRespPassFilter_4 ;
    size i_upTailWeight           ;
    size i_downTailWeight         ;
    size i_upBTagLightWeight      ;
    size i_upBTagHeavyWeight      ;
    size i_maxMuPT                ;
    size i_removeMuFrac           ;
    size i_lead_MMJ_pt            ;
    size i_lead_MMJ_eta           ;
    size i_lead_MMJ_dPhiGenReco   ;
    size i_lead_MMJ_res           ;
    size i_lead_MMJ_flavor        ;
    size i_lead_MMJ_rank          ;
    size i_lead_MMJ_recorank      ;
//    size i_secd_MMJ_pt            ;
//    size i_secd_MMJ_eta           ;
//    size i_secd_MMJ_dPhiGenReco   ;
//    size i_secd_MMJ_res           ;
//    size i_secd_MMJ_flavor        ;
//    size i_secd_MMJ_rank          ;
//    size i_secd_MMJ_recorank      ;
//    size i_secd_dMMJ2_dMMJ1       ;
//    size i_abso_MMJ_pt            ;
//    size i_abso_MMJ_eta           ;
//    size i_abso_MMJ_dPhiGenReco   ;
//    size i_abso_MMJ_res           ;
//    size i_abso_MMJ_flavor        ;
//    size i_abso_MMJ_rank          ;
//    size i_abso_MMJ_recorank      ;
//    size i_abso_dMMJ2_dMMJ1       ;
//    size i_jet_ID_min_dPhi_123    ;
//    size i_jet_ID_min_dPhi_1234   ;
//    size i_jet1_dPhi_genreco      ;
//    size i_jet2_dPhi_genreco      ;
//    size i_jet3_dPhi_genreco      ;
//    size i_jet4_dPhi_genreco      ;
//    size i_jet1_csv               ;
//    size i_jet2_csv               ;
//    size i_jet3_csv               ;
//    size i_jet4_csv               ;

    size i_maxFailPT                ;
    size i_maxFailETA               ;
    size i_ljChHad                  ;

    ZeroLeptonQCDAnalyzer(TString fileName, TString treeName, TString outfileName, size randSeed, bool isMCTree, cfgSet::ConfigSet *pars) :
      ZeroLeptonAnalyzer(fileName, treeName, outfileName, randSeed, isMCTree, pars), savedSmearWeights(new std::vector<size8>),
      i_bs                     (0),
//      i_trueResp               (0),
//      i_trueRespGenInd         (0),
//      i_trueRespRecoInd        (0),
//      i_trueRespFlv            (0),
//      i_trueRespPT             (0),
//      i_trueRespGenPT          (0),
//      i_pseudoResp             (0),
//      i_pseudoRespGenInd       (0),
//      i_pseudoRespRecoInd      (0),
//      i_pseudoRespPT           (0),
//      i_pseudoRespCSV          (0),
//      i_pseudoRespPseudoGenPT  (0),
//      i_pseudoRespPseudoGenHT  (0),
//      i_pseudoRespPassFilter   (0),
//      i_pseudoResp_4           (0),
//      i_pseudoRespGenInd_4     (0),
//      i_pseudoRespRecoInd_4    (0),
//      i_pseudoRespPT_4         (0),
//      i_pseudoRespCSV_4        (0),
//      i_pseudoRespPseudoGenPT_4(0),
//      i_pseudoRespPseudoGenHT_4(0),
//      i_pseudoRespPassFilter_4 (0),
      i_upTailWeight           (0),
      i_downTailWeight         (0),
      i_upBTagLightWeight      (0),
      i_upBTagHeavyWeight      (0),
      i_maxMuPT                (0),
      i_removeMuFrac           (0),
      i_lead_MMJ_pt            (0),
      i_lead_MMJ_eta           (0),
      i_lead_MMJ_dPhiGenReco   (0),
      i_lead_MMJ_res           (0),
      i_lead_MMJ_flavor        (0),
      i_lead_MMJ_rank          (0),
      i_lead_MMJ_recorank      (0),
      i_maxFailPT      (0),
      i_maxFailETA     (0),
      i_ljChHad (0)

//      i_secd_MMJ_pt            (0),
//      i_secd_MMJ_eta           (0),
//      i_secd_MMJ_dPhiGenReco   (0),
//      i_secd_MMJ_res           (0),
//      i_secd_MMJ_flavor        (0),
//      i_secd_MMJ_rank          (0),
//      i_secd_MMJ_recorank      (0),
//      i_secd_dMMJ2_dMMJ1       (0),
//      i_abso_MMJ_pt            (0),
//      i_abso_MMJ_eta           (0),
//      i_abso_MMJ_dPhiGenReco   (0),
//      i_abso_MMJ_res           (0),
//      i_abso_MMJ_flavor        (0),
//      i_abso_MMJ_rank          (0),
//      i_abso_MMJ_recorank      (0),
//      i_abso_dMMJ2_dMMJ1       (0),
//      i_jet_ID_min_dPhi_123    (0),
//      i_jet_ID_min_dPhi_1234   (0),
//      i_jet1_dPhi_genreco      (0),
//      i_jet2_dPhi_genreco      (0),
//      i_jet3_dPhi_genreco      (0),
//      i_jet4_dPhi_genreco      (0),
//      i_jet1_csv               (0),
//      i_jet2_csv               (0),
//      i_jet3_csv               (0),
//      i_jet4_csv               (0)
    {}

    virtual ~ZeroLeptonQCDAnalyzer() {delete savedSmearWeights;}

    void processVariables(){
      ZeroLeptonAnalyzer::processVariables();
    }

    void loadVariables() {
      setBranchAddress("","bootstrapWeight",&savedSmearWeights,false);
      ZeroLeptonAnalyzer::loadVariables();
    }

    void book() {
      ZeroLeptonAnalyzer::book();
      i_bs                      = data.addMulti<ucsbsusy::size8>("", "bootstrapWeight"   , 0);
//      i_trueResp                = data.add<float>       ("", "trueResp"               , "F", 0);
//      i_trueRespGenInd          = data.add<unsigned int>("", "trueRespGenInd"         , "i", 0);
//      i_trueRespRecoInd         = data.add<unsigned int>("", "trueRespRecoInd"        , "i", 0);
//      i_trueRespFlv             = data.add<unsigned int>("", "trueRespFlv"            , "i", 0);
//      i_trueRespPT              = data.add<float>       ("", "trueRespPT"             , "F", 0);
//      i_trueRespGenPT           = data.add<float>       ("", "trueRespGenPT"          , "F", 0);
//      i_pseudoResp              = data.add<float>       ("", "pseudoResp"             , "F", 0);
//      i_pseudoRespGenInd        = data.add<int>         ("", "pseudoRespGenInd"       , "I", 0);
//      i_pseudoRespRecoInd       = data.add<int>         ("", "pseudoRespRecoInd"      , "I", 0);
//      i_pseudoRespPT            = data.add<float>       ("", "pseudoRespPT"           , "F", 0);
//      i_pseudoRespCSV           = data.add<float>       ("", "pseudoRespCSV"          , "F", 0);
//      i_pseudoRespPseudoGenPT   = data.add<float>       ("", "pseudoRespPseudoGenPT"  , "F", 0);
//      i_pseudoRespPseudoGenHT   = data.add<float>       ("", "pseudoRespPseudoGenHT"  , "F", 0);
//      i_pseudoRespPassFilter    = data.add<bool>        ("", "pseudoRespPassFilter"   , "O", 0);
//      i_pseudoResp_4            = data.add<float>       ("", "pseudoResp_4"           , "F", 0);
//      i_pseudoRespGenInd_4      = data.add<int>         ("", "pseudoRespGenInd_4"     , "I", 0);
//      i_pseudoRespRecoInd_4     = data.add<int>         ("", "pseudoRespRecoInd_4"    , "I", 0);
//      i_pseudoRespPT_4          = data.add<float>       ("", "pseudoRespPT_4"         , "F", 0);
//      i_pseudoRespCSV_4         = data.add<float>       ("", "pseudoRespCSV_4"        , "F", 0);
//      i_pseudoRespPseudoGenPT_4 = data.add<float>       ("", "pseudoRespPseudoGenPT_4", "F", 0);
//      i_pseudoRespPseudoGenHT_4 = data.add<float>       ("", "pseudoRespPseudoGenHT_4", "F", 0);
//      i_pseudoRespPassFilter_4  = data.add<bool>        ("", "pseudoRespPassFilter_4" , "O", 0);
      i_upTailWeight            = data.add<float>       ("", "upTailWeight"           , "F", 0);
      i_downTailWeight          = data.add<float>       ("", "downTailWeight"         , "F", 0);
      i_upBTagLightWeight       = data.add<float>       ("", "upBTagLightWeight"      , "F", 0);
      i_upBTagHeavyWeight       = data.add<float>       ("", "upBTagHeavyWeight"      , "F", 0);
      i_maxMuPT                 = data.add<float>       ("", "maxMuPT"                , "F", 0);
      i_removeMuFrac            = data.add<float>       ("", "removeMuFrac"           , "F", 0);
      i_lead_MMJ_pt             = data.add<float>       ("", "lead_MMJ_pt"            , "F", 0);
      i_lead_MMJ_eta            = data.add<float>       ("", "lead_MMJ_eta"           , "F", 0);
      i_lead_MMJ_dPhiGenReco    = data.add<float>       ("", "lead_MMJ_dPhiGenReco"   , "F", 0);
      i_lead_MMJ_res            = data.add<float>       ("", "lead_MMJ_res"           , "F", 0);
      i_lead_MMJ_flavor         = data.add<int>         ("", "lead_MMJ_flavor"        , "I", 0);
      i_lead_MMJ_rank           = data.add<unsigned int>("", "lead_MMJ_rank"          , "i", 0);
      i_lead_MMJ_recorank       = data.add<unsigned int>("", "lead_MMJ_recorank"      , "i", 0);
      i_maxFailPT               = data.add<float>         ("", "maxFailPT"        , "F", 0);
      i_maxFailETA              = data.add<float>         ("", "maxFailETA"        , "F", -10);
      i_ljChHad                  = data.add<float>         ("", "ljChHad"        , "F", 1);
//      i_secd_MMJ_pt             = data.add<float>       ("", "secd_MMJ_pt"            , "F", 0);
//      i_secd_MMJ_eta            = data.add<float>       ("", "secd_MMJ_eta"           , "F", 0);
//      i_secd_MMJ_dPhiGenReco    = data.add<float>       ("", "secd_MMJ_dPhiGenReco"   , "F", 0);
//      i_secd_MMJ_res            = data.add<float>       ("", "secd_MMJ_res"           , "F", 0);
//      i_secd_MMJ_flavor         = data.add<int>         ("", "secd_MMJ_flavor"        , "I", 0);
//      i_secd_MMJ_rank           = data.add<unsigned int>("", "secd_MMJ_rank"          , "i", 0);
//      i_secd_MMJ_recorank       = data.add<unsigned int>("", "secd_MMJ_recorank"      , "i", 0);
//      i_secd_dMMJ2_dMMJ1        = data.add<float>       ("", "secd_dMMJ2_dMMJ1"       , "F", 0);
//      i_abso_MMJ_pt             = data.add<float>       ("", "abso_MMJ_pt"            , "F", 0);
//      i_abso_MMJ_eta            = data.add<float>       ("", "abso_MMJ_eta"           , "F", 0);
//      i_abso_MMJ_dPhiGenReco    = data.add<float>       ("", "abso_MMJ_dPhiGenReco"   , "F", 0);
//      i_abso_MMJ_res            = data.add<float>       ("", "abso_MMJ_res"           , "F", 0);
//      i_abso_MMJ_flavor         = data.add<int>         ("", "abso_MMJ_flavor"        , "I", 0);
//      i_abso_MMJ_rank           = data.add<unsigned int>("", "abso_MMJ_rank"          , "i", 0);
//      i_abso_MMJ_recorank       = data.add<unsigned int>("", "abso_MMJ_recorank"      , "i", 0);
//      i_abso_dMMJ2_dMMJ1        = data.add<float>       ("", "abso_dMMJ2_dMMJ1"       , "F", 0);
//      i_jet_ID_min_dPhi_123     = data.add<int>         ("", "jet_ID_min_dPhi_123"    , "I", 0);
//      i_jet_ID_min_dPhi_1234    = data.add<int>         ("", "jet_ID_min_dPhi_1234"   , "I", 0);
//      i_jet1_dPhi_genreco       = data.add<float>       ("", "jet1_dPhi_genreco"      , "F", 0);
//      i_jet2_dPhi_genreco       = data.add<float>       ("", "jet2_dPhi_genreco"      , "F", 0);
//      i_jet3_dPhi_genreco       = data.add<float>       ("", "jet3_dPhi_genreco"      , "F", 0);
//      i_jet4_dPhi_genreco       = data.add<float>       ("", "jet4_dPhi_genreco"      , "F", 0);
//      i_jet1_csv                = data.add<float>       ("", "jet1_csv"               , "F", 0);
//      i_jet2_csv                = data.add<float>       ("", "jet2_csv"               , "F", 0);
//      i_jet3_csv                = data.add<float>       ("", "jet3_csv"               , "F", 0);
//      i_jet4_csv                = data.add<float>       ("", "jet4_csv"               , "F", 0);
    }

    bool fillEvent() {
      if(!goodvertex) return false;
      if(met->pt() < 200  ) return false;

//get leading and second leading MM information
      if(isMC()){
        vector <pair <float, pair <int, int> > > jetMisMeasurementsByIdx;
        for(unsigned int iG = 0; iG < defaultJets->genJets.size(); ++iG){
          if(defaultJets->genJets[iG].pt() == 0) break;
  //        if(defaultJets->genJets[iG].pt() < 10.) continue;
          float reco_pt = -1;
          int rJI = -1;
          for(unsigned int iR = 0; iR < defaultJets->recoJets.size(); ++iR){
            if(defaultJets->recoJets[iR].genJet() != &defaultJets->genJets[iG]) continue;
            reco_pt = defaultJets->recoJets[iR].pt();
            rJI = iR;
            break;
          }
          if(reco_pt < 0 ) reco_pt = 9.5; //for the cases with no reco jet due to being below thresh
          jetMisMeasurementsByIdx.push_back(make_pair(fabs(reco_pt - defaultJets->genJets[iG].pt()), make_pair(iG, rJI)));
        }
        sort(jetMisMeasurementsByIdx.begin(), jetMisMeasurementsByIdx.end(), pairCompare_gt);
        int leadGenInd = process == defaults::QCD ? jetAndMETCorrections.getQCDRespTailCorrector()->mmInd : -1;
        int secdGenInd = -1, absoGenInd = -1, leadGenMatchInd = -1, secdGenMatchInd = -1, absoGenMatchInd = -1;
  //for(unsigned int iMM = 0; iMM < jetMisMeasurementsByIdx.size(); iMM++){
  //  cout << jetMisMeasurementsByIdx[iMM].first << ", " << jetMisMeasurementsByIdx[iMM].second.first << ", " << jetMisMeasurementsByIdx[iMM].second.second << endl;
  //}
        for(unsigned int iMM = 0; iMM < jetMisMeasurementsByIdx.size(); iMM++){
          if(jetMisMeasurementsByIdx[iMM].second.first < 3){
            if(jetMisMeasurementsByIdx[iMM].second.first == leadGenInd){
              leadGenMatchInd = jetMisMeasurementsByIdx[iMM].second.second;
            } else {
              secdGenInd      = jetMisMeasurementsByIdx[iMM].second.first;
              secdGenMatchInd = jetMisMeasurementsByIdx[iMM].second.second;
              break;
            }
          }
        }
        if(jetMisMeasurementsByIdx.size() > 0){
          absoGenInd      = jetMisMeasurementsByIdx[0].second.first;
          absoGenMatchInd = jetMisMeasurementsByIdx[0].second.second;
        }
  //cout << leadGenInd << ", " << leadGenMatchInd << endl;
  //cout << secdGenInd << ", " << secdGenMatchInd << endl;
  //cout << absoGenInd << ", " << absoGenMatchInd << endl << endl;

        data.fill<float>       (i_lead_MMJ_pt         , leadGenInd           >= 0 ? defaultJets->genJets[leadGenInd].pt() :  0);
        data.fill<float>       (i_lead_MMJ_eta        , leadGenInd           >= 0 ? defaultJets->genJets[leadGenInd].eta() : 0);
        data.fill<float>       (i_lead_MMJ_dPhiGenReco, leadGenMatchInd      >= 0 ? PhysicsUtilities::absDeltaPhi(defaultJets->recoJets[leadGenMatchInd], defaultJets->genJets[leadGenInd]) : 0);
        data.fill<float>       (i_lead_MMJ_res        , leadGenMatchInd      >= 0 ? defaultJets->recoJets[leadGenMatchInd].pt()/defaultJets->genJets[leadGenInd].pt() : 0);
        data.fill<int>         (i_lead_MMJ_flavor     , leadGenInd           >= 0 ? defaultJets->genJets[leadGenInd].flavor() : 99);
        data.fill<unsigned int>(i_lead_MMJ_rank       , leadGenInd           >= 0 ? leadGenInd : 99);
        data.fill<unsigned int>(i_lead_MMJ_recorank   , leadGenMatchInd      >= 0 ? leadGenMatchInd : 99);
//        data.fill<float>       (i_secd_MMJ_pt         , secdGenInd           >= 0 ? defaultJets->genJets[secdGenInd].pt() :  0);
//        data.fill<float>       (i_secd_MMJ_eta        , secdGenInd           >= 0 ? defaultJets->genJets[secdGenInd].eta() : 0);
//        data.fill<float>       (i_secd_MMJ_dPhiGenReco, secdGenMatchInd      >= 0 ? PhysicsUtilities::absDeltaPhi(defaultJets->recoJets[secdGenMatchInd], defaultJets->genJets[secdGenInd]) : 0);
//        data.fill<float>       (i_secd_MMJ_res        , secdGenMatchInd      >= 0 ? defaultJets->recoJets[secdGenMatchInd].pt()/defaultJets->genJets[secdGenInd].pt() : 0);
//        data.fill<int>         (i_secd_MMJ_flavor     , secdGenInd           >= 0 ? defaultJets->genJets[secdGenInd].flavor() : 99);
//        data.fill<unsigned int>(i_secd_MMJ_rank       , secdGenInd           >= 0 ? secdGenInd : 99);
//        data.fill<unsigned int>(i_secd_MMJ_recorank   , secdGenMatchInd      >= 0 ? secdGenMatchInd : 99);
//        data.fill<float>       (i_secd_dMMJ2_dMMJ1    , (leadGenMatchInd     >= 0 && secdGenMatchInd >= 0) ? (defaultJets->recoJets[secdGenMatchInd].pt() - defaultJets->genJets[secdGenInd].pt()) / (defaultJets->recoJets[leadGenMatchInd].pt() - defaultJets->genJets[leadGenInd].pt()) : 0);
//        data.fill<float>       (i_abso_MMJ_pt         , absoGenInd           >= 0 ? defaultJets->genJets[absoGenInd].pt() :  0);
//        data.fill<float>       (i_abso_MMJ_eta        , absoGenInd           >= 0 ? defaultJets->genJets[absoGenInd].eta() : 0);
//        data.fill<float>       (i_abso_MMJ_dPhiGenReco, absoGenMatchInd      >= 0 ? PhysicsUtilities::absDeltaPhi(defaultJets->recoJets[absoGenMatchInd], defaultJets->genJets[absoGenInd]) : 0);
//        data.fill<float>       (i_abso_MMJ_res        , absoGenMatchInd      >= 0 ? defaultJets->recoJets[absoGenMatchInd].pt()/defaultJets->genJets[absoGenInd].pt() : 0);
//        data.fill<int>         (i_abso_MMJ_flavor     , absoGenInd           >= 0 ? defaultJets->genJets[absoGenInd].flavor() : 99);
//        data.fill<unsigned int>(i_abso_MMJ_rank       , absoGenInd           >= 0 ? absoGenInd : 99);
//        data.fill<unsigned int>(i_abso_MMJ_recorank   , absoGenMatchInd      >= 0 ? absoGenMatchInd : 99);
//        data.fill<float>       (i_abso_dMMJ2_dMMJ1    , (absoGenMatchInd     >= 0 && secdGenMatchInd >= 0) ? (defaultJets->recoJets[secdGenMatchInd].pt() - defaultJets->genJets[secdGenInd].pt()) / (defaultJets->recoJets[absoGenMatchInd].pt() - defaultJets->genJets[absoGenInd].pt()) : 0);
      }

      // True response info
      int trueRespGenInd = process == defaults::QCD ? jetAndMETCorrections.getQCDRespTailCorrector()->mmInd : -1;
      int trueRespRecoInd = -1;
      unsigned int trueRespFlv = 99;
      float trueRespPT    = -1;
      float trueRespGenPT = -1;
      if(trueRespGenInd >= 0){
        for(const auto& j: defaultJets->genJets){
          if(j.index() != trueRespGenInd) continue;
          trueRespGenPT = j.pt();
          trueRespFlv = j.flavor();
          break;
        }
        for(unsigned int iJ = 0; iJ < defaultJets->recoJets.size(); ++iJ){
          if(defaultJets->recoJets[iJ].genJet() == &defaultJets->genJets[trueRespGenInd]){
            trueRespGenPT = defaultJets->recoJets[iJ].pt();
            trueRespRecoInd = iJ;
            break;
          }
        }
        assert(trueRespGenPT >= 0);
      }

//      data.fill<float>       (i_trueResp       , trueRespGenInd < 0 ? -1 : jetAndMETCorrections.getQCDRespTailCorrector()->mmResp);
//      data.fill<unsigned int>(i_trueRespGenInd , trueRespGenInd < 0 ? -1 : trueRespGenInd  + 1);
//      data.fill<unsigned int>(i_trueRespRecoInd, trueRespGenInd < 0 ? -1 : trueRespRecoInd + 1);
//      data.fill<unsigned int>(i_trueRespFlv    , trueRespFlv);
//      data.fill<float>       (i_trueRespPT     , trueRespPT);
//      data.fill<float>       (i_trueRespGenPT  , trueRespGenPT);

      //pseudo response info
      float jet_dPhis[4] = { -1., -1., -1., -1. };
      float jet_csvs[4]  = { -1., -1., -1., -1. };
      int pseudoRespGenInd = -1, pseudoRespGenInd_4 = -1, jetNearMETInd = -1, jetNearMETInd_4 = -1, MMJetDPhi = -1;
      for(unsigned int iJ = 0; iJ < defaultJets->recoJets.size() && iJ < 4; ++iJ){
        jet_csvs[iJ] = defaultJets->recoJets[iJ].csv();
        int genJetInd = -1;
        for(unsigned int iG = 0; iG < defaultJets->genJets.size(); ++iG){
          if(defaultJets->recoJets[iJ].genJet() == &defaultJets->genJets[iG]){
            genJetInd = iG;
            break;
          }
        }
        if(genJetInd >= 0){
          jet_dPhis[iJ] = PhysicsUtilities::absDeltaPhi(defaultJets->recoJets[iJ], defaultJets->genJets[genJetInd]);
        }
        double dPhi = PhysicsUtilities::absDeltaPhi(defaultJets->recoJets[iJ],*met);
        if(iJ < 3){
          if(MMJetDPhi < 0 || dPhi < MMJetDPhi){
            MMJetDPhi     = dPhi;
            jetNearMETInd   = iJ;
            jetNearMETInd_4 = iJ;
          }
        } else {
          if(dPhi < MMJetDPhi){
            jetNearMETInd_4 = iJ;
          }
        }
      }
      assert(jetNearMETInd >= 0);
      for(unsigned int iG = 0; iG < defaultJets->genJets.size(); ++iG){
        if(defaultJets->recoJets[jetNearMETInd].genJet() == &defaultJets->genJets[iG]){
          pseudoRespGenInd = iG;
          break;
        }
        if(defaultJets->recoJets[jetNearMETInd_4].genJet() == &defaultJets->genJets[iG]){
          pseudoRespGenInd_4 = iG;
          break;
        }
      }
      const auto* pJ   = &defaultJets->recoJets[jetNearMETInd];
      const auto* pJ_4 = &defaultJets->recoJets[jetNearMETInd_4];
      //There was a higher pt, filtered rank!
      bool passFilter   = jets.size() > jetNearMETInd   &&  pJ->index() == jets[jetNearMETInd  ]->index();
      bool passFilter_4 = jets.size() > jetNearMETInd_4 &&  pJ->index() == jets[jetNearMETInd_4]->index();

      double pseudoGenPT    = (met->p4() + pJ->p4()).pt();
      double pseudoGenPT_4  = (met->p4() + pJ_4->p4()).pt();
      double MMPseudoResp   = pseudoGenPT   > 0 ? pJ->pt()   / pseudoGenPT   : 999;
      double MMPseudoResp_4 = pseudoGenPT_4 > 0 ? pJ_4->pt() / pseudoGenPT_4 : 999;
      double pseudoGenHT = 0, pseudoGenHT_4 = 0;
      for(unsigned int iJ = 0; iJ < defaultJets->recoJets.size(); ++iJ){
        if(jetNearMETInd == iJ){
          pseudoGenHT += pseudoGenPT;
        } else {
          if(!JetKinematics::passCuts(defaultJets->recoJets[iJ], 20.0, 2.4)) continue;
          pseudoGenHT += defaultJets->recoJets[iJ].pt();
        }
        if(jetNearMETInd_4 == iJ){
          pseudoGenHT_4 += pseudoGenPT_4;
        } else {
          if(!JetKinematics::passCuts(defaultJets->recoJets[iJ], 20.0, 2.4)) continue;
          pseudoGenHT_4 += defaultJets->recoJets[iJ].pt();
        }
      }

//      data.fill<float> (i_pseudoResp             , MMPseudoResp);
//      data.fill<int>   (i_pseudoRespGenInd       , pseudoRespGenInd >= 0 ? pseudoRespGenInd + 1 : 99);
//      data.fill<int>   (i_pseudoRespRecoInd      , jetNearMETInd    >= 0 ? jetNearMETInd    + 1 : 99);
//      data.fill<float> (i_pseudoRespPT           , pJ->p4().pt());
//      data.fill<float> (i_pseudoRespCSV          , pJ->csv());
//      data.fill<float> (i_pseudoRespPseudoGenPT  , pseudoGenPT);
//      data.fill<float> (i_pseudoRespPseudoGenHT  , pseudoGenHT);
//      data.fill<bool>  (i_pseudoRespPassFilter   , passFilter);
//      data.fill<float> (i_pseudoResp_4           , MMPseudoResp_4);
//      data.fill<int>   (i_pseudoRespGenInd_4     , pseudoRespGenInd_4 >= 0 ? pseudoRespGenInd_4 + 1 : 99);
//      data.fill<int>   (i_pseudoRespRecoInd_4    , jetNearMETInd_4    >= 0 ? jetNearMETInd_4    + 1 : 99);
//      data.fill<float> (i_pseudoRespPT_4         , pJ_4->p4().pt());
//      data.fill<float> (i_pseudoRespCSV_4        , pJ_4->csv());
//      data.fill<float> (i_pseudoRespPseudoGenPT_4, pseudoGenPT_4);
//      data.fill<float> (i_pseudoRespPseudoGenHT_4, pseudoGenHT_4);
//      data.fill<bool>  (i_pseudoRespPassFilter_4 , passFilter_4);
//      data.fill<float> (i_jet1_dPhi_genreco      , jet_dPhis[0]);
//      data.fill<float> (i_jet2_dPhi_genreco      , jet_dPhis[1]);
//      data.fill<float> (i_jet3_dPhi_genreco      , jet_dPhis[2]);
//      data.fill<float> (i_jet4_dPhi_genreco      , jet_dPhis[3]);
//      data.fill<float> (i_jet1_csv               , jet_csvs[0]);
//      data.fill<float> (i_jet2_csv               , jet_csvs[1]);
//      data.fill<float> (i_jet3_csv               , jet_csvs[2]);
//      data.fill<float> (i_jet4_csv               , jet_csvs[3]);

      //extra weights for syst. studies
      data.fill<float>(i_upTailWeight    ,jetAndMETCorrections.getQCDRespTailCorrector()->getWeight(UP));
      data.fill<float>(i_downTailWeight  ,jetAndMETCorrections.getQCDRespTailCorrector()->getWeight(DOWN));

      //re-calculate b-tag SF
      {
        bool isTTBARLike = false;
        if(process == defaults::TTBAR || process == defaults::SINGLE_T || process == defaults::TTZ || process == defaults::TTW )
          isTTBARLike = true;
        data.fill<float>(i_upBTagLightWeight , float(bTagCorrections.getBTagByEvtWeightCorrector()->getEvtWeight(jets,UP,NOMINAL,isTTBARLike, configSet.jets.maxBJetEta,configSet.jets.minBJetPt)));
        data.fill<float>(i_upBTagHeavyWeight , float(bTagCorrections.getBTagByEvtWeightCorrector()->getEvtWeight(jets,NOMINAL,UP,isTTBARLike, configSet.jets.maxBJetEta,configSet.jets.minBJetPt)));
      }

      //copy the bootstrapweights
      for(auto i : *savedSmearWeights)
        data.fillMulti<ucsbsusy::size8>(i_bs,i);

      //filter
      float maxMuPT      = -1;
      float removeMuFrac = -10;
      const LeptonF * testMu = 0;
      for(const auto* m : allLeptons){
        if(m->iselectron()) continue;
        testMu = m;
        break;
      }
      if(testMu && met->pt() > 0){
        maxMuPT = testMu->pt();
        bool withinJet = false;
        for(unsigned int iJ = 0; iJ < jets.size() && iJ < 2; ++iJ){
          if(PhysicsUtilities::deltaR(*jets[iJ],*testMu) < .4 ){
            withinJet = true;
            break;
          }
        }
        if(withinJet && PhysicsUtilities::absDeltaPhi(met->p4(),testMu->p4()) > 2.6){
          auto metNoLep = met->p4();
          metNoLep += testMu->p4();
          removeMuFrac = 1 - metNoLep.pt()/met->pt();
        }
      }
      data.fill<float>(i_maxMuPT,maxMuPT);
      data.fill<float>(i_removeMuFrac,removeMuFrac);

      double maxPT = -1;
      double maxETA = -10;

      double leadJetCH = -1;
      double leadJetPT = 0;
//      for(auto& j : defaultJets->recoJets){
//        if(j.pt() > leadJetPT){
//          leadJetPT = j.pt();
//          leadJetCH = defaultJets->jetchHadEnFrac_->at(j.index());
//        }
//        if(j.absEta() > 4.7) continue;
//        if(defaultJets->jetchHadEnFrac_->at(j.index()) < .1 && j.pt() > maxPT){
//          maxPT = j.pt();
//          maxETA = j.eta();
//        }
//      }
      data.fill<float>(i_maxFailPT,maxPT);
      data.fill<float>(i_maxFailETA,maxETA);

      data.fill<float>(i_ljChHad,leadJetCH);

      filler.fillEventInfo(&data, this);
      filler.fillJetInfo  (&data, jets, bJets, met);





      return true;
    }
};

bool pairCompare_gt(const pair <float, pair <int, int> >& firstElem, const pair <float, pair <int, int> >& secondElem){
  return firstElem.first > secondElem.first;
}

void makeZeroLeptonQCDTrees_T2fbd_Plus(TString sname = "T2tt_375_355",
                                       const int fileindex = -1,
                                       const bool isMC = true,
                                       const TString fname = "/store/user/loukas/13TeV/20160315/T2fbd_375_355_ntuple_wgtxsec.root",
                                       const TString outputdir = "trees",
                                       const TString fileprefix = "root://cmseos:1094/",
//                                       const TString fileprefix = "",
                                       const TString json = "/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions15/13TeV/Cert_246908-258159_13TeV_PromptReco_Collisions15_25ns_JSON_v3.txt")
//void makeZeroLeptonQCDTrees_T2fbd_Plus(TString sname = "QCD",
//                                       const int fileindex = -1,
//                                       const bool isMC = true,
//                                       const TString fname = "/store/user/jzabel/13TeV/merged_combined_samples_wgtxsec_qcd_skim/qcd_ht100to200_1_ntuple_wgtxsec_1_qcdSkim.root",
//                                       const TString outputdir = "trees",
//                                       const TString fileprefix = "root://cmsxrootd.fnal.gov/",
////                                       const TString fileprefix = "",
//                                       const TString json = "/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions15/13TeV/Cert_246908-258159_13TeV_PromptReco_Collisions15_25ns_JSON_v3.txt")
{
  printf("Processing file %d of %s sample\n", (fileindex > -1 ? fileindex : 0), sname.Data());

  if(fileindex > -1)
    sname += TString::Format("_%d",fileindex);

  TString fullname = fileprefix+fname;

  gSystem->mkdir(outputdir,true);
  TString outfilename = outputdir+"/"+sname+"_tree.root";
  cfgSet::ConfigSet pars = pars0lep(json);
  pars.corrections.jetResTailCorrType = NOMINAL;
  pars.corrections.jetResCorrType = ucsbsusy::NONE;
  pars.corrections.jetAndMETCorrections     = ucsbsusy::JetAndMETCorrectionSet::QCDRESPTAIL;

// applying jet ResCoreSmearing
//  pars.corrections.jetResCorrType = NOMINAL;
//  pars.corrections.jetAndMETCorrections     = ucsbsusy::JetAndMETCorrectionSet::QCDRESPTAIL | ucsbsusy::JetAndMETCorrectionSet::JETRESOLUTION;

  TString treeName = "Events";
  ZeroLeptonQCDAnalyzer a(fullname, treeName, outfilename, fileindex+2, isMC, &pars);
  if (fname.Contains("ttbar-mg-onelep")){
    cerr << "##############################################" << endl;
    cerr << "######    Will correct ttbar-1l xsec   #######" << endl;
    cerr << "##############################################" << endl;
    a.isOneLepTTbar = true;
  }

  a.analyze(1000000,10000);
}
