#if !defined(__CINT__) || defined(__MAKECINT__)
#include "TTree.h"
#include "TFile.h"
#endif

void skimFlavorTrees( const TString infilename    = "dyjetstoll_tree.root"
                    , const TString outfilename   = "dyjetstoll_quarks_tree.root"
                    , const TString selection     = "j0flavor==1"
                    , const TString filedirectory = "trees/"
                    , const TString treename      = "Events"
) {

  TFile* infile = new TFile(filedirectory+infilename);
  //assert(infile);
  TTree* intree = (TTree*)infile->Get(treename);
  //assert(intree);

  TFile* outfile = new TFile(filedirectory+outfilename, "RECREATE");
  outfile->cd();
  //std::cout << "copying tree (" << treename << ") with selection " << selection << std::endl;
  TTree* outtree = intree->CopyTree(selection);
  outtree->Write();
  outfile->Close();

  delete infile;
  delete outfile;

}

