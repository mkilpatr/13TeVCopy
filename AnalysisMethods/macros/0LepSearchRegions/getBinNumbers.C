#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TString.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
using namespace std;
#endif

double getBinNumbers(TString filename,TString cuts) {

  TFile* f_tmp = TFile::Open(filename);
  TTree* t_tmp = (TTree*)f_tmp->Get("events");

  TH1::SetDefaultSumw2(kTRUE);
  TH1D *hTemp = new TH1D("hTemp","hTemp",1,0.,10000.);
  t_tmp->Project("hTemp","ht",cuts);
  double error =0.; double yield = hTemp->IntegralAndError(0,2,error);
  hTemp->Delete();

  return yield;
}
