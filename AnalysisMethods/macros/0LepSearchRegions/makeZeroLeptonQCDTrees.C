#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/macros/0LepSearchRegions/ZeroLeptonTreeHelper.hh"
#endif

using namespace ucsbsusy;

bool pairCompare_gt(const pair <float, pair <int, int> >& firstElem, const pair <float, pair <int, int> >& secondElem);

class ZeroLeptonQCDAnalyzer : public ZeroLeptonAnalyzer {

  public :

  ZeroLeptonQCDAnalyzer(TString fileName, TString treeName, TString outfileName, size randSeed, bool isMCTree, cfgSet::ConfigSet *pars) :
      ZeroLeptonAnalyzer(fileName, treeName, outfileName, randSeed, isMCTree, pars), savedSmearWeights(new std::vector<size8>),
      i_bs(0),
      i_trueResp             (0),
      i_trueRespFlv          (0),
      i_trueRespGenPT        (0),
      i_pseudoResp           (0),
      i_pseudoRespCSV        (0),
      i_pseudoRespPseudoGenPT(0),
      i_pseudoRespPassFilter (0),
      i_upTailWeight         (0),
      i_downTailWeight       (0),
      i_upBTagLightWeight    (0),
      i_upBTagHeavyWeight    (0),
      i_maxMuPT              (0),
      i_removeMuFrac         (0),
      i_whichQCD_HT_Sample   (0),
      i_whichQCD_Sample      (0),
      whichQCD_HT_Sample     (0),
      whichQCD_Sample        (0),
      i_lead_MMJ_res         (0),
      i_lead_MMJ_rank        (0),
      i_lead_MMJ_recorank    (0),
      i_lead_MMJ_MET_frac    (0),
      i_secd_MMJ_res         (0),
      i_secd_MMJ_rank        (0),
      i_secd_MMJ_recorank    (0),
      i_secd_MMJ_MET_frac    (0),
      i_abso_MMJ_res         (0),
      i_abso_MMJ_rank        (0),
      i_abso_MMJ_recorank    (0),
      i_abso_MMJ_MET_frac    (0),
      i_all_genJet_pt        (0),
      i_all_recoJet_pt       (0)
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
      i_bs                    = data.addMulti<ucsbsusy::size8>("","bootstrapWeight",     0);
      i_trueResp              = data.add<float>       ("", "trueResp"             , "F", 0);
      i_trueRespFlv           = data.add<unsigned int>("", "trueRespFlv"          , "i", 0);
      i_trueRespGenPT         = data.add<float>       ("", "trueRespGenPT"        , "F", 0);
      i_pseudoResp            = data.add<float>       ("", "pseudoResp"           , "F", 0);
      i_pseudoRespCSV         = data.add<float>       ("", "pseudoRespCSV"        , "F", 0);
      i_pseudoRespPseudoGenPT = data.add<float>       ("", "pseudoRespPseudoGenPT", "F", 0);
      i_pseudoRespPassFilter  = data.add<bool>        ("", "pseudoRespPassFilter" , "O", 0);
      i_upTailWeight          = data.add<float>       ("", "upTailWeight"         , "F", 0);
      i_downTailWeight        = data.add<float>       ("", "downTailWeight"       , "F", 0);
      i_upBTagLightWeight     = data.add<float>       ("", "upBTagLightWeight"    , "F", 0);
      i_upBTagHeavyWeight     = data.add<float>       ("", "upBTagHeavyWeight"    , "F", 0);
      i_maxMuPT               = data.add<float>       ("", "maxMuPT"              , "F", 0);
      i_removeMuFrac          = data.add<float>       ("", "removeMuFrac"         , "F", 0);
      i_whichQCD_HT_Sample    = data.add<int>         ("", "whichQCD_HT_Sample"   , "I", 0);
      i_whichQCD_Sample       = data.add<int>         ("", "whichQCD_Sample"      , "I", 0);
      i_lead_MMJ_res          = data.add<float>       ("", "lead_MMJ_res"         , "F", 0);
      i_lead_MMJ_rank         = data.add<unsigned int>("", "lead_MMJ_rank"        , "i", 0);
      i_lead_MMJ_recorank     = data.add<unsigned int>("", "lead_MMJ_recorank"    , "i", 0);
      i_lead_MMJ_MET_frac     = data.add<float>       ("", "lead_MMJ_MET_frac"    , "F", 0);
      i_secd_MMJ_res          = data.add<float>       ("", "secd_MMJ_res"         , "F", 0);
      i_secd_MMJ_rank         = data.add<unsigned int>("", "secd_MMJ_rank"        , "i", 0);
      i_secd_MMJ_recorank     = data.add<unsigned int>("", "secd_MMJ_recorank"    , "i", 0);
      i_secd_MMJ_MET_frac     = data.add<float>       ("", "secd_MMJ_MET_frac"    , "F", 0);
      i_abso_MMJ_res          = data.add<float>       ("", "abso_MMJ_res"         , "F", 0);
      i_abso_MMJ_rank         = data.add<unsigned int>("", "abso_MMJ_rank"        , "i", 0);
      i_abso_MMJ_recorank     = data.add<unsigned int>("", "abso_MMJ_recorank"    , "i", 0);
      i_abso_MMJ_MET_frac     = data.add<float>       ("", "abso_MMJ_MET_frac"    , "F", 0);
      i_all_genJet_pt         = data.addMulti<float>  ("", "all_genJet_pt",              0);
      i_all_recoJet_pt        = data.addMulti<float>  ("", "all_recoJet_pt",             0);
    }

    bool fillEvent() {
      if(!goodvertex)         return false;
      if(met->pt() < metcut_) return false;
      if(nJets < minnjets_)   return false;


      if (applyTightPresel){
        bool passLM = met->pt()/(std::sqrt(JetKinematics::ht(jets)))>10;
        bool passHM = nJets>=5 && nBJets>=1;
        if (!passLM && !passHM) return false;
      }
      processMoreVariables(); // call this before filling, but after all preselections

//get leading and second leading MM information
      if(isMC()){
        vector <pair <float, pair <int, int> > > jetMisMeasurementsByIdx;
        for(unsigned int iG = 0; iG < defaultJets->genJets.size(); ++iG){
          if(defaultJets->genJets[iG].pt() == 0) break;
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
        absoGenInd      = jetMisMeasurementsByIdx[0].second.first;
        absoGenMatchInd = jetMisMeasurementsByIdx[0].second.second;

        const GenJetF*  leadGenJet  = 0;
        const GenJetF*  secdGenJet  = 0;
        const GenJetF*  absoGenJet  = 0;
        const RecoJetF* leadRecoJet = 0;
        const RecoJetF* secdRecoJet = 0;
        const RecoJetF* absoRecoJet = 0;
        if(leadGenInd >= 0)      leadGenJet  = &defaultJets->genJets[leadGenInd];
        if(leadGenMatchInd >= 0) leadRecoJet = &defaultJets->recoJets[leadGenMatchInd];
        if(secdGenInd >= 0)      secdGenJet  = &defaultJets->genJets[secdGenInd];
        if(secdGenMatchInd >= 0) secdRecoJet = &defaultJets->recoJets[secdGenMatchInd];
        if(absoGenInd >= 0)      absoGenJet  = &defaultJets->genJets[absoGenInd];
        if(absoGenMatchInd >= 0) absoRecoJet = &defaultJets->recoJets[absoGenMatchInd];
        double leadActualMET = -1;
        double secdActualMET = -1;
        double absoActualMET = -1;
        if(leadGenJet && leadRecoJet) leadActualMET = (met->p4() + leadRecoJet->p4() - leadGenJet->p4()).pt();
        if(secdGenJet && secdRecoJet) secdActualMET = (met->p4() + secdRecoJet->p4() - secdGenJet->p4()).pt();
        if(absoGenJet && absoRecoJet) absoActualMET = (met->p4() + absoRecoJet->p4() - absoGenJet->p4()).pt();

        data.fill<float>       (i_lead_MMJ_res     , leadGenMatchInd >= 0 ? defaultJets->recoJets[leadGenMatchInd].pt()/defaultJets->genJets[leadGenInd].pt() : 0);
        data.fill<unsigned int>(i_lead_MMJ_rank    , leadGenInd      >= 0 ? leadGenInd : 99); 
        data.fill<unsigned int>(i_lead_MMJ_recorank, leadGenMatchInd >= 0 ? leadGenMatchInd : 99);
        data.fill<float>       (i_lead_MMJ_MET_frac, leadActualMET   >= 0 ? leadActualMET/met->pt() : 0);
        data.fill<float>       (i_secd_MMJ_res     , secdGenMatchInd >= 0 ? defaultJets->recoJets[secdGenMatchInd].pt()/defaultJets->genJets[secdGenInd].pt() : 0);
        data.fill<unsigned int>(i_secd_MMJ_rank    , secdGenInd      >= 0 ? secdGenInd : 99); 
        data.fill<unsigned int>(i_secd_MMJ_recorank, secdGenMatchInd >= 0 ? secdGenMatchInd : 99); 
        data.fill<float>       (i_secd_MMJ_MET_frac, secdActualMET   >= 0 ? secdActualMET/met->pt() : 0);
        data.fill<float>       (i_abso_MMJ_res     , absoGenMatchInd >= 0 ? defaultJets->recoJets[absoGenMatchInd].pt()/defaultJets->genJets[absoGenInd].pt() : 0);
        data.fill<unsigned int>(i_abso_MMJ_rank    , absoGenInd      >= 0 ? absoGenInd : 99); 
        data.fill<unsigned int>(i_abso_MMJ_recorank, absoGenMatchInd >= 0 ? absoGenMatchInd : 99); 
        data.fill<float>       (i_abso_MMJ_MET_frac, absoActualMET   >= 0 ? absoActualMET/met->pt() : 0);

        bool passmetfilters = this->evtInfoReader.HBHENoiseFilter &&
                              this->evtInfoReader.HBHENoiseIsoFilter &&
                              this->evtInfoReader.globalTightHalo2016Filter &&
                              this->evtInfoReader.EcalDeadCellTriggerPrimitiveFilter &&
                              this->evtInfoReader.goodVertices &&
                              this->evtInfoReader.eeBadScFilter &&
                              this->evtInfoReader.badChCand &&
                              this->evtInfoReader.badPFMuon;
        bool passmetfilters2017 = this->evtInfoReader.HBHENoiseFilter &&
                              this->evtInfoReader.HBHENoiseIsoFilter &&
                              this->evtInfoReader.globalSuperTightHalo2016Filter &&
                              this->evtInfoReader.EcalDeadCellTriggerPrimitiveFilter &&
                              this->evtInfoReader.goodVertices &&
                              this->evtInfoReader.eeBadScFilter &&
                              this->evtInfoReader.badChCand &&
                              this->evtInfoReader.badPFMuon;
        float ljCHEF = defaultJets->jetchHadEnFrac_->size() ? defaultJets->jetchHadEnFrac_->at(defaultJets->recoJets[0].index()) : 10;
        if((passmetfilters2017 || passmetfilters) && (ljCHEF > 0.1) && (ljCHEF < 0.99)){
//          for(unsigned int iJ = 0; iJ < defaultJets->genJets.size(); ++iJ){
          for(unsigned int iJ = 0; iJ < 2; ++iJ){
            const auto& gJ = defaultJets->genJets[iJ];
            if (gJ.pt()  < 20) continue;
            const RecoJetF* rJ = 0;
            for(unsigned int iR = 0; iR < defaultJets->recoJets.size(); ++iR){
              if(defaultJets->recoJets[iR].genJet() != &gJ) continue;
              rJ = &defaultJets->recoJets[iR];
              break;
            }
            data.fillMulti<float>(i_all_genJet_pt,  gJ.pt() );
            data.fillMulti<float>(i_all_recoJet_pt, rJ == 0 ? 9.5 : rJ->pt());
          }
        }
      }


      // True response info
      int trueRespInd = process == defaults::QCD ? jetAndMETCorrections.getQCDRespTailCorrector()->mmInd : -1;
      unsigned int trueRespFlv = 99;
      float trueRespGenPT = -1;
      if(trueRespInd >= 0){
        for(const auto& j: defaultJets->genJets){
          if(j.index() != trueRespInd) continue;
          trueRespGenPT = j.pt();
          trueRespFlv = j.flavor();
          break;
        }
        assert(trueRespGenPT >= 0);
      }

      data.fill<float>       (i_trueResp     ,trueRespInd < 0 ? -1 : jetAndMETCorrections.getQCDRespTailCorrector()->mmResp);
      data.fill<unsigned int>(i_trueRespFlv  ,trueRespFlv);
      data.fill<float>       (i_trueRespGenPT,trueRespGenPT);


      //pseudo response info
      int jetNearMETInd = -1, MMJetDPhi = -1;
      for(unsigned int iJ = 0; iJ < defaultJets->recoJets.size() && iJ < 3; ++iJ){
        double dPhi = PhysicsUtilities::absDeltaPhi(defaultJets->recoJets[iJ],*met);
        if(MMJetDPhi < 0 || dPhi < MMJetDPhi){
          MMJetDPhi     = dPhi;
          jetNearMETInd = iJ;
        }
      }
      assert(jetNearMETInd >= 0);
      const auto* pJ = &defaultJets->recoJets[jetNearMETInd];
      //There was a higher pt, filtered rank!
      bool passFilter  = jets.size() > jetNearMETInd &&  pJ->index() == jets[jetNearMETInd]->index();

      double pseudoGenPT = (met->p4() + pJ->p4()).pt();
      double MMPseudoResp = pseudoGenPT > 0 ? pJ->pt()/ pseudoGenPT : 999;

      data.fill<float>(i_pseudoResp           ,MMPseudoResp);
      data.fill<float>(i_pseudoRespCSV        ,pJ->csv());
      data.fill<float>(i_pseudoRespPseudoGenPT,pseudoGenPT);
      data.fill<bool> (i_pseudoRespPassFilter, passFilter);

      //extra weights for syst. studies
      data.fill<float>(i_upTailWeight    ,jetAndMETCorrections.getQCDRespTailCorrector()->getWeight(UP));
      data.fill<float>(i_downTailWeight  ,jetAndMETCorrections.getQCDRespTailCorrector()->getWeight(DOWN));

      //re-calculate b-tag SF
      {
        bool isTTBARLike = false;
        if(process == defaults::TTBAR || process == defaults::SINGLE_T || process == defaults::TTZ || process == defaults::TTW )
          isTTBARLike = true;
        data.fill<float>(i_upBTagLightWeight , float(bTagCorrections.getBTagByEvtWeightCorrector()->getEvtWeight(this,jets,UP,NOMINAL,isTTBARLike, configSet.jets.maxBJetEta,configSet.jets.minBJetPt)));
        data.fill<float>(i_upBTagHeavyWeight , float(bTagCorrections.getBTagByEvtWeightCorrector()->getEvtWeight(this,jets,NOMINAL,UP,isTTBARLike, configSet.jets.maxBJetEta,configSet.jets.minBJetPt)));
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
      data.fill<int>(i_whichQCD_HT_Sample, whichQCD_HT_Sample);
      data.fill<int>(i_whichQCD_Sample,    whichQCD_Sample);

      filler.fillEventInfo(&data, this);
      return true;
    }

    std::vector<size8>*  savedSmearWeights;
    size i_bs                   ;
    size i_trueResp             ;
    size i_trueRespFlv          ;
    size i_trueRespGenPT        ;
    size i_pseudoResp           ;
    size i_pseudoRespCSV        ;
    size i_pseudoRespPseudoGenPT;
    size i_pseudoRespPassFilter ;
    size i_upTailWeight         ;
    size i_downTailWeight       ;
    size i_upBTagLightWeight    ;
    size i_upBTagHeavyWeight    ;
    size i_maxMuPT              ;
    size i_removeMuFrac         ;
    size i_whichQCD_HT_Sample   ;
    size i_whichQCD_Sample      ;
    int whichQCD_HT_Sample      ;
    int whichQCD_Sample         ;

    size i_lead_MMJ_res         ;
    size i_lead_MMJ_rank        ;
    size i_lead_MMJ_recorank    ;
    size i_lead_MMJ_MET_frac    ;
    size i_secd_MMJ_res         ;
    size i_secd_MMJ_rank        ;
    size i_secd_MMJ_recorank    ;
    size i_secd_MMJ_MET_frac    ;
    size i_abso_MMJ_res         ;
    size i_abso_MMJ_rank        ;
    size i_abso_MMJ_recorank    ;
    size i_abso_MMJ_MET_frac    ;
    size i_all_genJet_pt        ;
    size i_all_recoJet_pt       ;
};

bool pairCompare_gt(const pair <float, pair <int, int> >& firstElem, const pair <float, pair <int, int> >& secondElem){
  return firstElem.first > secondElem.first;
}

//void makeZeroLeptonQCDTrees(TString sname = "htmht",
void makeZeroLeptonQCDTrees(TString sname = "qcd",
                           const int fileindex = 1,
//                           const bool isMC = false,
                           const bool isMC = true,
//                           const TString fname = "/store/user/gouskos/13TeV/Spring15/20150813/htmht-2015b-pr_ntuple_postproc.root",
//                           const TString fname = "/store/user/lpcstop/noreplica/13TeV/130117/Moriond17/patterson/merged/ttbar-2l-madgraph-ext_11_ntuple_postproc.root",
                           const TString fname = "/store/user/lpcstop/noreplica/13TeV/130117/Moriond17/qcd_orig/qcd_ht1000to1500-ext_10_ntuple_postproc.root",
                           const TString outputdir = "trees",
                           const TString fileprefix = "root://cmseos:1094/",
                           const TString json="Cert_294927-297723_13TeV_PromptReco_Collisions17_JSON.txt")
{
  int whichQCD_HT_Sample = 0;
  int whichQCD_Sample    = 0;
       if(fname.Contains("200to300")  ) { whichQCD_HT_Sample = 1; }
  else if(fname.Contains("300to500")  ) { whichQCD_HT_Sample = 2; }
  else if(fname.Contains("500to700")  ) { whichQCD_HT_Sample = 3; }
  else if(fname.Contains("700to1000") ) { whichQCD_HT_Sample = 4; }
  else if(fname.Contains("1000to1500")) { whichQCD_HT_Sample = 5; }
  else if(fname.Contains("1500to2000")) { whichQCD_HT_Sample = 6; }
  else if(fname.Contains("2000toinf") ) { whichQCD_HT_Sample = 7; }
       if(fname.Contains("-ext"))       { whichQCD_Sample    = 1; }
  else if(fname.Contains("genjets5"))   { whichQCD_Sample    = 2; }

  printf("Processing file %d of %s sample\n", (fileindex > -1 ? fileindex : 0), sname.Data());

  if(fileindex > -1)
    sname += TString::Format("_%d",fileindex);

  TString fullname = fileprefix+fname;
  //TString fullname = fname;

  gSystem->mkdir(outputdir,true);
  TString outfilename = outputdir+"/"+sname+"_tree.root";
  cfgSet::ConfigSet pars = pars0lep(json);
  pars.corrections.jetResTailCorrType = NOMINAL;
  pars.corrections.jetResCorrType = ucsbsusy::NONE;
  pars.corrections.jetAndMETCorrections     = ucsbsusy::JetAndMETCorrectionSet::QCDRESPTAIL;

  TString treeName = "Events";
  ZeroLeptonQCDAnalyzer a(fullname, treeName, outfilename, fileindex+2, isMC, &pars);

  a.whichQCD_HT_Sample = whichQCD_HT_Sample;
  a.whichQCD_Sample    = whichQCD_Sample;
  a.analyze(1000000);
//  a.analyze(1000,10000);
}
