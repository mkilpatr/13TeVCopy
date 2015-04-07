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
//#include "AnalysisMethods/macros/0LepSearchRegions/cncVariables.h"


using namespace ucsbsusy;

class Analyzer : public BaseTreeAnalyzer {

  public :

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
    outtree->Branch( "scaleFactor", &scaleFactor_, "scaleFactor/F" );
    outtree->Branch( "npv"        , &npv_        ,         "npv/I" );
    outtree->Branch( "nAK4pfJets" , &nAK4pfJets_ ,  "nAK4pfJets/I" );
    outtree->Branch( "nAK4pfBJets", &nAK4pfBJets_, "nAK4pfBJets/I" );
    outtree->Branch( "nVetoTau"   , &nVetoTau_   ,    "nVetoTau/I" );
    outtree->Branch( "met"        , &met_        ,         "met/F" );
    outtree->Branch( "ht"         , &ht_         ,          "ht/F" );
    outtree->Branch( "htAovA"     , &htAovA_     ,      "htAovA/F" );
  } // Analyzer()

  virtual ~Analyzer() {
    fout->cd();
    outtree->Write();
    fout->Close();
  } // ~Analyzer()

  const double  metcut_    = 200.0 ;
  const int     minNJets_  =   4   ;
  const int     minNBjets_ =   1   ;
  const int     maxNTaus_  =   0   ;
  const int     nLeptons_  =   0   ;

  const double  lumi_      = 1000.0 ; // in /pb
  const double  xsec_      ;
  const TString sample_    ;
  const TString outputdir_ ;

  TFile *fout    ;
  TTree *outtree ;

  float scaleFactor_ ;
  int   npv_         ;
  int   nAK4pfJets_  ;
  int   nAK4pfBJets_ ;
  int   nVetoTau_    ;
  float met_         ;
  float ht_          ;
  float htAovA_      ;

  void loadVariables();
  void runEvent();
  void loadPlots();
  void out(TString outputName, TString outputPath);
  //double getXsec(string sample);
  
  map<TString,TH1F*> plots;

}; // Analyzer : BaseTreeAnalyzer

// Define plots, names, and binning
void Analyzer::loadPlots() {
  string passJet = "N_{jets} >= 4, N_{bjets} >=1";
  string passMet = "MET > 200";
  string passPre = "MET > 200, N_{jets} >= 4, N_{bjets} >=1";
  string yTitle  = "Events";

  // partial presel plots
  plots["met__passJet"] = new TH1F("met_passjets"  , (passJet+"; #slash{E}_{T} [GeV]; "+yTitle).c_str(), 50,  0  , 500   );
  plots["nJet_passMet"] = new TH1F("njets_passmet" , (passMet+"; Number of Jets; "     +yTitle).c_str(), 10, -0.5,   9.5 );
  plots["nB___passMet"] = new TH1F("nbjets_passmet", (passMet+"; Number of B Jets; "   +yTitle).c_str(),  8, -0.5,   7.5 );
  // preselection plots
  plots["ht___passPre"] = new TH1F("ht_passpresel"    , (passPre+"; H_{T} [GeV]; "         +yTitle).c_str(),  60, 0, 1500 );
  plots["j1pt_passPre"] = new TH1F("jet1pt_passpresel", (passPre+"; Lead jet p_{T} [GeV]; "+yTitle).c_str(), 120, 0,  600 );

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
  scaleFactor_ = wt;
  npv_         = nPV;
  nAK4pfJets_  = nJets;
  nAK4pfBJets_ = nBJets;
  nVetoTau_    = nVetoedTaus;
  met_         = met->pt();
  ht_          = JetKinematics::ht(jets,20,2.4);
  htAovA_      = JetKinematics::htAlongHtAway(*met,jets,20,2.4);
  outtree->Fill();

  bool passMet = met->pt() > metcut_;
  bool passJet = nJets >= minNJets_ && nBJets >= minNBjets_;
  bool passPre = passMet && passJet;

  // ===== Fill histograms =====
  // partial preselection plots
  if(passJet) plots["met__passJet"]->Fill(met->pt(), wt);
  if(passMet) plots["nJet_passMet"]->Fill(nJets    , wt);
  if(passMet) plots["nB___passMet"]->Fill(nBJets   , wt);
  // plots after preselection
  if(!passPre) return;
  plots["ht___passPre" ]->Fill(JetKinematics::ht(jets), wt);
  plots["j1pt_passPre"] ->Fill(jets[0]->pt()          , wt);

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
  if (sample=="ttbar") return 831.76;
  if (sample=="ttZ")   return 0.5662;
  if (sample=="znunu_ht100to200") return 372.6;
  if (sample=="znunu_ht200to400") return 100.8;
  if (sample=="znunu_ht400to600") return 11.99;
  if (sample=="znunu_ht600toinf") return 4.113;
  if (sample=="T2tt_425_325") return 1.31169;
  if (sample=="T2tt_500_325") return 0.51848;
  if (sample=="T2tt_650_325") return 0.107045;
  if (sample=="T2tt_850_100") return 0.0189612;
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
  a.analyze(10000); // run: Argument is frequency of printout
  a.out(sample, outputdir); // write outputfile with plots

} // processSingleLepton()
