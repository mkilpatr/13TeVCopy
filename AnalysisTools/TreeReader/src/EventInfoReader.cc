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
  rho = 0;
  pvx = 0;
  pvy = 0;
  pvz = 0;
  met_pt = 0;
  met_phi = 0;
  metsumEt = 0;
  genmet_pt = 0;
  genmet_phi = 0;
  goodvertex = false;
  genweight = 1;
  genqscale = 0;
  xsecweight = 1;
  genevtweight = 1;
  evtweight = 1;
  proc = 0;
  process = defaults::NUMPROCESSES;
  trigbitflags = new vector<unsigned long>;
  trigbitpass = new vector<bool>;
  trigbitprescale = new vector<unsigned int>;
  triggerflag = 0;

}

void EventInfoReader::load(TreeReader *treeReader, int options, string branchName)
{
  clog << "Loading (" << branchName << ") event info with: ";

  loaded_ = true;
  treeReader->setBranchAddress(branchName,"run", &run);
  treeReader->setBranchAddress(branchName,"lumi", &lumi);
  treeReader->setBranchAddress(branchName,"event", &event);
  treeReader->setBranchAddress(branchName,"npv", &nPV);
  treeReader->setBranchAddress(branchName,"rho", &rho);
  treeReader->setBranchAddress(branchName,"pv_x", &pvx);
  treeReader->setBranchAddress(branchName,"pv_y", &pvy);
  treeReader->setBranchAddress(branchName,"pv_z", &pvz);
  treeReader->setBranchAddress(branchName,"met_pt", &met_pt);
  treeReader->setBranchAddress(branchName,"met_phi", &met_phi);
  treeReader->setBranchAddress(branchName,"met_sumEt", &metsumEt);
  treeReader->setBranchAddress(branchName,"genmet_pt", &genmet_pt);
  treeReader->setBranchAddress(branchName,"genmet_phi", &genmet_phi);
  treeReader->setBranchAddress(branchName,"goodvertex", &goodvertex);
  treeReader->setBranchAddress(branchName,"genweight", &genweight);
  treeReader->setBranchAddress(branchName,"genqscale", &genqscale);
  treeReader->setBranchAddress(defaults::BRANCH_TRIGGERS,"bit_flag", &trigbitflags);
  treeReader->setBranchAddress(defaults::BRANCH_TRIGGERS,"bit_pass", &trigbitpass);
  treeReader->setBranchAddress(defaults::BRANCH_TRIGGERS,"bit_prescale", &trigbitprescale);
  treeReader->setBranchAddress(branchName,"process", &proc);
  treeReader->setBranchAddress(branchName,"wgtXSec", &xsecweight);
  treeReader->setBranchAddress(branchName,"evtWgtGen", &genevtweight);
  clog << endl;
}

void EventInfoReader::refresh()
{
  triggerflag = 0;
  met.setP4(CylLorentzVectorF(met_pt,0,met_phi,0));
  genmet.setP4(CylLorentzVectorF(genmet_pt,0,genmet_phi,0));
  process = static_cast<defaults::Process>(proc);
  evtweight = xsecweight * genevtweight;

  for (unsigned int i = 0; i < trigbitflags->size(); ++i) {
    if(trigbitpass->at(i)) triggerflag |= trigbitflags->at(i);
  }

}
