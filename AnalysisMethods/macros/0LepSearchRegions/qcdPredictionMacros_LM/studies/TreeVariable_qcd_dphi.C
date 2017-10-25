#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/PlotUtils/interface/PlotROCCurves.h"
#include "AnalysisTools/Utilities/interface/JetFlavorInfo.h"
#include "AnalysisMethods/PlotUtils/interface/HistogramGetter.hh"
#include "AnalysisTools/TreeReader/interface/Defaults.h"
#include "../QCDPredictionHelper.h"
#include "TDirectory.h"
#include "TFile.h"
#endif

const TString originalTreeDir = "../../../run/plots_16_09_28_Smear";
const TString outputDir = "qcdROCCurves";
QCDSupport::SRegInfo srinfo;
vector<int> colors;

void TreeVariable_qcd_dphi() {
   //TFile *f = new TFile(QCDSupport::inputDir + "/Total_Bkg_tree.root","update");
   //TFile *f = new TFile(QCDSupport::inputDir + "/QCD_smeared_tree_skimmed.root","update");
   TFile *f = new TFile(QCDSupport::inputDir + "/T2tt_800_100_tree.root","update");
   TTree *T = (TTree*)f->Get("Events");
   float dphimhtj12, dphimhtj123;
   float dphimhtj12_500_Inf;
   float dphimhtj123_500_Inf;
   float dphimhtj12_qcd, dphimhtj123_qcd;
   float dphimhtj12_qcd_500_Inf;
   float dphimhtj123_qcd_500_Inf;
   TBranch *bdphimhtj12_qcd_500_Inf = T->Branch("dphimhtj12_qcd_500_Inf",&dphimhtj12_qcd_500_Inf,"dphimhtj12_qcd_500_Inf/F");
   TBranch *bdphimhtj123_qcd_500_Inf = T->Branch("dphimhtj123_qcd_500_Inf",&dphimhtj123_qcd_500_Inf,"dphimhtj123_qcd_500_Inf/F");
   TBranch *bdphimhtj12_500_Inf = T->Branch("dphimhtj12_500_Inf",&dphimhtj12_500_Inf,"dphimhtj12_500_Inf/F");
   TBranch *bdphimhtj123_500_Inf = T->Branch("dphimhtj123_500_Inf",&dphimhtj123_500_Inf,"dphimhtj123_500_Inf/F");
   T->SetBranchAddress("dphimhtj12",&dphimhtj12);
   T->SetBranchAddress("dphimhtj123",&dphimhtj123);
   Long64_t nentries = T->GetEntries();
   for (Long64_t i=0;i<nentries;i++) {
      T->GetEntry(i);
      if(i % 50000 == 0) std::cout << "Analyzing Event: " << i << std::endl;
      //dphimhtj12_qcd = dphimhtj12;
      //dphimhtj123_qcd = dphimhtj123;
      dphimhtj12_qcd_500_Inf = dphimhtj12;
      dphimhtj123_qcd_500_Inf = dphimhtj123;
      dphimhtj12_500_Inf = dphimhtj12;
      dphimhtj123_500_Inf = dphimhtj123;
      
      bdphimhtj12_qcd_500_Inf->Fill();
      bdphimhtj123_qcd_500_Inf->Fill();
      bdphimhtj12_500_Inf->Fill();
      bdphimhtj123_500_Inf->Fill();
   }
   T->Print();
   T->Write();
   delete f;
}
