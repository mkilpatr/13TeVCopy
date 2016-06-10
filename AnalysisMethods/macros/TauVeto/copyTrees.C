#include "TTree.h"
#include "TFile.h"
#include <iostream>

//void copyTrees(const TString infilename  = "tauMVATrees/ttbar.root",
//               const TString treename    = "Candidates",
//               const TString outfilename = "tauMVATrees/realtaus_tt1tau.root",
//               const TString selection   = "nGenLeptons==0 && nGenTaus==nGenHadTaus && nGenHadTaus==1 && gentaumatch==1 && pt>10 && ptmatch > 6.1") {
//
//  TFile* infile = new TFile(infilename);
//  assert(infile);
//  TTree* intree = (TTree*)infile->Get(treename);
//  assert(intree);
//
//  TFile* outfile = new TFile(outfilename, "RECREATE");
//  outfile->cd();
//  std::cout << "copying tree (" << treename << ") with selection " << selection << std::endl;
//  TTree* outtree = intree->CopyTree(selection);
//  outtree->Write();
//  outfile->Close();
//
//  delete infile;
//  delete outfile;
//
//}


void copyTrees(const TString infilename  = "tauMVATrees/T2tt_500_325_addtaumva.root",
               const TString treename    = "Candidates",
               const TString outfilename = "tauMVATrees/faketaus_T2tt_500_325_addtaumva.root",
               const TString selection   = "gentaumatch==0 && nGenLeptons==0 && nGenTaus==0") {

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

