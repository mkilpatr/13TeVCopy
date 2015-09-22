#include "TTree.h"
#include "TFile.h"

void copyTrees(const TString infilename  = "trees/ttbar.root",
               const TString treename    = "Events",
               const TString outfilename = "trees/realtaus_tt1tau.root",
               const TString selection   = "nGenLeptons==0 && nGenTaus==nGenHadTaus && nGenHadTaus==1 && gentaumatch==1") {

  TFile* infile = new TFile(infilename);
  assert(infile);
  TTree* intree = (TTree*)infile->Get(treename);
  assert(intree);

  TFile* outfile = new TFile(outfilename, "RECREATE");
  outfile->cd();
  std::cout << "copying tree (" << treename << ") with selection " << selection << std::endl;
  TTree* outtree = intree->CopyTree(selection);
  outtree->Write();
  outfile->Close();

  delete infile;
  delete outfile;

}
