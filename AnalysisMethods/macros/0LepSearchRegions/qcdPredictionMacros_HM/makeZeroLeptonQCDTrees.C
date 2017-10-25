#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/macros/0LepSearchRegions/ZeroLeptonTreeHelper.hh"
#endif

using namespace ucsbsusy;

bool pairCompare_gt(const pair <float, pair <int, int> >& firstElem, const pair <float, pair <int, int> >& secondElem);

class ZeroLeptonQCDAnalyzer : public ZeroLeptonAnalyzer {

  public :
    std::vector<size8>*  savedSmearWeights;
    size i_bs                   ;
    size i_trueResp             ;
    size i_trueRespGenInd       ;
    size i_trueRespRecoInd      ;
    size i_trueRespFlv          ;
    size i_trueRespPT           ;
    size i_trueRespGenPT        ;
    size i_pseudoResp           ;
    size i_pseudoRespGenInd     ;
    size i_pseudoRespRecoInd    ;
    size i_pseudoRespPT         ;
    size i_pseudoRespCSV        ;
    size i_pseudoRespPseudoGenPT;
    size i_pseudoRespPseudoGenHT;
    size i_pseudoRespPassFilter ;
    size i_upTailWeight         ;
    size i_downTailWeight       ;
    size i_upBTagLightWeight    ;
    size i_upBTagHeavyWeight    ;
    size i_ljCHEF               ;
    size i_whichQCD_HT_Sample   ;
    size i_whichQCD_Sample      ;
    int whichQCD_HT_Sample      ;
    int whichQCD_Sample         ;

    ZeroLeptonQCDAnalyzer(TString fileName, TString treeName, TString outfileName, size randSeed, bool isMCTree, cfgSet::ConfigSet *pars) :
      ZeroLeptonAnalyzer(fileName, treeName, outfileName, randSeed, isMCTree, pars), savedSmearWeights(new std::vector<size8>),
      i_bs                     (0),
      i_trueResp               (0),
      i_trueRespGenInd         (0),
      i_trueRespRecoInd        (0),
      i_trueRespFlv            (0),
      i_trueRespPT             (0),
      i_trueRespGenPT          (0),
      i_pseudoResp             (0),
      i_pseudoRespGenInd       (0),
      i_pseudoRespRecoInd      (0),
      i_pseudoRespPT           (0),
      i_pseudoRespCSV          (0),
      i_pseudoRespPseudoGenPT  (0),
      i_pseudoRespPseudoGenHT  (0),
      i_pseudoRespPassFilter   (0),
      i_upTailWeight           (0),
      i_downTailWeight         (0),
      i_upBTagLightWeight      (0),
      i_upBTagHeavyWeight      (0),
      i_ljCHEF                 (0),
      i_whichQCD_HT_Sample     (0),
      i_whichQCD_Sample        (0),
      whichQCD_HT_Sample       (0),
      whichQCD_Sample          (0)
    {}

    virtual ~ZeroLeptonQCDAnalyzer() {delete savedSmearWeights;}

    void processVariables(){
      ZeroLeptonAnalyzer::processVariables();
    }

    void loadVariables() {
      setBranchAddress("","bootstrapWeight",&savedSmearWeights,false);
//      load(cfgSet::EVTINFO);
//      load(cfgSet::AK4JETS,JetReader::LOADRECO | JetReader::LOADGEN | JetReader::FILLOBJ);
//      load(cfgSet::CMSTOPS);
      ZeroLeptonAnalyzer::loadVariables();
    }

    void book() {
      ZeroLeptonAnalyzer::book();
      i_bs                    = data.addMulti<ucsbsusy::size8>("", "bootstrapWeight"   , 0);
      i_trueResp              = data.add<float>       ("", "trueResp"             , "F", 0);
      i_trueRespGenInd        = data.add<unsigned int>("", "trueRespGenInd"       , "i", 0);
      i_trueRespRecoInd       = data.add<unsigned int>("", "trueRespRecoInd"      , "i", 0);
      i_trueRespFlv           = data.add<unsigned int>("", "trueRespFlv"          , "i", 0);
      i_trueRespPT            = data.add<float>       ("", "trueRespPT"           , "F", 0);
      i_trueRespGenPT         = data.add<float>       ("", "trueRespGenPT"        , "F", 0);
      i_pseudoResp            = data.add<float>       ("", "pseudoResp"           , "F", 0);
      i_pseudoRespGenInd      = data.add<int>         ("", "pseudoRespGenInd"     , "I", 0);
      i_pseudoRespRecoInd     = data.add<int>         ("", "pseudoRespRecoInd"    , "I", 0);
      i_pseudoRespPT          = data.add<float>       ("", "pseudoRespPT"         , "F", 0);
      i_pseudoRespCSV         = data.add<float>       ("", "pseudoRespCSV"        , "F", 0);
      i_pseudoRespPseudoGenPT = data.add<float>       ("", "pseudoRespPseudoGenPT", "F", 0);
      i_pseudoRespPseudoGenHT = data.add<float>       ("", "pseudoRespPseudoGenHT", "F", 0);
      i_pseudoRespPassFilter  = data.add<bool>        ("", "pseudoRespPassFilter" , "O", 0);
      i_upTailWeight          = data.add<float>       ("", "upTailWeight"         , "F", 0);
      i_downTailWeight        = data.add<float>       ("", "downTailWeight"       , "F", 0);
      i_upBTagLightWeight     = data.add<float>       ("", "upBTagLightWeight"    , "F", 0);
      i_upBTagHeavyWeight     = data.add<float>       ("", "upBTagHeavyWeight"    , "F", 0);
      i_ljCHEF                = data.add<float>       ("", "ljCHEF"               , "F", 2);
      i_whichQCD_HT_Sample    = data.add<int>         ("", "whichQCD_HT_Sample"   , "I", 0);
      i_whichQCD_Sample       = data.add<int>         ("", "whichQCD_Sample"      , "I", 0);
    }

    bool fillEvent() {
      if(!goodvertex) return false;
      if(met->pt() < 200) return false;

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

      data.fill<float>       (i_trueResp       , trueRespGenInd < 0 ? -1 : jetAndMETCorrections.getQCDRespTailCorrector()->mmResp);
      data.fill<unsigned int>(i_trueRespGenInd , trueRespGenInd < 0 ? -1 : trueRespGenInd  + 1);
      data.fill<unsigned int>(i_trueRespRecoInd, trueRespGenInd < 0 ? -1 : trueRespRecoInd + 1);
      data.fill<unsigned int>(i_trueRespFlv    , trueRespFlv);
      data.fill<float>       (i_trueRespPT     , trueRespPT);
      data.fill<float>       (i_trueRespGenPT  , trueRespGenPT);

      //pseudo response info
      int pseudoRespGenInd = -1, jetNearMETInd = -1, MMJetDPhi = -1;
      for(unsigned int iJ = 0; iJ < defaultJets->recoJets.size() && iJ < 4; ++iJ){
        double dPhi = PhysicsUtilities::absDeltaPhi(defaultJets->recoJets[iJ],*met);
        if(iJ < 3){
          if(MMJetDPhi < 0 || dPhi < MMJetDPhi){
            MMJetDPhi     = dPhi;
            jetNearMETInd   = iJ;
          }
        }
      }
      assert(jetNearMETInd >= 0);
      for(unsigned int iG = 0; iG < defaultJets->genJets.size(); ++iG){
        if(defaultJets->recoJets[jetNearMETInd].genJet() == &defaultJets->genJets[iG]){
          pseudoRespGenInd = iG;
          break;
        }
      }
      const auto* pJ   = &defaultJets->recoJets[jetNearMETInd];
      //There was a higher pt, filtered rank!
      bool passFilter   = jets.size() > jetNearMETInd   &&  pJ->index() == jets[jetNearMETInd  ]->index();

      double pseudoGenPT    = (met->p4() + pJ->p4()).pt();
      double MMPseudoResp   = pseudoGenPT   > 0 ? pJ->pt()   / pseudoGenPT   : 999;
      double pseudoGenHT    = 0;
      double maxPT          = -1;
      double maxETA         = -10;
      double leadJetCH      = -1;
      double leadJetPT      = 0;
      for(unsigned int iJ = 0; iJ < defaultJets->recoJets.size(); ++iJ){
        if(jetNearMETInd == iJ){
          pseudoGenHT += pseudoGenPT;
        } else {
          if(!JetKinematics::passCuts(defaultJets->recoJets[iJ], 20.0, 2.4)) continue;
          pseudoGenHT += defaultJets->recoJets[iJ].pt();
        }
      }

      data.fill<float> (i_pseudoResp             , MMPseudoResp);
      data.fill<int>   (i_pseudoRespGenInd       , pseudoRespGenInd >= 0 ? pseudoRespGenInd + 1 : 99);
      data.fill<int>   (i_pseudoRespRecoInd      , jetNearMETInd    >= 0 ? jetNearMETInd    + 1 : 99);
      data.fill<float> (i_pseudoRespPT           , pJ->p4().pt());
      data.fill<float> (i_pseudoRespCSV          , pJ->csv());
      data.fill<float> (i_pseudoRespPseudoGenPT  , pseudoGenPT);
      data.fill<float> (i_pseudoRespPseudoGenHT  , pseudoGenHT);
      data.fill<bool>  (i_pseudoRespPassFilter   , passFilter);

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

      data.fill<float>(i_ljCHEF, defaultJets->jetchHadEnFrac_->size() ? defaultJets->jetchHadEnFrac_->at(defaultJets->recoJets[0].index())  :10);
      data.fill<int>(i_whichQCD_HT_Sample, whichQCD_HT_Sample);
      data.fill<int>(i_whichQCD_Sample,    whichQCD_Sample);

      filler.fillEventInfo(&data, this);
//      filler.fillJetInfo  (&data, jets, bJets, met);

      return true;
    }
};

bool pairCompare_gt(const pair <float, pair <int, int> >& firstElem, const pair <float, pair <int, int> >& secondElem){
  return firstElem.first > secondElem.first;
}

//void makeZeroLeptonQCDTrees(TString sname = "met",
////void makeZeroLeptonQCDTrees(TString sname = "T2tt_375_355",
//                            const int fileindex = -1,
//                            const bool isMC = true,
//                            const TString fname = "/store/user/vdutta/13TeV/170516/merged/met-2015d-05oct15_2_ntuple_postproc.root",
////                            const TString fname = "/store/user/loukas/13TeV/20160315/T2fbd_375_355_ntuple_wgtxsec.root",
//                            const TString outputdir = "trees",
//                            const TString fileprefix = "root://cmseos:1094/",
////                            const TString fileprefix = "",
//                            const TString json = "/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions15/13TeV/Cert_246908-258159_13TeV_PromptReco_Collisions15_25ns_JSON_v3.txt")
void makeZeroLeptonQCDTrees(TString sname = "QCD",
                            const int fileindex = -1,
                            const bool isMC = true,
                            const TString fname = "/store/user/hqu/13TeV/110616/merged/qcd_ht300to500-ext_1_ntuple_postproc.root",
                            const TString outputdir = "trees",
                            const TString fileprefix = "root://cmsxrootd.fnal.gov/",
//                            const TString fileprefix = "",
                            const TString json = "/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions15/13TeV/Cert_246908-258159_13TeV_PromptReco_Collisions15_25ns_JSON_v3.txt")
{
  printf("Processing file %d of %s sample\n", (fileindex > -1 ? fileindex : 0), sname.Data());

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

  a.whichQCD_HT_Sample = whichQCD_HT_Sample;
  a.whichQCD_Sample    = whichQCD_Sample;
  a.analyze(1000000);
}
