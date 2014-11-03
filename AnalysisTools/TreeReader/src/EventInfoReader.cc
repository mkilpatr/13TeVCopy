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

  evtInfo.run = 0;
  evtInfo.lumi = 0;
  evtInfo.event = 0;
  evtInfo.nPV = 0;
  evtInfo.pvx = 0;
  evtInfo.pvy = 0;
  evtInfo.pvz = 0;
  evtInfo.met = 0;
  evtInfo.metphi = 0;
  evtInfo.metsumEt = 0;

}

void EventInfoReader::load(TTree *tree, int options, string branchName)
{

  tree->SetBranchAddress("run", &evtInfo.run);
  tree->SetBranchAddress("lumi", &evtInfo.lumi);
  tree->SetBranchAddress("event", &evtInfo.event);
  tree->SetBranchAddress("npv", &evtInfo.nPV);
  tree->SetBranchAddress("pv_x", &evtInfo.pvx);
  tree->SetBranchAddress("pv_y", &evtInfo.pvy);
  tree->SetBranchAddress("pv_z", &evtInfo.pvz);
  tree->SetBranchAddress("met", &evtInfo.met);
  tree->SetBranchAddress("met_phi", &evtInfo.metphi);
  tree->SetBranchAddress("met_sumEt", &evtInfo.metsumEt);

}

void EventInfoReader::refresh() {}
