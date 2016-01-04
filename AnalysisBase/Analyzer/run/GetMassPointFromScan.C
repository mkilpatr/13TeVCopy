#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisTools/Utilities/interface/Types.h"
#include "TROOT.h"
#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#endif

using namespace ucsbsusy;

void GetMassPointFromScan(const TString infilename,
                          const TString outputname,
                          const int mstop,
                          const int mlsp)
{

  vector<TString> infilenames;

  ifstream ifs;
  ifs.open(infilename.Data());
  assert(ifs.is_open());
  string line;
  while(getline(ifs,line)) { infilenames.push_back(TString(line)); }
  ifs.close();

  TTree::SetMaxTreeSize(kMaxLong64);

  TChain* chain = new TChain("Events");

  for(const auto fname : infilenames) {
    cout << "Adding " << fname.Data() << endl;
    chain->Add(fname);
  }

  TString cutstr = TString::Format("massparams[0] == %d && massparams[1] == %d",mstop,mlsp);
  int nselevents = chain->GetEntries(cutstr);

  if(nselevents == 0) {
    cout << "No events corresponding to mStop=" << mstop << ", mLSP=" << mlsp << endl;
    return;
  }

  chain->SetBranchStatus("*",1);
  chain->SetBranchStatus("wgtXSec",0);

  float xsec = 0;
  vector<size16> * massparams = new vector<size16>;
  chain->SetBranchAddress("xsection", &xsec);
  chain->SetBranchAddress("massparams", &massparams);

  TFile* outfile = new TFile(outputname,"RECREATE");
  TTree* outtree = chain->CloneTree(0);

  float weight = 0;
  outtree->Branch("wgtXSec",&weight,"F");

  int nevents = chain->GetEntries();

  cout << nevents << " events to process, " << nselevents << " selected" << endl;

  for(int ievent = 0; ievent < nevents; ++ievent) {
    chain->GetEntry(ievent);
    if(ievent%100000 == 0) cout << "Processing event " << ievent << endl;
    if(convertTo<int>(massparams->at(0),"Events.massparams[0]") != mstop || convertTo<int>(massparams->at(1),"Events.massparams[1]") != mlsp) continue;
    weight = 1000.0*xsec/nselevents;
    outtree->Fill();
  }

  outfile->cd();
  outtree->Write();
  outfile->Close();

  delete outfile;

}
