#if !defined(__CINT__) || defined(__MAKECINT__)
#include <algorithm>
#include <TSystem.h>
#include <TFile.h>
#include <TH1F.h>
#include "AnalysisMethods/PlotUtils/interface/Sample.hh"
#include "AnalysisMethods/PlotUtils/interface/PlotTools.hh"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
#include "AnalysisTools/KinematicVariables/interface/JetKinematics.h"
#include "AnalysisBase/TreeAnalyzer/interface/BaseTreeAnalyzer.h"
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"
#include "AnalysisTools/Utilities/interface/JetFlavorInfo.h"
#endif

using namespace ucsbsusy;

class Analyzer : public BaseTreeAnalyzer {

public :

  Analyzer(TString fileName, TString treeName, bool isMCTree, ConfigPars * pars, double xSec, TString sname, TString outputdir) :
    BaseTreeAnalyzer(fileName, treeName, isMCTree, pars),xsec_(xSec), sname_(sname), outputdir_(outputdir) {

    // initialize tree
    gSystem->mkdir(outputdir,true);
    fout = new TFile (outputdir+"/"+sname+"_tree.root","RECREATE");
    fout->cd();
    outtree = new TTree("events","analysis tree");
    outtree->Branch("weight",&weight,"weight/F");
    outtree->Branch("GenMET",&GenMET,"GenMET/F");
    outtree->Branch("MET",&MET,"MET/F");
    outtree->Branch("NGenJets",&NGenJets,"NGenJets/I");
    outtree->Branch("NGenBJets",&NGenBJets,"NGenBJets/I");
    outtree->Branch("genBosonPT",&genBosonPT,"genBosonPT/F");
    outtree->Branch("genBosonEta",&genBosonEta,"genBosonEta/F");
    outtree->Branch("NVetoLeps",&NVetoLeps,"NVetoLeps/I");
    outtree->Branch("NVetoTaus",&NVetoTaus,"NVetoTaus/I");
    outtree->Branch("passLoosePhotons",&passLoosePhotons,"passLoosePhotons/I");
    outtree->Branch("NJets",&nJets,"NJets/I");
    outtree->Branch("NJ60",&nj60,"NJ60/I");
    outtree->Branch("NBJets",&nBJets,"NBJets/I");
    outtree->Branch("dPhiMET12",&dPhiMET12,"dPhiMET12/F");
    outtree->Branch("dPhiMET3",&dPhiMET3,"dPhiMET3/F");
    outtree->Branch("mtB1MET",&mtB1MET,"mtB1MET/F");
    outtree->Branch("mtB2MET",&mtB2MET,"mtB2MET/F");
    outtree->Branch("NCTTstd",&NCTTstd,"NCTTstd/I");

  }

  virtual ~Analyzer() {
    fout->cd();
    outtree->Write();
    fout->Close();
  }

  virtual void processVariables(){
    BaseTreeAnalyzer::processVariables();

    // get boson
    int PID = (process == defaults::SINGLE_G ? ParticleInfo::p_gamma : ParticleInfo::p_Z0);
    for (auto &p : genParts){
      if (p->pdgId() == PID){
        boson = p;
        break;
      }
    }

    // add gen photon to gen met
    if (process == defaults::SINGLE_G)
      genmet->p4() += boson->p4();

    // clean jets
    nj60 = 0;
    jets.clear(); bJets.clear(); nonBJets.clear();
    for (auto &j : defaultJets->recoJets){
      if (!isGoodJet(j)) continue;
      if (PhysicsUtilities::deltaR2(j, *boson) < 0.16) continue;

      jets.push_back(&j);

      if (j.pt()>60) ++nj60;

      if(isMediumBJet(j)) bJets.push_back(&j);
      else nonBJets.push_back(&j);
    }
    nJets    = jets.size();
    nBJets   = bJets.size();

    // clean genjets
    NGenJets = 0; NGenBJets = 0;
    for (auto &j : defaultJets->genJets){
      if (!isGoodJet(j)) continue;
      if (PhysicsUtilities::deltaR2(j, *boson) < 0.16) continue;

      ++NGenJets;
      if (fabs(j.flavor()) == JetFlavorInfo::b_jet)
        ++NGenBJets;
    }

    // clean vetoedLeptons
    NVetoLeps = 0;
    for (auto &l : secondaryLeptons){
      if (PhysicsUtilities::deltaR2(*l, *boson) < 0.16) continue;
      ++NVetoLeps;
    }
    // clean vetoedTaus
    NVetoTaus = 0;
    for (auto &ta : vetoedTaus){
      if (PhysicsUtilities::deltaR2(*ta, *boson) < 0.16) continue;
      ++NVetoTaus;
    }

    //count NCTTstd
    NCTTstd = 0;
    for (auto &t : cttTops){
      if ( (t->fJMass()>140.) && (t->fJMass()<250.) && (t->minMass()>50.) && (t->nSubJets()>=3) )
        ++NCTTstd;
    }

  }

  void runEvent();


private:

  GenParticleF *boson = nullptr;

  const double xsec_;
  const double lumi_         = 1000.0; // in /pb
  const TString sname_       = "testHQ.root";
  const TString outputdir_   = "./plots/";  

  const double  metcut_    = 200.0 ;
  const int     minNJets_  =   5   ;
  const int     minNBjets_ =   1   ;
  const int     maxNTaus_  =   0   ;
  const int     maxNLeps_  =   0   ;

  int nj60 = 0;
  int NVetoLeps = 0;
  int NVetoTaus = 0;
  int passLoosePhotons = 0;
  int NCTTstd = 0;
  float GenMET = 0;
  float MET = 0;
  int NGenJets = -1;
  int NGenBJets = -1;
  float genBosonPT = 0;
  float genBosonEta = 0;

  float dPhiMET12 = -9;
  float dPhiMET3 = -9;
  float mtB1MET = -9;
  float mtB2MET = -9;


  TFile *fout;
  TTree *outtree;


  //  void out(TString outputName, TString outputPath);

  map<TString,TH1F*> plots;

};

// Analyze event and fill plots
void Analyzer::runEvent()
{

  //  // preselection
  //  if(NVetoLeps > 0)  return;
  //  if(met->pt() < metcut_) return;
  //  if(nJets<minNJets_)     return;
  //  if(nBJets < minNBjets_) return;
  //  if(NVetoTaus>0)       return;

  // constrain the Z/photons within the same eta acceptance
//  if (fabs(boson->eta())>2.4) return;

  // add photon to met
  passLoosePhotons = 1;
  if (process == defaults::SINGLE_G){
    if (!selectedPhotons.empty())
      met->p4() += selectedPhotons.front()->p4();
    else passLoosePhotons = 0;
  }

  vector<RecoJetF*> jetsCSV(jets);
  std::sort(jetsCSV.begin(), jetsCSV.end(), [](const RecoJetF *a, const RecoJetF *b) {
    return a->csv() > b->csv();
  });


  // fill tree variables
  GenMET        = genmet->pt();
  MET 				  = met->pt();
  genBosonPT    = boson->pt();
  genBosonEta   = boson->eta();
  dPhiMET12 = JetKinematics::absDPhiMETJ12(*met,jets);
  dPhiMET3  = JetKinematics::absDPhiMETJ3 (*met,jets);
  if(jetsCSV.size()>0) mtB1MET = JetKinematics::transverseMass(jetsCSV.at(0)->p4(),met->p4());
  if(jetsCSV.size()>1) mtB2MET = JetKinematics::transverseMass(jetsCSV.at(1)->p4(),met->p4());


  // fill the tree!
  outtree->Fill();
}

// Process file belonging to specified sample with a given cross section
void processPhotonZ(TString sname = "test",               // sample name
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

  // Declare analyzer
  Analyzer a(fullname, "Events", isMC, &pars, xsec, sname, outputdir);

  // Run! Argument is frequency of printout
  a.analyze(100000);

}
