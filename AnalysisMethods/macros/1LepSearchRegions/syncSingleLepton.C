#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TSystem.h>
#include <TFile.h>
#include <TH1F.h>
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
#include "AnalysisTools/KinematicVariables/interface/JetKinematics.h"
#include "AnalysisBase/TreeAnalyzer/interface/BaseTreeAnalyzer.h"
#endif

using namespace ucsbsusy;

class Analyzer : public BaseTreeAnalyzer {

  public :

  Analyzer(TString fileName, TString treeName, bool isMCTree, ConfigPars * pars, double xSec, TString sname, TString outputdir) : 
    BaseTreeAnalyzer(fileName, treeName, isMCTree, pars),xsec_(xSec), sname_(sname), outputdir_(outputdir) {

      // initiliaze tree
      gSystem->mkdir(outputdir,true);
      fout = new TFile (outputdir+"/"+sname+"_tree.root","RECREATE");
      fout->cd();
      outtree = new TTree("Events","Events");
      outtree->Branch("run",&run,"run/i");
      outtree->Branch("lumi",&lumi,"lumi/i");
      outtree->Branch("event",&event,"event/i");
      outtree->Branch("wgt",&wgt,"wgt/F");
      outtree->Branch("NPV",&nPV,"NPV/I");
      outtree->Branch("lepton_pdgid",&lepton_pdgid,"lepton_pdgid/I");
      outtree->Branch("lepton_pt",&lepton_pt,"lepton_pt/F");
      outtree->Branch("lepton_eta",&lepton_eta,"lepton_eta/F");
      outtree->Branch("lepton_phi",&lepton_phi,"lepton_phi/F");
      outtree->Branch("NJets",&nJets,"NJets/I");
      outtree->Branch("NBJets",&nBJets,"NBJets/I");
      outtree->Branch("MET",&MET,"MET/F");
      outtree->Branch("HT",&HT,"HT/F");
      outtree->Branch("mT",&mT,"mT/F");
    }

  virtual ~Analyzer() {
    fout->cd();
    outtree->Write();
    fout->Close();
  }
  

  const double xsec_;
  const double lumi_         = 1000.0; // in /pb
  const double metcut_       = 0.0;
  const double muonisocut_   = 0.15;
  const int    minNJets_     = 0;
  const int    minNBjets_    = 0;
  const int    nLeptons_     = 1;
  const TString sname_       = "test.root";
  const TString outputdir_   = "./plots/";  

  TFile *fout;
  TTree *outtree;

  float wgt;
  float MET;
  float HT;
  float mT;
  int lepton_pdgid;
  float lepton_pt;
  float lepton_eta;
  float lepton_phi;

  void runEvent();

  // override muon selection
  bool isSelMuon(const MuonF& muon) const {
    return (muon.pt() > config.minSelMuPt && fabs(muon.eta()) < config.maxSelMuETA && muon.pfdbetaiso()/muon.pt() < muonisocut_ && fabs(muon.d0()) < config.maxSelMuD0 && fabs(muon.dz()) < config.maxSelMuDz && (muon.*config.selectedMuon)());
  }
  
};

// Analyze event and fill tree
void Analyzer::runEvent()
{

  wgt = lumi_*xsec_/getEntries();

  if(nSelLeptons != nLeptons_) return; // check for exactly one lepton passing pog definitions

  // fill tree variables
  MET = met->pt();
  HT  = JetKinematics::ht(jets,30,2.4);

  LeptonF* lep = selectedLeptons.at(0);
  mT = JetKinematics::transverseMass(*lep, *met);

  lepton_pdgid = lep->pdgid();
  lepton_pt    = lep->pt();
  lepton_eta   = lep->eta();
  lepton_phi   = lep->phi();

  if(nJets < 4 || nBJets < 1 || MET < 80) return; // baseline selection

  outtree->Fill();

}

// Process file belonging to specified sample with a given cross section
void syncSingleLepton(TString sname = "T2tt_850_100",          // sample name
                      const int fileindex = -1,                // index of file (-1 means there is only 1 file for this sample)
                      const bool isMC = true,                  // data or MC
                      const TString fname = "/store/user/vdutta/13TeV/310315/merged/T2tt_850_100_ntuple.root", // path of file to be processed
                      const double xsec = 0.0189612,            // cross section to be used with this file
                      const TString outputdir = "run/trees",    // directory to which files with histograms will be written
                      const TString fileprefix = "root://eoscms//eos/cms") // prefix for file name, needed e.g. to access files with xrootd
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
  pars.defaultJetCollection = BaseTreeAnalyzer::AK4JETS;
  pars.cleanJetsvSelectedLeptons_ = true;
  pars.minSelMuPt = 20.0;
  pars.maxSelMuETA = 2.1;
  pars.minSelEPt = 20.0;
  pars.maxSelEETA = 2.1;
  pars.selectedElectron = &ElectronF::ismediumelectron;
  pars.selectedMuon = &MuonF::istightmuon;
  pars.minJetPt = 30.0;
  pars.maxJetEta = 2.4;
  pars.minBJetPt = 30.0;
  pars.maxBJetEta = 2.4;
  pars.cleanJetsMaxDR = 0.4;

  // Declare analyzer
  Analyzer a(fullname, "TestAnalyzer/Events", isMC, &pars, xsec, sname, outputdir);

  // Run! Argument is frequency of printout
  a.analyze(10000);

}
