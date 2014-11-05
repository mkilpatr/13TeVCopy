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
#include "AnalysisTools/TreeReader/interface/TreeReader.h"

using namespace std;
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

void EventInfoReader::load(TreeReader *treeReader, int options, string branchName)
{
  loaded_ = true;
  treeReader->setBranchAddress("run", &run);
  treeReader->setBranchAddress("lumi", &lumi);
  treeReader->setBranchAddress("event", &event);
  treeReader->setBranchAddress("npv", &nPV);
  treeReader->setBranchAddress("pv_x", &pvx);
  treeReader->setBranchAddress("pv_y", &pvy);
  treeReader->setBranchAddress("pv_z", &pvz);
  treeReader->setBranchAddress("met", &met_pt);
  treeReader->setBranchAddress("met_phi", &met_phi);
  treeReader->setBranchAddress("met_sumEt", &metsumEt);
}

void EventInfoReader::refresh()
{
  met.setP4(CylLorentzVectorF(met_pt,0,met_phi,0));
}
