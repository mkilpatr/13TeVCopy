#include <vector>
#include <assert.h>
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TH1F.h"
#include "TCanvas.h"
#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisBase/TreeAnalyzer/interface/BaseTreeAnalyzer.h"
#include "cncVariables.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"
#include "AnalysisTools/Parang/interface/Plotter.h"
#include "AnalysisTools/Parang/interface/Polybook.h"
//#include "AnalysisTools/Parang/interface/Panvariate.h"
//#include "AnalysisTools/KinematicVariables/interface/Topness.h"
using namespace std;
using namespace ucsbsusy;

class Analyze : public BaseTreeAnalyzer{
public:
  PlotterD*                       plotter;
  Polybook                        eventPlots;
  VariableCalculator              vars;


  Analyze(TString fname, string treeName, bool isMCTree, ConfigPars * pars)
  	  : BaseTreeAnalyzer(fname, treeName, isMCTree, pars)
      , plotter (new PlotterD(3))
      , eventPlots(plotter)
      {};

  void loadVariables() override{
    load(EVTINFO);
    load(AK4JETS,  JetReader::LOADRECO | JetReader::LOADJETSHAPE | JetReader::FILLOBJ);
    load(PICKYJETS,JetReader::LOADRECO | JetReader::LOADJETSHAPE | JetReader::FILLOBJ);
    load(ELECTRONS);
    load(MUONS);
    load(PFCANDS);
    load(GENPARTICLES);
  } // loadVariables()

  void runEvent(){
    //TString prefix =defaults::PROCESS_NAMES[process];
    //prefix += "_";
    //TString prefix = "";

    vector<RecoJetF*> ak4Jets;
    cleanJets(&ak4Reader,ak4Jets,0,0);

    vars.processVariables(this,&ak4Reader,ak4Jets,jets,met);

    // ===== preselection =====
    bool passPreselection = true;
    if(vars.ptMet < 200 || vars.nJ90 < 2) return; // trigger preselection, so auto reject events that don't pass
    if(vars.dPhiMET12 < .5 || vars.dPhiMET3 < .3) passPreselection = false;
    if(vars.ntBtag == 0) passPreselection = false;
    if(vars.nJ20 < 3)    passPreselection = false;

    eventPlots.rewind();

    // ===== set up bins =====

    // preselection
    eventPlots("all__", true);
    eventPlots("psel_", passPreselection);
    ++eventPlots;

    // b-tags
    eventPlots("g0b__", true);
    eventPlots("e1bt_", vars.ntBtag == 1);
    eventPlots("g2bt_", vars.ntBtag >= 2);
    eventPlots("e1bm_", vars.nmBtag == 1);
    eventPlots("g2bm_", vars.nmBtag >= 2);
    ++eventPlots;

    // met
    eventPlots("met200i_", vars.ptMet > 200);
    eventPlots("met300i_", vars.ptMet > 300);
    eventPlots("met400i_", vars.ptMet > 400);
    eventPlots("met500i_", vars.ptMet > 500);
    eventPlots("met200e_", vars.ptMet > 200 and vars.ptMet < 300);
    eventPlots("met300e_", vars.ptMet > 300 and vars.ptMet < 400);
    eventPlots("met400e_", vars.ptMet > 400 and vars.ptMet < 500);
    ++eventPlots;

    // jet bins
    eventPlots("nJ20ge3_",vars.nJ20 >= 3);
    eventPlots("nJ20ge4_",vars.nJ20 >= 4);
    eventPlots("nJ20ge5_",vars.nJ20 >= 5);
    eventPlots("nJ20ge6_",vars.nJ20 >= 6);
    eventPlots("nJ20ge7_",vars.nJ20 >= 7);

    // ===== filling plots =====

    //eventPlots.fill(1                , weight, "nEvts"         , ";nEvents"                             ,   1,   0.5,    1.5 , prefix);
    eventPlots.fill(1                  , weight, "nEvts"         , ";nEvents"                             ,   1,   0.5,    1.5  );

    eventPlots.fill(vars.ptMet         , weight, "met"           , ";#slash{E}_{T}"                       ,  60, 200  ,  800    ); // 10
    eventPlots.fill(vars.nJ90          , weight, "nj90"          , ";N jets p_{T} #geq 90"                ,  14,   1.5,   15.5  ); //  1
    eventPlots.fill(vars.nJ20          , weight, "nj20"          , ";N jets p_{T} #geq 30"                ,  19,   1.5,   20.5  ); //  1
    eventPlots.fill(vars.ntBtag        , weight, "ntBtag"        , ";N tight b p_{T} #geq 30"             ,   5,  -0.5,    4.5  ); //  1
    eventPlots.fill(vars.nmBtag        , weight, "nmBtag"        , ";N med b p_{T} #geq 30"               ,   8,  -0.5,    7.5  ); //  1
    eventPlots.fill(vars.dPhiMET12     , weight, "dPhiMET12"     , ";min|#Delta#phi(#slash{E}_{T},j1-2)|" ,  21,   0  ,    3.15 ); //  0.15
    eventPlots.fill(vars.dPhiMET3      , weight, "dPhiMET3"      , ";|#Delta#phi(#slash{E}_{T},j3)|"      ,  21,   0  ,    3.15 ); //  0.15
    eventPlots.fill(vars.qgl0          , weight, "qgl0"          , ";QGL_{0}"                             ,  50,   0  ,    1    ); //  0.02   (was 20 bins)
    eventPlots.fill(vars.qgl1          , weight, "qgl1"          , ";QGL_{1}"                             ,  50,   0  ,    1    ); //  0.02   (was 20 bins)
    eventPlots.fill(vars.qglprod       , weight, "qglprod"       , ";ln[#prod QGL_{q}]"                   ,  25, -12  ,    0    ); // 12
    eventPlots.fill(vars.ht            , weight, "ht"            , ";H_{T}"                               , 120,   0  , 3000    ); // 25
    eventPlots.fill(vars.htAlongAway   , weight, "htAlongAway"   , ";H_{T}^{along}/H_{T}^{away}"          ,  40,   0  ,    2    ); //  0.05
    eventPlots.fill(vars.rmsJetPT      , weight, "rmsJetPT"      , ";RMS[p_{T}(jet)]"                     ,  40,   0  ,  400    ); // 10
    eventPlots.fill(vars.rmsJetDphiMET , weight, "rmsJetDphiMET" , ";RMS[#Delta#phi(jet,#slash{E}_{T})]"  ,  25,   0  ,    2.5  ); //  0.1
    eventPlots.fill(vars.bInvMass      , weight, "bInvMass"      , ";m(b1+b2)"                            ,  60,   0  ,  600    ); // 10     (was 25 bins)
    eventPlots.fill(vars.bTransMass    , weight, "bTransMass"    , ";m_{T}(nearest b to #slash{E}_{T})"   ,  80,   0  ,  800    ); // 10
    eventPlots.fill(vars.rmsBEta       , weight, "rmsBEta"       , ";RMS[#Delta#eta(b,jets)]"             ,  25,   0  ,    2.5  ); //  0.1
    eventPlots.fill(vars.wInvMass      , weight, "wInvMass"      , ";m(max p_{T} jj)"                     ,  40,   0  ,  800    ); // 20
	eventPlots.fill(vars.Bpt0          , weight, "Bpt0"          , ";p_{t}(b0)"                           ,  40,   0  ,  800    ); // 20
	eventPlots.fill(vars.Bpt1          , weight, "Bpt1"          , ";p_{t}(b1)"                           ,  60,   0  ,  600    ); // 10

  } // runEvents()

  void out(TString outputPath){
    plotter->write(outputPath);
  }

}; // Analyze

#endif

void cncPlots(string outFileTag, string inFileName, string jetType, string treeName = "Events", bool isMCTree = true) {

  BaseTreeAnalyzer::ConfigPars pars;
  pars.cleanJetsvSelectedLeptons_ = false;
  if (jetType=="ak4")   pars.defaultJetCollection = BaseTreeAnalyzer::AK4JETS;
  if (jetType=="picky") pars.defaultJetCollection = BaseTreeAnalyzer::PICKYJETS;

  string outFileName = "plots_";
  if (jetType=="ak4")   outFileName += "ak4_";
  if (jetType=="picky") outFileName += "pck_";
  outFileName += outFileTag + ".root";

  Analyze a(inFileName, treeName, isMCTree, &pars);
  a.analyze(100000,-1);
  a.out(outFileName);
}
