#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TSystem.h>
#include <TFile.h>
#include <TH1F.h>
#include "AnalysisMethods/PlotUtils/interface/Sample.hh"
#include "AnalysisMethods/PlotUtils/interface/PlotTools.hh"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
#include "AnalysisTools/KinematicVariables/interface/JetKinematics.h"
#include "AnalysisBase/TreeAnalyzer/interface/BaseTreeAnalyzer.h"
#include "AnalysisTools/KinematicVariables/interface/Topness.h"
#include "AnalysisTools/TreeReader/interface/ElectronReader.h"
#endif

using namespace ucsbsusy;

class Analyzer : public BaseTreeAnalyzer {

  public :

  Analyzer(TString fileName, TString treeName, bool isMCTree, ConfigPars * pars, double xSec, TString sname, TString outputdir) :
    BaseTreeAnalyzer(fileName, treeName, isMCTree, pars),xsec_(xSec), sname_(sname), outputdir_(outputdir) {
      // configuration
      tNess     = new Topness();
      tNessInfo = new TopnessInformation();

      // initialize plots
      loadPlots();

      // initiliaze tree
      gSystem->mkdir(outputdir,true);
      fout = new TFile (outputdir+"/"+sname+"_tree.root","RECREATE");
      fout->cd();
      outtree = new TTree("events","analysis tree");
      outtree->Branch("ScaleFactor",&ScaleFactor,"ScaleFactor/F");
      outtree->Branch("NPV",&NPV,"NPV/I");
      outtree->Branch("NAK5PFJets",&NAK5PFJets,"NAK5PFJets/I");
      outtree->Branch("NAK5PFBJets",&NAK5PFBJets,"NAK5PFBJets/I");
      outtree->Branch("NRecoLep",&NRecoLep,"NRecoLep/I");
      outtree->Branch("NVetoTau",&NVetoTau,"NVetoTau/I");
      outtree->Branch("MET",&MET,"MET/F");
      outtree->Branch("HT",&HT,"HT/F");
      outtree->Branch("HTAovA",&HTAovA,"HTAovA/F");
      outtree->Branch("DphiLepW",&DphiLepW,"DphiLepW/F");
      outtree->Branch("DphiLepWRecoSeen",&DphiLepWRecoSeen,"DphiLepWRecoSeen/F");
      outtree->Branch("DphiLepWRecoMiss",&DphiLepWRecoMiss,"DphiLepWRecoMiss/F");
      outtree->Branch("minTopness",&minTopness,"minTopness/F");
      outtree->Branch("minTopnessNoLog",&minTopnessNoLog,"minTopnessNoLog/F");
      outtree->Branch("stdIso",&stdIso,"stdIso/F");
      outtree->Branch("LepPdgId",&LepPdgId,"LepPdgId/I");
      outtree->Branch("LepPt",&LepPt,"LepPt/F");
    } // Analyzer()

  virtual ~Analyzer() {
    fout->cd();
    outtree->Write();
    fout->Close();
  } // ~Analyzer()
  

  const double xsec_;
  const double lumi_         = 1000.0; // in /pb
  const double metcut_       = 200.0;
  const int    minNJets_     = 4;
  const int    minNBjets_    = 1;
  const int    maxNTaus_     = 0;
  const int    nLeptons_     = 0;
  const float  minTrigMuPt_  = 27.0;
  const float  maxTrigMuEta_ =  2.1;
  const float  minTrigEPt_   = 32.0;
  const float  maxTrigEEta_  =  2.1;
  const TString sname_       = "testSam.root";
  const TString outputdir_   = "./plots/";  

  Topness *tNess;
  TopnessInformation *tNessInfo;
  TFile *fout;
  TTree *outtree;

  float ScaleFactor;
  int   NPV;
  int   NAK5PFJets;
  int   NAK5PFBJets;
  int   NRecoLep;
  int   NVetoTau;
  float MET;
  float HT;
  float HTAovA;
  float DphiLepW;
  float DphiLepWRecoSeen;
  float DphiLepWRecoMiss;
  float minTopness;
  float minTopnessNoLog;
  float stdIso;
  int   LepPdgId;
  float LepPt;

  void runEvent();
  
  void loadPlots();
  
  void out(TString outputName, TString outputPath);
  
  map<TString,TH1F*> plots;

}; // Analyzer : BaseTreeAnalyzer

// Define plots, names, and binning
void Analyzer::loadPlots()
{

  TString txt_passjets   = "N_{jets} >= 4, N_{bjets} >=1";
  TString txt_passmet    = "MET > 200";
  TString txt_passpresel = "MET > 200, N_{jets} >= 4, N_{bjets} >=1";
  TString txt_ytitle     = "Events";

  // N-1 plots
  plots["met_passjets"]          = new TH1F("met_passjets"   , TString(txt_passjets+"; #slash{E}_{T} [GeV]; "    +txt_ytitle).Data(), 50,  0  , 500  );
  plots["njets_passmet"]         = new TH1F("njets_passmet"  , TString(txt_passmet +"; Number of Jets; "         +txt_ytitle).Data(), 10, -0.5,   9.5);
  plots["nbjets_passmet"]        = new TH1F("nbjets_passmet" , TString(txt_passmet +"; Number of B-tagged Jets; "+txt_ytitle).Data(),  8, -0.5,   7.5);
  // plots after preselection
  plots["ht_passpresel"]         = new TH1F("ht_passpresel"         , TString(txt_passpresel+"; H_{T} [GeV]; "                         +txt_ytitle).Data(),  60, 0, 1500   );
  plots["leppt_passpresel"]      = new TH1F("leppt_passpresel"      , TString(txt_passpresel+"; Lepton p_{T} [GeV]; "                  +txt_ytitle).Data(), 100, 0,  500   );
  plots["jet1pt_passpresel"]     = new TH1F("jet1pt_passpresel"     , TString(txt_passpresel+"; Leading jet p_{T} [GeV]; "             +txt_ytitle).Data(), 120, 0,  600   );
  plots["dphilepmet_passpresel"] = new TH1F("dphilepmet_passpresel" , TString(txt_passpresel+"; |#Delta#phi(l, #slash{E}_{T})| [GeV]; "+txt_ytitle).Data(),  50, 0,    3.15);
  plots["dphilepw_passpresel"]   = new TH1F("dphilepw_passpresel"   , TString(txt_passpresel+"; |#Delta#phi(l, W)| [GeV]; "            +txt_ytitle).Data(),  50, 0,    3.15);
  plots["mtlepmet_passpresel"]   = new TH1F("mtlepmet_passpresel"   , TString(txt_passpresel+"; m_{T}(l, #slash{E}_{T}) [GeV]; "       +txt_ytitle).Data(),  40, 0,  200   );

  // set Sumw2 for histograms
  for(map<TString,TH1F*>::iterator plotsIt = plots.begin(); plotsIt != plots.end(); ++plotsIt) {
    plotsIt->second->Sumw2();
  }

} // loadPlots()

// Analyze event and fill plots
void Analyzer::runEvent()
{

  double wgt = lumi_*xsec_/getEntries();

  if(nSelLeptons < nLeptons_) return; // check for at least one lepton passing pog definitions

  vector<LeptonF*> trigLeptons;
  for(auto* lepton : selectedLeptons) {
    if(lepton->ismuon() && lepton->pt() > minTrigMuPt_ && fabs(lepton->eta()) < maxTrigMuEta_) trigLeptons.push_back(lepton);
    else if(lepton->iselectron() && lepton->pt() > minTrigEPt_ && fabs(lepton->eta()) < maxTrigEEta_) trigLeptons.push_back(lepton);
  }

  if(trigLeptons.size() != 1) return; // require exactly one good lepton in trigger acceptance

  if(nSelLeptons > nLeptons_) return; // veto additional leptons passing pog definitions

  //if(nVetoedTaus > maxNTaus_)     return;

  // fill tree variables
  ScaleFactor = wgt;
  NPV = nPV;
  NAK5PFJets = nJets;
  NAK5PFBJets = nBJets;
  NRecoLep = nSelLeptons;
  NVetoTau = nVetoedTaus;
  MET = met->pt();
  HT = JetKinematics::ht(jets,40,2.4);
  HTAovA = JetKinematics::htAlongHtAway(*met,jets,40,2.4);

  LeptonF* lep = trigLeptons.at(0);
  MomentumF* W = new MomentumF(lep->p4() + met->p4());
  DphiLepW = PhysicsUtilities::deltaPhi(*lep, *W);

  stdIso = (lep->pfdbetaiso())/(lep->pt());

  minTopness = tNess->findMinTopnessConfiguration(selectedLeptons,jets,met,tNessInfo);
  minTopnessNoLog = std::exp(minTopness);

  DphiLepWRecoSeen = PhysicsUtilities::deltaPhi(tNessInfo->top1_l,tNessInfo->top1_w);
  DphiLepWRecoMiss = PhysicsUtilities::deltaPhi(tNessInfo->top1_l,tNessInfo->top2_w);


  LepPdgId = lep->pdgid();
  LepPt    = lep->pt();

  outtree->Fill();

  bool passmet = met->pt() > metcut_;
  bool passjets = nJets >= minNJets_ && nBJets >= minNBjets_;
  bool passpreselection = passmet && passjets;

  // Fill histograms

  // N-1 plots
  if(passjets) plots["met_passjets"]  ->Fill(met->pt(), wgt);
  if(passmet)  plots["njets_passmet"] ->Fill(nJets, wgt);
  if(passmet)  plots["nbjets_passmet"]->Fill(nBJets, wgt);

  if(!passpreselection) return;

  // plots after preselection
  plots["ht_passpresel"]        ->Fill(JetKinematics::ht(jets), wgt);
  plots["leppt_passpresel"]     ->Fill(lep->pt(), wgt);
  plots["jet1pt_passpresel"]    ->Fill(jets.at(0)->pt(), wgt);
  plots["dphilepmet_passpresel"]->Fill(fabs(PhysicsUtilities::deltaPhi(*lep, *met)), wgt);
  plots["dphilepw_passpresel"]  ->Fill(fabs(PhysicsUtilities::deltaPhi(*lep, *W)), wgt);
  plots["mtlepmet_passpresel"]  ->Fill(JetKinematics::transverseMass(*lep, *met), wgt);

}

// Write histograms to file in designated directory
void Analyzer::out(TString outputName, TString outputPath)
{

  gSystem->mkdir(outputPath,true);
  TString filename = outputPath + "/" + outputName + "_plots.root";
  TFile *outfile = new TFile(filename, "RECREATE");
  outfile->cd();

  for(map<TString,TH1F*>::iterator plotsIt = plots.begin(); plotsIt != plots.end(); ++plotsIt) {
    plotsIt->second->Write();
  }

  outfile->Write();
  outfile->Close();

  delete outfile;

}

// Process file belonging to specified sample with a given cross section
void processSingleLepton(TString sname = "test",               // sample name
                         const int fileindex = -1,             // index of file (-1 means there is only 1 file for this sample)
                         const bool isMC = true,               // data or MC
                         const TString fname = "evttree_numEvent1000.root", // path of file to be processed
                         const double xsec = 1.0,              // cross section to be used with this file
                         const TString outputdir = "run/plots",    // directory to which files with histograms will be written
                         const TString fileprefix = "file:$CMSSW_BASE/src/AnalysisBase/Analyzer/test/") // prefix for file name, needed e.g. to access files with xrootd
{

  printf("Processing file %d of %s sample\n", (fileindex > -1 ? fileindex : 0), sname.Data());

  // Make sure the output has a unique name in case there are multiple files to process
  if(fileindex > -1)
    sname += TString::Format("_%d",fileindex);

  if(isMC)
    printf("Cross section: %5.2f pb\n", xsec);

  TString fullname = fileprefix+fname;

  // Adjustments to default configuration
  BaseTreeAnalyzer::ConfigPars pars;
  pars.defaultJetCollection = BaseTreeAnalyzer::PICKYJETS;
  pars.cleanJetsvSelectedLeptons_ = true;

  // Declare analyzer
  Analyzer a(fullname, "TestAnalyzer/Events", isMC, &pars, xsec, sname, outputdir);

  // Run! Argument is frequency of printout
  a.analyze(100000);

  // Write outputfile with plots 
  a.out(sname, outputdir);

}
