#if !defined(__CINT__) || defined(__MAKECINT__)
#include <vector>
#include <assert.h>
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "AnalysisTools/Utilities/interface/Types.h"
#include "AnalysisTools/DataFormats/interface/Momentum.h"
#include "AnalysisTools/DataFormats/interface/Electron.h"
#include "AnalysisTools/DataFormats/interface/Muon.h"
#include "AnalysisTools/DataFormats/interface/Tau.h"
#include "AnalysisTools/TreeReader/interface/CollectionReader.h"
#include "AnalysisTools/TreeReader/interface/ElectronReader.h"
#include "AnalysisTools/TreeReader/interface/MuonReader.h"
#include "AnalysisTools/TreeReader/interface/TauReader.h"
#endif

using namespace ucsbsusy;

void readTreeDemo(string fname = "$CMSSW_BASE/src/AnalysisBase/Analyzer/test/evttree.root") {

  // initialize
  TFile *infile = new TFile(fname.c_str()); assert(infile);
  TTree *intree = (TTree*)infile->Get("TestAnalyzer/Events"); assert(intree);

  MomentumFCollection jets;
  ElectronFCollection electrons;
  MuonFCollection     muons;
  TauFCollection      taus;

  ucsbsusy::CollectionReader *jetreader = new ucsbsusy::CollectionReader(intree, ucsbsusy::CollectionReader::kAK4Jet); // dedicated jetreader not yet designed; for now, only read jet 4 vectors
  ucsbsusy::ElectronReader *elereader = new ucsbsusy::ElectronReader(intree);
  ucsbsusy::MuonReader *mureader = new ucsbsusy::MuonReader(intree);
  ucsbsusy::TauReader *taureader = new ucsbsusy::TauReader(intree);

  vector<ucsbsusy::CollectionReader*> readers;
  readers.push_back(jetreader);
  readers.push_back(elereader);
  readers.push_back(mureader);
  readers.push_back(taureader);

  // load branches
  for(unsigned int ir = 0; ir < readers.size(); ir++) {
    readers.at(ir)->initTree();
  }

  // event info reader not yet designed
  unsigned int event = 0;
  intree->SetBranchAddress("event", &event);

  // loop over events tree
  for(UInt_t ientry = 0; ientry < intree->GetEntries(); ientry++) {
    intree->GetEntry(ientry);

    // get this event for all the readers
    for(unsigned int ir = 0; ir < readers.size(); ir++) {
      readers.at(ir)->loadEvent(ientry);
    }

    jets.clear();
    electrons.clear();
    muons.clear();
    taus.clear();
   
    jets = jetreader->getMomenta();
    electrons = elereader->getElectrons();
    muons = mureader->getMuons();
    taus = taureader->getTaus();

    // Print a bunch of stuff

    cout << "For event " << event << endl;
    for(unsigned int i = 0; i < jets.size(); i++) {
      MomentumF jet = jets.at(i);
      cout << "Jet " << i << ": pt = " << jet.pt() << "; eta = " << jet.eta() << "; phi = " << jet.phi() <<  endl;
    }
    
    for(unsigned int i = 0; i < electrons.size(); i++) {
      ElectronF electron = electrons.at(i);
      cout << "Electron " << i << ": pt = " << electron.pt() << "; eta = " << electron.eta() << "; phi = " << electron.phi() <<  "; mvaID = " << electron.mvaidnontrig() << "; iso = " << electron.pfdbetaiso()/electron.pt() << endl;
    }

    for(unsigned int i = 0; i < muons.size(); i++) {
      MuonF muon = muons.at(i);
      cout << "Muon " << i << ": pt = " << muon.pt() << "; eta = " << muon.eta() << "; phi = " << muon.phi() <<  "; isPFMuon = " << muon.ispfmuon() << "; iso = " << muon.pfdbetaiso()/muon.pt() << endl;
    }

    for(unsigned int i = 0; i < taus.size(); i++) {
      TauF tau = taus.at(i);
      bool passantimutight = (tau.hpsid() & kTightMuMVA);
      bool passantielemedium = (tau.hpsid() & kMediumEleMVA);
      bool passmvaloose = (tau.hpsid() & kLooseIsoMVALT);
      cout << "Tau " << i << ": pt = " << tau.pt() << "; eta = " << tau.eta() << "; phi = " << tau.phi() <<  "; dxysig = " << tau.dxysig() << "; anti-muon tight rejection = " << passantimutight << "; anti-electron medium rejection = " << passantielemedium << "; pass mva iso (with lifetime) loose = " << passmvaloose << endl;
    }

  }

  delete infile;

}
