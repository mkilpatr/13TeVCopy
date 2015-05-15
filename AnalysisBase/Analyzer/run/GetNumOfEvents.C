#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TROOT.h>
#include <TTree.h>
#include <TFile.h>
#include <TString.h>
#include <TBenchmark.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#endif

void getListOfMergedFiles(const TString input);
int calcTotalNumOfEvents(std::vector<TString> filenames);

void GetNumOfEvents(const TString input) {

  getListOfMergedFiles(input);

}


void getListOfMergedFiles(const TString input) {
  
  vector<TString> filenames;

  ifstream readFile;
  readFile.open(input);
  string line;
  getline(readFile,line);
  if (readFile.is_open()) { 
    while(getline(readFile,line)) { filenames.push_back("root://eoscms/"+line); }
  }
  readFile.close();

  cout << calcTotalNumOfEvents(filenames) << "\n";

}


int calcTotalNumOfEvents(std::vector<TString> filenames) {

  int nEntries = 0;

  for (unsigned int i=0; i<filenames.size(); ++i) {

    TFile *f = TFile::Open(filenames[i],"READONLY");
    TTree *t = (TTree*)f->Get("TestAnalyzer/Events");
    
    nEntries += (Int_t)t->GetEntries();
    t->Delete();
    f->Delete();

  }

  return nEntries;
}

