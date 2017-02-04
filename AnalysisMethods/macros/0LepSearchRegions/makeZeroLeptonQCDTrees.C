#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/macros/0LepSearchRegions/ZeroLeptonTreeHelper.hh"
#endif

using namespace ucsbsusy;



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
      i_upBTagLightWeight         (0),
      i_upBTagHeavyWeight         (0),
      i_maxMuPT     (0),
      i_removeMuFrac(0)
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
      i_bs                     = data.addMulti<ucsbsusy::size8>("","bootstrapWeight"               ,0);
      i_trueResp               = data.add<float>("","trueResp","F",0);
      i_trueRespFlv            = data.add<unsigned int>("","trueRespFlv","i",0);
      i_trueRespGenPT          = data.add<float>("","trueRespGenPT","F",0);
      i_pseudoResp             = data.add<float>("","pseudoResp","F",0);
      i_pseudoRespCSV          = data.add<float>("","pseudoRespCSV","F",0);
      i_pseudoRespPseudoGenPT  = data.add<float>("","pseudoRespPseudoGenPT","F",0);
      i_pseudoRespPassFilter   = data.add<bool>("","pseudoRespPassFilter","O",0);
      i_upTailWeight           = data.add<float>("","upTailWeight","F",0);
      i_downTailWeight         = data.add<float>("","downTailWeight","F",0);
      i_upBTagLightWeight      = data.add<float>("","upBTagLightWeight","F",0);
      i_upBTagHeavyWeight      = data.add<float>("","upBTagHeavyWeight","F",0);
      i_maxMuPT                = data.add<float>("","maxMuPT","F",0);
      i_removeMuFrac           = data.add<float>("","removeMuFrac","F",0);


    }

    bool fillEvent() {

      if(!goodvertex) return false;

      if(met->pt() < metcut_  ) return false;
      if(nJets < minnjets_) return false;

      if (applyTightPresel){
        bool passLM = met->pt()/(std::sqrt(JetKinematics::ht(jets)))>10;
        bool passHM = nJets>=5 && nBJets>=1;
        if (!passLM && !passHM) return false;
      }
      processMoreVariables(); // call this before filling, but after all preselections


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

      data.fill<float>        (i_trueResp     ,trueRespInd < 0 ? -1 : jetAndMETCorrections.getQCDRespTailCorrector()->mmResp);
      data.fill<unsigned int> (i_trueRespFlv  ,trueRespFlv);
      data.fill<float>        (i_trueRespGenPT,trueRespGenPT);

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


      data.fill<float> (i_pseudoResp           ,MMPseudoResp);
      data.fill<float> (i_pseudoRespCSV        ,pJ->csv());
      data.fill<float> (i_pseudoRespPseudoGenPT,pseudoGenPT);
      data.fill<bool> (i_pseudoRespPassFilter,passFilter);


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
    size i_upTailWeight   ;
    size i_downTailWeight;
    size i_upBTagLightWeight   ;
    size i_upBTagHeavyWeight   ;

    size i_maxMuPT     ;
    size i_removeMuFrac;


};




void makeZeroLeptonQCDTrees(TString sname = "htmht",
                           const int fileindex = -1,
                           const bool isMC = false,
                           const TString fname = "/store/user/gouskos/13TeV/Spring15/20150813/htmht-2015b-pr_ntuple_postproc.root",
                           const TString outputdir = "trees",
                           const TString fileprefix = "root://eoscms//eos/cms",
                           const TString json="")
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

  TString treeName = "Events";
  ZeroLeptonQCDAnalyzer a(fullname, treeName, outfilename, fileindex+2, isMC, &pars);

  a.analyze(1000000);
}
