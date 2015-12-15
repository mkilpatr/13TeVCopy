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
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"

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
  ParamatrixMVA* param2;
  ParamatrixMVA* param3;
  ParamatrixMVA* param4;


  double getMVA(ParamatrixMVA* param){
    const Panvariate* mva = param->getOne();
    if(!mva) return -1;

    static const int i_met_pt             = mva->findVariable("met"               );
    static const int i_nJ20               = mva->findVariable("num_j20"           );
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
    mva->setVariable(i_nJ20             ,vars.nJ20           );
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

  double getMVAExtra(ParamatrixMVA* param){
    const Panvariate* mva = param->getOne();
    if(!mva) return -1;

    static const int i_met_pt             = mva->findVariable("met"               );
    static const int i_nJ20               = mva->findVariable("num_j20"           );
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
//    static const int i_leadBPT            = mva->findVariable("leadBPT"        );
//    static const int i_secLeadBPT         = mva->findVariable("secLeadBPT"   );
    static const int i_ht                 = mva->findVariable("ht"        );



    mva->setVariable(i_met_pt           ,vars.met_pt         );
    mva->setVariable(i_nJ20             ,vars.nJ20           );
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
    mva->setVariable(i_rmsBEta          ,vars.rmsBEta        );
    mva->setVariable(i_wInvMass         ,vars.wInvMass       );
//    mva->setVariable(i_leadBPT          ,vars.leadBPT       );
//    mva->setVariable(i_secLeadBPT       ,vars.secLeadBPT       );
    mva->setVariable(i_ht       ,vars.ht       );


    return mva->evaluateMethod(0);
  } //


  Analyze(TString fname, string treeName, bool isMCTree, ConfigPars * pars) : BaseTreeAnalyzer(fname,treeName, isMCTree,pars), plotter (new PlotterD(3)), eventPlots(plotter)
  {
    if(defaultJets == &pickyJetReader){
      TFile* recoFile = TFile::Open("sr_T2tt_850_100_picky_mva.root");
      param = dynamic_cast<ParamatrixMVA*>(recoFile->Get("mva_0"));
      param2 = dynamic_cast<ParamatrixMVA*>(recoFile->Get("mva_1"));
      delete recoFile;

      TFile* recoFile2 = TFile::Open("sr_T1tttt_1500_100_picky_mva.root");
      param3 = dynamic_cast<ParamatrixMVA*>(recoFile2->Get("mva_T2tttt_0"));
      param4 = dynamic_cast<ParamatrixMVA*>(recoFile2->Get("mva_T2tttt_1"));
      delete recoFile2;
    }
//    TFile* recoFile = TFile::Open("srmva.root", "READ");
//    param = dynamic_cast<ParamatrixMVA*>(recoFile->Get("mva_0"));
//    delete recoFile;
  };

  void loadVariables() override{
    load(EVTINFO);
    load(AK4JETS,JetReader::LOADRECO | JetReader::LOADJETSHAPE | JetReader::FILLOBJ);
    load(PICKYJETS,JetReader::LOADRECO | JetReader::LOADJETSHAPE | JetReader::FILLOBJ);
    load(ELECTRONS);
    load(MUONS);
    load(PFCANDS);
    load(GENPARTICLES);
  }

//  void runEvent(){
//    if(nSelLeptons == 0) return ;
//    if(met->pt() < 350) return ;
////    if(nBJets < 3) return ;
////    if(nJets < 7) return ;
//    if(PhysicsUtilities::absDeltaPhi(*selectedLeptons[0], (met->p4() + selectedLeptons[0]->p4())) <= 1) return;
//
//        eventPlots.rewind();
//        bool                          classified    = false;
//
//        eventPlots("MC__"             ,process > defaults::DATA && process < defaults::SIGNAL);
//        eventPlots(""                 ,process == defaults::SIGNAL);
//        eventPlots("t#bar{t}__", !classified && (classified = (process == defaults::TTBAR)));
//        eventPlots("Wlnu__"           , !classified && (classified = ( process == defaults::SINGLE_W)));
//        eventPlots("t#bar{t}Z__"      , !classified && (classified = process == defaults::TTZ));
//        eventPlots("other__"          , !classified);
//
//            static Topness * tNess     = new Topness();
//            double minTopness = tNess->findMinTopnessConfiguration(selectedLeptons,jets,met);
//            ++eventPlots;
//            eventPlots("bJets_eq2__",nBJets == 2);
//            eventPlots("bJets_geq3__",nBJets >= 3);
//            ++eventPlots;
//            eventPlots("nJets_geq7__",nJets >= 7);
//            eventPlots("nJets_eq4to7__",nJets >= 4 && nJets < 7);
//
//         eventPlots.fill(minTopness, weight, "minTopness"                , ";minTopness"         , 20,  -20,  20);
//        ++eventPlots;
//        eventPlots("topness_geq7__",minTopness >= 7);
//
//        eventPlots.fill(1, weight, "nEvts"                , ";nEvents"         , 1,  .5,  1.5);
//
//  }

  void runEvent(){
//    TString prefix =defaults::PROCESS_NAMES[process];
//    prefix += "_";
    TString prefix = "";


    vector<RecoJetF*> ak4Jets;
    cleanJets(&ak4Reader,ak4Jets,0,0);

    vars.processVariables(this,&ak4Reader,ak4Jets,jets,met);

    //Basic preselection
    if(vars.met_pt < 200 || vars.nJ90 < 2) return;
    if(vars.dPhiMET12 < .5 || vars.dPhiMET3 < .3) return;
    if(vars.nTightBTags == 0) return;
    if(vars.nJ20 < 4) return;

    //start with some lepton categories
    eventPlots.rewind();
    bool                          classified    = false;

    eventPlots("MC__"             ,process > defaults::DATA && process < defaults::SIGNAL);
    eventPlots(""                 ,process == defaults::SIGNAL);
    eventPlots("t#bar{t}_plus_W__", !classified && (classified = (process == defaults::TTBAR || process == defaults::SINGLE_W)));
    eventPlots("Znunu__"          , !classified && (classified = process == defaults::SINGLE_Z));
    eventPlots("t#bar{t}Z__"      , !classified && (classified = process == defaults::TTZ));
    eventPlots("t#bar{t}W__"      , !classified && (classified = process == defaults::TTW));
    eventPlots("t__"      , !classified && (classified = process == defaults::SINGLE_T));
    eventPlots("other__"          , !classified);

    ++eventPlots;

    eventPlots("incl__"   ,true);
    eventPlots("nMuE__"   ,nSecondaryLeptons == 0);
    eventPlots("nMuETau__",nSecondaryLeptons == 0 && nVetoedTaus == 0);

    ++eventPlots;
    eventPlots("nJ20_geq4__",true);
    eventPlots("nJ20_geq5__",vars.nJ20 >= 5);
    eventPlots("nJ20_geq6__",vars.nJ20 >= 6);



    double mva1 = getMVA(param);
    double mva2 = getMVA(param3);

    ++eventPlots;
    eventPlots("",true);
    eventPlots("mva1__",mva1 > .97);
    eventPlots("mva3__",mva2 > .96);


    eventPlots.fill(1, weight, "nEvts"                , ";nEvents"         , 1,  .5,  1.5, prefix);


    eventPlots.fill(vars.met_pt             , weight, "met"                , ";#slash{E}_{T}"                            , 33,  200,  405, prefix);
    eventPlots.fill(vars.nJ20               , weight, "num_j20"            , ";N. p_{T} #geq 30 jets"                    , 11,  1.5, 12.5, prefix);
    eventPlots.fill(vars.nJ90               , weight, "num_j90"            , ";N. p_{T} #geq 90 jets"                    , 11,  1.5, 12.5, prefix);
    eventPlots.fill(vars.nTightBTags        , weight, "num_tight_btags"    , ";N. tight b-tagged p_{T} #geq 30 jets"     , 5 , -0.5,  4.5, prefix);
    eventPlots.fill(vars.dPhiMET12          , weight, "min_dphi_j12_met"   , ";min |#Delta#phi(#slash{E}_{T},j1-2)|"     , 21,  0  , 3.15, prefix);
    eventPlots.fill(vars.dPhiMET3           , weight, "dphi_j3_met"        , ";|#Delta#phi(#slash{E}_{T},j3)|"           , 21,  0  , 3.15, prefix);
    eventPlots.fill(vars.nMedBTags          , weight, "num_medium_btags"   , ";N. medium b-tagged p_{T} #geq 30 jets"     , 5 , -0.5,  4.5,  prefix);
    eventPlots.fill(vars.secLeadQL          , weight, "q2_likeli"          , ";2nd-max L_{q}"                             , 22, 0   ,    1,  prefix);
    eventPlots.fill(vars.leadLeadQL         , weight, "q1_likeli"          , ";max L_{q}"                                 , 22, 0   ,    1,  prefix);
    eventPlots.fill(vars.prodQL             , weight, "quark_likeli"       , ";ln[ #prod L_{q} ]"                         , 25, -12 ,  0  ,  prefix);
    eventPlots.fill(vars.htAlongAway        , weight, "ht_along_over_away" , ";H_{T}^{along} / H_{T}^{away}"              , 24,  0  ,  1.2,  prefix);
    eventPlots.fill(vars.rmsJetPT           , weight, "rms_pt"             , ";RMS[ p_{T}(jet) ]"                         , 25,  0  ,  250,  prefix);
    eventPlots.fill(vars.rmsJetDphiMET      , weight, "rms_dphi"           , ";RMS[ #Delta#phi(jet,#slash{E}_{T}) ]"      , 25,  0  ,  2.5,  prefix);
    eventPlots.fill(vars.bInvMass           , weight, "bb_mass"            , ";m(b1 + b2)"                                , 24,  0  ,  600,  prefix);
    eventPlots.fill(vars.bTransverseMass    , weight, "mTb"                , ";m_{T}(nearest b to #slash{E}_{T})"         , 20,  0  ,  400,  prefix);
    eventPlots.fill(vars.rmsBEta            , weight, "deta_b_rms"         , ";RMS[ #Delta#eta(b,other jets) ]"           , 25,  0  ,  2.5,  prefix);
    eventPlots.fill(vars.wInvMass           , weight, "leading_jj_mass"    , ";m(max-p_{T} jj)"                           , 24,  0  ,  600,  prefix);

    eventPlots.fill(vars.ht, weight, "ht"    , ";H_{T}"                           , 24,  0  ,  2000,  prefix);
//    eventPlots.fill(vars.leadBPT, weight, "leading_bPT"    , ";p_{T} bJet 1"                     , 24,  0  ,  600,  prefix);
//    eventPlots.fill(vars.secLeadBPT, weight, "secleading_bPT"    , ";p_{T} bJet 2"                     , 24,  0  ,  600,  prefix);


    eventPlots.fill(mva1               , weight, "mva"    , ";MVA Disc"                                               , 200,  -1  ,  1,  prefix);
//    eventPlots.fill(getMVAExtra(param2)          , weight, "mva2"    , ";MVA Disc 2"                                            , 200,  -1  ,  1,  prefix);
    eventPlots.fill(mva2         , weight, "mva3"    , ";MVA Disc 3"                                            , 200,  -1  ,  1,  prefix);
//    eventPlots.fill(getMVAExtra(param4)          , weight, "mva4"    , ";MVA Disc 4"                                            , 200,  -1  ,  1,  prefix);

  }

//  void runEvent(){
////    TString prefix =defaults::PROCESS_NAMES[process];
////    prefix += "_";
//    TString prefix = "";
//
//
//    vector<RecoJetF*> ak4Jets;
//    cleanJets(&ak4Reader,ak4Jets,0,0);
//
//    vars.processVariables(this,&ak4Reader,ak4Jets,jets,met);
//    if(!vars.passPreselction) return;
//
//
//    //start with some lepton categories
//    eventPlots.rewind();
//
//    eventPlots("ttbar__" , process == defaults::TTBAR);
//    eventPlots("signal__", process == defaults::SIGNAL);
//
//    int nE  = 0;
//    int nM  = 0;
//    int nTH = 0;
//    for(auto* p : genParts){
//      if( TMath::Abs(p->pdgId()) != ParticleInfo::p_Wplus) continue;
//      for(unsigned int iD = 0; iD < p->numberOfDaughters(); ++iD){
//        const auto * d1 = p->daughter(iD);
//        if(TMath::Abs(d1->pdgId()) == ParticleInfo::p_eminus) nE++;
//        if(TMath::Abs(d1->pdgId()) == ParticleInfo::p_muminus) nM++;
//        if(TMath::Abs(d1->pdgId()) == ParticleInfo::p_tauminus){
//          int tE = 0;
//          int tM = 0;
//          for(unsigned int iD2 = 0; iD2 < d1->numberOfDaughters(); ++iD2){
//            const auto * d2 = d1->daughter(iD2);
//            if(TMath::Abs(d2->pdgId()) == ParticleInfo::p_eminus) tE++;
//            if(TMath::Abs(d2->pdgId()) == ParticleInfo::p_muminus) tM++;
//          }
//          if(tM)nM++;
//          else if(tE)nE++;
//          else nTH++;
//        }
//      }
//    }
//
//    ++eventPlots;
//    eventPlots("incl__"      ,true);
//    eventPlots("hadronic__"  ,nE + nM + nTH == 0);
//    eventPlots("diLepton__"  ,nE  + nM + nTH > 1);
//    eventPlots("singleE__"   ,nE  > 0 && (nM + nTH == 0));
//    eventPlots("singleMu__"  ,nM  > 0 && (nE + nTH == 0));
//    eventPlots("singleTauH__"  ,nTH  > 0 && (nE + nM == 0));
//
//    ++eventPlots;
//
//    int nVM = 0;
//    int nVE = 0;
//    for(auto * l : vetoedLeptons){
//      if(l->ismuon())nVM++;
//      else nVE++;
//    }
//
//    eventPlots("incl__"   ,true);
//    eventPlots("nM__"   ,nVM == 0);
//    eventPlots("nE__"   ,nVE == 0);
//    eventPlots("nT__"   ,nVetoedTaus == 0);
//    eventPlots("nEMu__"   ,nVM + nVE == 0);
//    eventPlots("nMuETau__",nVetoedLeptons == 0 && nVetoedTaus == 0);
//    eventPlots.fill(1, weight, "nEvts"                , ";nEvents"         , 1,  .5,  1.5, prefix);
//  }

//  void runEvent(){
//    TString prefix =defaults::PROCESS_NAMES[process];
//    prefix += "_";
//
//    if(leptons.size() == 0) return;
//    if(nTaus) return;
//    if(nJets < 4) return;
//    if(nBJets == 0) return;
//    if(met->pt() < 150) return;
//    if(leptons[0]->pt() < 30) return;
//
//    if(PhysicsUtilities::absDeltaPhi(*leptons[0], *met) <= 1) return;
//
//
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
//    eventPlots("sel1EMu__" ,nLeptons == 1);
//    eventPlots("sel2EMu__" ,nLeptons == 2);
//
//    ++eventPlots;
//    eventPlots("nJets_eq4to7__",nJets < 7);
//    eventPlots("nJets_geq7__",nJets >= 7);
//
//    ++eventPlots;
//    eventPlots("nBJets_eq1__",nBJets ==  1);
//    eventPlots("nBJets_eq2__",nBJets ==  2);
//    eventPlots("nBJets_geq3__",nBJets >=  2);
//
//    ++eventPlots;
//    eventPlots("met_eq150to250__",met->pt() >= 150 && met->pt() < 250);
//    eventPlots("met_eq250to350__",met->pt() >= 250 && met->pt() < 350);
//    eventPlots("met_geq350__",met->pt() >= 350);
//
//    eventPlots.checkPoint();
//
//    static Topness * tNess     = new Topness();
//    double minTopness = tNess->findMinTopnessConfiguration(leptons,jets,met);
//    eventPlots.fill(minTopness            , weight, "topness"                , ";Topness"                            , 40,  -20,  20, prefix);
//
//    eventPlots.revert();
//    ++eventPlots;
//    eventPlots("topness_lt1__",minTopness <  1);
//    eventPlots("topness_eq1to7__",minTopness >=  1 && minTopness < 7);
//    eventPlots("topness_geq7__",minTopness >=  7);
//
//    eventPlots.fill(1, weight, "nEvts"                , ";nEvents"                            , 1,  .5,  1.5, prefix);
//  }
//
  void out(TString outputPath){
    plotter->write(outputPath);
  }
};




#endif



void produceSRPlots(string fname = "evttree.root", string treeName = "Events", bool isMCTree = true) {

  BaseTreeAnalyzer::ConfigPars pars;
  pars.cleanJetsvSelectedLeptons_ = false;
  pars.leptonSelection = BaseTreeAnalyzer::SEL_0_LEP;
  pars.defaultJetCollection = BaseTreeAnalyzer::PICKYJETS;


  Analyze a(fname, treeName, isMCTree,&pars);
  TString name = fname;
  name = ((TObjString*)name.Tokenize(".")->At(0))->GetString();
  a.analyze(100000,-1);
  a.out(TString::Format("%s_plots.root",name.Data()));
}
