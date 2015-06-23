/*
 * PhotonReweighting.C
 *
 *  Created on: May 31, 2015
 *      Author: hqu
 */
#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/PlotUtils/interface/PlotStuff.h"
#include <fstream>
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

  if (origTree->GetBranch("ScaleFactor") || targetTree->GetBranch("ScaleFactor")){
    cerr << "ScaleFactor already exists! Exit..." << endl;
    delete inOrig;
    delete inTarget;
    return;
  }

  // Create hists
  const double xmin = 100, xmid = 300, xmax = 1000;
  const int nbinL = (int)(xmid-xmin)/20, nbinH = (int)(xmax-xmid)/50;
  const int njbin = 2, nbbin = 2;
  const double njmin = 3.5, njmax = 7.5, nbmin = -0.5, nbmax = 1.5;
  TH1D *h1origL   = new TH1D("h1origL",   "h1origL",   nbinL, xmin, xmid);
  TH1D *h1targetL = new TH1D("h1targetL", "h1targetL", nbinL, xmin, xmid);
  TH1D *h1origH   = new TH1D("h1origH",   "h1origH",   nbinH, xmid, xmax);
  TH1D *h1targetH = new TH1D("h1targetH", "h1targetH", nbinH, xmid, xmax);
  TH2D *h2orig    = new TH2D("h2orig",    "h2orig",    njbin, njmin, njmax, nbbin, nbmin, nbmax);
  TH2D *h2target  = new TH2D("h2target",  "h2target",  njbin, njmin, njmax, nbbin, nbmin, nbmax);
  h1origL->Sumw2();
  h1targetL->Sumw2();
  h1origH->Sumw2();
  h1targetH->Sumw2();
  h2orig->Sumw2();
  h2target->Sumw2();

  // constrain photons and Z to be in the same eta acceptance
  const float maxEta = 2.4;

  float weight;
  float genBosonPT;
  float genBosonEta;
  int NGenJets;
  int NGenBJets;

  float ScaleFactor;
  TBranch *brNewSF;

  targetTree->SetBranchAddress("weight", &weight);
  targetTree->SetBranchAddress("genBosonPT", &genBosonPT);
  targetTree->SetBranchAddress("genBosonEta", &genBosonEta);
  targetTree->SetBranchAddress("NGenJets", &NGenJets);
  targetTree->SetBranchAddress("NGenBJets", &NGenBJets);
  brNewSF = targetTree->Branch("ScaleFactor",&ScaleFactor,"ScaleFactor/F");
  for (Long64_t i = 0; i < targetTree->GetEntries(); ++i){
    targetTree->GetEntry(i);
    if (fabs(genBosonEta) < maxEta){
      if (genBosonPT < xmid)
        h1targetL->Fill(genBosonPT, weight);
      else
        h1targetH->Fill(genBosonPT, weight);
      h2target->Fill(NGenJets, NGenBJets, weight);
    }
    ScaleFactor = weight;
    brNewSF->Fill();
  }
  inTarget->cd();
  targetTree->Write("", TObject::kOverwrite);

  origTree->SetBranchAddress("weight", &weight);
  origTree->SetBranchAddress("genBosonPT", &genBosonPT);
  origTree->SetBranchAddress("genBosonEta", &genBosonEta);
  origTree->SetBranchAddress("NGenJets", &NGenJets);
  origTree->SetBranchAddress("NGenBJets", &NGenBJets);
  for (Long64_t i = 0; i < origTree->GetEntries(); ++i){
    origTree->GetEntry(i);
    if (fabs(genBosonEta) < maxEta){
      if (genBosonPT < xmid)
        h1origL->Fill(genBosonPT, weight);
      else
        h1origH->Fill(genBosonPT, weight);
    }
  }

  // Ratio
  h1targetL->Divide(h1origL);
  h1targetH->Divide(h1origH);

  // correct residual NGenJets difference
  for (Long64_t i = 0; i < origTree->GetEntries(); ++i){
    origTree->GetEntry(i);
    if (fabs(genBosonEta) < maxEta){
      TH1D *h1 = genBosonPT<300 ? h1targetL : h1targetH;
      h2orig->Fill(NGenJets, NGenBJets, weight * h1->GetBinContent(h1->FindBin(genBosonPT)));
    }
  }
  h2target->Divide(h2orig);


  // output the hists
  TCanvas *canvas = StyleTools::MakeCanvas("c","c");
  StyleTools::SetStyle();
  h1targetL->Draw("histE");
  canvas->Print(inputdir+"/_PtNbLow.pdf");
  h1targetH->Draw("histE");
  canvas->Print(inputdir+"/_PtNbHigh.pdf");
  h2target->Draw("legoE");
  canvas->Print(inputdir+"/_Nj.pdf");
  delete canvas;

  // add new SF
  cout << "Setting new ScaleFactors" << endl;
  brNewSF = origTree->Branch("ScaleFactor",&ScaleFactor,"ScaleFactor/F");
  for (Long64_t i = 0; i < origTree->GetEntries(); ++i){
    origTree->GetEntry(i);
    TH1D *h1 = genBosonPT<300 ? h1targetL : h1targetH;
    ScaleFactor = weight * h1->GetBinContent(h1->FindBin(genBosonPT)) * h2target->GetBinContent(h2target->FindBin(NGenJets, NGenBJets));
    brNewSF->Fill();
  }
  inOrig->cd();
  origTree->Write("", TObject::kOverwrite);

  delete inOrig;
  delete inTarget;

}
