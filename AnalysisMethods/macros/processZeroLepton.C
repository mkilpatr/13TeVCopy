#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TSystem.h>
#include <TFile.h>
#include <TH1F.h>
//#include "AnalysisMethods/PlotUtils/interface/Sample.hh"
//#include "AnalysisMethods/PlotUtils/interface/PlotTools.hh"
//#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
#include "AnalysisTools/KinematicVariables/interface/JetKinematics.h"
#include "AnalysisBase/TreeAnalyzer/interface/BaseTreeAnalyzer.h"
//#include "AnalysisTools/KinematicVariables/interface/Topness.h"
//#include "AnalysisTools/TreeReader/interface/ElectronReader.h"
#include "AnalysisMethods/macros/0LepSearchRegions/cncVariables.h"


using namespace ucsbsusy;

class Analyzer : public BaseTreeAnalyzer {

  public :
  VariableCalculator vars;

  Analyzer(TString fileName, TString treeName, bool isMCTree, ConfigPars * pars, double xSec, TString sample, TString outputdir)
          : BaseTreeAnalyzer(fileName, treeName, isMCTree, pars)
          , xsec_(xSec)
          , sample_(sample)
          , outputdir_(outputdir)
  {
    loadPlots(); // initialize plots

    // initiliaze tree
    gSystem->mkdir(outputdir,true);
    fout = new TFile (outputdir+"/"+sample+"_tree.root","RECREATE");
    fout->cd();
    outtree = new TTree("events","analysis tree");
    outtree->Branch( "scaleFactor", &scaleFactor, "scaleFactor/F" );
    outtree->Branch( "npv"        , &npv        ,         "npv/I" );
    outtree->Branch( "nAK4pfJets" , &nAK4pfJets ,  "nAK4pfJets/I" );
    outtree->Branch( "nVetoTau"   , &nVetoTau   ,    "nVetoTau/I" );
    //outtree->Branch( "nAK4pfBJets_0", &nAK4pfBJets_0, "nAK4pfBJets_0/I" );
    //outtree->Branch( "met_0"        , &met_0        ,         "met_0/F" );
    //outtree->Branch( "ht_0"         , &ht_0         ,          "ht_0/F" );
    //outtree->Branch( "htAoA_0"      , &htAoA_0      ,       "htAoA_0/F" );

    outtree->Branch( "ptMet"            , &vars.ptMet            ,            "ptMet/F" );
    outtree->Branch( "npv"              , &vars.npv              ,              "npv/I" );
    outtree->Branch( "nj60"             , &vars.nj60             ,             "nj60/I" );
    outtree->Branch( "nJ20"             , &vars.nJ20             ,             "nJ20/I" );
    outtree->Branch( "ntBtag"           , &vars.ntBtag           ,           "ntBtag/I" );
    outtree->Branch( "nmBtag"           , &vars.nmBtag           ,           "nmBtag/I" );
    outtree->Branch( "dPhiMET12"        , &vars.dPhiMET12        ,        "dPhiMET12/F" );
    outtree->Branch( "dPhiMET3"         , &vars.dPhiMET3         ,         "dPhiMET3/F" );
    outtree->Branch( "dPhiB0MET"        , &vars.dPhiB0MET        ,        "dPhiB0MET/F" );
    outtree->Branch( "dPhiB1MET"        , &vars.dPhiB1MET        ,        "dPhiB1MET/F" );
    outtree->Branch( "dPhiB2MET"        , &vars.dPhiB2MET        ,        "dPhiB2MET/F" );
    outtree->Branch( "dPhiB01MET"       , &vars.dPhiB01MET       ,       "dPhiB01MET/F" );
    outtree->Branch( "dPhiCVSnearMET"   , &vars.dPhiCVSnearMET   ,   "dPhiCVSnearMET/F" );
    outtree->Branch( "Mb1b2n"           , &vars.Mb1b2n           ,           "Mb1b2n/F" );
    outtree->Branch( "mtB0MET"          , &vars.mtB0MET          ,          "mtB0MET/F" );
    outtree->Branch( "mtB1MET"          , &vars.mtB1MET          ,          "mtB1MET/F" );
    outtree->Branch( "mtB2MET"          , &vars.mtB2MET          ,          "mtB1MET/F" );
    outtree->Branch( "mtB01MET"         , &vars.mtB01MET         ,         "mtB01MET/F" );
    outtree->Branch( "sSumB01oMET"      , &vars.sSumB01oMET      ,      "sSumB01oMET/F" );
    outtree->Branch( "vSumB01oMET"      , &vars.vSumB01oMET      ,      "vSumB01oMET/F" );
    outtree->Branch( "qgl0"             , &vars.qgl0             ,             "qgl0/F" );
    outtree->Branch( "qgl1"             , &vars.qgl1             ,             "qgl1/F" );
    outtree->Branch( "qglprod"          , &vars.qglprod          ,          "qglprod/F" );
    outtree->Branch( "qglprodNorm"      , &vars.qglprodNorm      ,      "qglprodNorm/F" );
    outtree->Branch( "qglJ0"            , &vars.qglJ0            ,            "qglJ0/F" );
    outtree->Branch( "qglJ1"            , &vars.qglJ1            ,            "qglJ1/F" );
    outtree->Branch( "qglCVS0"          , &vars.qglCVS0          ,          "qglCVS0/F" );
    outtree->Branch( "qglCVS1"          , &vars.qglCVS1          ,          "qglCVS1/F" );
    outtree->Branch( "qgl0noB"          , &vars.qgl0noB          ,          "qgl0noB/F" );
    outtree->Branch( "qgl1noB"          , &vars.qgl1noB          ,          "qgl1noB/F" );
    outtree->Branch( "qglprodnoB"       , &vars.qglprodnoB       ,       "qglprodnoB/F" );
    outtree->Branch( "qglprodNormnoB"   , &vars.qglprodNormnoB   ,   "qglprodNormnoB/F" );
    outtree->Branch( "qglJ0noB"         , &vars.qglJ0noB         ,         "qglJ0noB/F" );
    outtree->Branch( "qglJ1noB"         , &vars.qglJ1noB         ,         "qglJ1noB/F" );
    outtree->Branch( "ht"               , &vars.ht               ,               "ht/F" );
    outtree->Branch( "htAlongAway20"    , &vars.htAlongAway20    ,    "htAlongAway20/F" );
    outtree->Branch( "htAlongAway40"    , &vars.htAlongAway40    ,    "htAlongAway40/F" );
    outtree->Branch( "maxMj12"          , &vars.maxMj12          ,          "maxMj12/F" );
    outtree->Branch( "rmsJetPT"         , &vars.rmsJetPT         ,         "rmsJetPT/F" );
    outtree->Branch( "rmsJetDphiMET"    , &vars.rmsJetDphiMET    ,    "rmsJetDphiMET/F" );
    outtree->Branch( "bInvMass"         , &vars.bInvMass         ,         "bInvMass/F" );
    outtree->Branch( "bTransMass"       , &vars.bTransMass       ,       "bTransMass/F" );
    outtree->Branch( "rmsBEta"          , &vars.rmsBEta          ,          "rmsBEta/F" );
    outtree->Branch( "wInvMass"         , &vars.wInvMass         ,         "wInvMass/F" );
    outtree->Branch( "Bpt0"             , &vars.Bpt0             ,             "Bpt0/F" );
    outtree->Branch( "Bpt1"             , &vars.Bpt1             ,             "Bpt1/F" );
    outtree->Branch( "htnoB"            , &vars.htnoB            ,            "htnoB/F" );
    outtree->Branch( "htJ12"            , &vars.htJ12            ,            "htJ12/F" );
    outtree->Branch( "htJ12noB"         , &vars.htJ12noB         ,         "htJ12noB/F" );
    outtree->Branch( "metOsqrtHt"       , &vars.metOsqrtHt       ,       "metOsqrtHt/F" );
    outtree->Branch( "metOsqrtHtnoB"    , &vars.metOsqrtHtnoB    ,    "metOsqrtHtnoB/F" );
    outtree->Branch( "metOsqrtHtJ12"    , &vars.metOsqrtHtJ12    ,    "metOsqrtHtJ12/F" );
    outtree->Branch( "metOsqrtHtJ12noB" , &vars.metOsqrtHtJ12noB , "metOsqrtHtJ12noB/F" );
    outtree->Branch( "dPhivHtMET"       , &vars.dPhivHtMET       ,       "dPhivHtMET/F" );
    outtree->Branch( "dPhivHtMETnoB"    , &vars.dPhivHtMETnoB    ,    "dPhivHtMETnoB/F" );
    outtree->Branch( "dotHtAlongAway"   , &vars.dotHtAlongAway   ,   "dotHtAlongAway/F" );
    outtree->Branch( "dotHtAlongAwayNoB", &vars.dotHtAlongAwayNoB,"dotHtAlongAwayNoB/F" );

  } // Analyzer()

  virtual ~Analyzer() {
    fout->cd();
    outtree->Write();
    fout->Close();
  } // ~Analyzer()

  const double  metcut_    = 200.0 ;
  const int     minNJets_  =   5   ;
  const int     minNBjets_ =   1   ;
  const int     maxNTaus_  =   0   ;

  const double  lumi_      = 4000.0 ; // in /pb
  const double  xsec_      ;
  const TString sample_    ;
  const TString outputdir_ ;

  TFile *fout    ;
  TTree *outtree ;

  float scaleFactor ;
  int   npv         ;
  int   nAK4pfJets  ;
  int   nVetoTau    ;
  //int   nAK4pfBJets_0 ;
  //float met_0         ;
  //float ht_0          ;
  //float htAoA_0       ;
  int test;


  void loadVariables();
  void runEvent();
  void loadPlots();
  void out(TString outputName, TString outputPath);
  
  map<TString,TH1F*> plots;

}; // Analyzer : BaseTreeAnalyzer

// Define plots, names, and binning
void Analyzer::loadPlots() {
  string passJet = "N_{jets} >= 4, N_{bjets} >=1";
  string passMet = "MET > 200";
  string passPre = "MET > 200, N_{jets} >= 3, N_{bjets} >=1";
  string yTitle  = "Events";

  // partial presel plots
  plots["met_passJet" ] = new TH1F( "met_passJet" , (passJet+"; #slash{E}_{T} [GeV]; "+yTitle).c_str(), 100,  0  , 1000   );
  plots["nj60_passMet"] = new TH1F( "nj60_passMet", (passMet+"; Number of Jets; "     +yTitle).c_str(),  10, -0.5,   19.5 );
  plots["nJ20_passMet"] = new TH1F( "nJ20_passMet", (passMet+"; Number of B Jets; "   +yTitle).c_str(),  10, -0.5,   19.5 );
  // preselection plots
  plots["j1pt_passPre"]              = new TH1F("jet1pt_passPre"           , (passPre+"; Lead jet p_{T} [GeV]; "                  +yTitle).c_str(), 200,   0  , 1000    );
  plots["ntBtag_passPre"]            = new TH1F("ntBtag_passPre"           , (passPre+"; Number of B Jets; "                      +yTitle).c_str(),   8,  -0.5,    7.5  );
  plots["nmBtag_passPre"]            = new TH1F("nmBtag_passPre"           , (passPre+"; Number of B Jets; "                      +yTitle).c_str(),   8,  -0.5,    7.5  );
  plots["dPhiMET12_passPre"]         = new TH1F("dPhiMET12_passPre"        , (passPre+"; |#Delta#phi(#slash{E}_{T},j1-2)|; "      +yTitle).c_str(),  21,   0  ,    3.15 ); //  0.15
  plots["dPhiMET3_passPre"]          = new TH1F("dPhiMET3_passPre"         , (passPre+"; |#Delta#phi(#slash{E}_{T},j3)|; "        +yTitle).c_str(),  21,   0  ,    3.15 ); //  0.15
  plots["dPhiB0MET_passPre"]         = new TH1F("dPhiB0MET_passPre"        , (passPre+"; |#Delta#phi(#slash{E}_{T},B0)|; "        +yTitle).c_str(),  21,   0  ,    3.15 ); //  0.15
  plots["dPhiB1MET_passPre"]         = new TH1F("dPhiB1MET_passPre"        , (passPre+"; |#Delta#phi(#slash{E}_{T},B1)|; "        +yTitle).c_str(),  21,   0  ,    3.15 ); //  0.15
  plots["dPhiB2MET_passPre"]         = new TH1F("dPhiB2MET_passPre"        , (passPre+"; |#Delta#phi(#slash{E}_{T},B2)|; "        +yTitle).c_str(),  21,   0  ,    3.15 ); //  0.15
  plots["dPhiB01MET_passPre"]        = new TH1F("dPhiB01MET_passPre"       , (passPre+"; |#Delta#phi(#slash{E}_{T},B0-1)|; "      +yTitle).c_str(),  21,   0  ,    3.15 ); //  0.15
  plots["dPhiCVSnearMET_passPre"]    = new TH1F("dPhiCVSnearMET_passPre"   , (passPre+"; |#Delta#phi(#slash{E}_{T},Bner)|; "      +yTitle).c_str(),  21,   0  ,    3.15 ); //  0.15
  plots["Mb1b2n_passPre"]            = new TH1F("Mb1b2n_passPre"           , (passPre+"; m(b1+b2) (by CVS); "                     +yTitle).c_str(),  60,   0  ,  600    ); // 10
  plots["mtB0MET_passPre"]           = new TH1F("mtB0MET_passPre"          , (passPre+"; m_{T}(b0,#slash{E}_{T}); "               +yTitle).c_str(),  60,   0  ,  600    ); // 10
  plots["mtB1MET_passPre"]           = new TH1F("mtB1MET_passPre"          , (passPre+"; m_{T}(b1,#slash{E}_{T}); "               +yTitle).c_str(),  60,   0  ,  600    ); // 10
  plots["mtB2MET_passPre"]           = new TH1F("mtB2MET_passPre"          , (passPre+"; m_{T}(b2,#slash{E}_{T}); "               +yTitle).c_str(),  60,   0  ,  600    ); // 10
  plots["mtB01MET_passPre"]          = new TH1F("mtB01MET_passPre"         , (passPre+"; m_{T}(b0-1,#slash{E}_{T}); "             +yTitle).c_str(),  60,   0  ,  600    ); // 10
  plots["sSumB01oMET_passPre"]       = new TH1F("sSumB01oMET_passPre"      , (passPre+"; sSum(B0,B1)/#slash{E}_{T}; "             +yTitle).c_str(), 100,   0  ,    5    );
  plots["vSumB01oMET_passPre"]       = new TH1F("vSumB01oMET_passPre"      , (passPre+"; vSum(B0,B1)/#slash{E}_{T}; "             +yTitle).c_str(), 100,   0  ,    5    );
  plots["qgl0_passPre"]              = new TH1F("qgl0_passPre"             , (passPre+"; QGL_{0}; "                               +yTitle).c_str(),  50, -12  ,    0    ); //  0.02
  plots["qgl1_passPre"]              = new TH1F("qgl1_passPre"             , (passPre+"; QGL_{1}; "                               +yTitle).c_str(),  50, -12  ,    0    ); //  0.02
  plots["qglprod_passPre"]           = new TH1F("qglprod_passPre"          , (passPre+"; #prod QGL_{q}; "                         +yTitle).c_str(),  25,   0  ,    1    ); // 25, -12, 0
  plots["qglprodNorm_passPre"]       = new TH1F("qglprodNorm_passPre"      , (passPre+"; norm  #prod QGL_{q}; "                   +yTitle).c_str(),  25,   0  ,    1    );
  plots["qglJ0_passPre"]             = new TH1F("qglJ0_passPre"            , (passPre+"; QGL J_{0}; "                             +yTitle).c_str(),  25,   0  ,    1    );
  plots["qglJ1_passPre"]             = new TH1F("qglJ1_passPre"            , (passPre+"; QGL J_{1}; "                             +yTitle).c_str(),  25,   0  ,    1    );
  plots["qglCVS0_passPre"]           = new TH1F("qglCVS0_passPre"          , (passPre+"; QGL CVS_{0}; "                           +yTitle).c_str(),  25,   0  ,    1    );
  plots["qglCVS1_passPre"]           = new TH1F("qglCVS1_passPre"          , (passPre+"; QGL CVS_{1}; "                           +yTitle).c_str(),  25,   0  ,    1    );
  plots["qgl0noB_passPre"]           = new TH1F("qgl0noB_passPre"          , (passPre+"; QGL_{0} (noBs); "                        +yTitle).c_str(),  25, -12  ,    0    );
  plots["qgl1noB_passPre"]           = new TH1F("qgl1noB_passPre"          , (passPre+"; QGL_{1} (noBs); "                        +yTitle).c_str(),  25, -12  ,    0    );
  plots["qglprodnoB_passPre"]        = new TH1F("qglprodnoB_passPre"       , (passPre+"; #prod QGL_{q} (noBs); "                  +yTitle).c_str(),  25,   0  ,    1    );
  plots["qglprodNormnoB_passPre"]    = new TH1F("qglprodNormnoB_passPre"   , (passPre+"; norm  #prod QGL_{q} (noBs); "            +yTitle).c_str(),  25,   0  ,    1    );
  plots["qglJ0noB_passPre"]          = new TH1F("qglJ0noB_passPre"         , (passPre+"; QGL J_{0} (noBs); "                      +yTitle).c_str(),  25,   0  ,    1    );
  plots["qglJ1noB_passPre"]          = new TH1F("qglJ1noB_passPre"         , (passPre+"; QGL J_{1} (noBs); "                      +yTitle).c_str(),  25,   0  ,    1    );
  plots["ht_passPre"]                = new TH1F("ht_passPre"               , (passPre+"; H_{T} [GeV]; "                           +yTitle).c_str(),  80,   0  , 2000    );
  plots["htAlongAway20_passPre"]     = new TH1F("htAlongAway20_passPre"    , (passPre+"; H_{T}^{along}/H_{T}^{away} (pt20); "     +yTitle).c_str(),  40,   0  ,    2    ); //  0.05
  plots["htAlongAway40_passPre"]     = new TH1F("htAlongAway40_passPre"    , (passPre+"; H_{T}^{along}/H_{T}^{away} (pt40); "     +yTitle).c_str(),  40,   0  ,    2    ); //  0.05
  plots["maxMj12_passPre"]           = new TH1F("maxMj12_passPre"          , (passPre+"; highest pt jet pair; "                   +yTitle).c_str(),  60,   0  ,  600    ); // 10
  plots["rmsJetPT_passPre"]          = new TH1F("rmsJetPT_passPre"         , (passPre+"; rmsJetPT; "                              +yTitle).c_str(),  40,   0  ,  400    ); // 10
  plots["rmsJetDphiMET_passPre"]     = new TH1F("rmsJetDphiMET_passPre"    , (passPre+"; RMS[#Delta#phi(J,#slash{E}_{T})]; "      +yTitle).c_str(),  25,   0  ,    2.5  ); //  0.1
  plots["bInvMass_passPre"]          = new TH1F("bInvMass_passPre"         , (passPre+"; m(b1+b2); "                              +yTitle).c_str(),  60,   0  ,  600    ); // 10
  plots["bTransMass_passPre"]        = new TH1F("bTransMass_passPre"       , (passPre+"; m_{T}(near b to #slash{E}_{T}); "        +yTitle).c_str(),  80,   0  ,  800    ); // 10
  plots["rmsBEta_passPre"]           = new TH1F("rmsBEta_passPre"          , (passPre+"; RMS[#Delta#eta(b,jets)]; "               +yTitle).c_str(),  25,   0  ,    2.5  ); //  0.1
  plots["wInvMass_passPre"]          = new TH1F("wInvMass_passPre"         , (passPre+"; m(max p_{T} jj); "                       +yTitle).c_str(),  40,   0  ,  800    ); // 20
  plots["Bpt0_passPre"]              = new TH1F("Bpt0_passPre"             , (passPre+"; p_{t}(b0); "                             +yTitle).c_str(),  40,   0  ,  800    ); // 20
  plots["Bpt1_passPre"]              = new TH1F("Bpt1_passPre"             , (passPre+"; p_{t}(b1); "                             +yTitle).c_str(),  60,   0  ,  600    ); // 10
  // new stuff 2
  plots["htnoB_passPre"]             = new TH1F("htnoB_passPre"            , (passPre+"; H_{T} [GeV] (noBs); "                    +yTitle).c_str(),  80,   0  , 2000    ); // 10
  plots["htJ12_passPre"]             = new TH1F("htJ12_passPre"            , (passPre+"; p_{t}(j1)+p_{t}(j2); "                   +yTitle).c_str(),  60,   0  ,  600    ); // 10
  plots["htJ12noB_passPre"]          = new TH1F("htJ12noB_passPre"         , (passPre+"; p_{t}(j1)+p_{t}(j2) (noBs); "            +yTitle).c_str(),  60,   0  ,  600    ); // 10
  plots["metOsqrtHt_passPre"]        = new TH1F("metOsqrtHt_passPre"       , (passPre+"; #slash{E}_{T}/sqrt(H_{T}); "             +yTitle).c_str(),  50,   0  ,  100    ); // 10
  plots["metOsqrtHtnoB_passPre"]     = new TH1F("metOsqrtHtnoB_passPre"    , (passPre+"; #slash{E}_{T}/sqrt(H_{T}) (noBs); "      +yTitle).c_str(),  50,   0  ,  100    ); // 10
  plots["metOsqrtHtJ12_passPre"]     = new TH1F("metOsqrtHtJ12_passPre"    , (passPre+"; #slash{E}_{T}/sqrt(H_{T12}); "           +yTitle).c_str(),  50,   0  ,  100    ); // 10
  plots["metOsqrtHtJ12noB_passPre"]  = new TH1F("metOsqrtHtJ12noB_passPre" , (passPre+"; #slash{E}_{T}/sqrt(H_{T12}) (noBs); "    +yTitle).c_str(),  50,   0  ,   10    ); // 10
  plots["dPhivHtMET_passPre"]        = new TH1F("dPhivHtMET_passPre"       , (passPre+"; |#Delta#phi(#slash{E}_{T},vHt)|; "       +yTitle).c_str(),  21,   0  ,    3.15 ); // 10
  plots["dPhivHtMETnoB_passPre"]     = new TH1F("dPhivHtMETnoB_passPre"    , (passPre+"; |#Delta#phi(#slash{E}_{T},vHt)| (noBs); "+yTitle).c_str(),  21,   0  ,    3.15 ); // 10
  plots["dotHtAlongAway_passPre"]    = new TH1F("dotHtAlongAway_passPre"   , (passPre+"; dot H_{T}^{along}/H_{T}^{away}; "        +yTitle).c_str(),  40,   0  ,    2    ); // 10
  plots["dotHtAlongAwayNoB_passPre"] = new TH1F("dotHtAlongAwayNoB_passPre", (passPre+"; dot H_{T}^{along}/H_{T}^{away} (noBs); " +yTitle).c_str(),  40,   0  ,    2    ); // 10

  // set Sumw2 for all histograms
  for(map<TString,TH1F*>::iterator plotsIt = plots.begin(); plotsIt != plots.end(); ++plotsIt) plotsIt->second->Sumw2();
} // loadPlots()

// temporally overload to NOT load tau stuff; remove when new ttrees are avaliable
// also uncomment nVetoTau
void Analyzer::loadVariables() {
  load(EVTINFO);
  //load(AK4JETS,JetReader::LOADRECO | JetReader::LOADGEN | JetReader::LOADJETSHAPE | JetReader::FILLOBJ);
  load(AK4JETS,JetReader::LOADRECO | JetReader::LOADGEN | JetReader::LOADJETSHAPE | JetReader::FILLOBJ);
  load(PICKYJETS);
  load(ELECTRONS);
  load(MUONS);
  //load(PFCANDS);
  if(isMC()) load(GENPARTICLES);
} // Analyzer::loadVariables()

// Analyze event and fill plots
void Analyzer::runEvent() {

  double wt = lumi_ * xsec_ / getEntries();

  // preselection
  if(nVetoedLeptons > 0)  return;
  if(met->pt() < metcut_) return;
  if(nJets<minNJets_)     return;
  if(nBJets < minNBjets_) return;

  // fill tree variables
  scaleFactor = wt;
  npv         = nPV;
  nAK4pfJets  = nJets;
  nVetoTau    = nVetoedTaus;
  //met         = met->pt();
  //nAK4pfBJets = nBJets;
  //ht          = JetKinematics::ht(jets,20,2.4);
  //htAoA       = JetKinematics::htAlongHtAway(*met,jets,20,2.4);
  vars.processVariables(this,&ak4Reader,jets,jets,met); // calculate variables
  outtree->Fill();

  bool passMet = met->pt() > metcut_;
  bool passJet = nJets >= minNJets_ && nBJets >= minNBjets_;
  bool passPre = passMet && passJet;

  // ===== Fill histograms =====
  // partial preselection plots
  if (passJet) plots["met_passJet" ]->Fill(vars.ptMet, wt);
  if (passMet) plots["nj60_passMet"]->Fill(vars.nj60 , wt);
  if (passMet) plots["nJ20_passMet"]->Fill(vars.nJ20 , wt);
  // plots after preselection
  if(!passPre) return;
  plots["j1pt_passPre"]             ->Fill(jets[0]->pt()         , wt);
  plots["ntBtag_passPre"]           ->Fill(vars.ntBtag           , wt);
  plots["nmBtag_passPre"]           ->Fill(vars.nmBtag           , wt);
  plots["dPhiMET12_passPre"]        ->Fill(vars.dPhiMET12        , wt);
  plots["dPhiMET3_passPre"]         ->Fill(vars.dPhiMET3         , wt);
  plots["dPhiB0MET_passPre"]        ->Fill(vars.dPhiB0MET        , wt);
  plots["dPhiB1MET_passPre"]        ->Fill(vars.dPhiB1MET        , wt);
  plots["dPhiB2MET_passPre"]        ->Fill(vars.dPhiB2MET        , wt);
  plots["dPhiB01MET_passPre"]       ->Fill(vars.dPhiB01MET       , wt);
  plots["dPhiCVSnearMET_passPre"]   ->Fill(vars.dPhiCVSnearMET   , wt);
  plots["Mb1b2n_passPre"]           ->Fill(vars.Mb1b2n           , wt);
  plots["mtB0MET_passPre"]          ->Fill(vars.mtB0MET          , wt);
  plots["mtB1MET_passPre"]          ->Fill(vars.mtB1MET          , wt);
  plots["mtB2MET_passPre"]          ->Fill(vars.mtB2MET          , wt);
  plots["mtB01MET_passPre"]         ->Fill(vars.mtB01MET         , wt);
  plots["sSumB01oMET_passPre"]      ->Fill(vars.sSumB01oMET      , wt);
  plots["vSumB01oMET_passPre"]      ->Fill(vars.vSumB01oMET      , wt);
  plots["qgl0_passPre"]             ->Fill(vars.qgl0             , wt);
  plots["qgl1_passPre"]             ->Fill(vars.qgl1             , wt);
  plots["qglprod_passPre"]          ->Fill(vars.qglprod          , wt);
  plots["qglprodNorm_passPre"]      ->Fill(vars.qglprodNorm      , wt);
  plots["qglJ0_passPre"]            ->Fill(vars.qglJ0            , wt);
  plots["qglJ1_passPre"]            ->Fill(vars.qglJ1            , wt);
  plots["qglCVS0_passPre"]          ->Fill(vars.qglCVS0          , wt);
  plots["qglCVS1_passPre"]          ->Fill(vars.qglCVS1          , wt);
  plots["qgl0noB_passPre"]          ->Fill(vars.qgl0noB          , wt);
  plots["qgl1noB_passPre"]          ->Fill(vars.qgl1noB          , wt);
  plots["qglprodnoB_passPre"]       ->Fill(vars.qglprodnoB       , wt);
  plots["qglprodNormnoB_passPre"]   ->Fill(vars.qglprodNormnoB   , wt);
  plots["qglJ0noB_passPre"]         ->Fill(vars.qglJ0noB         , wt);
  plots["qglJ1noB_passPre"]         ->Fill(vars.qglJ1noB         , wt);
  plots["ht_passPre"]               ->Fill(vars.ht               , wt);
  plots["htAlongAway20_passPre"]    ->Fill(vars.htAlongAway20    , wt);
  plots["htAlongAway40_passPre"]    ->Fill(vars.htAlongAway40    , wt);
  plots["maxMj12_passPre"]          ->Fill(vars.maxMj12          , wt);
  plots["rmsJetPT_passPre"]         ->Fill(vars.rmsJetPT         , wt);
  plots["rmsJetDphiMET_passPre"]    ->Fill(vars.rmsJetDphiMET    , wt);
  plots["bInvMass_passPre"]         ->Fill(vars.bInvMass         , wt);
  plots["bTransMass_passPre"]       ->Fill(vars.bTransMass       , wt);
  plots["rmsBEta_passPre"]          ->Fill(vars.rmsBEta          , wt);
  plots["wInvMass_passPre"]         ->Fill(vars.wInvMass         , wt);
  plots["Bpt0_passPre"]             ->Fill(vars.Bpt0             , wt);
  plots["Bpt1_passPre"]             ->Fill(vars.Bpt1             , wt);
  plots["htnoB_passPre"]            ->Fill(vars.htnoB            , wt);
  plots["htJ12_passPre"]            ->Fill(vars.htJ12            , wt);
  plots["htJ12noB_passPre"]         ->Fill(vars.htJ12noB         , wt);
  plots["metOsqrtHt_passPre"]       ->Fill(vars.metOsqrtHt       , wt);
  plots["metOsqrtHtnoB_passPre"]    ->Fill(vars.metOsqrtHtnoB    , wt);
  plots["metOsqrtHtJ12_passPre"]    ->Fill(vars.metOsqrtHtJ12    , wt);
  plots["metOsqrtHtJ12noB_passPre"] ->Fill(vars.metOsqrtHtJ12noB , wt);
  plots["dPhivHtMET_passPre"]       ->Fill(vars.dPhivHtMET       , wt);
  plots["dPhivHtMETnoB_passPre"]    ->Fill(vars.dPhivHtMETnoB    , wt);
  plots["dotHtAlongAway_passPre"]   ->Fill(vars.dotHtAlongAway   , wt);
  plots["dotHtAlongAwayNoB_passPre"]->Fill(vars.dotHtAlongAwayNoB, wt);

} // Analyzer::runEvent()

// Write histograms to file in designated directory
void Analyzer::out(TString outputName, TString outputPath) {
  gSystem->mkdir(outputPath,true);
  TString filename = outputPath + "/" + outputName + "_plots.root";
  TFile  *outfile  = new TFile(filename, "RECREATE");
  outfile->cd();

  for(map<TString,TH1F*>::iterator plotsIt = plots.begin(); plotsIt != plots.end(); ++plotsIt) plotsIt->second->Write();
  outfile->Write();
  outfile->Close();
  delete outfile;
} // Analyzer::out()

// get the x-section for the given sample
double getXsec(string sample) {
  if (sample=="ttbar") return 831.76   ;
  if (sample=="ttZ")   return   0.5662 ;
  if (sample=="znunu_ht100to200") return 372.6   ;
  if (sample=="znunu_ht200to400") return 100.8   ;
  if (sample=="znunu_ht400to600") return  11.99  ;
  if (sample=="znunu_ht600toinf") return   4.113 ;
  if (sample=="T2tt_425_325") return 1.31169   ;
  if (sample=="T2tt_500_325") return 0.51848   ;
  if (sample=="T2tt_650_325") return 0.107045  ;
  if (sample=="T2tt_850_100") return 0.0189612 ;
  cout << "WARNING: no cross section available for " << sample << "!" << endl
	     << "         setting xsec = 1" << endl;
  return 1;
}

#endif

// Process file belonging to specified sample with a given cross section
void processZeroLepton(const string sample     = "ttbar" // sample name
                     ,       double xsec       = -1.0    // -1 to use hard-coded xsecs
                     , const bool   isMC       = true
                     , const string outputdir  = "plots"
                     , const string filePrefix = "/eos/uscms/store/user/vdutta/13TeV/310315/merged/" // location of files
                     , const string fileSuffix = "_ntuple.root"
                     ) {
  cout << "processing " << sample << endl;

  if (xsec<0) xsec = getXsec(sample);
  if (isMC) cout << "using cross section = " << xsec << " pb" << endl;
  string fullname = filePrefix + sample + fileSuffix;

  // Adjustments to default configuration
  BaseTreeAnalyzer::ConfigPars pars;
  pars.defaultJetCollection       = BaseTreeAnalyzer::AK4JETS; // BaseTreeAnalyzer::PICKYJETS;
  //pars.cleanJetsvSelectedLeptons_ = true;

  Analyzer a(fullname, "TestAnalyzer/Events", isMC, &pars, xsec, sample, outputdir); // declare analyzer
  a.analyze(10000); // run: Argument is frequency of printout
  //a.analyze(1000,100000); // for testing
  a.out(sample, outputdir); // write outputfile with plots

} // processSingleLepton()
