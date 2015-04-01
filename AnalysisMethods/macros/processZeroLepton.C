#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TSystem.h>
#include <TFile.h>
#include <TH1F.h>
#include "AnalysisMethods/PlotUtils/interface/Sample.hh"
#include "AnalysisMethods/PlotUtils/interface/PlotTools.hh"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
#include "AnalysisTools/KinematicVariables/interface/JetKinematics.h"
#include "AnalysisBase/TreeAnalyzer/interface/BaseTreeAnalyzer.h"
//#include "AnalysisTools/KinematicVariables/interface/Topness.h"
#include "AnalysisTools/TreeReader/interface/ElectronReader.h"
#include "AnalysisMethods/macros/0LepSearchRegions/cncVariables.h"
#endif

using namespace ucsbsusy;

class Analyzer : public BaseTreeAnalyzer {

  public :

  Analyzer(TString fileName, TString treeName, bool isMCTree, ConfigPars * pars, double xSec, TString sname, TString outputdir)
    : BaseTreeAnalyzer(fileName, treeName, isMCTree, pars)
    , xsec_(xSec)
    , sname_(sname)
    , outputdir_(outputdir)
  {
      loadPlots(); // initialize plots

      // initiliaze tree
      gSystem->mkdir(outputdir,true);
      fout = new TFile (outputdir+"/"+sname+"_tree.root","RECREATE");
      fout->cd();
      outtree = new TTree("events","analysis tree");
      outtree->Branch( "ScaleFactor", &ScaleFactor, "ScaleFactor/F" );
      outtree->Branch( "NPV"        , &NPV        ,         "NPV/I" );
      outtree->Branch( "NAK5PFJets" , &NAK5PFJets ,  "NAK5PFJets/I" );
      outtree->Branch( "NAK5PFBJets", &NAK5PFBJets, "NAK5PFBJets/I" );
      outtree->Branch( "NVetoTau"   , &NVetoTau   ,    "NVetoTau/I" );
      outtree->Branch( "MET"        , &MET        ,         "MET/F" );
      outtree->Branch( "HT"         , &HT         ,          "HT/F" );
      outtree->Branch( "HTAovA"     , &HTAovA     ,      "HTAovA/F" );
  } // Analyzer()

  virtual ~Analyzer() {
    fout->cd();
    outtree->Write();
    fout->Close();
  } // ~Analyzer()
  

  const double xsec_;
  const double lumi_       = 1000.0 ; // in /pb
  const double metcut_     =  200.0 ;
  const int    minNJets_   =    4   ;
  const int    minNBjets_  =    1   ;
  const int    maxNTaus_   =    0   ;
  const int    nLeptons_   =    0   ;
  //const float  minTrigMuPt_  =   27.0 ;
  //const float  maxTrigMuEta_ =    2.1 ;
  //const float  minTrigEPt_   =   32.0 ;
  //const float  maxTrigEEta_  =    2.1 ;
  const TString sname_     = "testSam.root";
  const TString outputdir_ = "./plots/";

  TFile *fout;
  TTree *outtree;

  float ScaleFactor;
  int   NPV;
  int   NAK5PFJets;
  int   NAK5PFBJets;
  int   NVetoTau;
  float MET;
  float HT;
  float HTAovA;
  float stdIso;

  void runEvent();
  
  void loadPlots();
  
  void out(TString outputName, TString outputPath);
  
  map<TString,TH1F*> plots;

}; // Analyzer : BaseTreeAnalyzer

// Define plots, names, and binning
void Analyzer::loadPlots() {
  TString passJet = "N_{jets} >= 4, N_{bjets} >=1";
  TString passMet = "MET > 200";
  TString passPre = "MET > 200, N_{jets} >= 4, N_{bjets} >=1";
  TString yTitle  = "Events";

  // partial presel plots
  plots["met__passJet"] = new TH1F("met_passjets"  , TString(passJet+"; #slash{E}_{T} [GeV]; "+yTitle).Data(), 50,  0  , 500  );
  plots["nJet_passMet"] = new TH1F("njets_passmet" , TString(passMet+"; Number of Jets; "     +yTitle).Data(), 10, -0.5,   9.5);
  plots["nB___passMet"] = new TH1F("nbjets_passmet", TString(passMet+"; Number of B Jets; "   +yTitle).Data(),  8, -0.5,   7.5);
  // preselection plots
  plots["ht___passPre"] = new TH1F("ht_passpresel"    , TString(passPre+"; H_{T} [GeV]; "         +yTitle).Data(),  60, 0, 1500   );
  plots["j1pt_passPre"] = new TH1F("jet1pt_passpresel", TString(passPre+"; Lead jet p_{T} [GeV]; "+yTitle).Data(), 120, 0,  600   );

  // set Sumw2 for all histograms
  for(map<TString,TH1F*>::iterator plotsIt = plots.begin(); plotsIt != plots.end(); ++plotsIt) plotsIt->second->Sumw2();\
} // loadPlots()

// Analyze event and fill plots
void Analyzer::runEvent() {

  double wgt = lumi_*xsec_/getEntries();

  if(nSelLeptons < nLeptons_) return; // only look at 0-lep events

  // fill tree variables
  ScaleFactor = wgt;
  NPV         = nPV;
  NAK5PFJets  = nJets;
  NAK5PFBJets = nBJets;
  NVetoTau    = nVetoedTaus;
  MET         = met->pt();
  HT          = JetKinematics::ht(jets,40,2.4);
  HTAovA      = JetKinematics::htAlongHtAway(*met,jets,40,2.4);

  outtree->Fill();

  bool passMet = met->pt() > metcut_;
  bool passJet = nJets >= minNJets_ && nBJets >= minNBjets_;
  bool passPre = passMet && passJet;

  // Fill histograms

  //  partial preselection plots
  if(passJet) plots["met__passJet"]->Fill(met->pt(), wgt);
  if(passMet) plots["nJet_passMet"]->Fill(nJets    , wgt);
  if(passMet) plots["nB___passMet"]->Fill(nBJets   , wgt);

  if(!passPre) return;
  // plots after preselection
  plots["ht___passPre" ]->Fill(JetKinematics::ht(jets), wgt);
  plots["j1pt_passPre"] ->Fill(jets[0]->pt()          , wgt);

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

// Process file belonging to specified sample with a given cross section
void processSingleLepton(TString       sname      = "test", // sample name
                         const int     fileindex  = -1,     // index of file (-1 means there is only 1 file for this sample)
                         const bool    isMC       = true,
                         const TString inFile     = "evttree_numEvent1000.root",
                         const double  xsec       = 1.0,
                         const TString outputdir  = "run/plots",
                         const TString filePrefix = "file:$CMSSW_BASE/src/AnalysisBase/Analyzer/test/") // needed e.g. to access files with xrootd
{
  cout << "processing file " << (fileindex > -1 ? fileindex : 0) << " of " << sname.Data() << endl;
  TString fullname = filePrefix+inFile;

  if(fileindex > -1) sname += "_" + string(fileindex); // Make sure the output has a unique name if there are multiple files to process
  if(isMC) cout << "cross section: " << xsec << " pb" << endl;

  // Adjustments to default configuration
  BaseTreeAnalyzer::ConfigPars pars;
  pars.defaultJetCollection       = BaseTreeAnalyzer::AK4JETS; // BaseTreeAnalyzer::PICKYJETS;
  pars.cleanJetsvSelectedLeptons_ = true;

  Analyzer a(fullname, "TestAnalyzer/Events", isMC, &pars, xsec, sname, outputdir); // declare analyzer
  a.analyze(1000); // run: Argument is frequency of printout
  a.out(sname, outputdir); // write outputfile with plots

} // processSingleLepton()
