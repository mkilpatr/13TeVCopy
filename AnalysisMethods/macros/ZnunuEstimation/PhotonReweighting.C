/*
 * PhotonReweighting.C
 *
 *  Created on: May 31, 2015
 *      Author: hqu
 */
#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/PlotUtils/interface/PlotStuff.h"
#include "TH3.h"
#endif

void PhotonReweighting(const TString inputdir = "../run/plots",
    const TString inOrigName = "gjets_tree.root",
    const TString inTargetName = "znunu_tree.root"){

  // Get the PT ratio plot
  TFile *inOrig = new TFile(inputdir+"/"+inOrigName, "UPDATE");
  TFile *inTarget = new TFile(inputdir+"/"+inTargetName, "UPDATE");

  TTree *origTree = (TTree *)inOrig->Get("events");
  TTree *targetTree = (TTree *)inTarget->Get("events");
  assert(origTree);
  assert(targetTree);

  // Create 3D hist
  const int nbin = 18;
  const double xmin = 100, xmax = 1000;
  const int njbin = 7, nbbin = 2;
  const double njmin = 0.5, njmax = njmin+njbin, nbmin = 0.5, nbmax = nbmin+nbbin;
  TH3D *h3corr = new TH3D("h3corr", "h3corr",       nbin, xmin, xmax, njbin, njmin, njmax, nbbin, nbmin, nbmax);
  TH3D *h3target = new TH3D("h3target", "h3target", nbin, xmin, xmax, njbin, njmin, njmax, nbbin, nbmin, nbmax);
  h3corr->Sumw2();
  h3target->Sumw2();

  float ScaleFactor;
  float genBosonPT;
  int NGenJets;
  int NGenBJets;

  float NewSF;
  TBranch *brNewSF;

  targetTree->SetBranchAddress("ScaleFactor", &ScaleFactor);
  targetTree->SetBranchAddress("genBosonPT", &genBosonPT);
  targetTree->SetBranchAddress("NGenJets", &NGenJets);
  targetTree->SetBranchAddress("NGenBJets", &NGenBJets);
  brNewSF = targetTree->Branch("NewSF",&NewSF,"NewSF/F");
  for (Long64_t i = 0; i < targetTree->GetEntries(); ++i){
    targetTree->GetEntry(i);
    h3target->Fill(genBosonPT, NGenJets, NGenBJets, ScaleFactor);
    NewSF = ScaleFactor;
    brNewSF->Fill();
  }
  inTarget->cd();
  targetTree->Write("", TObject::kOverwrite);

  origTree->SetBranchAddress("ScaleFactor", &ScaleFactor);
  origTree->SetBranchAddress("genBosonPT", &genBosonPT);
  origTree->SetBranchAddress("NGenJets", &NGenJets);
  origTree->SetBranchAddress("NGenBJets", &NGenBJets);
  for (Long64_t i = 0; i < origTree->GetEntries(); ++i){
    origTree->GetEntry(i);
    h3corr->Fill(genBosonPT, NGenJets, NGenBJets, ScaleFactor);
  }

  // Ratio
  h3target->Divide(h3corr);
  cout << "Setting new ScaleFactors" << endl;

  // add new SF
  brNewSF = origTree->Branch("NewSF",&NewSF,"NewSF/F");
  for (Long64_t i = 0; i < origTree->GetEntries(); ++i){
    origTree->GetEntry(i);
    NewSF = ScaleFactor * h3target->GetBinContent(h3target->FindBin(genBosonPT, NGenJets, NGenBJets));
    brNewSF->Fill();
  }
  inOrig->cd();
  origTree->Write("", TObject::kOverwrite);

  delete inOrig;
  delete inTarget;

}
