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
      i_upTailWeight         (0),
      i_downTailWeight       (0)

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
      i_upTailWeight           = data.add<float>("","upTailWeight","F",0);
      i_downTailWeight         = data.add<float>("","downTailWeight","F",0);

    }

    bool fillEvent() {

      if(!goodvertex) return false;
      if(met->pt() < 200  ) return false;
      if(jets.size() < 2 || jets[1]->pt() < 75) return false;

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
      for(unsigned int iJ = 0; iJ < jets.size() && iJ < 2; ++iJ){
        double dPhi = PhysicsUtilities::absDeltaPhi(*jets[iJ],*met);
        if(MMJetDPhi < 0 || dPhi < MMJetDPhi){
          MMJetDPhi     = dPhi;
          jetNearMETInd = iJ;
        }
      }
      assert(jetNearMETInd >= 0);
      double pseudoGenPT = (met->p4() + jets[jetNearMETInd]->p4()).pt();
      double MMPseudoResp = pseudoGenPT > 0 ? jets[jetNearMETInd]->pt()/ pseudoGenPT : 999;


      data.fill<float> (i_pseudoResp           ,MMPseudoResp);
      data.fill<float> (i_pseudoRespCSV        ,jets[jetNearMETInd]->csv());
      data.fill<float> (i_pseudoRespPseudoGenPT,pseudoGenPT);


      //extra weights for syst. studies
      data.fill<float>(i_upTailWeight    ,jetAndMETCorrections.getQCDRespTailCorrector()->getWeight(UP));
      data.fill<float>(i_downTailWeight  ,jetAndMETCorrections.getQCDRespTailCorrector()->getWeight(DOWN));

      //copy the bootstrapweights
      for(auto i : *savedSmearWeights)
        data.fillMulti<ucsbsusy::size8>(i_bs,i);



      filler.fillEventInfo(&data, this);
      filler.fillJetInfo  (&data, jets, bJets, met);


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
    size i_upTailWeight   ;
    size i_downTailWeight;


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
  pars.corrections.jetResCorrType = NOMINAL;
  pars.corrections.jetAndMETCorrections     = ucsbsusy::JetAndMETCorrectionSet::QCDRESPTAIL | ucsbsusy::JetAndMETCorrectionSet::JETRESOLUTION;

  TString treeName = "Events";
  ZeroLeptonQCDAnalyzer a(fullname, treeName, outfilename, fileindex+2, isMC, &pars);

  a.analyze(1000000,1000);

  //a.analyze(10000,100000);

}
