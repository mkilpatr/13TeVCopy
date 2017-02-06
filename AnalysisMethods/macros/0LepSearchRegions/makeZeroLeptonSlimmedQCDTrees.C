#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/macros/0LepSearchRegions/ZeroLeptonTreeHelper.hh"
#endif

using namespace ucsbsusy;



class ZeroLeptonQCDAnalyzer : public ZeroLeptonAnalyzer {

  public :

  ZeroLeptonQCDAnalyzer(TString fileName, TString treeName, TString outfileName, size randSeed, bool isMCTree, cfgSet::ConfigSet *pars) :
      ZeroLeptonAnalyzer(fileName, treeName, outfileName, randSeed, isMCTree, pars), savedSmearWeights(new std::vector<size8>),
      i_bs(0)
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
    }

    bool fillEvent() {

      // fill inclusive histograms
      extraFiller.fillHistograms(this);

      if(!goodvertex) return false;

      if(met->pt() < metcut_  ) return false;
      if(nJets < minnjets_) return false;

      if (applyTightPresel){
        double dphij1met = 0, dphij2met = 0, dphij3met = 3, dphij4met = 3;
        if(jets.size() > 0) {
          dphij1met = fabs(PhysicsUtilities::deltaPhi(*jets[0], *met));
        }
        if(jets.size() > 1) {
          dphij2met = fabs(PhysicsUtilities::deltaPhi(*jets[1], *met));
        }
        if(jets.size() > 2) {
          dphij3met = fabs(PhysicsUtilities::deltaPhi(*jets[2], *met));
        }
        if(jets.size() > 3){
          dphij4met = fabs(PhysicsUtilities::deltaPhi(*jets[3], *met));
        }

        if (isSR){
          bool passDPhiLM = dphij1met>0.5 && dphij2met>0.15 && dphij3met>0.15;
          bool passDPhiHM = dphij1met>0.5 && dphij2met>0.5 && dphij3met>0.5 && dphij4met>0.5;
          bool passLM = passDPhiLM && met->pt()/(std::sqrt(JetKinematics::ht(jets)))>10 && ak8isrJets.size() && ak8isrJets.front()->pt()>200;
          bool passHM = passDPhiHM && nJets>=5 && nBJets>=1;
          if (!passLM && !passHM) return false;
        }else{
          bool passInvertDPhi = (dphij1met<0.1 || dphij2met<0.1 || dphij3met<0.1);
          if (!passInvertDPhi) return false;

          bool passLM = met->pt()/(std::sqrt(JetKinematics::ht(jets)))>10 && ak8isrJets.size() && ak8isrJets.front()->pt()>200;
          bool passHM = nJets>=5 && nBJets>=1;
          if (!passLM && !passHM) return false;
        }
      }

      processMoreVariables(); // call this before filling, but after all preselections

      //copy the bootstrapweights
      for(auto i : *savedSmearWeights)
        data.fillMulti<ucsbsusy::size8>(i_bs,i);

      filler.fillEventInfo(&data, this);
      extraFiller.fillTestVars(&data, this);

      return true;
    }


    std::vector<size8>*  savedSmearWeights;
    size i_bs                   ;

    bool isSR = true;
};




void makeZeroLeptonSlimmedQCDTrees(TString sname = "htmht",
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
  if (sname.Contains("qcd-cr")) {
    clog << "Running w/ QCD CR selection!" << endl;
    a.isSR = false;
  }else if (sname.Contains("qcd-sr")){
    clog << "Running w/ QCD SR selection!" << endl;
    a.isSR = true;
  }

  a.analyze(1000000);
}
