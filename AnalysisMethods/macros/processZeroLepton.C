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

    outtree->Branch( "ptMet"        , &vars.ptMet         ,         "ptMet/F" );
    outtree->Branch( "nJ90"         , &vars.nJ90          ,          "nJ90/I" );
    outtree->Branch( "nJ20"         , &vars.nJ20          ,          "nJ20/I" );
    outtree->Branch( "ntBtag"       , &vars.ntBtag        ,        "ntBtag/I" );
    outtree->Branch( "nmBtag"       , &vars.nmBtag        ,        "nmBtag/I" );
    outtree->Branch( "dPhiMET12"    , &vars.dPhiMET12     ,     "dPhiMET12/F" );
    outtree->Branch( "dPhiMET3"     , &vars.dPhiMET3      ,      "dPhiMET3/F" );
    outtree->Branch( "qgl0"         , &vars.qgl0          ,          "qgl0/F" );
    outtree->Branch( "qgl1"         , &vars.qgl1          ,          "qgl1/F" );
    outtree->Branch( "qglprod"      , &vars.qglprod       ,       "qglprod/F" );
    outtree->Branch( "ht"           , &vars.ht            ,            "ht/F" );
    outtree->Branch( "htAlongAway"  , &vars.htAlongAway   ,   "htAlongAway/F" );
    outtree->Branch( "rmsJetPT"     , &vars.rmsJetPT      ,      "rmsJetPT/F" );
    outtree->Branch( "rmsJetDphiMET", &vars.rmsJetDphiMET , "rmsJetDphiMET/F" );
    outtree->Branch( "bInvMass"     , &vars.bInvMass      ,      "bInvMass/F" );
    outtree->Branch( "bTransMass"   , &vars.bTransMass    ,    "bTransMass/F" );
    outtree->Branch( "rmsBEta"      , &vars.rmsBEta       ,       "rmsBEta/F" );
    outtree->Branch( "wInvMass"     , &vars.wInvMass      ,      "wInvMass/F" );
    outtree->Branch( "Bpt0"         , &vars.Bpt0          ,          "Bpt0/F" );
    outtree->Branch( "Bpt1"         , &vars.Bpt1          ,          "Bpt1/F" );

  } // Analyzer()

  virtual ~Analyzer() {
    fout->cd();
    outtree->Write();
    fout->Close();
  } // ~Analyzer()

  const double  metcut_    = 200.0 ;
  const int     minNJets_  =   3   ;
  const int     minNBjets_ =   1   ;
  const int     maxNTaus_  =   0   ;
  const int     nLeptons_  =   0   ;

  const double  lumi_      = 1000.0 ; // in /pb
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
  plots["met_passJet" ] = new TH1F( "met_passPre" , (passJet+"; #slash{E}_{T} [GeV]; "+yTitle).c_str(), 50,  0  , 500   );
  plots["nJ90_passMet"] = new TH1F( "nJ90_passPre", (passMet+"; Number of Jets; "     +yTitle).c_str(), 10, -0.5,  19.5 );
  plots["nJ20_passMet"] = new TH1F( "nJ20_passPre", (passMet+"; Number of B Jets; "   +yTitle).c_str(), 10, -0.5,  19.5 );
  // preselection plots
  plots["j1pt_passPre"]         = new TH1F("jet1pt_passPre"       , (passPre+"; Lead jet p_{T} [GeV]; "            +yTitle).c_str(), 200,   0  , 1000    );
  plots["ntBtag_passPre"]       = new TH1F("ntBtag_passPre"       , (passPre+"; Number of B Jets; "                +yTitle).c_str(),   8,  -0.5,    7.5  );
  plots["nmBtag_passPre"]       = new TH1F("nmBtag_passPre"       , (passPre+"; Number of B Jets; "                +yTitle).c_str(),   8,  -0.5,    7.5  );
  plots["dPhiMET12_passPre"]    = new TH1F("dPhiMET12_passPre"    , (passPre+"; |#Delta#phi(#slash{E}_{T},j1-2)|; "+yTitle).c_str(),  21,   0  ,    3.15 ); //  0.15
  plots["dPhiMET3_passPre"]     = new TH1F("dPhiMET3_passPre"     , (passPre+"; |#Delta#phi(#slash{E}_{T},j3)|; "  +yTitle).c_str(),  21,   0  ,    3.15 ); //  0.15
  plots["qgl0_passPre"]         = new TH1F("qgl0_passPre"         , (passPre+"; QGL_{0}; "                         +yTitle).c_str(),  50,   0  ,    1    ); //  0.02
  plots["qgl1_passPre"]         = new TH1F("qgl1_passPre"         , (passPre+"; QGL_{1}; "                         +yTitle).c_str(),  50,   0  ,    1    ); //  0.02
  plots["qglprod_passPre"]      = new TH1F("qglprod_passPre"      , (passPre+"; ln[#prod QGL_{q}]; "               +yTitle).c_str(),  25, -12  ,    0    ); // 12
  plots["ht_passPre"]           = new TH1F("ht_passPre"           , (passPre+"; H_{T} [GeV]; "                     +yTitle).c_str(),  80,   0  , 2000    );
  plots["htAlongAway_passPre"]  = new TH1F("htAlongAway_passPre"  , (passPre+"; H_{T}^{along}/H_{T}^{away}; "      +yTitle).c_str(),  40,   0  ,    2    ); //  0.05
  plots["rmsJetPT_passPre"]     = new TH1F("rmsJetPT_passPre"     , (passPre+"; rmsJetPT; "                        +yTitle).c_str(),  40,   0  ,  400    ); // 10
  plots["rmsJetDphiMET_passPre"]= new TH1F("rmsJetDphiMET_passPre", (passPre+"; RMS[#Delta#phi(J,#slash{E}_{T})]; "+yTitle).c_str(),  25,   0  ,    2.5  ); //  0.1
  plots["bInvMass_passPre"]     = new TH1F("bInvMass_passPre"     , (passPre+"; m(b1+b2); "                        +yTitle).c_str(),  60,   0  ,  600    ); // 10
  plots["bTransMass_passPre"]   = new TH1F("bTransMass_passPre"   , (passPre+"; m_{T}(near b to #slash{E}_{T}); "  +yTitle).c_str(),  80,   0  ,  800    ); // 10
  plots["rmsBEta_passPre"]      = new TH1F("rmsBEta_passPre"      , (passPre+"; RMS[#Delta#eta(b,jets)]; "         +yTitle).c_str(),  25,   0  ,    2.5  ); //  0.1
  plots["wInvMass_passPre"]     = new TH1F("wInvMass_passPre"     , (passPre+"; m(max p_{T} jj); "                 +yTitle).c_str(),  40,   0  ,  800    ); // 20
  plots["Bpt0_passPre"]         = new TH1F("Bpt0_passPre"         , (passPre+"; p_{t}(b0); "                       +yTitle).c_str(),  40,   0  ,  800    ); // 20
  plots["Bpt1_passPre"]         = new TH1F("Bpt1_passPre"         , (passPre+"; p_{t}(b1); "                       +yTitle).c_str(),  60,   0  ,  600    ); // 10

  // set Sumw2 for all histograms
  for(map<TString,TH1F*>::iterator plotsIt = plots.begin(); plotsIt != plots.end(); ++plotsIt) plotsIt->second->Sumw2();
} // loadPlots()

// temporally overload to NOT load tau stuff; remove when new ttrees are avaliable
// also uncomment nVetoTau
void Analyzer::loadVariables() {
  load(EVTINFO);
  load(AK4JETS);
  load(PICKYJETS);
  load(ELECTRONS);
  load(MUONS);
  //load(PFCANDS);
  if(isMC()) load(GENPARTICLES);
} // Analyzer::loadVariables()

// Analyze event and fill plots
void Analyzer::runEvent() {

  double wt = lumi_ * xsec_ / getEntries();
  if(nSelLeptons < nLeptons_) return; // only look at 0-lep events

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
  if (passMet) plots["nJ90_passMet"]->Fill(vars.nJ90 , wt);
  if (passMet) plots["nJ20_passMet"]->Fill(vars.nJ20 , wt);
  // plots after preselection
  if(!passPre) return;
  plots["j1pt_passPre"]         ->Fill(jets[0]->pt()     , wt);
  plots["ntBtag_passPre"]       ->Fill(vars.ntBtag       , wt);
  plots["nmBtag_passPre"]       ->Fill(vars.nmBtag       , wt);
  plots["dPhiMET12_passPre"]    ->Fill(vars.dPhiMET12    , wt);
  plots["dPhiMET3_passPre"]     ->Fill(vars.dPhiMET3     , wt);
  plots["qgl0_passPre"]         ->Fill(vars.qgl0         , wt);
  plots["qgl1_passPre"]         ->Fill(vars.qgl1         , wt);
  plots["qglprod_passPre"]      ->Fill(vars.qglprod      , wt);
  plots["ht_passPre"]           ->Fill(vars.ht           , wt);
  plots["htAlongAway_passPre"]  ->Fill(vars.htAlongAway  , wt);
  plots["rmsJetPT_passPre"]     ->Fill(vars.rmsJetPT     , wt);
  plots["rmsJetDphiMET_passPre"]->Fill(vars.rmsJetDphiMET, wt);
  plots["bInvMass_passPre"]     ->Fill(vars.bInvMass     , wt);
  plots["bTransMass_passPre"]   ->Fill(vars.bTransMass   , wt);
  plots["rmsBEta_passPre"]      ->Fill(vars.rmsBEta      , wt);
  plots["wInvMass_passPre"]     ->Fill(vars.wInvMass     , wt);
  plots["Bpt0_passPre"]         ->Fill(vars.Bpt0         , wt);
  plots["Bpt1_passPre"]         ->Fill(vars.Bpt1         , wt);

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
                     , const string filePrefix = "/eos/uscms/store/user/vdutta/13TeV/040215/merged/" // location of files
                     , const string fileSuffix = "_ntuple.root"
                     ) {
  cout << "processing " << sample << endl;

  if (xsec<0) xsec = getXsec(sample);
  if (isMC) cout << "     using cross section = " << xsec << " pb" << endl;
  string fullname = filePrefix + sample + fileSuffix;

  // Adjustments to default configuration
  BaseTreeAnalyzer::ConfigPars pars;
  pars.defaultJetCollection       = BaseTreeAnalyzer::AK4JETS; // BaseTreeAnalyzer::PICKYJETS;
  //pars.cleanJetsvSelectedLeptons_ = true;

  Analyzer a(fullname, "TestAnalyzer/Events", isMC, &pars, xsec, sample, outputdir); // declare analyzer
  //a.analyze(10000); // run: Argument is frequency of printout
  a.analyze(10000); // run: Argument is frequency of printout
  a.out(sample, outputdir); // write outputfile with plots

} // processSingleLepton()
