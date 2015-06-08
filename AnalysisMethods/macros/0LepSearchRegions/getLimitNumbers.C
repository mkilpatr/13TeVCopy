#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TString.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
using namespace std;
#endif

void getLimitNumbers(TString filename) {

  TFile* f_tmp = TFile::Open(filename);
  TTree* t_tmp = (TTree*)f_tmp->Get("limit");

  TH1D *hTemp = new TH1D("hTemp","hTemp",1,0.,10000.);
  t_tmp->Project("hTemp","limit");
  double mean  = hTemp->GetMean();
  double error = hTemp->GetMeanError();
  cout << "mean=" << mean << "\terror=" << error << endl;
  hTemp->Delete();

  return;
}
