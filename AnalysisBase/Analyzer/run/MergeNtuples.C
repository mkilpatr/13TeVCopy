#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TROOT.h>
#include <TTree.h>
#include <TFile.h>
#include <TFileMerger.h>
#include <TString.h>
#include <TBenchmark.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#endif

// Main macro function
//--------------------------------------------------------------------------------------------------
void MergeNtuples(const TString input, bool batchMode = true)
{
  gBenchmark->Start("MergeNtuples");

  TString outfilename;          // output of merged files
  vector<TString> infilenames;  // list input ntuple files to be stored

  //
  // parse input file
  //
  ifstream ifs;
  ifs.open(input.Data());
  assert(ifs.is_open());
  string line;
  getline(ifs,line);
  outfilename = line;
  if (batchMode) {
    outfilename.Remove(0,outfilename.Last('/')+1);
  }
  while(getline(ifs,line)) { infilenames.push_back(line); }
  ifs.close();

  TTree::SetMaxTreeSize(kMaxLong64);

  //
  // Combine TTrees from each file
  //
  TFileMerger m(kFALSE);
  m.OutputFile(outfilename,"RECREATE");
  for(UInt_t ifile=0; ifile<infilenames.size(); ifile++) {
    cout << "Adding " << infilenames[ifile] << endl;
    m.AddFile(infilenames[ifile]);
  }

  cout << "Merging..." << endl;
  m.Merge();

  cout << outfilename << " created!" << endl;

  gBenchmark->Show("MergeNtuples");
}
