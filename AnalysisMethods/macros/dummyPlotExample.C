//---------------------------------------------------------------------------------------------------------------------------------
//
// Macro to read an output tree produced by running the EventInfoFiller on MiniAOD and make a couple of simple plots as an example.
// To run from the command line: root -l -q -b dummyPlotExample.C+\(\"myfilename.root\"\)
//
//---------------------------------------------------------------------------------------------------------------------------------
#if !defined(__CINT__) || defined(__MAKECINT__)
#include <assert.h>
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "AnalysisMethods/PlotUtils/interface/StyleTools.hh"
#endif

void dummyPlotExample(TString fname = "$CMSSW_BASE/src/AnalysisBase/Analyzer/test/evttree.root") {

  // initialize
  TFile *infile = TFile::Open(fname); assert(infile);
  TTree *intree = (TTree*)infile->Get("TestAnalyzer/Events"); assert(intree);

  // book some histograms
  TH1F *hNPV = new TH1F("hNPV","", 60, 0, 60);
  TH1F *hMet = new TH1F("hMet","", 60, 0, 600);

  // will eventually use a helper class to do this kind of stuff
  int npv = 0; intree->SetBranchAddress("npv", &npv);
  float met = 0; intree->SetBranchAddress("met", &met);

  // loop over branches of event tree
  for(UInt_t ientry = 0; ientry < intree->GetEntries(); ientry++) {

    intree->GetEntry(ientry);

    // fill histograms with info for each event
    hNPV->Fill(npv);
    hMet->Fill(met);

  }

  // now make some plots
  StyleTools::SetStyle();
  TCanvas *c = StyleTools::MakeCanvas("c","",600,600);

  StyleTools::InitHist(hNPV, "Number of vertices", "Events");
  StyleTools::InitHist(hMet, "#slash{E}_{T} [GeV]", "Events");

  c->cd();

  hNPV->Draw("hist");
  c->SaveAs("npv.png");

  hMet->Draw("hist");
  c->SaveAs("met.png");

  // clean up
  delete c;
  delete intree;
  delete infile;

}
