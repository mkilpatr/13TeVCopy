//--------------------------------------------------------------------------------------------------
// 
// EventInfoReader
// 
// Class for reading event level information from TTree.
// 
// EventInfoReader.cc created on Mon Nov 3 12:56:14 CET 2014 
// 
//--------------------------------------------------------------------------------------------------

#include "AnalysisTools/TreeReader/interface/EventInfoReader.h"

using namespace ucsbsusy;

EventInfoReader::EventInfoReader()
{

  run = 0;
  lumi = 0;
  event = 0;
  nPV = 0;
  pvx = 0;
  pvy = 0;
  pvz = 0;
  met_pt = 0;
  met_phi = 0;
  metsumEt = 0;

}

void EventInfoReader::load(TTree *tree, int options, string branchName)
{

  tree->SetBranchAddress("run", &run);
  tree->SetBranchAddress("lumi", &lumi);
  tree->SetBranchAddress("event", &event);
  tree->SetBranchAddress("npv", &nPV);
  tree->SetBranchAddress("pv_x", &pvx);
  tree->SetBranchAddress("pv_y", &pvy);
  tree->SetBranchAddress("pv_z", &pvz);
  tree->SetBranchAddress("met", &met_pt);
  tree->SetBranchAddress("met_phi", &met_phi);
  tree->SetBranchAddress("met_sumEt", &metsumEt);

}

void EventInfoReader::refresh()
{
  met.setP4(CylLorentzVectorF(met_pt,0,met_phi,0));
}
