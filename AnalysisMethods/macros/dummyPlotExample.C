//---------------------------------------------------------------------------------------------------------------------------------
//
// Macro to read an output tree produced by running the EventInfoFiller on MiniAOD and make a couple of simple plots as an example.
// To run from the command line: root -l -q -b dummyPlotExample.C+\(\"myfilename.root\"\)
//
//---------------------------------------------------------------------------------------------------------------------------------
#if !defined(__CINT__) || defined(__MAKECINT__)
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "AnalysisMethods/PlotUtils/Style.hh"
#include "AnalysisTools/DataFormats/interface/EventInfo.h"
#endif

using namespace ucsbsusy;

void dummyPlotExample(TString fname = "$CMSSW_BASE/src/AnalysisBase/Analyzer/test/evttree.root") {

  // initialize
  TFile *infile = new TFile(fname); assert(infile);
  TTree *intree = (TTree*)infile->Get("Events"); assert(intree);

  // book some histograms
  TH1F *hNPV = new TH1F("hNPV","", 60, 0, 60);
  TH1F *hMet = new TH1F("hMet","", 40, 0, 200);

  // instance of class which contains event specific information
  EventInfo *evt = new EventInfo;

  // will eventually use a helper class to do this kind of stuff
  intree->SetBranchAddress("EventInfo", &evt); TBranch *evtBr = intree->GetBranch("EventInfo");

  // loop over branches of event tree
  for(UInt_t ientry = 0; ientry < intree->GetEntries(); ientry++) {

    evtBr->GetEntry(ientry);

    // fill histograms with info for each event
    hNPV->Fill(evt->nPV());
    hMet->Fill(evt->met());

  }

  // now make some plots
  SetStyle();
  TCanvas *c = MakeCanvas("c","",600,600);

  InitHist(hNPV, "Number of vertices", "Events");
  InitHist(hMet, "#slash{E}_{T} [GeV]", "Events");

  c->cd();

  hNPV->Draw("hist");
  c->SaveAs("npv.png");

  hMet->Draw("hist");
  c->SaveAs("met.png");

  // clean up
  delete c;
  delete evt;
  delete intree;
  delete infile;

}
