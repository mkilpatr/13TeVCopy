#include <vector>
#include <assert.h>
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TH1F.h"
#include "TCanvas.h"
#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisBase/TreeAnalyzer/interface/BaseTreeAnalyzer.h"
#include "selectionVariables.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"

#include "AnalysisTools/Parang/interface/Plotter.h"
#include "AnalysisTools/Parang/interface/Polybook.h"
#include "AnalysisTools/Parang/interface/Panvariate.h"

#include "AnalysisTools/KinematicVariables/interface/Topness.h"
using namespace std;
using namespace ucsbsusy;

class Analyze : public BaseTreeAnalyzer{
public:
  PlotterD*                       plotter;
  Polybook                        eventPlots;
  VariableCalculator              vars;

  ParamatrixMVA* param;


  double getMVA(){
    const Panvariate* mva = param->getOne();
    if(!mva) return -1;

    static const int i_met_pt             = mva->findVariable("met"               );
    static const int i_nJ30               = mva->findVariable("num_j30"           );
    static const int i_dPhiMET3           = mva->findVariable("dphi_j3_met"       );
    static const int i_nMedBTags          = mva->findVariable("num_medium_btags"  );
    static const int i_secLeadQL          = mva->findVariable("q2_likeli"         );
    static const int i_leadLeadQL         = mva->findVariable("q1_likeli"         );
    static const int i_prodQL             = mva->findVariable("quark_likeli"      );
    static const int i_htAlongAway        = mva->findVariable("ht_along_over_away");
    static const int i_rmsJetPT           = mva->findVariable("rms_pt"            );
    static const int i_rmsJetDphiMET      = mva->findVariable("rms_dphi"          );
    static const int i_bInvMass           = mva->findVariable("bb_mass"           );
    static const int i_bTransverseMass    = mva->findVariable("mTb"               );
    static const int i_rmsBEta            = mva->findVariable("deta_b_rms"        );
    static const int i_wInvMass           = mva->findVariable("leading_jj_mass"   );


    mva->setVariable(i_met_pt           ,vars.met_pt         );
    mva->setVariable(i_nJ30             ,vars.nJ30           );
    mva->setVariable(i_dPhiMET3         ,vars.dPhiMET3       );
    mva->setVariable(i_nMedBTags        ,vars.nMedBTags      );
    mva->setVariable(i_secLeadQL        ,vars.secLeadQL      );
    mva->setVariable(i_leadLeadQL       ,vars.leadLeadQL     );
    mva->setVariable(i_prodQL           ,vars.prodQL         );
    mva->setVariable(i_htAlongAway      ,vars.htAlongAway    );
    mva->setVariable(i_rmsJetPT         ,vars.rmsJetPT       );
    mva->setVariable(i_rmsJetDphiMET    ,vars.rmsJetDphiMET  );
    mva->setVariable(i_bInvMass         ,vars.bInvMass       );
    mva->setVariable(i_bTransverseMass  ,vars.bTransverseMass);
    mva->setVariable(i_rmsBEta          ,vars.rmsBEta        );
    mva->setVariable(i_wInvMass         ,vars.wInvMass       );


    return mva->evaluateMethod(0);
  } //


  Analyze(TString fname, string treeName, bool isMCTree, ConfigPars * pars) : BaseTreeAnalyzer(fname,treeName, isMCTree,pars), plotter (new PlotterD(3)), eventPlots(plotter)
  {
    TFile* recoFile = TFile::Open("srmva.root", "READ");
    param = dynamic_cast<ParamatrixMVA*>(recoFile->Get("mva_0"));
    delete recoFile;
  };

  void loadVariables() override{
    load(EVTINFO);
    load(AK4JETS,JetReader::LOADRECO | JetReader::LOADJETSHAPE | JetReader::FILLOBJ);
    load(ELECTRONS);
    load(MUONS);
    load(TAUS);
    setDefaultJets(AK4JETS);
  }
//  void runEvent(){
//    TString prefix =defaults::PROCESS_NAMES[process];
//    prefix += "_";
//
//    vars.processVariables(this,defaultJets,jets,met);
//    if(!vars.passPreselction) return;
//
//    int nMu = 0;
//    int nE = 0;
//
//    for(auto& lep : electronReader.electrons)
//      if(isGoodElectron(lep))
//          nE++;
//
//    for(auto& lep : muonReader.muons)
//      if(isGoodMuon(lep))
//        nMu++;
//
//
//    eventPlots.rewind();
//    eventPlots("incl__",true);
//    eventPlots("nMu__",nMu == 0);
//    eventPlots("nMuE__",nLeptons == 0);
//    eventPlots("nMuETau__",nLeptons == 0 && nTaus == 0);
//    eventPlots("selEMu__" ,nLeptons == 1 && nTaus == 0);
//
//    ++eventPlots;
//    eventPlots("",true);
//    eventPlots("selE__",nE > 0);
//    eventPlots("selMu__",nMu > 0);
//    eventPlots("selTau__",nTaus > 0);
//
//    ++eventPlots;
//    eventPlots("",true);
//    eventPlots("passDPhi__",nLeptons == 1 && PhysicsUtilities::absDeltaPhi(*leptons[0], *met) > 1 );
//
//    eventPlots.fill(1, weight, "lepSF_nEvts"                , ";nEvents"                            , 1,  .5,  1.5, prefix);
//
//
//    eventPlots.fill(vars.met_pt             , weight, "met"                , ";#slash{E}_{T}"                            , 33,  200,  405, prefix);
//    eventPlots.fill(vars.nJ30               , weight, "num_j30"            , ";N. p_{T} #geq 30 jets"                    , 11,  1.5, 12.5, prefix);
//    eventPlots.fill(vars.nJ90               , weight, "num_j90"            , ";N. p_{T} #geq 90 jets"                    , 11,  1.5, 12.5, prefix);
//    eventPlots.fill(vars.nTightBTags        , weight, "num_tight_btags"    , ";N. tight b-tagged p_{T} #geq 30 jets"     , 5 , -0.5,  4.5, prefix);
//    eventPlots.fill(vars.dPhiMET12          , weight, "min_dphi_j12_met"   , ";min |#Delta#phi(#slash{E}_{T},j1-2)|"     , 21,  0  , 3.15, prefix);
//    eventPlots.fill(vars.dPhiMET3           , weight, "dphi_j3_met"        , ";|#Delta#phi(#slash{E}_{T},j3)|"           , 21,  0  , 3.15, prefix);
//    eventPlots.fill(vars.nMedBTags          , weight, "num_medium_btags"   , ";N. medium b-tagged p_{T} #geq 30 jets"     , 5 , -0.5,  4.5,  prefix);
//    eventPlots.fill(vars.secLeadQL          , weight, "q2_likeli"          , ";2nd-max L_{q}"                             , 22, 0   ,    1,  prefix);
//    eventPlots.fill(vars.leadLeadQL         , weight, "q1_likeli"          , ";max L_{q}"                                 , 22, 0   ,    1,  prefix);
//    eventPlots.fill(vars.prodQL             , weight, "quark_likeli"       , ";ln[ #prod L_{q} ]"                         , 25, -12 ,  0  ,  prefix);
//    eventPlots.fill(vars.htAlongAway        , weight, "ht_along_over_away" , ";H_{T}^{along} / H_{T}^{away}"              , 24,  0  ,  1.2,  prefix);
//    eventPlots.fill(vars.rmsJetPT           , weight, "rms_pt"             , ";RMS[ p_{T}(jet) ]"                         , 25,  0  ,  250,  prefix);
//    eventPlots.fill(vars.rmsJetDphiMET      , weight, "rms_dphi"           , ";RMS[ #Delta#phi(jet,#slash{E}_{T}) ]"      , 25,  0  ,  2.5,  prefix);
//    eventPlots.fill(vars.bInvMass           , weight, "bb_mass"            , ";m(b1 + b2)"                                , 24,  0  ,  600,  prefix);
//    eventPlots.fill(vars.bTransverseMass    , weight, "mTb"                , ";m_{T}(nearest b to #slash{E}_{T})"         , 20,  0  ,  400,  prefix);
//    eventPlots.fill(vars.rmsBEta            , weight, "deta_b_rms"         , ";RMS[ #Delta#eta(b,other jets) ]"           , 25,  0  ,  2.5,  prefix);
//    eventPlots.fill(vars.wInvMass           , weight, "leading_jj_mass"    , ";m(max-p_{T} jj)"                           , 24,  0  ,  600,  prefix);
//
//    eventPlots.fill(getMVA()               , weight, "mva"    , ";MVA Disc"                                               , 200,  -1  ,  1,  prefix);
//
//  }

  void runEvent(){
    TString prefix =defaults::PROCESS_NAMES[process];
    prefix += "_";

    if(leptons.size() == 0) return;
    if(nTaus) return;
    if(nJets < 4) return;
    if(nBJets == 0) return;
    if(met->pt() < 150) return;
    if(leptons[0]->pt() < 30) return;

    if(PhysicsUtilities::absDeltaPhi(*leptons[0], *met) <= 1) return;



    int nMu = 0;
    int nE = 0;

    for(auto& lep : electronReader.electrons)
      if(isGoodElectron(lep))
          nE++;

    for(auto& lep : muonReader.muons)
      if(isGoodMuon(lep))
        nMu++;


    eventPlots.rewind();
    eventPlots("incl__",true);
    eventPlots("sel1EMu__" ,nLeptons == 1);
    eventPlots("sel2EMu__" ,nLeptons == 2);

    ++eventPlots;
    eventPlots("nJets_eq4to7__",nJets < 7);
    eventPlots("nJets_geq7__",nJets >= 7);

    ++eventPlots;
    eventPlots("nBJets_eq1__",nBJets ==  1);
    eventPlots("nBJets_eq2__",nBJets ==  2);
    eventPlots("nBJets_geq3__",nBJets >=  2);

    ++eventPlots;
    eventPlots("met_eq150to250__",met->pt() >= 150 && met->pt() < 250);
    eventPlots("met_eq250to350__",met->pt() >= 250 && met->pt() < 350);
    eventPlots("met_geq350__",met->pt() >= 350);

    eventPlots.checkPoint();

    static Topness * tNess     = new Topness();
    double minTopness = tNess->findMinTopnessConfiguration(leptons,jets,met);
    eventPlots.fill(minTopness            , weight, "topness"                , ";Topness"                            , 40,  -20,  20, prefix);

    eventPlots.revert();
    ++eventPlots;
    eventPlots("topness_lt1__",minTopness <  1);
    eventPlots("topness_eq1to7__",minTopness >=  1 && minTopness < 7);
    eventPlots("topness_geq7__",minTopness >=  7);

    eventPlots.fill(1, weight, "nEvts"                , ";nEvents"                            , 1,  .5,  1.5, prefix);
  }

  void out(TString outputPath){
    plotter->write(outputPath);
  }
};




#endif



void produceSRPlots(string fname = "evttree.root", string treeName = "TestAnalyzer/Events", bool isMCTree = true) {

  ConfigPars pars;
  pars.minJetPt = 0;
  pars.maxJetEta = 9999;
  pars.cleanJetsvLeptons_ = true;
  pars.cleanJetsvTaus_ = true;

  Analyze a(fname, treeName, isMCTree,&pars);
  TString name = fname;
  name = ((TObjString*)name.Tokenize(".")->At(0))->GetString();
  a.analyze(100000,-1);
  a.out(TString::Format("%s_plots.root",name.Data()));
}
